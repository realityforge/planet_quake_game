// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_event.c -- handle entity events at snapshot or playerstate transitions

#include "cg_local.h"

//==========================================================================

/*
===================
CG_PlaceString

Also called by scoreboard drawing
===================
*/
const char	*CG_PlaceString( int rank )
{
	static char	str[64];
	char	*s, *t;

	if ( rank & RANK_TIED_FLAG ) {
		rank &= ~RANK_TIED_FLAG;
		t = "Tied for ";
	} else {
		t = "";
	}

	s = va("%ith", rank);

	Com_sprintf( str, sizeof( str ), "%s%s", t, s );
	return str;
}


/*
=============
CG_Obituary
=============
*/
static void CG_Obituary( entityState_t *ent ) {
	int			mod;
	int			target, attacker;
	char		*message;
	char		*message2;
	const char	*targetInfo;
	const char	*attackerInfo;
	char		targetName[32];
	char		attackerName[32];
	gender_t	gender;
	clientInfo_t	*ci;
	qbool	following;

	target = ent->otherEntityNum;
	attacker = ent->otherEntityNum2;
	mod = ent->eventParm;

	if ( target < 0 || target >= MAX_CLIENTS ) {
		CG_Error( "CG_Obituary: target out of range" );
	}
	ci = &cgs.clientinfo[target];

	if ( attacker < 0 || attacker >= MAX_CLIENTS ) {
		attacker = ENTITYNUM_WORLD;
		attackerInfo = NULL;
	} else {
		attackerInfo = CG_ConfigString( CS_PLAYERS + attacker );
	}

	targetInfo = CG_ConfigString( CS_PLAYERS + target );
	if ( !targetInfo[0] )
	{
		return;
	}
	Q_strncpyz( targetName, Info_ValueForKey( targetInfo, "n" ), sizeof(targetName) - 2);
	strcat( targetName, S_COLOR_WHITE );

	following = cg.snap->ps.pm_flags & PMF_FOLLOW;

	message2 = "";

	// check for double client messages
	if ( !attackerInfo ) {
		attacker = ENTITYNUM_WORLD;
		strcpy( attackerName, "noname" );
	} else {
		Q_strncpyz( attackerName, Info_ValueForKey( attackerInfo, "n" ), sizeof(attackerName) - 2);
		Q_CleanStr( attackerName );
		strcat( attackerName, S_COLOR_WHITE );
		// check for kill messages about the current clientNum
		if ( target == cg.snap->ps.clientNum ) {
			Q_strncpyz( cg.killerName, attackerName, sizeof( cg.killerName ) );
			// follow killer
			if ( following ) {
				if ( !cg.followTime && attacker != cg.snap->ps.clientNum && attacker < MAX_CLIENTS ) {
					cg.followClient = attacker;
					cg.followTime = cg.time + 1100;
				}
			}
		}
	}
}
//==========================================================================


/*
===============
CG_UseItem
===============
*/
static void CG_UseItem( centity_t *cent ) {
	clientInfo_t *ci;
	int			itemNum, clientNum;
	gitem_t		*item;
	entityState_t *es;

	es = &cent->currentState;

	itemNum = (es->event & ~EV_EVENT_BITS) - EV_USE_ITEM0;
	if ( itemNum < 0 || itemNum > HI_NUM_HOLDABLE ) {
		itemNum = 0;
	}

	// print a message if the local player
	// SLK add cvar to toggle holdable item message
    if ( hud_useHoldable.integer && ( es->number == cg.snap->ps.clientNum ) ) {
        if ( !itemNum ) {
            CG_CenterPrint( "No item to use", SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );
        } else {
            item = BG_FindItemForHoldable( itemNum );
            CG_CenterPrint( va("Use %s", item->pickup_name), SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );
        }
    }

	switch ( itemNum ) {
	default:
	case HI_NONE:
		trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.useNothingSound );
		break;

	case HI_TELEPORTER:
		break;

	case HI_MEDKIT:
		clientNum = cent->currentState.clientNum;
		if ( clientNum >= 0 && clientNum < MAX_CLIENTS ) {
			ci = &cgs.clientinfo[ clientNum ];
			ci->medkitUsageTime = cg.time;
		}
		trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.medkitSound );
		break;

	}
}


/*
================
CG_ItemPickup

A new item was picked up this frame
================
*/
static void CG_ItemPickup( int itemNum ) {
	static int oldItem = -1;

	cg.itemPickup = itemNum;
	cg.itemPickupTime = cg.time;
	cg.itemPickupBlendTime = cg.time;

	if ( oldItem != itemNum )
		cg.itemPickupCount = 1;
	else
		cg.itemPickupCount++;

	oldItem = itemNum;

	// see if it should be the grabbed weapon
	if ( bg_itemlist[itemNum].giType == IT_WEAPON ) {
		// select it immediately
		if ( cg_autoswitch.integer && bg_itemlist[itemNum].giTag != WP_MACHINEGUN ) {
			cg.weaponSelectTime = cg.time;
			cg.weaponSelect = bg_itemlist[itemNum].giTag;
		}
	}

}


/*
================
CG_WaterLevel

Returns waterlevel for entity origin
================
*/
int CG_WaterLevel(centity_t *cent) {
	vec3_t point;
	int contents, sample1, sample2, anim, waterlevel;
	int viewheight;

	anim = cent->currentState.legsAnim & ~ANIM_TOGGLEBIT;

	if ( anim == LEGS_WALKCR || anim == LEGS_IDLECR ) {
		viewheight = CROUCH_VIEWHEIGHT;
	} else {
		viewheight = DEFAULT_VIEWHEIGHT;
	}

	//
	// get waterlevel, accounting for ducking
	//
	waterlevel = 0;

	point[0] = cent->lerpOrigin[0];
	point[1] = cent->lerpOrigin[1];
	point[2] = cent->lerpOrigin[2] + MINS_Z + 1;
	contents = CG_PointContents(point, -1);

	if ( contents & MASK_WATER ) {
		sample2 = viewheight - MINS_Z;
		sample1 = sample2 / 2;
		waterlevel = 1;
		point[2] = cent->lerpOrigin[2] + MINS_Z + sample1;
		contents = CG_PointContents(point, -1);

		if (contents & MASK_WATER) {
			waterlevel = 2;
			point[2] = cent->lerpOrigin[2] + MINS_Z + sample2;
			contents = CG_PointContents(point, -1);

			if (contents & MASK_WATER) {
				waterlevel = 3;
			}
		}
	}

	return waterlevel;
}

/*
================
CG_PainEvent

Also called by playerstate transition
================
*/
void CG_PainEvent( centity_t *cent, int health ) {
	const char *snd;

	// don't do more than two pain sounds a second
	if ( cg.time - cent->pe.painTime < 500 ) {
		cent->pe.painIgnore = qfalse;
		return;
	}

	if ( cent->pe.painIgnore ) {
		cent->pe.painIgnore = qfalse;
		return;
	}

	if ( health < 25 ) {
		snd = "*pain25_1.wav";
	} else if ( health < 50 ) {
		snd = "*pain50_1.wav";
	} else if ( health < 75 ) {
		snd = "*pain75_1.wav";
	} else {
		snd = "*pain100_1.wav";
	}

	// play a gurp sound instead of a normal pain sound
	if (CG_WaterLevel(cent) == 3) {
		if (rand()&1) {
			trap_S_StartSound(NULL, cent->currentState.number, CHAN_VOICE, CG_CustomSound(cent->currentState.number, "sound/player/gurp1.wav"));
		} else {
			trap_S_StartSound(NULL, cent->currentState.number, CHAN_VOICE, CG_CustomSound(cent->currentState.number, "sound/player/gurp2.wav"));
		}
	} else {
		trap_S_StartSound(NULL, cent->currentState.number, CHAN_VOICE, CG_CustomSound(cent->currentState.number, snd));
	}

	// save pain time for programitic twitch animation
	cent->pe.painTime = cg.time;
	cent->pe.painDirection ^= 1;
}



/*
==============
CG_EntityEvent

An entity has an event value
also called by CG_CheckPlayerstateEvents
==============
*/
void CG_EntityEvent( centity_t *cent, vec3_t position, int entityNum ) {
	entityState_t	*es;
	entity_event_t	event;
	vec3_t			dir;
	const char		*s;
	int				clientNum;
	clientInfo_t	*ci;
	vec3_t			vec;
	float			fovOffset;
	centity_t		*ce;
	byte			r, g, b;

	es = &cent->currentState;
	event = es->event & ~EV_EVENT_BITS;

	if ( (unsigned) event >= EV_MAX ) {
		CG_Error( "Unknown event: %i", event );
		return;
	}

#ifdef DEVBUILD
	if ( cg_debugEvents.integer ) 
		CG_Printf( "ent:%3i  event:%3i %s", es->number, event, eventnames[ event ] );
#endif

	if ( !event ) 
		return;

	clientNum = es->clientNum;
	if ( (unsigned) clientNum >= MAX_CLIENTS ) 
		clientNum = 0;
	
	ci = &cgs.clientinfo[ clientNum ];

	switch ( event ) {
	//
	// movement generated events
	//
	case EV_FOOTSTEP:
		if (cg_footsteps.integer || cgs.gametype == GT_ADVENTURE) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY,	cgs.media.footsteps[ ci->footsteps ][rand()&3] );
		}
		break;

	case EV_FOOTSTEP_METAL:
		if (cg_footsteps.integer || cgs.gametype == GT_ADVENTURE) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY,	cgs.media.footsteps[ FOOTSTEP_METAL ][rand()&3] );
		}
		break;

    case EV_FOOTSTEP_FLESH:
		if (cg_footsteps.integer || cgs.gametype == GT_ADVENTURE) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY,	cgs.media.footsteps[ FOOTSTEP_FLESH ][rand()&3] );
		}
		break;

    case EV_FOOTSTEP_STONE:
		if (cg_footsteps.integer || cgs.gametype == GT_ADVENTURE) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY,	cgs.media.footsteps[ FOOTSTEP_STONE ][rand()&3] );
		}
		break;

     case EV_FOOTSTEP_GRASS:
		if (cg_footsteps.integer || cgs.gametype == GT_ADVENTURE) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY,	cgs.media.footsteps[ FOOTSTEP_GRASS ][rand()&3] );
		}
		break;

     case EV_FOOTSTEP_TALLGRASS:
		if (cg_footsteps.integer || cgs.gametype == GT_ADVENTURE) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY,	cgs.media.footsteps[ FOOTSTEP_TALLGRASS ][rand()&3] );
		}
		break;

     case EV_FOOTSTEP_SAND:
		if (cg_footsteps.integer || cgs.gametype == GT_ADVENTURE) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY,	cgs.media.footsteps[ FOOTSTEP_SAND ][rand()&3] );
		}
		break;

    case EV_FOOTSTEP_LAVA:
		if (cg_footsteps.integer || cgs.gametype == GT_ADVENTURE) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY,	cgs.media.footsteps[ FOOTSTEP_LAVA ][rand()&3] );
		}
		break;

     case EV_FOOTSTEP_ICE:
		if (cg_footsteps.integer || cgs.gametype == GT_ADVENTURE) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY,	cgs.media.footsteps[ FOOTSTEP_ICE ][rand()&3] );
		}
		break;


     case EV_FOOTSTEP_WOOD:
		if (cg_footsteps.integer || cgs.gametype == GT_ADVENTURE) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY,	cgs.media.footsteps[ FOOTSTEP_WOOD ][rand()&3] );
		}
		break;

    case EV_FOOTSTEP_ENERGY:
		if (cg_footsteps.integer || cgs.gametype == GT_ADVENTURE) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY,	cgs.media.footsteps[ FOOTSTEP_ENERGY ][rand()&3] );
		}
		break;

	case EV_FOOTSPLASH:
		if (cg_footsteps.integer || cgs.gametype == GT_ADVENTURE) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY,	cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		}
		break;

	case EV_FOOTWADE:
		if (cg_footsteps.integer || cgs.gametype == GT_ADVENTURE) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY,	cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		}
		break;

	case EV_SWIM:
		if (cg_footsteps.integer || cgs.gametype == GT_ADVENTURE) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY,	cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		}
		break;

	case EV_FALL_SHORT:
		/*if (cg_landingSound.integer || cgs.gametype == GT_ADVENTURE)
            trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound );*/

		if ( clientNum == cg.predictedPlayerState.clientNum ) {
			// smooth landing z changes
			cg.landChange = -8;
			cg.landTime = cg.time;
		}
		break;

	case EV_FALL_MEDIUM:
		// use normal pain sound
		if (cg_painSounds.integer || cgs.gametype == GT_ADVENTURE)
            trap_S_StartSound( NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*pain100_1.wav" ) );

		cent->pe.painIgnore = qtrue;
		cent->pe.painTime = cg.time;	// don't play a pain sound right after this
		if ( clientNum == cg.predictedPlayerState.clientNum ) {
			// smooth landing z changes
			cg.landChange = -16;
			cg.landTime = cg.time;
		}
		break;

	case EV_FALL_FAR:
	    if (cg_painSounds.integer || cgs.gametype == GT_ADVENTURE)
            trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound( es->number, "*fall1.wav" ) );

		cent->pe.painIgnore = qtrue;
		cent->pe.painTime = cg.time;	// don't play a pain sound right after this
		if ( clientNum == cg.predictedPlayerState.clientNum ) {
			// smooth landing z changes
			cg.landChange = -24;
			cg.landTime = cg.time;
		}
		break;

	case EV_STEP_4:
	case EV_STEP_8:
	case EV_STEP_12:
	case EV_STEP_16:		// smooth out step up transitions
	{
		float	oldStep;
		int		delta;
		int		step;

		if ( clientNum != cg.predictedPlayerState.clientNum ) 
			break;
		
		// if we are interpolating, we don't need to smooth steps
		if ( cg.demoPlayback || (cg.snap->ps.pm_flags & PMF_FOLLOW) ||
			cg_nopredict.integer || cgs.synchronousClients ) {
			break;
		}
		// check for stepping up before a previous step is completed
		delta = cg.time - cg.stepTime;
		if (delta < STEP_TIME) {
			oldStep = cg.stepChange * (STEP_TIME - delta) / STEP_TIME;
		} else {
			oldStep = 0;
		}

		// add this amount
		step = 4 * (event - EV_STEP_4 + 1 );
		cg.stepChange = oldStep + step;
		if ( cg.stepChange > MAX_STEP_CHANGE ) 
			cg.stepChange = MAX_STEP_CHANGE;
		
		cg.stepTime = cg.time;
		break;
	}

	case EV_JUMP_PAD:
		{
			vec3_t	up = {0, 0, 1};
			CG_SmokePuff( cent->lerpOrigin, up, 32, 1, 1, 1, 0.33f, 1000, cg.time, 0, LEF_PUFF_DONT_SCALE, cgs.media.smokePuffShader );
		}

		// boing sound at origin, jump sound on player
		trap_S_StartSound ( cent->lerpOrigin, -1, CHAN_VOICE, cgs.media.jumpPadSound );
		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*jump1.wav" ) );
		break;

	case EV_JUMP:
		// pain event with fast sequential jump just creates sound distortion
		if ( cg.time - cent->pe.painTime > 50 )
			trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*jump1.wav" ) );
		break;

	case EV_TAUNT:
		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*taunt.wav" ) );
		break;

	case EV_WATER_TOUCH:
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrInSound );
		break;

	case EV_WATER_LEAVE:
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrOutSound );
		break;

	case EV_WATER_UNDER:
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrUnSound );
		break;

	case EV_WATER_CLEAR:
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound( es->number, "*gasp.wav" ) );
		break;

	case EV_ITEM_PICKUP:
		{
			gitem_t	*item;
			int		index;

			index = es->eventParm;		// player predicted

			if ( index < 1 || index >= bg_numItems ) 
				break;

			if ( entityNum >= 0 ) {
				// our predicted entity
				ce = cg_entities + entityNum;
				if ( ce->delaySpawn > cg.time && ce->delaySpawnPlayed ) 
					break; // delay item pickup

			} else {
				ce = NULL;
			}

			item = &bg_itemlist[ index ];

			// powerups and team items will have a separate global sound, this one
			// will be played at prediction time
			if ( item->giType == IT_POWERUP || item->giType == IT_TEAM) {
				trap_S_StartSound (NULL, es->number, CHAN_AUTO,	cgs.media.n_healthSound );
			} else if (item->giType == IT_PERSISTANT_POWERUP) {

			} else {
				trap_S_StartSound (NULL, es->number, CHAN_AUTO,	trap_S_RegisterSound( item->pickup_sound, qfalse ) );
			}

			// show icon and name on status bar
			if ( es->number == cg.snap->ps.clientNum ) 
				CG_ItemPickup( index );

			if ( ce ) 
				ce->delaySpawnPlayed = qtrue;
		
		}
		break;

	case EV_GLOBAL_ITEM_PICKUP:
		{
			gitem_t	*item;
			int		index;

			index = es->eventParm;		// player predicted

			if ( index < 1 || index >= bg_numItems ) 
				break;

			if ( entityNum >= 0 ) {
				// our predicted entity
				ce = cg_entities + entityNum;
				if ( ce->delaySpawn > cg.time && ce->delaySpawnPlayed ) {
					break;
				}
			} else {
				ce = NULL;
			}

			item = &bg_itemlist[ index ];
			// powerup pickups are global
			if( item->pickup_sound ) {
				trap_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, trap_S_RegisterSound( item->pickup_sound, qfalse ) );
			}

			// show icon and name on status bar
			if ( es->number == cg.snap->ps.clientNum ) 
				CG_ItemPickup( index );

			if ( ce ) 
				ce->delaySpawnPlayed = qtrue;
		}
		break;

	//
	// weapon events
	//
	case EV_NOAMMO:
		if ( es->number == cg.snap->ps.clientNum )
			CG_OutOfAmmoChange();
			
		break;

	case EV_CHANGE_WEAPON:
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.selectSound );
		break;

	case EV_FIRE_WEAPON:
		CG_FireWeapon( cent );
		break;

	case EV_USE_ITEM0:
	case EV_USE_ITEM1:
	case EV_USE_ITEM2:
	case EV_USE_ITEM3:
	case EV_USE_ITEM4:
	case EV_USE_ITEM5:
	case EV_USE_ITEM6:
	case EV_USE_ITEM7:
	case EV_USE_ITEM8:
	case EV_USE_ITEM9:
	case EV_USE_ITEM10:
	case EV_USE_ITEM11:
	case EV_USE_ITEM12:
	case EV_USE_ITEM13:
	case EV_USE_ITEM14:
	case EV_USE_ITEM15:
		CG_UseItem( cent );
		break;

	//=================================================================

	//
	// other events
	//
	case EV_PLAYER_TELEPORT_IN:
		if (cg_teleportEffect.integer == 2 || (cgs.gametype == GT_ADVENTURE) )
            trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.teleInSound );

		CG_SpawnEffect( position);
		break;

	case EV_PLAYER_TELEPORT_OUT:
		if (cg_teleportEffect.integer == 2 || (cgs.gametype == GT_ADVENTURE) )
            trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.teleOutSound );

		CG_SpawnEffect(  position);
		break;

	case EV_ITEM_POP:
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.respawnSound );
		break;

	case EV_ITEM_RESPAWN:
		cent->miscTime = cg.time;	// scale up from this
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.respawnSound );
		break;

	case EV_GRENADE_BOUNCE:
		if ( rand() & 1 ) {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.hgrenb1aSound );
		} else {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.hgrenb2aSound );
		}
		break;

	//
	// missile impacts
	//
	case EV_MISSILE_HIT:
		ByteToDir( es->eventParm, dir );
		CG_MissileHitPlayer( es->weapon, position, dir, es->otherEntityNum );
		break;

	case EV_MISSILE_MISS:
		ByteToDir( es->eventParm, dir );
		CG_MissileHitWall( es->weapon, 0, position, dir, IMPACTSOUND_DEFAULT );
		break;

	case EV_MISSILE_MISS_METAL:
		ByteToDir( es->eventParm, dir );
		CG_MissileHitWall( es->weapon, 0, position, dir, IMPACTSOUND_METAL );
		break;

	case EV_RAILTRAIL:
		cent->currentState.weapon = WP_RAILGUN;

		if ( cent->currentState.clientNum == cg.snap->ps.clientNum && !cg_thirdPerson.integer )
		{
			VectorCopy( cg.refdef.vieworg, vec );
			fovOffset = -0.2f * ( cgs.fov - 90.0f );

			// 13.5, -5.5, -6.0
			VectorMA( vec, cg_gun_x.value + 13.5f, cg.refdef.viewaxis[0], vec );
			VectorMA( vec, cg_gun_y.value - 5.5f, cg.refdef.viewaxis[1], vec );
			VectorMA( vec, cg_gun_z.value + fovOffset - 6.0f, cg.refdef.viewaxis[2], vec );
		}
		else
			VectorCopy( es->origin2, vec );

		// if the end was on a nomark surface, don't make an explosion
		CG_RailTrail( ci, vec, es->pos.trBase );

		if ( es->eventParm != 255 ) {
			ByteToDir( es->eventParm, dir );
			CG_MissileHitWall( es->weapon, es->clientNum, position, dir, IMPACTSOUND_DEFAULT );
		}
		break;

	case EV_BULLET_HIT_WALL:
		ByteToDir( es->eventParm, dir );
		CG_Bullet( es->pos.trBase, es->otherEntityNum, dir, qfalse, ENTITYNUM_WORLD );
		break;

	case EV_BULLET_HIT_FLESH:
		CG_Bullet( es->pos.trBase, es->otherEntityNum, dir, qtrue, es->eventParm );
		break;

	case EV_SHOTGUN:
		CG_ShotgunFire( es );
		break;

	case EV_GENERAL_SOUND:
		if ( cgs.gameSounds[ es->eventParm ] ) {
			trap_S_StartSound (NULL, es->number, CHAN_VOICE, cgs.gameSounds[ es->eventParm ] );
		} else {
			s = CG_ConfigString( CS_SOUNDS + es->eventParm );
			trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, s ) );
		}
		break;

	case EV_GLOBAL_SOUND:	// play from the player's head so it never diminishes
		if ( cgs.gameSounds[ es->eventParm ] ) {
			trap_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, cgs.gameSounds[ es->eventParm ] );
		} else {
			s = CG_ConfigString( CS_SOUNDS + es->eventParm );
			trap_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, CG_CustomSound( es->number, s ) );
		}
		break;

	case EV_GLOBAL_TEAM_SOUND:	// play from the player's head so it never diminishes
		break;
		
	case EV_PAIN:
		// local player sounds are triggered in CG_CheckLocalSounds,
		// so ignore events on the player
		if ( cent->currentState.number != cg.snap->ps.clientNum ) {
			if (cg_painSounds.integer || cgs.gametype == GT_ADVENTURE)
                CG_PainEvent( cent, es->eventParm );

		}
		break;

	case EV_DEATH1:
	case EV_DEATH2:
	case EV_DEATH3:
		if ( CG_WaterLevel(cent) == 3 ) {
			trap_S_StartSound( NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, "*drown.wav") );
		} else {
			trap_S_StartSound( NULL, es->number, CHAN_VOICE, CG_CustomSound(es->number, va("*death%i.wav", event - EV_DEATH1 + 1)) );
		}

		break;

	case EV_OBITUARY:
		CG_Obituary( es );
		break;

	//
	// powerup events
	//
	case EV_POWERUP_QUAD:
		if ( es->number == cg.snap->ps.clientNum ) {
			cg.powerupActive = PW_QUAD;
			cg.powerupTime = cg.time;
		}
		trap_S_StartSound (NULL, es->number, CHAN_ITEM, cgs.media.quadSound );
		break;

	case EV_POWERUP_BATTLESUIT:
		if ( es->number == cg.snap->ps.clientNum ) {
			cg.powerupActive = PW_BATTLESUIT;
			cg.powerupTime = cg.time;
		}
		trap_S_StartSound (NULL, es->number, CHAN_ITEM, cgs.media.protectSound );
		break;

	case EV_POWERUP_REGEN:
		if ( es->number == cg.snap->ps.clientNum ) {
			cg.powerupActive = PW_REGEN;
			cg.powerupTime = cg.time;
		}
		trap_S_StartSound (NULL, es->number, CHAN_ITEM, cgs.media.regenSound );
		break;

	case EV_GIB_PLAYER:
		if (cg_gibEffect.integer == 2 || (cgs.gametype == GT_ADVENTURE) )
            trap_S_StartSound( NULL, es->number, CHAN_BODY, cgs.media.gibSound );

		CG_GibPlayer( cent->lerpOrigin );
		break;

	case EV_STOPLOOPINGSOUND:
		trap_S_StopLoopingSound( es->number );
		es->loopSound = 0;
		break;

	case EV_DEBUG_LINE:
		CG_Beam( cent );
		break;

	case EV_BREAKABLE:
		CG_GibEntity( cent->lerpOrigin );
		break;

    case EV_EARTHQUAKE:
		CG_StartEarthquake((es->eventParm & 0x0F) + 1, ((1 + ((es->eventParm & 0xF0) >> 4)) * 2000) + 1000);
		break;

    case EV_EXPLOSION:
        dir[0] = 0;
        dir[1] = 0;
        dir[2] = 25;
        CG_ParticleExplosion( "explode1", cent->lerpOrigin, dir, 1400, 20, 30 );

        // show explosion
		dir[0] = 0;
		dir[1] = 0;
		dir[2] = 0;
		CG_MakeExplosion( cent->lerpOrigin, dir, cgs.media.dishFlashModel, cgs.media.rocketExplosionShader, 1000, qtrue );
		break;

    case EV_PARTICLES_GRAVITY:
		CG_ParticlesFromEntityState( cent->lerpOrigin, PT_GRAVITY, es );
		break;

	case EV_PARTICLES_LINEAR:
		CG_ParticlesFromEntityState( cent->lerpOrigin, PT_LINEAR_BOTH, es );
		break;

    case EV_PARTICLES_LINEAR_UP:
		CG_ParticlesFromEntityState( cent->lerpOrigin, PT_LINEAR_UP, es );
		break;

	case EV_PARTICLES_LINEAR_DOWN:
		CG_ParticlesFromEntityState( cent->lerpOrigin, PT_LINEAR_DOWN, es );
		break;
//---------------------------- landing sounds below ------------------------------------------------------------------
    case EV_LANDSOUND:
        if (cg_landingSound.integer || cgs.gametype == GT_ADVENTURE)
            trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound[LANDSOUND_NORMAL] );
        break;

	case EV_LANDSOUND_METAL:
	    if (cg_landingSound.integer || cgs.gametype == GT_ADVENTURE)
            trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound[LANDSOUND_METAL] );
        break;

	case EV_LANDSOUND_FLESH:
	    if (cg_landingSound.integer || cgs.gametype == GT_ADVENTURE)
            trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound[LANDSOUND_FLESH] );
        break;

	case EV_LANDSOUND_STONE:
	    if (cg_landingSound.integer || cgs.gametype == GT_ADVENTURE)
            trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound[LANDSOUND_STONE] );
        break;

	case EV_LANDSOUND_GRASS:
	    if (cg_landingSound.integer || cgs.gametype == GT_ADVENTURE)
            trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound[LANDSOUND_GRASS] );
        break;

	case EV_LANDSOUND_TALLGRASS:
	    if (cg_landingSound.integer || cgs.gametype == GT_ADVENTURE)
            trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound[LANDSOUND_TALLGRASS] );
        break;

	case EV_LANDSOUND_WOOD:
	    if (cg_landingSound.integer || cgs.gametype == GT_ADVENTURE)
            trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound[LANDSOUND_WOOD] );
        break;

	case EV_LANDSOUND_SAND:
	    if (cg_landingSound.integer || cgs.gametype == GT_ADVENTURE)
            trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound[LANDSOUND_SAND] );
        break;

	case EV_LANDSOUND_LAVA:
	    if (cg_landingSound.integer || cgs.gametype == GT_ADVENTURE)
            trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound[LANDSOUND_LAVA] );
        break;

	case EV_LANDSOUND_ICE:
	    if (cg_landingSound.integer || cgs.gametype == GT_ADVENTURE)
            trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound[LANDSOUND_ICE] );
        break;

	default:
		CG_Error( "Unknown event: %i", event );
		break;
	}
}


/*
==============
CG_CheckEvents

==============
*/
void CG_CheckEvents( centity_t *cent ) {
	// check for event-only entities
	if ( cent->currentState.eType > ET_EVENTS ) {
		if ( cent->previousEvent ) 
			return;	// already fired
		
		// if this is a player event set the entity number of the client entity number
		if ( cent->currentState.eFlags & EF_PLAYER_EVENT ) 
			cent->currentState.number = cent->currentState.otherEntityNum;
		
		cent->previousEvent = 1;
		cent->currentState.event = cent->currentState.eType - ET_EVENTS;
	} else {
		// check for events riding with another entity
		if ( cent->currentState.event == cent->previousEvent ) 
			return;
		
		cent->previousEvent = cent->currentState.event;
		if ( ( cent->currentState.event & ~EV_EVENT_BITS ) == 0 ) 
			return;
	}

	// calculate the position at exactly the frame time
	BG_EvaluateTrajectory( &cent->currentState.pos, cg.snap->serverTime, cent->lerpOrigin );
	CG_SetEntitySoundPosition( cent );
	CG_EntityEvent( cent, cent->lerpOrigin, -1 );
}
