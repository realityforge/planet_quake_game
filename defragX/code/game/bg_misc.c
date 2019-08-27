// Copyright (C) 1999-2000 Id Software, Inc.
//
// bg_misc.c -- both games misc functions, all completely stateless

#include "q_shared.h"
#include "bg_public.h"

gitem_t	bg_itemlist[] =
{
	{ NULL,	NULL, { NULL, NULL,	0, 0} ,	NULL, NULL,	0, 0, 0, "", ""	},
	{ "item_armor_shard",  "sound/misc/ar1_pkup.wav",  { "models/powerups/armor/shard.md3", "models/powerups/armor/shard_sphere.md3",	0, 0} ,	"icons/iconr_shard", "Armor Shard",	5, IT_ARMOR, 0, "", "" },
	{ "item_armor_jacket", "sound/misc/ar2_pkup.wav",  { "models/powerups/armor/armor_grn.md3", 0, 0, 0}, "icons/iconr_green", "Light Armor", 25, IT_ARMOR, 0, "", "" },
	{ "item_armor_combat", "sound/misc/ar2_pkup.wav",  { "models/powerups/armor/armor_yel.md3", 0, 0, 0}, "icons/iconr_yellow", "Combat Armor", 50, IT_ARMOR, 0, "", "" },
	{ "item_armor_body",   "sound/misc/ar2_pkup.wav",  { "models/powerups/armor/armor_red.md3", 0, 0, 0}, "icons/iconr_red", "Heavy Armor", 100, IT_ARMOR,	0, "", "" },
	{ "item_health_small", "sound/items/s_health.wav", { "models/powerups/health/small_cross.md3", "models/powerups/health/small_sphere.md3", 0, 0 }, "icons/iconh_green", "5 Health", 5, IT_HEALTH, 0, "", "" },
	{ "item_health",       "sound/items/n_health.wav", { "models/powerups/health/medium_cross.md3", "models/powerups/health/medium_sphere.md3", 0, 0 }, "icons/iconh_yellow",	"25 Health", 25, IT_HEALTH,	0, "", "" },
	{ "item_health_large", "sound/items/l_health.wav", { "models/powerups/health/large_cross.md3", "models/powerups/health/large_sphere.md3", 0, 0 }, "icons/iconh_red", "50 Health", 50, IT_HEALTH, 0, "", "" },
	{ "item_health_mega",  "sound/items/m_health.wav", { "models/powerups/health/mega_cross.md3", "models/powerups/health/mega_sphere.md3", 0, 0 }, "icons/iconh_mega", "Mega Health", 100, IT_HEALTH, 0, "", ""	},
    { "item_key",          "sound/misc/am_pkup.wav",   { "models/powerups/holdable/silverkey.md3", 0, 0, 0}, "icons/key", "Key", 1, IT_KEY, 0, "", "" },

	{ "weapon_gauntlet", "sound/misc/w_pkup.wav", { "models/weapons2/gauntlet/gauntlet.md3", 0, 0, 0}, "icons/iconw_gauntlet", "Gauntlet", 0, IT_WEAPON, WP_GAUNTLET, "", "" },
	{ "weapon_machinegun", "sound/misc/w_pkup.wav", { "models/weapons2/machinegun/machinegun.md3", 0, 0, 0}, "icons/iconw_machinegun", "Machinegun", 40, IT_WEAPON,	WP_MACHINEGUN, "", "" },
	{ "weapon_shotgun",	"sound/misc/w_pkup.wav", { "models/weapons2/shotgun/shotgun.md3", 0, 0, 0},	"icons/iconw_shotgun", "Shotgun", 10, IT_WEAPON, WP_SHOTGUN, "", ""	},
	{ "weapon_grenadelauncher",	"sound/misc/w_pkup.wav", { "models/weapons2/grenadel/grenadel.md3",	0, 0, 0}, "icons/iconw_grenade", "Grenade Launcher", 10, IT_WEAPON,	WP_GRENADE_LAUNCHER, "", "sound/weapons/grenade/hgrenb1a.wav sound/weapons/grenade/hgrenb2a.wav" },
	{ "weapon_rocketlauncher", "sound/misc/w_pkup.wav", { "models/weapons2/rocketl/rocketl.md3", 0, 0, 0}, "icons/iconw_rocket", "Rocket Launcher", 10, IT_WEAPON, WP_ROCKET_LAUNCHER, "", "" },
	{ "weapon_lightning", "sound/misc/w_pkup.wav", { "models/weapons2/lightning/lightning.md3",	0, 0, 0}, "icons/iconw_lightning", "Lightning Gun",	100, IT_WEAPON,	WP_LIGHTNING, "", "" },
	{ "weapon_railgun",	"sound/misc/w_pkup.wav", { "models/weapons2/railgun/railgun.md3", 0, 0, 0},	"icons/iconw_railgun", "Railgun", 10, IT_WEAPON, WP_RAILGUN, "", "" },
	{ "weapon_plasmagun", "sound/misc/w_pkup.wav", { "models/weapons2/plasma/plasma.md3", 0, 0, 0},	"icons/iconw_plasma", "Plasma Gun",	50,	IT_WEAPON, WP_PLASMAGUN, "", ""	},
	{ "weapon_bfg", "sound/misc/w_pkup.wav", { "models/weapons2/bfg/bfg.md3", 0, 0, 0},	"icons/iconw_bfg", "BFG10K", 20, IT_WEAPON,	WP_BFG, "", "" },
	{ "weapon_grapplinghook", "sound/misc/w_pkup.wav", { "models/weapons2/grapple/grapple.md3",	0, 0, 0}, "icons/iconw_grapple", "Grappling Hook", 0, IT_WEAPON, WP_GRAPPLING_HOOK, "", "" },

	{ "ammo_shells", "sound/misc/am_pkup.wav", { "models/powerups/ammo/shotgunam.md3", 0, 0, 0}, "icons/icona_shotgun",	"Shells", 10, IT_AMMO, WP_SHOTGUN, "", "" },
	{ "ammo_bullets", "sound/misc/am_pkup.wav", { "models/powerups/ammo/machinegunam.md3", 0, 0, 0}, "icons/icona_machinegun", "Bullets", 50, IT_AMMO, WP_MACHINEGUN, "", "" },
	{ "ammo_grenades", "sound/misc/am_pkup.wav", { "models/powerups/ammo/grenadeam.md3", 0, 0, 0}, "icons/icona_grenade", "Grenades", 5, IT_AMMO, WP_GRENADE_LAUNCHER, "", "" },
	{ "ammo_cells",	"sound/misc/am_pkup.wav", { "models/powerups/ammo/plasmaam.md3",0 , 0, 0}, "icons/icona_plasma", "Cells", 30, IT_AMMO, WP_PLASMAGUN, "", ""	},
	{ "ammo_lightning", "sound/misc/am_pkup.wav", { "models/powerups/ammo/lightningam.md3",	0, 0, 0}, "icons/icona_lightning", "Lightning",	60,	IT_AMMO, WP_LIGHTNING, "", "" },
	{ "ammo_rockets", "sound/misc/am_pkup.wav", { "models/powerups/ammo/rocketam.md3", 0, 0, 0}, "icons/icona_rocket", "Rockets", 5, IT_AMMO, WP_ROCKET_LAUNCHER, "", "" },
	{ "ammo_slugs",	"sound/misc/am_pkup.wav", { "models/powerups/ammo/railgunam.md3", 0, 0, 0},	"icons/icona_railgun", "Slugs",	10,	IT_AMMO, WP_RAILGUN, "", ""	},
	{ "ammo_bfg", "sound/misc/am_pkup.wav", { "models/powerups/ammo/bfgam.md3",	0, 0, 0}, "icons/icona_bfg", "Bfg Ammo", 15, IT_AMMO, WP_BFG, "", "" },

	{ "holdable_medkit", "sound/items/holdable.wav", { "models/powerups/holdable/medkit.md3", "models/powerups/holdable/medkit_sphere.md3", 0, 0}, "icons/medkit", "Medkit", 60, IT_HOLDABLE, HI_MEDKIT, "", "sound/items/use_medkit.wav" },

	{ "item_quad", "sound/items/quaddamage.wav", { "models/powerups/instant/quad.md3", "models/powerups/instant/quad_ring.md3",	0, 0 },	"icons/quad", "Quad Damage", 30, IT_POWERUP, PW_QUAD, "", "sound/items/damage2.wav sound/items/damage3.wav"	},
	{ "item_enviro", "sound/items/protect.wav", { "models/powerups/instant/enviro.md3", "models/powerups/instant/enviro_ring.md3", 0, 0 }, "icons/envirosuit", "Battle Suit", 30, IT_POWERUP, PW_BATTLESUIT, "", "sound/items/airout.wav sound/items/protect3.wav" },
	{ "item_haste",	"sound/items/haste.wav", { "models/powerups/instant/haste.md3", "models/powerups/instant/haste_ring.md3", 0, 0 }, "icons/haste", "Haste", 30, IT_POWERUP, PW_HASTE, "", "" },
	{ "item_invis",	"sound/items/invisibility.wav", { "models/powerups/instant/invis.md3", "models/powerups/instant/invis_ring.md3", 0, 0 }, "icons/invis", "Invisibility",	30,	IT_POWERUP, PW_INVIS, "", "" },
	{ "item_regen",	"sound/items/regeneration.wav", { "models/powerups/instant/regen.md3", "models/powerups/instant/regen_ring.md3", 0, 0 }, "icons/regen",	"Regeneration",	30,	IT_POWERUP,	PW_REGEN, "", "sound/items/regen.wav" },
	{ "item_flight", "sound/items/flight.wav", { "models/powerups/instant/flight.md3", "models/powerups/instant/flight_ring.md3", 0, 0 }, "icons/flight", "Flight",	60,	IT_POWERUP,	PW_FLIGHT, "", "sound/items/flight.wav"	},

	{ "team_CTF_redflag",  NULL, { "models/flags/start_flag.md3", 0, 0, 0 }, "textures/common/invisible", "", 0, IT_TEAM, PW_BLUEFLAG, "", "" },
	{ "team_CTF_blueflag", NULL, { "models/flags/finish_flag.md3", 0, 0, 0 },"textures/common/invisible", "", 0, IT_TEAM, PW_REDFLAG,  "", "" },
	{ "start_flag",        NULL, { "models/flags/start_flag.md3", 0, 0, 0 }, "textures/common/invisible", "", 0, IT_TEAM, PW_BLUEFLAG, "", "" },
	{ "finish_flag",       NULL, { "models/flags/finish_flag.md3", 0, 0, 0 },"textures/common/invisible", "", 0, IT_TEAM, PW_REDFLAG,  "", "" },

	{NULL}
};

int	bg_numItems = ARRAY_LEN( bg_itemlist ) - 1;

#ifndef Q3UI
/*
==============
BG_FindItemForPowerup
==============
*/
gitem_t	*BG_FindItemForPowerup( powerup_t pw )
{
	int	i;

	for ( i = 0 ; i < bg_numItems ; i++ )
    {
		if ( (bg_itemlist[i].giType == IT_POWERUP || bg_itemlist[i].giType == IT_TEAM || bg_itemlist[i].giType == IT_PERSISTANT_POWERUP) && bg_itemlist[i].giTag == pw )
        {
            return &bg_itemlist[i];
        }
	}

	return NULL;
}


/*
==============
BG_FindItemForHoldable
==============
*/
gitem_t	*BG_FindItemForHoldable( holdable_t pw )
{
	int	i;

	for ( i = 0 ; i < bg_numItems ; i++ )
    {
		if ( bg_itemlist[i].giType == IT_HOLDABLE && bg_itemlist[i].giTag == pw ) return &bg_itemlist[i];
	}

	Com_Error( ERR_DROP, "HoldableItem not found" );

	return NULL;
}


/*
===============
BG_FindItemForWeapon
===============
*/
gitem_t	*BG_FindItemForWeapon( weapon_t weapon )
{
	gitem_t	*it;

	for ( it = bg_itemlist + 1 ; it->classname ; it++)
    {
		if ( it->giType == IT_WEAPON && it->giTag == weapon ) return it;
	}

	Com_Error( ERR_DROP, "Couldn't find item for weapon %i", weapon);
	return NULL;
}

/*
===============
BG_FindItem
===============
*/
gitem_t	*BG_FindItem( const char *pickupName )
{
	gitem_t	*it;

	for ( it = bg_itemlist + 1 ; it->classname ; it++ )
    {
		if (!Q_stricmp(it->pickup_name, pickupName)) return it;
	}

	return NULL;
}

/*
===============
BG_FindItemForClassname

===============
*/
gitem_t	*BG_FindItemForClassname( const char *classname )
{
	gitem_t	*it;

	for ( it = bg_itemlist + 1 ; it->classname ; it++ ) {
		if ( !Q_stricmp( it->classname, classname ) )
			return it;
	}

	return NULL;
}

/*
============
BG_PlayerTouchesItem

Items can be picked up without actually touching their physical bounds to make
grabbing them easier
============
*/
qbool BG_PlayerTouchesItem( playerState_t *ps, entityState_t *item, int atTime )
{
	vec3_t	origin;

	BG_EvaluateTrajectory( &item->pos, atTime, origin );

	// we are ignoring ducked differences here
	if ( ps->origin[0] - origin[0] > 44	|| ps->origin[0] - origin[0] < -50 || ps->origin[1] - origin[1] > 36 || ps->origin[1] - origin[1] < -36	|| ps->origin[2] - origin[2] > 36 || ps->origin[2] - origin[2] < -36 )
		return qfalse;

	return qtrue;
}


char *FormatDFTimer( int time, qbool sign )
{
	static char s[MAX_STRING_CHARS];
	char *prefix;
	int mins, tens, seconds;
    int msec = abs(time);

	seconds = msec / 1000;
	mins = seconds / 60;
	seconds -= mins * 60;
	tens = seconds / 10;
	seconds -= tens * 10;
	msec = msec % 1000;

    prefix = "";
	if ( sign )
    {
		if ( time > 0 )
		{
			prefix = "^2+";
		}
        else if ( time < 0 )
        {
			prefix = "^1-";
		}
        else
        {
			prefix = "^7+";
        }
	}

	if ( mins )
    {
		Com_sprintf( s, sizeof(s), "%s%d:%d%d:%03d", prefix, mins, tens, seconds, msec );
	}
	else if (tens)
    {
		Com_sprintf( s, sizeof(s), "%s%d%d:%03d", prefix, tens, seconds, msec );
	}
	else
    {
		Com_sprintf( s, sizeof(s), "%s%d:%03d", prefix, seconds, msec );
	}

	return s;
}


void SetItemAvailable( entityState_t *es, int clientNum, qbool available )
{
	// entity is not an item
	if ( es->eType != ET_ITEM )
    {
        Com_Printf( "SetItemAvailable: entity is not an item (eType: %d)\n", es->eType );
		return;
	}

	if ( clientNum < 0 || clientNum >= 64 )
    {
		Com_Printf( "SetItemAvailable: clientNum %d out of range\n", clientNum );
	}

	if ( clientNum < 32 )
    {
		if ( available )
		{
			es->time |= 1 << clientNum;
		} else {
			es->time &= ~(1 << clientNum);
		}
	}
	else if ( clientNum < 64 )
	{
		if ( available )
		{
			es->time2 |= 1 << (clientNum - 32);
		} else
		{
			es->time2 &= ~(1 << (clientNum - 32));
		}
	}
}

qbool IsItemAvailable( entityState_t *es, int clientNum )
{
	// entity is not an item
	if ( es->eType != ET_ITEM )
    {
        Com_Printf( "IsItemAvailable: entity is not an item (eType: %d)\n", es->eType );
		return qfalse;
	}

    if ( clientNum < 0 || clientNum >= 64 )
    {
		Com_Printf( "IsItemAvailable: clientNum %d out of range\n", clientNum );
        return qfalse;
    }

	if ( clientNum < 32 )
    {
		if ( es->time & (1 << clientNum) )
		{
			return qtrue;
		}
	}
	else if ( clientNum < 64 )
	{
		if ( es->time2 & (1 << (clientNum - 32)) )
		{
			return qtrue;
		}
	}

	return qfalse;
}

/*
================
BG_CanItemBeGrabbed

Returns false if the item should not be picked up.
This needs to be the same for client side prediction and server use.
================
*/
qbool BG_CanItemBeGrabbed( int gametype, const entityState_t *ent, const playerState_t *ps )
{
	gitem_t	*item;

	if ( ent->modelindex < 1 || ent->modelindex >= bg_numItems )
		Com_Error( ERR_DROP, "BG_CanItemBeGrabbed: index out of range" );

	item = &bg_itemlist[ent->modelindex];

	switch( item->giType )
	{
	case IT_WEAPON:
		return qtrue;	// weapons are always picked up

	case IT_AMMO:
		if ( ps->ammo[ item->giTag ] >= 200 ) {
			return qfalse;		// can't hold any more
		}
		return qtrue;

    case IT_KEY:
        if ( ps->persistant[PERS_KEYS] >= 9 ) {
            return qfalse;		// can't hold any more
		}
		return qtrue;

	case IT_ARMOR:

		if ( ps->stats[STAT_ARMOR] >= ps->stats[STAT_MAX_HEALTH] * 2 ) {
			return qfalse;
		}

		return qtrue;

	case IT_HEALTH:
		// small and mega healths will go over the max, otherwise
		// don't pick up if already at max

		if ( item->quantity == 5 || item->quantity == 100 ) {
			if ( ps->stats[STAT_HEALTH] >= ps->stats[STAT_MAX_HEALTH] * 2 ) {
				return qfalse;
			}
			return qtrue;
		}

		if ( ps->stats[STAT_HEALTH] >= ps->stats[STAT_MAX_HEALTH] ) {
			return qfalse;
		}
		return qtrue;

	case IT_POWERUP:
		return qtrue;	// powerups are always picked up

	case IT_TEAM: // team items, such as flags

		if( gametype == GT_FASTCAP )
			return qtrue;

		return qfalse;

	case IT_HOLDABLE:
		// can only hold one item at a time SLK: this should be improoved maybe
		if ( ps->stats[STAT_HOLDABLE_ITEM] ) {
			return qfalse;
		}
		return qtrue;

        case IT_BAD:
            Com_Error( ERR_DROP, "BG_CanItemBeGrabbed: IT_BAD" );
        default:
            break;
	}

	return qfalse;
}

//======================================================================

/*
================
BG_EvaluateTrajectory

================
*/
void BG_EvaluateTrajectory( const trajectory_t *tr, int atTime, vec3_t result )
{
	float	deltaTime;
	float	phase;

	switch( tr->trType )
	{
	case TR_STATIONARY:
	case TR_INTERPOLATE:
		VectorCopy( tr->trBase, result );
		break;
	case TR_LINEAR:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		break;
	case TR_SINE:
		deltaTime = ( ( atTime - tr->trTime ) % tr->trDuration ) / (float) tr->trDuration;
		phase = sin( deltaTime * M_PI * 2 );
		VectorMA( tr->trBase, phase, tr->trDelta, result );
		break;
	case TR_LINEAR_STOP:
		if ( atTime > tr->trTime + tr->trDuration )
        {
			atTime = tr->trTime + tr->trDuration;
		}
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		if ( deltaTime < 0 ) deltaTime = 0;
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		break;
	case TR_GRAVITY:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		result[2] -= 0.5 * DEFAULT_GRAVITY * deltaTime * deltaTime;		// FIXME: local gravity...
		break;
    case TR_ROTATING:
		if ( tr->trTime > 0 )
			deltaTime = tr->trTime * 0.001;	// milliseconds to seconds
		else if ( tr->trTime < 0 )
			deltaTime = ( atTime + tr->trTime ) * 0.001;
		else
			deltaTime = ( atTime - tr->trTime ) * 0.001;
		VectorMA( tr->trBase, deltaTime, tr->trDelta, result );
		break;
	default:
		Com_Error( ERR_DROP, "BG_EvaluateTrajectory: unknown trType: %i", tr->trType );
		break;
	}
}

/*
================
BG_EvaluateTrajectoryDelta

For determining velocity at a given time
================
*/
void BG_EvaluateTrajectoryDelta( const trajectory_t *tr, int atTime, vec3_t result )
{
	float	deltaTime;
	float	phase;

	switch( tr->trType ) {
	case TR_STATIONARY:
	case TR_INTERPOLATE:
		VectorClear( result );
		break;
    case TR_ROTATING:
	case TR_LINEAR:
		VectorCopy( tr->trDelta, result );
		break;
	case TR_SINE:
		deltaTime = ( atTime - tr->trTime ) / (float) tr->trDuration;
		phase = cos( deltaTime * M_PI * 2 );	// derivative of sin = cos
		phase *= 0.5;
		VectorScale( tr->trDelta, phase, result );
		break;
	case TR_LINEAR_STOP:
		if ( atTime > tr->trTime + tr->trDuration )
        {
			VectorClear( result );
			return;
		}
		VectorCopy( tr->trDelta, result );
		break;
	case TR_GRAVITY:
		deltaTime = ( atTime - tr->trTime ) * 0.001;	// milliseconds to seconds
		VectorCopy( tr->trDelta, result );
		result[2] -= DEFAULT_GRAVITY * deltaTime;		// FIXME: local gravity...
		break;
	default:
		Com_Error( ERR_DROP, "BG_EvaluateTrajectoryDelta: unknown trType: %i", tr->trType );
		break;
	}
}


const char *eventnames[EV_MAX] =
{
	"EV_NONE",

	"EV_FOOTSTEP",
	"EV_FOOTSTEP_METAL",
	"EV_FOOTSTEP_FLESH",
	"EV_FOOTSTEP_STONE",
	"EV_FOOTSTEP_GRASS",
	"EV_FOOTSTEP_TALLGRASS",
	"EV_FOOTSTEP_WOOD",
	"EV_FOOTSTEP_SAND",
	"EV_FOOTSTEP_ENERGY",
	"EV_FOOTSTEP_LAVA",
	"EV_FOOTSTEP_ICE",

	"EV_FOOTSPLASH",
	"EV_FOOTWADE",
	"EV_SWIM",

	"EV_STEP_4",
	"EV_STEP_8",
	"EV_STEP_12",
	"EV_STEP_16",

	"EV_FALL_SHORT",
	"EV_FALL_MEDIUM",
	"EV_FALL_FAR",

	"EV_JUMP_PAD",		        // boing sound at origin", jump sound on player

	"EV_JUMP",
	"EV_WATER_TOUCH",	        // foot touches
	"EV_WATER_LEAVE",	        // foot leaves
	"EV_WATER_UNDER",	        // head touches
	"EV_WATER_CLEAR",	        // head leaves

	"EV_ITEM_PICKUP",			// normal item pickups are predictable
	"EV_GLOBAL_ITEM_PICKUP",	// powerup / team sounds are broadcast to everyone

	"EV_NOAMMO",
	"EV_CHANGE_WEAPON",
	"EV_FIRE_WEAPON",

	"EV_USE_ITEM0",
	"EV_USE_ITEM1",
	"EV_USE_ITEM2",
	"EV_USE_ITEM3",
	"EV_USE_ITEM4",
	"EV_USE_ITEM5",
	"EV_USE_ITEM6",
	"EV_USE_ITEM7",
	"EV_USE_ITEM8",
	"EV_USE_ITEM9",
	"EV_USE_ITEM10",
	"EV_USE_ITEM11",
	"EV_USE_ITEM12",
	"EV_USE_ITEM13",
	"EV_USE_ITEM14",
	"EV_USE_ITEM15",

	"EV_ITEM_RESPAWN",
	"EV_ITEM_POP",
	"EV_PLAYER_TELEPORT_IN",
	"EV_PLAYER_TELEPORT_OUT",

	"EV_GRENADE_BOUNCE",		// eventParm will be the soundindex

	"EV_GENERAL_SOUND",
	"EV_GLOBAL_SOUND",		    // no attenuation
	"EV_GLOBAL_TEAM_SOUND",

	"EV_BULLET_HIT_FLESH",
	"EV_BULLET_HIT_WALL",

	"EV_MISSILE_HIT",
	"EV_MISSILE_MISS",
	"EV_MISSILE_MISS_METAL",
	"EV_RAILTRAIL",
	"EV_SHOTGUN",
	"EV_BULLET",				// otherEntity is the shooter

	"EV_PAIN",
	"EV_DEATH1",
	"EV_DEATH2",
	"EV_DEATH3",
	"EV_OBITUARY",

	"EV_POWERUP_QUAD",
	"EV_POWERUP_BATTLESUIT",
	"EV_POWERUP_REGEN",

	"EV_GIB_PLAYER",			// gib a previously living player

	"EV_DEBUG_LINE",
	"EV_STOPLOOPINGSOUND",

	"EV_TAUNT",
	"EV_TAUNT_YES",
	"EV_TAUNT_NO",
	"EV_TAUNT_FOLLOWME",
	"EV_TAUNT_GETFLAG",
	"EV_TAUNT_GUARDBASE",
	"EV_TAUNT_PATROL",

	"EV_BREAKABLE",
    "EV_EARTHQUAKE",
    "EV_EXPLOSION",
    "EV_PARTICLES_GRAVITY",
	"EV_PARTICLES_LINEAR",
    "EV_PARTICLES_LINEAR_UP",
	"EV_PARTICLES_LINEAR_DOWN",

	"EV_LANDSOUND",
	"EV_LANDSOUND_METAL",
	"EV_LANDSOUND_FLESH",
	"EV_LANDSOUND_STONE",
	"EV_LANDSOUND_GRASS",
	"EV_LANDSOUND_TALLGRASS",
	"EV_LANDSOUND_WOOD",
	"EV_LANDSOUND_SAND",
	"EV_LANDSOUND_LAVA",
	"EV_LANDSOUND_ICE"
};

/*
===============
BG_AddPredictableEventToPlayerstate

Handles the sequence numbers
===============
*/
#ifdef CGAME
void CG_StoreEvent( entity_event_t ev, int eventParm, int entityNum );
#endif

void trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize );

void BG_AddPredictableEventToPlayerstate( entity_event_t newEvent, int eventParm, playerState_t *ps, int entityNum )
{
#ifdef _DEBUG
	{
		char buf[256];
		trap_Cvar_VariableStringBuffer("showevents", buf, sizeof(buf));
		if ( atof(buf) != 0 )
        {
#ifdef QAGAME
			Com_Printf(" game event svt %5d -> %5d: num = %20s parm %d\n", ps->pmove_framecount/*ps->commandTime*/, ps->eventSequence, eventnames[newEvent], eventParm);
#else
			Com_Printf("Cgame event svt %5d -> %5d: num = %20s parm %d\n", ps->pmove_framecount/*ps->commandTime*/, ps->eventSequence, eventnames[newEvent], eventParm);
#endif
		}
	}
#endif

#ifdef CGAME
	CG_StoreEvent( newEvent, eventParm, entityNum );
#endif

	ps->events[ps->eventSequence & (MAX_PS_EVENTS-1)] = newEvent;
	ps->eventParms[ps->eventSequence & (MAX_PS_EVENTS-1)] = eventParm;
	ps->eventSequence++;
}


/*
========================
BG_TouchJumpPad
========================
*/

extern int pm_jumppad;

void BG_TouchJumpPad( playerState_t *ps, entityState_t *jumppad )
{
	vec3_t	angles;
	float   p;
	int     effectNum;

	if (pm_jumppad == 0)
        return;

	// spectators don't use jump pads | flying characters don't hit bounce pads
	if ( ps->pm_type != PM_NORMAL ||  ps->powerups[PW_FLIGHT] ) return;

	// if we didn't hit this same jumppad the previous frame
	// then don't play the event sound again if we are in a fat trigger
	if ( ps->jumppad_ent != jumppad->number )
    {

		vectoangles( jumppad->origin2, angles);
		p = fabs( AngleNormalize180( angles[PITCH] ) );
		if( p < 45 )
			effectNum = 0;
        else
			effectNum = 1;

		BG_AddPredictableEventToPlayerstate( EV_JUMP_PAD, effectNum, ps, -1 );
	}
	// remember hitting this jumppad this frame
	ps->jumppad_ent = jumppad->number;
	ps->jumppad_frame = ps->pmove_framecount;
	// give the player the velocity from the jumppad
	VectorCopy( jumppad->origin2, ps->velocity );
}

void BG_TouchVelocityJumpPad( playerState_t *ps, entityState_t *jumppad )
{
	vec3_t velocityXY, velocityZ, plus, minus;
	qbool clampNegative;
	float dot;
	int spawnflags;

	if ( pm_jumppad == 0 ) {
		return;
	}

	// spectators don't use jump pads unless they are specstrafing
	if ( ps->pm_type == PM_SPECTATOR ) {
		return;
	}

	if ( ps->pm_type != PM_NORMAL && ps->pm_type != PM_SPECTATOR ) {
		return;
	}

	// flying characters don't hit bounce pads
	if ( ps->powerups[PW_FLIGHT] ) {
		return;
	}

	// if we didn't hit this same jumppad the previous frame
	// then don't play the event sound again if we are in a fat trigger
	if ( ps->pm_type == PM_NORMAL && ps->jumppad_ent != jumppad->number ) {
		vec3_t	angles;
		float p;
		int effectNum;

		vectoangles( jumppad->origin2, angles);
		p = fabs( AngleNormalize180( angles[PITCH] ) );
		if( p < 45 ) {
			effectNum = 0;
		} else {
			effectNum = 1;
		}
		BG_AddPredictableEventToPlayerstate( EV_JUMP_PAD, effectNum, ps, -1 );
	}

	spawnflags = jumppad->weapon;      // weapon has a copy of the entity's spawnflags
	clampNegative = spawnflags & 0x40; // CLAMP_NEGATIVE_ADDS

	if ( spawnflags ) {

		if ( spawnflags & 1 ) { // PLAYERDIR_XY
			velocityXY[0] = ps->velocity[0];
			velocityXY[1] = ps->velocity[1];
			velocityXY[2] = 0;
			VectorNormalize( velocityXY );
			VectorScale( velocityXY, jumppad->angles2[0], velocityXY );
		} else {
			velocityXY[0] = jumppad->origin2[0];
			velocityXY[1] = jumppad->origin2[1];
			velocityXY[2] = 0;
			if ( spawnflags & 16 ) { // BIDIRECTIONAL_XY
				VectorCopy( ps->velocity, plus );
				VectorCopy( ps->velocity, minus );
				plus[0] += velocityXY[0];
				plus[1] += velocityXY[1];
				minus[0] -= velocityXY[0];
				minus[1] -= velocityXY[1];
				if ( VectorLength( minus ) > VectorLength( plus ) ) {
					VectorNegate( velocityXY, velocityXY );
				}
			}
		}

		if ( spawnflags & 4 ) { // PLAYERDIR_Z
			velocityZ[0] = 0;
			velocityZ[1] = 0;
			velocityZ[2] = ps->velocity[2];
			VectorNormalize( velocityZ );
			VectorScale( velocityZ, jumppad->angles2[2], velocityZ );
		} else {
			velocityZ[0] = 0;
			velocityZ[1] = 0;
			velocityZ[2] = jumppad->origin2[2];
			if ( spawnflags & 0x20 ) { // BIDIRECTIONAL_Z
				VectorCopy( ps->velocity, plus );
				VectorCopy( ps->velocity, minus );
				plus[2] += velocityZ[2];
				minus[2] -= velocityZ[2];
				if ( VectorLength( minus ) > VectorLength( plus ) ) {
					VectorNegate( velocityZ, velocityZ );
				}
			}
		}

		if ( spawnflags & 2 ) { // ADD_XY
			if ( ps->jumppad_ent != jumppad->number ) {
				plus[0] = ps->velocity[0] + velocityXY[0];
				plus[1] = ps->velocity[1] + velocityXY[1];
				plus[2] = 0;
				dot = DotProduct( plus, ps->velocity ) / (VectorLength( plus ) * VectorLength( ps->velocity ));
				if ( clampNegative && dot < 0 ) {
					ps->velocity[0] = 0;
					ps->velocity[1] = 0;
				} else {
					ps->velocity[0] = plus[0];
					ps->velocity[1] = plus[1];
				}
			}
		} else {
			ps->velocity[0] = velocityXY[0];
			ps->velocity[1] = velocityXY[1];
		}

		if ( spawnflags & 8 ) { // ADD_Z
			if ( ps->jumppad_ent != jumppad->number ) {
				plus[0] = 0;
				plus[1] = 0;
				plus[2] = ps->velocity[2] + velocityZ[2];
				dot = DotProduct( plus, ps->velocity ) / (VectorLength( plus ) * VectorLength( ps->velocity ));
				if ( clampNegative && dot < 0 ) {
					ps->velocity[2] = 0;
				} else {
					ps->velocity[2] = plus[2];
				}
			}
		} else {
			ps->velocity[2] = velocityZ[2];
		}

	} else {
		VectorCopy( jumppad->origin2, ps->velocity );
	}

	// remember hitting this jumppad this frame
	ps->jumppad_ent = jumppad->number;
	ps->jumppad_frame = ps->pmove_framecount;
}

/*
========================
BG_PlayerStateToEntityState

This is done after each set of usercmd_t on the server,
and after local prediction on the client
========================
*/
void BG_PlayerStateToEntityState( playerState_t *ps, entityState_t *s, qbool snap )
{
	int	i;

	if ( ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPECTATOR )
    {
		s->eType = ET_INVISIBLE;
	} else if ( ps->stats[STAT_HEALTH] <= GIB_HEALTH )
	{
		s->eType = ET_INVISIBLE;
	} else
	{
		s->eType = ET_PLAYER;
	}

	s->number = ps->clientNum;

	s->pos.trType = TR_INTERPOLATE;
	VectorCopy( ps->origin, s->pos.trBase );
	if ( snap ) SnapVector( s->pos.trBase );

	// set the trDelta for flag direction
	VectorCopy( ps->velocity, s->pos.trDelta );

	s->apos.trType = TR_INTERPOLATE;
	VectorCopy( ps->viewangles, s->apos.trBase );
	if ( snap ) SnapVector( s->apos.trBase );

	s->angles2[YAW] = ps->movementDir;
	s->legsAnim     = ps->legsAnim;
	s->torsoAnim    = ps->torsoAnim;
	s->clientNum    = ps->clientNum;	// ET_PLAYER looks here instead of at number
										// so corpses can also reference the proper config
	s->eFlags = ps->eFlags;
	if ( ps->stats[STAT_HEALTH] <= 0 )
		s->eFlags |= EF_DEAD;
	else
		s->eFlags &= ~EF_DEAD;

	if ( ps->externalEvent )
    {
		s->event = ps->externalEvent;
		s->eventParm = ps->externalEventParm;
	} else if ( ps->entityEventSequence < ps->eventSequence )
	{
		int	seq;

		if (ps->entityEventSequence < ps->eventSequence - MAX_PS_EVENTS)
        {
			ps->entityEventSequence = ps->eventSequence - MAX_PS_EVENTS;
		}
		seq = ps->entityEventSequence & (MAX_PS_EVENTS-1);
		s->event = ps->events[ seq ] | ( ( ps->entityEventSequence & 3 ) << 8 );
		s->eventParm = ps->eventParms[ seq ];
		ps->entityEventSequence++;
	}

	s->weapon = ps->weapon;
	s->groundEntityNum = ps->groundEntityNum;

	s->powerups = 0;
	for ( i = 0 ; i < MAX_POWERUPS ; i++ )
    {
		if (ps->powerups[ i ]) s->powerups |= 1 << i;
	}

	s->loopSound = ps->loopSound;
	s->generic1 = ps->generic1;
}

/*
========================
BG_PlayerStateToEntityStateExtraPolate

This is done after each set of usercmd_t on the server,
and after local prediction on the client
========================
*/
void BG_PlayerStateToEntityStateExtraPolate( playerState_t *ps, entityState_t *s, int time, qbool snap )
{
	int	i;

	if ( ps->pm_type == PM_INTERMISSION || ps->pm_type == PM_SPECTATOR )
    {
		s->eType = ET_INVISIBLE;
	} else if ( ps->stats[STAT_HEALTH] <= GIB_HEALTH )
	{
		s->eType = ET_INVISIBLE;
	} else
	{
		s->eType = ET_PLAYER;
	}

	s->number = ps->clientNum;

	s->pos.trType = TR_LINEAR_STOP;
	VectorCopy( ps->origin, s->pos.trBase );
	if ( snap )	SnapVector( s->pos.trBase );

	// set the trDelta for flag direction and linear prediction
	VectorCopy( ps->velocity, s->pos.trDelta );
	// set the time for linear prediction
	s->pos.trTime = time;
	// set maximum extra polation time
	s->pos.trDuration = 50; // 1000 / sv_fps (default = 20)

	s->apos.trType = TR_INTERPOLATE;
	VectorCopy( ps->viewangles, s->apos.trBase );
	if ( snap ) SnapVector( s->apos.trBase );

	s->angles2[YAW] = ps->movementDir;
	s->legsAnim     = ps->legsAnim;
	s->torsoAnim    = ps->torsoAnim;
	s->clientNum    = ps->clientNum;	// ET_PLAYER looks here instead of at number
										// so corpses can also reference the proper config
	s->eFlags = ps->eFlags;
	if ( ps->stats[STAT_HEALTH] <= 0 )
		s->eFlags |= EF_DEAD;
	else
		s->eFlags &= ~EF_DEAD;

	if ( ps->externalEvent )
    {
		s->event = ps->externalEvent;
		s->eventParm = ps->externalEventParm;
	} else if ( ps->entityEventSequence < ps->eventSequence )
	{
		int	seq;

		if ( ps->entityEventSequence < ps->eventSequence - MAX_PS_EVENTS)
			 ps->entityEventSequence = ps->eventSequence - MAX_PS_EVENTS;

		seq = ps->entityEventSequence & (MAX_PS_EVENTS-1);
		s->event = ps->events[seq] | ( ( ps->entityEventSequence & 3 ) << 8 );
		s->eventParm = ps->eventParms[seq];
		ps->entityEventSequence++;
	}

	s->weapon = ps->weapon;
	s->groundEntityNum = ps->groundEntityNum;

	s->powerups = 0;
	for ( i = 0 ; i < MAX_POWERUPS ; i++ )
    {
		if ( ps->powerups[ i ] ) s->powerups |= 1 << i;
	}

	s->loopSound = ps->loopSound;
	s->generic1 = ps->generic1;
}

qbool BG_CopyFile( const char *srcFilename, const char *dstFilename ) {
	static char buffer[32768];
	fileHandle_t src, dst;
	int len, total;

	total = trap_FS_FOpenFile( srcFilename, &src, FS_READ );
	if ( total <= 0 ) {
        Com_Printf("Can't open %s\n", srcFilename);
		return qfalse;
	}

	trap_FS_FOpenFile( dstFilename, &dst, FS_WRITE );

	while( total ) {
		len = MIN( sizeof(buffer), total );
		trap_FS_Read( buffer, len, src );
		trap_FS_Write( buffer, len, dst );
		total -= len;
	}

	trap_FS_FCloseFile( src );
	trap_FS_FCloseFile( dst );

	// make sure it was actually copied
	// if the file already existed this might be wrong
	if ( trap_FS_FOpenFile( dstFilename, &dst, FS_READ ) <= 0 ) {
        Com_Printf("Copy to %s failed\n", dstFilename);
		return qfalse;
	}

    trap_FS_FCloseFile( dst );

	return qtrue;
}

#endif // UI

int replace_s( char * str1, char * str2, char * src, int max_len )
{
	int         count = 0; // replace count
	int         len1, len2, d;
	char        *match, *s0, *s1, *s2, *max;

	match = strstr( src, str1 );

	if ( !match ) return count;

	len1 = (int)strlen( str1 );
	len2 = (int)strlen( str2 );
	d = len2-len1;

	if ( d > 0 ) // expand and replace mode
	{
		max = src + max_len;
		src += strlen( src );

		do
		{
			// expand source string
			s1 = src;
			src += d;
			if ( src >= max ) return count;
			s2 = src;
			s0 = match + len1;

			while ( s1 >= s0 )
            {
				*s2-- = *s1--;
			}

			// replace match
			s2 = str2;
			while ( *s2 )
            {
				*match = *s2; match++; s2++;
			}
			match = strstr ( match, str1 );
			count++;
		}
		while ( match );

		return count;
	}
	else if ( d < 0 ) // shrink and replace mode
	{
		do
		{
			// shrink source string
			s1 = match + len1;
			s2 = match + len2;
			while ( (*s2++ = *s1++) != 0 );

			//replace match
			s2 = str2;
			while ( *s2 )
            {
				*match = *s2;
				match++; s2++;
			}

			match = strstr ( match, str1 );
			count++;
		}
		while ( match );

		return count;
	}
	else
	do  // just replace match
	{
		s2 = str2;
		while ( *s2 )
        {
			*match = *s2;
			match++; s2++;
		}

		match = strstr ( match, str1 );
		count++;
	}
	while ( match );

	return count;
}


qbool replace1( const char match, const char replace, char *str )
{
	qbool	res = qfalse;

	if ( !str )	return res;

	while ( *str )
    {
		if ( *str == match )
		{
			*str = replace;
			res = qtrue;
		}
		str++;
	}

	return res;
}


char *strtok( char *strToken, const char *strDelimit )
{
	static char		*lastStr = NULL;

	const char		*delimit = strDelimit;
	int				loop = 1;

	// check some state
	if( !strToken )	strToken = lastStr; // use the last string given then

	// if there are no delimiters or no string to work with, bail
	if( !strToken )	return NULL;

	// first up we want to skip all delimiters at the start
	// and mark our place to return
	while( *strToken && loop )
	{
		loop = 0;

		// check all delimiters, we must find _one_ to continue the outside loop
		for( delimit = strDelimit ; *delimit ; delimit++ )
		{
			if( *strToken == *delimit )
			{
				strToken++;
				loop = 1;
				break;
			}
		}
	}

	// strToken now points to the first non-delimiter found
	// now we want to find the next delimiter to terminate on
	for( loop=1, lastStr=strToken ; *lastStr && loop ; lastStr++ )
	{
		// check all delimiters
		for( delimit=strDelimit ; *delimit ; delimit++ )
		{
			if( *delimit == *lastStr )
			{
				loop = 0;
				break;
			}
		}
	}

	// if the loop aborted then the token needs termination
	if( !loop )
	{
		*(lastStr-1) = '\0';

		if ( '\0' == *lastStr ) lastStr = NULL;
	}
	else
	{
		lastStr = NULL; // clean up for the next call
	}

	return strToken;
}


char *BG_StripColor( char *string )
{
	char	*d;
	char	*s;
	int		c;

	s = string;
	d = string;
	while ((c = *s) != 0 )
	{
		if ( Q_IsColorString( s ) )
			s++;
		else {
			*d = c; d++;
		}
		s++;
	}
	*d = '\0';
	return string;
}


char *EncodedString( const char *in )
{
	static const char   hextab[16] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };
	static char	        buf[16384];
	unsigned int        c;
	char		        *out;

	if ( !in )
    {
		buf[0] = '\0';
		return buf;
	}

	out = buf;

	while ( ( c = *in++ ) != '\0' )
    {
		if ( c == '#' )
		{
			*out++ = '#';
			*out++ = '#';
		}
        else if ( c > 127 || c == '%' )
        {
			*out++ = '#';
			*out++ = hextab [ (c & 0xF0) >> 4 ];
			*out++ = hextab [ (c & 0x0F) ];
		} else
		{
			*out++ = c;
		}
	}
	*out = '\0';
	return (char *)buf;
}


static int hex2dec ( char chr )
{
	switch ( chr ) {
		case '0':  return 0;
		case '1':  return 1;
		case '2':  return 2;
		case '3':  return 3;
		case '4':  return 4;
		case '5':  return 5;
		case '6':  return 6;
		case '7':  return 7;
		case '8':  return 8;
		case '9':  return 9;
		case 'a':  return 10;
		case 'A':  return 10;
		case 'b':  return 11;
		case 'B':  return 11;
		case 'c':  return 12;
		case 'C':  return 12;
		case 'd':  return 13;
		case 'D':  return 13;
		case 'e':  return 14;
		case 'E':  return 14;
		case 'f':  return 15;
		case 'F':  return 15;
	}
	return 0;
}

char *DecodedString( const char *in )
{
	static char	buf[16384];
	char		*out, c1, c2;

	if ( !in )
    {
		buf[0] = '\0';
		return (char *)buf;
	}

	out = buf;

	while ( *in )
    {
		// new encoding
		if ( *in == '#' && (c1 = *(in+1)) != 0 && (c2 = *(in+2)) != 0 &&
		    (( c1 >= '0' && c1 <= '9' ) || ( c1 >= 'a' && c1 <= 'f' )) &&
		    (( c2 >= '0' && c2 <= '9' ) || ( c2 >= 'a' && c2 <= 'f' )))
        {
			*out++ = hex2dec( c1 ) * 16 + hex2dec( c2 );
			in += 3;
		} else if ( *in == '#' && *(in+1) == '#' )
		{
			*out++ = '#';
			in += 2;
		} else
		{
			*out++ = *in++;
		}
	}

	*out = '\0';

	return (char *)buf;
}


// a bit faster string compare function
int BG_stricmp( const char *s1, const char *s2 )
{
	int	c1, c2;
	do
    {
		c1 = locase[(unsigned char)*s1]; s1++;
		c2 = locase[(unsigned char)*s2]; s2++;
		if (c1 != c2) return ((c1 < c2) ? -1 : 1);
	}  while ( c1 );
	return 0;		// strings are equal
}


char *Q_stristr( const char * str1, const char * str2 )
{
	char *cp = (char *) str1;
	char *s1, *s2;

	if ( !*str2 ) return( (char *)str1 );

	while ( *cp )
	{
		s1 = cp;
		s2 = (char *) str2;

		while ( *s1 && locase[(unsigned char)*s1] == locase[(unsigned char)*s2] )
		{
			s1++;
			s2++;
		}

		if ( !*s2 )	return( cp );

		cp++;
	}

	return( NULL );
}


/*
===========
BG_CleanName
============
*/
void BG_CleanName( const char *in, char *out, int outSize, const char *blankString )
{
	int		len, colorlessLen;
	char	ch;
	char	*p;
	int		spaces;

	//save room for trailing null byte
	outSize--;

	len = 0;
	colorlessLen = 0;
	p = out;
	*p = '\0';
	spaces = 0;

	while( 1 )
    {
		ch = *in++;
		if( !ch ) break;

		// don't allow leading spaces
		if( *p == '\0' && ch <= ' ' ) continue;

		// check colors
		if( ch == Q_COLOR_ESCAPE ) {
			// solo trailing carat is not a color prefix
			if( !*in ) break;

			// don't allow black in a name, period
			if( ColorIndex(*in) == 0 )
            {
				in++;
				continue;
			}

			// make sure room in dest for both chars
			if( len > outSize - 2 ) break;

			*out++ = ch;
			*out++ = *in++;
			len += 2;
			continue;
		}

		// let's keep it in printable range
		if ( ch < ' ' || ch > 126 ) continue;

		// don't allow too many consecutive spaces
		if( ch == ' ' )
        {
			spaces++;
			if( spaces > 2 ) continue;
		}
		else
		{
			spaces = 0;
		}

		if( len > outSize - 1 ) break;

		*out++ = ch;
		colorlessLen++;
		len++;
	}
	*out = '\0';

	if ( blankString )
    {
		// don't allow empty names
		if( *p == '\0' || colorlessLen == 0 )
			Q_strncpyz( p, blankString, outSize );
	}
}


/*
===================
Q_strcpy

string copy, without any checks
===================
*/
void Q_strcpy( char *dst, const char *src )
{
	char c;
	while ( (c = *src) != '\0' )
	{
		*dst = c; dst++; src++;
	}
	*dst = '\0';
}


char *Q_stradd( char *dst, const char *src )
{
	char c;
	while ( (c = *src) != '\0' )
	{
		*dst = c;
		dst++;
		src++;
	}
	*dst = '\0';
	return dst;
}

int Q_strlen( const char *s )
{
	const char *b = s;
	while ( *s != '\0' )
		s++;
	return s - b;
}


qbool BigEndian( void )
{
	const char *s = { "123" };
	int  *i;
	i = (void*)s;
	if ( *i != 0x00333231 )
		return qtrue;
	else
		return qfalse;
}


//=========================================================

#define ALT			0x00000001		/* alternate form */
#define HEXPREFIX	0x00000002		/* add 0x or 0X prefix */
#define LADJUST		0x00000004		/* left adjustment */
#define LONGDBL		0x00000008		/* long double */
#define LONGINT		0x00000010		/* long integer */
#define QUADINT		0x00000020		/* quad integer */
#define SHORTINT	0x00000040		/* short integer */
#define ZEROPAD		0x00000080		/* zero (as opposed to blank) pad */
#define FPT			0x00000100		/* floating point number */
#define REDUCE		0x00000200		/* extension: do not emit anything if value is zero */

#define to_digit(c)		((c) - '0')
#define is_digit(c)		((unsigned)to_digit(c) <= 9)
#define to_char(n)		((n) + '0')

static void AddInt( char **buf_p, int val, int width, int flags )
{
	char	text[32];
	int		digits;
	int		signedVal;
	char	*buf;

	if ( flags & REDUCE && val == 0 ) return;

	digits = 0;
	signedVal = val;
	if ( val < 0 ) val = -val;

	do
    {
		text[digits] = '0' + val % 10;
		digits++;
		val /= 10;
	} while ( val );

	if ( signedVal < 0 )
    {
		text[digits] = '-';
		digits++;
	}

	buf = *buf_p;

	if( !( flags & LADJUST ) )
    {
		while ( digits < width )
		{
			*buf = ( flags & ZEROPAD ) ? '0' : ' ';
			buf++;
			width--;
		}
	}

	while ( digits-- > 0 )
    {
		*buf = text[digits]; buf++;
		width--;
	}

	if( flags & LADJUST )
    {
		while ( width > 0 )
		{
			*buf = ( flags & ZEROPAD ) ? '0' : ' ';
			buf++;
			width--;
		}
	}

	*buf_p = buf;
}

static void AddFloat( char **buf_p, float fval, int width, int prec, int reduce )
{
	char	text[32];
	int		digits;
	float	signedVal;
	char	*buf;
	int		val;

	if ( reduce && fval == 0.0f ) return;

	// get the sign
	signedVal = fval;
	if ( fval < 0 ) fval = -fval;

	// write the float number
	digits = 0;
	val = (int)fval;
	do
    {
		text[digits] = '0' + val % 10;
		digits++;
		val /= 10;
	} while ( val );

	if ( signedVal < 0 )
    {
		text[digits] = '-';
		digits++;
	}

	buf = *buf_p;

	// fix precisiion
	if ( prec < 0 ) prec = 6;
	if ( prec ) width -= prec + 1;
	// end

	while ( digits < width )
    {
		*buf = ' ';	buf++;
		width--;
	}

	while ( digits-- > 0 )
    {
		*buf = text[digits]; buf++;
	}

	*buf_p = buf;

	// write the fraction
	digits = 0;
	while (digits < prec)
    {
		fval -= (int) fval;
		fval *= 10.0;
		val = (int) fval;
		text[digits] = '0' + val % 10;
		digits++;
	}

	if (digits > 0)
    {
		buf = *buf_p;
		*buf = '.';	buf++;
		for (prec = 0; prec < digits; prec++)
		{
			*buf = text[prec]; buf++;
		}
		*buf_p = buf;
	}
}


static void AddString( char **buf_p, const char *string, int width, int prec )
{
	int		size;
	char	*buf;

	buf = *buf_p;

	if ( string == NULL )
    {
		string = "(null)";
		prec = -1;
	}

	if ( prec >= 0 )
    {
		for( size = 0; size < prec; size++ )
		{
			if( string[size] == '\0' ) break;
		}
	}
	else
	{
		size = Q_strlen( string );
	}

	width -= size;

	while( size-- )
    {
		*buf = *string;
		buf++; string++;
	}

	while( width-- > 0 )
    {
		*buf = ' '; buf++;
	}

	*buf_p = buf;
}


/*
ED_vsprintf

I'm not going to support a bunch of the more arcane stuff in here
just to keep it simpler.  For example, the '$' is not
currently supported.  I've tried to make it so that it will just
parse and ignore formats we don't support.

returns: number of char written without ending '\0'
*/
int ED_vsprintf( char *buffer, const char *fmt, va_list ap )
{
	char	*buf_p;
	char	ch;
	int		flags;
	int		width;
	int		prec;
	int		n;

	buf_p = buffer;

	while( qtrue )
    {
		// run through the format string until we hit a '%' or '\0'
		for (; (ch = *fmt) != '\0' && ch != '%'; fmt++ )
		{
			*buf_p = ch; buf_p++;
		}
		if ( ch == '\0' ) break;

		// skip over the '%'
		fmt++;

		// reset formatting state
		flags = 0;
		width = 0;
		prec = -1;
rflag:
		ch = *fmt; fmt++;
reswitch:
		switch( ch )
		{
		case '-':
			flags |= LADJUST;
			goto rflag;
		case '.':
			if ( *fmt == '*' ) {
				fmt++;
				n = va_arg( ap, int );
				prec = n < 0 ? -1 : n;
				goto rflag;
			} else {
				n = 0;
				while( is_digit( ( ch = *fmt++ ) ) ) {
					n = 10 * n + ( ch - '0' );
				}
				prec = n < 0 ? -1 : n;
				goto reswitch;
			}
		case '0':
			flags |= ZEROPAD;
			goto rflag;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			n = 0;
			do
            {
				n = 10 * n + ( ch - '0' );
				ch = *fmt; fmt++;
			} while( is_digit( ch ) );
			width = n;
			goto reswitch;
		case '*':
			width = va_arg( ap, int );
			goto rflag;
		case 'c':
			*buf_p = va_arg( ap, char ); buf_p++;
			break;
		case 'd':
		case 'i':
			AddInt( &buf_p, va_arg( ap, int ), width, flags );
			break;
		case 'f':
			AddFloat( &buf_p, va_arg( ap, double ), width, prec, flags & REDUCE );
			break;
		case 's':
			AddString( &buf_p, va_arg( ap, char * ), width, prec );
			break;
		case '%':
			*buf_p = ch; buf_p++;
			break;
		 // edawn extension:
		case 'R':
			flags |= REDUCE;
			goto rflag;
		default:
			*buf_p = va_arg( ap, char ); buf_p++;
			break;
		}
	}

	*buf_p = '\0';
	return buf_p - buffer;
}


int BG_sprintf( char *buf, const char *format, ... )
{
	int len;
	va_list	argptr;
	va_start( argptr, format );
	len = ED_vsprintf( buf, format, argptr );
	va_end( argptr );
	return len;
}


static int _atoi( const char **stringPtr )
{
	int		    sign;
	int		    value;
	int		    c;
	const char	*string;

	string = *stringPtr;

	if ( !*string )	return 0;

	// check sign
	switch ( *string )
	{
	case '+':
		string++;
		sign = 1;
		break;
	case '-':
		string++;
		sign = -1;
		break;
	default:
		sign = 1;
		break;
	}

	// read digits
	value = 0;
	do
	{
		c = *string;
		if ( c < '0' || c > '9' ) break;
		c -= '0';
		value = value * 10 + c;
		string++;
	}
	while ( 1 );

	// not handling 10e10 notation...

	*stringPtr = string;

	return value * sign;
}


static float _atof( const char **stringPtr )
{
	const char	*string;
	float       sign;
	float       value;
	float       fraction;
	int	        c = '0'; // uninitialized use possible

	string = *stringPtr;

	if ( !*string )	return 0;

	// check sign
	switch ( *string )
	{
	case '+':
		string++;
		sign = 1;
		break;
	case '-':
		string++;
		sign = -1;
		break;
	default:
		sign = 1;
		break;
	}

	// read digits
	value = 0;
	if ( *string != '.' )
	{
		do
		{
			c = *string;
			if ( c < '0' || c > '9' )
			{
				break;
			}
			c -= '0';
			value = value * 10 + c;
			string++;
		}
		while ( 1 );
	}

	// check for decimal point
	if ( *string == '.' )
	{
		fraction = 0.1f;
		string++;
		do
		{
			c = *string;
			if ( c < '0' || c > '9' ) break;
			c -= '0';
			value += c * fraction;
			fraction *= 0.1f;
			string++;
		}
		while ( 1 );
	}

	// not handling 10e10 notation...
	*stringPtr = string;

	return value * sign;
}


static void _atos( const char **stringPtr, char *buffer, int delimiter, int width )
{
	const char	*string;

	string = *stringPtr;

	if ( !delimiter )
	{
		// skip whitespace
		while ( *string && *string != ' ' && *string != '\t' && width-- > 0 )
		{
			*buffer = *string;
			buffer++;
			string++;
		}
	}
	else while ( *string && *string != delimiter && width-- > 0 )
	{
		*buffer = *string;
		buffer++;
		string++;
	}

	*stringPtr = string;

	*buffer = '\0';
}


int Q_sscanf( const char *buffer, const char *fmt, ... )
{
	va_list ap;
	int count;
	int width;
	int cmd;
	const char *p;

	va_start( ap, fmt );
	count = 0;

	while ( *fmt )
	{
		// single whitespace char validates any quantity of whitespace characters
		// extracted from the stream (including none)
		if ( *fmt == ' ' || *fmt == '\t' || *fmt == '\n' )
		{
			while ( *buffer == ' ' || *buffer == '\t' || *buffer == '\n' )
				buffer++;
			fmt++;
		}

		if ( *fmt != '%' )
		{
			if ( *fmt != *buffer )
				break;

			buffer++;
			fmt++;
			continue;
		}

		width = fmt[1];
		fmt++; // %
		if ( width >= '0' && width <= '9' )
		{
			width -= '0'; // valid width;
			fmt++;	// ['0'..'9']
			cmd = *fmt;
		}
		else
		{
			cmd = width;
			width = 1024; // some assumption
		}

		p = buffer;

		fmt++; // switch to delimiter?

		switch ( cmd )
		{
		case 'i':
		case 'd':
		case 'u':
			*(va_arg(ap, int *)) = _atoi( &buffer );
			break;
		case 'f':
			*(va_arg(ap, float *)) = _atof( &buffer );
			break;
		case 'c':
			*(va_arg(ap, char *)) = *buffer; buffer++;
			break;
		case 's':
			_atos( &buffer, va_arg(ap, char *), *fmt, width );
			break;
		default:
			return count;
		}

		if ( p != buffer )
			count++;
		else
			break;
	}
	va_end( ap );

	return count;
}
