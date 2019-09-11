// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_event.c -- handle entity events at snapshot or playerstate transitions

#include "cg_local.h"

// for the voice chats
#ifdef MISSIONPACK // bk001205
#include "../../ui/menudef.h"
#endif
//==========================================================================

/*
===================
CG_PlaceString

Also called by scoreboard drawing
===================
*/
const char	*CG_PlaceString( int rank ) {
	static char	str[64];
	char	*s, *t;

	if ( rank & RANK_TIED_FLAG ) {
		rank &= ~RANK_TIED_FLAG;
		t = "Tied for ";
	} else {
		t = "";
	}

	if ( rank == 1 ) {
		s = S_COLOR_BLUE "1st" S_COLOR_WHITE;		// draw in blue
	} else if ( rank == 2 ) {
		s = S_COLOR_RED "2nd" S_COLOR_WHITE;		// draw in red
	} else if ( rank == 3 ) {
		s = S_COLOR_YELLOW "3rd" S_COLOR_WHITE;		// draw in yellow
	} else if ( rank == 11 ) {
		s = "11th";
	} else if ( rank == 12 ) {
		s = "12th";
	} else if ( rank == 13 ) {
		s = "13th";
	} else if ( rank % 10 == 1 ) {
		s = va("%ist", rank);
	} else if ( rank % 10 == 2 ) {
		s = va("%ind", rank);
	} else if ( rank % 10 == 3 ) {
		s = va("%ird", rank);
	} else {
		s = va("%ith", rank);
	}

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
	//NT - some randomness
	static int	seed = 0x4A;
	int			r;


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
	if ( !targetInfo ) {
		return;
	}
	Q_strncpyz( targetName, Info_ValueForKey( targetInfo, "n" ), sizeof(targetName) - 2);
	strcat( targetName, S_COLOR_WHITE );
	//NT - filter the name if necessary
	if ( cg_nameFilter.integer || ( cgs.forceNameFilter && cg_allowForceFilters.integer ) ) {
		CG_FilterName( targetName );
	}

	message2 = "";

	//NT
	r = abs( Q_rand( &seed ) ) % 3;

	// check for single client messages

	switch( mod ) {
	case MOD_SUICIDE:
		message = "suicides";
		break;
	case MOD_FALLING:
		message = "cratered";
		break;
	case MOD_CRUSH:
		message = "was squished";
		break;
	case MOD_WATER:
		message = "sank like a rock";
		break;
	case MOD_SLIME:
		message = "melted";
		break;
	case MOD_LAVA:
		message = "does a back flip into the lava";
		break;
	case MOD_TARGET_LASER:
		message = "saw the light";
		break;
	case MOD_TRIGGER_HURT:
		message = "was in the wrong place";
		break;
	default:
		message = NULL;
		break;
	}

	if (attacker == target) {
		gender = ci->gender;
		switch (mod) {
#ifdef MISSIONPACK
		case MOD_KAMIKAZE:
			message = "goes out with a bang";
			break;
#endif
		//NT - with holograms, you can grapple yourself
		case MOD_GRAPPLE:
			if ( gender == GENDER_FEMALE )
				message = "grappled herself";
			else if ( gender == GENDER_NEUTER )
				message = "grappled itself";
			else
				message = "grappled himself";
			break;
		//NT - with holograms, you can pummel yourself
		case MOD_GAUNTLET:
			if ( gender == GENDER_FEMALE )
				message = "pummeled herself";
			else if ( gender == GENDER_NEUTER )
				message = "pummeled itself";
			else
				message = "pummeled himself";
			break;
		//NT - with holograms, you can shoot yourself up
		case MOD_MACHINEGUN:
			if ( gender == GENDER_FEMALE )
				message = "shot herself up";
			else if ( gender == GENDER_NEUTER )
				message = "shot itself up";
			else
				message = "shot himself up";
			break;
		//NT - with holograms, you can kill yourself with a shotgun
		case MOD_SHOTGUN:
			if ( gender == GENDER_FEMALE )
				message = "chewed on her own broomstick";
			else if ( gender == GENDER_NEUTER )
				message = "chewed on its own broomstick";
			else
				message = "chewed on his own broomstick";
			break;
		//NT - PROX_SPLASH
		case MOD_PROX_SPLASH:
		case MOD_GRENADE:
		case MOD_GRENADE_SPLASH:
			if ( gender == GENDER_FEMALE )
				message = "tripped on her own grenade";
			else if ( gender == GENDER_NEUTER )
				message = "tripped on its own grenade";
			else
				message = "tripped on his own grenade";
			break;
		//NT - CHAFF_SPLASH
		case MOD_CHAFF:
		case MOD_CHAFF_SPLASH:
			if ( gender == GENDER_FEMALE )
				message = "chaffed herself";
			else if ( gender == GENDER_NEUTER )
				message = "chaffed itself";
			else
				message = "chaffed himself";
			break;
		//NT - HOMING_SPLASH
		case MOD_HOMING:
		case MOD_HOMING_SPLASH:
		case MOD_ROCKET:
		case MOD_ROCKET_SPLASH:
			if ( gender == GENDER_FEMALE )
				message = "blew herself up";
			else if ( gender == GENDER_NEUTER )
				message = "blew itself up";
			else
				message = "blew himself up";
			break;
		case MOD_ORBITING_PLASMA:
			if ( gender == GENDER_FEMALE )
				message = "got into her hologram's comfort zone";
			else if ( gender == GENDER_NEUTER )
				message = "got into its hologram's comfort zone";
			else
				message = "got into his hologram's comfort zone";
			break;
		case MOD_RADIUS_RAILGUN:
			if ( gender == GENDER_FEMALE )
				message = "ripped herself apart with uranium slugs";
			else if ( gender == GENDER_NEUTER )
				message = "ripped itself apart with uranium slugs";
			else
				message = "ripped himself apart with uranium slugs";
			break;
		case MOD_RAILGUN:
			if ( gender == GENDER_FEMALE )
				message = "railed herself";
			else if ( gender == GENDER_NEUTER )
				message = "railed itself";
			else
				message = "railed himself";
			break;
		case MOD_LIGHTNING:
			if ( gender == GENDER_FEMALE )
				message = "charged herself up in stasis";
			else if ( gender == GENDER_NEUTER )
				message = "charged itself up in stasis";
			else
				message = "charged himself up in stasis";
			break;
		case MOD_PLASMA:
		case MOD_PLASMA_SPLASH:
			if ( gender == GENDER_FEMALE )
				message = "melted herself";
			else if ( gender == GENDER_NEUTER )
				message = "melted itself";
			else
				message = "melted himself";
			break;
		case MOD_BFG:
		case MOD_BFG_SPLASH:
			message = "should have used a smaller gun";
			break;
#ifdef MISSIONPACK
		case MOD_PROXIMITY_MINE:
			if( gender == GENDER_FEMALE ) {
				message = "found her prox mine";
			} else if ( gender == GENDER_NEUTER ) {
				message = "found it's prox mine";
			} else {
				message = "found his prox mine";
			}
			break;
#endif
		//NT - AMMO
		case MOD_AMMO:
			if( gender == GENDER_FEMALE ) {
				message = "did herself in with an ammo box";
			} else if ( gender == GENDER_NEUTER ) {
				message = "did itself in with an ammo box";
			} else {
				message = "did himself in with an ammo box";
			}
			break;
		//NT - BFG overheat
		case MOD_BFG_OVERHEAT:
			if( gender == GENDER_FEMALE ) {
				message = "let her big bad BFG get a little too hot";
			} else if ( gender == GENDER_NEUTER ) {
				message = "let its big bad BFG get a little too hot";
			} else {
				message = "let his big bad BFG get a little too hot";
			}
			break;
		//NT - lightning gun discharge
		case MOD_DISCHARGE:
			if( gender == GENDER_FEMALE ) {
				message = "unsuccessfully tested her water-as-an-insulator theory";
			} else if ( gender == GENDER_NEUTER ) {
				message = "unsuccessfully tested its water-as-an-insulator theory";
			} else {
				message = "unsuccessfully tested his water-as-an-insulator theory";
			}
			break;
		//NT - with holograms, you can now telefrag yourself (muhaha)
		case MOD_TELEFRAG:
			if ( gender == GENDER_FEMALE )
				message = "telefragged herself";
			else if ( gender == GENDER_NEUTER )
				message = "telefragged itself";
			else
				message = "telefragged himself";
			break;
		default:
			if ( gender == GENDER_FEMALE )
				message = "killed herself";
			else if ( gender == GENDER_NEUTER )
				message = "killed itself";
			else
				message = "killed himself";
			break;
		}
	}

	if (message) {
		CG_Printf( "%s %s.\n", targetName, message);
		return;
	}

	// check for kill messages from the current clientNum
	if ( attacker == cg.snap->ps.clientNum ) {
		char	*s;

		if ( cgs.gametype < GT_TEAM ) {
			s = va("You fragged %s\n%s place with %i", targetName, 
				CG_PlaceString( cg.snap->ps.persistant[PERS_RANK] + 1 ),
				cg.snap->ps.persistant[PERS_SCORE] );
		} else {
			s = va("You fragged %s", targetName );
		}
#ifdef MISSIONPACK
		if (!(cg_singlePlayerActive.integer && cg_cameraOrbit.integer)) {
			CG_CenterPrint( s, SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );
		} 
#else
		CG_CenterPrint( s, SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );
#endif

		// print the text message as well
	}

	// check for double client messages
	if ( !attackerInfo ) {
		attacker = ENTITYNUM_WORLD;
		strcpy( attackerName, "noname" );
	} else {
		Q_strncpyz( attackerName, Info_ValueForKey( attackerInfo, "n" ), sizeof(attackerName) - 2);
		strcat( attackerName, S_COLOR_WHITE );
		// check for kill messages about the current clientNum
		if ( target == cg.snap->ps.clientNum ) {
			Q_strncpyz( cg.killerName, attackerName, sizeof( cg.killerName ) );
		}
	}

	//NT - added a bit of randomness (and wit, hopefully) to the obituaries
	//NT - also added obituaries for new MOD's
	if ( attacker != ENTITYNUM_WORLD ) {
		switch (mod) {
		case MOD_GRAPPLE:
			message = "was caught by";
			break;
		case MOD_GAUNTLET:
			if (r == 0) {
				message = "was pummeled by";
			} else if (r == 1) {
				message = "was most UN-surgically altered by";
			} else if (r == 2) {
				message = "GIVES";
				message2 = " a frag";
			}
			break;
		case MOD_MACHINEGUN:
			if (r == 0) {
				message = "was perforated by";
				message2 = "'s greaser";
			} else if (r == 1) {
				message = "was ripped through by";
				message2 = "'s violin";
			} else if (r == 2) {
				message = "was shot up by";
			}
			break;
		case MOD_SHOTGUN:
			if (r == 0) {
				message = "chewed on";
				message2 = "'s broomstick";
			} else if (r == 1) {
				message = "took a fistful of";
				message2 = "'s buckshot";
			} else if (r == 2) {
				message = "paletted a peck of";
				message2 = "'s pellets";
			}
			break;
		case MOD_GRENADE:
			if (r == 0) {
				message = "ate";
				message2 = "'s pineapple";
			} else if (r == 1) {
				message = "couldn't stomach";
				message2 = "'s grenade";
			} else if (r == 2) {
				message = "ingested an explosive gift from";
			}
			break;
		case MOD_PROX_SPLASH:
		case MOD_GRENADE_SPLASH:
			if (r == 0) {
				message = "was shredded by";
				message2 = "'s shrapnel";
			} else if (r == 1) {
				message = "got too close to";
				message2 = "'s ticker";
			} else if (r == 2) {
				message = "was deformed by";
				message2 = "'s metal bits";
			}
			break;
		case MOD_CHAFF:
			message = "gagged on";
			message2 = "'s hot chaff";
			break;
		case MOD_CHAFF_SPLASH:
			message = "was scorched by";
			message2 = "'s chaff";
			break;
		case MOD_HOMING:
			if (r == 0) {
				message = "ate";
				message2 = "'s homing missile";
			} else if (r == 1) {
				message = "was masticated by";
				message2 = "'s homing missile";
			} else if (r == 2) {
				message = "was too slow to get away from";
				message2 = "'s rocket";
			}
			break;
		case MOD_ROCKET:
			if (r == 0) {
				message = "ate";
				message2 = "'s rocket";
			} else if (r == 1) {
				message = "was masticated by";
				message2 = "'s missile";
			} else if (r == 2) {
				message = "didn't dodge";
				message2 = "'s rocket";
			}
			break;
		case MOD_HOMING_SPLASH:
			message = "was chased down by";
			message2 = "'s rocket";
			break;
		case MOD_ROCKET_SPLASH:
			message = "almost dodged";
			message2 = "'s rocket";
			break;
		case MOD_ORBITING_PLASMA:
			if (r == 0 ) {
				message = "got into";
				message2 = "'s comfort zone";
			} else if (r == 1 ) {
				message = "was mulched by";
				message2 = "'s blue disease";
			} else if (r == 2 ) {
				message = "was terminally blighted by";
				message2 = "'s blue disease";
			}
			break;
		case MOD_PLASMA:
		case MOD_PLASMA_SPLASH:
			message = "was melted by";
			message2 = "'s blue-hot plasma";
			break;
		case MOD_RADIUS_RAILGUN:
			if (r == 0 ) {
				message = "was ripped apart by";
				message2 = "'s depleted uranium nastiness";
			} else if (r == 1 ) {
				message = "was brutally battered by";
				message2 = "'s pet slugs";
			} else if (r == 2 ) {
				message = "was knocked around by";
				message2 = "'s lightspeed leftovers";
			}
			break;
		case MOD_RAILGUN:
			if (r == 0 ) {
				message = "was torn apart by";
				message2 = "'s relativistic depleted uranium";
			} else if (r == 1 ) {
				message = "was brutally battered by";
				message2 = "'s pet slug";
			} else if (r == 2 ) {
				message = "swallowed";
				message2 = "'s lightspeed leftovers";
			}
			break;
		case MOD_LIGHTNING:
			if (r == 0) {
				message = "was tested for conductivity by";
			} else if (r == 1) {
				message = "was negatively charged by";
			} else if (r == 2) {
				message = "was shafted by";
			}
			break;
		case MOD_BFG:
		case MOD_BFG_SPLASH:
			message = "was blasted by";
			message2 = "'s BFG";
			break;
		case MOD_BFG_OVERLOAD:
			message = "was sabotaged by";
			message2 = "'s overloaded BFG";
			break;
		case MOD_BFG_OVERHEAT:
			message = "got burned by";
			message2 = "'s overheated BFG";
			break;
#ifdef MISSIONPACK
		case MOD_NAIL:
			message = "was nailed by";
			break;
		case MOD_CHAINGUN:
			message = "got lead poisoning from";
			message2 = "'s Chaingun";
			break;
		case MOD_PROXIMITY_MINE:
			message = "was too close to";
			message2 = "'s Prox Mine";
			break;
		case MOD_KAMIKAZE:
			message = "falls to";
			message2 = "'s Kamikaze blast";
			break;
		case MOD_JUICED:
			message = "was juiced by";
			break;
#endif
		case MOD_TELEFRAG:
			if (r == 0) {
				message = "tried to invade";
				message2 = "'s personal space";
			} else if (r == 1) {
				message = "tried to occupy the same space as";
			} else if (r == 2) {
				message = "was telefragged by";
			}
			break;
		case MOD_LIGHTNING_WEB:
			if (r == 0) {
				message = "got tangled inside";
				message2 = "'s lightning web";
			} else if (r == 1) {
				message = "was sucked dry by";
				message2 = "'s electron spider";
			} else if (r == 2) {
				message = "was negatively charged by";
				message2 = "'s lightning web";
			}
			break;
		case MOD_AMMO:
			if (r == 0) {
				message = "got too close to";
				message2 = "'s ammo trap";
			} else if (r == 1) {
				message = "was indirectly done in by";
				message2 = "'s gunless gifts";
			} else if (r == 2) {
				message = "was ammo-ed by";
			}
			break;
		case MOD_DISCHARGE:
			if (r == 0) {
				message = "helped test";
				message2 = "'s water-as-an-insulator theory";
			} else if (r == 1) {
				message = "was electrocuted by";
				message2 = "'s underwater discharge";
			} else if (r == 2) {
				message = "was in close proximity to";
				message2= "'s discharging lightning gun";
			}
			break;
		case MOD_QUAD_REFLECTION:
			message = "took too much reflected damage from";
			break;
		default:
			message = "was killed by";
			break;
		}

		if (message) {
			CG_Printf( "%s %s %s%s\n", 
				targetName, message, attackerName, message2);
			return;
		}
	}

	// we don't know what it was
	CG_Printf( "%s died.\n", targetName );
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
	if ( es->number == cg.snap->ps.clientNum ) {
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

	//NT - hologram
	case HI_HOLOGRAM:
		break;

#ifdef MISSIONPACK
	case HI_KAMIKAZE:
		break;

	case HI_PORTAL:
		break;
	case HI_INVULNERABILITY:
		trap_S_StartSound (NULL, es->number, CHAN_BODY, cgs.media.useInvulnerabilitySound );
		break;
#endif
	}

}

/*
================
CG_ItemPickup

A new item was picked up this frame
================
*/
static void CG_ItemPickup( int itemNum ) {
	cg.itemPickup = itemNum;
	cg.itemPickupTime = cg.time;
	cg.itemPickupBlendTime = cg.time;
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
CG_PainEvent

Also called by playerstate transition
================
*/
void CG_PainEvent( centity_t *cent, int health ) {
	char	*snd;

	// don't do more than two pain sounds a second
	if ( cg.time - cent->pe.painTime < 500 ) {
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
	trap_S_StartSound( NULL, cent->currentState.number, CHAN_VOICE, 
		CG_CustomSound( cent->currentState.number, snd ) );

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
#define	DEBUGNAME(x) if(cg_debugEvents.integer){CG_Printf(x"\n");}
void CG_EntityEvent( centity_t *cent, vec3_t position ) {
	entityState_t	*es;
	int				event;
	vec3_t			dir;
	const char		*s;
	int				clientNum;
	clientInfo_t	*ci;

	es = &cent->currentState;
	event = es->event & ~EV_EVENT_BITS;

	if ( cg_debugEvents.integer ) {
		CG_Printf( "ent:%3i  event:%3i ", es->number, event );
	}

	if ( !event ) {
		DEBUGNAME("ZEROEVENT");
		return;
	}

	clientNum = es->clientNum;
	if ( clientNum < 0 || clientNum >= MAX_CLIENTS ) {
		clientNum = 0;
	}
	ci = &cgs.clientinfo[ clientNum ];

	switch ( event ) {
	//
	// movement generated events
	//
	case EV_FOOTSTEP:
		DEBUGNAME("EV_FOOTSTEP");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ ci->footsteps ][rand()&3] );
		}
		break;
	case EV_FOOTSTEP_METAL:
		DEBUGNAME("EV_FOOTSTEP_METAL");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_METAL ][rand()&3] );
		}
		break;
	case EV_FOOTSPLASH:
		DEBUGNAME("EV_FOOTSPLASH");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		}
		break;
	case EV_FOOTWADE:
		DEBUGNAME("EV_FOOTWADE");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		}
		break;
	case EV_SWIM:
		DEBUGNAME("EV_SWIM");
		if (cg_footsteps.integer) {
			trap_S_StartSound (NULL, es->number, CHAN_BODY, 
				cgs.media.footsteps[ FOOTSTEP_SPLASH ][rand()&3] );
		}
		break;


	case EV_FALL_SHORT:
		DEBUGNAME("EV_FALL_SHORT");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.landSound );
		if ( clientNum == cg.predictedPlayerState.clientNum ) {
			// smooth landing z changes
			cg.landChange = -8;
			cg.landTime = cg.time;
		}
		break;
	case EV_FALL_MEDIUM:
		DEBUGNAME("EV_FALL_MEDIUM");
		// use normal pain sound
		trap_S_StartSound( NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*pain100_1.wav" ) );
		if ( clientNum == cg.predictedPlayerState.clientNum ) {
			// smooth landing z changes
			cg.landChange = -16;
			cg.landTime = cg.time;
		}
		break;
	case EV_FALL_FAR:
		DEBUGNAME("EV_FALL_FAR");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound( es->number, "*fall1.wav" ) );
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
		DEBUGNAME("EV_STEP");
	{
		float	oldStep;
		int		delta;
		int		step;

		if ( clientNum != cg.predictedPlayerState.clientNum ) {
			break;
		}
		// if we are interpolating, we don't need to smooth steps
		if ( cg.demoPlayback || (cg.snap->ps.pm_flags & PMF_FOLLOW) ||
			cg_nopredict.integer || cg_synchronousClients.integer ) {
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
		if ( cg.stepChange > MAX_STEP_CHANGE ) {
			cg.stepChange = MAX_STEP_CHANGE;
		}
		cg.stepTime = cg.time;
		break;
	}

	case EV_JUMP_PAD:
		DEBUGNAME("EV_JUMP_PAD");
//		CG_Printf( "EV_JUMP_PAD w/effect #%i\n", es->eventParm );
		{
			localEntity_t	*smoke;
			vec3_t			up = {0, 0, 1};


			smoke = CG_SmokePuff( cent->lerpOrigin, up, 
						  32, 
						  1, 1, 1, 0.33f,
						  1000, 
						  cg.time, 0,
						  LEF_PUFF_DONT_SCALE, 
						  cgs.media.smokePuffShader );
		}

		// boing sound at origin, jump sound on player
		trap_S_StartSound ( cent->lerpOrigin, -1, CHAN_VOICE, cgs.media.jumpPadSound );
		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*jump1.wav" ) );
		break;

	case EV_JUMP:
		DEBUGNAME("EV_JUMP");
		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*jump1.wav" ) );
		break;
	case EV_TAUNT:
		DEBUGNAME("EV_TAUNT");
		trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, "*taunt.wav" ) );
		break;
#ifdef MISSIONPACK
	case EV_TAUNT_YES:
		DEBUGNAME("EV_TAUNT_YES");
		CG_VoiceChatLocal(SAY_TEAM, qfalse, es->number, COLOR_CYAN, VOICECHAT_YES);
		break;
	case EV_TAUNT_NO:
		DEBUGNAME("EV_TAUNT_NO");
		CG_VoiceChatLocal(SAY_TEAM, qfalse, es->number, COLOR_CYAN, VOICECHAT_NO);
		break;
	case EV_TAUNT_FOLLOWME:
		DEBUGNAME("EV_TAUNT_FOLLOWME");
		CG_VoiceChatLocal(SAY_TEAM, qfalse, es->number, COLOR_CYAN, VOICECHAT_FOLLOWME);
		break;
	case EV_TAUNT_GETFLAG:
		DEBUGNAME("EV_TAUNT_GETFLAG");
		CG_VoiceChatLocal(SAY_TEAM, qfalse, es->number, COLOR_CYAN, VOICECHAT_ONGETFLAG);
		break;
	case EV_TAUNT_GUARDBASE:
		DEBUGNAME("EV_TAUNT_GUARDBASE");
		CG_VoiceChatLocal(SAY_TEAM, qfalse, es->number, COLOR_CYAN, VOICECHAT_ONDEFENSE);
		break;
	case EV_TAUNT_PATROL:
		DEBUGNAME("EV_TAUNT_PATROL");
		CG_VoiceChatLocal(SAY_TEAM, qfalse, es->number, COLOR_CYAN, VOICECHAT_ONPATROL);
		break;
#endif
	case EV_WATER_TOUCH:
		DEBUGNAME("EV_WATER_TOUCH");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrInSound );
		break;
	case EV_WATER_LEAVE:
		DEBUGNAME("EV_WATER_LEAVE");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrOutSound );
		break;
	case EV_WATER_UNDER:
		DEBUGNAME("EV_WATER_UNDER");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.watrUnSound );
		break;
	case EV_WATER_CLEAR:
		DEBUGNAME("EV_WATER_CLEAR");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, CG_CustomSound( es->number, "*gasp.wav" ) );
		break;

	case EV_ITEM_PICKUP:
		DEBUGNAME("EV_ITEM_PICKUP");
		{
			gitem_t	*item;
			int		index;

			index = es->eventParm;		// player predicted

			if ( index < 1 || index >= bg_numItems ) {
				break;
			}
			item = &bg_itemlist[ index ];

			// powerups and team items will have a separate global sound, this one
			// will be played at prediction time
			if ( item->giType == IT_POWERUP || item->giType == IT_TEAM) {
				trap_S_StartSound (NULL, es->number, CHAN_AUTO,	cgs.media.n_healthSound );
			} else if (item->giType == IT_PERSISTANT_POWERUP) {
#ifdef MISSIONPACK
				switch (item->giTag ) {
					case PW_SCOUT:
						trap_S_StartSound (NULL, es->number, CHAN_AUTO,	cgs.media.scoutSound );
					break;
					case PW_GUARD:
						trap_S_StartSound (NULL, es->number, CHAN_AUTO,	cgs.media.guardSound );
					break;
					case PW_DOUBLER:
						trap_S_StartSound (NULL, es->number, CHAN_AUTO,	cgs.media.doublerSound );
					break;
					case PW_AMMOREGEN:
						trap_S_StartSound (NULL, es->number, CHAN_AUTO,	cgs.media.ammoregenSound );
					break;
				}
#endif
			} else {
				trap_S_StartSound (NULL, es->number, CHAN_AUTO,	trap_S_RegisterSound( item->pickup_sound, qfalse ) );
			}

			// show icon and name on status bar
			if ( es->number == cg.snap->ps.clientNum ) {
				CG_ItemPickup( index );
			}
		}
		break;

	case EV_GLOBAL_ITEM_PICKUP:
		DEBUGNAME("EV_GLOBAL_ITEM_PICKUP");
		{
			gitem_t	*item;
			int		index;

			index = es->eventParm;		// player predicted

			if ( index < 1 || index >= bg_numItems ) {
				break;
			}
			item = &bg_itemlist[ index ];
			// powerup pickups are global
			if( item->pickup_sound ) {
				trap_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, trap_S_RegisterSound( item->pickup_sound, qfalse ) );
			}

			// show icon and name on status bar
			if ( es->number == cg.snap->ps.clientNum ) {
				CG_ItemPickup( index );
			}
		}
		break;

	//
	// weapon events
	//
	case EV_NOAMMO:
		DEBUGNAME("EV_NOAMMO");
//		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.noAmmoSound );
		if ( es->number == cg.snap->ps.clientNum ) {
			CG_OutOfAmmoChange();
		}
		break;
	case EV_CHANGE_WEAPON:
		DEBUGNAME("EV_CHANGE_WEAPON");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.selectSound );
		break;
	case EV_FIRE_WEAPON:
		DEBUGNAME("EV_FIRE_WEAPON");
		CG_FireWeapon( cent );
		break;
	//NT - secondary attack events
	case EV_FIRE_WEAPON2:
		DEBUGNAME("EV_FIRE_WEAPON2");
		CG_FireWeapon2( cent );
		break;

	case EV_USE_ITEM0:
		DEBUGNAME("EV_USE_ITEM0");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM1:
		DEBUGNAME("EV_USE_ITEM1");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM2:
		DEBUGNAME("EV_USE_ITEM2");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM3:
		DEBUGNAME("EV_USE_ITEM3");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM4:
		DEBUGNAME("EV_USE_ITEM4");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM5:
		DEBUGNAME("EV_USE_ITEM5");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM6:
		DEBUGNAME("EV_USE_ITEM6");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM7:
		DEBUGNAME("EV_USE_ITEM7");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM8:
		DEBUGNAME("EV_USE_ITEM8");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM9:
		DEBUGNAME("EV_USE_ITEM9");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM10:
		DEBUGNAME("EV_USE_ITEM10");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM11:
		DEBUGNAME("EV_USE_ITEM11");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM12:
		DEBUGNAME("EV_USE_ITEM12");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM13:
		DEBUGNAME("EV_USE_ITEM13");
		CG_UseItem( cent );
		break;
	case EV_USE_ITEM14:
		DEBUGNAME("EV_USE_ITEM14");
		CG_UseItem( cent );
		break;

	//=================================================================

	//
	// other events
	//
	case EV_PLAYER_TELEPORT_IN:
		DEBUGNAME("EV_PLAYER_TELEPORT_IN");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.teleInSound );
		CG_SpawnEffect( position);
		break;

	case EV_PLAYER_TELEPORT_OUT:
		DEBUGNAME("EV_PLAYER_TELEPORT_OUT");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.teleOutSound );
		CG_SpawnEffect(  position);
		break;

	case EV_ITEM_POP:
		DEBUGNAME("EV_ITEM_POP");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.respawnSound );
		break;
	case EV_ITEM_RESPAWN:
		DEBUGNAME("EV_ITEM_RESPAWN");
		cent->miscTime = cg.time;	// scale up from this
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.respawnSound );
		break;

	case EV_GRENADE_BOUNCE:
		DEBUGNAME("EV_GRENADE_BOUNCE");
		if ( rand() & 1 ) {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.hgrenb1aSound );
		} else {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.hgrenb2aSound );
		}
		break;

#ifdef MISSIONPACK
	case EV_PROXIMITY_MINE_STICK:
		DEBUGNAME("EV_PROXIMITY_MINE_STICK");
		if( es->eventParm & SURF_FLESH ) {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.wstbimplSound );
		} else 	if( es->eventParm & SURF_METALSTEPS ) {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.wstbimpmSound );
		} else {
			trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.wstbimpdSound );
		}
		break;

	case EV_PROXIMITY_MINE_TRIGGER:
		DEBUGNAME("EV_PROXIMITY_MINE_TRIGGER");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.wstbactvSound );
		break;
	case EV_KAMIKAZE:
		DEBUGNAME("EV_KAMIKAZE");
		CG_KamikazeEffect( cent->lerpOrigin );
		break;
	case EV_OBELISKEXPLODE:
		DEBUGNAME("EV_OBELISKEXPLODE");
		CG_ObeliskExplode( cent->lerpOrigin, es->eventParm );
		break;
	case EV_OBELISKPAIN:
		DEBUGNAME("EV_OBELISKPAIN");
		CG_ObeliskPain( cent->lerpOrigin );
		break;
	case EV_INVUL_IMPACT:
		DEBUGNAME("EV_INVUL_IMPACT");
		CG_InvulnerabilityImpact( cent->lerpOrigin, cent->currentState.angles );
		break;
	case EV_JUICED:
		DEBUGNAME("EV_JUICED");
		CG_InvulnerabilityJuiced( cent->lerpOrigin );
		break;
	case EV_LIGHTNINGBOLT:
		DEBUGNAME("EV_LIGHTNINGBOLT");
		CG_LightningBoltBeam(es->origin2, es->pos.trBase);
		break;
#endif
	case EV_SCOREPLUM:
		DEBUGNAME("EV_SCOREPLUM");
		CG_ScorePlum( cent->currentState.otherEntityNum, cent->lerpOrigin, cent->currentState.time );
		break;

	//
	// missile impacts
	//
	case EV_MISSILE_HIT:
		DEBUGNAME("EV_MISSILE_HIT");
		ByteToDir( es->eventParm, dir );
		CG_MissileHitPlayer( es->weapon, position, dir, es->otherEntityNum );
		break;

	case EV_MISSILE_MISS:
		DEBUGNAME("EV_MISSILE_MISS");
		ByteToDir( es->eventParm, dir );
		//NT - added radius multiplier (send as desired multiplier * 16 in generic1)
		if ( es->generic1 )
			CG_MissileHitWall( es->weapon, 0, position, dir, IMPACTSOUND_DEFAULT, ((float)es->generic1) / 16.0 );
		else
			CG_MissileHitWall( es->weapon, 0, position, dir, IMPACTSOUND_DEFAULT, 1.0 );
		break;

	case EV_MISSILE_MISS_METAL:
		DEBUGNAME("EV_MISSILE_MISS_METAL");
		ByteToDir( es->eventParm, dir );
		//NT - radius multiplier
		CG_MissileHitWall( es->weapon, 0, position, dir, IMPACTSOUND_METAL, 1.0 );
		break;

	case EV_RAILTRAIL:
		DEBUGNAME("EV_RAILTRAIL");
		cent->currentState.weapon = WP_RAILGUN;
		// if the end was on a nomark surface, don't make an explosion
		if ( es->eventParm != 255 ) {
			ByteToDir( es->eventParm, dir );
			//NT - radius multiplier
			CG_MissileHitWall( es->weapon, es->clientNum, position, dir, IMPACTSOUND_DEFAULT, 1.0 );
		}
		CG_RailTrail( ci, es->origin2, es->pos.trBase );
		break;

	//NT - radius rail trail (much coolness)
	case EV_RAILTRAIL2:
		DEBUGNAME("EV_RAILTRAIL2");
		cent->currentState.weapon = WP_RAILGUN;
		// if the end was on a nomark surface, don't make an explosion
		if ( es->eventParm != 255 ) {
			ByteToDir( es->eventParm, dir );
			CG_MissileHitWall( es->weapon, es->clientNum, position, dir, IMPACTSOUND_DEFAULT, 3.0 );
		}
		CG_RailTrail2( ci, es->origin2, es->pos.trBase );
		break;

	case EV_BULLET_HIT_WALL:
		DEBUGNAME("EV_BULLET_HIT_WALL");
		ByteToDir( es->eventParm, dir );
		CG_Bullet( es->pos.trBase, es->otherEntityNum, dir, qfalse, ENTITYNUM_WORLD );
		break;

	case EV_BULLET_HIT_FLESH:
		DEBUGNAME("EV_BULLET_HIT_FLESH");
		CG_Bullet( es->pos.trBase, es->otherEntityNum, dir, qtrue, es->eventParm );
		break;

	case EV_SHOTGUN:
		DEBUGNAME("EV_SHOTGUN");
		CG_SuperShotgunFire( es );
		break;

	//NT - one-shell shotgun
	case EV_SHOTGUN2:
		DEBUGNAME("EV_SHOTGUN2");
		CG_ShotgunFire( es );
		break;

	//NT - start the warning sound so people can get away
	case EV_START_OVERLOAD:
		DEBUGNAME("EV_START_OVERLOAD");
		trap_S_StartSound( NULL, es->number, CHAN_WEAPON, cgs.media.bfgOverloadSound );
		break;

	case EV_GENERAL_SOUND:
		DEBUGNAME("EV_GENERAL_SOUND");
		if ( cgs.gameSounds[ es->eventParm ] ) {
			trap_S_StartSound (NULL, es->number, CHAN_VOICE, cgs.gameSounds[ es->eventParm ] );
		} else {
			s = CG_ConfigString( CS_SOUNDS + es->eventParm );
			trap_S_StartSound (NULL, es->number, CHAN_VOICE, CG_CustomSound( es->number, s ) );
		}
		break;

	case EV_GLOBAL_SOUND:	// play from the player's head so it never diminishes
		DEBUGNAME("EV_GLOBAL_SOUND");
		if ( cgs.gameSounds[ es->eventParm ] ) {
			trap_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, cgs.gameSounds[ es->eventParm ] );
		} else {
			s = CG_ConfigString( CS_SOUNDS + es->eventParm );
			trap_S_StartSound (NULL, cg.snap->ps.clientNum, CHAN_AUTO, CG_CustomSound( es->number, s ) );
		}
		break;

	case EV_GLOBAL_TEAM_SOUND:	// play from the player's head so it never diminishes
		{
			DEBUGNAME("EV_GLOBAL_TEAM_SOUND");
			switch( es->eventParm ) {
				case GTS_RED_CAPTURE: // CTF: red team captured the blue flag, 1FCTF: red team captured the neutral flag
					//NT
					if ( cgs.clientinfo[cg.clientNum].team == TEAM_RED || cgs.clientinfo[cg.clientNum].team == TEAM_WATCHING_RED )
						CG_AddBufferedSound( cgs.media.captureYourTeamSound );
					else
						CG_AddBufferedSound( cgs.media.captureOpponentSound );
					break;
				case GTS_BLUE_CAPTURE: // CTF: blue team captured the red flag, 1FCTF: blue team captured the neutral flag
					//NT
					if ( cgs.clientinfo[cg.clientNum].team == TEAM_BLUE || cgs.clientinfo[cg.clientNum].team == TEAM_WATCHING_BLUE )
						CG_AddBufferedSound( cgs.media.captureYourTeamSound );
					else
						CG_AddBufferedSound( cgs.media.captureOpponentSound );
					break;
				case GTS_RED_RETURN: // CTF: blue flag returned, 1FCTF: never used
					//NT
					if ( cgs.clientinfo[cg.clientNum].team == TEAM_RED || cgs.clientinfo[cg.clientNum].team == TEAM_WATCHING_RED )
						CG_AddBufferedSound( cgs.media.returnYourTeamSound );
					else
						CG_AddBufferedSound( cgs.media.returnOpponentSound );
					//
					CG_AddBufferedSound( cgs.media.blueFlagReturnedSound );
					break;
				case GTS_BLUE_RETURN: // CTF red flag returned, 1FCTF: neutral flag returned
					//NT
					if ( cgs.clientinfo[cg.clientNum].team == TEAM_BLUE || cgs.clientinfo[cg.clientNum].team == TEAM_WATCHING_BLUE )
						CG_AddBufferedSound( cgs.media.returnYourTeamSound );
					else
						CG_AddBufferedSound( cgs.media.returnOpponentSound );
					//
					CG_AddBufferedSound( cgs.media.redFlagReturnedSound );
					break;

				case GTS_RED_TAKEN: // CTF: red team took blue flag, 1FCTF: blue team took the neutral flag
					// if this player picked up the flag then a sound is played in CG_CheckLocalSounds
					if (cg.snap->ps.powerups[PW_BLUEFLAG] || cg.snap->ps.powerups[PW_NEUTRALFLAG]) {
					}
					else {
					//NT
					if (cgs.clientinfo[cg.clientNum].team == TEAM_BLUE || cgs.clientinfo[cg.clientNum].team == TEAM_WATCHING_BLUE) {
#ifdef MISSIONPACK
							if (cgs.gametype == GT_1FCTF) 
								CG_AddBufferedSound( cgs.media.yourTeamTookTheFlagSound );
							else
#endif
						 	CG_AddBufferedSound( cgs.media.enemyTookYourFlagSound );
						}
						//NT
						else if (cgs.clientinfo[cg.clientNum].team == TEAM_RED || cgs.clientinfo[cg.clientNum].team == TEAM_WATCHING_RED) {
#ifdef MISSIONPACK
							if (cgs.gametype == GT_1FCTF)
								CG_AddBufferedSound( cgs.media.enemyTookTheFlagSound );
							else
#endif
 							CG_AddBufferedSound( cgs.media.yourTeamTookEnemyFlagSound );
						}
					}
					break;
				case GTS_BLUE_TAKEN: // CTF: blue team took the red flag, 1FCTF red team took the neutral flag
					// if this player picked up the flag then a sound is played in CG_CheckLocalSounds
					if (cg.snap->ps.powerups[PW_REDFLAG] || cg.snap->ps.powerups[PW_NEUTRALFLAG]) {
					}
					else {
						//NT
						if (cgs.clientinfo[cg.clientNum].team == TEAM_RED || cgs.clientinfo[cg.clientNum].team == TEAM_WATCHING_RED) {
#ifdef MISSIONPACK
							if (cgs.gametype == GT_1FCTF)
								CG_AddBufferedSound( cgs.media.yourTeamTookTheFlagSound );
							else
#endif
							CG_AddBufferedSound( cgs.media.enemyTookYourFlagSound );
						}
						//NT
						else if (cgs.clientinfo[cg.clientNum].team == TEAM_BLUE || cgs.clientinfo[cg.clientNum].team == TEAM_WATCHING_BLUE) {
#ifdef MISSIONPACK
							if (cgs.gametype == GT_1FCTF)
								CG_AddBufferedSound( cgs.media.enemyTookTheFlagSound );
							else
#endif
							CG_AddBufferedSound( cgs.media.yourTeamTookEnemyFlagSound );
						}
					}
					break;

				//NT - added more cases for flags since you can grab your own
				case GTS_BLUE_TAKEN_OWN:
					// if this player picked up the flag then a sound is played in CG_CheckLocalSounds
					if ( cg.snap->ps.powerups[PW_BLUEFLAG] ) {
					}
					else {
						//NT
						if (cgs.clientinfo[cg.clientNum].team == TEAM_BLUE || cgs.clientinfo[cg.clientNum].team == TEAM_WATCHING_BLUE) {
							CG_AddBufferedSound( cgs.media.yourTeamHasYourFlagSound );
						} else if (cgs.clientinfo[cg.clientNum].team == TEAM_RED || cgs.clientinfo[cg.clientNum].team == TEAM_WATCHING_RED) {
							CG_AddBufferedSound( cgs.media.enemyHasFlagSound );
						}
					}
					break;

				//NT - added more cases for flags since you can grab your own
				case GTS_RED_TAKEN_OWN:
					// if this player picked up the flag then a sound is played in CG_CheckLocalSounds
					if ( cg.snap->ps.powerups[PW_REDFLAG] ) {
					}
					else {
						//NT
						if (cgs.clientinfo[cg.clientNum].team == TEAM_RED || cgs.clientinfo[cg.clientNum].team == TEAM_WATCHING_RED) {
							CG_AddBufferedSound( cgs.media.yourTeamHasYourFlagSound );
						} else if (cgs.clientinfo[cg.clientNum].team == TEAM_BLUE || cgs.clientinfo[cg.clientNum].team == TEAM_WATCHING_BLUE) {
							CG_AddBufferedSound( cgs.media.enemyHasFlagSound );
						}
					}
					break;

				case GTS_REDOBELISK_ATTACKED: // Overload: red obelisk is being attacked
					if (cgs.clientinfo[cg.clientNum].team == TEAM_RED) {
						CG_AddBufferedSound( cgs.media.yourBaseIsUnderAttackSound );
					}
					break;
				case GTS_BLUEOBELISK_ATTACKED: // Overload: blue obelisk is being attacked
					if (cgs.clientinfo[cg.clientNum].team == TEAM_BLUE) {
						CG_AddBufferedSound( cgs.media.yourBaseIsUnderAttackSound );
					}
					break;

				case GTS_REDTEAM_SCORED:
					CG_AddBufferedSound(cgs.media.redScoredSound);
					break;
				case GTS_BLUETEAM_SCORED:
					CG_AddBufferedSound(cgs.media.blueScoredSound);
					break;
				case GTS_REDTEAM_TOOK_LEAD:
					CG_AddBufferedSound(cgs.media.redLeadsSound);
					break;
				case GTS_BLUETEAM_TOOK_LEAD:
					CG_AddBufferedSound(cgs.media.blueLeadsSound);
					break;
				case GTS_TEAMS_ARE_TIED:
					CG_AddBufferedSound( cgs.media.teamsTiedSound );
					break;
#ifdef MISSIONPACK
				case GTS_KAMIKAZE:
					trap_S_StartLocalSound(cgs.media.kamikazeFarSound, CHAN_ANNOUNCER);
					break;
#endif
				default:
					break;
			}
			break;
		}

	case EV_PAIN:
		// local player sounds are triggered in CG_CheckLocalSounds,
		// so ignore events on the player
		DEBUGNAME("EV_PAIN");
		if ( cent->currentState.number != cg.snap->ps.clientNum ) {
			CG_PainEvent( cent, es->eventParm );
		}
		break;

	case EV_DEATH1:
	case EV_DEATH2:
	case EV_DEATH3:
		DEBUGNAME("EV_DEATHx");
		trap_S_StartSound( NULL, es->number, CHAN_VOICE, 
				CG_CustomSound( es->number, va("*death%i.wav", event - EV_DEATH1 + 1) ) );
		break;


	case EV_OBITUARY:
		DEBUGNAME("EV_OBITUARY");
		CG_Obituary( es );
		break;

	//
	// powerup events
	//
	case EV_POWERUP_QUAD:
		DEBUGNAME("EV_POWERUP_QUAD");
		if ( es->number == cg.snap->ps.clientNum ) {
			cg.powerupActive = PW_QUAD;
			cg.powerupTime = cg.time;
		}
		trap_S_StartSound (NULL, es->number, CHAN_ITEM, cgs.media.quadSound );
		break;
	case EV_POWERUP_BATTLESUIT:
		DEBUGNAME("EV_POWERUP_BATTLESUIT");
		if ( es->number == cg.snap->ps.clientNum ) {
			cg.powerupActive = PW_BATTLESUIT;
			cg.powerupTime = cg.time;
		}
		trap_S_StartSound (NULL, es->number, CHAN_ITEM, cgs.media.protectSound );
		break;
	case EV_POWERUP_REGEN:
		DEBUGNAME("EV_POWERUP_REGEN");
		if ( es->number == cg.snap->ps.clientNum ) {
			cg.powerupActive = PW_REGEN;
			cg.powerupTime = cg.time;
		}
		trap_S_StartSound (NULL, es->number, CHAN_ITEM, cgs.media.regenSound );
		break;
	//NT - new powerup (liquid metal)
	case EV_POWERUP_PROTECTION:
		DEBUGNAME("EV_POWERUP_PROTECTION");
		if ( es->number == cg.snap->ps.clientNum ) {
			cg.powerupActive = PW_PROTECTION;
			cg.powerupTime = cg.time;
		}
		break;
	//NT - new powerup (vitality)
	case EV_POWERUP_VITALITY:
		DEBUGNAME("EV_POWERUP_VITALITY");
		if ( es->number == cg.snap->ps.clientNum ) {
			cg.powerupActive = PW_VITALITY;
			cg.powerupTime = cg.time;
		}
		break;

	case EV_GIB_PLAYER:
		DEBUGNAME("EV_GIB_PLAYER");
		// don't play gib sound when using the kamikaze because it interferes
		// with the kamikaze sound, downside is that the gib sound will also
		// not be played when someone is gibbed while just carrying the kamikaze
		if ( !(es->eFlags & EF_KAMIKAZE) ) {
			trap_S_StartSound( NULL, es->number, CHAN_BODY, cgs.media.gibSound );
		}
		CG_GibPlayer( cent->lerpOrigin );
		break;

	case EV_STOPLOOPINGSOUND:
		DEBUGNAME("EV_STOPLOOPINGSOUND");
		trap_S_StopLoopingSound( es->number );
		es->loopSound = 0;
		break;

	case EV_DEBUG_LINE:
		DEBUGNAME("EV_DEBUG_LINE");
		CG_Beam( cent );
		break;

	//NT - destroy a player's webs bacause he died, disconnected, or changed teams
	case EV_DESTROY_WEBS:
		DEBUGNAME("EV_DESTROY_WEBS");
		CG_DestroyWebs( cent );
		break;

	//NT - start the railgun prefire sound and stuff
	case EV_RAILGUN_PREFIRE:
		DEBUGNAME("EV_RAILGUN_PREFIRE");
		CG_RailgunPrefire( cent );
		break;

	//NT - start the trigger sound (0.5 seconds isn't much warning - it's more for
	// the psychological effect)
	case EV_PROX_GREN_TRIGGER:
		DEBUGNAME("EV_PROX_GREN_TRIGGER");
		trap_S_StartSound (NULL, es->number, CHAN_WEAPON, cgs.media.sfx_proxtrig );
		break;

	//NT - do a lightning splash (lightning bolt hit the water)
	case EV_LIGHTNING_SPLASH:
		DEBUGNAME("EV_LIGHTNING_SPLASH");
		CG_LightningSplash( position );
		break;

	//NT - proximity grenade tick
	case EV_PROXIMITY_TICK:
		DEBUGNAME("EV_PROXIMITY_TICK");
		trap_S_StartSound( position, ENTITYNUM_WORLD, CHAN_AUTO, cgs.media.sfx_proxtick );
		break;

	//NT - some entities can bounce on the pads now
	case EV_JUMP_PAD_ENTITY:
		DEBUGNAME("EV_JUMP_PAD_ENTITY");
//		CG_Printf( "EV_JUMP_PAD w/effect #%i\n", es->eventParm );
		{
			localEntity_t	*smoke;
			vec3_t			up = {0, 0, 1};


			smoke = CG_SmokePuff( cent->lerpOrigin, up, 
						  32, 
						  1, 1, 1, 0.33f,
						  1000, 
						  cg.time, 0,
						  LEF_PUFF_DONT_SCALE, 
						  cgs.media.smokePuffShader );
		}

		// boing sound at origin
		trap_S_StartSound ( cent->lerpOrigin, -1, CHAN_VOICE, cgs.media.jumpPadSound );
		break;

	//NT - hologram - player hears sound going in and out of stasis
	case EV_STASIS_SOUND:
		DEBUGNAME("EV_STASIS_SOUND");
		trap_S_StartSound (NULL, es->number, CHAN_AUTO, cgs.media.holoStartSound );
		break;

	default:
		DEBUGNAME("UNKNOWN");
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
		if ( cent->previousEvent ) {
			return;	// already fired
		}
		// if this is a player event set the entity number of the client entity number
		if ( cent->currentState.eFlags & EF_PLAYER_EVENT ) {
			cent->currentState.number = cent->currentState.otherEntityNum;
		}

		cent->previousEvent = 1;

		cent->currentState.event = cent->currentState.eType - ET_EVENTS;
	} else {
		// check for events riding with another entity
		if ( cent->currentState.event == cent->previousEvent ) {
			return;
		}
		cent->previousEvent = cent->currentState.event;
		if ( ( cent->currentState.event & ~EV_EVENT_BITS ) == 0 ) {
			return;
		}
	}

	// calculate the position at exactly the frame time
	BG_EvaluateTrajectory( &cent->currentState.pos, cg.snap->serverTime, cent->lerpOrigin );
	CG_SetEntitySoundPosition( cent );

	CG_EntityEvent( cent, cent->lerpOrigin );
}

