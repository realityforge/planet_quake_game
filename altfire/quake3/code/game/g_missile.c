// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "g_local.h"

#define	MISSILE_PRESTEP_TIME	50
//NT - just a few defs for obvious reasons
#define FIRST_HOMING_THINK		500
#define HOMING_THINK			60
#define HOMING_MISSILE_SPEED	400

#define FIRST_ORBITING_THINK	30
#define ORBITING_THINK			15
#define ORBITING_MISSILE_SPEED	600
#define ORBITING_SPEED_SPREAD	200

#define MISSILE_SPEED			900

#define FIRST_PROXIMITY_THINK	3000
#define PROXIMITY_LIFE			25000


/*
================
G_BounceMissile

================
*/
void G_BounceMissile( gentity_t *ent, trace_t *trace ) {
	vec3_t	velocity;
	float	dot;
	int		hitTime;

	// reflect the velocity on the trace plane
	hitTime = level.previousTime + ( level.time - level.previousTime ) * trace->fraction;

	BG_EvaluateTrajectoryDelta( &ent->s.pos, hitTime, velocity );

	dot = DotProduct( velocity, trace->plane.normal );
	VectorMA( velocity, -2*dot, trace->plane.normal, ent->s.pos.trDelta );

	if ( ent->s.eFlags & EF_BOUNCE_HALF ) {
		VectorScale( ent->s.pos.trDelta, 0.65, ent->s.pos.trDelta );
		// check for stop
		if ( trace->plane.normal[2] > 0.2 && VectorLength( ent->s.pos.trDelta ) < 40 ) {
			G_SetOrigin( ent, trace->endpos );
			return;
		}
	}

	VectorAdd( ent->r.currentOrigin, trace->plane.normal, ent->r.currentOrigin);
	VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );
	ent->s.pos.trTime = level.time;
}


/*
================
G_ExplodeMissile

Explode a missile without an impact
================
*/
void G_ExplodeMissile( gentity_t *ent ) {
	vec3_t		dir;
	vec3_t		origin;

	BG_EvaluateTrajectory( &ent->s.pos, level.time, origin );

	SnapVector( origin );
	G_SetOrigin( ent, origin );

	// we don't have a valid direction, so just point straight up
	dir[0] = dir[1] = 0;
	dir[2] = 1;

	//NT - half-size the chaff explosion
	if ( ent->s.weapon == WP_CHAFF )
		ent->s.generic1 = 8;
	else
		ent->s.generic1 = 0;

	//NT - double-size the BFG overheat
	if ( ent->methodOfDeath == MOD_BFG_OVERHEAT ) {
		ent->s.generic1 = 32;
	}

	ent->s.eType = ET_GENERAL;
	G_AddEvent( ent, EV_MISSILE_MISS, DirToByte( dir ) );

	ent->freeAfterEvent = qtrue;

	// splash damage
	if ( ent->splashDamage ) {
		//NT - added push flag
		if( G_RadiusDamage( ent->r.currentOrigin, ent->parent, ent->splashDamage, ent->splashRadius, ent
			, ent->splashMethodOfDeath, qtrue ) ) {
			g_entities[ent->r.ownerNum].client->accuracy_hits++;
		}
	}

	trap_LinkEntity( ent );
}

//NT - anything that sticks and can be damaged dies with this
/*
================
G_Stick_Die
================
*/
void G_Stick_Die( gentity_t *ent, gentity_t *inflictor, gentity_t *attacker, int damage, int mod ) {
	ent->nextthink = level.time + 1;
	ent->think = G_ExplodeMissile;
}

//NT - anything that flies and can be damaged dies with this
/*
================
G_Missile_Die
================
*/
void G_Missile_Die( gentity_t *ent, gentity_t *inflictor, gentity_t *attacker, int damage, int mod ) {
	ent->nextthink = level.time + 1;
	ent->think = G_ExplodeMissile;
}


//NT - when a sticky missile hits a wall
/*
================
G_StickMissile

================
*/
void G_StickMissile( gentity_t *ent, trace_t *trace ) {
	VectorCopy(trace->endpos, ent->r.currentOrigin);
	VectorCopy(trace->endpos, ent->s.pos.trBase);
	ent->s.time = level.time;
	ent->s.pos.trType = TR_STATIONARY;
}

//NT - someone got too close
void G_ProximityGrenadeTrigger( gentity_t *ent ) {
	G_TempEntity( ent->r.currentOrigin, EV_PROX_GREN_TRIGGER );

	ent->nextthink = level.time + 500;
	ent->think = G_ExplodeMissile;

	ent->s.generic1 = 0;
}

//NT - takes a while to arm a proximity grenade
void G_ArmProximityGrenade( gentity_t *ent ) {
	ent->s.generic1 = 1;
	ent->nextthink = level.time + PROXIMITY_LIFE - FIRST_PROXIMITY_THINK;
	ent->think = G_ProximityGrenadeTrigger;
}

//NT - proximity grenade looks around for entities to blow up on
void G_ProximityGrenade( gentity_t *ent ) {
	gentity_t	*target = NULL;
	gentity_t	*blip = NULL;

	if ( ent->s.time2 < 1000 )
		ent->s.time2 += level.time - level.previousTime;

	if ( ent->s.time2 >= 1000 ) {
		gentity_t	*tent;

		tent = G_TempEntity( ent->r.currentOrigin, EV_PROXIMITY_TICK );
		ent->s.time2 -= 1000;		
	}

	while ((blip = FindRadius(blip, ent->r.currentOrigin, 128 * 128)) != NULL) {
		if (blip->s.weapon != WP_CHAFF)
		{
			if (blip->client==NULL)			continue;
			if (blip==ent->parent)			continue;
			if (blip->health<=0)			continue;
			if (blip->client->sess.sessionTeam >= TEAM_SPECTATOR)
				continue;

			if ((g_gametype.integer == GT_TEAM || g_gametype.integer == GT_CTF) &&
				OnSameTeam( blip, ent->parent ) )
				continue;
		}

		if (!IsVisible (ent, blip))
			continue;

		target = blip;
		break;
	}

	if ( target != NULL )
	{
		G_ProximityGrenadeTrigger( ent );
	}
}

//NT - homing missile's think function - find the closest target and go for it
void G_HomingMissile( gentity_t *ent ) {
	gentity_t	*target = NULL;
	gentity_t	*blip = NULL;
	vec3_t		dir, blipdir;
	vec_t		dot, cs;
	qboolean	chaff;
	qboolean	ignorechaff = qfalse;

	if (ent->parent->health <= 0)
	{
		ent->nextthink = level.time + 15000;
		ent->think = G_ExplodeMissile;
		return;
	}

	if ( ent->parent && ent->parent->client ) {
		ignorechaff = ( ent->parent->client->ps.powerups[PW_ACCURACY] > 0 );
	}

	while ((blip = FindRadius(blip, ent->r.currentOrigin, 2000 * 2000)) != NULL)	{
		if (blip->s.weapon == WP_CHAFF)
		{
			if ( ignorechaff ) {
				continue;
			}

			chaff = qtrue;
		}
		else
		{
			chaff = qfalse;

			if (blip->client==NULL)			continue;
			if (blip==ent->parent)			continue;
			if (blip->health<=0)			continue;
			if (blip->client->sess.sessionTeam >= TEAM_SPECTATOR)
				continue;

			if ((g_gametype.integer == GT_TEAM || g_gametype.integer == GT_CTF) &&
				OnSameTeam( blip, ent->parent ) )
				continue;
		}

		if (!IsVisible (ent, blip))
			continue;

		VectorSubtract(blip->r.currentOrigin, ent->r.currentOrigin, blipdir);

		//NT - give chaff a higher weight
		if (chaff)
		{
			VectorScale(blipdir, 0.5, blipdir);
		}

		if ((target == NULL) || (VectorLength(blipdir) < VectorLength(dir)))
		{
			if (chaff)
			{
				VectorScale(blipdir, 2, blipdir);
			}

			// E.B = |E||B|cos(theta)
			dot = _DotProduct(ent->r.currentAngles, blipdir);

			// Divide E.B by |E||B| to get cos(theta)
			cs = dot / (VectorLength(ent->r.currentAngles) * VectorLength(blipdir));

			// If angle is less than 120 degrees
			if (cs > cos(120.0f * M_PI / 180.0f))
			{
				// We add it as our target
				target = blip;
				VectorCopy(blipdir, dir);
			}
		}
	}

	if (target == NULL)	{
		ent->nextthink = level.time + HOMING_THINK; // + 10000;
		ent->think = G_HomingMissile;
	} else {
		// for exact trajectory calculation, set current point to base.
		VectorCopy(ent->r.currentOrigin, ent->s.pos.trBase );

		VectorNormalize(dir);
		// 0.5 is swing rate.
		VectorScale(dir, 0.5, dir);
		VectorAdd(dir,ent->r.currentAngles,dir);

		// turn nozzle to target angle
		VectorNormalize(dir);
		VectorCopy(dir, ent->r.currentAngles);

		// scale direction, put into trDelta
		VectorScale(dir, ent->constantSpeed, dir);
		VectorCopy(dir, ent->s.pos.trDelta);

		ent->s.pos.trTime = level.time;

		SnapVector (ent->s.pos.trDelta);                      // save net bandwidth
		ent->nextthink = level.time + HOMING_THINK;	// decrease this value also makes fast swing
		ent->think = G_HomingMissile;
	}
}

//NT - orbiting missile's think function
void G_OrbitingMissile( gentity_t *ent ) {
	gentity_t	*parent;

	parent = ent->parent;

	VectorCopy(ent->parent->s.pos.trBase, ent->s.pos.trBase);
	VectorCopy(ent->parent->s.pos.trDelta, ent->s.pos.trDelta);
	ent->s.pos.trTime = ent->parent->s.pos.trTime;

	ent->s.pos.trBase[2] += 8;

	if ( ent->parent->client->sess.sessionTeam >= TEAM_SPECTATOR ) {
		// make it invisible and massless
		ent->r.svFlags |= SVF_NOCLIENT;
		ent->s.eFlags |= EF_NODRAW;
		ent->clipmask = 0;
	} else {
		ent->r.svFlags &= ~SVF_NOCLIENT;
		ent->s.eFlags &= ~EF_NODRAW;
		ent->clipmask = CONTENTS_BODY;
	}

	ent->nextthink = level.time + ORBITING_THINK;
	ent->think = G_OrbitingMissile;
}

#ifdef MISSIONPACK
/*
================
ProximityMine_Explode
================
*/
static void ProximityMine_Explode( gentity_t *mine ) {
	G_ExplodeMissile( mine );
	// if the prox mine has a trigger free it
	if (mine->activator) {
		G_FreeEntity(mine->activator);
		mine->activator = NULL;
	}
}

/*
================
ProximityMine_Die
================
*/
static void ProximityMine_Die( gentity_t *ent, gentity_t *inflictor, gentity_t *attacker, int damage, int mod ) {
	ent->think = ProximityMine_Explode;
	ent->nextthink = level.time + 1;
}

/*
================
ProximityMine_Trigger
================
*/
void ProximityMine_Trigger( gentity_t *trigger, gentity_t *other, trace_t *trace ) {
	vec3_t		v;
	gentity_t	*mine;

	if( !other->client ) {
		return;
	}

	// trigger is a cube, do a distance test now to act as if it's a sphere
	VectorSubtract( trigger->s.pos.trBase, other->s.pos.trBase, v );
	if( VectorLength( v ) > trigger->parent->splashRadius ) {
		return;
	}


	if ( g_gametype.integer >= GT_TEAM ) {
		// don't trigger same team mines
		if (trigger->parent->s.generic1 == other->client->sess.sessionTeam) {
			return;
		}
	}

	// ok, now check for ability to damage so we don't get triggered thru walls, closed doors, etc...
	if( !CanDamage( other, trigger->s.pos.trBase ) ) {
		return;
	}

	// trigger the mine!
	mine = trigger->parent;
	mine->s.loopSound = 0;
	G_AddEvent( mine, EV_PROXIMITY_MINE_TRIGGER, 0 );
	mine->nextthink = level.time + 500;

	G_FreeEntity( trigger );
}

/*
================
ProximityMine_Activate
================
*/
static void ProximityMine_Activate( gentity_t *ent ) {
	gentity_t	*trigger;
	float		r;

	ent->think = ProximityMine_Explode;
	ent->nextthink = level.time + g_proxMineTimeout.integer;

	ent->takedamage = qtrue;
	ent->health = 1;
	ent->die = ProximityMine_Die;

	ent->s.loopSound = G_SoundIndex( "sound/weapons/proxmine/wstbtick.wav" );

	// build the proximity trigger
	trigger = G_Spawn ();

	trigger->classname = "proxmine_trigger";

	r = ent->splashRadius;
	VectorSet( trigger->r.mins, -r, -r, -r );
	VectorSet( trigger->r.maxs, r, r, r );

	G_SetOrigin( trigger, ent->s.pos.trBase );

	trigger->parent = ent;
	trigger->r.contents = CONTENTS_TRIGGER;
	trigger->touch = ProximityMine_Trigger;

	trap_LinkEntity (trigger);

	// set pointer to trigger so the entity can be freed when the mine explodes
	ent->activator = trigger;
}

/*
================
ProximityMine_ExplodeOnPlayer
================
*/
static void ProximityMine_ExplodeOnPlayer( gentity_t *mine ) {
	gentity_t	*player;

	player = mine->enemy;
	player->client->ps.eFlags &= ~EF_TICKING;

	if ( player->client->invulnerabilityTime > level.time ) {
		G_Damage( player, mine->parent, mine->parent, vec3_origin, mine->s.origin, 1000, DAMAGE_NO_KNOCKBACK, MOD_JUICED );
		player->client->invulnerabilityTime = 0;
		G_TempEntity( player->client->ps.origin, EV_JUICED );
	}
	else {
		G_SetOrigin( mine, player->s.pos.trBase );
		// make sure the explosion gets to the client
		mine->r.svFlags &= ~SVF_NOCLIENT;
		mine->splashMethodOfDeath = MOD_PROXIMITY_MINE;
		G_ExplodeMissile( mine );
	}
}

/*
================
ProximityMine_Player
================
*/
static void ProximityMine_Player( gentity_t *mine, gentity_t *player ) {
	if( mine->s.eFlags & EF_NODRAW ) {
		return;
	}

	G_AddEvent( mine, EV_PROXIMITY_MINE_STICK, 0 );

	if( player->s.eFlags & EF_TICKING ) {
		player->activator->splashDamage += mine->splashDamage;
		player->activator->splashRadius *= 1.50;
		mine->think = G_FreeEntity;
		mine->nextthink = level.time;
		return;
	}

	player->client->ps.eFlags |= EF_TICKING;
	player->activator = mine;

	mine->s.eFlags |= EF_NODRAW;
	mine->r.svFlags |= SVF_NOCLIENT;
	mine->s.pos.trType = TR_LINEAR;
	VectorClear( mine->s.pos.trDelta );

	mine->enemy = player;
	mine->think = ProximityMine_ExplodeOnPlayer;
	if ( player->client->invulnerabilityTime > level.time ) {
		mine->nextthink = level.time + 2 * 1000;
	}
	else {
		mine->nextthink = level.time + 10 * 1000;
	}
}
#endif

/*
================
G_MissileImpact
================
*/
void G_MissileImpact( gentity_t *ent, trace_t *trace ) {
	gentity_t		*other;
	qboolean		hitClient = qfalse;
#ifdef MISSIONPACK
	vec3_t			forward, impactpoint, bouncedir;
	int				eFlags;
#endif
	other = &g_entities[trace->entityNum];

	// check for bounce
	if ( !other->takedamage &&
		( ent->s.eFlags & ( EF_BOUNCE | EF_BOUNCE_HALF ) ) ) {
		G_BounceMissile( ent, trace );
		G_AddEvent( ent, EV_GRENADE_BOUNCE, 0 );
		return;
	}

	//NT - sticky projectiles don't stick to things that take damage
	if ( !other->takedamage &&
		( ent->s.eFlags & EF_STICK) ) {
		G_StickMissile( ent, trace );
		G_AddEvent( ent, EV_GRENADE_BOUNCE, 0 );
		return;
	}

#ifdef MISSIONPACK
	if ( other->takedamage ) {
		if ( ent->s.weapon != WP_PROX_LAUNCHER ) {
			if ( other->client && other->client->invulnerabilityTime > level.time ) {
				//
				VectorCopy( ent->s.pos.trDelta, forward );
				VectorNormalize( forward );
				if (G_InvulnerabilityEffect( other, forward, ent->s.pos.trBase, impactpoint, bouncedir )) {
					VectorCopy( bouncedir, trace->plane.normal );
					eFlags = ent->s.eFlags & EF_BOUNCE_HALF;
					ent->s.eFlags &= ~EF_BOUNCE_HALF;
					G_BounceMissile( ent, trace );
					ent->s.eFlags |= eFlags;
				}
				ent->target_ent = other;
				return;
			}
		}
	}
#endif
	// impact damage
	if (other->takedamage) {
		// FIXME: wrong damage direction?
		if ( ent->damage ) {
			vec3_t	velocity;

			if( LogAccuracyHit( other, &g_entities[ent->r.ownerNum] ) ) {
				g_entities[ent->r.ownerNum].client->accuracy_hits++;
				hitClient = qtrue;
			}
			BG_EvaluateTrajectoryDelta( &ent->s.pos, level.time, velocity );

			if ( VectorLength( velocity ) == 0 ) {
				velocity[2] = 1;	// stepped on a grenade
			}
			G_Damage (other, ent, &g_entities[ent->r.ownerNum], velocity,
				ent->s.origin, ent->damage, 
				0, ent->methodOfDeath);
		}
	}

#ifdef MISSIONPACK
	if( ent->s.weapon == WP_PROX_LAUNCHER ) {
		if( ent->s.pos.trType != TR_GRAVITY && ent->s.pos.trType != TR_SMALL_GRAVITY ) {
			return;
		}

		// if it's a player, stick it on to them (flag them and remove this entity)
		if( other->s.eType == ET_PLAYER && other->health > 0 ) {
			ProximityMine_Player( ent, other );
			return;
		}

		SnapVectorTowards( trace->endpos, ent->s.pos.trBase );
		G_SetOrigin( ent, trace->endpos );
		ent->s.pos.trType = TR_STATIONARY;
		VectorClear( ent->s.pos.trDelta );

		G_AddEvent( ent, EV_PROXIMITY_MINE_STICK, trace->surfaceFlags );

		ent->think = ProximityMine_Activate;
		ent->nextthink = level.time + 2000;

		vectoangles( trace->plane.normal, ent->s.angles );
		ent->s.angles[0] += 90;

		// link the prox mine to the other entity
		ent->enemy = other;
		ent->die = ProximityMine_Die;
		VectorCopy(trace->plane.normal, ent->movedir);
		VectorSet(ent->r.mins, -4, -4, -4);
		VectorSet(ent->r.maxs, 4, 4, 4);
		trap_LinkEntity(ent);

		return;
	}
#endif

	if (!strcmp(ent->classname, "hook")) {
		gentity_t *nent;
		vec3_t v;

		nent = G_Spawn();
		if ( other->takedamage && other->client ) {

			G_AddEvent( nent, EV_MISSILE_HIT, DirToByte( trace->plane.normal ) );
			nent->s.otherEntityNum = other->s.number;

			ent->enemy = other;

			v[0] = other->r.currentOrigin[0] + (other->r.mins[0] + other->r.maxs[0]) * 0.5;
			v[1] = other->r.currentOrigin[1] + (other->r.mins[1] + other->r.maxs[1]) * 0.5;
			v[2] = other->r.currentOrigin[2] + (other->r.mins[2] + other->r.maxs[2]) * 0.5;

			SnapVectorTowards( v, ent->s.pos.trBase );	// save net bandwidth
		} else {
			VectorCopy(trace->endpos, v);
			G_AddEvent( nent, EV_MISSILE_MISS, DirToByte( trace->plane.normal ) );
			ent->enemy = NULL;
		}

		SnapVectorTowards( v, ent->s.pos.trBase );	// save net bandwidth

		nent->freeAfterEvent = qtrue;
		// change over to a normal entity right at the point of impact
		nent->s.eType = ET_GENERAL;
		ent->s.eType = ET_GRAPPLE;

		G_SetOrigin( ent, v );
		G_SetOrigin( nent, v );

		ent->think = Weapon_HookThink;
		ent->nextthink = level.time + FRAMETIME;

		ent->parent->client->ps.pm_flags |= PMF_GRAPPLE_PULL;
		VectorCopy( ent->r.currentOrigin, ent->parent->client->ps.grapplePoint);

		trap_LinkEntity( ent );
		trap_LinkEntity( nent );

		return;
	}

	// is it cheaper in bandwidth to just remove this ent and create a new
	// one, rather than changing the missile into the explosion?

	if ( other->takedamage && other->client ) {
		G_AddEvent( ent, EV_MISSILE_HIT, DirToByte( trace->plane.normal ) );
		ent->s.otherEntityNum = other->s.number;
	} else if( trace->surfaceFlags & SURF_METALSTEPS ) {
		G_AddEvent( ent, EV_MISSILE_MISS_METAL, DirToByte( trace->plane.normal ) );
	} else {
		G_AddEvent( ent, EV_MISSILE_MISS, DirToByte( trace->plane.normal ) );
	}

	ent->freeAfterEvent = qtrue;

	// change over to a normal entity right at the point of impact
	ent->s.eType = ET_GENERAL;

	SnapVectorTowards( trace->endpos, ent->s.pos.trBase );	// save net bandwidth

	G_SetOrigin( ent, trace->endpos );

	// splash damage (doesn't apply to person directly hit)
	if ( ent->splashDamage ) {
		//NT - added push flag
		if( G_RadiusDamage( trace->endpos, ent->parent, ent->splashDamage, ent->splashRadius, 
			other, ent->splashMethodOfDeath, qtrue ) ) {
			if( !hitClient ) {
				g_entities[ent->r.ownerNum].client->accuracy_hits++;
			}
		}
	}

	trap_LinkEntity( ent );
}

/*
================
G_RunMissile
================
*/
void G_RunMissile( gentity_t *ent ) {
	vec3_t		origin;
	trace_t		tr;
	gentity_t	*traceEnt = NULL; //NT
	int			passent;

	// get current position
	BG_EvaluateTrajectory( &ent->s.pos, level.time, origin );

	// if this missile bounced off an invulnerability sphere
	if ( ent->target_ent ) {
		passent = ent->target_ent->s.number;
	}
#ifdef MISSIONPACK
	// prox mines that left the owner bbox will attach to anything, even the owner
	else if (ent->s.weapon == WP_PROX_LAUNCHER && ent->count) {
		passent = ENTITYNUM_NONE;
	}
#endif
	else {
		// ignore interactions with the missile owner
		passent = ent->r.ownerNum;
	}
	// trace a line from the previous position to the current position
	//NT - check to see if the missile is ignoring the min/max
	if ( ent->ignoresize )
		trap_Trace( &tr, ent->r.currentOrigin, NULL, NULL, origin, passent, ent->clipmask | CONTENTS_TRIGGER);
	else
		trap_Trace( &tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, origin, passent, ent->clipmask | CONTENTS_TRIGGER);

	if ( tr.entityNum != ENTITYNUM_NONE )
	{
		//NT - see if it's a teleport trigger and teleport the missile if it is
		traceEnt = &g_entities[ tr.entityNum ];
		if ( traceEnt && traceEnt->s.eType == ET_TELEPORT_TRIGGER ) {
			trigger_teleporter_touch ( traceEnt, ent, &tr );
			return;
		}
	}

	//NT - ignore the min/max?
	if ( ent->ignoresize )
		trap_Trace( &tr, ent->r.currentOrigin, NULL, NULL, origin, passent, ent->clipmask );
	else
		trap_Trace( &tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, origin, passent, ent->clipmask );

	if ( tr.startsolid || tr.allsolid ) {
		// make sure the tr.entityNum is set to the entity we're stuck in
		if ( ent->ignoresize )
			trap_Trace( &tr, ent->r.currentOrigin, NULL, NULL, ent->r.currentOrigin, passent, ent->clipmask );
		else
			trap_Trace( &tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, ent->r.currentOrigin, passent, ent->clipmask );
		tr.fraction = 0;
	}
	else {
		VectorCopy( tr.endpos, ent->r.currentOrigin );
	}

	trap_LinkEntity( ent );

	if ( tr.fraction != 1 ) {
		// never explode or bounce on sky
		if ( tr.surfaceFlags & SURF_NOIMPACT ) {
			// If grapple, reset owner
			if (ent->parent && ent->parent->client && ent->parent->client->hook == ent) {
				ent->parent->client->hook = NULL;
			}
			G_FreeEntity( ent );
			return;
		}
		G_MissileImpact( ent, &tr );
		if ( ent->s.eType != ET_MISSILE ) {
			return;		// exploded
		}
	}
#ifdef MISSIONPACK
	// if the prox mine wasn't yet outside the player body
	if (ent->s.weapon == WP_PROX_LAUNCHER && !ent->count) {
		// check if the prox mine is outside the owner bbox
		trap_Trace( &tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, ent->r.currentOrigin, ENTITYNUM_NONE, ent->clipmask );
		if (!tr.startsolid || tr.entityNum != ent->r.ownerNum) {
			ent->count = 1;
		}
	}
#endif

	//NT - make the proximity grenade think
	if ( ent->s.generic1 == 1 && ent->s.weapon == WP_PROX_LAUNCHER ) {
		G_ProximityGrenade( ent );
	}

	// check think function after bouncing
	G_RunThink( ent );
}

//=============================================================================

/*
=================
fire_plasma

=================
*/
gentity_t *fire_plasma (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "plasma";
	bolt->nextthink = level.time + 10000;
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_PLASMAGUN;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = 20;
	bolt->splashDamage = 15;
	bolt->splashRadius = 20;
	bolt->methodOfDeath = MOD_PLASMA;
	bolt->splashMethodOfDeath = MOD_PLASMA_SPLASH;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	VectorScale( dir, 2000, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth

	VectorCopy (start, bolt->r.currentOrigin);

	return bolt;
}	

//NT - orbiting plasma
/*
=================
fire_plasma2

=================
*/
gentity_t *fire_plasma2 (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;
	static int	seed = 0xAC;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "plasma2";
	bolt->nextthink = level.time + FIRST_ORBITING_THINK;
	bolt->think = G_OrbitingMissile;
	bolt->lastthinktime = level.time + 30000;
	bolt->lastthink = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_PLASMAGUN2;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = 40;
	bolt->splashDamage = 0;
	bolt->splashRadius = 0;
	bolt->methodOfDeath = MOD_ORBITING_PLASMA;
	bolt->splashMethodOfDeath = MOD_ORBITING_PLASMA;
	bolt->clipmask = CONTENTS_BODY;
	bolt->target_ent = NULL;

	bolt->s.pos.trType = TR_ORBITAL;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame

	bolt->s.pos.trDuration = abs( Q_rand( &seed ) ) % 360;

	VectorCopy( self->s.pos.trBase, bolt->s.pos.trBase );
	VectorCopy( self->s.pos.trDelta, bolt->s.pos.trDelta );
	bolt->s.pos.trTime = self->s.pos.trTime;

	VectorCopy( bolt->s.pos.trBase, bolt->r.currentOrigin );
	VectorCopy( bolt->s.pos.trDelta, bolt->r.currentAngles );

	return bolt;
}	

//=============================================================================

//NT - chaff
/*
=================
fire_chaff
=================
*/
gentity_t *fire_chaff (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "chaff";
	bolt->nextthink = level.time + 2500;
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_CHAFF;
	bolt->s.eFlags = EF_STICK;
	bolt->takedamage = qtrue;
	bolt->health = 10;
	bolt->die = G_Stick_Die;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = 10;
	bolt->splashDamage = 10;
	bolt->splashRadius = 15;
	bolt->methodOfDeath = MOD_CHAFF;
	bolt->splashMethodOfDeath = MOD_CHAFF_SPLASH;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;
	bolt->s.otherEntityNum2 = ENTITYNUM_WORLD;

	bolt->s.pos.trType = TR_SMALL_GRAVITY;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	VectorScale( dir, 700, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth

	VectorCopy (start, bolt->r.currentOrigin);

	return bolt;
}


/*
=================
fire_grenade
=================
*/
gentity_t *fire_grenade (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;
	vec3_t		mins = { -4, -4, -4 }, maxs = { 4, 4, 4 }; //NT

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "grenade";
	bolt->nextthink = level.time + 2500;
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_GRENADE_LAUNCHER;
	bolt->s.eFlags = EF_BOUNCE_HALF;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = 100;
	bolt->splashDamage = 100;
	bolt->splashRadius = 150;
	bolt->methodOfDeath = MOD_GRENADE;
	bolt->splashMethodOfDeath = MOD_GRENADE_SPLASH;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;
	bolt->s.otherEntityNum2 = ENTITYNUM_WORLD;

	//NT - make the grenades shootable (also keeps them out of the floor)
	bolt->r.contents = CONTENTS_SHOOTABLE;
	VectorCopy(mins, bolt->r.mins);
	VectorCopy(maxs, bolt->r.maxs);
	bolt->takedamage = qtrue;
	bolt->health = 45;
	bolt->die = G_Missile_Die;

	bolt->s.pos.trType = TR_GRAVITY;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	VectorScale( dir, 700, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth

	VectorCopy (start, bolt->r.currentOrigin);


	return bolt;
}

//NT - proximity grenades
/*
=================
fire_proximity_grenade
=================
*/
gentity_t *fire_proximity_grenade (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;
	vec3_t		mins = { -4, -4, -4 }, maxs = { 4, 4, 4 };

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "proximityGrenade";
	bolt->nextthink = level.time + FIRST_PROXIMITY_THINK;
	bolt->think = G_ArmProximityGrenade;
	bolt->s.generic1 = 0;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_PROX_LAUNCHER;
	bolt->s.eFlags = EF_STICK;
 	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = 100;
	bolt->splashDamage = 125;
	bolt->splashRadius = 256;
	bolt->methodOfDeath = MOD_GRENADE;
	bolt->splashMethodOfDeath = MOD_PROX_SPLASH;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;
	bolt->s.otherEntityNum2 = self->s.number;
	bolt->s.time2 = random() * 1000;

	//NT - give the grenade a size
	bolt->r.contents = CONTENTS_SHOOTABLE;
	VectorCopy (mins, bolt->r.mins);
	VectorCopy (maxs, bolt->r.maxs);
	bolt->takedamage = qtrue;
	bolt->health = 90;
	bolt->die = G_Stick_Die;

	bolt->s.pos.trType = TR_GRAVITY;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	VectorScale( dir, 900, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth

	VectorCopy (start, bolt->r.currentOrigin);

	return bolt;
}

//=============================================================================


/*
=================
fire_bfg
=================
*/
gentity_t *fire_bfg (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "bfg";
	bolt->nextthink = level.time + 10000;
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_BFG;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = 100;
	bolt->splashDamage = 100;
	bolt->splashRadius = 120;
	bolt->methodOfDeath = MOD_BFG;
	bolt->splashMethodOfDeath = MOD_BFG_SPLASH;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	VectorScale( dir, 2000, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy (start, bolt->r.currentOrigin);

	return bolt;
}

//=============================================================================

/*
=================
fire_rocket
=================
*/
gentity_t *fire_rocket (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;
	vec3_t		mins = { -8, -8, -8 }, maxs = { 8, 8, 8 };

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "rocket";
	bolt->nextthink = level.time + 15000;
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_ROCKET_LAUNCHER;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->clipmask = MASK_SHOT;
	bolt->damage = 100;
	bolt->splashDamage = 100;
	bolt->splashRadius = 120;
	bolt->methodOfDeath = MOD_ROCKET;
	bolt->splashMethodOfDeath = MOD_ROCKET_SPLASH;
	bolt->target_ent = NULL;
	bolt->s.otherEntityNum2 = ENTITYNUM_WORLD;

	//NT - make the rockets shootable
	bolt->r.contents = CONTENTS_SHOOTABLE;
	VectorCopy(mins, bolt->r.mins);
	VectorCopy(maxs, bolt->r.maxs);
	bolt->takedamage = qtrue;
	bolt->health = 35;
	bolt->die = G_Missile_Die;
	bolt->ignoresize = qtrue;

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	VectorScale( dir, MISSILE_SPEED, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy (start, bolt->r.currentOrigin);

	return bolt;
}

//NT - fire a homing missile
/*
=================
fire_homing
=================
*/
gentity_t *fire_homing (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*bolt;
	vec3_t		mins = { -8, -8, -8 }, maxs = { 8, 8, 8 };

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "rocket";
	bolt->think = G_HomingMissile;
	bolt->nextthink = level.time + FIRST_HOMING_THINK;
	bolt->lastthinktime = level.time + 15000;
	bolt->lastthink = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_ROCKET_LAUNCHER;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->clipmask = MASK_SHOT;
	bolt->damage = 100;
	bolt->splashDamage = 100;
	bolt->splashRadius = 120;
	bolt->methodOfDeath = MOD_HOMING;
	bolt->splashMethodOfDeath = MOD_HOMING_SPLASH;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;
	bolt->s.otherEntityNum2 = ENTITYNUM_WORLD;

	//NT - make the rockets shootable
	bolt->r.contents = CONTENTS_SHOOTABLE;
	VectorCopy(mins, bolt->r.mins);
	VectorCopy(maxs, bolt->r.maxs);
	bolt->takedamage = qtrue;
	bolt->health = 50;
	bolt->die = G_Missile_Die;
	bolt->ignoresize = qtrue;

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	bolt->constantSpeed = HOMING_MISSILE_SPEED;
	VectorCopy( start, bolt->s.pos.trBase );
	VectorScale( dir, bolt->constantSpeed, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth
	VectorCopy (start, bolt->r.currentOrigin);
	VectorCopy (dir, bolt->r.currentAngles);		// this is new line.

	return bolt;
}

/*
=================
fire_grapple
=================
*/
gentity_t *fire_grapple (gentity_t *self, vec3_t start, vec3_t dir) {
	gentity_t	*hook;

	VectorNormalize (dir);

	hook = G_Spawn();
	hook->classname = "hook";
	hook->nextthink = level.time + 10000;
	hook->think = Weapon_HookFree;
	hook->s.eType = ET_MISSILE;
	hook->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	hook->s.weapon = WP_GRAPPLING_HOOK;
	hook->r.ownerNum = self->s.number;
	hook->methodOfDeath = MOD_GRAPPLE;
	hook->clipmask = MASK_SHOT;
	hook->parent = self;
	hook->target_ent = NULL;

	hook->s.pos.trType = TR_LINEAR;
	hook->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	hook->s.otherEntityNum = self->s.number; // use to match beam in client
	VectorCopy( start, hook->s.pos.trBase );
	VectorScale( dir, 800, hook->s.pos.trDelta );
	SnapVector( hook->s.pos.trDelta );			// save net bandwidth
	VectorCopy (start, hook->r.currentOrigin);

	self->client->hook = hook;

	return hook;
}


#ifdef MISSIONPACK
/*
=================
fire_nail
=================
*/
#define NAILGUN_SPREAD	500

gentity_t *fire_nail( gentity_t *self, vec3_t start, vec3_t forward, vec3_t right, vec3_t up ) {
	gentity_t	*bolt;
	vec3_t		dir;
	vec3_t		end;
	float		r, u, scale;

	bolt = G_Spawn();
	bolt->classname = "nail";
	bolt->nextthink = level.time + 10000;
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_NAILGUN;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = 20;
	bolt->methodOfDeath = MOD_NAIL;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;

	bolt->s.pos.trType = TR_LINEAR;
	bolt->s.pos.trTime = level.time;
	VectorCopy( start, bolt->s.pos.trBase );

	r = random() * M_PI * 2.0f;
	u = sin(r) * crandom() * NAILGUN_SPREAD * 16;
	r = cos(r) * crandom() * NAILGUN_SPREAD * 16;
	VectorMA( start, 8192 * 16, forward, end);
	VectorMA (end, r, right, end);
	VectorMA (end, u, up, end);
	VectorSubtract( end, start, dir );
	VectorNormalize( dir );

	scale = 555 + random() * 1800;
	VectorScale( dir, scale, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );

	VectorCopy( start, bolt->r.currentOrigin );

	return bolt;
}	


/*
=================
fire_prox
=================
*/
gentity_t *fire_prox( gentity_t *self, vec3_t start, vec3_t dir ) {
	gentity_t	*bolt;

	VectorNormalize (dir);

	bolt = G_Spawn();
	bolt->classname = "prox mine";
	bolt->nextthink = level.time + 3000;
	bolt->think = G_ExplodeMissile;
	bolt->s.eType = ET_MISSILE;
	bolt->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	bolt->s.weapon = WP_PROX_LAUNCHER;
	bolt->s.eFlags = 0;
	bolt->r.ownerNum = self->s.number;
	bolt->parent = self;
	bolt->damage = 0;
	bolt->splashDamage = 100;
	bolt->splashRadius = 150;
	bolt->methodOfDeath = MOD_PROXIMITY_MINE;
	bolt->splashMethodOfDeath = MOD_PROXIMITY_MINE;
	bolt->clipmask = MASK_SHOT;
	bolt->target_ent = NULL;
	// count is used to check if the prox mine left the player bbox
	// if count == 1 then the prox mine left the player bbox and can attack to it
	bolt->count = 0;

	//FIXME: we prolly wanna abuse another field
	bolt->s.generic1 = self->client->sess.sessionTeam;

	bolt->s.pos.trType = TR_GRAVITY;
	bolt->s.pos.trTime = level.time - MISSILE_PRESTEP_TIME;		// move a bit on the very first frame
	VectorCopy( start, bolt->s.pos.trBase );
	VectorScale( dir, 700, bolt->s.pos.trDelta );
	SnapVector( bolt->s.pos.trDelta );			// save net bandwidth

	VectorCopy (start, bolt->r.currentOrigin);

	return bolt;
}
#endif

