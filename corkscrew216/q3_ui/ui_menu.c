// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

MAIN MENU

=======================================================================
*/


#include "ui_local.h"

#define	ID_ACCURACYCHALLENGE		8
#define ID_RANDOMSKIRMISH		9
#define ID_SINGLEPLAYER			10
#define ID_MULTIPLAYER			11
#define ID_SETUP				12
#define ID_DEMOS				13
#define ID_CINEMATICS			14
#define ID_TEAMARENA			15
#define ID_MODS					16
#define ID_EXIT					17

#define MAIN_BANNER_MODEL				"models/mapobjects/banner/banner5.md3"
#define MAIN_MENU_VERTICAL_SPACING		34


typedef struct {
	menuframework_s	menu;

	menutext_s		randomskirmish;
	menutext_s		accuracyChallenge;

	menutext_s		singleplayer;
	menutext_s		multiplayer;
	menutext_s		setup;
	menutext_s		demos;
	menutext_s		cinematics;
	menutext_s		teamArena;
	menutext_s		mods;
	menutext_s		exit;

	qhandle_t		bannerModel;
	qhandle_t		railgunModel;
	qhandle_t		titlepic;

	char			bots[16][16];
	int				numBots;
} mainmenu_t;


static mainmenu_t s_main;


/*
=================
MainMenu_ExitAction
=================
*/
static void MainMenu_ExitAction( qboolean result ) {
	if( !result ) {
		return;
	}
	UI_PopMenu();
	UI_CreditMenu();
}

/*
=================
UI_RandomSkirmish
=================
*/

qboolean IsDM( char *info ) {
	char	*p, *token;

	p = info;
	while( 1 ) {
		token = COM_ParseExt( &p, qfalse );
		if( token[0] == 0 ) {
			break;
		}

		if( Q_stricmp( token, "ffa" ) == 0 ) {
			return qtrue;
		}
	}

	return qfalse;
}

const char *GetRandomMapInfo( void ) {
	const char	*info;
	int		count, i, numArenas, maplist[MAX_ARENAS];

	count = UI_GetNumArenas();
	numArenas = 0;

	for ( i = 0; i < count; i++ ) {
		info = UI_GetArenaInfoByNumber( i );
		if ( info ) {
			if ( IsDM( Info_ValueForKey( info, "type" ) ) ) {
				maplist[numArenas] = i;
				numArenas++;
			}
		}
	}

	// this is a truly random number, much better than rand()
	i = trap_Milliseconds() % numArenas;

	return UI_GetArenaInfoByNumber( maplist[i] );
}

void SetBots( const char *info ) {
	char	*p;
	char	*bot;
	char	*botInfo;
	char	bots[MAX_INFO_STRING];

	s_main.numBots = 0;

	Q_strncpyz( bots, Info_ValueForKey( info, "bots" ), sizeof(bots) );

	p = &bots[0];
	while( *p && s_main.numBots < 7 ) {
		//skip spaces
		while( *p && *p == ' ' ) {
			p++;
		}
		if( !p ) {
			break;
		}

		// mark start of bot name
		bot = p;

		// skip until space of null
		while( *p && *p != ' ' ) {
			p++;
		}
		if( *p ) {
			*p++ = 0;
		}

		Q_strncpyz( s_main.bots[s_main.numBots], bot, 10 );
		s_main.numBots++;
	}
}

void UI_RandomSkirmish( qboolean inGame ) {
	const char	*info;
	char		*launchString;
	int		fraglimit, timelimit, i;

	s_main.numBots = 0;	// think! this caused levels without bots to be loaded <:(

	while ( !s_main.numBots ) {	// no arenas without bots
		info = GetRandomMapInfo();
		SetBots( info );
	}

	fraglimit = atoi( Info_ValueForKey( info, "fraglimit" ) );
	timelimit = atoi( Info_ValueForKey( info, "timelimit" ) );

	if( !fraglimit && !timelimit ) fraglimit = 20;

	trap_Cvar_SetValue( "fraglimit", fraglimit );
	trap_Cvar_SetValue( "timelimit", timelimit );
	trap_Cvar_SetValue( "g_gametype", 0 );
	trap_Cvar_SetValue( "bot_enable", 1 );
	trap_Cvar_SetValue( "g_botHack", 1 );

	if ( inGame ) {
		launchString = "kick allbots; wait; wait;";
		for ( i = 0; i < s_main.numBots; i++ )
			launchString = va( "%s addbot %s;", launchString, s_main.bots[i] );

		launchString = va( "%s wait; wait; map %s;", launchString, Info_ValueForKey( info, "map" ) );
	} else {
		launchString = va( "map %s; wait; wait;", Info_ValueForKey( info, "map" ) );
		for ( i = 0; i < s_main.numBots; i++ )
			launchString = va( "%s addbot %s;", launchString, s_main.bots[i] );
	}

	// in-game UI will not ask confirmation, and qagame will execute this function on
	// intermission exit, G_Spawn() will set ui_randomskirmishstartup to 0, so we
	// can detect when a new map has been started without using this function
	trap_Cvar_SetValue( "ui_randomskirmish", 1 );
	trap_Cvar_SetValue( "ui_randomskirmishstartup", 1 );

	trap_Cmd_ExecuteText( EXEC_APPEND, launchString );
}


/*
=================
UI_SkirmishByMap
=================
*/
void UI_SkirmishByMap( char *mapname, qboolean inGame ) {
	const char *info;
	char		*launchString;
	int		fraglimit, timelimit, i;

	info = UI_GetArenaInfoByMap( mapname );
	SetBots( info );

	fraglimit = atoi( Info_ValueForKey( info, "fraglimit" ) );
	timelimit = atoi( Info_ValueForKey( info, "timelimit" ) );

	if( !fraglimit && !timelimit ) fraglimit = 20;

	trap_Cvar_SetValue( "fraglimit", fraglimit );
	trap_Cvar_SetValue( "timelimit", timelimit );
	trap_Cvar_SetValue( "g_gametype", 0 );
	trap_Cvar_SetValue( "bot_enable", 1 );
	trap_Cvar_SetValue( "g_botHack", 1 );

	if ( inGame ) {
		launchString = "kick allbots; wait; wait;";
		for ( i = 0; i < s_main.numBots; i++ )
			launchString = va( "%s addbot %s;", launchString, s_main.bots[i] );

		launchString = va( "%s wait; wait; map %s;", launchString, Info_ValueForKey( info, "map" ) );
	} else {
		launchString = va( "map %s; wait; wait;", Info_ValueForKey( info, "map" ) );
		for ( i = 0; i < s_main.numBots; i++ )
			launchString = va( "%s addbot %s;", launchString, s_main.bots[i] );
	}

	trap_Cmd_ExecuteText( EXEC_APPEND, launchString );
}

/*
=================
UI_AccuracyChallenge
=================
*/
void UI_AccuracyChallenge( void ) {
	trap_Cvar_SetValue( "fraglimit", 150 );
	trap_Cvar_SetValue( "timelimit", 0 );
	trap_Cvar_SetValue( "timescale", 1 );
	trap_Cvar_SetValue( "g_gametype", 0 );
	trap_Cvar_SetValue( "ui_accuracychallengestartup", 1 );

	trap_Cmd_ExecuteText( EXEC_APPEND, "; wait; map q3dm17; wait; addbot klesk 3; addbot slash 3; addbot phobos 3; addbot major 3;" );
}

/*
=================
Main_MenuEvent
=================
*/
void Main_MenuEvent (void* ptr, int event) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_RANDOMSKIRMISH:
		UI_RandomSkirmish( qfalse );
		break;

	case ID_ACCURACYCHALLENGE:
		UI_AccuracyChallenge();
		break;

	case ID_SINGLEPLAYER:
		UI_SPLevelMenu();
		break;

	case ID_MULTIPLAYER:
		UI_ArenaServersMenu();
		break;

	case ID_SETUP:
		UI_SetupMenu();
		break;

	case ID_DEMOS:
		UI_DemosMenu();
		break;

	case ID_CINEMATICS:
		UI_CinematicsMenu();
		break;

	case ID_MODS:
		UI_ModsMenu();
		break;

	case ID_TEAMARENA:
		trap_Cvar_Set( "fs_game", "missionpack");
		trap_Cmd_ExecuteText( EXEC_APPEND, "vid_restart;" );
		break;

	case ID_EXIT:
		UI_ConfirmMenu( "EXIT GAME?", NULL, MainMenu_ExitAction );
		break;
	}
}


/*
===============
MainMenu_Cache
===============
*/
void MainMenu_Cache( void ) {
	//s_main.bannerModel = trap_R_RegisterModel( MAIN_BANNER_MODEL );
	s_main.railgunModel = trap_R_RegisterModel( "models/weapons2/railgun/railgun.md3" );
	s_main.titlepic = trap_R_RegisterShaderNoMip( "corkscrew_gfx/titlepic" );
}


/*
===============
Main_MenuDraw
===============
*/
static void Main_MenuDraw( void ) {
	refdef_t		refdef, refdef2;
	refEntity_t		ent;
	vec3_t			origin, angles, color;
	float			adjust;
	float			x, y, w, h;

	// setup the refdef

	memset( &refdef, 0, sizeof( refdef ) );

	refdef.rdflags = RDF_NOWORLDMODEL;

	AxisClear( refdef.viewaxis );

	x = 0;
	y = 240;
	w = 640;
	h = 140;
	UI_AdjustFrom640( &x, &y, &w, &h );
	refdef.x = x;
	refdef.y = y;
	refdef.width = w;
	refdef.height = h;

	adjust = 0;
	refdef.fov_x = 45;
	refdef.fov_y = 8.4375;

	refdef.time = uis.realtime;

	origin[2] = 0;

	trap_R_ClearScene();

	// add the model

	memset( &ent, 0, sizeof(ent) );

	ent.renderfx = RF_LIGHTING_ORIGIN | RF_NOSHADOW;

	VectorSet ( angles, 0, uis.realtime / 100.0, 0 );
	AnglesToAxis( angles, ent.axis );
	ent.hModel = s_main.railgunModel;
	origin[0] = 150;
	origin[1] = 40;
	VectorCopy( origin, ent.origin );
	VectorCopy( origin, ent.lightingOrigin );
	ent.renderfx = RF_LIGHTING_ORIGIN | RF_NOSHADOW;
	VectorCopy( ent.origin, ent.oldorigin );

	UI_GetTrippyColor( 0, color );
	VectorScale( color, 255, ent.shaderRGBA );

	trap_R_AddRefEntityToScene( &ent );

	VectorSet ( angles, 0, 180 - uis.realtime / 100.0, 0 );
	AnglesToAxis( angles, ent.axis );
	origin[0] = 150;
	origin[1] = -40;
	VectorCopy( origin, ent.origin );
	VectorCopy( origin, ent.lightingOrigin );
	VectorCopy( ent.origin, ent.oldorigin );

	UI_GetTrippyColor( 3000, color );
	VectorScale( color, 255, ent.shaderRGBA );

	trap_R_AddRefEntityToScene( &ent );

	trap_R_RenderScene( &refdef );

	// standard menu drawing
/*
	// setup the refdef

	memset( &refdef, 0, sizeof( refdef ) );

	refdef.rdflags = RDF_NOWORLDMODEL;

	AxisClear( refdef.viewaxis );

	x = 0;
	y = 0;
	w = 640;
	h = 120;
	UI_AdjustFrom640( &x, &y, &w, &h );
	refdef.x = x;
	refdef.y = y;
	refdef.width = w;
	refdef.height = h;

	adjust = 0; // JDC: Kenneth asked me to stop this 1.0 * sin( (float)uis.realtime / 1000 );
	refdef.fov_x = 60 + adjust;
	refdef.fov_y = 19.6875 + adjust;

	refdef.time = uis.realtime;

	origin[0] = 300;
	origin[1] = 0;
	origin[2] = -32;

	trap_R_ClearScene();

	// add the model

	memset( &ent, 0, sizeof(ent) );

	adjust = 5.0 * sin( (float)uis.realtime / 5000 );
	VectorSet( angles, 0, 180 + adjust, 0 );
	AnglesToAxis( angles, ent.axis );
	ent.hModel = s_main.bannerModel;
	VectorCopy( origin, ent.origin );
	VectorCopy( origin, ent.lightingOrigin );
	ent.renderfx = RF_LIGHTING_ORIGIN | RF_NOSHADOW;
	VectorCopy( ent.origin, ent.oldorigin );

	trap_R_AddRefEntityToScene( &ent );

	trap_R_RenderScene( &refdef );
*/
	Menu_Draw( &s_main.menu );

	UI_GetTrippyColor( 4500, color );
	trap_R_SetColor( color );

	UI_DrawHandlePic( 64, 0, 512, 128, s_main.titlepic );

	UI_DrawString( 320, 430, "The CorkScrew Mod: www.planetquake.com/fire", UI_CENTER|UI_SMALLFONT, colorWhite );
	UI_DrawString( 320, 455, "Made by Firestarter: fire@planetquake.com", UI_CENTER|UI_SMALLFONT, colorWhite );
}

/*
===============
UI_TeamArenaExists
===============
*/
static qboolean UI_TeamArenaExists( void ) {
	int		numdirs;
	char	dirlist[2048];
	char	*dirptr;
  char  *descptr;
	int		i;
	int		dirlen;

	numdirs = trap_FS_GetFileList( "$modlist", "", dirlist, sizeof(dirlist) );
	dirptr  = dirlist;
	for( i = 0; i < numdirs; i++ ) {
		dirlen = strlen( dirptr ) + 1;
    descptr = dirptr + dirlen;
		if (Q_stricmp(dirptr, "missionpack") == 0) {
			return qtrue;
		}
    dirptr += dirlen + strlen(descptr) + 1;
	}
	return qfalse;
}


/*
===============
UI_MainMenu

The main menu only comes up when not in a game,
so make sure that the attract loop server is down
and that local cinematics are killed
===============
*/
void UI_MainMenu( void ) {
	int		y;
	qboolean teamArena = qfalse;
	int		style = UI_CENTER | UI_DROPSHADOW;

	trap_Cvar_Set( "sv_killserver", "1" );
	trap_Cvar_Set( "g_accuracyChallenge", "0" );
	trap_Cvar_Set( "ui_randomskirmish", "0" );

	/*
	if( !uis.demoversion && !ui_cdkeychecked.integer ) {
		char	key[17];

		trap_GetCDKey( key, sizeof(key) );
		if( trap_VerifyCDKey( key, NULL ) == qfalse ) {
			UI_CDKeyMenu();
			return;
		}
	}
	*/

	memset( &s_main, 0 ,sizeof(mainmenu_t) );

	MainMenu_Cache();

	s_main.menu.draw = Main_MenuDraw;
	s_main.menu.fullscreen = qtrue;
	s_main.menu.wrapAround = qtrue;
	s_main.menu.showlogo = qtrue;

	y = 134;
	s_main.randomskirmish.generic.type		= MTYPE_PTEXT;
	s_main.randomskirmish.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.randomskirmish.generic.x		= 320;
	s_main.randomskirmish.generic.y		= y;
	s_main.randomskirmish.generic.id		= ID_RANDOMSKIRMISH;
	s_main.randomskirmish.generic.callback	= Main_MenuEvent;
	s_main.randomskirmish.string			= "RANDOM SKIRMISH";
	s_main.randomskirmish.color			= color_yellow;
	s_main.randomskirmish.style			= style;


	y += MAIN_MENU_VERTICAL_SPACING;

	s_main.accuracyChallenge.generic.type		= MTYPE_PTEXT;
	s_main.accuracyChallenge.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.accuracyChallenge.generic.x			= 320;
	s_main.accuracyChallenge.generic.y			= y;
	s_main.accuracyChallenge.generic.id			= ID_ACCURACYCHALLENGE;
	s_main.accuracyChallenge.generic.callback	= Main_MenuEvent;
	s_main.accuracyChallenge.string				= "ACCURACY CHALLENGE";
	s_main.accuracyChallenge.color				= color_yellow;
	s_main.accuracyChallenge.style				= style;

	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.singleplayer.generic.type		= MTYPE_PTEXT;
	s_main.singleplayer.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.singleplayer.generic.x			= 320;
	s_main.singleplayer.generic.y			= y;
	s_main.singleplayer.generic.id			= ID_SINGLEPLAYER;
	s_main.singleplayer.generic.callback	= Main_MenuEvent;
	s_main.singleplayer.string				= "SINGLE PLAYER";
	s_main.singleplayer.color				= color_red;
	s_main.singleplayer.style				= style;

	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.multiplayer.generic.type			= MTYPE_PTEXT;
	s_main.multiplayer.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.multiplayer.generic.x			= 320;
	s_main.multiplayer.generic.y			= y;
	s_main.multiplayer.generic.id			= ID_MULTIPLAYER;
	s_main.multiplayer.generic.callback		= Main_MenuEvent;
	s_main.multiplayer.string				= "MULTIPLAYER";
	s_main.multiplayer.color				= color_red;
	s_main.multiplayer.style				= style;

	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.setup.generic.type				= MTYPE_PTEXT;
	s_main.setup.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.setup.generic.x					= 320;
	s_main.setup.generic.y					= y;
	s_main.setup.generic.id					= ID_SETUP;
	s_main.setup.generic.callback			= Main_MenuEvent;
	s_main.setup.string						= "SETUP";
	s_main.setup.color						= color_red;
	s_main.setup.style						= style;

/*
	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.demos.generic.type				= MTYPE_PTEXT;
	s_main.demos.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.demos.generic.x					= 320;
	s_main.demos.generic.y					= y;
	s_main.demos.generic.id					= ID_DEMOS;
	s_main.demos.generic.callback			= Main_MenuEvent;
	s_main.demos.string						= "DEMOS";
	s_main.demos.color						= color_red;
	s_main.demos.style						= style;
*/

	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.cinematics.generic.type			= MTYPE_PTEXT;
	s_main.cinematics.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.cinematics.generic.x				= 320;
	s_main.cinematics.generic.y				= y;
	s_main.cinematics.generic.id			= ID_CINEMATICS;
	s_main.cinematics.generic.callback		= Main_MenuEvent;
	s_main.cinematics.string				= "CINEMATICS";
	s_main.cinematics.color					= color_red;
	s_main.cinematics.style					= style;

/*
	if (UI_TeamArenaExists()) {
		teamArena = qtrue;
		y += MAIN_MENU_VERTICAL_SPACING;
		s_main.teamArena.generic.type			= MTYPE_PTEXT;
		s_main.teamArena.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
		s_main.teamArena.generic.x				= 320;
		s_main.teamArena.generic.y				= y;
		s_main.teamArena.generic.id				= ID_TEAMARENA;
		s_main.teamArena.generic.callback		= Main_MenuEvent;
		s_main.teamArena.string					= "TEAM ARENA";
		s_main.teamArena.color					= color_red;
		s_main.teamArena.style					= style;
	}
*/

	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.mods.generic.type			= MTYPE_PTEXT;
	s_main.mods.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.mods.generic.x				= 320;
	s_main.mods.generic.y				= y;
	s_main.mods.generic.id				= ID_MODS;
	s_main.mods.generic.callback		= Main_MenuEvent;
	s_main.mods.string					= "MODS";
	s_main.mods.color					= color_red;
	s_main.mods.style					= style;

	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.exit.generic.type				= MTYPE_PTEXT;
	s_main.exit.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.exit.generic.x					= 320;
	s_main.exit.generic.y					= y;
	s_main.exit.generic.id					= ID_EXIT;
	s_main.exit.generic.callback			= Main_MenuEvent;
	s_main.exit.string						= "EXIT";
	s_main.exit.color						= color_red;
	s_main.exit.style						= style;

	Menu_AddItem( &s_main.menu,	&s_main.randomskirmish );
	Menu_AddItem( &s_main.menu,	&s_main.accuracyChallenge );
	Menu_AddItem( &s_main.menu,	&s_main.singleplayer );
	Menu_AddItem( &s_main.menu,	&s_main.multiplayer );
	Menu_AddItem( &s_main.menu,	&s_main.setup );
//	Menu_AddItem( &s_main.menu,	&s_main.demos );
	Menu_AddItem( &s_main.menu,	&s_main.cinematics );
	if (teamArena) {
		Menu_AddItem( &s_main.menu,	&s_main.teamArena );
	}
	Menu_AddItem( &s_main.menu,	&s_main.mods );
	Menu_AddItem( &s_main.menu,	&s_main.exit );

	trap_Key_SetCatcher( KEYCATCH_UI );
	uis.menusp = 0;
	UI_PushMenu ( &s_main.menu );
}
