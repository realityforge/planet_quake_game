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

		//NT - remember when it was picked up for proper speed calc
		if ( ent->item->giTag == PW_SPEED ) {
			other->client->ps.stats[STAT_SPEED_PICKUP] = other->client->ps.powerups[PW_SPEED];
		}
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
  	if ( g_gametype.integer >= GT_TEAM && other->client->sess.sessionTeam == client->sess.sessionTeam  ) {
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

	//NT - vitality powerup
	switch ( ent->item->giTag ) {
	case PW_VITALITY:
		if ( quantity >= 30 ) {
			if ( other->health + 100 > other->client->ps.stats[STAT_MAX_HEALTH] * 3 ) {
				other->health = other->client->ps.stats[STAT_MAX_HEALTH] * 3;
				other->client->ps.stats[STAT_HEALTH] = other->client->ps.stats[STAT_MAX_HEALTH] * 3;
			} else {
				other->health += 100;
				other->client->ps.stats[STAT_HEALTH] += 100;
			}
		} else {
			if ( other->health + 50 > other->client->ps.stats[STAT_MAX_HEALTH] * 3 ) {
				other->health = other->client->ps.stats[STAT_MAX_HEALTH] * 3;
				other->client->ps.stats[STAT_HEALTH] = other->client->ps.stats[STAT_MAX_HEALTH] * 3;
			} else {
				other->health += 50;
				other->client->ps.stats[STAT_HEALTH] += 50;
			}
		}
		break;
	}

	return RESPAWN_POWERUP;
}

//======================================================================

#ifdef MISSIONPACK
int Pickup_PersistantPowerup( gentity_t *ent, gentity_t *other ) {
	int		clientNum;
	char	userinfo[MAX_INFO_STRING];
	float	handicap;
	int		max;

	other->client->ps.stats[STAT_PERSISTANT_POWERUP] = ent->item - bg_itemlist;
	other->client->persistantPowerup = ent;

	switch( ent->item->giTag ) {
	case PW_GUARD:
		clientNum = other->client->ps.clientNum;
		trap_GetUserinfo( clientNum, userinfo, sizeof(userinfo) );
		handicap = atof( Info_ValueForKey( userinfo, "handicap" ) );
		if( handicap<=0.0f || handicap>100.0f) {
			handicap = 100.0f;
		}
		max = (int)(2 *  handicap);

		other->health = max;
		other->client->ps.stats[STAT_HEALTH] = max;
		other->client->ps.stats[STAT_MAX_HEALTH] = max;
		other->client->ps.stats[STAT_ARMOR] = max;
		other->client->pers.maxHealth = max;

		break;

	case PW_SCOUT:
		clientNum = other->client->ps.clientNum;
		trap_GetUserinfo( clientNum, userinfo, sizeof(userinfo) );
		handicap = atof( Info_ValueForKey( userinfo, "handicap" ) );
		if( handicap<=0.0f || handicap>100.0f) {
			handicap = 100.0f;
		}
		other->client->pers.maxHealth = handicap;
		other->client->ps.stats[STAT_ARMOR] = 0;
		break;

	case PW_DOUBLER:
		clientNum = other->client->ps.clientNum;
		trap_GetUserinfo( clientNum, userinfo, sizeof(userinfo) );
		handicap = atof( Info_ValueForKey( userinfo, "handicap" ) );
		if( handicap<=0.0f || handicap>100.0f) {
			handicap = 100.0f;
		}
		other->client->pers.maxHealth = handicap;
		break;
	case PW_AMMOREGEN:
		clientNum = other->client->ps.clientNum;
		trap_GetUserinfo( clientNum, userinfo, sizeof(userinfo) );
		handicap = atof( Info_ValueForKey( userinfo, "handicap" ) );
		if( handicap<=0.0f || handicap>100.0f) {
			handicap = 100.0f;
		}
		other->client->pers.maxHealth = handicap;
		memset(other->client->ammoTimes, 0, sizeof(other->client->ammoTimes));
		break;
	default:
		clientNum = other->client->ps.clientNum;
		trap_GetUserinfo( clientNum, userinfo, sizeof(userinfo) );
		handicap = atof( Info_ValueForKey( userinfo, "handicap" ) );
		if( handicap<=0.0f || handicap>100.0f) {
			handicap = 100.0f;
		}
		other->client->pers.maxHealth = handicap;
		break;
	}

	return -1;
}

//======================================================================
#endif

int Pickup_Holdable( gentity_t *ent, gentity_t *other ) {

	other->client->ps.stats[STAT_HOLDABLE_ITEM] = ent->item - bg_itemlist;

	if( ent->item->giTag == HI_KAMIKAZE ) {
		other->client->ps.eFlags |= EF_KAMIKAZE;
	}

	return RESPAWN_HOLDABLE;
}


//======================================================================

void Add_Ammo (gentity_t *ent, int weapon, int count)
{
	ent->client->ps.ammo[weapon] += count;
	if ( ent->client->ps.ammo[weapon] > 200 ) {
		ent->client->ps.ammo[weapon] = 200;
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

	//NT - shotgun ammo is doubled now
	if ( ent->item->giTag == WP_SHOTGUN )
		quantity *= 2;

	Add_Ammo (other, ent->item->giTag, quantity);

	return RESPAWN_AMMO;
}

//======================================================================


int Pickup_Weapon (gentity_t *ent, gentity_t *other) {
	int		quantity;

	if ( ent->count < 0 ) {
		quantity = 0; // None for you, sir!
	} else {
		if ( ent->count ) {
			quantity = ent->count;
		} else {
			quantity = ent->item->quantity;
		}

		//NT - shotgun ammo is doubled now
		if ( ent->item->giTag == WP_SHOTGUN )
			quantity *= 2;

		// dropped items and teamplay weapons always have full ammo
		if ( ! (ent->flags & FL_DROPPED_ITEM) && g_gametype.integer != GT_TEAM ) {
			// respawning rules
			// drop the quantity if the already have over the minimum
			if ( other->client->ps.ammo[ ent->item->giTag ] < quantity ) {
				quantity = quantity - other->client->ps.ammo[ ent->item->giTag ];
			} else {
				// only add a single shot
				//NT - shotgun ammo is doubled now
				if ( ent->item->giTag == WP_SHOTGUN )
					quantity = 2;
				else
					quantity = 1;
			}
		}
	}

	// add the weapon
	other->client->ps.stats[STAT_WEAPONS] |= ( 1 << ent->item->giTag );

	Add_Ammo( other, ent->item->giTag, quantity );

	if (ent->item->giTag == WP_GRAPPLING_HOOK)
		other->client->ps.ammo[ent->item->giTag] = -1; // unlimited ammo

	// team deathmatch has slow weapon respawns
	if ( g_gametype.integer == GT_TEAM ) {
		return g_weaponTeamRespawn.integer;
	}

	return g_weaponRespawn.integer;
}


//======================================================================

int Pickup_Health (gentity_t *ent, gentity_t *other) {
	int			max;
	int			quantity;

	// small and mega healths will go over the max
#ifdef MISSIONPACK
	if( other->client && bg_itemlist[other->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD ) {
		max = other->client->ps.stats[STAT_MAX_HEALTH];
	}
	else
#endif
	//NT - vitality powerup
	if ( other->client->ps.powerups[PW_VITALITY] ) {
		max = other->client->ps.stats[STAT_MAX_HEALTH] * 3;
	} else {
		if ( ent->item->quantity != 5 && ent->item->quantity != 100 ) {
			max = other->client->ps.stats[STAT_MAX_HEALTH];
		} else {
			max = other->client->ps.stats[STAT_MAX_HEALTH] * 2;
		}
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

int Pickup_Armor( gentity_t *ent, gentity_t *other ) {
#ifdef MISSIONPACK
	int		upperBound;

	other->client->ps.stats[STAT_ARMOR] += ent->item->quantity;

	if( other->client && bg_itemlist[other->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD ) {
		upperBound = other->client->ps.stats[STAT_MAX_HEALTH];
	}
	else {
		upperBound = other->client->ps.stats[STAT_MAX_HEALTH] * 2;
	}

	if ( other->client->ps.stats[STAT_ARMOR] > upperBound ) {
		other->client->ps.stats[STAT_ARMOR] = upperBound;
	}
#else
	other->client->ps.stats[STAT_ARMOR] += ent->item->quantity;
	if ( other->client->ps.stats[STAT_ARMOR] > other->client->ps.stats[STAT_MAX_HEALTH] * 2 ) {
		other->client->ps.stats[STAT_ARMOR] = other->client->ps.stats[STAT_MAX_HEALTH] * 2;
	}
#endif

	return RESPAWN_ARMOR;
}

//======================================================================
//NT - hologram

// this should never get called if everything damages through G_Damage,
// since G_Damage passes damage to proxies on to their owners
void PlayerProxy_Die( gentity_t *ent, gentity_t *inflictor, gentity_t *attacker, int damage, int mod ) {
}

void LastProxyThink( gentity_t *proxy ) {
	gentity_t		*ent, *tent;
	vec3_t			angles;

	ent = &g_entities[proxy->s.otherEntityNum];

	// toss the powerups and remove them from the player
	TossPowerups( ent );
	memset( ent->client->ps.powerups, 0, sizeof(ent->client->ps.powerups) );

	// turn off the broadcast flag
	ent->r.svFlags &= ~SVF_BROADCAST;

	tent = G_TempEntity( ent->r.currentOrigin, EV_PLAYER_TELEPORT_OUT );
	tent->s.clientNum = ent->s.clientNum;

	VectorCopy( proxy->s.apos.trBase, angles );
	angles[PITCH] = ent->client->ps.viewangles[PITCH];
	TeleportPlayer( ent, proxy->r.currentOrigin, angles, qfalse, qfalse, qfalse );
	VectorClear( ent->client->ps.velocity );

	// make the stasis sound for everyone but the player (that's in CG_CheckLocalSounds)
	tent = G_TempEntity( proxy->r.currentOrigin, EV_STASIS_SOUND );
	tent->r.svFlags |= SVF_NOTSINGLECLIENT;
	tent->r.singleClient = ent->s.number;

	G_FreeEntity( proxy );
}

void G_DestroyPlayerProxies( gentity_t *ent ) {
	gentity_t		*proxy;

	proxy = NULL;
	while ((proxy = G_Find (proxy, FOFS(classname), "playerproxy")) != NULL) {
		if ( proxy->s.otherEntityNum == ent->s.number ) {
			LastProxyThink( proxy );
		}
	}
}

// sometimes this is better to call - like when the player is in the loop
// where he touches triggers
// it's probably safer to call the other one, though
void G_DestroyPlayerProxiesLater( gentity_t *ent ) {
	gentity_t		*proxy;

	proxy = NULL;
	while ((proxy = G_Find (proxy, FOFS(classname), "playerproxy")) != NULL) {
		if ( proxy->s.otherEntityNum == ent->s.number ) {
			proxy->lastthinktime = level.time + 1;
		}
	}
}

void Use_Hologram_Generator( gentity_t *ent ) {
	gentity_t		*proxy, *spot, *tent;
	vec3_t			angles, origin;

	// round timing to seconds
	if ( !ent->client->ps.powerups[PW_HOLOGRAM] ) {
		ent->client->ps.powerups[PW_HOLOGRAM] = level.time - ( level.time % 1000 );
	}

	// add 30 seconds to the powerup time
	ent->client->ps.powerups[PW_HOLOGRAM] += 30000;

	// put the client's entity in broadcast mode so other players can see the proxy
	// mimicking him
	ent->r.svFlags |= SVF_BROADCAST;

	// there should only be one player proxy
	// if we find one, we'll just add to its life and return
	proxy = NULL;
	while ((proxy = G_Find (proxy, FOFS(classname), "playerproxy")) != NULL) {
		if ( proxy->s.otherEntityNum == ent->s.number ) {
			proxy->lastthinktime = ent->client->ps.powerups[PW_HOLOGRAM];
			return;
		}
	}

	// create a player proxy so people can shoot it and damage the player
	proxy = G_Spawn();
	proxy->s.eType = ET_PLAYER_PROXY;
	proxy->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	proxy->classname = "playerproxy";
	proxy->lastthinktime = ent->client->ps.powerups[PW_HOLOGRAM];
	proxy->lastthink = LastProxyThink;
	proxy->r.ownerNum = ENTITYNUM_WORLD;
	proxy->s.otherEntityNum = ent->s.number;
	proxy->s.time = level.time;
	proxy->parent = NULL;
	proxy->takedamage = qtrue;
	proxy->health = 100000;
	proxy->die = PlayerProxy_Die;

	proxy->clipmask = MASK_PLAYERSOLID;
	proxy->r.contents = CONTENTS_BODY;
	VectorCopy (ent->r.mins, proxy->r.mins);
	VectorCopy (ent->r.maxs, proxy->r.maxs);

	proxy->physicsObject = qtrue;
	proxy->physicsBounce = 0.2f;
	proxy->s.pos.trType = TR_GRAVITY;
	VectorCopy( ent->r.currentOrigin, proxy->s.pos.trBase );
	VectorCopy( ent->r.currentOrigin, proxy->r.currentOrigin );
	proxy->s.pos.trBase[2] += 1.0f;
	proxy->r.currentOrigin[2] += 1.0f;
	VectorCopy( ent->client->ps.velocity, proxy->s.pos.trDelta );
	proxy->s.pos.trTime = level.time;

	VectorCopy( ent->client->ps.viewangles, proxy->s.apos.trBase );

	trap_LinkEntity( proxy );

	// make the sound for everyone but the player
	// (the player's sound will be made in CG_CheckLocalSounds)
	tent = G_TempEntity( proxy->r.currentOrigin, EV_STASIS_SOUND );
	tent->r.svFlags |= SVF_NOTSINGLECLIENT;
	tent->r.singleClient = ent->s.number;

	spot = NULL;
	if ( g_gametype.integer < GT_CTF ) {
		spot = SelectSpawnPoint(ent->client->ps.origin, origin, angles);
//		spot = SelectNearestOfClass(ent->client->ps.origin, origin, angles, "info_player_deathmatch");
	} else {
		if ( ent->client->sess.sessionTeam == TEAM_RED || ent->client->sess.sessionTeam == TEAM_WATCHING_RED ) {
			if ( ent->client->ps.powerups[PW_BLUEFLAG] ) {
				spot = SelectNearestOfClass(ent->client->ps.origin, origin, angles, "team_CTF_bluespawn");
			} else {
				spot = SelectNearestOfClass(ent->client->ps.origin, origin, angles, "team_CTF_redspawn");
			}
		}
		else if ( ent->client->sess.sessionTeam == TEAM_BLUE || ent->client->sess.sessionTeam == TEAM_WATCHING_BLUE ) {
			if ( ent->client->ps.powerups[PW_REDFLAG] ) {
				spot = SelectNearestOfClass(ent->client->ps.origin, origin, angles, "team_CTF_redspawn");
			} else {
				spot = SelectNearestOfClass(ent->client->ps.origin, origin, angles, "team_CTF_bluespawn");
			}
		}

		if ( !spot ) {
			spot = SelectSpawnPoint(ent->client->ps.origin, origin, angles);
//			spot = SelectNearestOfClass(ent->client->ps.origin, origin, angles, "info_player_deathmatch");
		}
	}

	TeleportPlayer(ent, origin, angles, qfalse, qtrue, qtrue);

	tent = G_TempEntity( origin, EV_PLAYER_TELEPORT_IN );
	tent->s.clientNum = ent->s.clientNum;
}

//======================================================================
//NT - ammo explosions

void Railgun_Ammo_Expl_Think( gentity_t *ent ) {
	vec3_t		dir;
	vec_t		angle, pitch;
	int			ammoleft;

	ammoleft = ent->s.generic1 - random() * 10;
	if ( ammoleft < 0 )
		ammoleft = 0;

	while ( ent->s.generic1 > ammoleft ) {
		angle = random() * 360;
		pitch = random() * 20 - 5;

		dir[0] = cos( DEG2RAD(angle) ) * cos( DEG2RAD(pitch) );
		dir[1] = sin( DEG2RAD(angle) ) * cos( DEG2RAD(pitch) );
		dir[2] = sin( DEG2RAD(pitch) );

		weapon_railgun_fire( ent->parent, ent->r.currentOrigin, dir, qtrue, 25, MOD_AMMO, ent->damage );

		ent->s.generic1--;
	}

	if ( ent->s.generic1 > 0 )
		ent->nextthink = level.time + 1;
}

/*
===============
Railgun_Ammo_Expl
===============
*/
void Railgun_Ammo_Expl( gentity_t *ammo, gentity_t *attacker, int quadFactor ) {
	gentity_t	*ent;

	ent = G_Spawn();
	ent->classname = "railgunammoexploder";
	ent->nextthink = level.time + 1;
	ent->think = Railgun_Ammo_Expl_Think;
	ent->lastthinktime = level.time + 1000;
	ent->lastthink = G_FreeEntity;
	ent->r.svFlags = SVF_NOCLIENT;
	ent->r.ownerNum = attacker->s.number;
	ent->parent = attacker;
	ent->target_ent = NULL;
	ent->s.generic1 = ammo->item->quantity * 4;
	ent->s.pos.trType = TR_STATIONARY;
	ent->damage = quadFactor;
	VectorCopy( ammo->r.currentOrigin, ent->s.origin );
	VectorCopy( ammo->r.currentOrigin, ent->r.currentOrigin );
	trap_LinkEntity( ent );
}

//======================================================================

void Machinegun_Ammo_Expl_Think( gentity_t *ent ) {
	vec3_t		dir, a_up, a_right;
	vec_t		angle, pitch;
	int			ammoleft;

	ammoleft = ent->s.generic1 - random() * 10;
	if ( ammoleft < 0 )
		ammoleft = 0;

	while ( ent->s.generic1 > ammoleft ) {
		angle = random() * 360;
		pitch = random() * 15;

		dir[0] = cos( DEG2RAD(angle) ) * cos( DEG2RAD(pitch) );
		dir[1] = sin( DEG2RAD(angle) ) * cos( DEG2RAD(pitch) );
		dir[2] = sin( DEG2RAD(pitch) );

		// Find the up and right vectors
		VectorNormalizeFast( dir );
		PerpendicularVector( a_right, dir );
		CrossProduct( dir, a_right, a_up );

		if ( g_gametype.integer != GT_TEAM )
			Bullet_Fire( ent->parent, MACHINEGUN_SPREAD, ent->r.currentOrigin, dir, a_up, a_right, qtrue, MACHINEGUN_DAMAGE, MOD_AMMO, ent->damage );
		else
			Bullet_Fire( ent->parent, MACHINEGUN_SPREAD, ent->r.currentOrigin, dir, a_up, a_right, qtrue, MACHINEGUN_TEAM_DAMAGE, MOD_AMMO, ent->damage );

		ent->s.generic1--;
	}

	if ( ent->s.generic1 > 0 )
		ent->nextthink = level.time + 1;
}

/*
===============
Machinegun_Ammo_Expl
===============
*/
void Machinegun_Ammo_Expl( gentity_t *ammo, gentity_t *attacker, int quadFactor ) {
	gentity_t	*ent;

	ent = G_Spawn();
	ent->classname = "machinegunammoexploder";
	ent->nextthink = level.time + 1;
	ent->think = Machinegun_Ammo_Expl_Think;
	ent->lastthinktime = level.time + 1000;
	ent->lastthink = G_FreeEntity;
	ent->r.svFlags = SVF_NOCLIENT;
	ent->r.ownerNum = attacker->s.number;
	ent->parent = attacker;
	ent->target_ent = NULL;
	ent->s.generic1 = ammo->item->quantity;
	ent->s.pos.trType = TR_STATIONARY;
	ent->damage = quadFactor;
	VectorCopy( ammo->r.currentOrigin, ent->s.origin );
	VectorCopy( ammo->r.currentOrigin, ent->r.currentOrigin );
	trap_LinkEntity( ent );
}

//======================================================================

void Shotgun_Ammo_Expl_Think( gentity_t *ent ) {
	vec3_t		dir;
	vec_t		angle, pitch;
	int			ammoleft;

	ammoleft = ent->s.generic1 - random() * 5;
	if ( ammoleft < 0 )
		ammoleft = 0;

	while ( ent->s.generic1 > ammoleft ) {
		angle = random() * 360;
		pitch = random() * 15;

		dir[0] = cos( DEG2RAD(angle) ) * cos( DEG2RAD(pitch) );
		dir[1] = sin( DEG2RAD(angle) ) * cos( DEG2RAD(pitch) );
		dir[2] = sin( DEG2RAD(pitch) );

		weapon_shotgun_fire( ent->parent, ent->r.currentOrigin, dir, qtrue, MOD_AMMO, ent->damage );

		ent->s.generic1--;
	}

	if ( ent->s.generic1 > 0 )
		ent->nextthink = level.time + 1;
}

/*
===============
Shotgun_Ammo_Expl
===============
*/
void Shotgun_Ammo_Expl( gentity_t *ammo, gentity_t *attacker, int quadFactor ) {
	gentity_t	*ent;

	ent = G_Spawn();
	ent->classname = "shotgunammoexploder";
	ent->nextthink = level.time + 1;
	ent->think = Shotgun_Ammo_Expl_Think;
	ent->lastthinktime = level.time + 1000;
	ent->lastthink = G_FreeEntity;
	ent->r.svFlags = SVF_NOCLIENT;
	ent->r.ownerNum = attacker->s.number;
	ent->parent = attacker;
	ent->target_ent = NULL;
	ent->s.generic1 = ammo->item->quantity * 4;
	ent->s.pos.trType = TR_STATIONARY;
	ent->damage = quadFactor;
	VectorCopy( ammo->r.currentOrigin, ent->s.origin );
	VectorCopy( ammo->r.currentOrigin, ent->r.currentOrigin );
	trap_LinkEntity( ent );
}


//======================================================================

void Lightning_Ammo_Expl_Think( gentity_t *ent ) {
	if( G_RadiusDamage( ent->r.currentOrigin, ent->parent, 20 * ent->damage, 300, ent, MOD_AMMO, qfalse ) ) {
		if ( ent->parent && ent->parent->client ) {
			ent->parent->client->accuracy_hits++;
		}
	}
	ent->nextthink = level.time + 1;
}

/*
===============
Lightning_Ammo_Expl
===============
*/
void Lightning_Ammo_Expl( gentity_t *ammo, gentity_t *attacker, int quadFactor ) {
	gentity_t	*ent;

	ent = G_Spawn();
	ent->classname = "lightningammoexploder";
	ent->s.eType = ET_LIGHTNING_AMMO_EXPL;
	ent->nextthink = level.time + 1;
	ent->think = Lightning_Ammo_Expl_Think;
	ent->lastthinktime = level.time + 500;
	ent->lastthink = G_FreeEntity;
	ent->r.ownerNum = attacker->s.number;
	ent->parent = attacker;
	ent->target_ent = NULL;
	ent->s.generic1 = ammo->item->quantity * 2;
	ent->s.eventParm = GetUniqueId();
	ent->s.pos.trType = TR_STATIONARY;
	ent->damage = quadFactor;
	ent->s.time = level.time;
	VectorCopy( ammo->r.currentOrigin, ent->s.origin );
	VectorCopy( ammo->r.currentOrigin, ent->r.currentOrigin );
	VectorCopy( ammo->r.currentOrigin, ent->s.pos.trBase );
	trap_LinkEntity( ent );
}

//======================================================================
//NT - ammo needed a death function - this is it
/*
===============
Ammo_Die
===============
*/
void Ammo_Die( gentity_t *ent, gentity_t *inflictor, gentity_t *attacker, int damage, int mod ) {
	vec3_t		dir, origin;
	gentity_t	*tent;
	int			respawn = RESPAWN_AMMO;
	int			quadFactor;

	// we don't have a valid direction, so just point straight up
	dir[0] = dir[1] = 0;
	dir[2] = 1;

	// wait of -1 will not respawn
	if ( ent->wait == -1 ) {
		ent->r.svFlags |= SVF_NOCLIENT;
		ent->s.eFlags |= EF_NODRAW;
		ent->r.contents = 0;
		ent->takedamage = qfalse;
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

	// picked up items still stay around, they just don't
	// draw anything.  This allows respawnable items
	// to be placed on movers.
	ent->r.svFlags |= SVF_NOCLIENT;
	ent->s.eFlags |= EF_NODRAW;
	ent->r.contents = 0;
	ent->takedamage = qfalse;


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

	if ( ent->item->giTag == WP_ROCKET_LAUNCHER ||
		ent->item->giTag == WP_BFG ||
		ent->item->giTag == WP_PLASMAGUN ||
		ent->item->giTag == WP_GRENADE_LAUNCHER ) {
		tent = G_TempEntity( ent->r.currentOrigin, EV_MISSILE_MISS );
		tent->s.eventParm = DirToByte( dir );
		tent->s.generic1 = 32;
		if ( ent->item->giTag == WP_PLASMAGUN ) {
			tent->s.weapon = WP_BFG;
		} else {
			tent->s.weapon = ent->item->giTag;
		}
	} else if ( ent->item->giTag == WP_MACHINEGUN ||
		ent->item->giTag == WP_SHOTGUN ||
		ent->item->giTag == WP_LIGHTNING ||
		ent->item->giTag == WP_RAILGUN ) {
		VectorCopy( ent->r.currentOrigin, origin );
		origin[2] -= 16;
		tent = G_TempEntity( origin, EV_MISSILE_MISS );
		tent->s.eventParm = DirToByte( dir );
		tent->s.generic1 = 12;
		tent->s.weapon = WP_ROCKET_LAUNCHER;
	}

	if (attacker->client && attacker->client->ps.powerups[PW_QUAD] ) {
		G_AddEvent( attacker, EV_POWERUP_QUAD, 0 );
		quadFactor = g_quadfactor.value;
	} else {
		quadFactor = 1;
	}

	if ( ent->splashDamage ) {
		if( G_RadiusDamage( ent->r.currentOrigin, attacker, ent->splashDamage * quadFactor,
			ent->splashRadius, ent, ent->splashMethodOfDeath, qtrue ) ) {
			if ( attacker && attacker->client ) {
				attacker->client->accuracy_hits++;
			}
		}
	}

	switch ( ent->item->giTag ) {
	case WP_MACHINEGUN:
		Machinegun_Ammo_Expl( ent, attacker, quadFactor );
		break;
	case WP_SHOTGUN:
		Shotgun_Ammo_Expl( ent, attacker, quadFactor );
		break;
	case WP_LIGHTNING:
		Lightning_Ammo_Expl( ent, attacker, quadFactor );
		break;
	case WP_RAILGUN:
		Railgun_Ammo_Expl( ent, attacker, quadFactor );
		break;
	}
}

//======================================================================

/*
===============
SetupAmmo
===============
*/
void SetupAmmo( gentity_t *ent ) {
	ent->r.contents |= CONTENTS_SHOOTABLE;
	ent->takedamage = qtrue;
	ent->health = 100;
	ent->die = Ammo_Die;
	ent->splashMethodOfDeath = MOD_AMMO;

	switch ( ent->item->giTag ) {
	case WP_GRENADE_LAUNCHER:
		ent->splashDamage = 200;
		ent->splashRadius = 400;
		break;
	case WP_BFG:
		ent->splashDamage = 300;
		ent->splashRadius = 320;
		break;
	case WP_PLASMAGUN:
	case WP_ROCKET_LAUNCHER:
		ent->splashDamage = 200;
		ent->splashRadius = 320;
		break;
	default:
		ent->splashDamage = 75;
		ent->splashRadius = 150;
		break;
	}
}

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
	
	//NT - if the item is ammo, set it up for explosions
	if ( ent->item->giType == IT_AMMO && g_explodingAmmo.integer == 1 ) {
		SetupAmmo( ent );
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

	// the same pickup rules are used for client side and server side
	if ( !BG_CanItemBeGrabbed( g_gametype.integer, &ent->s, &other->client->ps ) ) {
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
	case IT_ARMOR:
		respawn = Pickup_Armor(ent, other);
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
	if ( ent->item->giType == IT_POWERUP || ent->item->giType == IT_TEAM
		//NT - hologram holdable has a global pickup sound now
		|| ( ent->item->giType == IT_HOLDABLE && ent->item->giTag == HI_HOLOGRAM ) ) {
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
		//NT - setting this avoids infinite recursion (two ammo boxes blowing each other up)
		ent->takedamage = qfalse;
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
	//NT - setting this avoids infinite recursion (two ammo boxes blowing each other up)
	ent->takedamage = qfalse;


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

	//NT - alternating powerups
	if ( ent->item->giType == IT_POWERUP ) {
		if ( ent->item->giTag == PW_QUAD ) {
			ent->item = BG_FindItemForPowerup( PW_ACCURACY );
			ent->s.modelindex = ent->item - bg_itemlist;		// store item number in modelindex
		} else if ( ent->item->giTag == PW_ACCURACY ) {
			ent->item = BG_FindItemForPowerup( PW_QUAD );
			ent->s.modelindex = ent->item - bg_itemlist;		// store item number in modelindex
		} else if ( ent->item->giTag == PW_HASTE ) {
			ent->item = BG_FindItemForPowerup( PW_SPEED );
			ent->s.modelindex = ent->item - bg_itemlist;		// store item number in modelindex
		} else if ( ent->item->giTag == PW_SPEED ) {
			ent->item = BG_FindItemForPowerup( PW_HASTE );
			ent->s.modelindex = ent->item - bg_itemlist;		// store item number in modelindex
		} else if ( ent->item->giTag == PW_REGEN ) {
			ent->item = BG_FindItemForPowerup( PW_VITALITY );
			ent->s.modelindex = ent->item - bg_itemlist;		// store item number in modelindex
		} else if ( ent->item->giTag == PW_VITALITY ) {
			ent->item = BG_FindItemForPowerup( PW_REGEN );
			ent->s.modelindex = ent->item - bg_itemlist;		// store item number in modelindex
		} else if ( ent->item->giTag == PW_INVIS ) {
			ent->item = BG_FindItemForHoldable( HI_HOLOGRAM );
			ent->s.modelindex = ent->item - bg_itemlist;		// store item number in modelindex
		}
	} else if ( ent->item->giType == IT_HOLDABLE ) {
		if ( ent->item->giTag == HI_HOLOGRAM ) {
			// if this was alternating with a teleporter, generic1 will have something in it 
			if (ent->s.generic1 > 0 ) {
				// put in the teleporter and reset generic1
				ent->item = BG_FindItemForHoldable( HI_TELEPORTER );
				ent->s.modelindex = ent->item - bg_itemlist;		// store item number in modelindex
				ent->s.generic1 = 0;
			} else {
				// it was alternating with invisibility
				ent->item = BG_FindItemForPowerup( PW_INVIS );
				ent->s.modelindex = ent->item - bg_itemlist;		// store item number in modelindex
			}
		} else if ( ent->item->giTag == HI_TELEPORTER ) {
			// is it time to put in the hologram? (every ninth spawn)
			if ( ent->s.generic1 == 7 ) {
				// yep, so put one in
				ent->item = BG_FindItemForHoldable( HI_HOLOGRAM );
				ent->s.modelindex = ent->item - bg_itemlist;		// store item number in modelindex
			} else {
				ent->s.generic1++;
			}
		}
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
gentity_t *LaunchItem( gitem_t *item, vec3_t origin, vec3_t velocity ) {
	gentity_t	*dropped;

	dropped = G_Spawn();

	dropped->s.eType = ET_ITEM;
	dropped->s.modelindex = item - bg_itemlist;	// store item number in modelindex
	dropped->s.modelindex2 = 1; // This is non-zero is it's a dropped item

	dropped->classname = item->classname;
	dropped->item = item;
	VectorSet (dropped->r.mins, -ITEM_RADIUS, -ITEM_RADIUS, -ITEM_RADIUS);
	VectorSet (dropped->r.maxs, ITEM_RADIUS, ITEM_RADIUS, ITEM_RADIUS);
	dropped->r.contents = CONTENTS_TRIGGER;

	dropped->touch = Touch_Item;

	G_SetOrigin( dropped, origin );
	dropped->s.pos.trType = TR_GRAVITY;
	dropped->s.pos.trTime = level.time;
	VectorCopy( velocity, dropped->s.pos.trDelta );

	dropped->s.eFlags |= EF_BOUNCE_HALF;
#ifdef MISSIONPACK
	if ((g_gametype.integer == GT_CTF || g_gametype.integer == GT_1FCTF)			&& item->giType == IT_TEAM) { // Special case for CTF flags
#else
	if (g_gametype.integer == GT_CTF && item->giType == IT_TEAM) { // Special case for CTF flags
#endif
		dropped->think = Team_DroppedFlagThink;
		dropped->nextthink = level.time + 30000;
		Team_CheckDroppedItem( dropped );
	} else { // auto-remove after 30 seconds
		dropped->think = G_FreeEntity;
		dropped->nextthink = level.time + 30000;
		//NT - what a kludge; just wanting to make the item shrink when it's time to go
		// so I'm abusing apos.trTime to do it (see cg_items.c/CG_Item)
		dropped->s.apos.trTime = level.time + 30000;
	}

	dropped->flags = FL_DROPPED_ITEM;

	trap_LinkEntity (dropped);

	return dropped;
}

/*
================
Drop_Item

Spawns an item and tosses it forward
================
*/
gentity_t *Drop_Item( gentity_t *ent, gitem_t *item, float angle ) {
	vec3_t	velocity;
	vec3_t	angles;

	VectorCopy( ent->s.apos.trBase, angles );
	angles[YAW] += angle;
	angles[PITCH] = 0;	// always forward

	AngleVectors( angles, velocity, NULL, NULL );
	VectorScale( velocity, 150, velocity );
	velocity[2] += 200 + crandom() * 50;
	
	return LaunchItem( item, ent->s.pos.trBase, velocity );
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

	//NT - make ammo a bit bigger if it's explodable
	if ( ent->item->giTag == IT_AMMO && g_explodingAmmo.integer == 1 ) {
		VectorSet( ent->r.mins, -ITEM_RADIUS - 2, -ITEM_RADIUS - 2, -ITEM_RADIUS );
		VectorSet( ent->r.maxs, ITEM_RADIUS + 2, ITEM_RADIUS + 2, ITEM_RADIUS + 8 );
	} else {
		VectorSet( ent->r.mins, -ITEM_RADIUS, -ITEM_RADIUS, -ITEM_RADIUS );
		VectorSet( ent->r.maxs, ITEM_RADIUS, ITEM_RADIUS, ITEM_RADIUS );
	}

	ent->s.eType = ET_ITEM;
	ent->s.modelindex = ent->item - bg_itemlist;		// store item number in modelindex
	ent->s.modelindex2 = 0; // zero indicates this isn't a dropped item

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
	
	//NT - set ammo up to be explodable
	if ( ent->item->giType == IT_AMMO && g_explodingAmmo.integer == 1) {
		SetupAmmo( ent );
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

	if( g_gametype.integer == GT_CTF ) {
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
#endif
}

/*
==============
ClearRegisteredItems
==============
*/
void ClearRegisteredItems( void ) {
	memset( itemRegistered, 0, sizeof( itemRegistered ) );

	// players always start with the base weapon
	RegisterItem( BG_FindItemForWeapon( WP_MACHINEGUN ) );
	RegisterItem( BG_FindItemForWeapon( WP_GAUNTLET ) );
#ifdef MISSIONPACK
	if( g_gametype.integer == GT_HARVESTER ) {
		RegisterItem( BG_FindItem( "Red Cube" ) );
		RegisterItem( BG_FindItem( "Blue Cube" ) );
	}
#endif
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

	RegisterItem( item );
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
		//NT - SMALL_GRAVITY trajectoried objects need a ground check
		if ( ent->s.pos.trType != TR_GRAVITY && ent->s.pos.trType != TR_SMALL_GRAVITY ) {
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
		if (ent->item && ent->item->giType == IT_TEAM) {
			Team_FreeEntity(ent);
		} else {
			G_FreeEntity( ent );
		}
		return;
	}

	G_BounceItem( ent, &tr );
}

