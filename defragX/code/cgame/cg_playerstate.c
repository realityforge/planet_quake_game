// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_playerstate.c -- this file acts on changes in a new playerState_t
// With normal play, this will be done after local prediction, but when
// following another player or playing back a demo, it will be checked
// when the snapshot transitions like all the other entities

#include "cg_local.h"

/*
==============
CG_CheckAmmo

If the ammo has gone low enough to generate the warning, play a sound
==============
*/
void CG_CheckAmmo( void ) {
	int		i;
	int		total;
	int		previous;
	int		weapons;

	// see about how many seconds of ammo we have remaining
	weapons = cg.snap->ps.stats[ STAT_WEAPONS ];

	//don't draw low ammo warning if we don't have guns
	if ( weapons < 1 << WP_MACHINEGUN ) {
		cg.lowAmmoWarning = 0;
		return;
	}

	total = 0;
	for ( i = WP_MACHINEGUN ; i < WP_NUM_WEAPONS ; i++ ) {
		if ( ! ( weapons & ( 1 << i ) ) ) {
			continue;
		}
		switch ( i ) {
		case WP_ROCKET_LAUNCHER:
		case WP_GRENADE_LAUNCHER:
		case WP_RAILGUN:
		case WP_SHOTGUN:

			total += cg.snap->ps.ammo[i] * 1000;
			break;
		default:
			total += cg.snap->ps.ammo[i] * 200;
			break;
		}
		if ( total >= 5000 || cg.snap->ps.ammo[i] < 0 ) {
			cg.lowAmmoWarning = 0;
			return;
		}
	}

	previous = cg.lowAmmoWarning;

	if ( total == 0 ) {
		cg.lowAmmoWarning = 2;
	} else {
		cg.lowAmmoWarning = 1;
	}

	// play a sound on transitions
	if ( cg.lowAmmoWarning != previous ) {
		trap_S_StartLocalSound( cgs.media.noAmmoSound, CHAN_LOCAL_SOUND );
	}
}

/*
==============
CG_CheckWeapons

If we don't have the current weapon anymore, switch to one we do have
==============
*/
void CG_CheckWeapons( playerState_t *ps, playerState_t *ops ) {
	int weapon, i;

	// if we don't have the current weapon anymore, switch to one we do have
	if ( !(ps->stats[STAT_WEAPONS] & (1 << ps->weapon)) ) {

		weapon = 0;
		for ( i = WP_NUM_WEAPONS - 1; i > 0; i-- ) {
			if ( ps->stats[STAT_WEAPONS] & (1 << i) ) {
				weapon = i;

				// preferably one that has ammo
				if ( ps->ammo[i] )
					break;

			}
		}

		cg.weaponSelectTime = cg.time;
		cg.weaponSelect = weapon;
	}

    if ( cg_autoswitch.integer && ps->stats[STAT_WEAPONS] != ops->stats[STAT_WEAPONS] ) {
		int weapons = ps->stats[STAT_WEAPONS] ^ ops->stats[STAT_WEAPONS];

		for ( i = 0; i < MAX_WEAPONS && weapons != 1; i++ )
			weapons >>= 1;

		if ( (ps->stats[STAT_WEAPONS] & (1 << i)) && i != WP_MACHINEGUN ) {
			cg.weaponSelectTime = cg.time;
			cg.weaponSelect = i;
		}
	}
}

/*
==============
CG_DamageFeedback
==============
*/
void CG_DamageFeedback( int yawByte, int pitchByte, int damage ) {
	float		left, front, up;
	float		kick;
	int			health;
	int			attacker;
	float		scale;
	vec3_t		dir;
	vec3_t		angles;
	float		dist;
	float		yaw, pitch;
	const char	*info;

	// show the attacking player's head and name in corner
	// but only if client is valid

	//attacker = cg.predictedPlayerState.persistant[PERS_ATTACKER]; ?
	attacker = cg.snap->ps.persistant[PERS_ATTACKER];

	if ( (unsigned)attacker < MAX_CLIENTS && attacker != cg.snap->ps.clientNum ) {
		// dont do it when cg_draw2d = 0 ?
		info = CG_ConfigString( CS_PLAYERS + attacker );
		if ( *info ) {
			BG_CleanName( Info_ValueForKey( info, "n" ), cg.attackerName, sizeof( cg.attackerName ), "???" );
			cg.attackerClientNum = attacker;
			cg.attackerTime = cg.time;
		}
	}

	// the lower on health you are, the greater the view kick will be
	health = cg.snap->ps.stats[STAT_HEALTH];
	if ( health < 40 ) {
		scale = 1;
	} else {
		scale = 40.0 / health;
	}
	kick = damage * scale;

	if (kick < 5)
		kick = 5;
	if (kick > 10)
		kick = 10;

	// if yaw and pitch are both 255, make the damage always centered (falling, etc)
	if ( yawByte == 255 && pitchByte == 255 ) {
		cg.damageX = 0;
		cg.damageY = 0;
		cg.v_dmg_roll = 0;
		cg.v_dmg_pitch = -kick;
	} else {
		// positional
		pitch = pitchByte / 255.0 * 360;
		yaw = yawByte / 255.0 * 360;

		angles[PITCH] = pitch;
		angles[YAW] = yaw;
		angles[ROLL] = 0;

		AngleVectors( angles, dir, NULL, NULL );
		VectorSubtract( vec3_origin, dir, dir );

		front = DotProduct (dir, cg.refdef.viewaxis[0] );
		left = DotProduct (dir, cg.refdef.viewaxis[1] );
		up = DotProduct (dir, cg.refdef.viewaxis[2] );

		dir[0] = front;
		dir[1] = left;
		dir[2] = 0;
		dist = VectorLength( dir );
		if ( dist < 0.1 ) {
			dist = 0.1f;
		}

		cg.v_dmg_roll = kick * left;

		cg.v_dmg_pitch = -kick * front;

		if ( front <= 0.1 ) {
			front = 0.1f;
		}
		cg.damageX = -left / front;
		cg.damageY = up / dist;
	}

	// clamp the position
	if ( cg.damageX > 1.0 ) {
		cg.damageX = 1.0;
	}
	if ( cg.damageX < - 1.0 ) {
		cg.damageX = -1.0;
	}

	if ( cg.damageY > 1.0 ) {
		cg.damageY = 1.0;
	}
	if ( cg.damageY < - 1.0 ) {
		cg.damageY = -1.0;
	}

	// don't let the screen flashes vary as much
	if ( kick > 10 ) {
		kick = 10;
	}
	cg.damageValue = kick;
	cg.v_dmg_time = cg.time + DAMAGE_TIME;
	cg.damageTime = cg.snap->serverTime;
}



void CG_ResetCheckpoints( void )
{
	int i;

	if ( cgs.gametype == GT_FREESTYLE )
    {
		return;
	}

	if ( cg.newBestCheckpoints )
    {
		for ( i = 0; i < MAX_CHECKPOINTS; i++ )
		{
			cg.bestCheckpoints[i] = cg.checkpoints[i];
		}
		cg.newBestCheckpoints = qfalse;
	}

	for (i = 0; i < MAX_CHECKPOINTS; i++ )
    {
		cg.checkpoints[i] = 0;
	}

	cg.currentCheckpoint = 0;
}

/*
================
CG_Respawn

A respawn happened this snapshot
================
*/
void CG_Respawn( void )
{
	// no error decay on player movement
	cg.thisFrameTeleport = qtrue;

	// display weapons available
	cg.weaponSelectTime = cg.time;

	// select the weapon the server says we are using
	cg.weaponSelect = cg.snap->ps.weapon;

	CG_ResetCheckpoints();

    CG_StartAutorecord();

	cg.timeResidual = cg.snap->ps.commandTime + 1000;
}


/*
==============
CG_CheckPlayerstateEvents
==============
*/
extern int		eventStack;
extern int		eventParm2[ MAX_PREDICTED_EVENTS ];

static void CG_CheckPlayerstateEvents( const playerState_t *ps, const playerState_t *ops )
{
	int			i, n;
	int			event;
	centity_t	*cent;

	if ( ps->externalEvent && ps->externalEvent != ops->externalEvent )
    {
		cent = &cg_entities[ ps->clientNum ];
		cent->currentState.event = ps->externalEvent;
		cent->currentState.eventParm = ps->externalEventParm;
		CG_EntityEvent( cent, cent->lerpOrigin, -1 );
	}

	cent = &cg.predictedPlayerEntity; // cg_entities[ ps->clientNum ];
	n = eventStack - MAX_PS_EVENTS;
	if ( n < 0 ) n  = 0;
	// go through the predictable events buffer
	for ( i = ps->eventSequence - MAX_PS_EVENTS ; i < ps->eventSequence ; i++ )
    {
		// if we have a new predictable event
		if ( i >= ops->eventSequence
			// or the server told us to play another event instead of a predicted event we already issued
			// or something the server told us changed our prediction causing a different event
			|| (i > ops->eventSequence - MAX_PS_EVENTS && ps->events[i & (MAX_PS_EVENTS-1)] != ops->events[i & (MAX_PS_EVENTS-1)]) )
			{

                event = ps->events[ i & (MAX_PS_EVENTS-1) ];
                if ( event == EV_NONE ) // ignore empty events
                    continue;

                cent->currentState.event = event;
                cent->currentState.eventParm = ps->eventParms[ i & (MAX_PS_EVENTS-1) ];

                CG_EntityEvent( cent, cent->lerpOrigin, eventParm2[ n++ ] );

                cg.predictableEvents[ i & (MAX_PREDICTED_EVENTS-1) ] = event;

                cg.eventSequence++;
		}
	}
}


/*
==================
CG_CheckLocalSounds
==================
*/
void CG_CheckLocalSounds( playerState_t *ps, playerState_t *ops )
{

	int	highScore, reward;
	sfxHandle_t sfx;

	// don't play the sounds if the player just changed teams
	if ( ps->persistant[PERS_TEAM] != ops->persistant[PERS_TEAM] )
		return;

	// health changes of more than -1 should make pain sounds
	if ( ps->stats[STAT_HEALTH] < ops->stats[STAT_HEALTH] - 1 )
    {
		if ( ps->stats[STAT_HEALTH] > 0 && (cg_painSounds.integer || cgs.gametype == GT_ADVENTURE))
			CG_PainEvent( &cg.predictedPlayerEntity, ps->stats[STAT_HEALTH] );

	}

	// if we are going into the intermission, don't start any voices
	if ( cg.intermissionStarted )
		return;

	// if any of the player event bits changed
	if (ps->persistant[PERS_PLAYEREVENTS] != ops->persistant[PERS_PLAYEREVENTS])
		reward = qtrue;

	// check for flag pickup
	if ( cgs.gametype == GT_FASTCAP ) {
		if ((ps->powerups[PW_REDFLAG]  != ops->powerups[PW_REDFLAG]  && ps->powerups[PW_REDFLAG]) || (ps->powerups[PW_BLUEFLAG] != ops->powerups[PW_BLUEFLAG] && ps->powerups[PW_BLUEFLAG]) )
            trap_S_StartLocalSound( cgs.media.flagPickup, CHAN_ANNOUNCER );
	}

	// lead changes
	if (!reward)
    {
		//
		if ( !cg.warmup )
		{
			// never play lead changes during warmup
			if ( ps->persistant[PERS_RANK] != ops->persistant[PERS_RANK] )
			{
				if ( cgs.gametype == GT_FASTCAP || cgs.gametype == GT_DEFRAG )
				{
					if (  ps->persistant[PERS_RANK] == 0 )
					{
						CG_AddBufferedSound(cgs.media.takenLeadSound);
					}
                    else if ( ps->persistant[PERS_RANK] == RANK_TIED_FLAG )
                    {
						CG_AddBufferedSound(cgs.media.tiedLeadSound);
					}
					else if ( ( ops->persistant[PERS_RANK] & ~RANK_TIED_FLAG ) == 0 )
                    {
						CG_AddBufferedSound(cgs.media.lostLeadSound);
					}
				}
			}
		}
	}

}

/*
===============
CG_TransitionPlayerState
===============
*/
void CG_TransitionPlayerState( playerState_t *ps, playerState_t *ops )
{
	qbool respawn;

    // start autorecord if rejoining after following someone
	if ( (ps->pm_flags & PMF_FOLLOW) != (ops->pm_flags & PMF_FOLLOW) ) {
		CG_StartAutorecord();
	}

	// check for changing follow mode
	if ( ps->clientNum != ops->clientNum )
    {
		cg.thisFrameTeleport = qtrue;
		// make sure we don't get any unwanted transition effects
		*ops = *ps;
	}

	// damage events (player is getting wounded)
	if ( ps->damageEvent != ops->damageEvent && ps->damageCount )
    {
		CG_DamageFeedback( ps->damageYaw, ps->damagePitch, ps->damageCount );
	}

	// respawning / map restart
	respawn = ps->persistant[PERS_SPAWN_COUNT] != ops->persistant[PERS_SPAWN_COUNT];
	if ( respawn || cg.mapRestart )
    {
		cg.mapRestart = qfalse;
		CG_Respawn();
	}

	// update forced enemy model and reset timer/checkpoints if team changed
	if ( ps->persistant[PERS_TEAM] != ops->persistant[PERS_TEAM] ) {

		CG_ResetCheckpoints();
		// start autorecord if changing team
		if ( ps->persistant[PERS_TEAM] != ops->persistant[PERS_TEAM] ) {
			CG_StartAutorecord();
		}
	}

	if ( cg.snap->ps.pm_type != PM_INTERMISSION	&& ps->persistant[PERS_TEAM] != TEAM_SPECTATOR )
    {
		CG_CheckLocalSounds( ps, ops );
	}

	// check for going low on ammo
	CG_CheckAmmo();

	// switch to a weapon we have if the current one was removed by a target_init
	CG_CheckWeapons(ps, ops);

	 // try to play potentially dropped events
	CG_PlayDroppedEvents( ps, ops );

	// run events
	CG_CheckPlayerstateEvents( ps, ops );

	// reset event stack
	eventStack = 0;

	// smooth the ducking viewheight change
	if ( ps->viewheight != ops->viewheight && !respawn )
    {
		cg.duckChange = ps->viewheight - ops->viewheight;
		cg.duckTime = cg.time;
	}
}
