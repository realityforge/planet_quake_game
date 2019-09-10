/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "cg_local.h"
#include "../ui/ui_shared.h"

extern displayContextDef_t cgDC;

int drawTeamOverlayModificationCount = -1;

void CG_InitTeamChat() {
  memset(teamChat1, 0, sizeof(teamChat1));
  memset(teamChat2, 0, sizeof(teamChat2));
  memset(systemChat, 0, sizeof(systemChat));
}


static void CG_DrawText( const ownerDrawDef_t *def, const char *text ) {
	float x;
	if ( def->align == ITEM_ALIGN_LEFT ) {
		x = def->x; 
	} else if ( def->align == ITEM_ALIGN_CENTER ) {
		x = def->x + 0.5 * (def->w - CG_Text_Width( text, def->scale, 0 ));
	} else if ( def->align == ITEM_ALIGN_CENTER ) {
		x = def->x + def->w - CG_Text_Width( text, def->scale, 0 );
	} else {
		return;
	}
	CG_Text_Paint( x, def->y + def->h, def->scale, def->color, text, 0, 0, def->style );
}

static void CG_DrawPlayerScore( ownerDrawDef_t *def ) {
	char text[16];
	int value = cg.snap->ps.persistant[PERS_SCORE];

	Com_sprintf ( text, sizeof(text), "%i", value);
	CG_DrawText( def, text );
}

static const char *CG_TimeString( int time ) {
	int msec = time % 1000;
	int sec = (time / 1000) % 60;
	int min = time / ( 60*1000 );

	if ( min )
		return va( "%2d:%02d:%03d", min, sec, msec );
	else
		return va( "%2d:%03d", sec, msec );
}

static void CG_DrawTimerCurrent( ownerDrawDef_t *def ) {
	playerState_t *ps = &cg.snap->ps;
	int raceTime = ps->powerups[PW_RACE_TIME];

	if (raceTime <= 0)
		return;

	if (ps->stats[STAT_RACE_CHECKPOINT] >= 0)
		raceTime = cg.time - raceTime;

	CG_DrawText( def, CG_TimeString( raceTime ) );
}

static void CG_DrawTimerBest( ownerDrawDef_t *def ) {
	if ( cg.timerBest <= 0 )
		return;
	CG_DrawText( def, CG_TimeString( cg.timerBest ) );
}

static void CG_DrawPlayerSpeed( ownerDrawDef_t *def ) {
	char *s;
	int     w;
	int speed;
	vec3_t vel;

	//Only care about the x,y speed
	vel[0] = cg.predictedPlayerState.velocity[0];
	vel[1] = cg.predictedPlayerState.velocity[1];
	vel[2] = 0;

	speed = sqrt( vel[0] * vel[0] + vel[1] * vel[1] );
	
	s = va("RPM: %i", speed); //RPM for ALL Languages!
	CG_DrawText( def, s );
}

static void CG_DrawPlayerItem( rectDef_t *rect, float scale, qboolean draw2D) {
	int		value;
	vec3_t origin, angles;

	value = cg.snap->ps.stats[STAT_HOLDABLE_ITEM];
	if ( value ) {
		CG_RegisterItemVisuals( value );

		if (qtrue) {
		  CG_RegisterItemVisuals( value );
		  CG_DrawPic( rect->x, rect->y, rect->w, rect->h, cg_items[ value ].icon );
		} else {
			VectorClear( angles );
			origin[0] = 90;
			origin[1] = 0;
			origin[2] = -10;
			angles[YAW] = ( cg.time & 2047 ) * 360 / 2048.0;
			CG_Draw3DModel(rect->x, rect->y, rect->w, rect->h, cg_items[ value ].models[0], 0, origin, angles );
		}
	}
}


static void CG_DrawRedScore(rectDef_t *rect, float scale, vec4_t color, qhandle_t shader, int textStyle ) {
	int value;
	char num[16];
	if ( cgs.scores1 == SCORE_NOT_PRESENT ) {
		Com_sprintf (num, sizeof(num), "-");
	} else {
		Com_sprintf (num, sizeof(num), "%i", cgs.scores1);
	}
	value = CG_Text_Width(num, scale, 0);
	CG_Text_Paint(rect->x + rect->w - value, rect->y + rect->h, scale, color, num, 0, 0, textStyle);
}

static void CG_DrawBlueScore(rectDef_t *rect, float scale, vec4_t color, qhandle_t shader, int textStyle ) {
	int value;
	char num[16];

	if ( cgs.scores2 == SCORE_NOT_PRESENT ) {
		Com_sprintf (num, sizeof(num), "-");
	}
	else {
		Com_sprintf (num, sizeof(num), "%i", cgs.scores2);
	}
	value = CG_Text_Width(num, scale, 0);
	CG_Text_Paint(rect->x + rect->w - value, rect->y + rect->h, scale, color, num, 0, 0, textStyle);
}


float CG_GetValue(int ownerDraw) {
	centity_t	*cent;
 	clientInfo_t *ci;
	playerState_t	*ps;

	cent = &cg_entities[cg.snap->ps.clientNum];
	ps = &cg.snap->ps;

	switch (ownerDraw) {
	case CG_PLAYER_SCORE:
		return cg.snap->ps.persistant[PERS_SCORE];
		break;
	default:
		break;
	}
	return -1;
}

// THINKABOUTME: should these be exclusive or inclusive.. 
// 
qboolean CG_OwnerDrawVisible(int flags) {

	if (flags & CG_SHOW_TEAMGAME) {
		if( cgs.gametype >= GT_TEAM) {
			return qtrue;
		}
	}

	if (flags & CG_SHOW_NOTTEAMGAME) {
		if( cgs.gametype < GT_TEAM) {
			return qtrue;
		}
	}


	if (flags & CG_SHOW_SINGLEPLAYER) {
		if( cgs.gametype == GT_SINGLE_PLAYER ) {
			return qtrue;
		}
	}

	if ( flags & CG_SHOW_TIMER ) {
//TODO Really test for timer mode sometime :)
		return qtrue;
	}
	return qfalse;
}


const char *CG_GetGameStatusText() {
	const char *s = "";
	if ( cgs.gametype < GT_TEAM) {
		if (cg.snap->ps.persistant[PERS_TEAM] != TEAM_SPECTATOR ) {
			s = va("%s place with %i",CG_PlaceString( cg.snap->ps.persistant[PERS_RANK] + 1 ),cg.snap->ps.persistant[PERS_SCORE] );
		}
	} else {
		if ( cg.teamScores[0] == cg.teamScores[1] ) {
			s = va("Teams are tied at %i", cg.teamScores[0] );
		} else if ( cg.teamScores[0] >= cg.teamScores[1] ) {
			s = va("Red leads Blue, %i to %i", cg.teamScores[0], cg.teamScores[1] );
		} else {
			s = va("Blue leads Red, %i to %i", cg.teamScores[1], cg.teamScores[0] );
		}
	}
	return s;
}
	
static void CG_DrawGameStatus( ownerDrawDef_t *def ) {
	CG_DrawText( def, CG_GetGameStatusText() );
}

const char *CG_GameTypeString() {
	if ( cgs.gametype == GT_FFA ) {
		return "Free For All";
	} else if ( cgs.gametype == GT_TEAM ) {
		return "Team Deathmatch";
	} else if ( cgs.gametype == GT_CTF ) {
		return "Capture the Flag";
	} else if ( cgs.gametype == GT_1FCTF ) {
		return "One Flag CTF";
	} else if ( cgs.gametype == GT_OBELISK ) {
		return "Overload";
	} else if ( cgs.gametype == GT_HARVESTER ) {
		return "Harvester";
	}
	return "";
}
static void CG_DrawGameType( ownerDrawDef_t *def ) {
	CG_DrawText( def, CG_GameTypeString() ) ;
}

#define	FPS_FRAMES	8
static void CG_DrawSystemFPS( ownerDrawDef_t *def ) {
	static int	previousTimes[FPS_FRAMES];
	static int	index;
	static int	previous;
	static int	total;
	int		fps, t, frameTime;

	if ( !cg_drawFPS.integer )
		return;
	// don't use serverTime, because that will be drifting to
	// correct for internet lag changes, timescales, timedemos, etc
	t = trap_Milliseconds();
	frameTime = t - previous;
	previous = t;

	/* Just keep the total value directly */
	total -= previousTimes[index % FPS_FRAMES];
	total += frameTime;
	previousTimes[index % FPS_FRAMES] = frameTime;
	index++;
	if ( index > FPS_FRAMES ) {
		// average multiple frames together to smooth changes out a bit
		if ( total ) {
			fps = 1000 * FPS_FRAMES / total;
		} else {
			fps = -1;
		}
		CG_DrawText( def, va( "FPS:%i", fps ) );
	}
}


/*
==============
CG_DrawDisconnect

Should we draw something differnet for long lag vs no packets?
==============
*/
static void CG_DrawDisconnect( const ownerDrawDef_t *def ) {
	float		x, y;
	int			cmdNum;
	usercmd_t	cmd;
	const char		*s;
	int			w;  // bk010215 - FIXME char message[1024];

	// draw the phone jack if we are completely past our buffers
	cmdNum = trap_GetCurrentCmdNumber() - CMD_BACKUP + 1;
	trap_GetUserCmd( cmdNum, &cmd );
	if ( cmd.serverTime <= cg.snap->ps.commandTime
		|| cmd.serverTime > cg.time ) {	// special check for map_restart // bk 0102165 - FIXME
		return;
	}

	// also add text in center of screen
	s = "Connection Interrupted"; // bk 010215 - FIXME
	w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH;
	CG_DrawBigString( 320 - w/2, 100, s, 1.0F);

	// blink the icon
	if ( ( cg.time >> 9 ) & 1 ) {
		return;
	}

	x = 640 - 48;
	y = 480 - 48;

	CG_DrawPic( def->x, def->y, def->w, def->h, trap_R_RegisterShader("gfx/2d/net.tga" ) );
}
/*
==============
CG_DrawSystemLagometer
==============
*/
#define	MAX_LAGOMETER_PING	900
#define	MAX_LAGOMETER_RANGE	300
static void CG_DrawSystemLagometer( const ownerDrawDef_t *def ) {
	int		a, i;
	float	v;
	float	ax, ay, aw, ah, mid, range;
	int		color;
	float	vscale;

	if ( !cg_lagometer.integer /*/| cgs.localServer */ ) {
		CG_DrawDisconnect( def );
		return;
	}

	//
	// draw the graph
	//

	trap_R_SetColor( NULL );
	CG_DrawPic( def->x, def->y, def->w, def->h, cgs.media.lagometerShader );

	ax = def->x;
	ay = def->y;
	aw = def->w;
	ah = def->h;
	CG_AdjustFrom640( &ax, &ay, &aw, &ah );

	color = -1;
	range = ah / 3;
	mid = ay + range;

	vscale = range / MAX_LAGOMETER_RANGE;

	// draw the frame interpoalte / extrapolate graph
	for ( a = 0 ; a < aw ; a++ ) {
		i = ( cg.lagometer.frameCount - 1 - a ) & (LAG_SAMPLES - 1);
		v = cg.lagometer.frameSamples[i];
		v *= vscale;
		if ( v > 0 ) {
			if ( color != 1 ) {
				color = 1;
				trap_R_SetColor( g_color_table[ColorIndex(COLOR_YELLOW)] );
			}
			if ( v > range ) {
				v = range;
			}
			trap_R_DrawStretchPic ( ax + aw - a, mid - v, 1, v, 0, 0, 0, 0, cgs.media.whiteShader );
		} else if ( v < 0 ) {
			if ( color != 2 ) {
				color = 2;
				trap_R_SetColor( g_color_table[ColorIndex(COLOR_BLUE)] );
			}
			v = -v;
			if ( v > range ) {
				v = range;
			}
			trap_R_DrawStretchPic( ax + aw - a, mid, 1, v, 0, 0, 0, 0, cgs.media.whiteShader );
		}
	}

	// draw the snapshot latency / drop graph
	range = ah / 2;
	vscale = range / MAX_LAGOMETER_PING;

	for ( a = 0 ; a < aw ; a++ ) {
		i = ( cg.lagometer.snapshotCount - 1 - a ) & (LAG_SAMPLES - 1);
		v = cg.lagometer.snapshotSamples[i];
		if ( v > 0 ) {
			if ( cg.lagometer.snapshotFlags[i] & SNAPFLAG_RATE_DELAYED ) {
				if ( color != 5 ) {
					color = 5;	// YELLOW for rate delay
					trap_R_SetColor( g_color_table[ColorIndex(COLOR_YELLOW)] );
				}
			} else {
				if ( color != 3 ) {
					color = 3;
					trap_R_SetColor( g_color_table[ColorIndex(COLOR_GREEN)] );
				}
			}
			v = v * vscale;
			if ( v > range ) {
				v = range;
			}
			trap_R_DrawStretchPic( ax + aw - a, ay + ah - v, 1, v, 0, 0, 0, 0, cgs.media.whiteShader );
		} else if ( v < 0 ) {
			if ( color != 4 ) {
				color = 4;		// RED for dropped snapshots
				trap_R_SetColor( g_color_table[ColorIndex(COLOR_RED)] );
			}
			trap_R_DrawStretchPic( ax + aw - a, ay + ah - range, 1, range, 0, 0, 0, 0, cgs.media.whiteShader );
		}
	}

	trap_R_SetColor( NULL );

	if ( cg_nopredict.integer || cg_synchronousClients.integer ) {
		CG_DrawBigString( ax, ay, "snc", 1.0 );
	}

	CG_DrawDisconnect( def );
}


void CG_OwnerDraw( ownerDrawDef_t *def ) {
	if ( cg_drawStatus.integer == 0 ) {
		return;
	}

	switch (def->number) {
	case CG_PLAYER_SCORE:
		CG_DrawPlayerScore( def );
		break;
	case CG_PLAYER_SPEED:
		CG_DrawPlayerSpeed( def );
		break;

	case CG_TIMER_CURRENT:
		CG_DrawTimerCurrent( def );
		break;
	case CG_TIMER_BEST:
		CG_DrawTimerBest( def );
		break;


	case CG_GAME_TYPE:
		CG_DrawGameType( def );
		break;
	case CG_GAME_STATUS:
		CG_DrawGameStatus( def );
		break;

	case CG_SYSTEM_FPS:
		CG_DrawSystemFPS( def );
		break;
	case CG_SYSTEM_LAGOMETER:
		CG_DrawSystemLagometer( def );
		break;

	}
}

void CG_MouseEvent(int x, int y) {
	int n;

	if ( (cg.predictedPlayerState.pm_type == PM_NORMAL || cg.predictedPlayerState.pm_type == PM_SPECTATOR) && cg.showScores == qfalse) {
    trap_Key_SetCatcher(0);
		return;
	}

	cgs.cursorX+= x;
	if (cgs.cursorX < 0)
		cgs.cursorX = 0;
	else if (cgs.cursorX > 640)
		cgs.cursorX = 640;

	cgs.cursorY += y;
	if (cgs.cursorY < 0)
		cgs.cursorY = 0;
	else if (cgs.cursorY > 480)
		cgs.cursorY = 480;

	n = Display_CursorType(cgs.cursorX, cgs.cursorY);
	cgs.activeCursor = 0;
	if (n == CURSOR_ARROW) {
		cgs.activeCursor = cgs.media.selectCursor;
	} else if (n == CURSOR_SIZER) {
		cgs.activeCursor = cgs.media.sizeCursor;
	}

  if (cgs.capturedItem) {
	  Display_MouseMove(cgs.capturedItem, x, y);
  } else {
	  Display_MouseMove(NULL, cgs.cursorX, cgs.cursorY);
  }

}

/*
==================
CG_HideTeamMenus
==================

*/
void CG_HideTeamMenu() {
  Menus_CloseByName("teamMenu");
  Menus_CloseByName("getMenu");
}

/*
==================
CG_ShowTeamMenus
==================

*/
void CG_ShowTeamMenu() {
  Menus_OpenByName("teamMenu");
}

/*
==================
CG_EventHandling
==================
 type 0 - no event handling
      1 - team menu
      2 - hud editor

*/
void CG_EventHandling(int type) {
	cgs.eventHandling = type;
  if (type == CGAME_EVENT_NONE) {
    CG_HideTeamMenu();
  } else if (type == CGAME_EVENT_TEAMMENU) {
    //CG_ShowTeamMenu();
  } else if (type == CGAME_EVENT_SCOREBOARD) {
  }

}



void CG_KeyEvent(int key, qboolean down) {

	if (!down) {
		return;
	}

	if ( cg.predictedPlayerState.pm_type == PM_NORMAL || (cg.predictedPlayerState.pm_type == PM_SPECTATOR && cg.showScores == qfalse)) {
		CG_EventHandling(CGAME_EVENT_NONE);
		trap_Key_SetCatcher(0);
		return;
	}
	
	Display_HandleKey(key, down, cgs.cursorX, cgs.cursorY);

	if (cgs.capturedItem) {
		cgs.capturedItem = NULL;
	}	else {
		if (key == K_MOUSE2 && down) {
			cgs.capturedItem = Display_CaptureItem(cgs.cursorX, cgs.cursorY);
		}
	}
}

void CG_RunMenuScript(char **args) {
}

void CG_GetTeamColor(vec4_t *color) {
  if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_RED) {
    (*color)[0] = 1.0f;
    (*color)[3] = 0.25f;
    (*color)[1] = (*color)[2] = 0.0f;
  } else if (cg.snap->ps.persistant[PERS_TEAM] == TEAM_BLUE) {
    (*color)[0] = (*color)[1] = 0.0f;
    (*color)[2] = 1.0f;
    (*color)[3] = 0.25f;
  } else {
    (*color)[0] = (*color)[2] = 0.0f;
    (*color)[1] = 0.17f;
    (*color)[3] = 0.25f;
	}
}

