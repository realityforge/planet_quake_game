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
static void UI_CreditMenu_Draw( void ) {
	int		y;

#define titlex	315
#define namex	325

	y = 16;
	UI_DrawString( 320, y, "id Software is:", UI_CENTER|UI_SMALLFONT, color_red );

	y += SMALLCHAR_HEIGHT + SMALLCHAR_WIDTH;
	UI_DrawString( titlex, y, "Programming", UI_RIGHT|UI_SMALLFONT, color_orange );
	UI_DrawString( namex, y, "John Carmack, John Cash", UI_LEFT|UI_SMALLFONT, color_white );

	y += SMALLCHAR_HEIGHT + SMALLCHAR_WIDTH;
	UI_DrawString( titlex, y, "Art", UI_RIGHT|UI_SMALLFONT, color_orange );
	UI_DrawString( namex, y, "Adrian Carmack, Kevin Cloud,", UI_LEFT|UI_SMALLFONT, color_white );
	y += SMALLCHAR_HEIGHT;
	UI_DrawString( namex, y, "Paul Steed, Kenneth Scott", UI_LEFT|UI_SMALLFONT, color_white );

	y += SMALLCHAR_HEIGHT + SMALLCHAR_WIDTH;
	UI_DrawString( titlex, y, "Game Designer", UI_RIGHT|UI_SMALLFONT, color_orange );
	UI_DrawString( namex, y, "Graeme Devine", UI_LEFT|UI_SMALLFONT, color_white );

	y += SMALLCHAR_HEIGHT + SMALLCHAR_WIDTH;
	UI_DrawString( titlex, y, "Level Design", UI_RIGHT|UI_SMALLFONT, color_orange );
	UI_DrawString( namex, y, "Tim Willits, Christian Antkow", UI_LEFT|UI_SMALLFONT, color_white );
	y += SMALLCHAR_HEIGHT;
	UI_DrawString( namex, y, "Paul Jaquays", UI_LEFT|UI_SMALLFONT, color_white );

	y += SMALLCHAR_HEIGHT + SMALLCHAR_WIDTH;
	UI_DrawString( titlex, y, "CEO", UI_RIGHT|UI_SMALLFONT, color_orange );
	UI_DrawString( namex, y, "Todd Hollenshead", UI_LEFT|UI_SMALLFONT, color_white );

	y += SMALLCHAR_HEIGHT + SMALLCHAR_WIDTH;
	UI_DrawString( titlex, y, "Director of Business Development", UI_RIGHT|UI_SMALLFONT, color_orange );
	UI_DrawString( namex, y, "Katherine Anna Kang", UI_LEFT|UI_SMALLFONT, color_white );

	y += SMALLCHAR_HEIGHT + SMALLCHAR_WIDTH;
	UI_DrawString( titlex, y, "Biz Assist and id Mom", UI_RIGHT|UI_SMALLFONT, color_orange );
	UI_DrawString( namex, y, "Donna Jackson", UI_LEFT|UI_SMALLFONT, color_white );

/**/

	y += SMALLCHAR_HEIGHT*2;
	UI_DrawString( 320, y, "Team Reaction is:", UI_CENTER|UI_SMALLFONT, color_red );

	y += SMALLCHAR_HEIGHT + SMALLCHAR_WIDTH;
	UI_DrawString( titlex, y, "Programming", UI_RIGHT|UI_SMALLFONT, color_orange );
	UI_DrawString( namex, y, "BinaryC", UI_LEFT|UI_SMALLFONT, color_white );

	y += SMALLCHAR_HEIGHT + SMALLCHAR_WIDTH;
	UI_DrawString( titlex, y, "Mapping", UI_RIGHT|UI_SMALLFONT, color_orange );
	UI_DrawString( namex, y, "Lifer!, Sul", UI_LEFT|UI_SMALLFONT, color_white );
	y += SMALLCHAR_HEIGHT;
	UI_DrawString( namex, y, "Vansen, Llurk", UI_LEFT|UI_SMALLFONT, color_white );

	y += SMALLCHAR_HEIGHT + SMALLCHAR_WIDTH;
	UI_DrawString( titlex, y, "Art", UI_RIGHT|UI_SMALLFONT, color_orange );
	UI_DrawString( namex, y, "Smthng, Veda, Vansen", UI_LEFT|UI_SMALLFONT, color_white );

	y += SMALLCHAR_HEIGHT + SMALLCHAR_WIDTH;
	UI_DrawString( titlex, y, "Sound", UI_RIGHT|UI_SMALLFONT, color_orange );
	UI_DrawString( namex, y, "Xypher, Vansen", UI_LEFT|UI_SMALLFONT, color_white );

/**/

	UI_DrawString( 320, 432, "QPong Arena(c) 1999-2001, Team Reaction.  All Rights Reserved", UI_CENTER|UI_SMALLFONT, color_red );
	UI_DrawString( 320, 448, "Quake III Arena(c) 1999-2000, Id Software, Inc.  All Rights Reserved", UI_CENTER|UI_SMALLFONT, color_red );
	UI_DrawString( 320, 464, "1-800-idgames | www.quake3arena.com | www.idsoftware.com | www.teamreaction.com", UI_CENTER|UI_SMALLFONT, color_red );
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
