// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

CREDITS

=======================================================================
*/


#include "ui_local.h"
#define MAIN_BANNER_MODEL				"models/mapobjects/banner/banner5.md3"


typedef struct {
	menuframework_s	menu;
	menutext_s		banner;

	qhandle_t		titlepic;
} creditsmenu_t;

static creditsmenu_t	s_credits;


/*
=================
UI_CreditMenu_Key
=================
*/
static sfxHandle_t UI_CreditMenu_Key( int key ) {
	if( key & K_CHAR_FLAG ) {
		return 0;
	}

	trap_Cmd_ExecuteText( EXEC_APPEND, "quit\n" );
	return 0;
}

/*
=================
CreditsMenu_Cache
=================
*/
void CreditsMenu_Cache( void ) {
	s_credits.titlepic = trap_R_RegisterShaderNoMip( "corkscrew_gfx/titlepic" );
}


/*
===============
UI_CreditMenu_Draw
===============
*/
static void UI_CreditMenu_Draw( void ) {
	int			y;
	vec4_t		color;

	color[3] = 1;

	UI_DrawHandlePic( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.menuBackShader );

	UI_GetTrippyColor( 0, color );
	trap_R_SetColor( color );
	UI_DrawHandlePic( 64, 0, 512, 128, s_credits.titlepic );

	y = 100;

	y += 3 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Programming, art and sounds:", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW|UI_DROPSHADOW, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;

	UI_GetTrippyColor( 1500, color );
	UI_DrawProportionalString( 320, y, "Firestarter", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, color );

	y += 1.65 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Beta testers:", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;

	UI_GetTrippyColor( 3000, color );
	UI_DrawProportionalString( 320, y, "SiCdeth and MeatBall", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, color );

	y += 1.65 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Special thanks to:", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;

	UI_GetTrippyColor( 4500, color );
	UI_DrawProportionalString( 320, y, "PHHucker and L_Dogg", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, color );

	y += 2*(PROP_HEIGHT * PROP_SMALL_SIZE_SCALE);

	UI_DrawProportionalString( 320, y, "For updates go to:", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "http://www.planetquake.com/fire", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, color_white );

	y += 1.65 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "For support and feedback mail Firestarter:", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "fire@planetquake.com", UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, color_white );
}


/*
===============
UI_CreditMenu
===============
*/
void UI_CreditMenu( void ) {
	memset( &s_credits, 0 ,sizeof(s_credits) );

	s_credits.menu.draw = UI_CreditMenu_Draw;
	s_credits.menu.key = UI_CreditMenu_Key;
	s_credits.menu.fullscreen = qtrue;
	CreditsMenu_Cache();	// the titlepic should alredy be loaded
	UI_PushMenu ( &s_credits.menu );
}
