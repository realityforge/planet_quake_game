// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

CREDITS

=======================================================================
*/


#include "ui_local.h"


typedef struct {
	menuframework_s	menu;
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
===============
UI_CreditMenu_Draw
===============
*/
//NT - credits
static void UI_CreditMenu_Draw( void ) {
	int		y;

	y = 12;
	UI_DrawProportionalString( 320, y, "Alternate Fire Mod Version " ALTERNATEFIRE_VERSION, UI_CENTER|UI_SMALLFONT, color_yellow );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "http://www.planetquake.com/alternatefire", UI_CENTER|UI_SMALLFONT, color_yellow );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "The following people made this possible", UI_CENTER|UI_SMALLFONT, color_lightblue );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "with the following contributions:", UI_CENTER|UI_SMALLFONT, color_lightblue );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "\"Bender\": brainstorming", UI_CENTER|UI_SMALLFONT, color_lightblue );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "\"Octovus\": great ideas, bug-", UI_CENTER|UI_SMALLFONT, color_lightblue );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "finding, and consultation", UI_CENTER|UI_SMALLFONT, color_lightblue );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "John \"Ronin{TRIAD}\" Fisher: more great", UI_CENTER|UI_SMALLFONT, color_lightblue );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "ideas and the cool new powerup models", UI_CENTER|UI_SMALLFONT, color_lightblue );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "\"Kell_Dragon\": de-spamming the BFG", UI_CENTER|UI_SMALLFONT, color_lightblue );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "\"Allchemic\": the awesome new logo", UI_CENTER|UI_SMALLFONT, color_lightblue );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "\"Clutz\": ticking proxies", UI_CENTER|UI_SMALLFONT, color_lightblue );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "Mark \"Ebola_One\" Leckie: hologram ideas", UI_CENTER|UI_SMALLFONT, color_lightblue );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "\"Pappy-R\" and PlanetQuake:", UI_CENTER|UI_SMALLFONT, color_lightblue );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "web space and great pimping", UI_CENTER|UI_SMALLFONT, color_lightblue );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "id Software: the game engine and code", UI_CENTER|UI_SMALLFONT, color_lightblue );

	y += 1.42 * PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "a few thousand people: downloading", UI_CENTER|UI_SMALLFONT, color_white );
	y += PROP_HEIGHT * PROP_SMALL_SIZE_SCALE;
	UI_DrawProportionalString( 320, y, "this mod and trying it out!", UI_CENTER|UI_SMALLFONT, color_white );
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
	UI_PushMenu ( &s_credits.menu );
}
