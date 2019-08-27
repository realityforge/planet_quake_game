//
// SETUP MENU

#include "ui_local.h"

#define ID_CUSTOMIZEPLAYER		10
#define ID_CUSTOMIZECONTROLS	11
#define ID_SYSTEMCONFIG			12
#define ID_GAME					13
#define ID_BACK					15
#define ID_FRAME				16
#define FRAME_ART				"gfx/ui/frame"

typedef struct
{
	menuframework_s	menu;
	menubitmap_s	frame;
	menutext_s		setupplayer;
	menutext_s		setupcontrols;
	menutext_s		setupsystem;
	menutext_s		game;
	menutext_s		back;
} setupMenuInfo_t;

static setupMenuInfo_t	setupMenuInfo;


/*
===============
UI_SetupMenu_Event
===============
*/
static void UI_SetupMenu_Event( void *ptr, int event )
{
	if( event != QM_ACTIVATED )
		return;

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_CUSTOMIZEPLAYER:
		UI_PlayerSettingsMenu();
		break;

	case ID_CUSTOMIZECONTROLS:
		UI_ControlsMenu();
		break;

	case ID_SYSTEMCONFIG:
		UI_GraphicsOptionsMenu();
		break;

	case ID_GAME:
		UI_PreferencesMenu();
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	}
}


/*
===============
UI_SetupMenu_Init
===============
*/
static void UI_SetupMenu_Init( void )
{
	int	y;

	trap_R_RegisterShaderNoMip( FRAME_ART );

	memset( &setupMenuInfo, 0, sizeof(setupMenuInfo) );
	setupMenuInfo.menu.wrapAround = qtrue;
	setupMenuInfo.menu.fullscreen = qfalse;

	setupMenuInfo.frame.generic.type			= MTYPE_BITMAP;
	setupMenuInfo.frame.generic.flags			= QMF_INACTIVE;
	setupMenuInfo.frame.generic.name			= FRAME_ART;
	setupMenuInfo.frame.width					= 300;
	setupMenuInfo.frame.height					= 225;
	setupMenuInfo.frame.generic.x				= (640-setupMenuInfo.frame.width)/2;
	setupMenuInfo.frame.generic.y				= (480-setupMenuInfo.frame.height)/2;

	y = 142;
	setupMenuInfo.setupplayer.generic.type			= MTYPE_PTEXT;
	setupMenuInfo.setupplayer.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	setupMenuInfo.setupplayer.generic.x				= 320;
	setupMenuInfo.setupplayer.generic.y				= y;
	setupMenuInfo.setupplayer.generic.id			= ID_CUSTOMIZEPLAYER;
	setupMenuInfo.setupplayer.generic.callback		= UI_SetupMenu_Event;
	setupMenuInfo.setupplayer.string				= "PLAYER";
	setupMenuInfo.setupplayer.color					= text_color_normal;
	setupMenuInfo.setupplayer.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

	y += 23;
	setupMenuInfo.setupcontrols.generic.type		= MTYPE_PTEXT;
	setupMenuInfo.setupcontrols.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	setupMenuInfo.setupcontrols.generic.x			= 320;
	setupMenuInfo.setupcontrols.generic.y			= y;
	setupMenuInfo.setupcontrols.generic.id			= ID_CUSTOMIZECONTROLS;
	setupMenuInfo.setupcontrols.generic.callback	= UI_SetupMenu_Event;
	setupMenuInfo.setupcontrols.string				= "CONTROLS";
	setupMenuInfo.setupcontrols.color				= text_color_normal;
	setupMenuInfo.setupcontrols.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

	y += 23;
	setupMenuInfo.setupsystem.generic.type			= MTYPE_PTEXT;
	setupMenuInfo.setupsystem.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	setupMenuInfo.setupsystem.generic.x				= 320;
	setupMenuInfo.setupsystem.generic.y				= y;
	setupMenuInfo.setupsystem.generic.id			= ID_SYSTEMCONFIG;
	setupMenuInfo.setupsystem.generic.callback		= UI_SetupMenu_Event;
	setupMenuInfo.setupsystem.string				= "SYSTEM";
	setupMenuInfo.setupsystem.color					= text_color_normal;
	setupMenuInfo.setupsystem.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

	y += 23;
	setupMenuInfo.game.generic.type					= MTYPE_PTEXT;
	setupMenuInfo.game.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	setupMenuInfo.game.generic.x					= 320;
	setupMenuInfo.game.generic.y					= y;
	setupMenuInfo.game.generic.id					= ID_GAME;
	setupMenuInfo.game.generic.callback				= UI_SetupMenu_Event;
	setupMenuInfo.game.string						= "GAME OPTIONS";
	setupMenuInfo.game.color						= text_color_normal;
	setupMenuInfo.game.style						= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

	y += 23;
	setupMenuInfo.back.generic.type					= MTYPE_PTEXT;
	setupMenuInfo.back.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	setupMenuInfo.back.generic.x					= 320;
	setupMenuInfo.back.generic.y					= y;
	setupMenuInfo.back.generic.id					= ID_BACK;
	setupMenuInfo.back.generic.callback				= UI_SetupMenu_Event;
	setupMenuInfo.back.string						= "BACK";
	setupMenuInfo.back.color						= text_color_normal;
	setupMenuInfo.back.style						=  UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;

	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.frame );
	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.setupplayer );
	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.setupcontrols );
	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.setupsystem );
	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.game );
	Menu_AddItem( &setupMenuInfo.menu, &setupMenuInfo.back );
}

/*
===============
UI_SetupMenu
===============
*/
void UI_SetupMenu( void )
{
	UI_SetupMenu_Init();
	UI_PushMenu( &setupMenuInfo.menu );
}
