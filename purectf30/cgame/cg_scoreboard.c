// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_scoreboard -- draw the scoreboard on top of the game screen
#include "cg_local.h"


#define	SCOREBOARD_X		(0)

#define SB_HEADER			86
#define SB_TOP				(SB_HEADER+32)

// Where the status bar starts, so we don't overwrite it
#define SB_STATUSBAR		420

#define SB_NORMAL_HEIGHT	40
#define SB_INTER_HEIGHT		16 // interleaved height

#define SB_MAXCLIENTS_NORMAL  ((SB_STATUSBAR - SB_TOP) / SB_NORMAL_HEIGHT)
#define SB_MAXCLIENTS_INTER   ((SB_STATUSBAR - SB_TOP) / SB_INTER_HEIGHT - 1)

// Used when interleaved



#define SB_LEFT_BOTICON_X	(SCOREBOARD_X+0)
#define SB_LEFT_HEAD_X		(SCOREBOARD_X+32)
#define SB_RIGHT_BOTICON_X	(SCOREBOARD_X+64)
#define SB_RIGHT_HEAD_X		(SCOREBOARD_X+96)
// Normal
#define SB_BOTICON_X		(SCOREBOARD_X+32)
#define SB_HEAD_X			(SCOREBOARD_X+64)

#define SB_SCORELINE_X		112

#define SB_RATING_WIDTH	    (6 * BIGCHAR_WIDTH) // width 6
#define SB_SCORE_X			(SB_SCORELINE_X + BIGCHAR_WIDTH) // width 6
#define SB_RATING_X			(SB_SCORELINE_X + 6 * BIGCHAR_WIDTH) // width 6
#define SB_PING_X			(SB_SCORELINE_X + 12 * BIGCHAR_WIDTH + 8) // width 5
#define SB_TIME_X			(SB_SCORELINE_X + 17 * BIGCHAR_WIDTH + 8) // width 5
#define SB_NAME_X			(SB_SCORELINE_X + 22 * BIGCHAR_WIDTH) // width 15

// KILDEREAN : new defines
#define SB_BOTICON_X_L		(SCOREBOARD_X+10)
#define SB_BOTICON_X_R		(SCOREBOARD_X+330)
#define SB_HEAD_X_L		(SCOREBOARD_X+20)
#define SB_HEAD_X_R		(SCOREBOARD_X+340)
 
#define SB_SCORELINE_X_L		32
#define SB_SCORELINE_X_R		352

#define SB_SCORE_X_L			(SB_SCORELINE_X_L + BIGCHAR_WIDTH) // width 6
#define SB_PING_X_L			(SB_SCORELINE_X_L +5+ 3 * BIGCHAR_WIDTH) // width 5
#define SB_TIME_X_L			(SB_SCORELINE_X_L +3+ 5 * BIGCHAR_WIDTH) // width 5
#define SB_NAME_X_L			(SB_SCORELINE_X_L + 7 * BIGCHAR_WIDTH) // width 15
#define SB_SCORE_X_R			(SB_SCORELINE_X_R + BIGCHAR_WIDTH) // width 6
#define SB_PING_X_R			(SB_SCORELINE_X_R +5+ 3 * BIGCHAR_WIDTH) // width 5
#define SB_TIME_X_R			(SB_SCORELINE_X_R +3+ 5 * BIGCHAR_WIDTH) // width 5
#define SB_NAME_X_R			(SB_SCORELINE_X_R + 7 * BIGCHAR_WIDTH) // width 15
// END KILDEREAN

// The new and improved score board
//
// In cases where the number of clients is high, the score board heads are interleaved
// here's the layout

//
//	0   32   80  112  144   240  320  400   <-- pixel position
//  bot head bot head score ping time name
//  
//  wins/losses are drawn on bot icon now

static qboolean localClient; // true if local client has been displayed


// KILDEREAN : lotsa changes...
/*
=================
CG_DrawScoreboard
=================
*/
// KILDEREAN : new func params
//static void CG_DrawClientScore( int y, score_t *score, float *color, float fade, qboolean largeFormat ) {
static void CG_DrawClientScore( int team, int y, score_t *score, float *color, float fade, qboolean largeFormat, int spec ) {

	char	string[1024];
	vec3_t	headAngles;
	clientInfo_t	*ci;
	
	// KILDEREAN
	//int iconx, headx;
 	char	namebuf[512];
 	int iconx_l, headx_l, iconx_r, headx_r;
 	float scoreC[4],pingC1[4],pingC2[4],pingC3[4],timeC[4],nameC[4];
 	float *pingC;	
 	
 	scoreC[0]=1;scoreC[1]=1;scoreC[2]=0;scoreC[3]=1;
	pingC1[0]=0;pingC1[1]=1;pingC1[2]=0;pingC1[3]=1;
 	pingC2[0]=1;pingC2[1]=1;pingC2[2]=0;pingC2[3]=1;
 	pingC3[0]=1;pingC3[1]=0;pingC3[2]=0;pingC3[3]=1;
 	timeC[0]=1;timeC[1]=1;timeC[2]=1;timeC[3]=1;
 	nameC[0]=0.2f;nameC[1]=1;nameC[2]=0.2f;nameC[3]=1;
 	
 	largeFormat=qfalse;
 	
 	if(team==TEAM_SPECTATOR){
 		y-=16*((spec+1)/2);
 	}
	// END KILDEREAN

	if ( score->client < 0 || score->client >= cgs.maxclients ) {
		Com_Printf( "Bad score->client: %i\n", score->client );
		return;
	}
	
	ci = &cgs.clientinfo[score->client];

	// KILDEREAN

	//iconx = SB_BOTICON_X + (SB_RATING_WIDTH / 2);
	//headx = SB_HEAD_X + (SB_RATING_WIDTH / 2);

 	iconx_l = SB_BOTICON_X_L;
 	headx_l = SB_HEAD_X_L;
 	iconx_r = SB_BOTICON_X_R;
 	headx_r = SB_HEAD_X_R;
 	
 	//highlight player
 	if ( score->client == cg.snap->ps.clientNum ) {
 		float	bcolor[4];
 		float	rcolor[4];
 		float	hcolor[4];
 
 		rcolor[0] = 1;
 		rcolor[1] = 0;
 		rcolor[2] = 0;
 		rcolor[3] = 0.3f;
 		bcolor[0] = 0;
 		bcolor[1] = 0;
 		bcolor[2] = 1;
 		bcolor[3] = 0.3f;
 		bcolor[0] = 1;
 		bcolor[1] = 1;
 		bcolor[2] = 1;
 		bcolor[3] = 0.3f;
 		if(team==TEAM_RED)
 			CG_FillRect( 0,y,315,SMALLCHAR_HEIGHT+1, rcolor );
 		if(team==TEAM_BLUE)
 			CG_FillRect( 330,y,315,SMALLCHAR_HEIGHT+1, bcolor );
 		if(team==TEAM_SPECTATOR){
 			if(spec%2==0)	
 				CG_FillRect( 0,y,315,SMALLCHAR_HEIGHT+1, hcolor );
 			else
 				CG_FillRect( 330,y,315,SMALLCHAR_HEIGHT+1, hcolor );
 		}
 	}
	// END KILDEREAN


	// draw the handicap or bot skill marker (unless player has flag)
	if ( ci->powerups & ( 1 << PW_NEUTRALFLAG ) ) {
		// KILDEREAN
		CG_DrawFlagModel( iconx_l, y, 16, 16, TEAM_FREE, qfalse );
		// END KILDEREAN
	} else if ( ci->powerups & ( 1 << PW_REDFLAG ) ) {
		// KILDEREAN
		CG_DrawFlagModel( iconx_r, y, 16, 16, TEAM_RED, qfalse );
		// END KILDEREAN
	} else if ( ci->powerups & ( 1 << PW_BLUEFLAG ) ) {
		// KILDEREAN
		CG_DrawFlagModel( iconx_l, y, 16, 16, TEAM_BLUE, qfalse );
		// END KILDEREAN
	} else {
		if ( ci->botSkill > 0 && ci->botSkill <= 5 ) {
			if ( cg_drawIcons.integer ) {
				// KILDEREAN
				//CG_DrawPic( iconx, y, 16, 16, cgs.media.botSkillShaders[ ci->botSkill - 1 ] );
 				if(team==TEAM_RED)
					CG_DrawPic( iconx_l, y, 16, 16, cgs.media.botSkillShaders[ ci->botSkill - 1 ] );
				if(team==TEAM_BLUE)
					CG_DrawPic( iconx_r, y, 16, 16, cgs.media.botSkillShaders[ ci->botSkill - 1 ] );
				// END KILDEREAN
			}
		} else if ( ci->handicap < 100 ) {
			Com_sprintf( string, sizeof( string ), "%i", ci->handicap );

			// KILDEREAN
			//if ( cgs.gametype == GT_TOURNAMENT )
			//	CG_DrawSmallStringColor( iconx, y - SMALLCHAR_HEIGHT/2, string, color );
			//else
			//	CG_DrawSmallStringColor( iconx, y, string, color );
 			if(team==TEAM_RED)
 				CG_DrawSmallStringColor( iconx_l, y, string, color );
 			if(team==TEAM_BLUE)
 				CG_DrawSmallStringColor( iconx_r, y, string, color );	
 			if(team==TEAM_SPECTATOR){
 				if(spec%2==0)
 					CG_DrawSmallStringColor( iconx_l, y, string, color );	
 				else
 					CG_DrawSmallStringColor( iconx_r, y, string, color );	
			}
			// END KILDEREAN
		}

		// draw the wins / losses
		if ( cgs.gametype == GT_TOURNAMENT ) {
			Com_sprintf( string, sizeof( string ), "%i/%i", ci->wins, ci->losses );
			if( ci->handicap < 100 && !ci->botSkill ) {
				CG_DrawSmallStringColor( iconx_l, y + SMALLCHAR_HEIGHT/2, string, color );
			}
			else {
				CG_DrawSmallStringColor( iconx_l, y, string, color );
			}
		}
	}

	// draw the face
	VectorClear( headAngles );
	headAngles[YAW] = 180;

	// KILDEREAN
 	if(team==TEAM_RED)
 		CG_DrawHead( headx_l, y, 16, 16, score->client, headAngles );
 	if(team==TEAM_BLUE)
 		CG_DrawHead( headx_r, y, 16, 16, score->client, headAngles );
 
 	if(score->ping<100)pingC=pingC1;
 	else {
 		if(score->ping<250)pingC=pingC2;
 		else pingC=pingC3;
 	}
	// END KILDEREAN

#ifdef MISSIONPACK
	// draw the team task
	if ( ci->teamTask != TEAMTASK_NONE ) {
		if ( ci->teamTask == TEAMTASK_OFFENSE ) {
			CG_DrawPic( headx + 48, y, 16, 16, cgs.media.assaultShader );
		}
		else if ( ci->teamTask == TEAMTASK_DEFENSE ) {
			CG_DrawPic( headx + 48, y, 16, 16, cgs.media.defendShader );
		}
	}
#endif
	// draw the score line
	if ( score->ping == -1 ) {
		// KILDEREAN
 		Com_sprintf(string, sizeof(string),"CONNECTING");
		// END KILDEREAN
	} else if ( ci->team == TEAM_SPECTATOR ) {
		// KILDEREAN
 		Com_sprintf(string, sizeof(string),"SPECT     %3i", score->time);
		Com_sprintf(namebuf,sizeof(namebuf),"%s",ci->name);
		if(Q_PrintStrlen(ci->name)>21)			
 			namebuf[21]='\0';
 		if(spec%2==0){
 			CG_DrawSmallString( SB_SCORELINE_X_L, y, string, fade );
 			if(score->ping!=-1){
 				Com_sprintf(string, sizeof(string),"%3i",score->ping);
 				CG_DrawSmallStringColor(SB_PING_X_L, y, string,pingC);
 			}
 			CG_DrawSmallStringColor(SB_NAME_X_L, y, namebuf,nameC);
 		}
 		else{
 			CG_DrawSmallString( SB_SCORELINE_X_R, y, string, fade );
 			if(score->ping!=-1){
 				Com_sprintf(string, sizeof(string),"%3i",score->ping);
 				CG_DrawSmallStringColor(SB_PING_X_R, y, string,pingC);
 			}
 			CG_DrawSmallStringColor(SB_NAME_X_R, y, namebuf,nameC);
  		}
		// END KILDEREAN
	} else {
		// KILDEREAN
 		if(team==TEAM_RED){
 			Com_sprintf(string, sizeof(string),"%4i",score->score);
 			CG_DrawSmallStringColor(SB_SCORE_X_L, y, string,scoreC);
 			Com_sprintf(string, sizeof(string),"%3i",score->ping);
 			CG_DrawSmallStringColor(SB_PING_X_L, y, string,pingC);
 			Com_sprintf(string, sizeof(string),"%3i",score->time);
 			CG_DrawSmallStringColor(SB_TIME_X_L, y, string,timeC);
 			Com_sprintf(namebuf,sizeof(namebuf),"%s",ci->name);
 			if(Q_PrintStrlen(ci->name)>21){
 				namebuf[21]='\0';
 				CG_DrawSmallStringColor(SB_NAME_X_L, y, namebuf,nameC);
 			}
 			else{
 				CG_DrawSmallStringColor(SB_NAME_X_L, y, namebuf,nameC);
 			}
 		}
 		if(team==TEAM_BLUE){
 			Com_sprintf(string, sizeof(string),"%4i",score->score);
 			CG_DrawSmallStringColor(SB_SCORE_X_R, y, string,scoreC);
 			Com_sprintf(string, sizeof(string),"%3i",score->ping);
 			CG_DrawSmallStringColor(SB_PING_X_R, y, string,pingC);
 			Com_sprintf(string, sizeof(string),"%3i",score->time);
 			CG_DrawSmallStringColor(SB_TIME_X_R, y, string,timeC);
			Com_sprintf(namebuf,sizeof(namebuf),"%s",ci->name);
 			if(Q_PrintStrlen(ci->name)>21){
 				namebuf[21]='\0';
				CG_DrawSmallStringColor(SB_NAME_X_R, y, namebuf,nameC);
 			}
 			else
 				CG_DrawSmallStringColor(SB_NAME_X_R, y, namebuf,nameC);
 		}		
		/*
 		if(team==TEAM_SPECTATOR){
			Com_sprintf(namebuf,sizeof(namebuf),"%s",ci->name);
			if(Q_PrintStrlen(ci->name)>21)			
 				namebuf[21]='\0';
 			if(spec%2==0){
 				CG_DrawSmallString( SB_SCORELINE_X_L, y, string, fade );
 				if(score->ping!=-1){
 					Com_sprintf(string, sizeof(string),"%3i",score->ping);
 					CG_DrawSmallStringColor(SB_PING_X_L, y, string,pingC);
 				}
 				CG_DrawSmallStringColor(SB_NAME_X_L, y, namebuf,nameC);
 			}
 			else{
 				CG_DrawSmallString( SB_SCORELINE_X_R, y, string, fade );
 				if(score->ping!=-1){
 					Com_sprintf(string, sizeof(string),"%3i",score->ping);
 					CG_DrawSmallStringColor(SB_PING_X_R, y, string,pingC);
 				}
 				CG_DrawSmallStringColor(SB_NAME_X_R, y, namebuf,nameC);
  			}
 		}
		*/
		// END KILDEREAN
	}


/*	// highlight your position
	if ( score->client == cg.snap->ps.clientNum ) {
		float	hcolor[4];
		int		rank;

		localClient = qtrue;

		if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR 
			|| cgs.gametype >= GT_TEAM ) {
			rank = -1;
		} else {
			rank = cg.snap->ps.persistant[PERS_RANK] & ~RANK_TIED_FLAG;
		}
		if ( rank == 0 ) {
			hcolor[0] = 0;
			hcolor[1] = 0;
			hcolor[2] = 0.7f;
		} else if ( rank == 1 ) {
			hcolor[0] = 0.7f;
			hcolor[1] = 0;
			hcolor[2] = 0;
		} else if ( rank == 2 ) {
			hcolor[0] = 0.7f;
			hcolor[1] = 0.7f;
			hcolor[2] = 0;
		} else {
			hcolor[0] = 0.7f;
			hcolor[1] = 0.7f;
			hcolor[2] = 0.7f;
		}

		hcolor[3] = fade * 0.7;
		CG_FillRect( SB_SCORELINE_X + BIGCHAR_WIDTH + (SB_RATING_WIDTH / 2), y, 
			640 - SB_SCORELINE_X - BIGCHAR_WIDTH, BIGCHAR_HEIGHT+1, hcolor );
	}

	CG_DrawBigString( SB_SCORELINE_X + (SB_RATING_WIDTH / 2), y, string, fade );
*/
	// add the "ready" marker for intermission exiting
	if ( cg.snap->ps.stats[ STAT_CLIENTS_READY ] & ( 1 << score->client ) ) {

		// KILDEREAN
		//CG_DrawBigStringColor( iconx, y, "READY", color );
 		if(team==TEAM_RED)
 			CG_DrawSmallStringColor( iconx_l, y, "READY", color );
 		if(team==TEAM_BLUE)
 			CG_DrawSmallStringColor( iconx_r, y, "READY", color );			
 		if(team==TEAM_SPECTATOR){
 			if(spec%2==0)
 				CG_DrawSmallStringColor( iconx_l, y, "READY", color );
 			else
 				CG_DrawSmallStringColor( iconx_r, y, "READY", color );
 		}
		// END KILDEREAN
	}
}

/*
=================
CG_TeamScoreboard
=================
*/
static int CG_TeamScoreboard( int y, team_t team, float fade, int maxClients, int lineHeight ) {
	int		i;
	score_t	*score;
	float	color[4];
	// KILDEREAN : add spec
	int		count, spec;
	// END KILDEREAN
	clientInfo_t	*ci;

	color[0] = color[1] = color[2] = 1.0;
	color[3] = fade;

	count = 0;
	// KILDEREAN
	spec = 0;
	// END KILDEREAN


	for ( i = 0 ; i < cg.numScores && count < maxClients ; i++ ) {
		score = &cg.scores[i];
		ci = &cgs.clientinfo[ score->client ];

		if ( team != ci->team ) {
			continue;
		}

		// KILDEREAN
		//CG_DrawClientScore( y + lineHeight * count, score, color, fade, lineHeight == SB_NORMAL_HEIGHT );
 		CG_DrawClientScore( team,y + lineHeight * count, score, color, fade, lineHeight == SB_NORMAL_HEIGHT,spec);		
 		if(team==TEAM_SPECTATOR)
 			spec++;
		// END KILDEREAN

		count++;
	}

	return count;
}

/*
=================
CG_DrawScoreboard

Draw the normal in-game scoreboard
=================
*/
qboolean CG_DrawOldScoreboard( void ) {
	// KILDEREAN : add n3, and *s2
	int		x, y, w, n1, n2, n3;//,i;
	float	fade;
	float	*fadeColor;
	char	*s;//, s2;
	// END KILDEREAN
	int maxClients;
	int lineHeight;
	int topBorderSize, bottomBorderSize;

	// don't draw amuthing if the menu or console is up
	if ( cg_paused.integer ) {
		cg.deferredPlayerLoading = 0;
		return qfalse;
	}

	if ( cgs.gametype == GT_SINGLE_PLAYER && cg.predictedPlayerState.pm_type == PM_INTERMISSION ) {
		cg.deferredPlayerLoading = 0;
		return qfalse;
	}

	// don't draw scoreboard during death while warmup up
	if ( cg.warmup && !cg.showScores ) {
		return qfalse;
	}

	if ( cg.showScores || cg.predictedPlayerState.pm_type == PM_DEAD ||
		 cg.predictedPlayerState.pm_type == PM_INTERMISSION ) {
		fade = 1.0;
		fadeColor = colorWhite;
	} else {
		fadeColor = CG_FadeColor( cg.scoreFadeTime, FADE_TIME );
		
		if ( !fadeColor ) {
			// next time scoreboard comes up, don't print killer
			cg.deferredPlayerLoading = 0;
			cg.killerName[0] = 0;
			return qfalse;
		}
		fade = *fadeColor;
	}


	// fragged by ... line
	if ( cg.killerName[0] ) {
		s = va("Fragged by %s", cg.killerName );
		w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH;
		x = ( SCREEN_WIDTH - w ) / 2;
		y = 40;
		CG_DrawBigString( x, y, s, fade );
	}

	// current rank
	if ( cgs.gametype < GT_TEAM) {
		if (cg.snap->ps.persistant[PERS_TEAM] != TEAM_SPECTATOR ) {
			s = va("%s place with %i",
				CG_PlaceString( cg.snap->ps.persistant[PERS_RANK] + 1 ),
				cg.snap->ps.persistant[PERS_SCORE] );
			w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH;
			x = ( SCREEN_WIDTH - w ) / 2;
			y = 60;
			CG_DrawBigString( x, y, s, fade );
		}
	} else {
		if ( cg.teamScores[0] == cg.teamScores[1] ) {
			s = va("Teams are tied at %i", cg.teamScores[0] );
		} else if ( cg.teamScores[0] >= cg.teamScores[1] ) {
			s = va("Red leads %i to %i",cg.teamScores[0], cg.teamScores[1] );
		} else {
			s = va("Blue leads %i to %i",cg.teamScores[1], cg.teamScores[0] );
		}

		w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH;
		x = ( SCREEN_WIDTH - w ) / 2;
		y = 60;
		CG_DrawBigString( x, y, s, fade );
	}

	// scoreboard
	y = SB_HEADER;

	// KILDEREAN

	/*
	CG_DrawPic( SB_SCORE_X + (SB_RATING_WIDTH / 2), y, 64, 32, cgs.media.scoreboardScore );
	CG_DrawPic( SB_PING_X - (SB_RATING_WIDTH / 2), y, 64, 32, cgs.media.scoreboardPing );
	CG_DrawPic( SB_TIME_X - (SB_RATING_WIDTH / 2), y, 64, 32, cgs.media.scoreboardTime );
	CG_DrawPic( SB_NAME_X - (SB_RATING_WIDTH / 2), y, 64, 32, cgs.media.scoreboardName );
	*/

 	CG_DrawPic( SB_SCORE_X_L, y,32, 16, cgs.media.scoreboardScore );
 	CG_DrawPic( SB_PING_X_L, y, 32, 16, cgs.media.scoreboardPing );
 	CG_DrawPic( SB_TIME_X_L, y, 32, 16, cgs.media.scoreboardTime );
 	CG_DrawPic( SB_NAME_X_L, y, 32, 16, cgs.media.scoreboardName );
 	CG_DrawPic( SB_SCORE_X_R, y,32, 16, cgs.media.scoreboardScore );
 	CG_DrawPic( SB_PING_X_R, y, 32, 16, cgs.media.scoreboardPing );
 	CG_DrawPic( SB_TIME_X_R, y, 32, 16, cgs.media.scoreboardTime );
 	CG_DrawPic( SB_NAME_X_R, y, 32, 16, cgs.media.scoreboardName );
 	CG_DrawFlagModel( SB_SCORE_X_L-32, y-16 , 32, 32, TEAM_RED , qfalse);
 	CG_DrawFlagModel( SB_SCORE_X_R-32, y-16 , 32, 32, TEAM_BLUE, qfalse );
	// END KILDEREAN

	y = SB_TOP;

	// KILDEREAN
 	maxClients = SB_MAXCLIENTS_INTER;
 	lineHeight = SB_INTER_HEIGHT;
 	topBorderSize = 8;
 	bottomBorderSize = 16;
	/*	
	// If there are more than SB_MAXCLIENTS_NORMAL, use the interleaved scores
	if ( cg.numScores > SB_MAXCLIENTS_NORMAL ) {
		maxClients = SB_MAXCLIENTS_INTER;
		lineHeight = SB_INTER_HEIGHT;
		topBorderSize = 8;
		bottomBorderSize = 16;
	} else {
		maxClients = SB_MAXCLIENTS_NORMAL;
		lineHeight = SB_NORMAL_HEIGHT;
		topBorderSize = 16;
		bottomBorderSize = 16;
	}
	*/
	// END KILDEREAN
	localClient = qfalse;

	if ( cgs.gametype >= GT_TEAM ) {
		//
		// teamplay scoreboard
		//

		// KILDEREAN
 		y = SB_HEADER+20;
 		n1 = CG_TeamScoreboard( y, TEAM_RED, 1, maxClients, lineHeight );
 		maxClients -= n1;
 		n2 = CG_TeamScoreboard( y, TEAM_BLUE, 1, maxClients, lineHeight );
 		n3=(n1>=n2)?n1:n2;
 		if(n3<6)n3=6;
 		maxClients -= n2;
 		y += (n3 * lineHeight);
 		CG_DrawTeamBackground( 0,100,315,y-75, 0.15f, TEAM_RED );		
 		CG_DrawTeamBackground( 330,100,390,y-75, 0.15f, TEAM_BLUE );				
 		y += BIGCHAR_HEIGHT*2;
 		n1 = CG_TeamScoreboard(y,TEAM_SPECTATOR,1,maxClients,lineHeight);
		/*
		y += lineHeight/2;
		if ( cg.teamScores[0] >= cg.teamScores[1] ) {
			n1 = CG_TeamScoreboard( y, TEAM_RED, fade, maxClients, lineHeight );
			CG_DrawTeamBackground( 0, y - topBorderSize, 640, n1 * lineHeight + bottomBorderSize, 0.33f, TEAM_RED );
			y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n1;
			n2 = CG_TeamScoreboard( y, TEAM_BLUE, fade, maxClients, lineHeight );
			CG_DrawTeamBackground( 0, y - topBorderSize, 640, n2 * lineHeight + bottomBorderSize, 0.33f, TEAM_BLUE );
			y += (n2 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n2;
		} else {
			n1 = CG_TeamScoreboard( y, TEAM_BLUE, fade, maxClients, lineHeight );
			CG_DrawTeamBackground( 0, y - topBorderSize, 640, n1 * lineHeight + bottomBorderSize, 0.33f, TEAM_BLUE );
			y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n1;
			n2 = CG_TeamScoreboard( y, TEAM_RED, fade, maxClients, lineHeight );
			CG_DrawTeamBackground( 0, y - topBorderSize, 640, n2 * lineHeight + bottomBorderSize, 0.33f, TEAM_RED );
			y += (n2 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n2;
		}
		n1 = CG_TeamScoreboard( y, TEAM_SPECTATOR, fade, maxClients, lineHeight );
		y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
		*/
		// END KILDEREAN

	} else {
		//
		// free for all scoreboard
		//
		n1 = CG_TeamScoreboard( y, TEAM_FREE, fade, maxClients, lineHeight );
		y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
		n2 = CG_TeamScoreboard( y, TEAM_SPECTATOR, fade, maxClients - n1, lineHeight );
		y += (n2 * lineHeight) + BIGCHAR_HEIGHT;
	}

	// KILDEREAN
	/*
	if (!localClient) {
		// draw local client at the bottom
		for ( i = 0 ; i < cg.numScores ; i++ ) {
			if ( cg.scores[i].client == cg.snap->ps.clientNum ) {
				CG_DrawClientScore( y, &cg.scores[i], fadeColor, fade, lineHeight == SB_NORMAL_HEIGHT );
				break;
			}
		}
	}
	*/
	// END KILDEREAN

	// load any models that have been deferred
	if ( ++cg.deferredPlayerLoading > 10 ) {
		CG_LoadDeferredPlayers();
	}

	return qtrue;
}

//================================================================================

/*
================
CG_CenterGiantLine
================
*/
static void CG_CenterGiantLine( float y, const char *string ) {
	float		x;
	vec4_t		color;

	color[0] = 1;
	color[1] = 1;
	color[2] = 1;
	color[3] = 1;

	x = 0.5 * ( 640 - GIANT_WIDTH * CG_DrawStrlen( string ) );

	CG_DrawStringExt( x, y, string, color, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0 );
}

/*
=================
CG_DrawTourneyScoreboard

Draw the oversize scoreboard for tournements
=================
*/
void CG_DrawOldTourneyScoreboard( void ) {
	const char		*s;
	vec4_t			color;
	int				min, tens, ones;
	clientInfo_t	*ci;
	int				y;
	int				i;

	// request more scores regularly
	if ( cg.scoresRequestTime + 2000 < cg.time ) {
		cg.scoresRequestTime = cg.time;
		trap_SendClientCommand( "score" );
	}

	color[0] = 1;
	color[1] = 1;
	color[2] = 1;
	color[3] = 1;

	// draw the dialog background
	color[0] = color[1] = color[2] = 0;
	color[3] = 1;
	CG_FillRect( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color );

	// print the mesage of the day
	s = CG_ConfigString( CS_MOTD );
	if ( !s[0] ) {
		s = "Scoreboard";
	}

	// print optional title
	CG_CenterGiantLine( 8, s );

	// print server time
	ones = cg.time / 1000;
	min = ones / 60;
	ones %= 60;
	tens = ones / 10;
	ones %= 10;
	s = va("%i:%i%i", min, tens, ones );

	CG_CenterGiantLine( 64, s );


	// print the two scores

	y = 160;
	if ( cgs.gametype >= GT_TEAM ) {
		//
		// teamplay scoreboard
		//
		CG_DrawStringExt( 8, y, "Red Team", color, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0 );
		s = va("%i", cg.teamScores[0] );
		CG_DrawStringExt( 632 - GIANT_WIDTH * strlen(s), y, s, color, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0 );
		
		y += 64;

		CG_DrawStringExt( 8, y, "Blue Team", color, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0 );
		s = va("%i", cg.teamScores[1] );
		CG_DrawStringExt( 632 - GIANT_WIDTH * strlen(s), y, s, color, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0 );
	} else {
		//
		// free for all scoreboard
		//
		for ( i = 0 ; i < MAX_CLIENTS ; i++ ) {
			ci = &cgs.clientinfo[i];
			if ( !ci->infoValid ) {
				continue;
			}
			if ( ci->team != TEAM_FREE ) {
				continue;
			}

			CG_DrawStringExt( 8, y, ci->name, color, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0 );
			s = va("%i", ci->score );
			CG_DrawStringExt( 632 - GIANT_WIDTH * strlen(s), y, s, color, qtrue, qtrue, GIANT_WIDTH, GIANT_HEIGHT, 0 );
			y += 64;
		}
	}


}

