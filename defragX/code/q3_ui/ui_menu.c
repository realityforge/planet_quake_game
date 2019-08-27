// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================
MAIN MENU
=======================================================================
*/

#include "ui_local.h"

#define ID_OFFLINE		10
#define ID_MULTIPLAYER	11
#define ID_DEMOS		13
#define ID_MODS			16
#define ID_EXIT			17
#define FRAME_ART		"gfx/ui/frame"

#define MAIN_MENU_VERTICAL_SPACING		23


typedef struct {
	menuframework_s	menu;

	menutext_s		banner;
	menubitmap_s	frame;
	menutext_s		offline;
	menutext_s		multiplayer;
	menutext_s		demos;
	menutext_s		mods;
	menutext_s		exit;

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
static void MainMenu_ExitAction( qbool result ) {
	if( !result ) {
		return;
	}
    trap_Cmd_ExecuteText( EXEC_APPEND, "quit\n" );
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
	case ID_OFFLINE:
		UI_StartServerMenu( qfalse );
		break;

	case ID_MULTIPLAYER:
		UI_ArenaServersMenu();
		break;

	case ID_DEMOS:
		UI_DemosMenu();
		break;

	case ID_MODS:
		UI_ModsMenu();
		break;

	case ID_EXIT:
	    trap_Cmd_ExecuteText( EXEC_APPEND, "quit\n" );
		break;
	}
}


/*
===============
MainMenu_Cache
===============
*/
void MainMenu_Cache( void ) {
	uis.startTitleMusic = qtrue;
	trap_R_RegisterShaderNoMip( FRAME_ART );
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

	if (strlen(s_errorMessage.errorMessage))
	{
		UI_DrawProportionalString_AutoWrapped( 320, 192, 600, 20, s_errorMessage.errorMessage, UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, menu_text_color );
	}
	else
	{
		// standard menu drawing
		Menu_Draw( &s_main.menu );
	}

	UI_DrawProportionalString_AutoWrapped( 320, 456, 600, 20, Q3_VERSION, UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorCyan );

	if (uis.startTitleMusic) {
		trap_S_StartBackgroundTrack("music/sonic6.wav", "");
		uis.startTitleMusic = qfalse;
	}

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
	int	y;
	int	style = UI_CENTER | UI_DROPSHADOW | UI_SMALLFONT;

	trap_Cvar_Set( "sv_killserver", "1" );

	memset( &s_main, 0 ,sizeof(mainmenu_t) );
	memset( &s_errorMessage, 0 ,sizeof(errorMessage_t) );

	// com_errorMessage would need that too
	MainMenu_Cache();

	trap_Cvar_VariableStringBuffer( "com_errorMessage", s_errorMessage.errorMessage, sizeof(s_errorMessage.errorMessage) );
	if ( s_errorMessage.errorMessage[0] )
	{
		s_errorMessage.menu.draw = Main_MenuDraw;
		s_errorMessage.menu.key = ErrorMessage_Key;
		s_errorMessage.menu.fullscreen = qtrue;
		s_errorMessage.menu.wrapAround = qtrue;

		trap_Key_SetCatcher( KEYCATCH_UI );
		uis.menusp = 0;
		UI_PushMenu ( &s_errorMessage.menu );

		return;
	}

	s_main.menu.draw			= Main_MenuDraw;
	s_main.menu.fullscreen		= qtrue;
	s_main.menu.wrapAround		= qtrue;

	s_main.banner.generic.type	= MTYPE_BTEXT;
	s_main.banner.generic.x		= 320;
	s_main.banner.generic.y		= UI_BANNER_Y;
	s_main.banner.string		= "DEFRAG X";
	s_main.banner.color			= color_white;
	s_main.banner.style			= UI_CENTER;
	
	s_main.frame.generic.type	= MTYPE_BITMAP;
	s_main.frame.generic.flags	= QMF_INACTIVE;
	s_main.frame.generic.name	= FRAME_ART;
	s_main.frame.width			= 180;
	s_main.frame.height			= 162;
	s_main.frame.generic.x		= (640-s_main.frame.width)/2;
	s_main.frame.generic.y		= (480-s_main.frame.height)/2;

	y = 182;
	s_main.offline.generic.type		        = MTYPE_PTEXT;
	s_main.offline.generic.flags		    = QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.offline.generic.x			    = 320;
	s_main.offline.generic.y			    = y;
	s_main.offline.generic.id			    = ID_OFFLINE;
	s_main.offline.generic.callback	        = Main_MenuEvent;
	s_main.offline.string				    = "PLAY";
	s_main.offline.color				    = text_color_normal;
	s_main.offline.style				    = style;

	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.multiplayer.generic.type			= MTYPE_PTEXT;
	s_main.multiplayer.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.multiplayer.generic.x			= 320;
	s_main.multiplayer.generic.y			= y;
	s_main.multiplayer.generic.id			= ID_MULTIPLAYER;
	s_main.multiplayer.generic.callback		= Main_MenuEvent;
	s_main.multiplayer.string				= "ONLINE";
	s_main.multiplayer.color				= text_color_normal;
	s_main.multiplayer.style				= style;

	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.demos.generic.type				= MTYPE_PTEXT;
	s_main.demos.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.demos.generic.x					= 320;
	s_main.demos.generic.y					= y;
	s_main.demos.generic.id					= ID_DEMOS;
	s_main.demos.generic.callback			= Main_MenuEvent;
	s_main.demos.string						= "DEMOS";
	s_main.demos.color						= text_color_normal;
	s_main.demos.style						= style;

	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.mods.generic.type				= MTYPE_PTEXT;
	s_main.mods.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.mods.generic.x					= 320;
	s_main.mods.generic.y					= y;
	s_main.mods.generic.id					= ID_MODS;
	s_main.mods.generic.callback			= Main_MenuEvent;
	s_main.mods.string						= "MODS";
	s_main.mods.color						= text_color_normal;
	s_main.mods.style						= style;

	y += MAIN_MENU_VERTICAL_SPACING;
	s_main.exit.generic.type				= MTYPE_PTEXT;
	s_main.exit.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.exit.generic.x					= 320;
	s_main.exit.generic.y					= y;
	s_main.exit.generic.id					= ID_EXIT;
	s_main.exit.generic.callback			= Main_MenuEvent;
	s_main.exit.string						= "QUIT";
	s_main.exit.color						= text_color_normal;
	s_main.exit.style						= style;

	Menu_AddItem( &s_main.menu, &s_main.banner );
	Menu_AddItem( &s_main.menu, &s_main.frame );
	Menu_AddItem( &s_main.menu,	&s_main.offline );
	Menu_AddItem( &s_main.menu,	&s_main.multiplayer );
	Menu_AddItem( &s_main.menu,	&s_main.demos );
	Menu_AddItem( &s_main.menu,	&s_main.mods );
	Menu_AddItem( &s_main.menu,	&s_main.exit );

	trap_Key_SetCatcher( KEYCATCH_UI );
	uis.menusp = 0;
	UI_PushMenu ( &s_main.menu );
}
