// Copyright (C) 1999-2000 Id Software, Inc.
//
// g_weapon.c 
// perform the server side effects of a weapon firing

#include "g_local.h"

static	float	s_quadFactor;
static	vec3_t	forward, right, up;
static	vec3_t	muzzle;

extern	int		maxclients;		// CCH

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

	damage = 50 * s_quadFactor;
	G_Damage( traceEnt, ent, ent, forward, tr.endpos,
		damage, 0, MOD_GAUNTLET );

	// CCH: Add an uplifting punch to throw the target
	forward[2] += 0.5;
	VectorMA(traceEnt->client->ps.velocity, 1500, forward, traceEnt->client->ps.velocity);

	return qtrue;
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

#define MACHINEGUN_SPREAD	200
#define	MACHINEGUN_DAMAGE	7
#define	MACHINEGUN_TEAM_DAMAGE	5		// wimpier MG in teamplay
#define MAX_MG_HITS				4

// CCH: This function is now a hybrid of the original Bullet_Fire() and weapon_railgun_fire()
void Bullet_Fire (gentity_t *ent, float spread, int damage ) {
	trace_t		trace;
	vec3_t		end;
	float		r;
	float		u;
	gentity_t	*tent;
	gentity_t	*traceEnt;
	int			i;
	int			hits;
	int			unlinked;
	gentity_t	*unlinkedEntities[MAX_MG_HITS];

	damage *= 2;
	damage *= s_quadFactor;

	r = crandom()*spread;
	u = crandom()*spread;
	VectorMA (muzzle, 8192, forward, end);
	VectorMA (end, r, right, end);
	VectorMA (end, u, up, end);

	// trace only against the solids, so the railgun will go through people
	unlinked = 0;
	hits = 0;
	do {
		trap_Trace (&trace, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT );
		if ( trace.entityNum >= ENTITYNUM_MAX_NORMAL ) {
			break;
		}
		traceEnt = &g_entities[ trace.entityNum ];
		if ( traceEnt->takedamage ) {
			if( LogAccuracyHit( traceEnt, ent ) ) {
				hits++;
			}
			G_Damage (traceEnt, ent, ent, forward, trace.endpos, damage, 0, MOD_MACHINEGUN);
		}
		if ( trace.contents & CONTENTS_SOLID ) {
			break;		// we hit something solid enough to stop the beam
		}
		// unlink this entity, so the next trace will go past it
		trap_UnlinkEntity( traceEnt );
		unlinkedEntities[unlinked] = traceEnt;
		unlinked++;
	} while ( unlinked < MAX_MG_HITS );

	// link back in any entities we unlinked
	for ( i = 0 ; i < unlinked ; i++ ) {
		trap_LinkEntity( unlinkedEntities[i] );
	}

	// the final trace endpos will be the terminal point of the rail trail

	// snap the endpos to integers to save net bandwidth, but nudged towards the line
	SnapVectorTowards( trace.endpos, muzzle );

	// send railgun beam effect
	tent = G_TempEntity( trace.endpos, EV_RAILTRAIL );

	// set player number for custom colors on the railtrail
	tent->s.clientNum = ent->s.clientNum;

	VectorCopy( muzzle, tent->s.origin2 );
	// move origin a bit to come closer to the drawn gun muzzle
	VectorMA( tent->s.origin2, 4, right, tent->s.origin2 );
	VectorMA( tent->s.origin2, -1, up, tent->s.origin2 );

	// no explosion at end if SURF_NOIMPACT, but still make the trail
	if ( trace.surfaceFlags & SURF_NOIMPACT ) {
		tent->s.eventParm = 255;	// don't make the explosion at the end
		tent->s.clientNum = ent->s.clientNum;
		return;
	}

	tent = G_TempEntity( trace.endpos, EV_BULLET_HIT_WALL );
	tent->s.eventParm = DirToByte( trace.plane.normal );
	tent->s.otherEntityNum = ent->s.number;
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
}


/*
======================================================================

SHOTGUN

======================================================================
*/

/*
================
CCH: shotgun_explosive

3. There is a proper season for making attacks with fire, 
and special days for starting a conflagration.
--Art of War - Sun-Tzu tr. by Lionel Giles, M. A.
================
*/
void shotgun_explosive( gentity_t *ent, trace_t *trace ) {
	gentity_t	*tent;

	SnapVectorTowards( trace->endpos, muzzle );	// save net bandwidth

	tent = G_TempEntity( trace->endpos, EV_MISSILE_MISS );
	tent->s.eventParm = DirToByte( trace->plane.normal );
	tent->s.weapon = WP_GRENADE_LAUNCHER;

	G_RadiusDamage( trace->endpos, ent, 5 * s_quadFactor /* splashDamage */, 50 /* splashRadius */, 
		NULL, MOD_SHOTGUN_SPLASH );
}

// DEFAULT_SHOTGUN_SPREAD and DEFAULT_SHOTGUN_COUNT	are in bg_public.h, because
// client predicts same spreads
#define	DEFAULT_SHOTGUN_DAMAGE	10

qboolean ShotgunPellet( vec3_t start, vec3_t end, gentity_t *ent ) {
	trace_t		tr;
	int			damage;
	gentity_t		*traceEnt;

	trap_Trace (&tr, start, NULL, NULL, end, ent->s.number, MASK_SHOT);
	traceEnt = &g_entities[ tr.entityNum ];

	// send bullet impact
	if (  tr.surfaceFlags & SURF_NOIMPACT ) {
		return qfalse;
	}

	if ( traceEnt->takedamage) {
		damage = DEFAULT_SHOTGUN_DAMAGE * s_quadFactor;

		G_Damage( traceEnt, ent, ent, forward, tr.endpos,
			damage, 0, MOD_SHOTGUN);
		shotgun_explosive(ent, &tr);				// CCH
		if( LogAccuracyHit( traceEnt, ent ) ) {
			return qtrue;
		}
	}
	else {
		shotgun_explosive(ent, &tr);				// CCH
	}

	return qfalse;
}

// this should match CG_ShotgunPattern
void ShotgunPattern( vec3_t origin, vec3_t origin2, int seed, gentity_t *ent ) {
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
	for ( i = 0 ; i < DEFAULT_SHOTGUN_COUNT ; i++ ) {
		r = Q_crandom( &seed ) * DEFAULT_SHOTGUN_SPREAD;
		u = Q_crandom( &seed ) * DEFAULT_SHOTGUN_SPREAD;
		VectorMA( origin, 8192, forward, end);
		VectorMA (end, r, right, end);
		VectorMA (end, u, up, end);
		if( ShotgunPellet( origin, end, ent ) && !hitClient ) {
			hitClient = qtrue;
			ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
		}
	}
}


void weapon_supershotgun_fire (gentity_t *ent) {
	gentity_t		*tent;

	// send shotgun blast
	tent = G_TempEntity( muzzle, EV_SHOTGUN );
	VectorScale( forward, 4096, tent->s.origin2 );
	SnapVector( tent->s.origin2 );
	tent->s.eventParm = rand() & 255;		// seed for spread pattern
	tent->s.otherEntityNum = ent->s.number;

	ShotgunPattern( tent->s.pos.trBase, tent->s.origin2, tent->s.eventParm, ent );
}


/*
======================================================================

GRENADE LAUNCHER

======================================================================
*/

void weapon_grenadelauncher_fire (gentity_t *ent) {
	gentity_t	*m;

	// extra vertical velocity
	forward[2] += 0.2;
	VectorNormalize( forward );

	m = fire_grenade (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
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

/*
======================================================================

RAILGUN

======================================================================
*/

/*
================
CCH: railgun_antimatter

Doubtless the Almighty God will use some highly scientific means, whether including antimatter or
not, to dispose of the surplus of decaying bodies in a speedy and sanitary way.
--List of Prophecies - Chronological list of Prophecies from Watchtower - Prominent Bethelite
================
*/
void railgun_antimatter( gentity_t *ent, trace_t *trace ) {
	gentity_t	*tent;

	SnapVectorTowards( trace->endpos, muzzle );	// save net bandwidth

	tent = G_TempEntity( trace->endpos, EV_MISSILE_MISS );
	tent->s.eventParm = DirToByte( trace->plane.normal );
	tent->s.weapon = WP_GRENADE_LAUNCHER;

	G_RadiusDamage( trace->endpos, ent, 100 * s_quadFactor /* splashDamage */, 150 /* splashRadius */, 
		NULL, MOD_ANTIMATTER_SPLASH );
}

/*
=================
weapon_railgun_fire
=================
*/
#define	MAX_RAIL_HITS	4
void weapon_railgun_fire (gentity_t *ent) {
	vec3_t		end;
	trace_t		trace;
	gentity_t	*tent;
	gentity_t	*traceEnt;
	int			damage;
	int			radiusDamage;
	int			i;
	int			hits;
	int			unlinked;
	gentity_t	*unlinkedEntities[MAX_RAIL_HITS];

	damage = 100 * s_quadFactor;
	radiusDamage = 30 * s_quadFactor;

	VectorMA (muzzle, 8192, forward, end);

	// trace only against the solids, so the railgun will go through people
	unlinked = 0;
	hits = 0;
	do {
		trap_Trace (&trace, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT );
		if ( trace.entityNum >= ENTITYNUM_MAX_NORMAL ) {
			break;
		}
		traceEnt = &g_entities[ trace.entityNum ];
		if ( traceEnt->takedamage ) {
			if( LogAccuracyHit( traceEnt, ent ) ) {
				hits++;
			}
			G_Damage (traceEnt, ent, ent, forward, trace.endpos, damage, 0, MOD_RAILGUN);
		}
		if ( trace.contents & CONTENTS_SOLID ) {
			break;		// we hit something solid enough to stop the beam
		}
		// unlink this entity, so the next trace will go past it
		trap_UnlinkEntity( traceEnt );
		unlinkedEntities[unlinked] = traceEnt;
		unlinked++;
	} while ( unlinked < MAX_RAIL_HITS );

	// link back in any entities we unlinked
	for ( i = 0 ; i < unlinked ; i++ ) {
		trap_LinkEntity( unlinkedEntities[i] );
	}

	// the final trace endpos will be the terminal point of the rail trail

	// snap the endpos to integers to save net bandwidth, but nudged towards the line
	SnapVectorTowards( trace.endpos, muzzle );

	// send railgun beam effect
	tent = G_TempEntity( trace.endpos, EV_RAILTRAIL );

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
		railgun_antimatter(ent, &trace);				// CCH
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
			ent->client->ps.persistant[PERS_REWARD_COUNT]++;
			ent->client->ps.persistant[PERS_REWARD] = REWARD_IMPRESSIVE;
			ent->client->ps.persistant[PERS_IMPRESSIVE_COUNT]++;
			// add the sprite over the player's head
			ent->client->ps.eFlags &= ~(EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET );
			ent->client->ps.eFlags |= EF_AWARD_IMPRESSIVE;
			ent->client->rewardTime = level.time + REWARD_SPRITE_TIME;
		}
		ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
	}

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

/*
================
CCH: chain_lightning

Wis 5:21 Then shall the right aiming thunderbolts go abroad; ...
--Apocrypha
================
*/
static gentity_t	*cltargets[MAX_CLIENTS];

void chain_lightning( gentity_t *ent, gentity_t *target ) {
	trace_t		tr, targettr;
	vec3_t		vecto;
	gentity_t	*tent, *oldtarget;
	float		tentlength, targetlength;
	int			damage, i, index, arrlen;
	qboolean	found;
//	float		s_quadFactor;

	cltargets[0] = target;
	arrlen = 1;
	damage = 8 * s_quadFactor;

	while (1) {
		oldtarget = target;
		target = NULL;
		targetlength = LIGHTNING_RANGE;
		for (i = 0; i < maxclients; i++) {
			// Here we use tent to point to potential targets
			tent = &g_entities[i];

			if ( !tent->inuse ) continue;
			if ( tent == ent ) continue;
			if ( OnSameTeam( tent, ent ) ) continue;

			VectorSubtract(oldtarget->r.currentOrigin, tent->r.currentOrigin, vecto);
			tentlength = VectorLength(vecto);
			if ( tentlength > targetlength ) continue;

			found = qfalse;
			for (index = 0; index < arrlen; index++) {
				if (cltargets[index] == tent) {
					found = qtrue;
					continue;
				}
			}
			if ( found ) continue;

			trap_Trace( &tr, oldtarget->r.currentOrigin, NULL, NULL, tent->r.currentOrigin, ENTITYNUM_NONE, MASK_SHOT );
			if ( tent != &g_entities[tr.entityNum] ) continue;

			target = tent;
			targetlength = tentlength;
			targettr = tr;
		}

		if (!target) break;

		cltargets[arrlen] = target;
		arrlen++;

		// Here we use tent for the creation of temporary event entities
		tent = G_TempEntity( targettr.endpos, EV_MISSILE_HIT );
		tent->s.otherEntityNum = target->s.number;
		tent->s.eventParm = DirToByte( targettr.plane.normal );
		tent->s.weapon = WP_LIGHTNING;

		if ( target->takedamage) {
			G_Damage( target, ent, ent, vecto, targettr.endpos,
				damage, 0, MOD_LIGHTNING);
			tent = G_TempEntity(oldtarget->r.currentOrigin, EV_LIGHTNING);
			VectorCopy(target->r.currentOrigin, tent->s.origin2);
		}
	}
}

void Weapon_LightningFire( gentity_t *ent ) {
	trace_t		tr;
	vec3_t		end;
	gentity_t	*traceEnt, *tent;
	int			damage;

	damage = 8 * s_quadFactor;

	VectorMA( muzzle, LIGHTNING_RANGE, forward, end );

	trap_Trace( &tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT );

	if ( tr.entityNum == ENTITYNUM_NONE ) {
		return;
	}

	traceEnt = &g_entities[ tr.entityNum ];

	if ( traceEnt->takedamage && traceEnt->client ) {
		tent = G_TempEntity( tr.endpos, EV_MISSILE_HIT );
		tent->s.otherEntityNum = traceEnt->s.number;
		tent->s.eventParm = DirToByte( tr.plane.normal );
		tent->s.weapon = ent->s.weapon;
		if( LogAccuracyHit( traceEnt, ent ) ) {
			ent->client->ps.persistant[PERS_ACCURACY_HITS]++;
		}
	} else if ( !( tr.surfaceFlags & SURF_NOIMPACT ) ) {
		tent = G_TempEntity( tr.endpos, EV_MISSILE_MISS );
		tent->s.eventParm = DirToByte( tr.plane.normal );
	}

	if ( traceEnt->takedamage) {
		G_Damage( traceEnt, ent, ent, forward, tr.endpos,
			damage, 0, MOD_LIGHTNING);
		chain_lightning(ent, traceEnt);						// CCH
	}
}

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



/*
===============
FireWeapon
===============
*/
void FireWeapon( gentity_t *ent ) {
	if (ent->client->ps.powerups[PW_QUAD] ) {
		s_quadFactor = g_quadfactor.value;
	} else {
		s_quadFactor = 1;
	}

	// track shots taken for accuracy tracking.  Grapple is not a weapon and gauntet is just not tracked
	if( ent->s.weapon != WP_GRAPPLING_HOOK && ent->s.weapon != WP_GAUNTLET ) {
		ent->client->ps.persistant[PERS_ACCURACY_SHOTS]++;
	}

	// set aiming directions
	AngleVectors (ent->client->ps.viewangles, forward, right, up);

	CalcMuzzlePoint ( ent, forward, right, up, muzzle );

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
		if ( g_gametype.integer != GT_TEAM ) {
			Bullet_Fire( ent, MACHINEGUN_SPREAD, MACHINEGUN_DAMAGE );
		} else {
			Bullet_Fire( ent, MACHINEGUN_SPREAD, MACHINEGUN_TEAM_DAMAGE );
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
		weapon_railgun_fire( ent );
		break;
	case WP_BFG:
		BFG_Fire( ent );
		break;
	case WP_GRAPPLING_HOOK:
		Weapon_GrapplingHook_Fire( ent );
		break;
	default:
// FIXME		G_Error( "Bad ent->s.weapon" );
		break;
	}
}

