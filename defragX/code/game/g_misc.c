// Copyright (C) 1999-2000 Id Software, Inc.
//
// g_misc.c

#include "g_local.h"

/*QUAKED info_null (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for calculations in the utilities (spotlights, etc), but removed during gameplay.
*/
void SP_info_null( gentity_t *self )
{
	G_FreeEntity( self );
}

/*QUAKED light (0 1 0) (-8 -8 -8) (8 8 8) linear
Non-displayed light.
"light" overrides the default 300 intensity.
Linear checbox gives linear falloff instead of inverse square
Lights pointed at a target will be spotlights.
"radius" overrides the default 64 unit radius of a spotlight at the target point.
*/
void SP_light( gentity_t *self )
{
	G_FreeEntity( self );
}



/*
=================================================================================

TELEPORTERS

=================================================================================
*/
void TeleportPlayer( gentity_t *player, vec3_t origin, vec3_t angles, qbool keepSpeed )
{
	gentity_t	*tent;

	// use temp events at source and destination to prevent the effect
	// from getting dropped by a second player event
	if ( player->client->sess.sessionTeam != TEAM_SPECTATOR )
    {
		tent = G_TempEntity( player->client->ps.origin, EV_PLAYER_TELEPORT_OUT );
		tent->s.clientNum = player->s.clientNum;

		tent = G_TempEntity( origin, EV_PLAYER_TELEPORT_IN );
		tent->s.clientNum = player->s.clientNum;
	}

	// unlink to make sure it can't possibly interfere with G_KillBox
	trap_UnlinkEntity( player );

	VectorCopy( origin, player->client->ps.origin );
	player->client->ps.origin[2] += 1.0f;

	// spit the player out
	if ( angles )
    {
		// spit the player out
		vec_t oldspeed = VectorLength( player->client->ps.velocity );

		AngleVectors( angles, player->client->ps.velocity, NULL, NULL );
		if ( keepSpeed )
        {
			VectorScale( player->client->ps.velocity, oldspeed, player->client->ps.velocity );
		}
		else
        {
			VectorScale( player->client->ps.velocity, 400, player->client->ps.velocity );
		}
		player->client->ps.pm_time = 160;		// hold time
		player->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
		// set angles
		SetClientViewAngle(player, angles);
    }
	// toggle the teleport bit so the client knows to not lerp
	player->client->ps.eFlags ^= EF_TELEPORT_BIT;

	// set angles
	if ( angles )
		SetClientViewAngle( player, angles );

	// unlagged
	G_ResetHistory( player );

	// kill anything at the destination
	if ( player->client->sess.sessionTeam != TEAM_SPECTATOR )
    {
		G_KillBox( player );
	}

	// save results of pmove
	BG_PlayerStateToEntityState( &player->client->ps, &player->s, qtrue );

	// use the precise origin for linking
	VectorCopy( player->client->ps.origin, player->r.currentOrigin );

	if ( player->client->sess.sessionTeam != TEAM_SPECTATOR )
    {
		trap_LinkEntity( player );
	}
}


/*QUAKED misc_teleporter_dest (1 0 0) (-32 -32 -24) (32 32 -16)
Point teleporters at these.
Now that we don't have teleport destination pads, this is just
an info_notnull
*/
void SP_misc_teleporter_dest( gentity_t *ent )
{
}


//===========================================================

/*QUAKED misc_model (1 0 0) (-16 -16 -16) (16 16 16)
"model"		arbitrary .md3 file to display
*/
void SP_misc_model( gentity_t *ent )
{
	G_FreeEntity( ent );
}

//===========================================================

void locateCamera( gentity_t *ent )
{
	vec3_t		dir;
	gentity_t	*target;
	gentity_t	*owner;

	owner = G_PickTarget( ent->target );
	if ( !owner )
    {
		G_Printf( "Couldn't find target for misc_portal_surface\n" );
		G_FreeEntity( ent );
		return;
	}
	ent->r.ownerNum = owner->s.number;

	// frame holds the rotate speed
	if ( owner->spawnflags & 1 )
    {
		ent->s.frame = 25;
	}
	else if ( owner->spawnflags & 2 )
    {
		ent->s.frame = 75;
	}

	// swing camera ?
	if ( owner->spawnflags & 4 )
    {
		// set to 0 for no rotation at all
		ent->s.powerups = 0;
	}
	else
    {
		ent->s.powerups = 1;
	}

	// clientNum holds the rotate offset
	ent->s.clientNum = owner->s.clientNum;

	VectorCopy( owner->s.origin, ent->s.origin2 );

	// see if the portal_camera has a target
	target = G_PickTarget( owner->target );
	if ( target )
    {
		VectorSubtract( target->s.origin, owner->s.origin, dir );
		VectorNormalize( dir );
	}
	else
    {
		G_SetMovedir( owner->s.angles, dir );
	}

	ent->s.eventParm = DirToByte( dir );
}

/*QUAKED misc_portal_surface (0 0 1) (-8 -8 -8) (8 8 8)
The portal surface nearest this entity will show a view from the targeted misc_portal_camera, or a mirror view if untargeted.
This must be within 64 world units of the surface!
*/
void SP_misc_portal_surface(gentity_t *ent)
{
	VectorClear( ent->r.mins );
	VectorClear( ent->r.maxs );
	trap_LinkEntity (ent);

	ent->r.svFlags = SVF_PORTAL;
	ent->s.eType = ET_PORTAL;

	if ( !ent->target )
    {
		VectorCopy( ent->s.origin, ent->s.origin2 );
	}
	else
    {
		ent->think = locateCamera;
		ent->nextthink = level.time + 100;
	}
}

/*QUAKED misc_portal_camera (0 0 1) (-8 -8 -8) (8 8 8) slowrotate fastrotate noswing
The target for a misc_portal_director.  You can set either angles or target another entity to determine the direction of view.
"roll" an angle modifier to orient the camera around the target vector;
*/
void SP_misc_portal_camera(gentity_t *ent)
{
	float	roll;

	VectorClear( ent->r.mins );
	VectorClear( ent->r.maxs );
	trap_LinkEntity (ent);

	G_SpawnFloat( "roll", "0", &roll );

	ent->s.clientNum = roll/360.0 * 256;
}

/*
======================================================================

  SHOOTERS

======================================================================
*/

void Use_Shooter( gentity_t *ent, gentity_t *other, gentity_t *activator )
{
	vec3_t		dir;
	float		deg;
	vec3_t		up, right;

	// see if we have a target
	if ( ent->enemy )
    {
		VectorSubtract( ent->enemy->r.currentOrigin, ent->s.origin, dir );
		VectorNormalize( dir );
	}
	else
    {
		VectorCopy( ent->movedir, dir );
	}

	// randomize a bit
	PerpendicularVector( up, dir );
	CrossProduct( up, dir, right );

	deg = crandom() * ent->random;
	VectorMA( dir, deg, up, dir );

	deg = crandom() * ent->random;
	VectorMA( dir, deg, right, dir );

	VectorNormalize( dir );

	switch ( ent->s.weapon ) {
	case WP_GRENADE_LAUNCHER:
		fire_grenade( ent, ent->s.origin, dir );
		break;
	case WP_ROCKET_LAUNCHER:
		fire_rocket( ent, ent->s.origin, dir );
		break;
	case WP_PLASMAGUN:
		fire_plasma( ent, ent->s.origin, dir );
		break;
    case WP_BFG:
		fire_bfg( ent, ent->s.origin, dir );
		break;
	}

	G_AddEvent( ent, EV_FIRE_WEAPON, 0 );
}


static void InitShooter_Finish( gentity_t *ent )
{
	ent->enemy = G_PickTarget( ent->target );
	ent->think = 0;
	ent->nextthink = 0;
}

void InitShooter( gentity_t *ent, int weapon )
{
	ent->use = Use_Shooter;
	ent->s.weapon = weapon;

	RegisterItem( BG_FindItemForWeapon( weapon ) );

	G_SetMovedir( ent->s.angles, ent->movedir );

	if ( !ent->random )
    {
		ent->random = 1.0;
	}
	ent->random = sin( M_PI * ent->random / 180 );
	// target might be a moving object, so we can't set movedir for it
	if ( ent->target )
	{
		ent->think = InitShooter_Finish;
		ent->nextthink = level.time + 500;
	}
	trap_LinkEntity( ent );
}

/*QUAKED shooter_rocket (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_rocket( gentity_t *ent )
{
	InitShooter( ent, WP_ROCKET_LAUNCHER );
}

/*QUAKED shooter_plasma (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_plasma( gentity_t *ent )
{
	InitShooter( ent, WP_PLASMAGUN);
}

/*QUAKED shooter_grenade (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_grenade( gentity_t *ent ) {
	InitShooter( ent, WP_GRENADE_LAUNCHER);
}

/*QUAKED shooter_bfg (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_bfg( gentity_t *ent ) {
	InitShooter( ent, WP_BFG );
}


/*QUAKED func_breakable (0 .5 .8) ?
Brush model that breaks when damaged or targeted
"health"	amount of damage to take before breaking (0 means don't break from damage)
*/
void Func_Breakable_Break( gentity_t *ent )
{
	vec3_t size, center;

	if ( !ent->r.linked )
		return;

	VectorSubtract( ent->r.maxs, ent->r.mins, size );
	VectorMA( ent->r.mins, 0.5, size, center );

	G_TempEntity( center, EV_BREAKABLE );
	trap_UnlinkEntity( ent );
}

void Use_Func_Breakable( gentity_t *ent, gentity_t *other, gentity_t *activator )
{
	if ( !ent->r.linked )
		return;

	Func_Breakable_Break( ent );
	G_UseTargets( ent, activator );
}

void Func_Breakable_Die( gentity_t *ent, gentity_t *inflictor, gentity_t *attacker, int damage, int mod )
{
	Func_Breakable_Break( ent );
	G_UseTargets( ent, attacker );
}

void SP_func_breakable( gentity_t *ent )
{
    if ( ent->model ) {
		trap_SetBrushModel( ent, ent->model );
	}
	ent->s.eType = ET_BREAKABLE;

	G_SpawnInt( "health", "10", &ent->health );

	if ( ent->health )
    {
		ent->takedamage = qtrue;
		ent->die = Func_Breakable_Die;
	}
	ent->use = Use_Func_Breakable;

	trap_LinkEntity( ent );
}


void DF_Timer_Start( gentity_t *ent )
{
	if ( !ent->client )
		return;

	ent->client->dfTimerOn = qtrue;
	ent->client->dfTimer = level.time;
	trap_SendServerCommand( ent - g_entities, "starttimer" );
}

void DF_Timer_Stop( gentity_t *ent )
{
	qbool newbest = qfalse;

	if ( !ent->client )
		return;

	if ( ent->client->dfTimerOn ) {
		ent->client->dfTimerOn = qfalse;
		ent->client->dfTimer = level.time - ent->client->dfTimer;

		if ( !ent->client->pers.dfBestTime || ent->client->pers.dfBestTime > ent->client->dfTimer ) {
			newbest = qtrue;
			ent->client->pers.dfBestTime = ent->client->dfTimer;
			CalculateRanks();
		}

		trap_SendServerCommand( -1, va("print \"%s ^2finished in %s\n\"",
					ent->client->pers.netname,
					FormatDFTimer( ent->client->dfTimer, qfalse )) );
		trap_SendServerCommand( ent - g_entities, va("stoptimer %d %d", ent->client->dfTimer, newbest) );
	}
}

