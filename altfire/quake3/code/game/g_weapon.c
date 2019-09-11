// Copyright (C) 1999-2000 Id Software, Inc.
//
// g_weapon.c 
// perform the server side effects of a weapon firing

#include "g_local.h"

static	float	s_quadFactor;
static	vec3_t	forward, right, up;
static	vec3_t	muzzle;

#define NUM_NAILSHOTS 15

/*
================
G_BounceProjectile
================
*/
void G_BounceProjectile( vec3_t start, vec3_t impact, vec3_t dir, vec3_t endout ) {
	vec3_t v, newv;
	float dot;

	VectorSubtract( impact, start, v );
	dot = DotProduct( v, dir );
	VectorMA( v, -2*dot, dir, newv );

	VectorNormalize(newv);
	VectorMA(impact, 8192, newv, endout);
}


/*
======================================================================

GAUNTLET

======================================================================
*/

void Weapon_Gauntlet( gentity_t *ent ) {

}

/*
===============
CheckGauntletAttack
===============
*/
qboolean CheckGauntletAttack( gentity_t *ent ) {
	trace_t		tr;
	vec3_t		end;
	gentity_t	*tent;
	gentity_t	*traceEnt;
	int			damage;

	// set aiming directions
	AngleVectors (ent->client->ps.viewangles, forward, right, up);

	CalcMuzzlePoint ( ent, forward, right, up, muzzle );

	VectorMA (muzzle, 32, forward, end);

	trap_Trace (&tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT);
	if ( tr.surfaceFlags & SURF_NOIMPACT ) {
		return qfalse;
	}

	traceEnt = &g_entities[ tr.entityNum ];

	// send blood impact
	if ( traceEnt->takedamage && traceEnt->client ) {
		tent = G_TempEntity( tr.endpos, EV_MISSILE_HIT );
		tent->s.otherEntityNum = traceEnt->s.number;
		tent->s.eventParm = DirToByte( tr.plane.normal );
		tent->s.weapon = ent->s.weapon;
	}

	if ( !traceEnt->takedamage) {
		return qfalse;
	}

	if (ent->client->ps.powerups[PW_QUAD] ) {
		G_AddEvent( ent, EV_POWERUP_QUAD, 0 );
		s_quadFactor = g_quadfactor.value;
	} else {
		s_quadFactor = 1;
	}
#ifdef MISSIONPACK
	if( ent->client->persistantPowerup && ent->client->persistantPowerup->item && ent->client->persistantPowerup->item->giTag == PW_DOUBLER ) {
		s_quadFactor *= 2;
	}
#endif

	damage = 50 * s_quadFactor;
	//NT - return qtrue if there was any damage given
	return G_Damage( traceEnt, ent, ent, forward, tr.endpos,
		damage, 0, MOD_GAUNTLET );
}


/*
======================================================================

MACHINEGUN

======================================================================
*/

/*
======================
SnapVectorTowards

Round a vector to integers for more efficient network
transmission, but make sure that it rounds towards a given point
rather than blindly truncating.  This prevents it from truncating 
into a wall.
======================
*/
void SnapVectorTowards( vec3_t v, vec3_t to ) {
	int		i;

	for ( i = 0 ; i < 3 ; i++ ) {
		if ( to[i] <= v[i] ) {
			v[i] = (int)v[i];
		} else {
			v[i] = (int)v[i] + 1;
		}
	}
}

//NT - moved some defines to g_local.h

//NT - added a_* parameters, quadfactor, hurtOwner, and mod because this may be called
// for an ammo box projectile and not just a player's machinegun bullet
void Bullet_Fire (gentity_t *ent, float spread, vec3_t a_muzzle, vec3_t a_forward,
				  vec3_t a_up, vec3_t a_right, qboolean hurtOwner, int damage, int mod, int quadFactor) {
	trace_t		tr;
	vec3_t		end;
#ifdef MISSIONPACK
	vec3_t		impactpoint, bouncedir;
#endif
	float		r;
	float		u;
	gentity_t	*tent;
	gentity_t	*traceEnt;
	int			i, passent;

	damage *= quadFactor;

	r = random() * M_PI * 2.0f;
	u = sin(r) * crandom() * spread * 16;
	r = cos(r) * crandom() * spread * 16;
	VectorMA (a_muzzle, 8192*16, a_forward, end);
	VectorMA (end, r, a_right, end);
	VectorMA (end, u, a_up, end);

	if ( hurtOwner )
		passent = ENTITYNUM_NONE;
	else
		passent = ent->s.number;

	for (i = 0; i < 10; i++) {

		trap_Trace (&tr, a_muzzle, NULL, NULL, end, passent, MASK_SHOT);
		if ( tr.surfaceFlags & SURF_NOIMPACT ) {
			return;
		}

		traceEnt = &g_entities[ tr.entityNum ];

		// snap the endpos to integers, but nudged towards the line
		SnapVectorTowards( tr.endpos, a_muzzle );

		// send bullet impact
		if ( traceEnt->takedamage && traceEnt->client ) {
			tent = G_TempEntity( tr.endpos, EV_BULLET_HIT_FLESH );
			tent->s.eventParm = traceEnt->s.number;
			if( LogAccuracyHit( traceEnt, ent ) ) {
				ent->client->accuracy_hits++;
			}
		} else {
			tent = G_TempEntity( tr.endpos, EV_BULLET_HIT_WALL );
			tent->s.eventParm = DirToByte( tr.plane.normal );
		}
		tent->s.otherEntityNum = ent->s.number;

		if ( traceEnt->takedamage) {
#ifdef MISSIONPACK
			if ( traceEnt->client && traceEnt->client->invulnerabilityTime > level.time ) {
				if (G_InvulnerabilityEffect( traceEnt, a_forward, tr.endpos, impactpoint, bouncedir )) {
					G_BounceProjectile( a_muzzle, impactpoint, bouncedir, end );
					VectorCopy( impactpoint, a_muzzle );
					// the player can hit him/herself with the bounced rail
					passent = ENTITYNUM_NONE;
				}
				else {
					VectorCopy( tr.endpos, a_muzzle );
					passent = traceEnt->s.number;
				}
				continue;
			}
			else {
#endif
				G_Damage( traceEnt, ent, ent, a_forward, tr.endpos,
					damage, 0, mod );
#ifdef MISSIONPACK
			}
#endif
		}
		break;
	}
}


/*
======================================================================

BFG

======================================================================
*/

void BFG_Fire ( gentity_t *ent ) {
	gentity_t	*m;

	m = fire_bfg (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics

	//NT - BFG overheat code
	if ( ent->client->ps.stats[STAT_BFG_OVERHEAT] > 1600 ) {
		m->nextthink = level.time + 1;
		m->s.pos.trType = TR_STATIONARY;
		m->s.pos.trTime = level.time;
		m->damage *= 10;
		m->splashDamage *= 10;
		m->methodOfDeath = MOD_BFG_OVERHEAT;
		m->splashMethodOfDeath = MOD_BFG_OVERHEAT;
		m->splashRadius *= 2;

		//NT - get rid of all BFG ammo
		ent->client->ps.ammo[WP_BFG] = 0;
		G_AddEvent(ent, EV_NOAMMO, 0);

		// Take the BFG away from the current player
		ent->client->ps.stats[STAT_WEAPONS] &= ~( 1 << WP_BFG );
		ent->client->ps.weapon = WP_NONE;
	}
}

//NT - BFG overload stuff :)

//NT - this is used to teleport the player after he watches the carnage
void BFG_TeleportPlayer( gentity_t *ent ) {
	vec3_t		origin, angles;
	gentity_t	*spot = NULL, *tent;

	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR )
		return;

	if ( ent->client->ps.stats[STAT_HEALTH] > 0 ) {
		if ( g_gametype.integer < GT_CTF ) {
			spot = SelectSpawnPoint(ent->client->ps.origin, origin, angles);
		} else {
			if ( ent->client->sess.sessionTeam == TEAM_RED || ent->client->sess.sessionTeam == TEAM_WATCHING_RED ) {
//				if ( ent->client->ps.powerups[PW_BLUEFLAG] ) {
//					spot = SelectRandomFurthestOfClass(ent->client->ps.origin, origin, angles, "team_CTF_bluespawn");
//				} else {
					spot = SelectRandomFurthestOfClass(ent->client->ps.origin, origin, angles, "team_CTF_redspawn");
//				}
			}
			else if ( ent->client->sess.sessionTeam == TEAM_BLUE || ent->client->sess.sessionTeam == TEAM_WATCHING_BLUE ) {
//				if ( ent->client->ps.powerups[PW_REDFLAG] ) {
//					spot = SelectRandomFurthestOfClass(ent->client->ps.origin, origin, angles, "team_CTF_redspawn");
//				} else {
					spot = SelectRandomFurthestOfClass(ent->client->ps.origin, origin, angles, "team_CTF_bluespawn");
//				}
			}

			if ( !spot ) {
				spot = SelectSpawnPoint(ent->client->ps.origin, origin, angles);
			}
		}

		TeleportPlayer(ent, origin, angles, qtrue, qtrue, qtrue);
	}

	if ( ent->client->ps.persistant[ PERS_TEAM ] > TEAM_SPECTATOR ) {
		if ( ent->client->ps.stats[STAT_HEALTH] > 0 ) {
			tent = G_TempEntity( origin, EV_PLAYER_TELEPORT_IN );
			tent->s.clientNum = ent->s.clientNum;
		}

		// Put them back on their own team
		ent->client->ps.persistant[ PERS_TEAM ] -= WATCHING_DIFF;
		ent->client->sess.sessionTeam -= WATCHING_DIFF;
		// hide the weapon for the switch
		ent->client->ps.weapon = WP_NONE;
	}
}

//NT - last think in the explosion cycle
void BFG_ExplosionEnd( gentity_t *ent ) {
	BFG_TeleportPlayer( ent->parent );
	G_FreeEntity( ent );
}

//NT - called every 30ms (+/- depending on frame length)
// does radius damage based on size of shockwave so far
// this code must be well-synced with the cg_localents.c/CG_AddOverload
void BFG_ExplosionThink( gentity_t *ent ) {
	int			t;
	float		c, s;

	t = level.time - ent->s.time;
	if( t < 2750 ) {
		c = (float)t / 2750;
	} else {
		c = (float)(3000 - t) / 250;
	}

	// just in case
	if ( c > 1.0f )
		c = 1.0f;

	s = sqrt( 1 - (1 - c) * (1 - c) );

	G_RadiusDamage( ent->r.currentOrigin, ent->parent, ent->splashDamage * ent->s.generic1,
		s * 56 * ent->s.generic1, ent, ent->splashMethodOfDeath, qtrue );

	ent->nextthink = level.time + 30;
}

//NT - this gets called after the BFG is down for 2 seconds
// creates the explosion and shockwave
void BFG_TimerThink( gentity_t *ent ) {
	gentity_t	*bfg, *expl;

	bfg = &g_entities[ent->s.otherEntityNum];

	if ( bfg->s.weapon == WP_BFG && bfg->s.eType == ET_ITEM ) {
		VectorCopy( bfg->r.currentOrigin, ent->r.currentOrigin );

		bfg->s.eType = ET_GENERAL;
		bfg->s.modelindex = 0;
		bfg->s.pos.trType = TR_STATIONARY;
		bfg->touch = NULL;
		bfg->s.generic1 = 32; // make the explosion big
		VectorCopy( bfg->r.currentOrigin, bfg->s.pos.trBase );
		G_AddEvent( bfg, EV_MISSILE_MISS, 32.0 );
		bfg->freeAfterEvent = qtrue;

		trap_LinkEntity( bfg );

		expl = G_Spawn();
		expl->classname = "BFGexpl";
		expl->nextthink = level.time + 30;
		expl->think = BFG_ExplosionThink;
		expl->lastthinktime = level.time + 3000;
		expl->lastthink = BFG_ExplosionEnd;
		expl->s.time = level.time;
		expl->r.svFlags = SVF_BROADCAST;
		expl->r.ownerNum = ent->r.ownerNum;
		expl->s.otherEntityNum2 = ent->r.ownerNum;
		expl->s.eType = ET_OVERLOAD;
		expl->s.eventParm = GetUniqueId();
		expl->damage = ent->damage;
		expl->splashDamage = ent->damage;
		expl->parent = ent->parent;
		expl->methodOfDeath = MOD_BFG_OVERLOAD;
		expl->splashMethodOfDeath = MOD_BFG_OVERLOAD;
		expl->target_ent = NULL;
		expl->s.generic1 = ent->s.generic1;

		expl->s.pos.trType = TR_STATIONARY;
		G_SetOrigin( expl, ent->r.currentOrigin );
		VectorCopy( ent->r.currentOrigin, expl->s.origin );

		trap_LinkEntity( expl );
	} else {
		BFG_TeleportPlayer( ent->parent );
	}

	G_FreeEntity( ent );
}

#define TELE_YAW_NUM		36
#define TELE_PITCH_NUM		6
#define TELE_START_PITCH	20
#define TELE_END_PITCH		80
#define TELE_PITCH_STEP		( ( TELE_END_PITCH - TELE_START_PITCH ) / TELE_PITCH_NUM )
#define TELE_POS_NUM		( TELE_YAW_NUM * TELE_PITCH_NUM )

//NT - called when the player fires the BFG secondary attack
// drops the BFG, creates the timer, and finds a good place to
// teleport the player to watch all the fun
void BFG_Teleport( gentity_t *ent ) {
	gitem_t		*item;
	gentity_t	*bfg, *timer = NULL, *tent;
	int			ammo;
	vec3_t		pos[TELE_POS_NUM], angle[TELE_POS_NUM], bestposes[TELE_POS_NUM], bestangles[TELE_POS_NUM];
	vec3_t		mins = { -32, -32, -32}, maxs = {32, 32, 32};
	int			i, j, bestnum;
	vec_t		bestposlen = 0.0f, yaw, pitch, size;
	static int	seed = 0xFF;

	// drop the player's flags
	Team_DropFlags( ent );

	ammo = ent->client->ps.ammo[WP_BFG];

	// Set ammo to nothing
	ent->client->ps.ammo[WP_BFG] = 0;
	G_AddEvent(ent, EV_NOAMMO, 0);

	if ( ammo > 0 ) {
		// Take the BFG away from the current player
		ent->client->ps.stats[STAT_WEAPONS] &= ~( 1 << WP_BFG );

		// Drop the BFG
		item = BG_FindItemForWeapon( WP_BFG );
		bfg = Drop_Item( ent, item, 0 );
		bfg->count = ammo;
		bfg->s.weapon = WP_BFG;

		timer = G_Spawn();
		timer->classname = "BFGtimer";
		timer->nextthink = level.time + 1500;
		timer->think = BFG_TimerThink;
		timer->r.svFlags = SVF_NOCLIENT;
		timer->r.ownerNum = ent->s.number;
		timer->damage = 10 * s_quadFactor;
		timer->parent = ent;
		timer->target_ent = NULL;
		timer->s.generic1 = ammo;
		if ( timer->s.generic1 > 30 ) {
			timer->s.generic1 = 30;
		}
		timer->s.otherEntityNum = bfg->s.number;

		timer->s.pos.trType = TR_STATIONARY;
		VectorCopy( ent->r.currentOrigin, timer->s.origin );
		VectorCopy( ent->r.currentOrigin, timer->r.currentOrigin );

		trap_LinkEntity( timer );

		G_TempEntity(ent->r.currentOrigin, EV_START_OVERLOAD);

		// Put them on a watching team
		ent->client->ps.persistant[ PERS_TEAM ] += WATCHING_DIFF;
		ent->client->sess.sessionTeam += WATCHING_DIFF;

		// Teleport to a nice viewing spot
		tent = G_TempEntity( ent->client->ps.origin, EV_PLAYER_TELEPORT_OUT );
		tent->s.clientNum = ent->s.clientNum;

		// Trace all around
		size = 560; //timer->s.generic1 * 56;
		pitch = TELE_START_PITCH;
		for ( i = 0; i < TELE_PITCH_NUM; i++, pitch += TELE_PITCH_STEP ) {
			yaw = 0;
			for ( j = 0; j < TELE_YAW_NUM; j++, yaw += 360 / TELE_YAW_NUM ) {
				vec3_t		delta, endpos;
				trace_t		trace;
				int			passent = ent->s.number;
				int			arraypos = i * TELE_YAW_NUM + j;

				delta[0] = cos( DEG2RAD(yaw) ) * cos( DEG2RAD(pitch) ) * size;
				delta[1] = sin( DEG2RAD(yaw) ) * cos( DEG2RAD(pitch) ) * size;
				delta[2] = sin( DEG2RAD(pitch) ) * size;

				VectorAdd( timer->s.origin, delta, endpos );

				trap_Trace( &trace, timer->s.origin, mins, maxs, endpos, passent, MASK_DEADSOLID );

				VectorCopy( trace.endpos, pos[arraypos]);

				angle[arraypos][0] = pitch;
				angle[arraypos][1] = 180 + yaw;
				angle[arraypos][2] = 0;
			}
		}

		// Find the farthest trace
		for ( i = 0; i < TELE_POS_NUM; i++ ) {
			vec_t		len;
			vec3_t		delta;

			VectorSubtract( pos[i], timer->s.origin, delta );

			len = VectorLength( delta );

			if ( len > bestposlen ) {
				bestposlen = len;
			}
		}

		// Put all the near-duplicates in
		bestnum = 0;
		for ( i = 0; i < TELE_POS_NUM; i++ ) {
			vec_t		len, diff;
			vec3_t		delta;

			VectorSubtract( pos[i], timer->s.origin, delta );
			len = VectorLength( delta );

			diff = fabs( len - bestposlen );
			if ( fabs( len - bestposlen ) < 16.0f ) {
				VectorCopy( pos[i], bestposes[bestnum] );
				VectorCopy( angle[i], bestangles[bestnum] );
				bestnum++;
			}
		}

		i = abs( Q_rand( &seed ) ) % bestnum;

		TeleportPlayer( ent, bestposes[i], bestangles[i], qtrue, qtrue, qtrue );
		VectorClear( ent->client->ps.velocity );
	} else {
		BFG_TeleportPlayer( ent );
	}
}

/*
======================================================================

SHOTGUN

======================================================================
*/

// DEFAULT_SHOTGUN_SPREAD and DEFAULT_SHOTGUN_COUNT	are in bg_public.h, because
// client predicts same spreads
#define	DEFAULT_SHOTGUN_DAMAGE	10

//NT - added hurtOwner, mod, and quadFactor because this could be called because
// of an exploding ammo box
qboolean ShotgunPellet( vec3_t start, vec3_t end, gentity_t *ent, qboolean hurtOwner, int mod, int quadFactor ) {
	trace_t		tr;
	int			damage, i, passent;
	gentity_t	*traceEnt;
#ifdef MISSIONPACK
	vec3_t		impactpoint, bouncedir;
#endif
	vec3_t		tr_start, tr_end, dir;

	if ( hurtOwner )
		passent = ENTITYNUM_NONE;
	else
		passent = ent->s.number;

	//NT - calculate the knockback direction (was using muzzle)
	VectorSubtract( end, start, dir );
	VectorNormalizeFast( dir );

	VectorCopy( start, tr_start );
	VectorCopy( end, tr_end );
	for (i = 0; i < 10; i++) {
		trap_Trace (&tr, tr_start, NULL, NULL, tr_end, passent, MASK_SHOT);
		traceEnt = &g_entities[ tr.entityNum ];

		// send bullet impact
		if (  tr.surfaceFlags & SURF_NOIMPACT ) {
			return qfalse;
		}

		if ( traceEnt->takedamage) {
			damage = DEFAULT_SHOTGUN_DAMAGE * quadFactor;
#ifdef MISSIONPACK
			if ( traceEnt->client && traceEnt->client->invulnerabilityTime > level.time ) {
				if (G_InvulnerabilityEffect( traceEnt, dir, tr.endpos, impactpoint, bouncedir )) {
					G_BounceProjectile( tr_start, impactpoint, bouncedir, tr_end );
					VectorCopy( impactpoint, tr_start );
					// the player can hit him/herself with the bounced rail
					passent = ENTITYNUM_NONE;
				}
				else {
					VectorCopy( tr.endpos, tr_start );
					passent = traceEnt->s.number;
				}
				continue;
			}
			else {
				G_Damage( traceEnt, ent, ent, dir, tr.endpos,
					damage, 0, mod);
				if( LogAccuracyHit( traceEnt, ent ) ) {
					return qtrue;
				}
			}
#else
			G_Damage( traceEnt, ent, ent, dir, tr.endpos,	damage, 0, mod);
				if( LogAccuracyHit( traceEnt, ent ) ) {
					return qtrue;
				}
#endif
		}
		return qfalse;
	}
	return qfalse;
}

// this should match CG_ShotgunPattern
//NT - added count and spread for one-shell shotgun
// added hurtOwner, mod, and quadFactor because this could be from an exploding
// ammo box
void ShotgunPattern( vec3_t origin, vec3_t origin2, int seed, gentity_t *ent, int count, int spread, qboolean hurtOwner, int mod, int quadFactor ) {
	int			i;
	float		r, u;
	vec3_t		end;
	vec3_t		forward, right, up;
	int			oldScore;
	qboolean	hitClient = qfalse;

	// derive the right and up vectors from the forward vector, because
	// the client won't have any other information
	VectorNormalize2( origin2, forward );
	PerpendicularVector( right, forward );
	CrossProduct( forward, right, up );

	oldScore = ent->client->ps.persistant[PERS_SCORE];

	// generate the "random" spread pattern
	for ( i = 0 ; i < count ; i++ ) {
		r = Q_crandom( &seed ) * spread * 16;
		u = Q_crandom( &seed ) * spread * 16;
		VectorMA( origin, 8192 * 16, forward, end);
		VectorMA (end, r, right, end);
		VectorMA (end, u, up, end);
		if( ShotgunPellet( origin, end, ent, hurtOwner, mod, quadFactor ) && !hitClient ) {
			hitClient = qtrue;
			ent->client->accuracy_hits++;
		}
	}
}

//NT - "normal" shotgun
void weapon_supershotgun_fire (gentity_t *ent) {
	gentity_t		*tent;

	// send shotgun blast
	tent = G_TempEntity( muzzle, EV_SHOTGUN );
	VectorScale( forward, 4096, tent->s.origin2 );
	SnapVector( tent->s.origin2 );
	tent->s.eventParm = rand() & 255;		// seed for spread pattern
	tent->s.otherEntityNum = ent->s.number;

	//NT - changed this to comply to new ShotgunPattern interface
	ShotgunPattern( tent->s.pos.trBase, tent->s.origin2, tent->s.eventParm, ent, DEFAULT_SHOTGUN_COUNT, DEFAULT_SHOTGUN_SPREAD, qfalse, MOD_SHOTGUN, s_quadFactor );
}

//NT - one-shell shotgun
void weapon_shotgun_fire (gentity_t *ent, vec3_t a_muzzle, vec3_t a_forward, qboolean hurtOwner, int mod, int quadFactor) {
	gentity_t		*tent;

	// send shotgun blast
	tent = G_TempEntity( a_muzzle, EV_SHOTGUN2 );
	VectorScale( a_forward, 4096, tent->s.origin2 );
	SnapVector( tent->s.origin2 );
	tent->s.eventParm = rand() & 255;		// seed for spread pattern
	tent->s.otherEntityNum = ent->s.number;

	ShotgunPattern( tent->s.pos.trBase, tent->s.origin2, tent->s.eventParm, ent, DEFAULT_SHOTGUN_COUNT / 2, DEFAULT_SHOTGUN_SPREAD / 2, hurtOwner, mod, quadFactor );
}

/*
======================================================================

GRENADE LAUNCHER

======================================================================
*/

void weapon_grenadelauncher_fire (gentity_t *ent) {
	gentity_t	*m;

	// extra vertical velocity
	forward[2] += 0.2f;
	VectorNormalize( forward );

	m = fire_grenade (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

//NT - proximity grenades
void weapon_proximitygrenade_fire (gentity_t *ent) {
	gentity_t	*m;

	// extra vertical velocity
	forward[2] += 0.2f;
	VectorNormalize( forward );

	m = fire_proximity_grenade (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

void G_DestroyProximityGrenades (gentity_t *self) {
	gentity_t	*ent;

	ent = NULL;
	while ((ent = G_Find (ent, FOFS(classname), "proximityGrenade")) != NULL) {
		if ( ent->parent == self && ent->nextthink > level.time + 2000 ) {
			ent->nextthink = level.time + 2000;
			ent->think = G_ProximityGrenadeTrigger;
		}
	}
}

/*
======================================================================

ROCKET

======================================================================
*/

void Weapon_RocketLauncher_Fire (gentity_t *ent) {
	gentity_t	*m;

	m = fire_rocket (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

//NT - homing missiles
void Weapon_HomingMissile_Fire (gentity_t *ent) {
	gentity_t	*m;

	m = fire_homing (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

//NT - added chaff
/*
======================================================================

CHAFF

======================================================================
*/

void Weapon_Chaff_Fire (gentity_t *ent) {
	gentity_t	*m;

	m = fire_chaff (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

/*
======================================================================

PLASMA GUN

======================================================================
*/

void Weapon_Plasmagun_Fire (gentity_t *ent) {
	gentity_t	*m;

	m = fire_plasma (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

//NT - orbiting plasma balls
void Weapon_Plasmagun_Fire2 (gentity_t *ent) {
	gentity_t	*m;

	m = fire_plasma2 (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

void G_DestroyOrbitingPlasma (gentity_t *self) {
	gentity_t	*ent;

	ent = NULL;
	while ((ent = G_Find (ent, FOFS(classname), "plasma2")) != NULL) {
		if ( ent->parent == self && ent->lastthinktime > level.time + 1 ) {
			ent->lastthinktime = level.time + 1;
		}
	}
}

/*
======================================================================

RAILGUN

======================================================================
*/


/*
=================
weapon_railgun_fire
=================
*/
#define	MAX_RAIL_HITS	4

//NT - added a_muzzle, a_forward, hurtOwner, damage, mod, and quadFactor
// because this could be called because of an exploding ammo box
void weapon_railgun_fire (gentity_t *ent, vec3_t a_muzzle, vec3_t a_forward, qboolean hurtOwner, int damage, int mod, int quadFactor) {
	vec3_t		end, oldmuzzle;
#ifdef MISSIONPACK
	vec3_t impactpoint, bouncedir;
#endif
	trace_t		trace;
	gentity_t	*tent;
	gentity_t	*traceEnt;
	int			i;
	int			hits;
	int			unlinked;
	int			passent;
	gentity_t	*unlinkedEntities[MAX_RAIL_HITS];

	damage *= quadFactor;

	VectorMA (a_muzzle, 8192, a_forward, end);

	// trace only against the solids, so the railgun will go through people
	unlinked = 0;
	hits = 0;

	if ( hurtOwner )
		passent = ENTITYNUM_NONE;
	else
		passent = ent->s.number;

	VectorCopy(a_muzzle, oldmuzzle);
	do {
		trap_Trace (&trace, a_muzzle, NULL, NULL, end, passent, MASK_SHOT );
		if ( trace.entityNum >= ENTITYNUM_MAX_NORMAL ) {
			break;
		}
		traceEnt = &g_entities[ trace.entityNum ];
		if ( traceEnt->takedamage ) {
#ifdef MISSIONPACK
			if ( traceEnt->client && traceEnt->client->invulnerabilityTime > level.time ) {
				if ( G_InvulnerabilityEffect( traceEnt, a_forward, trace.endpos, impactpoint, bouncedir ) ) {
					G_BounceProjectile( a_muzzle, impactpoint, bouncedir, end );
					// snap the endpos to integers to save net bandwidth, but nudged towards the line
					SnapVectorTowards( trace.endpos, a_muzzle );
					// send railgun beam effect
					tent = G_TempEntity( trace.endpos, EV_RAILTRAIL );
					// set player number for custom colors on the railtrail
					tent->s.clientNum = ent->s.clientNum;
					VectorCopy( a_muzzle, tent->s.origin2 );
					// move origin a bit to come closer to the drawn gun muzzle
					VectorMA( tent->s.origin2, 4, right, tent->s.origin2 );
					VectorMA( tent->s.origin2, -1, up, tent->s.origin2 );
					tent->s.eventParm = 255;	// don't make the explosion at the end
					//
					VectorCopy( impactpoint, a_muzzle );
					// the player can hit him/herself with the bounced rail
					passent = ENTITYNUM_NONE;
				}
			}
			else {
				if( LogAccuracyHit( traceEnt, ent ) ) {
					hits++;
				}
				G_Damage (traceEnt, ent, ent, a_forward, trace.endpos, damage, 0, mod);
			}
#else
				if( LogAccuracyHit( traceEnt, ent ) ) {
					hits++;
				}
				if ( hurtOwner && traceEnt->s.number == ent->s.number )
					G_Damage (traceEnt, ent, ent, a_forward, trace.endpos, damage * 2, 0, mod);
				else
					G_Damage (traceEnt, ent, ent, a_forward, trace.endpos, damage, 0, mod);
#endif
		}
		if ( trace.contents & CONTENTS_SOLID ) {
				break;
		}
		// unlink this entity, so the next trace will go past it
		trap_UnlinkEntity( traceEnt );
		unlinkedEntities[unlinked] = traceEnt;
		unlinked++;
	} while ( unlinked < MAX_RAIL_HITS );
	
	VectorCopy(oldmuzzle, a_muzzle);

	// link back in any entities we unlinked
	for ( i = 0 ; i < unlinked ; i++ ) {
		trap_LinkEntity( unlinkedEntities[i] );
	}

	// the final trace endpos will be the terminal point of the rail trail

	// snap the endpos to integers to save net bandwidth, but nudged towards the line
	SnapVectorTowards( trace.endpos, a_muzzle );

	// send railgun beam effect
	tent = G_TempEntity( trace.endpos, EV_RAILTRAIL );

	// set player number for custom colors on the railtrail
	tent->s.clientNum = ent->s.clientNum;

	VectorCopy( a_muzzle, tent->s.origin2 );
	// move origin a bit to come closer to the drawn gun muzzle
	VectorMA( tent->s.origin2, 4, right, tent->s.origin2 );
	VectorMA( tent->s.origin2, -1, up, tent->s.origin2 );

	// no explosion at end if SURF_NOIMPACT, but still make the trail
	if ( trace.surfaceFlags & SURF_NOIMPACT ) {
		tent->s.eventParm = 255;	// don't make the explosion at the end
	} else {
		tent->s.eventParm = DirToByte( trace.plane.normal );
	}
	tent->s.clientNum = ent->s.clientNum;

	// give the shooter a reward sound if they have made two railgun hits in a row
	if ( hits == 0 ) {
		// complete miss
		ent->client->accurateCount = 0;
	} else {
		// check for "impressive" reward sound
		ent->client->accurateCount += hits;
		if ( ent->client->accurateCount >= 2 ) {
			ent->client->accurateCount -= 2;
			ent->client->ps.persistant[PERS_IMPRESSIVE_COUNT]++;
			// add the sprite over the player's head
			ent->client->ps.eFlags &= ~(EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP );
			ent->client->ps.eFlags |= EF_AWARD_IMPRESSIVE;
			ent->client->rewardTime = level.time + REWARD_SPRITE_TIME;
		}
		ent->client->accuracy_hits++;
	}

}

//NT - radius rail fire
/*
=================
weapon_railgun_fire2
=================
*/
void weapon_railgun_fire2 (gentity_t *ent) {
	vec3_t		end, knock;
	trace_t		trace;
	gentity_t	*tent;
	int			damage;
	int			passent;
	vec3_t		move, step;
	float		len, j, spacing;

	damage = 16 * s_quadFactor;

	VectorMA (muzzle, 8192, forward, end);

	passent = ent->s.number;

	trap_Trace (&trace, muzzle, NULL, NULL, end, passent, MASK_SOLID );

	spacing = 16.0f;
	VectorCopy( muzzle, move );
	VectorSubtract( trace.endpos, muzzle, step );
	len = VectorNormalize( step );
	VectorScale( step, spacing, step );

	for (j = 0; j < len; j += spacing ) {
		if( G_RadiusDamage( move, ent, damage, 128, ent, MOD_RADIUS_RAILGUN, qtrue ) ) {
			ent->client->accuracy_hits++;
		}
		VectorAdd( move, step, move );
	}

	// give the thing a big kick
	VectorScale( forward, -1.0f, knock );
	G_Damage( ent, ent, ent, knock, muzzle, 75, DAMAGE_NO_SELF, MOD_RADIUS_RAILGUN );

	// the final trace endpos will be the terminal point of the rail trail

	// snap the endpos to integers to save net bandwidth, but nudged towards the line
	SnapVectorTowards( trace.endpos, muzzle );

	// send railgun beam effect
	tent = G_TempEntity( trace.endpos, EV_RAILTRAIL2 );

	// set player number for custom colors on the railtrail
	tent->s.clientNum = ent->s.clientNum;

	VectorCopy( muzzle, tent->s.origin2 );
	// move origin a bit to come closer to the drawn gun muzzle
	VectorMA( tent->s.origin2, 4, right, tent->s.origin2 );
	VectorMA( tent->s.origin2, -1, up, tent->s.origin2 );

	// no explosion at end if SURF_NOIMPACT, but still make the trail
	if ( trace.surfaceFlags & SURF_NOIMPACT ) {
		tent->s.eventParm = 255;	// don't make the explosion at the end
	} else {
		tent->s.eventParm = DirToByte( trace.plane.normal );
	}
	tent->s.clientNum = ent->s.clientNum;
}


/*
======================================================================

GRAPPLING HOOK

======================================================================
*/

void Weapon_GrapplingHook_Fire (gentity_t *ent)
{
	if (!ent->client->fireHeld && !ent->client->hook)
		fire_grapple (ent, muzzle, forward);

	ent->client->fireHeld = qtrue;
}

void Weapon_HookFree (gentity_t *ent)
{
	ent->parent->client->hook = NULL;
	ent->parent->client->ps.pm_flags &= ~PMF_GRAPPLE_PULL;
	G_FreeEntity( ent );
}

void Weapon_HookThink (gentity_t *ent)
{
	if (ent->enemy) {
		vec3_t v, oldorigin;

		VectorCopy(ent->r.currentOrigin, oldorigin);
		v[0] = ent->enemy->r.currentOrigin[0] + (ent->enemy->r.mins[0] + ent->enemy->r.maxs[0]) * 0.5;
		v[1] = ent->enemy->r.currentOrigin[1] + (ent->enemy->r.mins[1] + ent->enemy->r.maxs[1]) * 0.5;
		v[2] = ent->enemy->r.currentOrigin[2] + (ent->enemy->r.mins[2] + ent->enemy->r.maxs[2]) * 0.5;
		SnapVectorTowards( v, oldorigin );	// save net bandwidth

		G_SetOrigin( ent, v );
	}

	VectorCopy( ent->r.currentOrigin, ent->parent->client->ps.grapplePoint);
}

/*
======================================================================

LIGHTNING GUN

======================================================================
*/

void Lightning_Discharge_Think( gentity_t *ent ) {
	if( G_RadiusDamage( ent->r.currentOrigin, ent->parent, ent->damage, 300, ent, MOD_DISCHARGE, qfalse ) ) {
		if ( ent->parent && ent->parent->client ) {
			ent->parent->client->accuracy_hits++;
		}
	}
	ent->nextthink = level.time + 1;
}

//NT
/*
===============
Lightning_Discharge
===============
*/
void Lightning_Discharge( gentity_t *ent ) {
	gentity_t	*dis;

	dis = G_Spawn();
	dis->classname = "lightningdischarge";
	dis->s.eType = ET_LIGHTNING_AMMO_EXPL;
	dis->nextthink = level.time + 1;
	dis->think = Lightning_Discharge_Think;
	dis->lastthinktime = level.time + 500;
	dis->lastthink = G_FreeEntity;
	dis->r.ownerNum = ent->s.number;
	dis->parent = ent;
	dis->target_ent = NULL;
	dis->s.eventParm = GetUniqueId();
	dis->s.pos.trType = TR_STATIONARY;
	dis->damage = ent->client->ps.stats[ STAT_LAST_AMMO ] * 3 * s_quadFactor / 2;
	dis->s.time = level.time;
	VectorCopy( muzzle, dis->s.origin );
	VectorCopy( muzzle, dis->r.currentOrigin );
	VectorCopy( muzzle, dis->s.pos.trBase );
	trap_LinkEntity( dis );
}

qboolean Lightning_CheckUnderwater( gentity_t *ent ) {
	int			pc;

	pc = trap_PointContents( muzzle, ent->s.number );

	if ( pc & ( CONTENTS_WATER | CONTENTS_SLIME ) ) {
		Lightning_Discharge( ent );
		ent->client->ps.ammo[WP_LIGHTNING] = 0;
		return qtrue;
	} else {
		return qfalse;
	}
}

void Weapon_LightningFire( gentity_t *ent ) {
	trace_t		tr;
	vec3_t		end;
#ifdef MISSIONPACK
	vec3_t impactpoint, bouncedir;
#endif
	gentity_t	*traceEnt, *tent;
	int			damage, i, passent;

	//NT - added water check
	if ( Lightning_CheckUnderwater( ent ) ) {
		return;
	}

	damage = 8 * s_quadFactor;

	passent = ent->s.number;
	for (i = 0; i < 10; i++) {
		VectorMA( muzzle, LIGHTNING_RANGE, forward, end );

		trap_Trace( &tr, muzzle, NULL, NULL, end, passent, MASK_SHOT | CONTENTS_WATER | CONTENTS_SLIME );

#ifdef MISSIONPACK
		// if not the first trace (the lightning bounced of an invulnerability sphere)
		if (i) {
			// add bounced off lightning bolt temp entity
			// the first lightning bolt is a cgame only visual
			//
			tent = G_TempEntity( muzzle, EV_LIGHTNINGBOLT );
			VectorCopy( tr.endpos, end );
			SnapVector( end );
			VectorCopy( end, tent->s.origin2 );
		}
#endif
		if ( tr.entityNum == ENTITYNUM_NONE ) {
			return;
		}

		traceEnt = &g_entities[ tr.entityNum ];

		if ( traceEnt->takedamage) {
#ifdef MISSIONPACK
			if ( traceEnt->client && traceEnt->client->invulnerabilityTime > level.time ) {
				if (G_InvulnerabilityEffect( traceEnt, forward, tr.endpos, impactpoint, bouncedir )) {
					G_BounceProjectile( muzzle, impactpoint, bouncedir, end );
					VectorCopy( impactpoint, muzzle );
					VectorSubtract( end, impactpoint, forward );
					VectorNormalize(forward);
					// the player can hit him/herself with the bounced lightning
					passent = ENTITYNUM_NONE;
				}
				else {
					VectorCopy( tr.endpos, muzzle );
					passent = traceEnt->s.number;
				}
				continue;
			}
			else {
				G_Damage( traceEnt, ent, ent, forward, tr.endpos,
					damage, 0, MOD_LIGHTNING);
			}
#else
				G_Damage( traceEnt, ent, ent, forward, tr.endpos,
					damage, 0, MOD_LIGHTNING);
#endif
		}

		if ( traceEnt->takedamage && traceEnt->client ) {
			tent = G_TempEntity( tr.endpos, EV_MISSILE_HIT );
			tent->s.otherEntityNum = traceEnt->s.number;
			tent->s.eventParm = DirToByte( tr.plane.normal );
			tent->s.weapon = ent->s.weapon;
			if( LogAccuracyHit( traceEnt, ent ) ) {
				ent->client->accuracy_hits++;
			}
		//NT - added water check
		} else if ( tr.contents & ( CONTENTS_WATER | CONTENTS_SLIME ) ) {
			G_RadiusDamage( tr.endpos, ent, 15 * s_quadFactor, 60, NULL, MOD_LIGHTNING, qfalse );
			tent = G_TempEntity( tr.endpos, EV_LIGHTNING_SPLASH );
		} else if ( !( tr.surfaceFlags & SURF_NOIMPACT ) ) {
			tent = G_TempEntity( tr.endpos, EV_MISSILE_MISS );
			tent->s.eventParm = DirToByte( tr.plane.normal );
		}

		break;
	}
}

//NT - all this is new, for the lightning webs
// (I love 'em)
/*
======================================================================

LIGHTNING WEB

======================================================================
*/
//NT - destroys all of a player's lightning webs
// there is a similar function in the CG_ stuff
void G_DestroyWebs( gentity_t *self ) {
	gentity_t	*ent, *tent;

	// find all of a player's lightning webs and kill them
	ent = NULL;
	while ((ent = G_Find (ent, FOFS(classname), "lightningweb")) != NULL) {
		if ( ent->s.otherEntityNum == self->s.number && ent->lastthinktime > level.time + 2500 ) {
			ent->lastthinktime = level.time + 2500;
			tent = G_TempEntity( ent->r.currentOrigin, EV_DESTROY_WEBS );
			tent->s.otherEntityNum = ent->s.otherEntityNum;
			tent->r.svFlags = SVF_BROADCAST;
		}
	}

	ent = NULL;
	while ((ent = G_Find (ent, FOFS(classname), "strand")) != NULL) {
		if ( ent->parent == self && ent->lastthinktime > level.time + 2500 ) {
			ent->lastthinktime = level.time + 2500;
		}
	}
}

#define NANGLES	24
#define MAXSTRAND 128
#define STRANDOVERLAP -16

//NT - this is where a strand hurts things
void G_StrandThink( gentity_t *ent ) {
	trace_t			trace;
	gentity_t		*traceEnt;
	vec3_t			dir;
	int				passent;
	vec3_t			mins = { -4, -4, -4 }, maxs = { 4, 4, 4 };

	passent = ent->s.number;

	trap_Trace( &trace, ent->s.origin, mins, maxs, ent->s.origin2, passent, MASK_WEBSTRAND );
	traceEnt = &g_entities[ trace.entityNum ];
	if ( traceEnt->takedamage ) {
		VectorSubtract( ent->s.origin, ent->s.origin2, dir );
		G_Damage ( traceEnt, ent, ent->parent, dir, trace.endpos, ent->damage, DAMAGE_NO_SELF, MOD_LIGHTNING_WEB );
	}

	ent->nextthink = level.time + 1;
}

//NT - create the lightning web controller entity and the server-side strands
void Weapon_LightningFire2( gentity_t *ent ) {
	trace_t		trace;
	vec_t		theta;
	vec3_t		d, upd, rightd, end, origin, startpos;
	int			passent, j;
	gentity_t	*strand, *web;

	if ( Lightning_CheckUnderwater( ent ) ) {
		return;
	}

	VectorMA( muzzle, 40, forward, origin );

	passent = ent->s.number;
	trap_Trace( &trace, muzzle, NULL, NULL, origin, passent, MASK_SOLID );
	VectorCopy( trace.endpos, origin );
	VectorMA( origin, -8, forward, origin );

	// create a web entity
	// this will be used on the client side to create the effects
	web = G_Spawn();
	web->s.eType = ET_LIGHTNING_WEB;
	web->classname = "lightningweb";
	web->lastthinktime = level.time + 20000;
	web->lastthink = G_FreeEntity;
	web->s.weapon = WP_LIGHTNING;
	web->r.ownerNum = ent->s.number;
	web->s.otherEntityNum = ent->s.number;
	web->s.time = level.time;
	web->parent = ent;

	web->s.pos.trType = TR_STATIONARY;
	VectorCopy( origin, web->s.pos.trBase );
	VectorClear( web->s.pos.trDelta );
	web->s.pos.trTime = 0;
	web->s.pos.trDuration = 0;

	// let's abuse some fields so the client can draw the web properly
	VectorCopy( up, web->s.origin );
	VectorCopy( right, web->s.origin2 );
	VectorCopy( origin, web->r.currentOrigin );
	
	trap_LinkEntity( web );

	// the server side has only radial strands that damage inward
	// these are never sent to the client - the client draws everything
	// using the above "web" entity
	for ( j = 0; j < NANGLES; j++ ) {
		theta = 2 * M_PI * j / NANGLES;

		VectorScale( up, sin(theta), upd );
		VectorScale( right, cos(theta), rightd );
		VectorAdd( upd, rightd, d );

		VectorMA( origin, MAXSTRAND, d, end );

		trap_Trace( &trace, origin, NULL, NULL, end, passent, MASK_SOLID | CONTENTS_WATER | CONTENTS_SLIME );

		strand = G_Spawn();
		strand->classname = "strand";
		strand->nextthink = level.time + 1;
		strand->think = G_StrandThink;
		strand->lastthinktime = level.time + 20000;
		strand->lastthink = G_FreeEntity;
		strand->s.eType = ET_BEAM;
		strand->r.svFlags = SVF_USE_CURRENT_ORIGIN | SVF_NOCLIENT;
		strand->s.weapon = WP_LIGHTNING;
		strand->r.ownerNum = ent->s.number;
		strand->s.generic1 = 1;
		strand->damage = 15 * s_quadFactor;
		strand->parent = ent;
		strand->methodOfDeath = MOD_LIGHTNING_WEB;
		strand->splashMethodOfDeath = MOD_LIGHTNING_WEB;
		strand->clipmask = MASK_SHOT;
		strand->target_ent = NULL;

		VectorMA( origin, STRANDOVERLAP, d, startpos );

		strand->s.pos.trType = TR_STATIONARY;
		VectorCopy( startpos, strand->s.origin );
		VectorCopy( trace.endpos, strand->s.origin2 );
		VectorCopy( startpos, strand->r.currentOrigin );

		trap_LinkEntity( strand );
	}
}


#ifdef MISSIONPACK
/*
======================================================================

NAILGUN

======================================================================
*/

void Weapon_Nailgun_Fire (gentity_t *ent) {
	gentity_t	*m;
	int			count;

	for( count = 0; count < NUM_NAILSHOTS; count++ ) {
		m = fire_nail (ent, muzzle, forward, right, up );
		m->damage *= s_quadFactor;
		m->splashDamage *= s_quadFactor;
	}

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}


/*
======================================================================

PROXIMITY MINE LAUNCHER

======================================================================
*/

void weapon_proxlauncher_fire (gentity_t *ent) {
	gentity_t	*m;

	// extra vertical velocity
	forward[2] += 0.2f;
	VectorNormalize( forward );

	m = fire_prox (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

#endif

//======================================================================


/*
===============
LogAccuracyHit
===============
*/
qboolean LogAccuracyHit( gentity_t *target, gentity_t *attacker ) {
	if( !target->takedamage ) {
		return qfalse;
	}

	if ( target == attacker ) {
		return qfalse;
	}

	if( !target->client ) {
		return qfalse;
	}

	if( !attacker->client ) {
		return qfalse;
	}

	if( target->client->ps.stats[STAT_HEALTH] <= 0 ) {
		return qfalse;
	}

	if ( OnSameTeam( target, attacker ) ) {
		return qfalse;
	}

	return qtrue;
}


/*
===============
CalcMuzzlePoint

set muzzle location relative to pivoting eye
===============
*/
void CalcMuzzlePoint ( gentity_t *ent, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint ) {
	VectorCopy( ent->s.pos.trBase, muzzlePoint );
	muzzlePoint[2] += ent->client->ps.viewheight;
	VectorMA( muzzlePoint, 14, forward, muzzlePoint );
	// snap to integer coordinates for more efficient network bandwidth usage
	SnapVector( muzzlePoint );
}

//NT - for PW_ACCURACY
static void AimAtNearest( gentity_t *ent ) {
	gentity_t		*target = NULL, *blip = NULL;
	vec3_t			dir, blipdir, angles, eyeorigin, bliporigin;
	vec_t			dot, cs, cosine = -1;
	int				radius = 16000;
	float			cone = 10.0f;

	VectorCopy( ent->s.pos.trBase, eyeorigin );
	eyeorigin[2] += ent->client->ps.viewheight;

	if ( ent->client->ps.weapon == WP_LIGHTNING )
		radius = LIGHTNING_RANGE;

	while ((blip = FindRadius(blip, ent->r.currentOrigin, radius * radius)) != NULL) {
		if ( blip->client ) {
			if ( ( g_gametype.integer == GT_TEAM || g_gametype.integer == GT_CTF ) &&
				OnSameTeam( blip, ent ) )
				continue;

			if ( blip->client->sess.sessionTeam >= TEAM_SPECTATOR )
				continue;
		} else {
			if ( !blip->takedamage || ( blip->s.eType != ET_MISSILE && blip->s.eType != ET_ITEM ) )
				continue;

			if ( blip->parent == ent )
				continue;
		}

		if ( blip == ent || blip->health <= 0 )
			continue;

		if (!IsVisible (ent, blip))
			continue;

		VectorCopy( blip->r.currentOrigin, bliporigin );
		VectorSubtract(bliporigin, eyeorigin, blipdir);

		// E.B = |E||B|cos(theta)
		dot = _DotProduct(forward, blipdir);

		// Divide E.B by |E||B| to get cos(theta)
		cs = dot / (VectorLength(forward) * VectorLength(blipdir));

		if ( ent->client->ps.weapon == WP_RAILGUN ) {
			cone = 5.0f;
		}

		// If angle is less than "cone" degrees
		if (cs > cos(cone * M_PI / 180.0f))
		{
			if ( target == NULL || cs > cosine )
			{
					// We add it as our target
					target = blip;
					VectorCopy(blipdir, dir);
					cosine = cs;
			}
		}
	}

	if ( target != NULL ) {
		vectoangles( dir, angles );

		angles[0] = AngleMod(angles[0]);
		angles[1] = AngleMod(angles[1]);

		AngleVectors ( angles, forward, right, up );
		ent->client->ps.stats[STAT_AIMING] = qtrue;
		ent->client->ps.stats[STAT_AIMING_PITCH] = ANGLE2SHORT( angles[0] );
		ent->client->ps.stats[STAT_AIMING_YAW] = ANGLE2SHORT( angles[1] );
	}
}

/*
===============
CalcMuzzlePointOrigin

set muzzle location relative to pivoting eye
===============
*/
void CalcMuzzlePointOrigin ( gentity_t *ent, vec3_t origin, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint ) {
	VectorCopy( ent->s.pos.trBase, muzzlePoint );
	muzzlePoint[2] += ent->client->ps.viewheight;
	VectorMA( muzzlePoint, 14, forward, muzzlePoint );
	// snap to integer coordinates for more efficient network bandwidth usage
	SnapVector( muzzlePoint );
}


//NT - needed two FireWeapon functions - this represents common functionality
/*
===============
FireWeaponCommon

Functions common to FireWeapon and FireWeapon2
===============
*/
void FireWeaponCommon( gentity_t *ent, qboolean secondary ) {
	if (ent->client->ps.powerups[PW_QUAD] ) {
		s_quadFactor = g_quadfactor.value;
	} else {
		s_quadFactor = 1;
	}
#ifdef MISSIONPACK
	if( ent->client->persistantPowerup && ent->client->persistantPowerup->item && ent->client->persistantPowerup->item->giTag == PW_DOUBLER ) {
		s_quadFactor *= 2;
	}
#endif

	// track shots taken for accuracy tracking.  Grapple is not a weapon and gauntet is just not tracked
	if( ent->s.weapon != WP_GRAPPLING_HOOK && ent->s.weapon != WP_GAUNTLET ) {
#ifdef MISSIONPACK
		if( ent->s.weapon == WP_NAILGUN ) {
			ent->client->accuracy_shots += NUM_NAILSHOTS;
		} else {
			ent->client->accuracy_shots++;
		}
#else
		ent->client->accuracy_shots++;
#endif
	}

	// set aiming directions
	AngleVectors (ent->client->ps.viewangles, forward, right, up);
	CalcMuzzlePointOrigin ( ent, ent->client->oldOrigin, forward, right, up, muzzle );

	//NT -  aim at the nearest player if the current player has PW_ACCURACY
	ent->client->ps.stats[STAT_AIMING] = qfalse;
	if ( ent->client->ps.powerups[PW_ACCURACY] && BG_IsHitScanWeapon( ent->client->ps.weapon, secondary ) ) {
		AimAtNearest( ent );
		CalcMuzzlePointOrigin ( ent, ent->client->oldOrigin, forward, right, up, muzzle );
	}
}

/*
===============
FireWeapon
===============
*/
void FireWeapon( gentity_t *ent ) {
	FireWeaponCommon( ent, qfalse );

	// fire the specific weapon
	switch( ent->s.weapon ) {
	case WP_GAUNTLET:
		Weapon_Gauntlet( ent );
		break;
	case WP_LIGHTNING:
		Weapon_LightningFire( ent );
		break;
	case WP_SHOTGUN:
		weapon_supershotgun_fire( ent );
		break;
	case WP_MACHINEGUN:
		//NT - changed Bullet_Fire call to conform
		if ( g_gametype.integer != GT_TEAM ) {
			Bullet_Fire( ent, MACHINEGUN_SPREAD, muzzle, forward, up, right, qfalse, MACHINEGUN_DAMAGE, MOD_MACHINEGUN, s_quadFactor );
		} else {
			Bullet_Fire( ent, MACHINEGUN_SPREAD, muzzle, forward, up, right, qfalse, MACHINEGUN_TEAM_DAMAGE, MOD_MACHINEGUN, s_quadFactor );
		}
		break;
	case WP_GRENADE_LAUNCHER:
		weapon_grenadelauncher_fire( ent );
		break;
	case WP_ROCKET_LAUNCHER:
		Weapon_RocketLauncher_Fire( ent );
		break;
	case WP_PLASMAGUN:
		Weapon_Plasmagun_Fire( ent );
		break;
	case WP_RAILGUN:
		//NT - changed weapon_railgun_fire call to conform
		weapon_railgun_fire( ent, muzzle, forward, qfalse, 100, MOD_RAILGUN, s_quadFactor );
		break;
	case WP_BFG:
		BFG_Fire( ent );
		break;
	case WP_GRAPPLING_HOOK:
		Weapon_GrapplingHook_Fire( ent );
		break;
#ifdef MISSIONPACK
	case WP_NAILGUN:
		Weapon_Nailgun_Fire( ent );
		break;
	case WP_PROX_LAUNCHER:
		weapon_proxlauncher_fire( ent );
		break;
	case WP_CHAINGUN:
		Bullet_Fire( ent, CHAINGUN_SPREAD, muzzle, forward, up, right, qfalse, MACHINEGUN_DAMAGE, MOD_MACHINEGUN, s_quadFactor );
		break;
#endif
	default:
// FIXME		G_Error( "Bad ent->s.weapon" );
		break;
	}
}

//NT - called for the secondary attack
/*
===============
FireWeapon2
===============
*/
void FireWeapon2( gentity_t *ent ) {
	FireWeaponCommon( ent, qtrue );

	// fire the specific weapon
	switch( ent->s.weapon ) {
	case WP_GAUNTLET:
		ent->client->ps.powerups[PW_PROTECTION] = level.time + 100;
		G_AddEvent(ent, EV_POWERUP_PROTECTION, 0);
		break;
	case WP_LIGHTNING:
		Weapon_LightningFire2( ent );
		break;
	case WP_SHOTGUN:
		weapon_shotgun_fire( ent, muzzle, forward, qfalse, MOD_SHOTGUN, s_quadFactor );
		break;
	case WP_MACHINEGUN:
		Weapon_Chaff_Fire( ent );
		break;
	case WP_GRENADE_LAUNCHER:
		weapon_proximitygrenade_fire( ent );
		break;
	case WP_ROCKET_LAUNCHER:
		Weapon_HomingMissile_Fire( ent );
		break;
	case WP_PLASMAGUN:
		Weapon_Plasmagun_Fire2( ent );
		break;
	case WP_RAILGUN:
		weapon_railgun_fire2( ent );
		break;
	case WP_BFG:
		BFG_Teleport( ent );
		break;
	default:
// FIXME		G_Error( "Bad ent->s.weapon" );
		break;
	}
}

#ifdef MISSIONPACK

/*
===============
KamikazeRadiusDamage
===============
*/
static void KamikazeRadiusDamage( vec3_t origin, gentity_t *attacker, float damage, float radius ) {
	float		dist;
	gentity_t	*ent;
	int			entityList[MAX_GENTITIES];
	int			numListedEntities;
	vec3_t		mins, maxs;
	vec3_t		v;
	vec3_t		dir;
	int			i, e;

	if ( radius < 1 ) {
		radius = 1;
	}

	for ( i = 0 ; i < 3 ; i++ ) {
		mins[i] = origin[i] - radius;
		maxs[i] = origin[i] + radius;
	}

	numListedEntities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

	for ( e = 0 ; e < numListedEntities ; e++ ) {
		ent = &g_entities[entityList[ e ]];

		if (!ent->takedamage) {
			continue;
		}

		// dont hit things we have already hit
		if( ent->kamikazeTime > level.time ) {
			continue;
		}

		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ ) {
			if ( origin[i] < ent->r.absmin[i] ) {
				v[i] = ent->r.absmin[i] - origin[i];
			} else if ( origin[i] > ent->r.absmax[i] ) {
				v[i] = origin[i] - ent->r.absmax[i];
			} else {
				v[i] = 0;
			}
		}

		dist = VectorLength( v );
		if ( dist >= radius ) {
			continue;
		}

//		if( CanDamage (ent, origin) ) {
			VectorSubtract (ent->r.currentOrigin, origin, dir);
			// push the center of mass higher than the origin so players
			// get knocked into the air more
			dir[2] += 24;
			G_Damage( ent, NULL, attacker, dir, origin, damage, DAMAGE_RADIUS|DAMAGE_NO_TEAM_PROTECTION, MOD_KAMIKAZE );
			ent->kamikazeTime = level.time + 3000;
//		}
	}
}

/*
===============
KamikazeShockWave
===============
*/
static void KamikazeShockWave( vec3_t origin, gentity_t *attacker, float damage, float push, float radius ) {
	float		dist;
	gentity_t	*ent;
	int			entityList[MAX_GENTITIES];
	int			numListedEntities;
	vec3_t		mins, maxs;
	vec3_t		v;
	vec3_t		dir;
	int			i, e;

	if ( radius < 1 )
		radius = 1;

	for ( i = 0 ; i < 3 ; i++ ) {
		mins[i] = origin[i] - radius;
		maxs[i] = origin[i] + radius;
	}

	numListedEntities = trap_EntitiesInBox( mins, maxs, entityList, MAX_GENTITIES );

	for ( e = 0 ; e < numListedEntities ; e++ ) {
		ent = &g_entities[entityList[ e ]];

		// dont hit things we have already hit
		if( ent->kamikazeShockTime > level.time ) {
			continue;
		}

		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ ) {
			if ( origin[i] < ent->r.absmin[i] ) {
				v[i] = ent->r.absmin[i] - origin[i];
			} else if ( origin[i] > ent->r.absmax[i] ) {
				v[i] = origin[i] - ent->r.absmax[i];
			} else {
				v[i] = 0;
			}
		}

		dist = VectorLength( v );
		if ( dist >= radius ) {
			continue;
		}

//		if( CanDamage (ent, origin) ) {
			VectorSubtract (ent->r.currentOrigin, origin, dir);
			dir[2] += 24;
			G_Damage( ent, NULL, attacker, dir, origin, damage, DAMAGE_RADIUS|DAMAGE_NO_TEAM_PROTECTION, MOD_KAMIKAZE );
			//
			dir[2] = 0;
			VectorNormalize(dir);
			if ( ent->client ) {
				ent->client->ps.velocity[0] = dir[0] * push;
				ent->client->ps.velocity[1] = dir[1] * push;
				ent->client->ps.velocity[2] = 100;
			}
			ent->kamikazeShockTime = level.time + 3000;
//		}
	}
}

/*
===============
KamikazeDamage
===============
*/
static void KamikazeDamage( gentity_t *self ) {
	int i;
	float t;
	gentity_t *ent;
	vec3_t newangles;

	self->count += 100;

	if (self->count >= KAMI_SHOCKWAVE_STARTTIME) {
		// shockwave push back
		t = self->count - KAMI_SHOCKWAVE_STARTTIME;
		KamikazeShockWave(self->s.pos.trBase, self->activator, 25, 400,	(int) (float) t * KAMI_SHOCKWAVE_MAXRADIUS / (KAMI_SHOCKWAVE_ENDTIME - KAMI_SHOCKWAVE_STARTTIME) );
	}
	//
	if (self->count >= KAMI_EXPLODE_STARTTIME) {
		// do our damage
		t = self->count - KAMI_EXPLODE_STARTTIME;
		KamikazeRadiusDamage( self->s.pos.trBase, self->activator, 400,	(int) (float) t * KAMI_BOOMSPHERE_MAXRADIUS / (KAMI_IMPLODE_STARTTIME - KAMI_EXPLODE_STARTTIME) );
	}

	// either cycle or kill self
	if( self->count >= KAMI_SHOCKWAVE_ENDTIME ) {
		G_FreeEntity( self );
		return;
	}
	self->nextthink = level.time + 100;

	// add earth quake effect
	newangles[0] = crandom() * 2;
	newangles[1] = crandom() * 2;
	newangles[2] = 0;
	for (i = 0; i < MAX_CLIENTS; i++)
	{
		ent = &g_entities[i];
		if (!ent->inuse)
			continue;
		if (!ent->client)
			continue;

		if (ent->client->ps.groundEntityNum != ENTITYNUM_NONE) {
			ent->client->ps.velocity[0] += crandom() * 120;
			ent->client->ps.velocity[1] += crandom() * 120;
			ent->client->ps.velocity[2] = 30 + random() * 25;
		}

		ent->client->ps.delta_angles[0] += ANGLE2SHORT(newangles[0] - self->movedir[0]);
		ent->client->ps.delta_angles[1] += ANGLE2SHORT(newangles[1] - self->movedir[1]);
		ent->client->ps.delta_angles[2] += ANGLE2SHORT(newangles[2] - self->movedir[2]);
	}
	VectorCopy(newangles, self->movedir);
}

/*
===============
G_StartKamikaze
===============
*/
void G_StartKamikaze( gentity_t *ent ) {
	gentity_t	*explosion;
	gentity_t	*te;
	vec3_t		snapped;

	// start up the explosion logic
	explosion = G_Spawn();

	explosion->s.eType = ET_EVENTS + EV_KAMIKAZE;
	explosion->eventTime = level.time;

	if ( ent->client ) {
		VectorCopy( ent->s.pos.trBase, snapped );
	}
	else {
		VectorCopy( ent->activator->s.pos.trBase, snapped );
	}
	SnapVector( snapped );		// save network bandwidth
	G_SetOrigin( explosion, snapped );

	explosion->classname = "kamikaze";
	explosion->s.pos.trType = TR_STATIONARY;

	explosion->kamikazeTime = level.time;

	explosion->think = KamikazeDamage;
	explosion->nextthink = level.time + 100;
	explosion->count = 0;
	VectorClear(explosion->movedir);

	trap_LinkEntity( explosion );

	if (ent->client) {
		//
		explosion->activator = ent;
		//
		ent->s.eFlags &= ~EF_KAMIKAZE;
		// nuke the guy that used it
		G_Damage( ent, ent, ent, NULL, NULL, 100000, DAMAGE_NO_PROTECTION, MOD_KAMIKAZE );
	}
	else {
		if ( !strcmp(ent->activator->classname, "bodyque") ) {
			explosion->activator = &g_entities[ent->activator->r.ownerNum];
		}
		else {
			explosion->activator = ent->activator;
		}
	}

	// play global sound at all clients
	te = G_TempEntity(snapped, EV_GLOBAL_TEAM_SOUND );
	te->r.svFlags |= SVF_BROADCAST;
	te->s.eventParm = GTS_KAMIKAZE;
}
#endif
