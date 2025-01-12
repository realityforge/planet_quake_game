/*
**  	Copyright (C) 2004 by the Q3Rally Development team
**  	All rights reserved.
**
**  	ui_rally_gfxloading.c
**
**  	Initial loading menu.
**
**      Author: STONELANCE
**
*/

#include "ui_local.h"

typedef struct {
	menuframework_s	menu;

	playerInfo_t	playerinfo;
	int				currentCache;
	float			loadPercent;
} gfxloading_t;

static gfxloading_t s_gfxloading;


/*
===============
UI_GFX_Loading_HandleCaching
===============
*/
#define NUM_OF_CACHES		7
static void UI_GFX_Loading_HandleCaching( void ) {
	char	model[MAX_QPATH];
	char	rim[MAX_QPATH];
	char	head[MAX_QPATH];
	char	plate[MAX_QPATH];

	switch (s_gfxloading.currentCache){
	case 0:
		s_gfxloading.loadPercent = 0.0f;
		s_gfxloading.currentCache++;
		break;
/*
	case 1:
		MainMenu_Cache();
		s_gfxloading.loadPercent += 1.0f / NUM_OF_CACHES;
		s_gfxloading.currentCache++;
		break;
*/
	case 1:
		UI_SetupMenu_Cache();
		s_gfxloading.loadPercent += 1.0f / NUM_OF_CACHES;
		s_gfxloading.currentCache++;
		break;
	case 2:
		PlayerModel_Cache();
		s_gfxloading.loadPercent += 1.0f / NUM_OF_CACHES;
		s_gfxloading.currentCache++;
		break;
	case 3:
		PlayerSettings_Cache();
		s_gfxloading.loadPercent += 1.0f / NUM_OF_CACHES;
		s_gfxloading.currentCache++;
		break;
	case 4:
		Controls_Cache();
		s_gfxloading.loadPercent += 1.0f / NUM_OF_CACHES;
		s_gfxloading.currentCache++;
		break;
	case 5:
		ArenaServers_Cache();
		s_gfxloading.loadPercent += 1.0f / NUM_OF_CACHES;
		s_gfxloading.currentCache++;
		break;
	case 6:
		trap_Cvar_VariableStringBuffer( "model", model, sizeof( model ) );
		trap_Cvar_VariableStringBuffer( "rim", rim, sizeof( rim ) );
		trap_Cvar_VariableStringBuffer( "head", head, sizeof( head ) );
		trap_Cvar_VariableStringBuffer( "plate", plate, sizeof( plate ) );
		UI_PlayerInfo_SetModel( &s_gfxloading.playerinfo, model, rim, head, plate );
		s_gfxloading.loadPercent += 1.0f / NUM_OF_CACHES;
		s_gfxloading.currentCache++;
		break;
	default:
	case 7:
		StartServer_Cache();
		s_gfxloading.loadPercent = 1.0f;
		s_gfxloading.currentCache++;
		break;
	}
}


/*
===============
UI_GFX_Loading_MenuDraw
===============
*/
static void UI_GFX_Loading_MenuDraw( void ) {
	Menu_Draw( &s_gfxloading.menu );

	UI_GFX_Loading_HandleCaching();

	UI_DrawString( 330, 200, "Loading Graphics:", UI_CENTER, text_color_normal );

	UI_FillRect( 215, 225, 210, 30, menu_back_color );
	UI_FillRect( 220, 230, 200 * s_gfxloading.loadPercent, 20, text_color_highlight );
	UI_DrawString( 320, 240 - SMALLCHAR_HEIGHT/2, va("%0.1f...", s_gfxloading.loadPercent*100), UI_CENTER|UI_SMALLFONT, text_color_normal );

	if (s_gfxloading.loadPercent >= 1.0f){
		UI_PopMenu();
		UI_MainMenu();
	}
}


/*
===============
UI_GFX_Loading
===============
*/
void UI_GFX_Loading( void ) {

	memset( &s_gfxloading, 0 ,sizeof(gfxloading_t) );

	s_gfxloading.menu.draw = UI_GFX_Loading_MenuDraw;
	s_gfxloading.menu.fullscreen = qtrue;

	uis.menusp = 0;

	UI_PushMenu ( &s_gfxloading.menu );
	m_entersound = qfalse;

	s_gfxloading.currentCache = 0;
}
