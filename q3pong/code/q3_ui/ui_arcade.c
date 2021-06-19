/*
	UI  Arcade for Quake3
	Binary.Cowboy - Team Reaction
	http://binaryc.teamreaction.com
	copyright 2000
*/

#include "ui_local.h"

typedef struct {
	menuframework_s		menu;

	int					lastTime;
	int					aiTime;

	int					left;
	int					right;

	int					lscore;
	int					rscore;

	struct {
		double x;
		double y;
		double sx;
		double sy;
	} ball;

	int					py;

} pong_t;

static pong_t s_pong;

typedef struct {
	qhandle_t			colorBase;
} arcade_t;

static arcade_t s_arcade;

static arcade_init = 0;

#define X_OFF	8
#define Y_OFF	6

/*
=================
Arcade_PongKey
=================
*/
static sfxHandle_t Arcade_PongKey( int key ) {
	switch (key){
	case K_UPARROW:
		s_pong.left --;
		break;
	case K_DOWNARROW:
		s_pong.left ++;
		break;
	default:
		return Menu_DefaultKey( &s_pong.menu, key );
	}
	return 0;
}

/*
=================
Arcade_PongDraw
=================
*/
static void Arcade_PongDraw( void ) {
	int i;
	vec4_t	hcolor;
/*
	if (!ui_games.value){
		UI_PopMenu();
		return;
	}
*/
	s_pong.left = (uis.cursory + 4 ) / 8;

	hcolor[3] = 0.7f;
	hcolor[0] = 0;
	hcolor[1] = 0;
	hcolor[2] = 0;

	trap_R_SetColor( hcolor );
	UI_DrawHandlePic( 0, 0, 640, 480, s_arcade.colorBase);
	trap_R_SetColor( NULL );
	
	UI_DrawString( 320, 16, va("%i   --   %i", s_pong.lscore, s_pong.rscore), UI_CENTER | UI_SMALLFONT, color_blue);

	UI_DrawString( (int)(s_pong.ball.x * 8), (int)((s_pong.ball.y * 8) - 4), "@", UI_CENTER | UI_SMALLFONT, color_white );

	for ( i = - 4; i < 4; i++){
		UI_DrawString( 4, ((s_pong.left + i) * 8) - 4, "|", UI_CENTER | UI_SMALLFONT, color_white );
	}
	for ( i = - 4; i < 4; i++){
		UI_DrawString( 640 - 4, ((s_pong.right + i) * 8) - 4, "|", UI_CENTER | UI_SMALLFONT, color_white );
	}

	if ( uis.realtime > s_pong.lastTime ){
		if (s_pong.ball.x < 2){
			if ( (s_pong.ball.y < (s_pong.left - 5) ) || (s_pong.ball.y > (s_pong.left + 4) ) ) {
				s_pong.rscore ++;
				s_pong.ball.x = rand() % 20 + 30;
				s_pong.ball.y = rand() % 50 + 5;

				s_pong.ball.sx = 1;
				s_pong.ball.sy = 0.5;
			}else{
				s_pong.ball.sx *= -1;
				s_pong.ball.sy += (s_pong.ball.y - s_pong.left) / 4;
			}
		}
		if (s_pong.ball.x > 77){
			if ( (s_pong.ball.y < (s_pong.right - 5) ) || (s_pong.ball.y > (s_pong.right + 4) ) ) {
				s_pong.lscore ++;
				s_pong.ball.x = rand() % 20 + 30;
				s_pong.ball.y = rand() % 50 + 5;

				s_pong.ball.sx = 1;
				s_pong.ball.sy = 0.5;
			}else{
				s_pong.ball.sx *= -1;
				s_pong.ball.sy += (s_pong.ball.y - s_pong.right) / 4;
			}
		}
		if (s_pong.ball.y < 2 || s_pong.ball.y > 58)
			s_pong.ball.sy *= -1;
	}

	if ( uis.realtime > s_pong.aiTime ){
		int delta;
		double d,r;
		int py;
		s_pong.aiTime = uis.realtime + 30;

		r = random() * s_pong.ball.sy;
		r -= r / 2;
		delta = 78 - s_pong.ball.x;
		d = (s_pong.ball.sy + r) * delta;
		py = s_pong.ball.y + d;

		if (py < 2)
			py = 2 + abs(py);
		if (py > 58)
			py = 58 - (py - 58);
		if (py < 2)
			py = 2 + abs(py);

		if ( py < s_pong.py - 4)
			s_pong.py --;
		else if (py > s_pong.py + 3)
			s_pong.py ++;

		py = s_pong.py;

		if (py < s_pong.right)
			s_pong.right --;
		if (py > s_pong.right)
			s_pong.right ++;
	}
	
	if ( uis.realtime > s_pong.lastTime ){
		s_pong.lastTime = uis.realtime + 20;
		s_pong.ball.x += s_pong.ball.sx;
		s_pong.ball.y += s_pong.ball.sy;
	}
}

/*
=================
Arcade_PongInit
=================
*/
static void Arcade_PongInit( void ){
//	int i;
	uiClientState_t	cstate;

	srand(uis.realtime);

	// zero set all our globals
	memset( &s_pong, 0 ,sizeof(pong_t) );

	s_pong.menu.key = Arcade_PongKey;
	s_pong.menu.draw = Arcade_PongDraw;
	s_pong.menu.wrapAround = qtrue;

	trap_GetClientState( &cstate );
	if ( cstate.connState >= CA_CONNECTED ) {
		s_pong.menu.fullscreen = qfalse;
	}else {
		s_pong.menu.fullscreen = qtrue;
	}

	s_pong.ball.x = rand() % 20 + 30;
	s_pong.ball.y = rand() % 60;

	s_pong.ball.sx = 1;
	s_pong.ball.sy = 0.5;

	s_pong.left = 30;
	s_pong.right = 30;

	s_pong.lscore = 0;
	s_pong.rscore = 0;

	if (!arcade_init){
		s_arcade.colorBase = trap_R_RegisterShaderNoMip( "gfx/2d/colorbar" );
		arcade_init = 1;
	}
}


/*
===============
UI_Pong
===============
*/
void UI_Pong( void ) {
	Arcade_PongInit();
	UI_PushMenu( &s_pong.menu );
}



