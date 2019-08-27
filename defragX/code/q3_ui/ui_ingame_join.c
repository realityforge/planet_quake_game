//
// ui_ingame_join.c
//

#include "ui_local.h"

#define JOIN_FRAME		"gfx/ui/frame"
#define ID_FREE			100
#define ID_RED			101
#define ID_BLUE			102
#define ID_GREEN		103
#define ID_YELLOW		104
#define ID_CANCEL		105

typedef struct
{
	menuframework_s	menu;
	menubitmap_s	frame;
	
	menutext_s		free;
	menutext_s		red;
	menutext_s		blue;
	menutext_s		green;
	menutext_s		yellow;
	
	menutext_s		cancel;
} join_t;

static join_t		s_join;


/*
===============
Join_MenuEvent
===============
*/
static void Join_MenuEvent( void* ptr, int event ) {

	if( event != QM_ACTIVATED ) 
		return;

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_FREE:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cmd team free\n" );
		UI_ForceMenuOff();
		break;
	case ID_RED:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cmd team red\n" );
		UI_ForceMenuOff();
		break;
	case ID_BLUE:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cmd team blue\n" );
		UI_ForceMenuOff();
		break;
	case ID_GREEN:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cmd team green\n" );
		UI_ForceMenuOff();
		break;
	case ID_YELLOW:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cmd team yellow\n" );
		UI_ForceMenuOff();
		break;
	case ID_CANCEL:
		UI_PopMenu();
		break;
	}
}

/*
===============
IngameJoin_MenuInit
===============
*/
void IngameJoin_MenuInit( void ) {
	char			info[BIG_INFO_STRING];
	uiClientState_t	cs;
	gametype_t 		gametype;
	int				y, team;

	memset( &s_join, 0, sizeof(s_join) );

	trap_R_RegisterShaderNoMip( JOIN_FRAME );

	s_join.menu.wrapAround = qtrue;
	s_join.menu.fullscreen = qfalse;

	s_join.frame.generic.type			= MTYPE_BITMAP;
	s_join.frame.generic.flags			= QMF_INACTIVE;
	s_join.frame.generic.name			= JOIN_FRAME;
	s_join.frame.width					= 300;
	s_join.frame.height					= 225;
	s_join.frame.generic.x				= (640-s_join.frame.width)/2;
	s_join.frame.generic.y				= (480-s_join.frame.height)/2;

	y = 142;
	s_join.free.generic.type			= MTYPE_PTEXT;
	s_join.free.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_join.free.generic.id				= ID_FREE;
	s_join.free.generic.callback		= Join_MenuEvent;
	s_join.free.generic.x				= 320;
	s_join.free.generic.y				= y;
	s_join.free.string					= "FREE SPAWN";
	s_join.free.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_join.free.color					= text_color_normal;

	y += 46;
	s_join.red.generic.type				= MTYPE_PTEXT;
	s_join.red.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_join.red.generic.id				= ID_RED;
	s_join.red.generic.callback			= Join_MenuEvent;
	s_join.red.generic.x				= 320;
	s_join.red.generic.y				= y;
	s_join.red.string					= "RED SPAWN";
	s_join.red.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_join.red.color					= text_color_normal;

	y += 23;
	s_join.blue.generic.type			= MTYPE_PTEXT;
	s_join.blue.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_join.blue.generic.id				= ID_BLUE;
	s_join.blue.generic.x				= 320;
	s_join.blue.generic.callback		= Join_MenuEvent;
	s_join.blue.generic.y				= y;
	s_join.blue.string					= "BLUE SPAWN";
	s_join.blue.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_join.blue.color					= text_color_normal;

	y += 23;
	s_join.green.generic.type			= MTYPE_PTEXT;
	s_join.green.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_join.green.generic.id				= ID_GREEN;
	s_join.green.generic.callback		= Join_MenuEvent;
	s_join.green.generic.x				= 320;
	s_join.green.generic.y				= y;
	s_join.green.string					= "GREEN SPAWN";
	s_join.green.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_join.green.color					= text_color_normal;

	y += 23;
	s_join.yellow.generic.type			= MTYPE_PTEXT;
	s_join.yellow.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_join.yellow.generic.id			= ID_YELLOW;
	s_join.yellow.generic.callback		= Join_MenuEvent;
	s_join.yellow.generic.x				= 320;
	s_join.yellow.generic.y				= y;
	s_join.yellow.string				= "YELLOW SPAWN";
	s_join.yellow.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_join.yellow.color					= text_color_normal;

	y += 46;
	s_join.cancel.generic.type			= MTYPE_PTEXT;
	s_join.cancel.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_join.cancel.generic.id			= ID_CANCEL;
	s_join.cancel.generic.callback		= Join_MenuEvent;
	s_join.cancel.generic.x				= 320;
	s_join.cancel.generic.y				= y;
	s_join.cancel.string				= "CANCEL";
	s_join.cancel.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_join.cancel.color					= text_color_normal;

	// get the gametype we are in, so we can determine what menuitems to gray out
	trap_GetConfigString( CS_SERVERINFO, info, sizeof( info ) );
	gametype = atoi( Info_ValueForKey( info, "dfx_gametype" ) );

	// get current team we are in
	trap_GetClientState( &cs );
	trap_GetConfigString( CS_PLAYERS + cs.clientNum, info, MAX_INFO_STRING );
	team = atoi( Info_ValueForKey( info, "t" ) );

	if (gametype != GT_TEAMRUN ) {
		s_join.red.generic.flags |= QMF_GRAYED;
		s_join.blue.generic.flags |= QMF_GRAYED;
		s_join.green.generic.flags |= QMF_GRAYED;
		s_join.yellow.generic.flags |= QMF_GRAYED;
	} else {
		s_join.free.generic.flags |= QMF_GRAYED;
	}

	Menu_AddItem( &s_join.menu, (void*) &s_join.frame );
	Menu_AddItem( &s_join.menu, (void*) &s_join.free );
	Menu_AddItem( &s_join.menu, (void*) &s_join.red );
	Menu_AddItem( &s_join.menu, (void*) &s_join.blue );
	Menu_AddItem( &s_join.menu, (void*) &s_join.green );
	Menu_AddItem( &s_join.menu, (void*) &s_join.yellow );
	Menu_AddItem( &s_join.menu, (void*) &s_join.cancel );
}

/*
===============
UI_IngameJoinMenu
===============
*/
void UI_IngameJoinMenu( void ) {
	IngameJoin_MenuInit();
	UI_PushMenu ( &s_join.menu );
}


