// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================
SOUND OPTIONS MENU
=======================================================================
*/

#include "ui_local.h"

#define ART_BACK0			"gfx/ui/back_0"
#define ART_BACK1			"gfx/ui/back_1"

#define ID_GRAPHICS			10
#define ID_DISPLAY			11
#define ID_SOUND			12
#define ID_NETWORK			13
#define ID_EFFECTSVOLUME	14
#define ID_MUSICVOLUME		15
#define ID_QUALITY			16
#define ID_BACK				18
#define ART_FRAME			"gfx/ui/frame"


static const char *quality_items[] =
{
	"Low",
	"High",
	0
};

typedef struct
{
	menuframework_s		menu;

	menubitmap_s		frame;
	menutext_s			graphics;
	menutext_s			display;
	menutext_s			sound;
	menutext_s			network;

	menuslider_s		sfxvolume;
	menuslider_s		musicvolume;
	menulist_s			quality;

	menubitmap_s		back;
} soundOptionsInfo_t;

static soundOptionsInfo_t	soundOptionsInfo;


/*
=================
UI_SoundOptionsMenu_Event
=================
*/
static void UI_SoundOptionsMenu_Event( void* ptr, int event )
{
	if( event != QM_ACTIVATED )
    {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_GRAPHICS:
		UI_PopMenu();
		UI_GraphicsOptionsMenu();
		break;

	case ID_DISPLAY:
		UI_PopMenu();
		UI_DisplayOptionsMenu();
		break;

	case ID_SOUND:
		break;

	case ID_NETWORK:
		UI_PopMenu();
		UI_NetworkOptionsMenu();
		break;

	case ID_EFFECTSVOLUME:
		trap_Cvar_SetValue( "s_volume", soundOptionsInfo.sfxvolume.curvalue / 10 );
		break;

	case ID_MUSICVOLUME:
		trap_Cvar_SetValue( "s_musicvolume", soundOptionsInfo.musicvolume.curvalue / 10 );
		break;

	case ID_QUALITY:
		if( soundOptionsInfo.quality.curvalue )
        {
			trap_Cvar_SetValue( "s_khz", 22 );
			trap_Cvar_SetValue( "s_compression", 0 );
		}
		else
		{
			trap_Cvar_SetValue( "s_khz", 11 );
			trap_Cvar_SetValue( "s_compression", 1 );
		}
		UI_ForceMenuOff();
		trap_Cmd_ExecuteText( EXEC_APPEND, "snd_restart\n" );
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	}
}


/*
===============
UI_SoundOptionsMenu_Init
===============
*/
static void UI_SoundOptionsMenu_Init( void )
{
	int	y;

	memset( &soundOptionsInfo, 0, sizeof(soundOptionsInfo) );

	UI_SoundOptionsMenu_Cache();
	soundOptionsInfo.menu.wrapAround = qtrue;
	soundOptionsInfo.menu.fullscreen = qfalse;
	
	soundOptionsInfo.frame.generic.type		= MTYPE_BITMAP;
	soundOptionsInfo.frame.generic.flags	= QMF_INACTIVE;
	soundOptionsInfo.frame.generic.name		= ART_FRAME;
	soundOptionsInfo.frame.width			= 640;
	soundOptionsInfo.frame.height			= 480;
	soundOptionsInfo.frame.generic.x		= 0;
	soundOptionsInfo.frame.generic.y		= 0;

	soundOptionsInfo.graphics.generic.type		= MTYPE_PTEXT;
	soundOptionsInfo.graphics.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	soundOptionsInfo.graphics.generic.id		= ID_GRAPHICS;
	soundOptionsInfo.graphics.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.graphics.generic.x			= 216;
	soundOptionsInfo.graphics.generic.y			= 240 - 2 * PROP_HEIGHT;
	soundOptionsInfo.graphics.string			= "GRAPHICS";
	soundOptionsInfo.graphics.style				= UI_RIGHT;
	soundOptionsInfo.graphics.color				= color_white;

	soundOptionsInfo.display.generic.type		= MTYPE_PTEXT;
	soundOptionsInfo.display.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	soundOptionsInfo.display.generic.id			= ID_DISPLAY;
	soundOptionsInfo.display.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.display.generic.x			= 216;
	soundOptionsInfo.display.generic.y			= 240 - PROP_HEIGHT;
	soundOptionsInfo.display.string				= "DISPLAY";
	soundOptionsInfo.display.style				= UI_RIGHT;
	soundOptionsInfo.display.color				= color_white;

	soundOptionsInfo.sound.generic.type			= MTYPE_PTEXT;
	soundOptionsInfo.sound.generic.flags		= QMF_RIGHT_JUSTIFY;
	soundOptionsInfo.sound.generic.id			= ID_SOUND;
	soundOptionsInfo.sound.generic.callback		= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.sound.generic.x			= 216;
	soundOptionsInfo.sound.generic.y			= 240;
	soundOptionsInfo.sound.string				= "SOUND";
	soundOptionsInfo.sound.style				= UI_RIGHT;
	soundOptionsInfo.sound.color				= color_white;

	soundOptionsInfo.network.generic.type		= MTYPE_PTEXT;
	soundOptionsInfo.network.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	soundOptionsInfo.network.generic.id			= ID_NETWORK;
	soundOptionsInfo.network.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.network.generic.x			= 216;
	soundOptionsInfo.network.generic.y			= 240 + PROP_HEIGHT;
	soundOptionsInfo.network.string				= "NETWORK";
	soundOptionsInfo.network.style				= UI_RIGHT;
	soundOptionsInfo.network.color				= color_white;

	y = 240 - 1.5 * (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.sfxvolume.generic.type		= MTYPE_SLIDER;
	soundOptionsInfo.sfxvolume.generic.name		= "Effects Volume:";
	soundOptionsInfo.sfxvolume.generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	soundOptionsInfo.sfxvolume.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.sfxvolume.generic.id		= ID_EFFECTSVOLUME;
	soundOptionsInfo.sfxvolume.generic.x		= 400;
	soundOptionsInfo.sfxvolume.generic.y		= y;
	soundOptionsInfo.sfxvolume.minvalue			= 0;
	soundOptionsInfo.sfxvolume.maxvalue			= 10;

	y += BIGCHAR_HEIGHT+2;
	soundOptionsInfo.musicvolume.generic.type		= MTYPE_SLIDER;
	soundOptionsInfo.musicvolume.generic.name		= "Music Volume:";
	soundOptionsInfo.musicvolume.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	soundOptionsInfo.musicvolume.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.musicvolume.generic.id			= ID_MUSICVOLUME;
	soundOptionsInfo.musicvolume.generic.x			= 400;
	soundOptionsInfo.musicvolume.generic.y			= y;
	soundOptionsInfo.musicvolume.minvalue			= 0;
	soundOptionsInfo.musicvolume.maxvalue			= 10;

	y += BIGCHAR_HEIGHT+2;
	soundOptionsInfo.quality.generic.type		= MTYPE_SPINCONTROL;
	soundOptionsInfo.quality.generic.name		= "Sound Quality:";
	soundOptionsInfo.quality.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	soundOptionsInfo.quality.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.quality.generic.id			= ID_QUALITY;
	soundOptionsInfo.quality.generic.x			= 400;
	soundOptionsInfo.quality.generic.y			= y;
	soundOptionsInfo.quality.itemnames			= quality_items;

	soundOptionsInfo.back.generic.type			= MTYPE_BITMAP;
	soundOptionsInfo.back.generic.name			= ART_BACK0;
	soundOptionsInfo.back.generic.flags			= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	soundOptionsInfo.back.generic.callback		= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.back.generic.id			= ID_BACK;
	soundOptionsInfo.back.generic.x				= 0;
	soundOptionsInfo.back.generic.y				= 480-64;
	soundOptionsInfo.back.width					= 128;
	soundOptionsInfo.back.height				= 64;
	soundOptionsInfo.back.focuspic				= ART_BACK1;

	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.frame );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.graphics );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.display );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.sound );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.network );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.sfxvolume );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.musicvolume );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.quality );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.back );

	soundOptionsInfo.sfxvolume.curvalue = trap_Cvar_VariableValue( "s_volume" ) * 10;
	soundOptionsInfo.musicvolume.curvalue = trap_Cvar_VariableValue( "s_musicvolume" ) * 10;
	soundOptionsInfo.quality.curvalue = !trap_Cvar_VariableValue( "s_compression" );

}


/*
===============
UI_SoundOptionsMenu_Cache
===============
*/
void UI_SoundOptionsMenu_Cache( void )
{
	trap_R_RegisterShaderNoMip( ART_FRAME);
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
}


/*
===============
UI_SoundOptionsMenu
===============
*/
void UI_SoundOptionsMenu( void )
{
	UI_SoundOptionsMenu_Init();
	UI_PushMenu( &soundOptionsInfo.menu );
	Menu_SetCursorToItem( &soundOptionsInfo.menu, &soundOptionsInfo.sound );
}
