/*
**  	Copyright (C) 2004 by the Q3Rally Development team
**  	All rights reserved.
**
**  	cg_rally_racetools.c
**
**  	These functions are used to assist with race modes
**
**		Author: STONELANCE
*/

#include "cg_local.h"


void CG_NewLapTime( int lap, int time ) {
	centity_t	*cent;
	char		*t;

	cent = &cg_entities[cg.snap->ps.clientNum];

	if ((time - cent->startLapTime) < cent->bestLapTime || cent->bestLapTime == 0){
		// New bestlap
		cent->bestLapTime = (time - cent->startLapTime);
		cent->bestLap = cent->currentLap;

		t = getStringForTime( cent->bestLapTime );

		Com_Printf("You got a personal record lap time of %s!\n", t);
	}

	cent->currentLap = lap;
	cent->lastStartLapTime = cent->startLapTime;
	cent->startLapTime = time;
}

void CG_FinishedRace( int client, int time ) {
	centity_t	*cent;
	char		*t;

	cent = &cg_entities[client];

	if ( client == cg.snap->ps.clientNum
		&& ((time - cent->startLapTime) < cent->bestLapTime || cent->bestLapTime == 0) ){
		// New bestlap
		cent->bestLapTime = (time - cent->startLapTime);
		cent->bestLap = cent->currentLap;

		t = getStringForTime( cent->bestLapTime );

		Com_Printf("You got a personal record lap time of %s!\n", t);
	}

	cent->finishRaceTime = time;
}

void CG_StartRace( int time ) {
	int			i;
	centity_t	*player;

	for (i = 0; i < MAX_CLIENTS; i++){
		player = &cg_entities[i];
		if (!player) continue;

		if (!player->startRaceTime){
			player->startRaceTime = time;
			player->finishRaceTime = 0;
			player->startLapTime = time;
			player->currentLap = 1;
			player->bestLapTime = 0;
			player->lastStartLapTime = 0;
		}
	}
}

void CG_DrawRaceCountDown( void ){
	float	f, scale;
	int		x, y, w, h;
	vec4_t	color;

	if (cg.countDownEnd + 1000 < cg.time || cg.countDownPrint[0] == 0)
		return;

	f = cg.countDownEnd < cg.time ? 0.0f : (cg.countDownEnd - cg.time) / 3000.0f;

	color[0] = 1.0f * f;
	color[1] = 1.0f * (1-f);
	color[2] = 0;
	color[3] = 1.0f;

	scale = cg.countDownEnd < cg.time ? 0.8f : ((cg.countDownEnd - cg.time) % 1000) / 1000.0f;
	w = 3*GIANTCHAR_WIDTH * scale;
	h = 3*GIANTCHAR_HEIGHT * scale;
	x = 320 - (strlen(cg.countDownPrint) * w) / 2;
	y = 240 - h/2;
	CG_DrawStringExt( x, y, cg.countDownPrint, color, qfalse, qtrue, w, h, 0 );
}

void CG_RaceCountDown( const char *str, int secondsLeft ){
	cg.centerPrintTime = 0;
	cg.countDownEnd = cg.time + secondsLeft * 1000;
	Q_strncpyz( cg.countDownPrint, str, sizeof(cg.countDownPrint) );
}
