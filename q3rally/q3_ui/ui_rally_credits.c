/*
**  	Copyright (C) 2004 by the Q3Rally Development team
**  	All rights reserved.
**
**  	ui_rally_credits.c
**
**  	Adds q3rally specific credits menu.
**
**      Author: STONELANCE
**
*/


#include "ui_local.h"


typedef struct {
	menuframework_s	menu;
} rallycreditsmenu_t;

static rallycreditsmenu_t	s_rally_credits;


/*
=================
UI_CreditMenu_Key
=================
*/
static sfxHandle_t UI_Rally_CreditMenu_Key( int key ) {
	if( key & K_CHAR_FLAG ) {
		return 0;
	}

	trap_Cmd_ExecuteText( EXEC_APPEND, "quit\n" );
	return 0;
}


/*
===============
UI_CreditMenu_Draw
===============
*/
static void UI_Rally_CreditMenu_Draw( void ) {
	int		y;

	y = (480 - (22 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE)) / 2;
	UI_DrawProportionalString( 320, y, "Quake III Rally is brought to you by:", UI_CENTER|UI_SMALLFONT, color_white );


	y += 2.0 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Programming:", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Steve \"Stone Lance\" Heijster", UI_CENTER|UI_SMALLFONT, color_white );

	y += 2.0 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Art and Models:", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Jeff \"Stecki\" Garstecki", UI_CENTER|UI_SMALLFONT, color_white );

	y += 2.0 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Maps:", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Jeff \"Stecki\" Garstecki", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Jim \"gout\" Bahe", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Simon \"System Krash\" Batty", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Jonathan \"Amphetamine\" Garrod", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Michael \"Cyberdemon\" Kaminsky", UI_CENTER|UI_SMALLFONT, color_white );
//	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
//	UI_DrawProportionalString( 320, y, "Phillip \"Killazontherun\" Basil-Jones", UI_CENTER|UI_SMALLFONT, color_white );

	y += 2.0 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "SDK Document Design and Layout:", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Richard Smith", UI_CENTER|UI_SMALLFONT, color_white );

	y += 2.0 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Special thanks to:", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Cyberdemon, Killaz and skw|d", UI_CENTER|UI_SMALLFONT, color_white );

	y += 2.0 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawString( 320, y, GAME_VERSION " (c) 2002 Team Quake III Rally | WWW.QUAKERALLY.COM", UI_CENTER|UI_SMALLFONT, text_color_normal );
}


/*
===============
UI_Rally_CreditMenu
===============
*/
void UI_Rally_CreditMenu( void ) {
// STONELANCE
	uis.transitionIn = 0;
// END

	memset( &s_rally_credits, 0 ,sizeof(s_rally_credits) );

	s_rally_credits.menu.draw = UI_Rally_CreditMenu_Draw;
	s_rally_credits.menu.key = UI_Rally_CreditMenu_Key;
	s_rally_credits.menu.fullscreen = qtrue;
	UI_PushMenu ( &s_rally_credits.menu );
}
