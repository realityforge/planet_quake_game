/*
=======================================================================
INGAME MENU
=======================================================================
*/

#include "ui_local.h"

#define ID_CALLVOTE				8
#define ID_STATS				9
#define ID_JOINGAME				10
#define ID_SPECTATE				11
#define ID_SETUP				13
#define ID_SERVERINFO			14
#define ID_LEAVEARENA			15
#define ID_RESTART				16
#define ID_QUIT					17
#define ID_RESUME				18

#define ID_FRAME				20
#define FRAME_ART				"gfx/ui/frame"

typedef struct {
	menuframework_s	menu;

	menubitmap_s	frame;
	menutext_s		joingame;
	menutext_s		stats;
	menutext_s		callvote;
	menutext_s		spectate;
	menutext_s		setup;
	menutext_s		server;
	menutext_s		leave;
	menutext_s		restart;
	menutext_s		quit;
	menutext_s		resume;

} ingamemenu_t;
static ingamemenu_t	s_ingame;

/*
=================
InGame_LeaveArenaAction
=======================
*/
static void InGame_LeaveArenaAction( qbool result ) {
	if( !result ) {
		return;
	}

	UI_PopMenu();
	trap_Cmd_ExecuteText( EXEC_APPEND, "disconnect\n" );
}

/*
=================
InGame_RestartAction
=================
*/
static void InGame_RestartAction( qbool result ) {
	if( !result ) {
		return;
	}

	UI_PopMenu();
	trap_Cmd_ExecuteText( EXEC_APPEND, "map_restart 0\n" );
}


/*
=================
InGame_QuitAction
=================
*/
static void InGame_QuitAction( qbool result ) {
	if( !result ) {
		return;
	}
    trap_Cmd_ExecuteText( EXEC_APPEND, "quit\n" );
}


/*
=================
InGame_Event
=================
*/
void InGame_Event( void *ptr, int notification ) {
	if( notification != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {

    case ID_JOINGAME:
		UI_IngameJoinMenu();
		break;

	case ID_SPECTATE:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cmd team spectator\n" );
		UI_ForceMenuOff();
		break;

	case ID_SETUP:
		UI_SetupMenu();
		break;

	case ID_LEAVEARENA:
	    UI_ConfirmMenu( "TO MAIN MENU?", (voidfunc_f)0, InGame_LeaveArenaAction );
		break;

	case ID_RESTART:
		UI_ConfirmMenu( "RESTART MAP?", (voidfunc_f)0, InGame_RestartAction );
		break;

	case ID_QUIT:
		UI_ConfirmMenu( "EXIT GAME?",  (voidfunc_f)0, InGame_QuitAction );
		break;

	case ID_SERVERINFO:
		UI_ServerInfoMenu();
		break;

    case ID_STATS:
        UI_StatsMenu();
        break;
    case ID_CALLVOTE:
        UI_CallvoteMenu();
        break;
	case ID_RESUME:
		UI_PopMenu();
		break;
	}
}


/*
=================
InGame_MenuInit
=================
*/
void InGame_MenuInit( void )
{
	uiClientState_t	cs;
	gametype_t      gametype;
	char	info[MAX_INFO_STRING];
	int		team;
	int		x, y;

	memset( &s_ingame, 0 ,sizeof(ingamemenu_t) );

	s_ingame.menu.wrapAround = qtrue;
	s_ingame.menu.fullscreen = qfalse;

	// SLK: get current team we are in
	trap_GetClientState( &cs );
	trap_GetConfigString( CS_PLAYERS + cs.clientNum, info, MAX_INFO_STRING );
	team = atoi( Info_ValueForKey( info, "t" ) );

	// SLK: load our frame
	trap_R_RegisterShaderNoMip( FRAME_ART );

	y = 132;

	s_ingame.frame.generic.type		= MTYPE_BITMAP;
	s_ingame.frame.generic.flags	= QMF_INACTIVE;
	s_ingame.frame.generic.name		= FRAME_ART;
	s_ingame.frame.width			= 300;
	s_ingame.frame.height			= 264;
	s_ingame.frame.generic.x		= (640-s_ingame.frame.width)/2;
	s_ingame.frame.generic.y		= (480-s_ingame.frame.height)/2;

	s_ingame.joingame.generic.type     = MTYPE_PTEXT;
	s_ingame.joingame.generic.flags    = QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.joingame.generic.id       = ID_JOINGAME;
	s_ingame.joingame.generic.callback = InGame_Event;
	s_ingame.joingame.generic.x        = 320;
	s_ingame.joingame.generic.y        = y;
	s_ingame.joingame.string           = "JOIN GAME";
	s_ingame.joingame.color            = text_color_normal;
	s_ingame.joingame.style            = UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

	y += 23;
	s_ingame.spectate.generic.type     = MTYPE_PTEXT;
	s_ingame.spectate.generic.flags    = QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.spectate.generic.id       = ID_SPECTATE;
	s_ingame.spectate.generic.callback = InGame_Event;
	s_ingame.spectate.generic.x        = 320;
	s_ingame.spectate.generic.y        = y;
	s_ingame.spectate.string           = "SPECTATE";
	s_ingame.spectate.color            = text_color_normal;
	s_ingame.spectate.style            = UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

	y += 23;
	s_ingame.setup.generic.type			= MTYPE_PTEXT;
	s_ingame.setup.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.setup.generic.x			= 320;
	s_ingame.setup.generic.y			= y;
	s_ingame.setup.generic.id			= ID_SETUP;
	s_ingame.setup.generic.callback		= InGame_Event;
	s_ingame.setup.string				= "SETTINGS";
	s_ingame.setup.color				= text_color_normal;
	s_ingame.setup.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

	y += 23;
	s_ingame.server.generic.type		= MTYPE_PTEXT;
	s_ingame.server.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.server.generic.x			= 320;
	s_ingame.server.generic.y			= y;
	s_ingame.server.generic.id			= ID_SERVERINFO;
	s_ingame.server.generic.callback	= InGame_Event;
	s_ingame.server.string				= "SERVERINFO";
	s_ingame.server.color				= text_color_normal;
	s_ingame.server.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

	y += 23;
	s_ingame.restart.generic.type		= MTYPE_PTEXT;
	s_ingame.restart.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.restart.generic.x			= 320;
	s_ingame.restart.generic.y			= y;
	s_ingame.restart.generic.id			= ID_RESTART;
	s_ingame.restart.generic.callback	= InGame_Event;
	s_ingame.restart.string				= "RESTART";
	s_ingame.restart.color				= text_color_normal;
	s_ingame.restart.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

	y += 23;
	s_ingame.stats.generic.type		    = MTYPE_PTEXT;
	s_ingame.stats.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.stats.generic.x			= 320;
	s_ingame.stats.generic.y			= y;
	s_ingame.stats.generic.id			= ID_STATS;
	s_ingame.stats.generic.callback	    = InGame_Event;
	s_ingame.stats.string				= "STATS";
	s_ingame.stats.color				= text_color_normal;
	s_ingame.stats.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

	y += 23;
	s_ingame.callvote.generic.type	    = MTYPE_PTEXT;
	s_ingame.callvote.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.callvote.generic.x			= 320;
	s_ingame.callvote.generic.y			= y;
	s_ingame.callvote.generic.id		= ID_CALLVOTE;
	s_ingame.callvote.generic.callback	= InGame_Event;
	s_ingame.callvote.string			= "CALLVOTE";
	s_ingame.callvote.color				= text_color_normal;
	s_ingame.callvote.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

	y += 23;
	s_ingame.resume.generic.type		= MTYPE_PTEXT;
	s_ingame.resume.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.resume.generic.x			= 320;
	s_ingame.resume.generic.y			= y;
	s_ingame.resume.generic.id			= ID_RESUME;
	s_ingame.resume.generic.callback	= InGame_Event;
	s_ingame.resume.string				= "RESUME";
	s_ingame.resume.color				= text_color_normal;
	s_ingame.resume.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

	y += 23;
	s_ingame.leave.generic.type			= MTYPE_PTEXT;
	s_ingame.leave.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.leave.generic.x			= 320;
	s_ingame.leave.generic.y			= y;
	s_ingame.leave.generic.id			= ID_LEAVEARENA;
	s_ingame.leave.generic.callback		= InGame_Event;
	s_ingame.leave.string				= "DISCONNECT";
	s_ingame.leave.color				= text_color_normal;
	s_ingame.leave.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

	y += 23;
	s_ingame.quit.generic.type			= MTYPE_PTEXT;
	s_ingame.quit.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ingame.quit.generic.x				= 320;
	s_ingame.quit.generic.y				= y;
	s_ingame.quit.generic.id			= ID_QUIT;
	s_ingame.quit.generic.callback		= InGame_Event;
	s_ingame.quit.string				= "QUIT";
	s_ingame.quit.color					= text_color_normal;
	s_ingame.quit.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

    // SLK: serverinfo and callvote only when connected to a server
	if ( trap_Cvar_VariableValue( "sv_running" )) {
		s_ingame.server.generic.flags  |= QMF_GRAYED;
		s_ingame.callvote.generic.flags|= QMF_GRAYED;
	}

	// SLK: restart map only in GT_ADVENTURE without callvote
	if( trap_Cvar_VariableValue( "dfx_gametype") != GT_ADVENTURE )
        s_ingame.restart.generic.flags |= QMF_GRAYED;

	// SLK: Gray out the current Teams position
	switch ( team ){
    case TEAM_FREE:
        s_ingame.joingame.generic.flags |= QMF_GRAYED;
        break;

    case TEAM_SPECTATOR:
        s_ingame.spectate.generic.flags |= QMF_GRAYED;
		s_ingame.callvote.generic.flags |= QMF_GRAYED;
        break;
	}

	Menu_AddItem( &s_ingame.menu, (void*) &s_ingame.frame );
	Menu_AddItem( &s_ingame.menu, (void*) &s_ingame.joingame );
    Menu_AddItem( &s_ingame.menu, (void*) &s_ingame.spectate );
	Menu_AddItem( &s_ingame.menu, (void*) &s_ingame.setup );
	Menu_AddItem( &s_ingame.menu, (void*) &s_ingame.server );
	Menu_AddItem( &s_ingame.menu, (void*) &s_ingame.stats );
	Menu_AddItem( &s_ingame.menu, (void*) &s_ingame.callvote );
	Menu_AddItem( &s_ingame.menu, (void*) &s_ingame.restart );
	Menu_AddItem( &s_ingame.menu, (void*) &s_ingame.resume );
	Menu_AddItem( &s_ingame.menu, (void*) &s_ingame.leave );
	Menu_AddItem( &s_ingame.menu, (void*) &s_ingame.quit );
}

/*
=================
UI_InGameMenu
=================
*/
void UI_InGameMenu( void ) {
	// force as top level menu
	uis.menusp = 0;

	// set menu cursor to a nice location
	uis.cursorx = 40.0;
	uis.cursory = 16.0;

	InGame_MenuInit();
	UI_PushMenu( &s_ingame.menu );
}
