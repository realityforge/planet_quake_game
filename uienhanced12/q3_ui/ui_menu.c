// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

MAIN MENU

=======================================================================
*/


/*
The work contained within this file is software written by various
copyright holders.  The initial contributor, Id Software holds all
copyright over their software.  However, software used and written by
and for UI Enhanced has copyrights held by the initial author of the
software.

The changes written by and for UI Enhanced are contained alongside the
original work from Id Software for convenience and ease of interoperability.

For the code contained herein that was written by Id Software, see
the license agreement on their original archive for restrictions and
limitations.

The UI Enhanced copyright owner permit free reuse of his code contained
herein, as long as the following terms are met:

----------------------------------------------------------------------
1) Credit is given in a place where users of the mod may read
   it. (Title screen, credit screen or README will do).  The
   recommended format is: "First, Last, alias, email"

2) There are no attempts to misrepresent the public as to who made the
   alterations.  The UI Enhanced copyright owner does not give permission
   for others to release software under the UI Enhanced name.
----------------------------------------------------------------------

Ian Jefferies   - HypoThermia ( uie@planetquake.com )
http://www.planetquake.com/uie
*/


#include "ui_local.h"


#define ID_SINGLEPLAYER			10
#define ID_MULTIPLAYER			11
#define ID_SETUP				12
#define ID_DEMOS				13
#define ID_CINEMATICS			14
#define ID_TEAMARENA		15
#define ID_MODS					16
#define ID_EXIT					17
#define ID_SKIRMISH             18
#define ID_PLAYERNAME			19
#define ID_MODELTYPE			20

#define MAIN_BANNER_MODEL				"models/mapobjects/banner/banner5.md3"
#define MAIN_MENU_VERTICAL_SPACING		34

#define MAIN_MENU_CENTER 200


vec4_t color_translucent	= {1.0f, 1.0f, 1.0f, 0.2f};



typedef struct {
	menuframework_s	menu;

	menutext_s		singleplayer;
	menutext_s      skirmish;
	menutext_s		multiplayer;
	menutext_s		setup;
	menutext_s		demos;
	menutext_s		cinematics;
	menutext_s		teamArena;
	menutext_s		mods;
	menutext_s		exit;
	menutext_s		name;
	menutext_s		modeltype;

#ifndef NO_UIE_MINILOGO
	menubitmap_s	logo;
#endif

	modelAnim_t 	model;
	char 			playername[MAX_NAME_LENGTH];

	qhandle_t		bannerModel;
} mainmenu_t;


static mainmenu_t s_main;

typedef struct {
	menuframework_s menu;	
	char errorMessage[4096];
} errorMessage_t;

static errorMessage_t s_errorMessage;

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
Main_SetPlayerModelType
=================
*/
static void Main_SetPlayerModelType( void )
{
	if (UIE_PlayerInfo_IsTeamModel())
	{
		s_main.modeltype.string = "Team Model";
	}
	else
	{
		s_main.modeltype.string = "DM Model";
	}

	PText_Init(&s_main.modeltype);
}



/*
=================
Main_ToggleModelType
=================
*/
static void Main_ToggleModelType( void )
{
	qboolean type;

	if (UIE_PlayerInfo_IsTeamModel()) {
		type = qfalse;
	}
	else {
		type = qtrue;
	}

	UIE_PlayerInfo_DrawTeamModel(&s_main.model, type);
	Main_SetPlayerModelType();
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
	case ID_SINGLEPLAYER:
		UI_SPLevelMenu();
		break;

    case ID_SKIRMISH:
        UI_StartServerMenu(qfalse);
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

	case ID_PLAYERNAME:
		UI_PlayerSettingsMenu();
		break;	

	case ID_MODELTYPE:
		Main_ToggleModelType();
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
	s_main.bannerModel = trap_R_RegisterModel( MAIN_BANNER_MODEL );
}

sfxHandle_t ErrorMessage_Key(int key)
{
	trap_Cvar_Set( "com_errorMessage", "" );
	UI_MainMenu();
	return (menu_null_sound);
}

/*
===============
Main_MenuDraw
TTimo: this function is common to the main menu and errorMessage menu
===============
*/

static void Main_MenuDraw( void ) {
	refdef_t		refdef;
	refEntity_t		ent;
	vec3_t			origin;
	vec3_t			angles;
	float			adjust;
	float			x, y, w, h;
	vec4_t			color = {0.5, 0, 0, 1};
	char 			buffer[MAX_NAME_LENGTH];

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

	if (strlen(s_errorMessage.errorMessage))
	{
		UI_DrawProportionalString_AutoWrapped( 320, 192, 600, 20, s_errorMessage.errorMessage, UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, menu_text_color );
	}
	else
	{
      // change the players name if different to control
   
	   trap_Cvar_VariableStringBuffer( "name", buffer, MAX_NAME_LENGTH);
	   Q_CleanStr(buffer);
	   if (strcmp(s_main.playername, buffer)) {
		   strcpy(s_main.playername, buffer);
		   PText_Init(&s_main.name);
	   }
   
	   // update type of model displayed
	   if (uis.firstdraw)
		   Main_SetPlayerModelType();
   
	   // standard menu drawing
	   Menu_Draw( &s_main.menu );
   }

	if (uis.demoversion) {
		UI_DrawString( 320, 420 - 2*SMALLCHAR_HEIGHT - 8, "UI Enhanced "UIE_VERSION" http://www.planetquake.com/uie/", UI_CENTER|UI_SMALLFONT, color );
		UI_DrawProportionalString( 320, 420 - SMALLCHAR_HEIGHT - 4, "DEMO      FOR MATURE AUDIENCES      DEMO", UI_CENTER|UI_SMALLFONT, color );
		UI_DrawString( 320, 400, "Quake III Arena(c) 1999-2000, Id Software, Inc.  All Rights Reserved", UI_CENTER|UI_SMALLFONT, color );
	} else {
		UI_DrawString( 320, 450 - SMALLCHAR_HEIGHT - 4, "UI Enhanced "UIE_VERSION" http://www.planetquake.com/uie/", UI_CENTER|UI_SMALLFONT, color );
		UI_DrawString( 320, 450, "Quake III Arena(c) 1999-2000, Id Software, Inc.  All Rights Reserved", UI_CENTER|UI_SMALLFONT, color );
	}
}


/*
=================
Main_DrawPlayer
=================
*/
static void Main_DrawPlayer( void *self )
{
	UIE_PlayerInfo_AnimateModel(&s_main.model);
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
	int		style;
	float	sizeScale;

	trap_Cvar_Set( "sv_killserver", "1" );

	if( !uis.demoversion && !ui_cdkeychecked.integer ) {
		char	key[17];

		trap_GetCDKey( key, sizeof(key) );
		if( trap_VerifyCDKey( key, NULL ) == qfalse ) {
			UI_CDKeyMenu();
			return;
		}
	}

	memset( &s_main, 0 ,sizeof(mainmenu_t) );
	memset( &s_errorMessage, 0 ,sizeof(errorMessage_t) );

	// com_errorMessage would need that too
	MainMenu_Cache();
	
	trap_Cvar_VariableStringBuffer( "com_errorMessage", s_errorMessage.errorMessage, sizeof(s_errorMessage.errorMessage) );
	if (strlen(s_errorMessage.errorMessage))
	{	
		s_errorMessage.menu.draw = Main_MenuDraw;
		s_errorMessage.menu.key = ErrorMessage_Key;
		s_errorMessage.menu.fullscreen = qtrue;
		s_errorMessage.menu.wrapAround = qtrue;
		s_errorMessage.menu.showlogo = qtrue;		

		trap_Key_SetCatcher( KEYCATCH_UI );
		uis.menusp = 0;
		UI_PushMenu ( &s_errorMessage.menu );
		
		return;
	}

	sizeScale = UI_ProportionalSizeScale( UI_SMALLFONT );
	style = UI_CENTER | UI_DROPSHADOW;

	MainMenu_Cache();

	s_main.menu.draw = Main_MenuDraw;
	s_main.menu.fullscreen = qtrue;
	s_main.menu.wrapAround = qtrue;
	s_main.menu.showlogo = qtrue;

	y = 134;
	s_main.singleplayer.generic.type		= MTYPE_PTEXT;
	s_main.singleplayer.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.singleplayer.generic.x			= MAIN_MENU_CENTER;
	s_main.singleplayer.generic.y			= y;
	s_main.singleplayer.generic.id			= ID_SINGLEPLAYER;
	s_main.singleplayer.generic.callback	= Main_MenuEvent;
	s_main.singleplayer.string				= "SINGLE PLAYER";
	s_main.singleplayer.color				= color_red;
	s_main.singleplayer.style				= style;

	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.skirmish.generic.type			= MTYPE_PTEXT;
	s_main.skirmish.generic.flags	    	= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.skirmish.generic.x			    = MAIN_MENU_CENTER;
	s_main.skirmish.generic.y   			= y;
	s_main.skirmish.generic.id	    		= ID_SKIRMISH;
	s_main.skirmish.generic.callback		= Main_MenuEvent;
	s_main.skirmish.string			    	= "SKIRMISH";
	s_main.skirmish.color				    = color_red;
	s_main.skirmish.style		    		= style;

	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.multiplayer.generic.type			= MTYPE_PTEXT;
	s_main.multiplayer.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.multiplayer.generic.x			= MAIN_MENU_CENTER;
	s_main.multiplayer.generic.y			= y;
	s_main.multiplayer.generic.id			= ID_MULTIPLAYER;
	s_main.multiplayer.generic.callback		= Main_MenuEvent;
	s_main.multiplayer.string				= "MULTIPLAYER";
	s_main.multiplayer.color				= color_red;
	s_main.multiplayer.style				= style;

	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.setup.generic.type				= MTYPE_PTEXT;
	s_main.setup.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.setup.generic.x					= MAIN_MENU_CENTER;
	s_main.setup.generic.y					= y;
	s_main.setup.generic.id					= ID_SETUP;
	s_main.setup.generic.callback			= Main_MenuEvent;
	s_main.setup.string						= "SETUP";
	s_main.setup.color						= color_red;
	s_main.setup.style						= style | UI_SMALLFONT;

	y += MAIN_MENU_VERTICAL_SPACING * sizeScale;
	s_main.demos.generic.type				= MTYPE_PTEXT;
	s_main.demos.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.demos.generic.x					= MAIN_MENU_CENTER;
	s_main.demos.generic.y					= y;
	s_main.demos.generic.id					= ID_DEMOS;
	s_main.demos.generic.callback			= Main_MenuEvent;
	s_main.demos.string						= "DEMOS";
	s_main.demos.color						= color_red;
	s_main.demos.style						= style | UI_SMALLFONT;

	y += MAIN_MENU_VERTICAL_SPACING * sizeScale;
	s_main.cinematics.generic.type			= MTYPE_PTEXT;
	s_main.cinematics.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.cinematics.generic.x				= MAIN_MENU_CENTER;
	s_main.cinematics.generic.y				= y;
	s_main.cinematics.generic.id			= ID_CINEMATICS;
	s_main.cinematics.generic.callback		= Main_MenuEvent;
	s_main.cinematics.string				= "CINEMATICS";
	s_main.cinematics.color					= color_red;
	s_main.cinematics.style					= style | UI_SMALLFONT;

	if (UI_TeamArenaExists()) {
		teamArena = qtrue;
		y += MAIN_MENU_VERTICAL_SPACING * sizeScale;
		s_main.teamArena.generic.type			= MTYPE_PTEXT;
		s_main.teamArena.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
		s_main.teamArena.generic.x				= MAIN_MENU_CENTER;
		s_main.teamArena.generic.y				= y;
		s_main.teamArena.generic.id				= ID_TEAMARENA;
		s_main.teamArena.generic.callback		= Main_MenuEvent;
		s_main.teamArena.string					= "TEAM ARENA";
		s_main.teamArena.color					= color_red;
		s_main.teamArena.style					= style | UI_SMALLFONT;
	}

	y += MAIN_MENU_VERTICAL_SPACING * sizeScale;
	s_main.mods.generic.type			= MTYPE_PTEXT;
	s_main.mods.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.mods.generic.x				= MAIN_MENU_CENTER;
	s_main.mods.generic.y				= y;
	s_main.mods.generic.id				= ID_MODS;
	s_main.mods.generic.callback		= Main_MenuEvent;
	s_main.mods.string					= "MODS";
	s_main.mods.color					= color_red;
	s_main.mods.style					= style | UI_SMALLFONT;

	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.exit.generic.type				= MTYPE_PTEXT;
	s_main.exit.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.exit.generic.x					= MAIN_MENU_CENTER;
	s_main.exit.generic.y					= y;
	s_main.exit.generic.id					= ID_EXIT;
	s_main.exit.generic.callback			= Main_MenuEvent;
	s_main.exit.string						= "EXIT";
	s_main.exit.color						= color_red;
	s_main.exit.style						= style;

	if (!teamArena)
		y += MAIN_MENU_VERTICAL_SPACING;

	s_main.name.generic.type			= MTYPE_PTEXT;
	s_main.name.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.name.generic.x				= 480;
	s_main.name.generic.y				= PLAYERMODEL_TEXTHEIGHT;
	s_main.name.generic.id				= ID_PLAYERNAME;
	s_main.name.generic.callback		= Main_MenuEvent;
	s_main.name.string					= s_main.playername;
	s_main.name.color					= text_color_normal;
	s_main.name.style					= style|UI_SMALLFONT;

	s_main.modeltype.generic.type			= MTYPE_PTEXT;
	s_main.modeltype.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.modeltype.generic.x				= 480;
	s_main.modeltype.generic.y				= PLAYERMODEL_TEXTHEIGHT + (PROP_HEIGHT*sizeScale);
	s_main.modeltype.generic.id				= ID_MODELTYPE;
	s_main.modeltype.generic.callback		= Main_MenuEvent;
	s_main.modeltype.string					= "";
	s_main.modeltype.color					= text_color_normal;
	s_main.modeltype.style					= style|UI_SMALLFONT;

	s_main.model.bitmap.generic.type      = MTYPE_BITMAP;
	s_main.model.bitmap.generic.flags     = QMF_INACTIVE;
	s_main.model.bitmap.generic.ownerdraw = Main_DrawPlayer;
	s_main.model.bitmap.generic.x	        = PLAYERMODEL_X;
	s_main.model.bitmap.generic.y	        = PLAYERMODEL_Y;
	s_main.model.bitmap.width	            = PLAYERMODEL_WIDTH;
	s_main.model.bitmap.height            = PLAYERMODEL_HEIGHT;

#ifndef NO_UIE_MINILOGO
	s_main.logo.generic.type			= MTYPE_BITMAP;
	s_main.logo.generic.flags		= QMF_INACTIVE|QMF_HIGHLIGHT;
	s_main.logo.generic.x			= UIE_LOGO_X;
	s_main.logo.generic.y			= UIE_LOGO_Y;
	s_main.logo.width				= 64;
	s_main.logo.height				= 16;
	s_main.logo.focuspic 			= UIE_LOGO_NAME;
	s_main.logo.focuscolor 			= color_translucent;

	Menu_AddItem( &s_main.menu,	&s_main.logo);
#endif

	Menu_AddItem( &s_main.menu,	&s_main.singleplayer );
	Menu_AddItem( &s_main.menu,	&s_main.skirmish );
	Menu_AddItem( &s_main.menu,	&s_main.multiplayer );
	Menu_AddItem( &s_main.menu,	&s_main.setup );
	Menu_AddItem( &s_main.menu,	&s_main.demos );
	Menu_AddItem( &s_main.menu,	&s_main.cinematics );
	if (teamArena) {
		Menu_AddItem( &s_main.menu,	&s_main.teamArena );
	}
	Menu_AddItem( &s_main.menu,	&s_main.mods );
	Menu_AddItem( &s_main.menu,	&s_main.exit );
	Menu_AddItem( &s_main.menu,	&s_main.name );
	Menu_AddItem( &s_main.menu,	&s_main.modeltype );
	Menu_AddItem( &s_main.menu,	&s_main.model.bitmap);

	// prepare the player model
	UIE_PlayerInfo_InitModel(&s_main.model);

	trap_Key_SetCatcher( KEYCATCH_UI );
	uis.menusp = 0;
	UI_PushMenu ( &s_main.menu );
		
}





