// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_scoreboard -- draw the scoreboard on top of the game screen
#include "cg_local.h"

#define	SCOREBOARD_X		(0)
#define SB_HEADER			64
#define SB_TOP				(SB_HEADER+32)
// Where the status bar starts, so we don't overwrite it
#define SB_STATUSBAR		420

#define SB_NORMAL_HEIGHT	20
#define SB_INTER_HEIGHT		16 // interleaved height

#define SB_MAXCLIENTS_NORMAL  ((SB_STATUSBAR - SB_TOP) / SB_NORMAL_HEIGHT)
#define SB_MAXCLIENTS_INTER   ((SB_STATUSBAR - SB_TOP) / SB_INTER_HEIGHT - 1)

// Normal
#define SB_BOTICON_X		(SCOREBOARD_X+32)
#define SB_HEAD_X			(SCOREBOARD_X+64)
#define SB_SCORELINE_X		64
#define SB_RATING_WIDTH	    (6 * BIGCHAR_WIDTH) // width 6

//static qbool localClient; // true if local client has been displayed

/*
=================
CG_DrawScoreboard
=================
*/
static void CG_DrawClientScore( float y, score_t *score, float *color, float fade, qbool largeFormat ) {

	char	string[ 64 ];
	clientInfo_t	*ci;
	int iconx, headx;
	vec4_t c;
	
	// highlight your position
	if ( score->client == cg.snap->ps.clientNum ) {
		float	hcolor[4];
		int		rank;

		//localClient = qtrue;

		if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR || cgs.gametype == GT_TEAMRUN  || cgs.gametype == GT_FREESTYLE || cgs.gametype == GT_ADVENTURE ) {
			rank = -1;
		} else {
			rank = cg.snap->ps.persistant[PERS_RANK] & ~RANK_TIED_FLAG;
		}

		hcolor[0] = 0.0f;
		hcolor[1] = 0.0f;
		hcolor[2] = 0.7f;
		hcolor[3] = fade * 0.5;

		CG_FillRect( SB_HEAD_X, y,	640-SB_HEAD_X, SMALLCHAR_HEIGHT+1, hcolor );
	}

	VectorSet( c, 1, 1, 1 ); c[3] = fade;

	if ( score->client < 0 || score->client >= cgs.maxclients ) {
		Com_Printf( "Bad score->client: %i\n", score->client );
		return;
	}

	ci = &cgs.clientinfo[score->client];
	if ( !ci->infoValid )
		return;

	iconx = SB_BOTICON_X + (SB_RATING_WIDTH / 2);
	headx = SB_HEAD_X + (SB_RATING_WIDTH / 2);

	trap_R_SetColor( NULL );

	// draw the score line
	if ( score->ping == -1 ) {
		BG_sprintf( string, " connecting" );
	} else if ( ci->team == TEAM_SPECTATOR ) {
		if ( ci->name == cgs.clientinfo[cg.snap->ps.clientNum].name )
			BG_sprintf(string, " FREE SPECTATOR" );	
		else
			BG_sprintf(string, " FOLLOWING %s", cgs.clientinfo[cg.snap->ps.clientNum].name);
		
	} else {
		
		if (cgs.gametype != GT_FREESTYLE ) {
			int mins, tens, secs, msec = score->score;

			secs = msec / 1000;
			mins = secs / 60;
			secs -= mins * 60;
			tens = secs / 10;
			secs -= tens * 10;
			msec = msec % 1000;
			BG_sprintf( string, "^7%10s %3i ms", va("%d:%d%d.%03d", mins, tens, secs, msec), score->ping ) ;
		} else {
			BG_sprintf( string, "%3i ms", score->ping ) ;
		}
	}

	CG_DrawString( SB_SCORELINE_X + (SB_RATING_WIDTH / 2)+SMALLCHAR_WIDTH*16, y, string, c, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0, DS_SHADOW /*| DS_PROPORTIONAL*/ /*| DS_FORCE_COLOR*/ );
	// name
	CG_DrawString( SB_SCORELINE_X + (SB_RATING_WIDTH / 2), y, ci->name, c, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 12, DS_SHADOW );

	// add the "ready" marker for intermission exiting
	if ( (cgs.gametype == GT_TEAMRUN) && cg.snap->ps.stats[ STAT_CLIENTS_READY ] & ( 1 << score->client ) ) {
		CG_DrawString( iconx, y, "READY", color, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0, DS_SHADOW | DS_FORCE_COLOR );
	}

	// set bounds for scoreboard clicks
	score->minx = SB_SCORELINE_X;
	score->maxx = SCREEN_WIDTH - 8;
	score->miny = y;
	score->maxy = y + SMALLCHAR_HEIGHT;
	if ( largeFormat )
	{
		score->miny -= ( ICON_SIZE - SMALLCHAR_HEIGHT ) / 2;
		score->maxy += ( ICON_SIZE - SMALLCHAR_HEIGHT ) / 2;
	}
}

/*
=================
CG_Scoreboard
=================
*/
static int CG_Scoreboard( int y, team_t team, float fade, int maxClients, int lineHeight ) {
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

		CG_DrawClientScore( y + lineHeight * count, score, color, fade, qfalse /*lineHeight == SB_NORMAL_HEIGHT*/ );

		count++;
	}

	return count;
}


/*
=================
CG_ScoreboardClick
=================
*/
void CG_ScoreboardClick( void )
{
	score_t	*score;
	int i;

	if ( cg.intermissionStarted )
		return;

	if ( !cg.snap || cg.snap->ps.pm_type == PM_INTERMISSION )
		return;

	score = cg.scores;
	for ( i = 0; i < cg.numScores; i++, score++ ) {
		if ( score->team >= TEAM_SPECTATOR ) {
			continue;
		}
		if ( cgs.cursorX < score->minx || cgs.cursorX > score->maxx )
			continue;
		if ( cgs.cursorY < score->miny || cgs.cursorY > score->maxy )
			continue;
		if ( !cgs.clientinfo[ score->client ].infoValid ) {
			continue;
		}

		if ( !cg.demoPlayback ) {
			trap_SendClientCommand( va( "follow %i", score->client ) );
		}
	}
}

static const char *gameNames[] = {
	"Fastcap",
	"Adventure",
	"Defrag",
	"Teamrun",
	"Freestyle"
};
/*
static const char *fcmodes[] = {
	"fc_default",
	"fc_noteles",
	"fc_nopads",
	"fc_nodoors",
	"fc_noteles_nopads"
	"fc_disableall",
	"fc_rocket",
	"fc_plasma",
	"fc_combo"
};
*/
/*
static const char *dfmodes[] = {
	"df_default",
	"df_strafe",
	"df_rocket",
	"df_plasma",
	"df_combo",
	"df_icemode"
};
*/

/*
static const char *onoff[] = {
	"off",
	"on"
};
*/

static const char *rsNames[] = {
	"Vq3",
	"Cpm",
	"xVq3",
	"xCpm"
};

/*
=================
CG_DrawScoreboard

Draw the normal in-game scoreboard
=================
*/
qbool CG_DrawScoreboard( void ) {
    if ( cgs.gametype != GT_TEAMRUN ) {
            float 	y;
			int		i, n1, n2, w;
            int     secrets, secretsCount;
            float	fade;
            float	*fadeColor;
            char	*s;
            int maxClients;
            int lineHeight;
            int topBorderSize, bottomBorderSize;
            qtime_t time;
			qhandle_t hshader;
			float lx, ly, lw, lh;

            // don't draw anything if the menu or console is up
            if ( cg_paused.integer ) {
                cg.deferredPlayerLoading = 0;
                return qfalse;
            }
/*
            if ( cgs.gametype == GT_DEFRAG && cg.predictedPlayerState.pm_type == PM_INTERMISSION ) {
                cg.deferredPlayerLoading = 0;
                return qfalse;
            }
*/
            // don't draw scoreboard during death while warmup up
            if ( cg.warmup && !cg.showScores ) {
                return qfalse;
            }

            if ( cg.showScores || cg.predictedPlayerState.pm_type == PM_DEAD || cg.predictedPlayerState.pm_type == PM_INTERMISSION ) {
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
                fade = fadeColor[3];
            }

            // scoreboard
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

			// draw background shader
			lx = SB_HEAD_X;
			ly = SB_HEADER;
			lw = 640 - SB_HEAD_X;
			lh = 400-ly;
			CG_AdjustFrom640(&lx, &ly, &lw, &lh);
			trap_R_SetColor( NULL );
			trap_R_DrawStretchPic (lx, ly, lw, lh, 0, 0, 1, 1, cgs.media.sbback);
			
			// draw header background shader
			lx = SB_HEAD_X;
			ly = SB_HEADER;
			lw = 640 - SB_HEAD_X;
			lh = 30;
			CG_AdjustFrom640(&lx, &ly, &lw, &lh);
			trap_R_DrawStretchPic (lx, ly, lw, lh, 0, 0, 1, 1, cgs.media.sbheader);
			
			//
			// rocket icon
			//
			if ( cgs.dfmode == DF_COMBO || cgs.dfmode == DF_ROCKET || cgs.fcmode == FC_COMBO || cgs.fcmode == FC_ROCKET ) {
				lx = 640 - SCOREBOARD_X - ( ICON_SIZE * 10);
				ly = SB_HEADER+8;
				lw = 16;
				lh = 16;
				CG_AdjustFrom640(&lx, &ly, &lw, &lh);
				trap_R_DrawStretchPic( lx, ly, lw, lh, 0, 0, 1, 1, cgs.media.sbrocket );
			}
			
			//
			// plasma icon
			//
			if ( cgs.dfmode == DF_COMBO || cgs.dfmode == DF_PLASMA || cgs.fcmode == FC_COMBO || cgs.fcmode == FC_PLASMA ) {
				lx = 640 - SCOREBOARD_X - ( ICON_SIZE * 9);
				ly = SB_HEADER+8;
				lw = 16;
				lh = 16;
				CG_AdjustFrom640(&lx, &ly, &lw, &lh);
				trap_R_DrawStretchPic( lx, ly, lw, lh, 0, 0, 1, 1, cgs.media.sbplasma );
			}
			
			//
			// grenade icon
			//
			if ( cgs.dfmode == DF_COMBO || cgs.fcmode == FC_COMBO ) {
				lx = 640 - SCOREBOARD_X - ( ICON_SIZE * 8);
				ly = SB_HEADER+8;
				lw = 16;
				lh = 16;
				CG_AdjustFrom640(&lx, &ly, &lw, &lh);
				trap_R_DrawStretchPic( lx, ly, lw, lh, 0, 0, 1, 1, cgs.media.sbgrenade );
			}
			
			//
			// slick icon
			//
			if ( cgs.dfmode == DF_ICEMODE ) {
				lx = 640 - SCOREBOARD_X - ( ICON_SIZE * 7);
				ly = SB_HEADER+8;
				lw = 16;
				lh = 16;
				CG_AdjustFrom640(&lx, &ly, &lw, &lh);
				trap_R_DrawStretchPic( lx, ly, lw, lh, 0, 0, 1, 1, cgs.media.sbslick );
			}
			
			//
			// tele icon
			//
			if ( (cgs.fcmode < FC_DISABLEALL) && (cgs.fcmode != FC_NOTELES) && (cgs.fcmode != FC_NOTELES_NOPADS ) ) {
				lx = 640 - SCOREBOARD_X - ( ICON_SIZE * 6);
				ly = SB_HEADER+8;
				lw = 16;
				lh = 16;
				CG_AdjustFrom640(&lx, &ly, &lw, &lh);
				trap_R_DrawStretchPic( lx, ly, lw, lh, 0, 0, 1, 1, cgs.media.sbtele );
			}
			
			//
			// pad icon
			//
			if ( (cgs.fcmode < FC_DISABLEALL) && (cgs.fcmode != FC_NOPADS) && (cgs.fcmode != FC_NOTELES_NOPADS ) ) {
				lx = 640 - SCOREBOARD_X - ( ICON_SIZE * 5);
				ly = SB_HEADER+8;
				lw = 16;
				lh = 16;
				CG_AdjustFrom640(&lx, &ly, &lw, &lh);
				trap_R_DrawStretchPic( lx, ly, lw, lh, 0, 0, 1, 1, cgs.media.sbpad );
			}
			
			//
			// door icon
			//
			if ( (cgs.fcmode < FC_DISABLEALL) && (cgs.fcmode != FC_NODOORS) ) {
				lx = 640 - SCOREBOARD_X - ( ICON_SIZE * 4);
				ly = SB_HEADER+8;
				lw = 16;
				lh = 16;
				CG_AdjustFrom640(&lx, &ly, &lw, &lh);
				trap_R_DrawStretchPic( lx, ly, lw, lh, 0, 0, 1, 1, cgs.media.sbdoor );
			}
			
			//
			// overbounces icon
			//
			if ( cgs.obs ) {
				lx = 640 - SCOREBOARD_X - ( ICON_SIZE * 3);
				ly = SB_HEADER+8;
				lw = 16;
				lh = 16;
				CG_AdjustFrom640(&lx, &ly, &lw, &lh);
				trap_R_DrawStretchPic( lx, ly, lw, lh, 0, 0, 1, 1, cgs.media.sboverbounce );
			}
			
			//
			// interference icon
			//
			if ( cgs.interference ) {
				lx = 640 - SCOREBOARD_X - ( ICON_SIZE * 2);
				ly = SB_HEADER+8;
				lw = 16;
				lh = 16;
				CG_AdjustFrom640(&lx, &ly, &lw, &lh);
				trap_R_DrawStretchPic( lx, ly, lw, lh, 0, 0, 1, 1, cgs.media.sbinterference );
			}
			
			//
			// reversemap icon
			//
			if (/*cgs.gametype == GT_DEFRAG &&*/ cgs.reversemap ) {
				lx = 640 - SCOREBOARD_X - ICON_SIZE;
				ly = SB_HEADER+8;
				lw = 16;
				lh = 16;
				CG_AdjustFrom640(&lx, &ly, &lw, &lh);
				trap_R_DrawStretchPic( lx, ly, lw, lh, 0, 0, 1, 1, cgs.media.sbreverse );
			}
			
			
			// print gametype / ruleset
			s = va( "%s %s",  gameNames[cgs.gametype], rsNames[cgs.ruleset] );
			w = CG_DrawStrlen( s ) * 6;
            CG_DrawStringExt( SB_SCORELINE_X + (SB_RATING_WIDTH)-w, SB_HEADER+8, s, colorBlackTrans, qtrue, qfalse, 6, 12, 64 );
			
			// print mapname
            s = Info_ValueForKey( CG_ConfigString( CS_SERVERINFO ), "mapname" );
            w = CG_DrawStrlen( s ) * 5;
            CG_DrawStringExt( 320 - w, SB_HEADER+8 , s, colorBlackTrans, qtrue, qfalse, 8, 16, 24 );

			// scoreboard
			n1 = CG_Scoreboard( y, TEAM_FREE, fade, maxClients, lineHeight );
			y += (n1 * lineHeight) + BIGCHAR_HEIGHT;
			n2 = CG_Scoreboard( y, TEAM_SPECTATOR, fade, maxClients - n1, lineHeight );
			y += (n2 * lineHeight) + BIGCHAR_HEIGHT;
			
            // load any models that have been deferred
            if ( ++cg.deferredPlayerLoading > 10 ) 
                CG_LoadDeferredPlayers();
				
			
			// draw infoline background shader	
			lx = SB_HEAD_X;
			ly = 475-95;
			lw = 640 - SB_HEAD_X;
			lh = 400-ly;
			CG_AdjustFrom640(&lx, &ly, &lw, &lh);
			trap_R_SetColor( NULL );
			trap_R_DrawStretchPic (lx, ly, lw, lh, 0, 0, 1, 1, cgs.media.sbbottom);
			
			
			
			/*
			// print mode/fastcapmode
			switch ( cgs.gametype ) {
				case GT_FASTCAP:
					// print fastcapmode
					s = va( "Mode: %s", fcmodes[cgs.fcmode] );
					break;
				case GT_DEFRAG:
					// print fastcapmode
					s = va( "Mode: %s", dfmodes[cgs.dfmode] );
					break;
			}
			w = CG_DrawStrlen( s ) * 6;
			if (cgs.gametype == GT_FASTCAP || cgs.gametype == GT_DEFRAG )
				CG_DrawStringExt( 640- SB_HEAD_X - w, SB_HEADER+8, s, colorRed, qtrue, qfalse, 6, 12, 64 );
			*/
			
			/*
			// print obs
			s = va( "OBs are: %s", onoff[ cgs.obs ] );
			w = CG_DrawStrlen( s ) * 6;
            CG_DrawStringExt(  640- SB_HEAD_X - w, SB_HEADER+16, s, colorRed, qfalse, qfalse, 6, 12, 64 );
			*/
			
			 // print time (bottom line)
            trap_RealTime( &time );
            s = va("%02d:%02d:%02d", time.tm_hour, time.tm_min, time.tm_sec );
            w = CG_DrawStrlen( s ) * 6;
            CG_DrawStringExt( 635 - w - 6, 475 - 90, s, colorCyan, qtrue, qfalse, 6, 12, 64 );
			
            /*
			if ( cgs.gametype == GT_ADVENTURE ) {
				// print secrets found/count
				secrets = (cg.snap->ps.persistant[PERS_SECRETS] & 0x7F);
				secretsCount = ((cg.snap->ps.persistant[PERS_SECRETS] >> 7) & 0x7F);
				CG_DrawStringExt( 0, 10 , va("       Secrets : (%i/%i)", secrets, secretsCount), colorWhite, qtrue, qtrue, 5, 10, 0 );
			}
			*/
            return qtrue;
    }

    return qfalse;

}
