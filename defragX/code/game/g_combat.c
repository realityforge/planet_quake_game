// Copyright (C) 1999-2000 Id Software, Inc.
//
// g_combat.c

#include "g_local.h"
#include "bg_shared.h"

/*
============
RespawnAllPlayers()

Forces a respawn of all playing clients in teamrun
============
*/
void RespawnAllPlayers() {
	int i;
	gentity_t *client;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].pers.connected != CON_DISCONNECTED
			&& level.clients[i].pers.connected != CON_CONNECTING
			&& level.clients[i].sess.sessionTeam != TEAM_FREE
			&& level.clients[i].sess.sessionTeam != TEAM_SPECTATOR) {
			// found a client
			client = g_entities + i;
			client->client->ps.pm_type = PM_NORMAL;
			client->client->sess.spectatorState = SPECTATOR_NOT;
			client->takedamage = qtrue;
			ClientSpawn(client);
		}
	}
}

/*
============
AddScore

Adds score (frags) to the client
============
*/
void AddScore( gentity_t *ent, vec3_t origin, int score )
{
	if ( !ent->client )
		return;
	
	ent->client->ps.persistant[PERS_SCORE] += score;

	if ( dfx_gametype.integer == GT_TEAMRUN  ) {
		level.teamScores[ ent->client->ps.persistant[PERS_TEAM] ] += score;
	}
	
	CalculateRanks();
}

/*
============
AddKey

Adds a (key) to the client
============
*/
void AddKey( gentity_t *ent )
{
	if ( !ent->client )
		return;

	ent->client->ps.persistant[PERS_KEYS] += 1;
}

/*
==================
GibEntity
==================
*/
void GibEntity( gentity_t *self, int killer )
{
	G_AddEvent( self, EV_GIB_PLAYER, killer );
	self->takedamage = qfalse;
	self->s.eType = ET_INVISIBLE;
	self->r.contents = 0;
}

/*
==================
body_die
==================
*/
void body_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath )
{
	if ( self->health > GIB_HEALTH )
		return;

	if ( !g_blood.integer ) {
		self->health = GIB_HEALTH+1;
		return;
	}

	GibEntity( self, 0 );
}

// these are just for logging, the client prints its own messages
char *modNames[] = {
	"MOD_UNKNOWN",
	"MOD_SHOTGUN",
	"MOD_GAUNTLET",
	"MOD_MACHINEGUN",
	"MOD_GRENADE",
	"MOD_GRENADE_SPLASH",
	"MOD_ROCKET",
	"MOD_ROCKET_SPLASH",
	"MOD_PLASMA",
	"MOD_PLASMA_SPLASH",
	"MOD_RAILGUN",
	"MOD_LIGHTNING",
	"MOD_BFG",
	"MOD_BFG_SPLASH",
	"MOD_WATER",
	"MOD_SLIME",
	"MOD_LAVA",
	"MOD_CRUSH",
	"MOD_TELEFRAG",
	"MOD_FALLING",
	"MOD_SUICIDE",
	"MOD_TARGET_LASER",
	"MOD_TRIGGER_HURT",
	"MOD_GRAPPLE"
};

/*
==================
player_die
==================
*/
void player_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath )
{
	gentity_t	*ent;
	int			anim;
	int			contents;
	int			killer;
	int			i;
	char		*killerName, *obit;

	extern int  pm_minrespawntime;

	// freeze player when dead in teamrun
	if (dfx_gametype.integer == GT_TEAMRUN) {
		self->client->ps.pm_type = PM_FREEZE;
		return;
	}
	
	if ( self->client->ps.pm_type == PM_DEAD || level.intermissiontime )
		return;

	//unlag the client
	G_UnTimeShiftClient( self );

	if (self->client && self->client->hook)
		Weapon_HookFree(self->client->hook);

	self->client->ps.pm_type = PM_DEAD;

	if ( attacker ) {
		killer = attacker->s.number;
		if ( attacker->client )
			killerName = attacker->client->pers.netname;
		else
			killerName = "<non-client>";

	} else {
		killer = ENTITYNUM_WORLD;
		killerName = "<world>";
	}

	if ( killer < 0 || killer >= MAX_CLIENTS ) {
		killer = ENTITYNUM_WORLD;
		killerName = "<world>";
	}

	if ( (unsigned)meansOfDeath >= ARRAY_LEN( modNames ) ) {
		obit = "<bad obituary>";
	} else {
		obit = modNames[ meansOfDeath ];
	}

	G_LogPrintf("Kill: %i %i %i: %s killed %s by %s\n",	killer, self->s.number, meansOfDeath, killerName, self->client->pers.netname, obit );

	// if I committed suicide, the flag does not fall, it returns.
	/*
	if (meansOfDeath == MOD_SUICIDE) {

		if ( self->client->ps.powerups[PW_REDFLAG] ) {		// only happens in standard CTF
			Team_ReturnFlag( TEAM_RED );
			self->client->ps.powerups[PW_REDFLAG] = 0;
		}
		else if ( self->client->ps.powerups[PW_BLUEFLAG] ) {	// only happens in standard CTF
			Team_ReturnFlag( TEAM_BLUE );
			self->client->ps.powerups[PW_BLUEFLAG] = 0;
		}
	}
    */

	Cmd_Score_f( self );	// show scores

	// send updated scores to any clients that are following this one,
	// or they would get stale scoreboards
	for ( i = 0 ; i < level.maxclients ; i++ )
    {
		gclient_t	*client;
		client = &level.clients[i];

		if ( client->pers.connected != CON_CONNECTED )
			continue;

		if ( client->sess.sessionTeam != TEAM_SPECTATOR )
			continue;

		if ( client->sess.spectatorClient == self->s.number )
			Cmd_Score_f( g_entities + i );
	}

	self->takedamage = qfalse; // can't be gibbed

	self->s.weapon = WP_NONE;
	self->s.powerups = 0;
	self->r.contents = CONTENTS_CORPSE;

	self->s.angles[0] = 0;
	self->s.angles[2] = 0;

	VectorCopy( self->s.angles, self->client->ps.viewangles );

	self->s.loopSound = 0;

	self->r.maxs[2] = -8;

	// don't allow respawn until the death anim is done
	// g_forcerespawn may force spawning at some later time
	if (dfx_gametype.integer == GT_ADVENTURE)
        self->client->respawnTime = level.time + 3000;
    else
        self->client->respawnTime = level.time + pm_minrespawntime;
		
	if (dfx_gametype.integer == GT_TEAMRUN)  {
		qbool lastOne = qtrue;
		for ( i = 0 ; i < level.maxclients ; i++ ) {
			if ( level.clients[i].pers.connected != CON_DISCONNECTED
				&& level.clients[i].pers.connected != CON_CONNECTING
				&& level.clients[i].sess.sessionTeam == self->client->sess.sessionTeam) {
				if (level.clients[i].ps.stats[STAT_HEALTH] > 0) {
					lastOne = qfalse;
				}
			}
		}
		if (lastOne) {
			if ( attacker && attacker != self && !(OnSameTeam (self, attacker ))) {
				// attacker scores
				level.teamScores[attacker->client->sess.sessionTeam]++;
			} else {
				// self unscores..
				level.teamScores[self->client->sess.sessionTeam]--;
			}
			CalculateRanks();
			// Respawn everyone.
			RespawnAllPlayers( 1 );

		} else {
			// all clients are respawned in next round
			self->client->respawnTime = INT_MAX;
			if (dfx_gametype.integer == GT_TEAMRUN) {
				self->client->sess.spectatorState = SPECTATOR_FREE;
				self->client->ps.pm_type = PM_SPECTATOR;
			} else {
			//	self->client->sess.spectatorState = SPECTATOR_FREE;
			}
		}
	}

	// remove powerups
	memset( self->client->ps.powerups, 0, sizeof(self->client->ps.powerups) );

	// never gib in a nodrop
	if ( (self->health <= GIB_HEALTH && !(contents & CONTENTS_NODROP) && g_blood.integer) || meansOfDeath == MOD_SUICIDE)
    {
		// gib death
		GibEntity( self, killer );
	}
	else
    {
		// normal death
		static int i;

		switch ( i ) {
		case 0:
			anim = BOTH_DEATH1;
			break;
		case 1:
			anim = BOTH_DEATH2;
			break;
		case 2:
		default:
			anim = BOTH_DEATH3;
			break;
		}

		// for the no-blood option, we need to prevent the health
		// from going to gib level
		if ( self->health <= GIB_HEALTH )
			self->health = GIB_HEALTH + 1;

		self->client->ps.legsAnim =	 ( ( self->client->ps.legsAnim &  ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;
		self->client->ps.torsoAnim = ( ( self->client->ps.torsoAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;

		G_AddEvent( self, EV_DEATH1 + i, killer );

		// the body can still be gibbed
		self->die = body_die;

		// globally cycle through the different death animations
		i = ( i + 1 ) % 3;

	}

	trap_LinkEntity (self);
}


/*
================
CheckArmor
================
*/
int CheckArmor (gentity_t *ent, int damage, int dflags)
{
	gclient_t	*client;
	int			save;
	int			count;

	if (!damage)
		return 0;

	client = ent->client;

	if (!client)
		return 0;

	if (dflags & DAMAGE_NO_ARMOR)
		return 0;

	// armor
	count = client->ps.stats[STAT_ARMOR];
	save = ceil( damage * ARMOR_PROTECTION );
	if (save >= count)
		save = count;

	if (!save)
		return 0;

	client->ps.stats[STAT_ARMOR] -= save;

	return save;
}


/*
============
G_Damage

targ		entity that is being damaged
inflictor	entity that is causing the damage
attacker	entity that caused the inflictor to damage targ
	example: targ=monster, inflictor=rocket, attacker=player

dir			direction of the attack for knockback
point		point at which the damage is being inflicted, used for headshots
damage		amount of damage being inflicted
knockback	force to be applied against targ as a result of the damage

inflictor, attacker, dir, and point can be NULL for environmental effects

dflags		these flags are used to control how T_Damage works
	DAMAGE_RADIUS			damage was indirect (from a nearby explosion)
	DAMAGE_NO_ARMOR			armor does not protect from this damage
	DAMAGE_NO_KNOCKBACK		do not affect velocity, just view angles
	DAMAGE_NO_PROTECTION	kills godmode, armor, everything
============
*/

void G_Damage( gentity_t *targ, gentity_t *inflictor, gentity_t *attacker, vec3_t dir, vec3_t point, int damage, int dflags, int mod )
{
	gclient_t	*client;
	int			take, asave, knockback, max;

    if( (!dfx_interference.integer) && (targ->client != NULL) && (attacker && attacker != targ) && (attacker->client != NULL) && (targ->s.eType != ET_MOVER) )
        return;

	if (!targ->takedamage)
		return;

	// the intermission has allready been qualified for, so don't
	// allow any extra scoring
	if ( level.intermissionQueued )
		return;

	if ( !inflictor )
		inflictor = &g_entities[ENTITYNUM_WORLD];

	if ( !attacker )
		attacker = &g_entities[ENTITYNUM_WORLD];

	// shootable doors / buttons don't actually have any health
	if ( targ->s.eType == ET_MOVER )
    {
		if ( targ->use && (targ->moverState == MOVER_POS1 || targ->moverState == ROTATOR_POS1) )
			targ->use( targ, inflictor, attacker );

		return;
	}

	// reduce damage by the attacker's handicap value
	// unless they are rocket jumping
	if ( attacker->client && attacker != targ ) {
		max = attacker->client->ps.stats[STAT_MAX_HEALTH];
		damage = damage * max / 100;
	}

	client = targ->client;

	if ( (client) && client->noclip )
        return;

	if ( !dir )
		dflags |= DAMAGE_NO_KNOCKBACK;
	 else
		VectorNormalize(dir);

	knockback = damage;
	if ( knockback > 200 )
		knockback = 200;

	if ( targ->flags & FL_NO_KNOCKBACK )
		knockback = 0;

	if ( dflags & DAMAGE_NO_KNOCKBACK )
		knockback = 0;

	// figure momentum add, even if the damage won't be taken
	if ( knockback && targ->client )
    {
		vec3_t	kvel;
		float	mass;

		mass = 200;

		VectorScale (dir, g_knockback.value * (float)knockback / mass, kvel);
		VectorAdd (targ->client->ps.velocity, kvel, targ->client->ps.velocity);

		// set the timer so that the other client can't cancel
		// out the movement immediately
		if ( !targ->client->ps.pm_time )
        {
			int		t;

			t = knockback * 2;
			if ( t < 50 )
				t = 50;

			if ( t > 200 )
				t = 200;

			targ->client->ps.pm_time = t;
			targ->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
		}
	}

	// check for completely getting out of the damage
	if ( !(dflags & DAMAGE_NO_PROTECTION) ) {
		// check for godmode
		if ( targ->flags & FL_GODMODE )
			return;
	}

	// battlesuit protects from all radius damage (but takes knockback)
	// and protects 50% against all damage
	if ( client && client->ps.powerups[PW_BATTLESUIT] )
    {
		G_AddEvent( targ, EV_POWERUP_BATTLESUIT, 0 );
		if ( ( dflags & DAMAGE_RADIUS ) || ( mod == MOD_FALLING ) )
			return;

		// SLK: no self damage in fastcap and freestyle gametype, no selfdamage in other defrag modes than default one
        if ( (dfx_gametype.integer == GT_FASTCAP || dfx_gametype.integer == GT_FREESTYLE) || dfx_gametype.integer == GT_DEFRAG && dfx_mode.integer >= DF_STRAFE )
            damage = 0;
        else
            damage *= 0.5;
	}

	// always give half damage if hurting self
	// calculated after knockback, so rocket jumping works
	if ( targ == attacker)
        // SLK: no self damage in fastcap and freestyle gametype, no selfdamage in other defrag modes than default one
        if ( (dfx_gametype.integer == GT_FASTCAP || dfx_gametype.integer == GT_FREESTYLE) || dfx_gametype.integer == GT_DEFRAG && dfx_mode.integer >= DF_STRAFE )
            damage = 0;
        else
            damage *= 0.5;

	if ( damage < 1 && damage != 0)
		damage = 1;

	take = damage;

	// save some from armor
	asave = CheckArmor( targ, take, dflags );
	take -= asave;

	// add to the damage inflicted on a player this frame
	// the total will be turned into screen blends and view angle kicks
	// at the end of the frame
	if ( client ) {
		if ( attacker ) { // FIXME: always true?
			client->ps.persistant[PERS_ATTACKER] = attacker->s.number;
		} else {
			client->ps.persistant[PERS_ATTACKER] = ENTITYNUM_WORLD;
		}

		client->damage_armor += asave;
		client->damage_blood += take;
		client->damage_knockback += knockback;

		if ( dir ) {
			VectorCopy ( dir, client->damage_from );
			client->damage_fromWorld = qfalse;
		} else {
			VectorCopy ( targ->r.currentOrigin, client->damage_from );
			client->damage_fromWorld = qtrue;
		}
	}

	if (targ->client) {
		// set the last client who damaged the target
		targ->client->lasthurt_client = attacker->s.number;
		targ->client->lasthurt_mod = mod;
	}

	// do the damage
	if (take ) {
		targ->health = targ->health - take;

		if ( targ->client )
			targ->client->ps.stats[STAT_HEALTH] = targ->health;

		if ( targ->health <= 0 ) {
			if ( client )
				targ->flags |= FL_NO_KNOCKBACK;

			if (targ->health < -999)
				targ->health = -999;

			targ->enemy = attacker;
			targ->die (targ, inflictor, attacker, take, mod);
			return;

		} else if ( targ->pain ) {
			targ->pain (targ, attacker, take);
		}
	}

}


/*
============
CanDamage

Returns qtrue if the inflictor can directly damage the target.  Used for
explosions and melee attacks.
============
*/
qbool CanDamage( gentity_t *targ, vec3_t origin )
{
	//we check if the attacker can damage the target, return qtrue if yes, qfalse if no
	vec3_t	dest;
	trace_t	tr;
	vec3_t	midpoint;
	vec3_t	size;

	// use the midpoint of the bounds instead of the origin, because bmodels may have their origin 0,0,0
	VectorAdd (targ->r.absmin, targ->r.absmax, midpoint);
	VectorScale( midpoint, 0.5, dest );

	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0 || tr.entityNum == targ->s.number)
		return qtrue;

	VectorSubtract( targ->r.absmax, targ->r.absmin, size );

	// top quad

	// - +
	// - -
	VectorCopy( targ->r.absmax, dest );
	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	// + -
	// - -
	dest[0] -= size[0];
	trap_Trace( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID );
	if ( tr.fraction == 1.0 )
		return qtrue;

	// - -
	// + -
	dest[1] -= size[1];
	trap_Trace( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID );
	if ( tr.fraction == 1.0 )
		return qtrue;

	// - -
	// - +
	dest[0] += size[0];
	trap_Trace( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID );
	if ( tr.fraction == 1.0 )
		return qtrue;

	// bottom quad

	// - -
	// + -
	VectorCopy( targ->r.absmin, dest );
	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	// - -
	// - +
	dest[0] += size[0];
	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	// - +
	// - -
	dest[1] += size[1];
	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	// + -
	// - -
	dest[0] -= size[0];
	trap_Trace( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID );
	if ( tr.fraction == 1.0 )
		return qtrue;

	return qfalse;
}


/*
============
G_RadiusDamage
============
*/
qbool G_RadiusDamage ( vec3_t origin, gentity_t *attacker, float damage, float radius, gentity_t *ignore, int mod)
{
	float		points, dist;
	gentity_t	*ent;
	int			entityList[MAX_GENTITIES], numListedEntities, i, e;
	vec3_t		mins, maxs, v, dir;
	qbool	    hitClient = qfalse;
	extern float pm_knockback_z;

	if ( radius < 1 )
		radius = 1;

	for ( i = 0 ; i < 3 ; i++ )
    {
		mins[i] = origin[i] - radius;
		maxs[i] = origin[i] + radius;
	}

	numListedEntities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

	for ( e = 0 ; e < numListedEntities ; e++ )
    {
		ent = &g_entities[entityList[ e ]];

		if (ent == ignore || !ent->takedamage)
			continue;

		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ )
        {
			if ( origin[i] < ent->r.absmin[i] ) {
				v[i] = ent->r.absmin[i] - origin[i];
			} else if ( origin[i] > ent->r.absmax[i] ) {
				v[i] = origin[i] - ent->r.absmax[i];
			} else {
				v[i] = 0;
			}
		}

		dist = VectorLength( v );
		if ( dist >= radius )
			continue;

		points = damage * ( 1.0 - dist / radius );

		if( CanDamage (ent, origin) )
        {

			VectorSubtract (ent->r.currentOrigin, origin, dir);
			// push the center of mass higher than the origin so players
			// get knocked into the air more
			if (ent == attacker) { // rjumps are same as in normal Q3A
				dir[2] +=24;
			} else {
				dir[2] += pm_knockback_z;
			}
			G_Damage (ent, NULL, attacker, dir, origin, (int)points, DAMAGE_RADIUS, mod);
		}
	}

	return hitClient;
}
