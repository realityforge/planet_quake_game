// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "g_local.h"

/*

  Items are any object that a player can touch to gain some effect.

  Pickup will return the number of seconds until they should respawn.

  all items should pop when dropped in lava or slime

  Respawnable items don't actually go away when picked up, they are
  just made invisible and untouchable.  This allows them to ride
  movers and respawn apropriately.
*/


#define	RESPAWN_ARMOR		25
#define	RESPAWN_HEALTH		35
#define	RESPAWN_AMMO		40
#define	RESPAWN_HOLDABLE	60
#define	RESPAWN_MEGAHEALTH	35//120
#define	RESPAWN_POWERUP		120


//======================================================================

int Pickup_Powerup( gentity_t *ent, gentity_t *other ) {
	int			quantity;
	int			i;
	gclient_t	*client;

	if ( !other->client->ps.powerups[ent->item->giTag] ) {
		// round timing to seconds to make multiple powerup timers
		// count in sync
		other->client->ps.powerups[ent->item->giTag] = 
			level.time - ( level.time % 1000 );
	}

	if ( ent->count ) {
		quantity = ent->count;
	} else {
		quantity = ent->item->quantity;
	}

	other->client->ps.powerups[ent->item->giTag] += quantity * 1000;

	// give any nearby players a "denied" anti-reward
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		vec3_t		delta;
		float		len;
		vec3_t		forward;
		trace_t		tr;

		client = &level.clients[i];
		if ( client == other->client ) {
			continue;
		}
		if ( client->pers.connected == CON_DISCONNECTED ) {
			continue;
		}
		if ( client->ps.stats[STAT_HEALTH] <= 0 ) {
			continue;
		}

    // if same team in team game, no sound
    // cannot use OnSameTeam as it expects to g_entities, not clients
  	if ( g_gametype.integer < GT_DM && other->client->sess.sessionTeam == client->sess.sessionTeam  ) {
      continue;
    }

		// if too far away, no sound
		VectorSubtract( ent->s.pos.trBase, client->ps.origin, delta );
		len = VectorNormalize( delta );
		if ( len > 192 ) {
			continue;
		}

		// if not facing, no sound
		AngleVectors( client->ps.viewangles, forward, NULL, NULL );
		if ( DotProduct( delta, forward ) < 0.4 ) {
			continue;
		}

		// if not line of sight, no sound
		trap_Trace( &tr, client->ps.origin, NULL, NULL, ent->s.pos.trBase, ENTITYNUM_NONE, CONTENTS_SOLID );
		if ( tr.fraction != 1.0 ) {
			continue;
		}

		// anti-reward
		client->ps.persistant[PERS_PLAYEREVENTS] ^= PLAYEREVENT_DENIEDREWARD;
	}
	return RESPAWN_POWERUP;
}

//======================================================================

int Pickup_Holdable( gentity_t *ent, gentity_t *other ) {

	other->client->ps.stats[STAT_HOLDABLE_ITEM] = ent->item - bg_itemlist;

	if( ent->item->giTag == HI_KAMIKAZE ) {
		other->client->ps.eFlags |= EF_KAMIKAZE;
	}

	return RESPAWN_HOLDABLE;
}


//======================================================================

void Add_Ammo (gentity_t *ent, int wpcat, int weapon, int count)
{
	ent->client->ps.bagammo[wpcat] += count; // add ammo
	if ( ent->client->ps.bagammo[wpcat] > weLi[weapon].ammo_max ) { // check for max ammo
		ent->client->ps.bagammo[wpcat] = weLi[weapon].ammo_max;
	}
}

int Pickup_Ammo (gentity_t *ent, gentity_t *other)
{
	int		quantity;

	if ( ent->count ) {
		quantity = ent->count;
	} else {
		quantity = ent->item->quantity;
	}

	Add_Ammo (other, weLi[ent->item->giTag].category, ent->item->giTag, quantity);

	return RESPAWN_AMMO;
}

//======================================================================


int Pickup_Weapon (gentity_t *ent, gentity_t *other) {
	// add the weapon
	other->client->ps.weapons[weLi[ent->item->giTag].category] = ent->item->giTag;

	other->client->ps.bagammo[weLi[ent->item->giTag].category]  = ent->count;  // set ammo in bag
	other->client->ps.clipammo[weLi[ent->item->giTag].category] = ent->count2; // set ammo in clip

	return g_weaponRespawn.integer;
}


//======================================================================

int Pickup_Health (gentity_t *ent, gentity_t *other) {
	int			max;
	int			quantity;

	// small and mega healths will go over the max
	if ( ent->item->quantity != 5 && ent->item->quantity != 100 ) {
		max = other->client->ps.stats[STAT_MAX_HEALTH];
	} else {
		max = other->client->ps.stats[STAT_MAX_HEALTH] * 2;
	}

	if ( ent->count ) {
		quantity = ent->count;
	} else {
		quantity = ent->item->quantity;
	}

	other->health += quantity;

	if (other->health > max ) {
		other->health = max;
	}
	other->client->ps.stats[STAT_HEALTH] = other->health;

	if ( ent->item->quantity == 100 ) {		// mega health respawns slow
		return RESPAWN_MEGAHEALTH;
	}

	return RESPAWN_HEALTH;
}

//======================================================================

/*int Pickup_Armor( gentity_t *ent, gentity_t *other ) {
	other->client->ps.stats[STAT_ARMOR] += ent->item->quantity;
	if ( other->client->ps.stats[STAT_ARMOR] > other->client->ps.stats[STAT_MAX_HEALTH] * 2 ) {
		other->client->ps.stats[STAT_ARMOR] = other->client->ps.stats[STAT_MAX_HEALTH] * 2;
	}

	return RESPAWN_ARMOR;
}*/

//======================================================================

/*
===============
RespawnItem
===============
*/
void RespawnItem( gentity_t *ent ) {
	// randomly select from teamed entities
	if (ent->team) {
		gentity_t	*master;
		int	count;
		int choice;

		if ( !ent->teammaster ) {
			G_Error( "RespawnItem: bad teammaster");
		}
		master = ent->teammaster;

		for (count = 0, ent = master; ent; ent = ent->teamchain, count++)
			;

		choice = rand() % count;

		for (count = 0, ent = master; count < choice; ent = ent->teamchain, count++)
			;
	}

	ent->r.contents = CONTENTS_TRIGGER;
	ent->s.eFlags &= ~EF_NODRAW;
	ent->r.svFlags &= ~SVF_NOCLIENT;
	trap_LinkEntity (ent);

	if ( ent->item->giType == IT_POWERUP ) {
		// play powerup spawn sound to all clients
		gentity_t	*te;

		// if the powerup respawn sound should Not be global
		if (ent->speed) {
			te = G_TempEntity( ent->s.pos.trBase, EV_GENERAL_SOUND );
		}
		else {
			te = G_TempEntity( ent->s.pos.trBase, EV_GLOBAL_SOUND );
		}
		te->s.eventParm = G_SoundIndex( "sound/items/poweruprespawn.wav" );
		te->r.svFlags |= SVF_BROADCAST;
	}

	if ( ent->item->giType == IT_HOLDABLE && ent->item->giTag == HI_KAMIKAZE ) {
		// play powerup spawn sound to all clients
		gentity_t	*te;

		// if the powerup respawn sound should Not be global
		if (ent->speed) {
			te = G_TempEntity( ent->s.pos.trBase, EV_GENERAL_SOUND );
		}
		else {
			te = G_TempEntity( ent->s.pos.trBase, EV_GLOBAL_SOUND );
		}
		te->s.eventParm = G_SoundIndex( "sound/items/kamikazerespawn.wav" );
		te->r.svFlags |= SVF_BROADCAST;
	}

	// play the normal respawn sound only to nearby clients
	G_AddEvent( ent, EV_ITEM_RESPAWN, 0 );

	ent->nextthink = 0;
}


/*
===============
Touch_Item
===============
*/
void Touch_Item (gentity_t *ent, gentity_t *other, trace_t *trace) {
	int			respawn;
	qboolean	predict;

	if (!other->client)
		return;
	if (other->health < 1)
		return;		// dead people can't pickup
	
	// test for an objective item
	if ( ( other->client->sess.sessionTeam == TEAM_RED  && (ent->parent->spawnflags & 1) ||
		   other->client->sess.sessionTeam == TEAM_BLUE && (ent->parent->spawnflags & 2) ) &&
		 !other->client->ps.powerups[PW_ITEM] && (ent->s.eFlags & EF_OBJECTIVE) && 
		 level.time > ent->s.time + ITEM_TAKETIME ) {

		other->client->ps.powerups[PW_ITEM] = ent->s.modelindex2;
		other->client->itemParent = ent->parent;

		// item is in the player's possession
		ent->parent->count = 0;
		G_AddEvent( other, EV_ITEM_OBJECTIVE, 0 );

		if ( ent->parent->message ) {
			trap_SendServerCommand( -1, va( "cp \"%s\n\"", ent->parent->message ) );
		}
		G_LogPrintf( "Item: %i objective\n", other->s.number );

		G_FreeEntity( ent );
		return;
	}
	
	// the same pickup rules are used for client side and server side
	if ( !BG_CanItemBeGrabbed( g_gametype.integer, &ent->s, &other->client->ps, level.time ) ) {
		return;
	}

	G_LogPrintf( "Item: %i %s\n", other->s.number, ent->item->classname );

	predict = other->client->pers.predictItemPickup;

	// call the item-specific pickup function
	switch( ent->item->giType ) {
	case IT_WEAPON:
		respawn = Pickup_Weapon(ent, other);
//		predict = qfalse;
		break;
	case IT_AMMO:
		respawn = Pickup_Ammo(ent, other);
//		predict = qfalse;
		break;
	case IT_HEALTH:
		respawn = Pickup_Health(ent, other);
		break;
	case IT_POWERUP:
		respawn = Pickup_Powerup(ent, other);
		predict = qfalse;
		break;
#ifdef MISSIONPACK
	case IT_PERSISTANT_POWERUP:
		respawn = Pickup_PersistantPowerup(ent, other);
		break;
#endif
	case IT_TEAM:
		respawn = Pickup_Team(ent, other);
		break;
	case IT_HOLDABLE:
		respawn = Pickup_Holdable(ent, other);
		break;
	default:
		return;
	}

	if ( !respawn ) {
		return;
	}

	// play the normal pickup sound
	if (predict) {
		G_AddPredictableEvent( other, EV_ITEM_PICKUP, ent->s.modelindex );
	} else {
		G_AddEvent( other, EV_ITEM_PICKUP, ent->s.modelindex );
	}

	// powerup pickups are global broadcasts
	if ( ent->item->giType == IT_POWERUP || ent->item->giType == IT_TEAM) {
		// if we want the global sound to play
		if (!ent->speed) {
			gentity_t	*te;

			te = G_TempEntity( ent->s.pos.trBase, EV_GLOBAL_ITEM_PICKUP );
			te->s.eventParm = ent->s.modelindex;
			te->r.svFlags |= SVF_BROADCAST;
		} else {
			gentity_t	*te;

			te = G_TempEntity( ent->s.pos.trBase, EV_GLOBAL_ITEM_PICKUP );
			te->s.eventParm = ent->s.modelindex;
			// only send this temp entity to a single client
			te->r.svFlags |= SVF_SINGLECLIENT;
			te->r.singleClient = other->s.number;
		}
	}

	// fire item targets
	G_UseTargets (ent, other);

	// wait of -1 will not respawn
	if ( ent->wait == -1 ) {
		ent->r.svFlags |= SVF_NOCLIENT;
		ent->s.eFlags |= EF_NODRAW;
		ent->r.contents = 0;
		ent->unlinkAfterEvent = qtrue;
		return;
	}

	// non zero wait overrides respawn time
	if ( ent->wait ) {
		respawn = ent->wait;
	}

	// random can be used to vary the respawn time
	if ( ent->random ) {
		respawn += crandom() * ent->random;
		if ( respawn < 1 ) {
			respawn = 1;
		}
	}

	// dropped items will not respawn
	if ( ent->flags & FL_DROPPED_ITEM ) {
		ent->freeAfterEvent = qtrue;
	}

	// picked up items still stay around, they just don't
	// draw anything.  This allows respawnable items
	// to be placed on movers.
	ent->r.svFlags |= SVF_NOCLIENT;
	ent->s.eFlags |= EF_NODRAW;
	ent->r.contents = 0;

	// ZOID
	// A negative respawn times means to never respawn this item (but don't 
	// delete it).  This is used by items that are respawned by third party 
	// events such as ctf flags
	if ( respawn <= 0 ) {
		ent->nextthink = 0;
		ent->think = 0;
	} else {
		ent->nextthink = level.time + respawn * 1000;
		ent->think = RespawnItem;
	}
	trap_LinkEntity( ent );
}


//======================================================================

/*
================
LaunchItem

Spawns an item and tosses it forward
================
*/
gentity_t *LaunchItem( gitem_t *item, vec3_t origin, vec3_t velocity, int flags ) {
	gentity_t	*dropped;

	dropped = G_Spawn();

	dropped->s.eType = ET_ITEM;
	dropped->s.modelindex = item - bg_itemlist;	// store item number in modelindex
	dropped->s.modelindex2 = 0;					// zero because of objective item
	dropped->s.time = level.time;

	dropped->s.angles[YAW] = random() * 360;
	dropped->s.angles[ROLL] = 90;

	dropped->classname = item->classname;
	dropped->item = item;
	VectorSet (dropped->r.mins, -ITEM_RADIUS, -ITEM_RADIUS, -ITEM_RADIUS);
	VectorSet (dropped->r.maxs, ITEM_RADIUS, ITEM_RADIUS, 0);
	dropped->r.contents = CONTENTS_TRIGGER;

	dropped->touch = Touch_Item;

	G_SetOrigin( dropped, origin );
	dropped->s.pos.trType = TR_GRAVITY;
	dropped->s.pos.trTime = level.time;
	VectorCopy( velocity, dropped->s.pos.trDelta );

	dropped->s.eFlags |= EF_BOUNCE_HALF;
	if ( g_gametype.integer != GT_TACTICAL || item->giType != IT_WEAPON ) {
		// auto-remove after 30 seconds
		dropped->think = G_FreeEntity;
		dropped->nextthink = level.time + 10000;
	} else { 
		// don't remove weapons if in GT_TACTICAL
		dropped->think = NULL;
		dropped->nextthink = 0;		
	}
	if ( g_gametype.integer == GT_TACTICAL && item->giType == IT_WEAPON && item->giTag == WP_THERMO ) {
		// thermo lance explodes in GT_TACTICAL
		dropped->think = G_ExplodeEntity;
		dropped->nextthink = level.time + weLi[item->giTag].recoil_time;
	}

	dropped->flags = flags;

	trap_LinkEntity (dropped);

	return dropped;
}


/*
================
LaunchObjectiveItem

Spawns an item and tosses it forward
================
*/
gentity_t *LaunchObjectiveItem( gentity_t *parent, vec3_t origin, vec3_t velocity ) {
	gentity_t	*item;
	
	item = G_Spawn();

	item->s.eType = ET_ITEM;
	item->s.modelindex = parent->s.modelindex;
	item->s.modelindex2 = parent->s.modelindex2;
	item->s.time = level.time;

	item->parent = parent;
	item->classname = "objective";
	VectorSet (item->r.mins, -7, -7, -7);
	VectorSet (item->r.maxs, 7, 7, 7);
	item->r.contents = CONTENTS_TRIGGER;

	item->touch = Touch_Item;

	G_SetOrigin( item, origin );
	item->s.pos.trType = TR_GRAVITY;
	item->s.pos.trTime = level.time;
	VectorCopy( velocity, item->s.pos.trDelta );

	item->s.eFlags |= EF_BOUNCE_HALF | EF_OBJECTIVE;
	item->think = NULL;
	item->nextthink = 0;		

	item->flags = FL_DROPPED_ITEM;

	trap_LinkEntity (item);

	return item;
}


/*
================
Drop_Item

Spawns an item and tosses it forward
================
*/
gentity_t *Drop_Item( gentity_t *ent, gitem_t *item, qboolean thrown ) {
	vec3_t	velocity;
	vec3_t	angles;
	vec3_t origin;

	BG_EvaluateTrajectory( &ent->s.pos, level.time, origin );
	origin[2] += ent->client->ps.viewheight;

	BG_EvaluateTrajectory( &ent->s.apos, level.time, angles );
	angles[PITCH] = 0;

	if ( thrown ) {
		AngleVectors( angles, velocity, NULL, NULL );
		VectorScale( velocity, 500, velocity );
		velocity[2] += 40;
	} else {
		angles[YAW] += crandom()*60;
		AngleVectors( angles, velocity, NULL, NULL );
		VectorScale( velocity, random()*200, velocity );
		velocity[2] = 30;		
	}
	
	return LaunchItem( item, origin, velocity, FL_DROPPED_ITEM );
}


/*
=================
Drop_Objective_Item
=================
*/
#define THROW_DELAY		3000

void Drop_Objective_Item( gentity_t *ent, qboolean thrown ) {
	vec3_t	origin;
	vec3_t	velocity;
	vec3_t	angles;

	if ( !ent->client->ps.powerups[PW_ITEM] )
		return;

	if ( ent->client->itemThrowTime + THROW_DELAY > level.time ) 
		return;

	ent->client->itemThrowTime = level.time;

	// remove the item from the player
	ent->client->ps.powerups[PW_ITEM] = 0;
	ent->client->itemParent->count = 1;
	if ( ent->client->itemParent->message2 ) {
		trap_SendServerCommand( -1, va( "cp \"%s\n\"", ent->client->itemParent->message2 ) );
	}

	BG_EvaluateTrajectory( &ent->s.pos, level.time, origin );
	origin[2] += ent->client->ps.viewheight;

	BG_EvaluateTrajectory( &ent->s.apos, level.time, angles );
	angles[PITCH] = 0;

	if ( thrown ) {
		AngleVectors( angles, velocity, NULL, NULL );
		VectorScale( velocity, 500, velocity );
		velocity[2] += 40;
	} else {
		angles[YAW] += crandom()*180 - 90;
		AngleVectors( angles, velocity, NULL, NULL );
		VectorScale( velocity, crandom()*150+150, velocity );
		velocity[2] += 30;		
	}
	
	LaunchObjectiveItem( ent->client->itemParent, origin, velocity );
}


/*
================
Use_Item

Respawn the item
================
*/
void Use_Item( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	RespawnItem( ent );
}

//======================================================================

/*
================
FinishSpawningItem

Traces down to find where an item should rest, instead of letting them
free fall from their spawn points
================
*/
void FinishSpawningItem( gentity_t *ent ) {
	trace_t		tr;
	vec3_t		dest;

	VectorSet( ent->r.mins, -ITEM_RADIUS, -ITEM_RADIUS, -ITEM_RADIUS );
	VectorSet( ent->r.maxs, ITEM_RADIUS, ITEM_RADIUS, 0 );

	ent->s.eType = ET_ITEM;
	ent->s.modelindex = ent->item - bg_itemlist;		// store item number in modelindex
	ent->s.modelindex2 = 0;								// zero because of objective item

	ent->r.contents = CONTENTS_TRIGGER;
	ent->touch = Touch_Item;
	// useing an item causes it to respawn
	ent->use = Use_Item;

	if ( ent->spawnflags & 1 ) {
		// suspended
		G_SetOrigin( ent, ent->s.origin );
	} else {
		// drop to floor
		VectorSet( dest, ent->s.origin[0], ent->s.origin[1], ent->s.origin[2] - 4096 );
		trap_Trace( &tr, ent->s.origin, ent->r.mins, ent->r.maxs, dest, ent->s.number, MASK_SOLID );
		if ( tr.startsolid ) {
			G_Printf ("FinishSpawningItem: %s startsolid at %s\n", ent->classname, vtos(ent->s.origin));
			G_FreeEntity( ent );
			return;
		}

		// allow to ride movers
		ent->s.groundEntityNum = tr.entityNum;

		G_SetOrigin( ent, tr.endpos );
	}

	// weapons need ammo values
	if ( ent->item->giType == IT_WEAPON ) {
		ent->count = weLi[ent->item->giTag].ammo_max;
		ent->count2 = weLi[ent->item->giTag].clip_max;
	}

	// team slaves and targeted items aren't present at start
	if ( ( ent->flags & FL_TEAMSLAVE ) || ent->targetname ) {
		ent->s.eFlags |= EF_NODRAW;
		ent->r.contents = 0;
		return;
	}

	// powerups don't spawn in for a while
	if ( ent->item->giType == IT_POWERUP ) {
		float	respawn;

		respawn = 45 + crandom() * 15;
		ent->s.eFlags |= EF_NODRAW;
		ent->r.contents = 0;
		ent->nextthink = level.time + respawn * 1000;
		ent->think = RespawnItem;
		return;
	}

	if ( g_gametype.integer == GT_TACTICAL ) {
		ent->s.eFlags |= EF_NODRAW;
		ent->touch = 0;
		ent->r.contents = 0;
		ent->nextthink = 0;
		ent->think = 0;
		return;
	}

	trap_LinkEntity (ent);
}


qboolean	itemRegistered[MAX_ITEMS];

/*
==================
G_CheckTeamItems
==================
*/
void G_CheckTeamItems( void ) {

	// Set up team stuff
	Team_InitGame();

/*	if( g_gametype.integer == GT_CTF ) {
		gitem_t	*item;

		// check for the two flags
		item = BG_FindItem( "Red Flag" );
		if ( !item || !itemRegistered[ item - bg_itemlist ] ) {
			G_Printf( S_COLOR_YELLOW "WARNING: No team_CTF_redflag in map" );
		}
		item = BG_FindItem( "Blue Flag" );
		if ( !item || !itemRegistered[ item - bg_itemlist ] ) {
			G_Printf( S_COLOR_YELLOW "WARNING: No team_CTF_blueflag in map" );
		}
	}
#ifdef MISSIONPACK
	if( g_gametype.integer == GT_1FCTF ) {
		gitem_t	*item;

		// check for all three flags
		item = BG_FindItem( "Red Flag" );
		if ( !item || !itemRegistered[ item - bg_itemlist ] ) {
			G_Printf( S_COLOR_YELLOW "WARNING: No team_CTF_redflag in map" );
		}
		item = BG_FindItem( "Blue Flag" );
		if ( !item || !itemRegistered[ item - bg_itemlist ] ) {
			G_Printf( S_COLOR_YELLOW "WARNING: No team_CTF_blueflag in map" );
		}
		item = BG_FindItem( "Neutral Flag" );
		if ( !item || !itemRegistered[ item - bg_itemlist ] ) {
			G_Printf( S_COLOR_YELLOW "WARNING: No team_CTF_neutralflag in map" );
		}
	}

	if( g_gametype.integer == GT_OBELISK ) {
		gentity_t	*ent;

		// check for the two obelisks
		ent = NULL;
		ent = G_Find( ent, FOFS(classname), "team_redobelisk" );
		if( !ent ) {
			G_Printf( S_COLOR_YELLOW "WARNING: No team_redobelisk in map" );
		}

		ent = NULL;
		ent = G_Find( ent, FOFS(classname), "team_blueobelisk" );
		if( !ent ) {
			G_Printf( S_COLOR_YELLOW "WARNING: No team_blueobelisk in map" );
		}
	}

	if( g_gametype.integer == GT_HARVESTER ) {
		gentity_t	*ent;

		// check for all three obelisks
		ent = NULL;
		ent = G_Find( ent, FOFS(classname), "team_redobelisk" );
		if( !ent ) {
			G_Printf( S_COLOR_YELLOW "WARNING: No team_redobelisk in map" );
		}

		ent = NULL;
		ent = G_Find( ent, FOFS(classname), "team_blueobelisk" );
		if( !ent ) {
			G_Printf( S_COLOR_YELLOW "WARNING: No team_blueobelisk in map" );
		}

		ent = NULL;
		ent = G_Find( ent, FOFS(classname), "team_neutralobelisk" );
		if( !ent ) {
			G_Printf( S_COLOR_YELLOW "WARNING: No team_neutralobelisk in map" );
		}
	}
#endif*/
}


/*
==============
ClearRegisteredItems
==============
*/
void ClearRegisteredItems( void ) {
	memset( itemRegistered, 0, sizeof( itemRegistered ) );

	// players always start with the base weapon
	RegisterItem( BG_FindItemForWeapon( WP_SHOCKER ) );
	RegisterItem( BG_FindItemForWeapon( WP_STANDARD ) );
}

/*
===============
RegisterItem

The item will be added to the precache list
===============
*/
void RegisterItem( gitem_t *item ) {
	if ( !item ) {
		G_Error( "RegisterItem: NULL" );
	}
	itemRegistered[ item - bg_itemlist ] = qtrue;
}


/*
===============
SaveRegisteredItems

Write the needed items to a config string
so the client will know which ones to precache
===============
*/
void SaveRegisteredItems( void ) {
	char	string[MAX_ITEMS+1];
	int		i;
	int		count;

	count = 0;
	for ( i = 0 ; i < bg_numItems ; i++ ) {
		if ( itemRegistered[i] ) {
			count++;
			string[i] = '1';
		} else {
			string[i] = '0';
		}
	}
	string[ bg_numItems ] = 0;

	G_Printf( "%i items registered\n", count );
	trap_SetConfigstring(CS_ITEMS, string);
}


/*
============
G_ItemDisabled
============
*/
int G_ItemDisabled( gitem_t *item ) {

	char name[128];

	Com_sprintf(name, sizeof(name), "disable_%s", item->classname);
	return trap_Cvar_VariableIntegerValue( name );
}

/*
============
G_SpawnItem

Sets the clipping size and plants the object on the floor.

Items can't be immediately dropped to floor, because they might
be on an entity that hasn't spawned yet.
============
*/
void G_SpawnItem (gentity_t *ent, gitem_t *item) {
	G_SpawnFloat( "random", "0", &ent->random );
	G_SpawnFloat( "wait", "0", &ent->wait );

	// make them lie on the ground
	ent->s.angles[ROLL] = 90;

	// don't register item if in GT_TACTICAL, they are registered in the worldspawn function
	if ( g_gametype.integer != GT_TACTICAL ) RegisterItem( item );
	if ( G_ItemDisabled(item) )
		return;

	ent->item = item;
	// some movers spawn on the second frame, so delay item
	// spawns until the third frame so they can ride trains
	ent->nextthink = level.time + FRAMETIME * 2;
	ent->think = FinishSpawningItem;

	ent->physicsBounce = 0.50;		// items are bouncy

	if ( item->giType == IT_POWERUP ) {
		G_SoundIndex( "sound/items/poweruprespawn.wav" );
		G_SpawnFloat( "noglobalsound", "0", &ent->speed);
	}

#ifdef MISSIONPACK
	if ( item->giType == IT_PERSISTANT_POWERUP ) {
		ent->s.generic1 = ent->spawnflags;
	}
#endif
}


/*
================
G_BounceItem

================
*/
void G_BounceItem( gentity_t *ent, trace_t *trace ) {
	vec3_t	velocity;
	float	dot;
	int		hitTime;

	// reflect the velocity on the trace plane
	hitTime = level.previousTime + ( level.time - level.previousTime ) * trace->fraction;
	BG_EvaluateTrajectoryDelta( &ent->s.pos, hitTime, velocity );
	dot = DotProduct( velocity, trace->plane.normal );
	VectorMA( velocity, -2*dot, trace->plane.normal, ent->s.pos.trDelta );

	// cut the velocity to keep from bouncing forever
	VectorScale( ent->s.pos.trDelta, ent->physicsBounce, ent->s.pos.trDelta );

	// check for stop
	if ( trace->plane.normal[2] > 0 && ent->s.pos.trDelta[2] < 40 ) {
		trace->endpos[2] += 1.0;	// make sure it is off ground
		SnapVector( trace->endpos );
		G_SetOrigin( ent, trace->endpos );
		ent->s.groundEntityNum = trace->entityNum;
		return;
	}

	VectorAdd( ent->r.currentOrigin, trace->plane.normal, ent->r.currentOrigin);
	VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );
	ent->s.pos.trTime = level.time;
}


/*
================
G_RunItem

================
*/
void G_RunItem( gentity_t *ent ) {
	vec3_t		origin;
	trace_t		tr;
	int			contents;
	int			mask;

	// if groundentity has been set to -1, it may have been pushed off an edge
	if ( ent->s.groundEntityNum == -1 ) {
		if ( ent->s.pos.trType != TR_GRAVITY ) {
			ent->s.pos.trType = TR_GRAVITY;
			ent->s.pos.trTime = level.time;
		}
	}

	if ( ent->s.pos.trType == TR_STATIONARY ) {
		// check think function
		G_RunThink( ent );
		return;
	}

	// get current position
	BG_EvaluateTrajectory( &ent->s.pos, level.time, origin );

	// trace a line from the previous position to the current position
	if ( ent->clipmask ) {
		mask = ent->clipmask;
	} else {
		mask = MASK_PLAYERSOLID & ~CONTENTS_BODY;//MASK_SOLID;
	}
	trap_Trace( &tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, origin, 
		ent->r.ownerNum, mask );

	VectorCopy( tr.endpos, ent->r.currentOrigin );

	if ( tr.startsolid ) {
		tr.fraction = 0;
	}

	trap_LinkEntity( ent );	// FIXME: avoid this for stationary?

	// check think function
	G_RunThink( ent );

	if ( tr.fraction == 1 ) {
		return;
	}

	// if it is in a nodrop volume, remove it
	contents = trap_PointContents( ent->r.currentOrigin, -1 );
	if ( contents & CONTENTS_NODROP ) {
		if ( (ent->s.eFlags & EF_OBJECTIVE) ) {
			LaunchObjectiveItem( ent->parent, ent->parent->s.origin, vec3_origin );
			ent->parent->count = 0;
			G_FreeEntity( ent );
		} else if (ent->item && ent->item->giType == IT_TEAM) {
			Team_FreeEntity(ent);
		} else {
			G_FreeEntity( ent );
		}
		return;
	}

	G_BounceItem( ent, &tr );
}

