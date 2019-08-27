// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_main.c -- initialization and primary entry point for cgame
#include "cg_local.h"
#include "bg_shared.h"

int forceModelModificationCount = -1;
int enemyModelModificationCount  = -1;
int	enemyColorsModificationCount = -1;
int teamModelModificationCount  = -1;
int	teamColorsModificationCount = -1;

void CG_Init( int serverMessageNum, int serverCommandSequence, int clientNum );
void CG_Shutdown( void );

// extension interface
qbool intShaderTime = qfalse;
qbool linearLight = qfalse;

qbool (*trap_GetValue)( char *value, int valueSize, const char *key );
void  (*trap_R_AddRefEntityToScene2)( const refEntity_t *re );
void  (*trap_R_AddLinearLightToScene)( const vec3_t start, const vec3_t end, float intensity, float r, float g, float b );


/*
================
vmMain

This is the only way control passes into the module.
This must be the very first function compiled into the .q3vm file
================
*/
DLLEXPORT intptr_t vmMain( int command, int arg0, int arg1, int arg2 )
{

	switch ( command ) {
	case CG_INIT:
		CG_Init( arg0, arg1, arg2 );
		return 0;
	case CG_SHUTDOWN:
		CG_Shutdown();
		return 0;
	case CG_CONSOLE_COMMAND:
		return CG_ConsoleCommand();
	case CG_DRAW_ACTIVE_FRAME:
		CG_DrawActiveFrame( arg0, arg1, arg2 );
		return 0;
	case CG_CROSSHAIR_PLAYER:
		return CG_CrosshairPlayer();
	case CG_LAST_ATTACKER:
		return CG_LastAttacker();
	case CG_KEY_EVENT:
		CG_KeyEvent(arg0, arg1);
		return 0;
	case CG_MOUSE_EVENT:
		CG_MouseEvent(arg0, arg1);
		return 0;
	case CG_EVENT_HANDLING:
		return 0;
	default:
		CG_Error( "vmMain: unknown command %i", command );
		break;
	}
	return -1;
}


cg_t			cg;
cgs_t			cgs;
centity_t		cg_entities[MAX_GENTITIES];
weaponInfo_t	cg_weapons[MAX_WEAPONS];
itemInfo_t		cg_items[MAX_ITEMS];

vmCvar_t	cg_railTrailTime;
vmCvar_t	cg_railTrailRadius;
vmCvar_t	cg_centertime;
vmCvar_t	cg_runpitch;
vmCvar_t	cg_runroll;
vmCvar_t	cg_bobup;
vmCvar_t	cg_bobpitch;
vmCvar_t	cg_bobroll;
vmCvar_t	cg_swingSpeed;
vmCvar_t	cg_shadows;
vmCvar_t	cg_gibs;
vmCvar_t	cg_drawCrosshair;
vmCvar_t	cg_drawCrosshairNames;
vmCvar_t	cg_drawWeaponSelect;
vmCvar_t	cg_crosshairSize;
vmCvar_t	cg_crosshairHealth;
vmCvar_t    cg_crosshairColor;
vmCvar_t	cg_draw2D;
vmCvar_t	cg_errorDecay;
vmCvar_t	cg_nopredict;
vmCvar_t	cg_showmiss;
vmCvar_t	cg_footsteps;
vmCvar_t	cg_addMarks;
vmCvar_t	cg_brassTime;
vmCvar_t	cg_drawGun;
vmCvar_t	cg_gun_frame;
vmCvar_t	cg_gun_x;
vmCvar_t	cg_gun_y;
vmCvar_t	cg_gun_z;
vmCvar_t	cg_tracerChance;
vmCvar_t	cg_tracerWidth;
vmCvar_t	cg_tracerLength;
vmCvar_t	cg_autoswitch;
vmCvar_t	cg_simpleItems;
vmCvar_t	cg_fov;
vmCvar_t	cg_zoomFov;
vmCvar_t	cg_thirdPerson;
vmCvar_t	cg_thirdPersonRange;
vmCvar_t	cg_thirdPersonAngle;
vmCvar_t	cg_lagometer;
vmCvar_t	rate;
vmCvar_t	snaps;
#ifdef DEVBUILD
vmCvar_t	cg_drawSnapshot;
vmCvar_t 	cg_stats;
//vmCvar_t	cg_ignore;
vmCvar_t    cg_psf;
vmCvar_t	cg_debugAutorecord;
vmCvar_t	cg_debugAnim;
vmCvar_t	cg_debugEvents;
#endif // DEVBUILD
vmCvar_t 	cg_buildScript;
vmCvar_t 	cg_forceModel;
vmCvar_t	cg_paused;
vmCvar_t	cg_blood;
vmCvar_t	cg_predictItems;
vmCvar_t	cg_deferPlayers;
vmCvar_t 	cg_smoothClients;
vmCvar_t	cg_timescaleFadeEnd;
vmCvar_t	cg_timescaleFadeSpeed;
vmCvar_t	cg_timescale;
vmCvar_t	cg_smallFont;
vmCvar_t	cg_bigFont;
vmCvar_t	cg_noTaunt;
vmCvar_t	cg_oldRail;
vmCvar_t	cg_oldRocket;
vmCvar_t	cg_oldPlasma;
vmCvar_t	cg_trueLightning;
vmCvar_t	cg_enemyModel;
vmCvar_t	cg_enemyColors;
vmCvar_t	cg_teamModel;
vmCvar_t	cg_teamColors;
vmCvar_t	cg_fovAdjust;
vmCvar_t	cg_noChatBeep;
vmCvar_t    cg_showSpawns;
vmCvar_t	cg_autorecord;
vmCvar_t    cg_drawPickup;
vmCvar_t    cg_teleportEffect;
vmCvar_t    cg_gibEffect;
vmCvar_t    cg_landingSound;
vmCvar_t    cg_painSounds;
vmCvar_t    cg_alwaysDrawItems;

vmCvar_t    hud_fontSize;
vmCvar_t    hud_fontColor;
vmCvar_t    hud_frameColor;

vmCvar_t    hud_useHoldable;
vmCvar_t    hud_timer;
vmCvar_t    hud_timerX;
vmCvar_t    hud_timerY;
vmCvar_t    hud_cgaz;
vmCvar_t    hud_cgazAlpha;
vmCvar_t    hud_cgazY;
vmCvar_t    hud_bbox;
vmCvar_t    hud_bboxFPS;
vmCvar_t    hud_bboxColor;
vmCvar_t    hud_bboxMinsZ;
vmCvar_t    hud_snap;
vmCvar_t    hud_snapY;
vmCvar_t    hud_snapRGBA;

vmCvar_t    chs_fontSize;
vmCvar_t    chs_fontColor;
vmCvar_t    chs_fontHighlightColor;
vmCvar_t    chs_keypress;
vmCvar_t    chs_ob;
vmCvar_t    chs_obX;
vmCvar_t    chs_obY;
vmCvar_t    chs_speed;
vmCvar_t    chs_speedX;
vmCvar_t    chs_speedY;
vmCvar_t    chs_angles;
vmCvar_t    chs_anglesX;
vmCvar_t    chs_anglesY;
vmCvar_t    chs_levelTimer;
vmCvar_t    chs_levelTimerX;
vmCvar_t    chs_levelTimerY;
vmCvar_t    chs_fps;
vmCvar_t    chs_fpsX;
vmCvar_t    chs_fpsY;
vmCvar_t    chs_ping;
vmCvar_t    chs_pingX;
vmCvar_t    chs_pingY;

vmCvar_t	cp_centerDraw;

typedef struct {
	vmCvar_t	*vmCvar;
	const char	*cvarName;
	const char	*defaultString;
	const int	cvarFlags;
} cvarTable_t;

static const cvarTable_t cvarTable[] = {
	{ &cg_autoswitch, "cg_autoswitch", "1", CVAR_ARCHIVE },
	{ &cg_drawGun, "cg_drawGun", "1", CVAR_ARCHIVE },
	{ &cg_zoomFov, "cg_zoomfov", "30", CVAR_ARCHIVE },
	{ &cg_fov, "cg_fov", "120", CVAR_ARCHIVE },
	{ &cg_shadows, "cg_shadows", "1", CVAR_ARCHIVE },
	{ &cg_gibs, "cg_gibs", "1", CVAR_ARCHIVE },
	{ &cg_draw2D, "cg_draw2D", "1", CVAR_ARCHIVE },
	
	{ &cg_drawCrosshair, "cg_drawCrosshair", "10", CVAR_ARCHIVE },
	{ &cg_drawCrosshairNames, "cg_drawCrosshairNames", "1", CVAR_ARCHIVE },
	{ &cg_drawWeaponSelect, "cg_drawWeaponSelect", "3", CVAR_ARCHIVE },
	{ &cg_crosshairSize, "cg_crosshairSize", "24", CVAR_ARCHIVE },
	{ &cg_crosshairHealth, "cg_crosshairHealth", "0", CVAR_ARCHIVE },
	{ &cg_crosshairColor, "cg_crosshairColor", "7", CVAR_ARCHIVE },
	{ &cg_brassTime, "cg_brassTime", "2500", CVAR_ARCHIVE },
	{ &cg_simpleItems, "cg_simpleItems", "0", CVAR_ARCHIVE },
	{ &cg_addMarks, "cg_marks", "1", CVAR_ARCHIVE },
	{ &cg_lagometer, "cg_lagometer", "0", CVAR_ARCHIVE },
	{ &rate, "rate", "25000", CVAR_USERINFO | CVAR_ARCHIVE },
	{ &snaps, "snaps", "125", CVAR_USERINFO | CVAR_ARCHIVE },
	{ &cg_railTrailTime, "cg_railTrailTime", "400", CVAR_ARCHIVE  },
	{ &cg_railTrailRadius, "cg_railTrailRadius", "75", CVAR_ARCHIVE  },
	{ &cg_gun_x, "cg_gunX", "0", CVAR_ARCHIVE },
	{ &cg_gun_y, "cg_gunY", "0", CVAR_ARCHIVE },
	{ &cg_gun_z, "cg_gunZ", "0", CVAR_ARCHIVE },
	{ &cg_centertime, "cg_centertime", "3", CVAR_CHEAT },
	{ &cg_runpitch, "cg_runpitch", "0.002", CVAR_ARCHIVE},
	{ &cg_runroll, "cg_runroll", "0.005", CVAR_ARCHIVE },
	{ &cg_bobup , "cg_bobup", "0.005", CVAR_ARCHIVE },
	{ &cg_bobpitch, "cg_bobpitch", "0.002", CVAR_ARCHIVE },
	{ &cg_bobroll, "cg_bobroll", "0.002", CVAR_ARCHIVE },
	{ &cg_swingSpeed, "cg_swingSpeed", "0.3", CVAR_CHEAT },
	
	{ &cg_errorDecay, "cg_errordecay", "100", 0 },
	{ &cg_nopredict, "cg_nopredict", "0", 0 },
	{ &cg_showmiss, "cg_showmiss", "0", 0 },
	{ &cg_tracerChance, "cg_tracerchance", "0.4", CVAR_CHEAT },
	{ &cg_tracerWidth, "cg_tracerwidth", "1", CVAR_CHEAT },
	{ &cg_tracerLength, "cg_tracerlength", "100", CVAR_CHEAT },
	{ &cg_thirdPersonRange, "cg_thirdPersonRange", "40", CVAR_CHEAT },
	{ &cg_thirdPersonAngle, "cg_thirdPersonAngle", "0", CVAR_CHEAT },
	{ &cg_thirdPerson, "cg_thirdPerson", "0", CVAR_ARCHIVE },
	{ &cg_forceModel, "cg_forceModel", "0", CVAR_ARCHIVE  },
	{ &cg_predictItems, "cg_predictItems", "1", CVAR_ARCHIVE },
	{ &cg_deferPlayers, "cg_deferPlayers", "1", CVAR_ARCHIVE },
#ifdef DEVBUILD
	{ &cg_stats, "cg_stats", "0", 0 },
    { &cg_psf, "cg_PSF", "0", 0 },
	{ &cg_debugAnim, "cg_debuganim", "0", 0 },
	{ &cg_debugEvents, "cg_debugevents", "0", 0 },
    { &cg_debugAutorecord, "cg_debugAutorecord", "0", 0},
	{ &cg_drawSnapshot, "cg_drawSnapshot", "0", 0 },
	//{ &cg_ignore, "cg_ignore", "0", 0 }, // used for debugging
#endif


	// the following variables are created in other parts of the system,
	// but we also reference them here
	{ &cg_buildScript, "com_buildScript", "0", 0 },	// force loading of all possible data and error on failures
	{ &cg_paused, "cl_paused", "0", CVAR_ROM },
	{ &cg_blood, "com_blood", "1", CVAR_ARCHIVE },
	{ &cg_timescaleFadeEnd, "cg_timescaleFadeEnd", "1", 0},
	{ &cg_timescaleFadeSpeed, "cg_timescaleFadeSpeed", "0", 0},
	{ &cg_timescale, "timescale", "1", 0},
	{ &cg_smoothClients, "cg_smoothClients", "0", CVAR_USERINFO | CVAR_ARCHIVE},
	{ &cg_noTaunt, "cg_noTaunt", "0", CVAR_ARCHIVE},
	{ &cg_smallFont, "ui_smallFont", "0.25", CVAR_ARCHIVE},
	{ &cg_bigFont, "ui_bigFont", "0.4", CVAR_ARCHIVE},
	{ &cg_oldRail, "cg_oldRail", "1", CVAR_ARCHIVE},
	{ &cg_oldRocket, "cg_oldRocket", "1", CVAR_ARCHIVE},
	{ &cg_oldPlasma, "cg_oldPlasma", "1", CVAR_ARCHIVE},
	{ &cg_trueLightning, "cg_trueLightning", "0.0", CVAR_ARCHIVE},
	{ &cg_enemyModel, "cg_enemyModel", "", CVAR_ARCHIVE},
	{ &cg_enemyColors, "cg_enemyColors", "", CVAR_ARCHIVE},
	{ &cg_teamModel, "cg_teamModel", "", CVAR_ARCHIVE},
	{ &cg_teamColors, "cg_teamColors", "", CVAR_ARCHIVE},
	{ &cg_fovAdjust, "cg_fovAdjust", "0", CVAR_ARCHIVE},
	{ &cg_noChatBeep, "cg_noChatBeep", "0", CVAR_ARCHIVE},
    { &cg_showSpawns, "cg_showSpawns", "0", CVAR_ARCHIVE},
    { &cg_autorecord, "cg_autorecord", "1", CVAR_ARCHIVE},
    { &cg_drawPickup, "cg_drawPickup", "1", CVAR_ARCHIVE},
    { &cg_teleportEffect, "cg_teleportEffect", "2", CVAR_ARCHIVE},
    { &cg_gibEffect, "cg_gibEffect", "2", CVAR_ARCHIVE},
    { &cg_landingSound, "cg_landingSound", "1", CVAR_ARCHIVE},
    { &cg_painSounds, "cg_painSounds", "1", CVAR_ARCHIVE},
    { &cg_footsteps, "cg_footsteps", "1", CVAR_ARCHIVE },
    { &cg_alwaysDrawItems, "cg_alwaysDrawItems", "1", CVAR_ARCHIVE},

    { &hud_fontSize, "hud_fontSize", "6", CVAR_ARCHIVE},
    { &hud_fontColor, "hud_fontColor", "7", CVAR_ARCHIVE},
    { &hud_frameColor, "hud_frameColor", "0", CVAR_ARCHIVE},
	{ &hud_useHoldable, "hud_useHoldable", "0", CVAR_ARCHIVE},
    { &hud_timer, "hud_timer", "1", CVAR_ARCHIVE},
    { &hud_timerX, "hud_timerX", "45", CVAR_ARCHIVE},
    { &hud_timerY, "hud_timerY", "200", CVAR_ARCHIVE},
    { &hud_bboxFPS, "hud_bboxFPS", "1", CVAR_ARCHIVE },
    { &hud_bbox, "hud_bbox", "0", CVAR_ARCHIVE },
    { &hud_bboxColor, "hud_bboxColor", "2", CVAR_ARCHIVE },
    { &hud_bboxMinsZ, "hud_bboxMinsZ", "-24", CVAR_ARCHIVE },
    { &hud_cgaz, "hud_cgaz", "1", CVAR_ARCHIVE},
    { &hud_cgazAlpha, "hud_cgazAlpha", "0.75", CVAR_ARCHIVE},
    { &hud_cgazY, "hud_cgazY", "232", CVAR_ARCHIVE},
    { &hud_snap, "hud_snap", "1", CVAR_ARCHIVE},
	{ &hud_snapY, "hud_snapY", "216", CVAR_ARCHIVE},
	{ &hud_snapRGBA, "hud_snapRGBA", "0 0 .8 .4", CVAR_ARCHIVE},

    { &chs_fontSize, "chs_fontSize", "8", CVAR_ARCHIVE},
    { &chs_fontColor, "chs_fontColor", "7", CVAR_ARCHIVE},
    { &chs_fontHighlightColor, "chs_fontHighlightColor", "5", CVAR_ARCHIVE},
    { &chs_keypress, "chs_keypress", "1", CVAR_ARCHIVE},
    { &chs_ob, "chs_ob", "1", CVAR_ARCHIVE},
	{ &chs_obX, "chs_obX", "300", CVAR_ARCHIVE},
	{ &chs_obY, "chs_obY", "216", CVAR_ARCHIVE},
    { &chs_speed, "chs_speed", "1", CVAR_ARCHIVE},
    { &chs_speedX, "chs_speedX", "318", CVAR_ARCHIVE},
    { &chs_speedY, "chs_speedY", "208", CVAR_ARCHIVE},
    { &chs_angles, "chs_angles", "2", CVAR_ARCHIVE},
    { &chs_anglesX, "chs_anglesX", "318", CVAR_ARCHIVE},
    { &chs_anglesY, "chs_anglesY", "264", CVAR_ARCHIVE},
    { &chs_levelTimer, "chs_levelTimer", "0", CVAR_ARCHIVE },
    { &chs_levelTimerX, "chs_levelTimerX", "640", CVAR_ARCHIVE },
    { &chs_levelTimerY, "chs_levelTimerY", "0", CVAR_ARCHIVE },
    { &chs_fps, "chs_fps", "1", CVAR_ARCHIVE },
    { &chs_fpsX, "chs_fpsX", "640", CVAR_ARCHIVE },
    { &chs_fpsY, "chs_fpsY", "10", CVAR_ARCHIVE },
    { &chs_ping, "chs_ping", "1", CVAR_ARCHIVE },
    { &chs_pingX, "chs_pingX", "640", CVAR_ARCHIVE },
    { &chs_pingY, "chs_pingY", "20", CVAR_ARCHIVE },
	
	{ &cp_centerDraw, "cp_centerDraw", "1", CVAR_ARCHIVE }
};


/*
=================
CG_RegisterCvars
=================
*/
void CG_RegisterCvars( void ) {
	int			i;
	const cvarTable_t	*cv;
	char		var[MAX_TOKEN_CHARS];

	for ( i = 0, cv = cvarTable ; i < ARRAY_LEN( cvarTable ) ; i++, cv++ ) {
		trap_Cvar_Register( cv->vmCvar, cv->cvarName,
			cv->defaultString, cv->cvarFlags );
	}

	// see if we are also running the server on this machine
	trap_Cvar_VariableStringBuffer( "sv_running", var, sizeof( var ) );
	cgs.localServer = atoi( var );

	forceModelModificationCount = cg_forceModel.modificationCount;
	enemyModelModificationCount = cg_enemyModel.modificationCount;
	enemyColorsModificationCount = cg_enemyColors.modificationCount;
	teamModelModificationCount = cg_teamModel.modificationCount;
	teamColorsModificationCount = cg_teamColors.modificationCount;


	trap_Cvar_Register(NULL, "model", DEFAULT_MODEL, CVAR_USERINFO | CVAR_ARCHIVE );
	trap_Cvar_Register(NULL, "headmodel", DEFAULT_MODEL, CVAR_USERINFO | CVAR_ARCHIVE );
}


/*
===================
CG_ForceModelChange
===================
*/
void CG_ForceModelChange( void ) {
	const char *clientInfo;
	int	i;

	for ( i = 0 ; i < MAX_CLIENTS ; i++ ) {
		clientInfo = CG_ConfigString( CS_PLAYERS + i );
		if ( !clientInfo[0] ) {
			continue;
		}
		CG_NewClientInfo( i );
	}
}


/*
=================
CG_UpdateCvars
=================
*/
void CG_UpdateCvars( void ) {
	int			i;
	const cvarTable_t	*cv;

	for ( i = 0, cv = cvarTable ; i < ARRAY_LEN( cvarTable ) ; i++, cv++ ) {
		trap_Cvar_Update( cv->vmCvar );
	}

	// if model changed
	if ( forceModelModificationCount != cg_forceModel.modificationCount
		|| enemyModelModificationCount != cg_enemyModel.modificationCount
		|| enemyColorsModificationCount != cg_enemyColors.modificationCount
		|| teamModelModificationCount != cg_teamModel.modificationCount
		|| teamColorsModificationCount != cg_teamColors.modificationCount ) {

		forceModelModificationCount = cg_forceModel.modificationCount;
		enemyModelModificationCount = cg_enemyModel.modificationCount;
		enemyColorsModificationCount = cg_enemyColors.modificationCount;
		teamModelModificationCount = cg_teamModel.modificationCount;
		teamColorsModificationCount = cg_teamColors.modificationCount;

		CG_ForceModelChange();
	}
}


int CG_CrosshairPlayer( void ) {
	if ( cg.time > ( cg.crosshairClientTime + 1000 ) ) {
		return -1;
	}
	return cg.crosshairClientNum;
}

int CG_LastAttacker( void ) {
	if ( !cg.attackerTime ) {
		return -1;
	}
	return cg.snap->ps.persistant[PERS_ATTACKER];
}

void QDECL CG_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	ED_vsprintf (text, msg, argptr);
	va_end (argptr);

	trap_Print( text );
}

void QDECL CG_Error( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	ED_vsprintf (text, msg, argptr);
	va_end (argptr);

	trap_Error( text );
}

#ifndef CGAME_HARD_LINKED
// this is only here so the functions in q_shared.c and bg_*.c can link (FIXME)

void QDECL Com_Error( int level, const char *error, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, error);
	ED_vsprintf (text, error, argptr);
	va_end (argptr);

	trap_Error( text );
}

void QDECL Com_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	ED_vsprintf (text, msg, argptr);
	va_end (argptr);

	trap_Print( text );
}

int CG_VariableIntegerValue( const char *cvar ) {
	static char buffer[MAX_STRING_CHARS];

	trap_Cvar_VariableStringBuffer( cvar, buffer, sizeof(buffer) );
	return atoi( buffer );
}

#endif

/*
================
CG_Argv
================
*/
const char *CG_Argv( int arg )
{
	static char	buffer[ 2 ][ MAX_STRING_CHARS ];
	static int index = 0;

	index ^= 1;
	trap_Argv( arg, buffer[ index ], sizeof( buffer[ 0 ] ) );

	return buffer[ index ];
}


//========================================================================

/*
=================
CG_RegisterItemSounds

The server says this item is used on this level
=================
*/
static void CG_RegisterItemSounds( int itemNum ) {
	gitem_t			*item;
	char			data[MAX_QPATH];
	const char		*s, *start;
	int				len;

	item = &bg_itemlist[ itemNum ];

	if( item->pickup_sound ) {
		trap_S_RegisterSound( item->pickup_sound, qfalse );
	}

	// parse the space seperated precache string for other media
	s = item->sounds;
	if (!s || !s[0])
		return;

	while (*s) {
		start = s;
		while (*s && *s != ' ') {
			s++;
		}

		len = s-start;
		if (len >= MAX_QPATH || len < 5) {
			CG_Error( "PrecacheItem: %s has bad precache string",
				item->classname);
			return;
		}
		memcpy (data, start, len);
		data[len] = 0;
		if ( *s ) {
			s++;
		}

		if ( !strcmp(data+len-3, "wav" )) {
			trap_S_RegisterSound( data, qfalse );
		}
	}
}


/*
=================
CG_RegisterSounds

called during a precache command
=================
*/
static void CG_RegisterSounds( void )
{
	int		i;
	char	items[MAX_ITEMS+1];
	char	name[MAX_QPATH];
	const char	*soundName;

	cgs.media.countSound       = trap_S_RegisterSound( "sound/misc/count.wav", qtrue );
	cgs.media.tracerSound       = trap_S_RegisterSound( "sound/weapons/machinegun/buletby1.wav", qfalse );
	cgs.media.selectSound       = trap_S_RegisterSound( "sound/weapons/change.wav", qfalse );
	cgs.media.wearOffSound      = trap_S_RegisterSound( "sound/items/wearoff.wav", qfalse );
	cgs.media.useNothingSound   = trap_S_RegisterSound( "sound/items/use_nothing.wav", qfalse );
	cgs.media.gibSound          = trap_S_RegisterSound( "sound/player/gibsplt1.wav", qfalse );
	cgs.media.gibBounce1Sound   = trap_S_RegisterSound( "sound/player/gibimp1.wav", qfalse );
	cgs.media.gibBounce2Sound   = trap_S_RegisterSound( "sound/player/gibimp2.wav", qfalse );
	cgs.media.gibBounce3Sound   = trap_S_RegisterSound( "sound/player/gibimp3.wav", qfalse );
	cgs.media.teleInSound       = trap_S_RegisterSound( "sound/world/telein.wav", qfalse );
	cgs.media.teleOutSound      = trap_S_RegisterSound( "sound/world/teleout.wav", qfalse );
	cgs.media.respawnSound      = trap_S_RegisterSound( "sound/items/respawn1.wav", qfalse );
	cgs.media.noAmmoSound       = trap_S_RegisterSound( "sound/weapons/noammo.wav", qfalse );
	cgs.media.talkSound         = trap_S_RegisterSound( "sound/player/talk.wav", qfalse );
	cgs.media.takenLeadSound    = trap_S_RegisterSound( "sound/feedback/takenlead.wav", qtrue);
	cgs.media.tiedLeadSound     = trap_S_RegisterSound( "sound/feedback/tiedlead.wav", qtrue);
	cgs.media.lostLeadSound     = trap_S_RegisterSound( "sound/feedback/lostlead.wav", qtrue);
	cgs.media.watrInSound       = trap_S_RegisterSound( "sound/player/watr_in.wav", qfalse);
	cgs.media.watrOutSound      = trap_S_RegisterSound( "sound/player/watr_out.wav", qfalse);
	cgs.media.watrUnSound       = trap_S_RegisterSound( "sound/player/watr_un.wav", qfalse);
	cgs.media.jumpPadSound      = trap_S_RegisterSound( "sound/world/jumppad.wav", qfalse );
	cgs.media.flagPickup        = trap_S_RegisterSound( "sound/feedback/flagpkp.wav", qfalse );

	cgs.media.landSound[LANDSOUND_NORMAL]      = trap_S_RegisterSound( "sound/player/landing/normal1.wav", qfalse);
	cgs.media.landSound[LANDSOUND_FLESH]       = trap_S_RegisterSound( "sound/player/landing/flesh1.wav", qfalse);
	cgs.media.landSound[LANDSOUND_METAL]       = trap_S_RegisterSound( "sound/player/landing/metal1.wav", qfalse);
	cgs.media.landSound[LANDSOUND_SPLASH]      = trap_S_RegisterSound( "sound/player/landing/water1.wav", qfalse);
	cgs.media.landSound[LANDSOUND_STONE]       = trap_S_RegisterSound( "sound/player/landing/stone1.wav", qfalse);
	cgs.media.landSound[LANDSOUND_GRASS]       = trap_S_RegisterSound( "sound/player/landing/grass1.wav", qfalse);
    cgs.media.landSound[LANDSOUND_TALLGRASS]   = trap_S_RegisterSound( "sound/player/landing/tallgrass1.wav", qfalse);
    cgs.media.landSound[LANDSOUND_WOOD]        = trap_S_RegisterSound( "sound/player/landing/wood1.wav", qfalse);
    cgs.media.landSound[LANDSOUND_SAND]        = trap_S_RegisterSound( "sound/player/landing/sand1.wav", qfalse);
    cgs.media.landSound[LANDSOUND_LAVA]        = trap_S_RegisterSound( "sound/player/landing/lava1.wav", qfalse);
    cgs.media.landSound[LANDSOUND_ICE]         = trap_S_RegisterSound( "sound/player/landing/ice1.wav", qfalse);

	for (i=0 ; i<4 ; i++)
    {
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/step%i.wav", i+1);
            cgs.media.footsteps[FOOTSTEP_NORMAL][i] = trap_S_RegisterSound (name, qfalse);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/step%i.wav", i+1);         //boot
            cgs.media.footsteps[FOOTSTEP_BOOT][i] = trap_S_RegisterSound (name, qfalse);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/flesh%i.wav", i+1);
            cgs.media.footsteps[FOOTSTEP_FLESH][i] = trap_S_RegisterSound (name, qfalse);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/mech%i.wav", i+1);
            cgs.media.footsteps[FOOTSTEP_MECH][i] = trap_S_RegisterSound (name, qfalse);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/step%i.wav", i+1);         //energy
            cgs.media.footsteps[FOOTSTEP_ENERGY][i] = trap_S_RegisterSound (name, qfalse);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/splash%i.wav", i+1);
            cgs.media.footsteps[FOOTSTEP_SPLASH][i] = trap_S_RegisterSound (name, qfalse);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/clank%i.wav", i+1);
            cgs.media.footsteps[FOOTSTEP_METAL][i] = trap_S_RegisterSound (name, qfalse);

        // ----- new surface walk sounds below -------------
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/stone%i.wav", i+1);
            cgs.media.footsteps[FOOTSTEP_STONE][i] = trap_S_RegisterSound (name, qfalse);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/grass%i.wav", i+1);
            cgs.media.footsteps[FOOTSTEP_GRASS][i] = trap_S_RegisterSound (name, qfalse);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/tallgrass%i.wav", i+1);
            cgs.media.footsteps[FOOTSTEP_TALLGRASS][i] = trap_S_RegisterSound (name, qfalse);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/wood%i.wav", i+1);
            cgs.media.footsteps[FOOTSTEP_WOOD][i] = trap_S_RegisterSound (name, qfalse);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/sand%i.wav", i+1);
            cgs.media.footsteps[FOOTSTEP_SAND][i] = trap_S_RegisterSound (name, qfalse);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/ice%i.wav", i+1);
            cgs.media.footsteps[FOOTSTEP_ICE][i] = trap_S_RegisterSound (name, qfalse);
		Com_sprintf (name, sizeof(name), "sound/player/footsteps/lava%i.wav", i+1);
            cgs.media.footsteps[FOOTSTEP_LAVA][i] = trap_S_RegisterSound (name, qfalse);
	}

	// only register the items that the server says we need
	Q_strncpyz(items, CG_ConfigString(CS_ITEMS), sizeof(items));

	for ( i = 1 ; i < bg_numItems ; i++ ) {
			CG_RegisterItemSounds( i );
	}

	for ( i = 1 ; i < MAX_SOUNDS ; i++ )
    {
		soundName = CG_ConfigString( CS_SOUNDS+i );
		if ( !soundName[0] )
		{
			break;
		}
		if ( soundName[0] == '*' )
		{
			continue;	// custom sound
		}
		cgs.gameSounds[i] = trap_S_RegisterSound( soundName, qfalse );
	}

	// FIXME: only needed with item
	cgs.media.flightSound   = trap_S_RegisterSound ("sound/items/flight.wav", qfalse);
	cgs.media.medkitSound   = trap_S_RegisterSound ("sound/items/use_medkit.wav", qfalse);
	cgs.media.quadSound     = trap_S_RegisterSound ("sound/items/damage3.wav", qfalse);
	cgs.media.sfx_ric1      = trap_S_RegisterSound ("sound/weapons/machinegun/ric1.wav", qfalse);
	cgs.media.sfx_ric2      = trap_S_RegisterSound ("sound/weapons/machinegun/ric2.wav", qfalse);
	cgs.media.sfx_ric3      = trap_S_RegisterSound ("sound/weapons/machinegun/ric3.wav", qfalse);
	cgs.media.sfx_rockexp   = trap_S_RegisterSound ("sound/weapons/rocket/rocklx1a.wav", qfalse);
	cgs.media.sfx_plasmaexp = trap_S_RegisterSound ("sound/weapons/plasma/plasmx1a.wav", qfalse);
	cgs.media.regenSound    = trap_S_RegisterSound ("sound/items/regen.wav", qfalse);
	cgs.media.protectSound  = trap_S_RegisterSound ("sound/items/protect3.wav", qfalse);
	cgs.media.n_healthSound = trap_S_RegisterSound ("sound/items/n_health.wav", qfalse);
	cgs.media.hgrenb1aSound = trap_S_RegisterSound ("sound/weapons/grenade/hgrenb1a.wav", qfalse);
	cgs.media.hgrenb2aSound = trap_S_RegisterSound ("sound/weapons/grenade/hgrenb2a.wav", qfalse);

}


//===================================================================================


/*
=================
CG_RegisterGraphics

This function may execute for a couple of minutes with a slow disk.
=================
*/
static void CG_RegisterGraphics( void )
{
	int			i;
	char		items[MAX_ITEMS+1];
	static char	*sb_nums[11] =
	{
		"gfx/2d/numbers/zero_32b",
		"gfx/2d/numbers/one_32b",
		"gfx/2d/numbers/two_32b",
		"gfx/2d/numbers/three_32b",
		"gfx/2d/numbers/four_32b",
		"gfx/2d/numbers/five_32b",
		"gfx/2d/numbers/six_32b",
		"gfx/2d/numbers/seven_32b",
		"gfx/2d/numbers/eight_32b",
		"gfx/2d/numbers/nine_32b",
		"gfx/2d/numbers/minus_32b",
	};

	// clear any references to old media
	memset( &cg.refdef, 0, sizeof( cg.refdef ) );
	trap_R_ClearScene();

	CG_LoadingString( cgs.mapname );

	trap_R_LoadWorldMap( cgs.mapname );

	// precache status bar pics
	CG_LoadingString( "game media" );

	for ( i = 0 ; i < ARRAY_LEN( sb_nums ) ; i++ )
    {
		cgs.media.numberShaders[i] = trap_R_RegisterShader( sb_nums[i] );
	}

	cgs.media.viewBloodShader        = trap_R_RegisterShader( "viewBloodBlend" );
	cgs.media.deferShader            = trap_R_RegisterShaderNoMip( "gfx/2d/defer.tga" );
	cgs.media.smokePuffShader        = trap_R_RegisterShader( "smokePuff" );
	cgs.media.smokePuffRageProShader = trap_R_RegisterShader( "smokePuffRagePro" );
	cgs.media.shotgunSmokePuffShader = trap_R_RegisterShader( "shotgunSmokePuff" );
	cgs.media.plasmaBallShader       = trap_R_RegisterShader( "sprites/plasma1" );
	cgs.media.bloodTrailShader       = trap_R_RegisterShader( "bloodTrail" );
	cgs.media.lagometerShader        = trap_R_RegisterShader( "lagometer" );
	cgs.media.connectionShader       = trap_R_RegisterShader( "disconnected" );
	cgs.media.waterBubbleShader      = trap_R_RegisterShader( "waterBubble" );
	cgs.media.sparkShader            = trap_R_RegisterShaderNoMip( "spark" );
	cgs.media.tracerShader           = trap_R_RegisterShader( "gfx/misc/tracer" );
	cgs.media.selectShader           = trap_R_RegisterShader( "gfx/2d/select2" );

	for ( i = 0 ; i < NUM_CROSSHAIRS ; i++ ) {
		cgs.media.crosshairShader[i] = trap_R_RegisterShaderNoMip( va("gfx/crosshairs/crosshair%c", 'a'+i) );
	}

	cgs.media.noammoShader       = trap_R_RegisterShader( "icons/noammo" );

	// powerup shaders
	cgs.media.quadShader         = trap_R_RegisterShader("powerups/quad");
	cgs.media.quadWeaponShader   = trap_R_RegisterShader("powerups/quadWeapon");
	cgs.media.battleSuitShader   = trap_R_RegisterShader("powerups/battleSuit");
	cgs.media.battleWeaponShader = trap_R_RegisterShader("powerups/battleWeapon");
	cgs.media.invisShader        = trap_R_RegisterShader("powerups/invisibility");
	cgs.media.regenShader        = trap_R_RegisterShader("powerups/regen");
	cgs.media.hastePuffShader    = trap_R_RegisterShader("hasteSmokePuff");

    cgs.media.redFlagModel      = trap_R_RegisterModel( "models/flags/r_flag.md3");
    cgs.media.blueFlagModel     = trap_R_RegisterModel( "models/flags/b_flag.md3");
	cgs.media.redQuadShader     = trap_R_RegisterShader("powerups/blueflag");

	cgs.media.armorModel           = trap_R_RegisterModel( "models/powerups/armor/armor_yel.md3" );
	cgs.media.machinegunBrassModel = trap_R_RegisterModel( "models/weapons2/shells/m_shell.md3" );
	cgs.media.shotgunBrassModel    = trap_R_RegisterModel( "models/weapons2/shells/s_shell.md3" );
	cgs.media.gibAbdomen           = trap_R_RegisterModel( "models/gibs/abdomen.md3" );
	cgs.media.gibArm               = trap_R_RegisterModel( "models/gibs/arm.md3" );
	cgs.media.gibChest             = trap_R_RegisterModel( "models/gibs/chest.md3" );
	cgs.media.gibFist              = trap_R_RegisterModel( "models/gibs/fist.md3" );
	cgs.media.gibFoot              = trap_R_RegisterModel( "models/gibs/foot.md3" );
	cgs.media.gibForearm           = trap_R_RegisterModel( "models/gibs/forearm.md3" );
	cgs.media.gibIntestine         = trap_R_RegisterModel( "models/gibs/intestine.md3" );
	cgs.media.gibLeg               = trap_R_RegisterModel( "models/gibs/leg.md3" );
	cgs.media.gibSkull             = trap_R_RegisterModel( "models/gibs/skull.md3" );
	cgs.media.gibBrain             = trap_R_RegisterModel( "models/gibs/brain.md3" );
	cgs.media.smoke2               = trap_R_RegisterModel( "models/weapons2/shells/s_shell.md3" );
	cgs.media.balloonShader        = trap_R_RegisterShader( "sprites/balloon3" );
	cgs.media.bloodExplosionShader = trap_R_RegisterShader( "bloodExplosion" );
	cgs.media.bulletFlashModel     = trap_R_RegisterModel("models/weaphits/bullet.md3");
	cgs.media.ringFlashModel       = trap_R_RegisterModel("models/weaphits/ring02.md3");
	cgs.media.dishFlashModel       = trap_R_RegisterModel("models/weaphits/boom01.md3");
	cgs.media.teleportEffectModel  = trap_R_RegisterModel( "models/misc/telep.md3" );
	cgs.media.teleportEffectShader = trap_R_RegisterShader( "teleportEffect" );

    // for lasers
	cgs.media.railCoreShader       = trap_R_RegisterShader( "railCore" );

	// hud
	cgs.media.clockIcon       	   = trap_R_RegisterShaderNoMip("hud/clock");
	cgs.media.keyIcon	           = trap_R_RegisterShaderNoMip("hud/key");
	cgs.media.timerFrame		   = trap_R_RegisterShaderNoMip("hud/timerframe");
	cgs.media.checkpointFrame	   = trap_R_RegisterShaderNoMip("hud/checkpointFrame");
	cgs.media.unavailableItem      = trap_R_RegisterShaderNoMip( "sprites/notAvailable" );
	cgs.media.healthIcon           = trap_R_RegisterShaderNoMip( "hud/health" );
	cgs.media.armorIcon            = trap_R_RegisterShaderNoMip( "hud/armor" );
		
	// scoreboard
	cgs.media.sbbottom 			   = trap_R_RegisterShaderNoMip("sb/sbbottom");
	cgs.media.sbback 			   = trap_R_RegisterShaderNoMip("sb/sbback");
	cgs.media.sbheader 			   = trap_R_RegisterShaderNoMip("sb/sbheader");
	
	cgs.media.sbrocket 			   = trap_R_RegisterShaderNoMip("sb/rocket");
	cgs.media.sbplasma 			   = trap_R_RegisterShaderNoMip("sb/plasma");
	cgs.media.sbgrenade 		   = trap_R_RegisterShaderNoMip("sb/grenade");
	cgs.media.sbslick 			   = trap_R_RegisterShaderNoMip("sb/slick");
	cgs.media.sbtele 			   = trap_R_RegisterShaderNoMip("sb/tele");
	cgs.media.sbpad 			   = trap_R_RegisterShaderNoMip("sb/pad");
	cgs.media.sbdoor 			   = trap_R_RegisterShaderNoMip("sb/door");
	cgs.media.sboverbounce 		   = trap_R_RegisterShaderNoMip("sb/obs");
	cgs.media.sbinterference 	   = trap_R_RegisterShaderNoMip("sb/interference");
	cgs.media.sbreverse  		   = trap_R_RegisterShaderNoMip("sb/reverse");

	memset( cg_items,   0, sizeof( cg_items ) );
	memset( cg_weapons, 0, sizeof( cg_weapons ) );

	// only register the items that the server says we need
	Q_strncpyz( items, CG_ConfigString(CS_ITEMS), sizeof( items ) );

	for ( i = 1 ; i < bg_numItems ; i++ )
    {
		if ( items[ i ] == '1' || cg_buildScript.integer )
		{
			CG_LoadingItem( i );
			CG_RegisterItemVisuals( i );
		}
	}

	cg.skipDFshaders = qfalse;

	// wall marks
	cgs.media.bulletMarkShader = trap_R_RegisterShader( "gfx/damage/bullet_mrk" );
	cgs.media.burnMarkShader   = trap_R_RegisterShader( "gfx/damage/burn_med_mrk" );
	cgs.media.holeMarkShader   = trap_R_RegisterShader( "gfx/damage/hole_lg_mrk" );
	cgs.media.energyMarkShader = trap_R_RegisterShader( "gfx/damage/plasma_mrk" );
	cgs.media.shadowMarkShader = trap_R_RegisterShader( "markShadow" );
	cgs.media.wakeMarkShader   = trap_R_RegisterShader( "wake" );
	cgs.media.bloodMarkShader  = trap_R_RegisterShader( "bloodMark" );

	// register the inline models
	cgs.numInlineModels = trap_CM_NumInlineModels();
	for ( i = 1 ; i < cgs.numInlineModels ; i++ )
    {
		char	name[10];
		vec3_t	mins, maxs;
		int		j;

		Com_sprintf( name, sizeof(name), "*%i", i );
		cgs.inlineDrawModel[i] = trap_R_RegisterModel( name );
		trap_R_ModelBounds( cgs.inlineDrawModel[i], mins, maxs );
		for ( j = 0 ; j < 3 ; j++ )
        {
			cgs.inlineModelMidpoints[i][j] = mins[j] + 0.5 * ( maxs[j] - mins[j] );
		}
	}

	// register all the server specified models
	for (i=1 ; i<MAX_MODELS ; i++)
    {
		const char	*modelName;

		modelName = CG_ConfigString( CS_MODELS+i );
		if ( !modelName[0] )
        {
			break;
		}
		cgs.gameModels[i] = trap_R_RegisterModel( modelName );
	}

	cgs.media.cursor = trap_R_RegisterShaderNoMip( "gfx/ui/cursor" );

	CG_ClearParticles ();

}



/*
=======================
CG_BuildSpectatorString

=======================
*/
void CG_BuildSpectatorString( void )
{
	int i;
	cg.spectatorList[0] = 0;
	for (i = 0; i < MAX_CLIENTS; i++)
    {
		if (cgs.clientinfo[i].infoValid && cgs.clientinfo[i].team == TEAM_SPECTATOR )
			Q_strcat(cg.spectatorList, sizeof(cg.spectatorList), va("%s     ", cgs.clientinfo[i].name));
	}

	i = strlen(cg.spectatorList);
	if (i != cg.spectatorLen) {
		cg.spectatorLen = i;
		cg.spectatorWidth = -1;
	}
}


/*
===================
CG_RegisterClients
===================
*/
static void CG_RegisterClients( void )
{
	int		i;

	CG_LoadingClient(cg.clientNum);
	CG_NewClientInfo(cg.clientNum);

	for (i=0 ; i<MAX_CLIENTS ; i++)
    {
		const char	*clientInfo;

		if (cg.clientNum == i)
			continue;

		clientInfo = CG_ConfigString( CS_PLAYERS+i );
		if ( !clientInfo[0])
			continue;

		CG_LoadingClient( i );
		CG_NewClientInfo( i );
	}
	CG_BuildSpectatorString();
}

//===========================================================================

/*
=================
CG_ConfigString
=================
*/
const char *CG_ConfigString( int index )
{
	if ( index < 0 || index >= MAX_CONFIGSTRINGS ) {
		CG_Error( "CG_ConfigString: bad index: %i", index );
		return "";
	}
	return cgs.gameState.stringData + cgs.gameState.stringOffsets[ index ];
}

//==================================================================

/*
======================
CG_StartMusic

======================
*/
void CG_StartMusic( void )
{
	char	*s;
	char	parm1[MAX_QPATH], parm2[MAX_QPATH];

	// start the background music
	s = (char *)CG_ConfigString( CS_MUSIC );
	if ( !s || strlen(s) == 0) {
		trap_S_StopBackgroundTrack();
	} else {
		Q_strncpyz( parm1, COM_Parse( &s ), sizeof( parm1 ) );
		Q_strncpyz( parm2, COM_Parse( &s ), sizeof( parm2 ) );

        trap_S_StartBackgroundTrack( parm1, parm2 );
	}
}


/*
=================
CG_Init

Called after every level change or subsystem restart
Will perform callbacks to make the loading info screen update.
=================
*/
void CG_Init( int serverMessageNum, int serverCommandSequence, int clientNum )
{
	char  value[MAX_CVAR_VALUE_STRING];
	const char	*s;
	const char  *gt, *rs, *dfmode, *fcmode, *obs, *interference, *rev;

	// clear everything
	memset( &cgs, 0, sizeof( cgs ) );
	memset( &cg, 0, sizeof( cg ) );
	memset( cg_entities, 0, sizeof(cg_entities) );
	memset( cg_weapons, 0, sizeof(cg_weapons) );
	memset( cg_items, 0, sizeof(cg_items) );

	cg.clientNum = clientNum;

	cgs.processedSnapshotNum = serverMessageNum;
	cgs.serverCommandSequence = serverCommandSequence;

	trap_Cvar_VariableStringBuffer( "//trap_GetValue", value, sizeof( value ) );
	if ( value[0] )
    {
		trap_GetValue = (void*)~atoi( value );
		if ( trap_GetValue( value, sizeof( value ), "trap_R_AddRefEntityToScene2" ) )
		{
			trap_R_AddRefEntityToScene2 = (void*)~atoi( value );
			intShaderTime = qtrue;
		}

		if ( trap_GetValue( value, sizeof( value ), "trap_R_AddLinearLightToScene_Q3E" ) )
        {
			trap_R_AddLinearLightToScene = (void*)~atoi( value );
			linearLight = qtrue;
		}
	}

	// load a few needed things before we do any screen updates
	cgs.media.charsetShader		= trap_R_RegisterShader( "gfx/2d/bigchars" );
	cgs.media.whiteShader		= trap_R_RegisterShader( "white" );
	cgs.media.charsetProp		= trap_R_RegisterShaderNoMip( "menu/art/font1_prop.tga" );
	cgs.media.charsetPropGlow	= trap_R_RegisterShaderNoMip( "menu/art/font1_prop_glo.tga" );
	cgs.media.charsetPropB		= trap_R_RegisterShaderNoMip( "menu/art/font2_prop.tga" );

	CG_RegisterCvars();

	CG_InitConsoleCommands();

	cg.weaponSelect = WP_MACHINEGUN;

	cgs.redflag = cgs.blueflag = -1; // For compatibily, default to unset for
	cgs.flagStatus = -1;
	// old servers

	// get the rendering configuration from the client system
	trap_GetGlconfig( &cgs.glconfig );

	cgs.screenXBias = 0.0;
	cgs.screenYBias = 0.0;

	if ( cgs.glconfig.vidWidth * 480 > cgs.glconfig.vidHeight * 640 )
    {
		// wide screen, scale by height
		cgs.screenXScale = cgs.screenYScale = cgs.glconfig.vidHeight * (1.0/480.0);
		cgs.screenXBias = 0.5 * ( cgs.glconfig.vidWidth - ( cgs.glconfig.vidHeight * (640.0/480.0) ) );
	}
	else
	{
		// no wide screen, scale by width
		cgs.screenXScale = cgs.screenYScale = cgs.glconfig.vidWidth * (1.0/640.0);
		cgs.screenYBias = 0.5 * ( cgs.glconfig.vidHeight - ( cgs.glconfig.vidWidth * (480.0/640.0) ) );
	}

	cgs.screenXmin = 0.0 - (cgs.screenXBias / cgs.screenXScale);
	cgs.screenXmax = 640.0 + (cgs.screenXBias / cgs.screenXScale);

	cgs.screenYmin = 0.0 - (cgs.screenYBias / cgs.screenYScale);
	cgs.screenYmax = 480.0 + (cgs.screenYBias / cgs.screenYScale);

	cgs.cursorScaleR = 1.0 / cgs.screenXScale;
	if ( cgs.cursorScaleR < 0.5 )
		cgs.cursorScaleR = 0.5;

	CG_LoadFonts();

	// get the gamestate from the client system
	trap_GetGameState( &cgs.gameState );

	// check version SLK: keep an eye on this
	s = CG_ConfigString( CS_GAME_VERSION );
	if ( strcmp( s, GAME_VERSION ) )
		CG_Error( "Client/Server game mismatch: %s/%s", GAME_VERSION, s );

	s = CG_ConfigString( CS_LEVEL_START_TIME );
	cgs.levelStartTime = atoi( s );

	CG_ParseServerinfo();

	// update settings from configstrings
	gt = CG_ConfigString( CS_GAMETYPE );
	rs = CG_ConfigString( CS_RULESET );
	dfmode = CG_ConfigString( CS_DFMODE );
	fcmode = CG_ConfigString( CS_FCMODE );
	obs = CG_ConfigString( CS_OVERBOUNCES );
	interference = CG_ConfigString( CS_INTERFERENCE );
    rev = CG_ConfigString( CS_REVERSEMAP );

	BG_SharedCvarsUpdate( atoi(gt), atoi(rs), atoi(dfmode), atoi(fcmode), atoi(obs), atoi(interference), atoi(rev) );



	CG_ParseSysteminfo();

	// load the new map
	CG_LoadingString( "collision map" );

	trap_CM_LoadMap( cgs.mapname );

	cg.loading = qtrue;		// force players to load instead of defer

	CG_LoadingString( "sounds" );

	CG_RegisterSounds();

	CG_LoadingString( "graphics" );

	CG_RegisterGraphics();

	CG_LoadingString( "clients" );

	CG_RegisterClients();		// if low on memory, some clients will be deferred

	cg.loading = qfalse;	// future players will be deferred

	CG_InitLocalEntities();

	CG_InitMarkPolys();

	// remove the last loading update
	cg.infoScreenText[0] = 0;

	// Make sure we have update values (scores)
	// CG_SetConfigValues();

	CG_StartMusic();

	CG_LoadingString( "" );

	CG_ShaderStateChanged();

	trap_S_ClearLoopingSounds( qtrue );
}


/*
=================
CG_Shutdown

Called before every level change or subsystem restart
=================
*/
void CG_Shutdown( void )
{
	// some mods may need to do cleanup work here,
	// like closing files or archiving session data
}


void CG_SetScoreCatcher( qbool enable )
{
	int	currentCatcher, newCatcher, old_state, new_state;
	qbool spectator;

	currentCatcher = trap_Key_GetCatcher();

	if ( currentCatcher & KEYCATCH_CONSOLE )
		return;

	spectator = cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR || cg.demoPlayback || ( cg.snap->ps.pm_flags & PMF_FOLLOW );

	if ( enable && spectator )
    {
		cgs.score_key = trap_Key_GetKey( "+scores" );
		cgs.score_catched = qtrue;
		newCatcher = currentCatcher | KEYCATCH_CGAME;
	}
	else
    {
		cgs.score_catched = qfalse;
		newCatcher = currentCatcher & ~KEYCATCH_CGAME;
	}

	if ( newCatcher != currentCatcher )
    {
		if ( cgs.score_key )
		{
			// keycatcher change may cause reset of all pressed buttons on new engines
			// so track state of scoreboard key and ignore first upcoming keyup event for it
			old_state = trap_Key_IsDown( cgs.score_key );
			trap_Key_SetCatcher( newCatcher );
			new_state = trap_Key_IsDown( cgs.score_key );
			if ( new_state != old_state )
			{
				cgs.filterKeyUpEvent = qtrue;
			}
		}
        else
        {
			trap_Key_SetCatcher( newCatcher );
		}
	}

}


void CG_KeyEvent( int key, qbool down )
{
	// process scoreboard clicks etc.
	if ( cgs.score_catched && down ) {
		if ( key == cgs.score_key )
			return;

		if ( key == 178 )
			CG_ScoreboardClick();
		else
			CG_SetScoreCatcher( qfalse );
	}
}


void CG_MouseEvent( int x, int y )
{
	cgs.cursorX += x * cgs.cursorScaleR;
	cgs.cursorY += y * cgs.cursorScaleR;

	if ( cgs.cursorX < cgs.screenXmin )
    {
		cgs.cursorX = cgs.screenXmin;
	}
	else if ( cgs.cursorX > cgs.screenXmax )
	{
		cgs.cursorX = cgs.screenXmax;
	}

	if ( cgs.cursorY < cgs.screenYmin )
    {
		cgs.cursorY = cgs.screenYmin;
	}
	else if ( cgs.cursorY > cgs.screenYmax )
	{
		cgs.cursorY = cgs.screenYmax;
	}
}
