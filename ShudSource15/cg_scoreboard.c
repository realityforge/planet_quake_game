// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_scoreboard -- draw the scoreboard on top of the game screen
#include "cg_local.h"


#define	SCOREBOARD_X		(16)

// #define SB_HEADER			86
// #define SB_TOP				(SB_HEADER+32)

#define SB_HEADER			90	// SHUD
#define SB_TOP				(SB_HEADER+40) // SHUD

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


#define SB_SCORELINE_X		128

#define SB_SCORE_X		(SB_SCORELINE_X + BIGCHAR_WIDTH) // width 6
#define SB_PING_X			(SB_SCORELINE_X + 6 * BIGCHAR_WIDTH + 8) // width 5
#define SB_TIME_X			(SB_SCORELINE_X + 11 * BIGCHAR_WIDTH + 8) // width 5
#define SB_NAME_X			(SB_SCORELINE_X + 16 * BIGCHAR_WIDTH) // width 15

// ================================
// SHUD Advanced scoreboard defines
// ================================

#define SB_SCORELINE_X_ADV		64
#define SB_BOTICON_X_ADV	      0 
#define SB_HEAD_X_ADV		      32 
#define SB_BOTICON_INTER_X_ADV	24 
#define SB_HEAD_INTER_X_ADV		56 
#define SB_STATICONS			(SB_HEADER+22)
#define SB_SCORE_X_ADV			(SB_SCORELINE_X_ADV + 2 * SMALLCHAR_WIDTH)
#define SB_PING_X_ADV			(SB_SCORELINE_X_ADV + 7 * SMALLCHAR_WIDTH)
#define SB_TIME_X_ADV			(SB_SCORELINE_X_ADV + 11 * SMALLCHAR_WIDTH)
#define SB_STATS_X_ADV			(SB_SCORELINE_X_ADV + 17 * SMALLCHAR_WIDTH)
#define SB_STATS_X_ADV_2      	(SB_STATS_X_ADV+BIGCHAR_WIDTH*1.5)
#define SB_STATS_X_ADV_3      	(SB_STATS_X_ADV+BIGCHAR_WIDTH*3)
#define SB_STATS_X_ADV_4      	(SB_STATS_X_ADV+BIGCHAR_WIDTH*4.5)
#define SB_STATS_X_ADV_5      	(SB_STATS_X_ADV+BIGCHAR_WIDTH*6)
#define SB_STATS_X_ADV_6      	(SB_STATS_X_ADV+BIGCHAR_WIDTH*7.5)
#define SB_STATS_X_ADV_7      	(SB_STATS_X_ADV+BIGCHAR_WIDTH*9)
#define SB_STATS_X_ADV_8      	(SB_STATS_X_ADV+BIGCHAR_WIDTH*10.5)
#define SB_STATS_X_ADV_9      	(SB_STATS_X_ADV+BIGCHAR_WIDTH*12)
#define SB_STATS_X_ADV_10     	(SB_STATS_X_ADV+BIGCHAR_WIDTH*13.5)
#define SB_STATS_X_ADV_11     	(SB_STATS_X_ADV+BIGCHAR_WIDTH*15)
#define SB_NAME_X_ADV			(SB_SCORELINE_X_ADV + 50 * SMALLCHAR_WIDTH)

static int score_data[16];
static int num_scores;

// ======================================
// SHUD Advanced scoreboard defines [end]
// ======================================


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

							 /*
=================
CG_DrawScoreboard
=================
*/
static void CG_DrawClientScore( int y, score_t *score, float *color, float fade, qboolean largeFormat ) {
	char	string[1024];
	vec3_t	headAngles;
	clientInfo_t	*ci;
	int iconx, headx;
	
	// =================
	// SHUD declarations
 	// =================
 	char *s0;
      playerState_t  *ps;
	int f, h, p, i, fph, dfph, msec, sec, i_score, time;
   	qboolean r;
   	char s2[128];
	// =======================
	// SHUD declarations [end]
 	// =======================

	if ( score->client < 0 || score->client >= cgs.maxclients ) {
		Com_Printf( "Bad score->client: %i\n", score->client );
		return;
	}
	
	ci = &cgs.clientinfo[score->client];


	// ================
	// SHUD adv. scores 
  	// ================
   	ps = &cg.snap->ps;
    
	stats[score->client].score = score->score;
  
	if(level_end_time!=-1) time = level_end_time;
	else time = cg.time;

   	if ( score->client == cg.snap->ps.clientNum ) msec = time - start_time;
      else msec = time - cgs.levelStartTime;
   	sec = msec / 1000;
   	if(sec>0) fph = (score->score*cg_fragsPerTime.integer)/sec;
   	else fph = 0;
    	dfph = 0;
   	if(score->score!=0)
   	{
         if ((fph>=0)&(fph<10))
         {
         	dfph = (score->score*cg_fragsPerTime.integer*10)/sec;
         	dfph-=fph*10;
   	   	s0 = va("%2i.%1i",fph,dfph);
         }
         else s0=va("%4i",fph);
   	} else s0="   0";
	stats[score->client].fph = fph;
	stats[score->client].dfph = dfph;
      i_score = 0;
      if(stats[score->client].kills == 0) stats[score->client].efficiency = 0;
      else stats[score->client].efficiency = (stats[score->client].kills*100)/(stats[score->client].kills+stats[score->client].killed);
	if((cg_advScoreboard.integer & pow_of2[14])==pow_of2[14]) // Efficiency
      {
         score_data[i_score] = stats[score->client].efficiency;
         i_score++;
      };
      if((cg_advScoreboard.integer & pow_of2[1])==pow_of2[1])
      {
         score_data[i_score] = stats[score->client].weapon[0];i_score++;
      };
      if((cg_advScoreboard.integer & pow_of2[13])==pow_of2[13]) // Players
      {
         f=i_score;
         for(i=0;(i_score<(f+cg_clientsOnScoreboard.integer))&&(i<cg.numScores);i++)  
         if(cgs.clientinfo[i].team!=TEAM_SPECTATOR) 
         {
            score_data[i_score] = stats[score->client].frags[i];
            i_score++;
         };
      }
	else for(i=2;i<10;i++)	// Weapons
      if((cg_advScoreboard.integer & pow_of2[i])==pow_of2[i])
      if (cg_weapons[i].weaponIcon) {score_data[i_score] = stats[score->client].weapon[i-1];i_score++;};

      if((cg_advScoreboard.integer & pow_of2[10])==pow_of2[10])
      {
         score_data[i_score] = stats[score->client].excellent;
         i_score++;
      };
      if((cg_advScoreboard.integer & pow_of2[11])==pow_of2[11]) // Deaths
      {
         score_data[i_score] = stats[score->client].killed;
         i_score++;
      };
      if((cg_advScoreboard.integer & pow_of2[12])==pow_of2[12]) // Suicides
      {
         score_data[i_score] = stats[score->client].frags[score->client];
         i_score++;
      };

	if(cg_advScoreboard.integer!=0)
 	{
  		if(largeFormat)
    		{
			iconx = SB_BOTICON_X_ADV;
			headx = SB_HEAD_X_ADV;
  		} else
    		{
 			iconx = SB_BOTICON_INTER_X_ADV;
			headx = SB_HEAD_INTER_X_ADV;
	     	}
  	}
   	else
    	{
		iconx = SB_BOTICON_X;
		headx = SB_HEAD_X;
 	};
  
	// ======================
	// SHUD adv. scores [end]
  	// ======================
	// draw the handicap or bot skill marker (unless player has flag)
	if ( ci->powerups & ( 1 << PW_REDFLAG ) ) {
		if( largeFormat ) {
			CG_DrawFlagModel( iconx, y - ( 32 - BIGCHAR_HEIGHT ) / 2, 32, 32, TEAM_RED );
		}
		else {
			CG_DrawFlagModel( iconx, y, 16, 16, TEAM_RED );
		}
	} else if ( ci->powerups & ( 1 << PW_BLUEFLAG ) ) {
		if( largeFormat ) {
			CG_DrawFlagModel( iconx, y - ( 32 - BIGCHAR_HEIGHT ) / 2, 32, 32, TEAM_BLUE );
		}
		else {
			CG_DrawFlagModel( iconx, y, 16, 16, TEAM_BLUE );
		}
	} else {
		if ( ci->botSkill > 0 && ci->botSkill <= 5 ) {
			if ( cg_drawIcons.integer ) {
				if( largeFormat ) {
					CG_DrawPic( iconx, y - ( 32 - BIGCHAR_HEIGHT ) / 2, 32, 32, cgs.media.botSkillShaders[ ci->botSkill - 1 ] );
				}
				else {
					CG_DrawPic( iconx, y, 16, 16, cgs.media.botSkillShaders[ ci->botSkill - 1 ] );
				}
			}
		} else if ( ci->handicap < 100 ) {
			Com_sprintf( string, sizeof( string ), "%i", ci->handicap );
			if ( cgs.gametype == GT_TOURNAMENT )
				CG_DrawSmallStringColor( iconx, y - SMALLCHAR_HEIGHT/2, string, color );
			else
				CG_DrawSmallStringColor( iconx, y, string, color );
		}

		// draw the wins / losses
		if ( cgs.gametype == GT_TOURNAMENT ) {
			Com_sprintf( string, sizeof( string ), "%i/%i", ci->wins, ci->losses );
			if( ci->handicap < 100 && !ci->botSkill ) {
				CG_DrawSmallStringColor( iconx, y + SMALLCHAR_HEIGHT/2, string, color );
			}
			else {
				CG_DrawSmallStringColor( iconx, y, string, color );
			}
		}
	}

	// draw the face
	VectorClear( headAngles );
	headAngles[YAW] = 180;
	if( largeFormat ) {
		CG_DrawHead( headx, y - ( ICON_SIZE - BIGCHAR_HEIGHT ) / 2, ICON_SIZE, ICON_SIZE, 
			score->client, headAngles );
	}
	else {
		CG_DrawHead( headx, y, 16, 16, score->client, headAngles );
	}

	// draw the score line

	// ================
	// SHUD adv. scores 
  	// ================
      if(cg_advScoreboard.integer!=0)
      {
//         f = ps->persistant[PERS_ACCURACY_SHOTS];
//         h = ps->persistant[PERS_ACCURACY_HITS];
         f = stats[score->client].shots;
         h = stats[score->client].hits;
         if (f==0) p = 0;
         else p = (h*100)/f;
         if ( score->ping == -1 ) {
		      Com_sprintf(string, sizeof(string),
			      " connecting    %s", ci->name);
	      } else if ( ci->team == TEAM_SPECTATOR ) {
	      	Com_sprintf(string, sizeof(string),
			      " SPECT %3i %3i     %s", score->ping, score->time, ci->name);
	      } else {
            Com_sprintf(string, sizeof(string)," %5i %3i %3i ", score->score, score->ping, score->time);
            if((cg_advScoreboard.integer & 1)==1)
            {
               strcat(string,s0);
            };// else strcat(string," "); // width correction for next fields
            i=0;
            Com_sprintf(s2,sizeof(s2),"%4i",score_data[i]);i++;
            strcat(string,s2);
            while(i<i_score)
            {
               Com_sprintf(s2,sizeof(s2),"%3i",score_data[i]);i++;
               strcat(string,s2);
            };
            Com_sprintf(s2,sizeof(s2),"   %s",ci->name);
            strcat(string,s2);
 	         }
      } else
         {

 	// ======================
	// SHUD adv. scores [end]
  	// ======================

  	   if ( score->ping == -1 ) {
		Com_sprintf(string, sizeof(string),
			" connecting    %s", ci->name);
	   } else if ( ci->team == TEAM_SPECTATOR ) {
		Com_sprintf(string, sizeof(string),
			" SPECT %3i %4i %s", score->ping, score->time, ci->name);
	   } else {
		Com_sprintf(string, sizeof(string),
			"%5i %4i %4i %s", score->score, score->ping, score->time, ci->name);
	   }
 
	} // SHUD

	// highlight your position
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
			hcolor[2] = 0.7;
		} else if ( rank == 1 ) {
			hcolor[0] = 0.7;
			hcolor[1] = 0;
			hcolor[2] = 0;
		} else if ( rank == 2 ) {
			hcolor[0] = 0.7;
			hcolor[1] = 0.7;
			hcolor[2] = 0;
		} else {
			hcolor[0] = 0.7;
			hcolor[1] = 0.7;
			hcolor[2] = 0.7;
		}

		hcolor[3] = fade * 0.7;
   		if(cg_advScoreboard.integer!=0) // SHUD
         		CG_FillRect( SB_SCORELINE_X_ADV + SMALLCHAR_WIDTH, y,
			640 - SB_SCORELINE_X_ADV - BIGCHAR_WIDTH, BIGCHAR_HEIGHT+1, hcolor );
   		else 		
     			CG_FillRect( SB_SCORELINE_X + BIGCHAR_WIDTH, y, 
			640 - SB_SCORELINE_X - BIGCHAR_WIDTH, BIGCHAR_HEIGHT+1, hcolor );
	}

   	if(cg_advScoreboard.integer!=0) // SHUD
      	CG_DrawSmallString( SB_SCORELINE_X_ADV, y, string, fade );
      else
		CG_DrawBigString( SB_SCORELINE_X, y, string, fade );

	// add the "ready" marker for intermission exiting
	if ( cg.snap->ps.stats[ STAT_CLIENTS_READY ] & ( 1 << score->client ) ) {
		CG_DrawBigStringColor( iconx, y, "READY", color );
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
	int		count;
	clientInfo_t	*ci;

	color[0] = color[1] = color[2] = 1.0;
	color[3] = fade;

	count = 0;

	for ( i = 0 ; i < cg.numScores && count < maxClients ; i++ ) {
		score = &cg.scores[i];
		ci = &cgs.clientinfo[ score->client ];

		if ( team != ci->team ) {
			continue;
		}

		CG_DrawClientScore( y + lineHeight * count, score, color, fade, lineHeight == SB_NORMAL_HEIGHT );

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
qboolean CG_DrawScoreboard( void ) {
	int	x, y, w, i, n1, n2, p, pf;
	float	fade;
	float	*fadeColor;
	char	*s;
	int maxClients;
	int lineHeight;
	int topBorderSize, bottomBorderSize;
      char s0[] = "---";
	vec4_t hcolor;
 	
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
           	teamred_getbacks = teamblue_getflag - cg.teamScores[1];
           	teamblue_getbacks = teamred_getflag - cg.teamScores[0];
		
		w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH;
		x = ( SCREEN_WIDTH - w ) / 2;
		y = 60;
		CG_DrawBigString( x, y, s, fade );
	}

	// scoreboard
	y = SB_HEADER;

// =====================
// SHUD : Adv Scoreboard   
// =====================

  if( cg.predictedPlayerState.pm_type == PM_INTERMISSION )
  {
  	if(level_end_time == -1) level_end_time = cg.time;
   	SHUDLog_Status = SHUD_STATUS_NONE;
  	if((cgs.capturelimit!=0)&( cgs.gametype == GT_CTF))
      {
            if((cg.teamScores[0]>=cgs.capturelimit)|(cg.teamScores[1]>=cgs.capturelimit)) SHUDLog_Status = SHUD_STATUS_CAPTURELIMIT_HIT;
      };
  	if((cgs.timelimit!=0) && ((level_end_time-cgs.levelStartTime)>=cgs.timelimit*60000))
      {
      	SHUDLog_Status = SHUD_STATUS_TIMELIMIT_HIT;
      };
  	if(cgs.fraglimit!=0)
      {
      	if( cgs.gametype < GT_TEAM)
       	{
      		x=0;
       		for(i=0;i<cg.numScores;i++) if(x<cg.scores[i].score) x = cg.scores[i].score;
        		if(x>=cgs.fraglimit) SHUDLog_Status = SHUD_STATUS_FRAGLIMIT_HIT;
          	} else if((cg.teamScores[0]>=cgs.fraglimit)|(cg.teamScores[1]>=cgs.fraglimit)) SHUDLog_Status = SHUD_STATUS_FRAGLIMIT_HIT;
      };
  };

  if(cg_advScoreboard.integer!=0)
   {
	   CG_DrawPic( SB_SCORE_X_ADV, y, 40, 20, cgs.media.scoreboardScore );
	   CG_DrawPic( SB_PING_X_ADV, y, 40, 20, cgs.media.scoreboardPing );
	   CG_DrawPic( SB_TIME_X_ADV, y, 40, 20, cgs.media.scoreboardTime );
	   CG_DrawPic( SB_STATS_X_ADV, y, 40, 20, scoreboardStats );
      num_scores = 0;
      x = SB_STATS_X_ADV;
      if((cg_advScoreboard.integer & pow_of2[0])==pow_of2[0])
      {
         CG_DrawPic( x, SB_STATICONS , BIGCHAR_WIDTH, BIGCHAR_HEIGHT,medal_Frags);
         num_scores++;
         x+=4*SMALLCHAR_WIDTH;
      };
      if((cg_advScoreboard.integer & pow_of2[14])==pow_of2[14])
      {
         CG_DrawPic( x, SB_STATICONS , BIGCHAR_WIDTH, BIGCHAR_HEIGHT,medal_Victory);
         num_scores++;
         x+=3*SMALLCHAR_WIDTH;
      };
      if((cg_advScoreboard.integer & pow_of2[1])==pow_of2[1])
      {
         CG_DrawPic( x, SB_STATICONS , BIGCHAR_WIDTH, BIGCHAR_HEIGHT,cgs.media.medalGauntlet);
         num_scores++;
         x+=3*SMALLCHAR_WIDTH;
      };
      
	if((cg_advScoreboard.integer & pow_of2[13])==pow_of2[13]) // Players
 	{
   	   vec3_t headAngles;
         VectorClear(headAngles);
         headAngles[YAW] = 180;
         n1=num_scores;
  	   for(i=0;(num_scores<(n1+cg_clientsOnScoreboard.integer))&&(i<cg.numScores);i++) 
         if(cgs.clientinfo[i].team!=TEAM_SPECTATOR) 
         {
  	      CG_DrawHead( x, SB_STATICONS , BIGCHAR_WIDTH, BIGCHAR_HEIGHT, i, headAngles );
            num_scores++;
          	x+=3*SMALLCHAR_WIDTH;
        };
  	} else for(i=2;i<10;i++) // Weapons
      if((cg_advScoreboard.integer & pow_of2[i])==pow_of2[i])
      if (cg_weapons[i].weaponIcon )
      {
         CG_DrawPic( x, SB_STATICONS , BIGCHAR_WIDTH, BIGCHAR_HEIGHT,cg_weapons[i].weaponIcon );
         num_scores++;
         x+=3*SMALLCHAR_WIDTH;
      };
      if((cg_advScoreboard.integer & pow_of2[10])==pow_of2[10])
      {
         CG_DrawPic( x, SB_STATICONS , BIGCHAR_WIDTH, BIGCHAR_HEIGHT,cgs.media.medalExcellent);
         num_scores++;
         x+=3*SMALLCHAR_WIDTH;
     };
      if((cg_advScoreboard.integer & pow_of2[11])==pow_of2[11])
      {
         CG_DrawPic( x, SB_STATICONS , BIGCHAR_WIDTH, BIGCHAR_HEIGHT,icon_Deaths);
         num_scores++;
         x+=3*SMALLCHAR_WIDTH;
     };
      if((cg_advScoreboard.integer & pow_of2[12])==pow_of2[12])
      {
         CG_DrawPic( x, SB_STATICONS , BIGCHAR_WIDTH, BIGCHAR_HEIGHT,icon_Suicides);
//         CG_DrawSmallStringColor( SB_STATS_X_ADV+1.5*num_scores*BIGCHAR_WIDTH, SB_STATICONS, "S.", fadeColor );
         x+=3*SMALLCHAR_WIDTH;
         num_scores++;
      };
      x+=2*SMALLCHAR_WIDTH;
      if(num_scores==0) num_scores++;
	CG_DrawPic(  x, y, 40, 20, cgs.media.scoreboardName );
   }
     else
   {
// ===========================
// SHUD : Adv Scoreboard [end]   
// ===========================

	CG_DrawPic( SB_SCORE_X, y, 64, 32, cgs.media.scoreboardScore );
	CG_DrawPic( SB_PING_X, y, 64, 32, cgs.media.scoreboardPing );
	CG_DrawPic( SB_TIME_X, y, 64, 32, cgs.media.scoreboardTime );
	CG_DrawPic( SB_NAME_X, y, 64, 32, cgs.media.scoreboardName );
   } // SHUD

	y = SB_TOP;

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

	localClient = qfalse;

	if ( cgs.gametype >= GT_TEAM ) {
		//
		// teamplay scoreboard
		//
		y += lineHeight/2;

		if ( cg.teamScores[0] >= cg.teamScores[1] ) {
			n1 = CG_TeamScoreboard( y, TEAM_RED, fade, maxClients, lineHeight );
			CG_DrawTeamBackground( 0, y - topBorderSize, 640, n1 * lineHeight + bottomBorderSize, 0.33, TEAM_RED );
			y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n1;
			n2 = CG_TeamScoreboard( y, TEAM_BLUE, fade, maxClients, lineHeight );
			CG_DrawTeamBackground( 0, y - topBorderSize, 640, n2 * lineHeight + bottomBorderSize, 0.33, TEAM_BLUE );
			y += (n2 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n2;
		} else {
			n1 = CG_TeamScoreboard( y, TEAM_BLUE, fade, maxClients, lineHeight );
			CG_DrawTeamBackground( 0, y - topBorderSize, 640, n1 * lineHeight + bottomBorderSize, 0.33, TEAM_BLUE );
			y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n1;
			n2 = CG_TeamScoreboard( y, TEAM_RED, fade, maxClients, lineHeight );
			CG_DrawTeamBackground( 0, y - topBorderSize, 640, n2 * lineHeight + bottomBorderSize, 0.33, TEAM_RED );
			y += (n2 * lineHeight) + BIGCHAR_HEIGHT;
			maxClients -= n2;
		}
		n1 = CG_TeamScoreboard( y, TEAM_SPECTATOR, fade, maxClients, lineHeight );
		y += (n1 * lineHeight) + BIGCHAR_HEIGHT;

	} else {
		//
		// free for all scoreboard
		//
		n1 = CG_TeamScoreboard( y, TEAM_FREE, fade, maxClients, lineHeight );
		y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
		n2 = CG_TeamScoreboard( y, TEAM_SPECTATOR, fade, maxClients - n1, lineHeight );
		y += (n2 * lineHeight) + BIGCHAR_HEIGHT;
	}

	if (!localClient) {
		// draw local client at the bottom
		for ( i = 0 ; i < cg.numScores ; i++ ) {
			if ( cg.scores[i].client == cg.snap->ps.clientNum ) {
				CG_DrawClientScore( y, &cg.scores[i], fadeColor, fade, lineHeight == SB_NORMAL_HEIGHT );
				break;
			}
		}
	}

	// load any models that have been deferred
	if ( ++cg.deferredPlayerLoading > 10 ) {
		CG_LoadDeferredPlayers();
	}
 

	// ==============================
	// SHUD : Client summary (bottom)
	// ==============================
	hcolor[0] = 1;
	hcolor[1] = 1;
	hcolor[2] = 1;
	hcolor[3] = 0.33;
	if ((cg.snap->ps.persistant[PERS_TEAM] != TEAM_SPECTATOR ) 
 	   & (cg.predictedPlayerState.pm_type == PM_INTERMISSION)
     	   & (cg_advScoreboard.integer!=0))
 	{
		hcolor[0] = 1;
		hcolor[1] = 1;
		hcolor[2] = 1;
		hcolor[3] = 0.33;
  		x = SMALLCHAR_WIDTH*11;
    		y = 480-BIGCHAR_HEIGHT*2;
		CG_FillRect( 0, y-2 ,  640, BIGCHAR_HEIGHT*2+2, hcolor );
    		CG_DrawSmallString( 0 , y, "Accuracy :", 1);
 		for(i=2;i<MAX_WEAPONS;i++)
      	if (cg_weapons[i].weaponIcon)
       	{
		      if(acc_weap_f[i]==0)
      		{
		         s = s0;
		      } else
		      {
		         p = (acc_weap_h[i]*100)/acc_weap_f[i];
		         pf = ((acc_weap_h[i]*1000)/acc_weap_f[i])-p*10;
		         if(pf!=0) s = va("%i.%i",p,pf);
		         else s = va("%i",p);
		      };
         		CG_DrawPic(x, y, BIGCHAR_WIDTH, BIGCHAR_HEIGHT,cg_weapons[i].weaponIcon );
			CG_DrawSmallString( x + BIGCHAR_WIDTH, y, s, 1);
           		x+=BIGCHAR_WIDTH+SMALLCHAR_WIDTH*4;
          	};
		if(cg.snap->ps.persistant[PERS_ACCURACY_SHOTS]==0)
     		{
	         s = s0;
	      } else
	      {
	         p = (cg.snap->ps.persistant[PERS_ACCURACY_HITS]*100)/cg.snap->ps.persistant[PERS_ACCURACY_SHOTS];
	         pf = ((cg.snap->ps.persistant[PERS_ACCURACY_HITS]*1000)/cg.snap->ps.persistant[PERS_ACCURACY_SHOTS])-p*10;
	         if(pf!=0) s = va("%i.%i",p,pf);
	         else s = va("%i",p);
	      };
        	CG_DrawPic(x, y, BIGCHAR_WIDTH, BIGCHAR_HEIGHT,medal_Accuracy );
		CG_DrawSmallString( x + BIGCHAR_WIDTH, y, s, 1);
            

           	y+=BIGCHAR_HEIGHT;
  		x = SMALLCHAR_WIDTH*13;
    		CG_DrawSmallString( 0 , y, "Other info :", 1);
            CG_DrawPic(x, y, BIGCHAR_WIDTH, BIGCHAR_HEIGHT,iconh_red);
       	CG_DrawSmallString( x+BIGCHAR_WIDTH, y, va("%i",avg_health), 1);
      	x+=BIGCHAR_WIDTH+SMALLCHAR_WIDTH*3;
            CG_DrawPic(x, y, BIGCHAR_WIDTH, BIGCHAR_HEIGHT,cgs.media.armorIcon);
      	CG_DrawSmallString( x+BIGCHAR_WIDTH, y, va("%i",avg_armor), 1);
      	x+=BIGCHAR_WIDTH+SMALLCHAR_WIDTH*3;
            CG_DrawPic(x, y, BIGCHAR_WIDTH, BIGCHAR_HEIGHT,iconr_shard);
      	CG_DrawSmallString( x+BIGCHAR_WIDTH, y, va("%i",armor_picked_up), 1);
      	x+=BIGCHAR_WIDTH+SMALLCHAR_WIDTH*4;
            CG_DrawPic(x, y, BIGCHAR_WIDTH, BIGCHAR_HEIGHT,icon_haste);
      	CG_DrawSmallString( x+BIGCHAR_WIDTH, y, va("%f",max_xyspeed), 1);
      	x+=BIGCHAR_WIDTH+SMALLCHAR_WIDTH*4;
		CG_DrawPic( x, y , BIGCHAR_WIDTH, BIGCHAR_HEIGHT,cgs.media.medalImpressive);
      	CG_DrawSmallString( x+BIGCHAR_WIDTH, y, va("%i",cg.snap->ps.persistant[PERS_IMPRESSIVE_COUNT]), 1);
	}; 
      if (( cgs.gametype >= GT_CTF ) & ( cg.predictedPlayerState.pm_type == PM_INTERMISSION ))
     	{
            y = 480-BIGCHAR_HEIGHT*3;
            hcolor[0] = hcolor[1] = 0;
            CG_FillRect( 0, y-4 ,  320, BIGCHAR_HEIGHT+2, hcolor );
            hcolor[0] = 1; hcolor[2] = 0;
            CG_FillRect( 320, y-4 ,  640, BIGCHAR_HEIGHT+2, hcolor );
            CG_DrawSmallString( 0 , y-2, "Blue -", 1); 	    		
            CG_DrawSmallString( SMALLCHAR_WIDTH*7 , y-2, va("Attacks : %i - Retrieved : %i", teamblue_getflag,teamblue_getbacks ),1); 
            CG_DrawSmallString( 320 , y-2, "Red -", 1);	    		
            CG_DrawSmallString( 320+SMALLCHAR_WIDTH*6 , y-2, va("Attacks : %i - Retrieved : %i", teamred_getflag,teamred_getbacks ),1); 
      };

	// ==============================
	// SHUD : Client summary (bottom) [end]
	// ==============================

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
void CG_DrawTourneyScoreboard( void ) {
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

