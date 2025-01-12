// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_servercmds.c -- reliably sequenced text commands sent by the server
// these are processed at snapshot transition time, so there will definately
// be a valid snapshot this frame

#include "cg_local.h"
#include "bg_shared.h"

/*
=================
CG_ParseScores

=================
*/

#define SCORE_ITEMS	15
static void CG_ParseScores( void )
{
	int		i, powerups;

	cg.numScores = atoi( CG_Argv( 1 ) );
	if ( cg.numScores > MAX_CLIENTS )
    {
		cg.numScores = MAX_CLIENTS;
	}

	cg.teamScores[0] = atoi( CG_Argv( 2 ) );
	cg.teamScores[1] = atoi( CG_Argv( 3 ) );

	memset( cg.scores, 0, sizeof( cg.scores ) );
	for ( i = 0 ; i < cg.numScores ; i++ )
    {
		cg.scores[i].client = atoi( CG_Argv( i * SCORE_ITEMS + 4 ) );
		cg.scores[i].score = atoi( CG_Argv( i * SCORE_ITEMS + 5 ) );
		cg.scores[i].ping = atoi( CG_Argv( i * SCORE_ITEMS + 6 ) );
		cg.scores[i].time = atoi( CG_Argv( i * SCORE_ITEMS + 7 ) );
		cg.scores[i].scoreFlags = atoi( CG_Argv( i * SCORE_ITEMS + 8 ) );
		powerups = atoi( CG_Argv( i * SCORE_ITEMS + 9 ) );
		cg.scores[i].accuracy = atoi(CG_Argv(i * SCORE_ITEMS + 10));
		cg.scores[i].impressiveCount = atoi(CG_Argv(i * SCORE_ITEMS + 11));
		cg.scores[i].excellentCount = atoi(CG_Argv(i * SCORE_ITEMS + 12));
		cg.scores[i].gauntletCount = atoi(CG_Argv(i * SCORE_ITEMS + 13));
		cg.scores[i].defendCount = atoi(CG_Argv(i * SCORE_ITEMS + 14));
		cg.scores[i].assistCount = atoi(CG_Argv(i * SCORE_ITEMS + 15));
		cg.scores[i].perfect = atoi(CG_Argv(i * SCORE_ITEMS + 16));
		//cg.scores[i].captures = atoi(CG_Argv(i * SCORE_ITEMS + 17));
		cg.scores[i].keys = atoi(CG_Argv(i * SCORE_ITEMS + 17));

		if ( cg.scores[i].client < 0 || cg.scores[i].client >= MAX_CLIENTS )
        {
			cg.scores[i].client = 0;
		}
		cgs.clientinfo[ cg.scores[i].client ].score = cg.scores[i].score;
		cgs.clientinfo[ cg.scores[i].client ].powerups = powerups;

		cg.scores[i].team = cgs.clientinfo[cg.scores[i].client].team;
	}
}

/*
================
CG_ParseServerinfo

This is called explicitly when the gamestate is first received,
and whenever the server updates any serverinfo flagged cvars
================
*/
void CG_ParseServerinfo( void )
{
	const char	*info;
	char	*mapname;

	info = CG_ConfigString( CS_SERVERINFO );
	cgs.gametype = atoi( Info_ValueForKey( info, "dfx_gametype" ) );
	cgs.ruleset = atoi( Info_ValueForKey( info, "dfx_ruleset") );
	cgs.dfmode =  atoi( Info_ValueForKey( info, "dfx_mode") );
	cgs.fcmode =  atoi( Info_ValueForKey( info, "dfx_fastcapmode") );
	cgs.obs = ( atoi( Info_ValueForKey( info, "dfx_obs") ) ) ? qtrue : qfalse;
	cgs.interference = ( atoi( Info_ValueForKey( info, "dfx_interference") ) ) ? qtrue : qfalse;
	cgs.reversemap = ( atoi( Info_ValueForKey( info, "dfx_reversemap") ) ) ? qtrue : qfalse;

	cgs.dmflags = atoi( Info_ValueForKey( info, "dmflags" ) );
	cgs.teamflags = atoi( Info_ValueForKey( info, "teamflags" ) );

	cgs.fraglimit = atoi( Info_ValueForKey( info, "fraglimit" ) );

	cgs.maxclients = atoi( Info_ValueForKey( info, "sv_maxclients" ) );
	mapname = Info_ValueForKey( info, "mapname" );
	Com_sprintf( cgs.mapname, sizeof( cgs.mapname ), "maps/%s.bsp", mapname );
	//Q_strncpyz( cgs.redTeam, Info_ValueForKey( info, "g_redTeam" ), sizeof(cgs.redTeam) );
	//Q_strncpyz( cgs.blueTeam, Info_ValueForKey( info, "g_blueTeam" ), sizeof(cgs.blueTeam) );
}


void CG_ParseSysteminfo( void )
{
	const char	*info;

	info = CG_ConfigString( CS_SYSTEMINFO );

	cgs.pmove_fixed = ( atoi( Info_ValueForKey( info, "pmove_fixed" ) ) ) ? qtrue : qfalse;
	cgs.pmove_msec = atoi( Info_ValueForKey( info, "pmove_msec" ) );
	if ( cgs.pmove_msec < 8 )
    {
		cgs.pmove_msec = 8;
	} else if ( cgs.pmove_msec > 33 )
	{
		cgs.pmove_msec = 33;
	}

	cgs.synchronousClients = ( atoi( Info_ValueForKey( info, "g_synchronousClients" ) ) ) ? qtrue : qfalse;
}


/*
==================
CG_ParseWarmup
==================
*/
static void CG_ParseWarmup( void )
{
	const char	*info;
	int			warmup;

	info = CG_ConfigString( CS_WARMUP );

	warmup = atoi( info );
	cg.warmupCount = -1;

	if ( warmup ) {
		cg.fraglimitWarnings |= 1 | 2 | 4;
	}

	if ( cg.clientFrame == 0 ) {
		if ( warmup == 0 && cgs.gametype == GT_TEAMRUN ) {
			if ( cg.snap && ( cg.snap->ps.persistant[PERS_TEAM] != TEAM_SPECTATOR || cg.snap->ps.pm_flags & PMF_FOLLOW ) ) {
				// force sound playback in CG_WarmupEvents()
				cg.warmup = cg.time;
				cg.warmupCount = -2; // special value to silent FIGHT sound for demo playback
			}
			return;
		}
	}

	/*
	if ( warmup == 0 && cg.warmup ) {

	} else if ( warmup > 0 && cg.warmup <= 0 ) {

        if ( cg.soundPlaying != cgs.media.countPrepareSound ) {
            CG_AddBufferedSound( -1 );
            //CG_AddBufferedSound( cgs.media.countPrepareSound );
            cg.soundTime = cg.time + 1; // play in next frame
        }

	}
	*/
	
	cg.warmup = warmup;
}


/*
================
CG_SetConfigValues

Called on load to set the initial values from configure strings
================
*/
void CG_SetConfigValues( void ) {
	const char *s;

	cgs.scores1 = atoi( CG_ConfigString( CS_SCORES1 ) );
	cgs.scores2 = atoi( CG_ConfigString( CS_SCORES2 ) );
	cgs.scores3 = atoi( CG_ConfigString( CS_SCORES3 ) );
	cgs.scores4 = atoi( CG_ConfigString( CS_SCORES4 ) );
	cgs.levelStartTime = atoi( CG_ConfigString( CS_LEVEL_START_TIME ) );
	if( cgs.gametype == GT_FASTCAP ) {
		s = CG_ConfigString( CS_FLAGSTATUS );
		cgs.redflag = s[0] - '0';
		cgs.blueflag = s[1] - '0';
	}

    CG_ParseWarmup();
}


/*
=====================
CG_ShaderStateChanged
=====================
*/
void CG_ShaderStateChanged(void)
{
	char originalShader[MAX_QPATH];
	char newShader[MAX_QPATH];
	char timeOffset[16];
	const char *o;
	char *n,*t;

	o = CG_ConfigString( CS_SHADERSTATE );
	while (o && *o) {
		n = strchr(o, '=');
		if (n) {
			strncpy(originalShader, o, n-o);
			originalShader[n-o] = '\0';
			n++;
			t = strchr(n, ':');
			if (t) {
				strncpy(newShader, n, t-n);
				newShader[t-n] = '\0';
			} else {
				break;
			}
			t++;
			o = strchr(t, '@');
			if (o) {
				strncpy(timeOffset, t, o-t);
				timeOffset[o-t] = 0;
				o++;
				trap_R_RemapShader( originalShader, newShader, timeOffset );
			}
		} else {
			break;
		}
	}
}


/*
================
CG_ConfigStringModified

================
*/
static void CG_ConfigStringModified( void )
{
	const char	*str;
	int		num;

	num = atoi( CG_Argv( 1 ) );

	// get the gamestate from the client system, which will have the
	// new configstring already integrated
	trap_GetGameState( &cgs.gameState );

	// look up the individual string that was modified
	str = CG_ConfigString( num );

	// do something with it if necessary
	if ( num == CS_MUSIC ) {
		CG_StartMusic();
	} else if ( num == CS_SYSTEMINFO ) {
		CG_ParseSysteminfo();
	} else if ( num == CS_SERVERINFO ) {
		CG_ParseServerinfo();
	} else if ( num == CS_WARMUP ) {
		CG_ParseWarmup();
	} else if ( num == CS_SCORES1 ) {
		cgs.scores1 = atoi( str );
	} else if ( num == CS_SCORES2 ) {
		cgs.scores2 = atoi( str );
	} else if ( num == CS_SCORES3 ) {
		cgs.scores3 = atoi( str );
	} else if ( num == CS_SCORES4 ) {
		cgs.scores4 = atoi( str );
	} else if ( num == CS_LEVEL_START_TIME ) {
		cgs.levelStartTime = atoi( str );
	} else if ( num == CS_VOTE_TIME ) {
		cgs.voteTime = atoi( str );
		cgs.voteModified = qtrue;
	} else if ( num == CS_VOTE_YES ) {
		cgs.voteYes = atoi( str );
		cgs.voteModified = qtrue;
	} else if ( num == CS_VOTE_NO ) {
		cgs.voteNo = atoi( str );
		cgs.voteModified = qtrue;
	} else if ( num == CS_VOTE_STRING ) {
		Q_strncpyz( cgs.voteString, str, sizeof( cgs.voteString ) );
	} else if ( num == CS_INTERMISSION ) {
		cg.intermissionStarted = atoi( str );
	} else if ( num >= CS_MODELS && num < CS_MODELS+MAX_MODELS ) {
		cgs.gameModels[ num-CS_MODELS ] = trap_R_RegisterModel( str );
	} else if ( num >= CS_SOUNDS && num < CS_SOUNDS+MAX_SOUNDS ) {
		if ( str[0] != '*' ) {	// player specific sounds don't register here
			cgs.gameSounds[ num-CS_SOUNDS] = trap_S_RegisterSound( str, qfalse );
		}
	} else if ( num >= CS_PLAYERS && num < CS_PLAYERS+MAX_CLIENTS ) {
		CG_NewClientInfo( num - CS_PLAYERS );
		CG_BuildSpectatorString();
	} else if ( num == CS_FLAGSTATUS ) {
		if( cgs.gametype == GT_FASTCAP ) {
			// format is rb where its red/blue, 0 is at base, 1 is taken, 2 is dropped
			cgs.redflag = str[0] - '0';
			cgs.blueflag = str[1] - '0';
		}

	} else if ( num == CS_SHADERSTATE ) {
		CG_ShaderStateChanged();

	} else if ( num == CS_GAMETYPE ||
                num == CS_RULESET ||
                num == CS_DFMODE ||
                num == CS_FCMODE ||
                num == CS_OVERBOUNCES ||
                num == CS_INTERFERENCE ||
                num == CS_REVERSEMAP ) {
                    BG_SharedCvarsUpdate( cgs.gametype, cgs.ruleset, cgs.dfmode, cgs.fcmode, cgs.obs, cgs.interference, cgs.reversemap );
	}
}


/*
=======================
CG_AddToTeamChat

=======================
*/
static void CG_AddToTeamChat( const char *str ) {
	int len;
	char *p, *ls;
	int lastcolor;
	int chatHeight;

	chatHeight = TEAMCHAT_HEIGHT;

	if (chatHeight <= 0 ) {
		// team chat disabled, dump into normal chat
		cgs.teamChatPos = cgs.teamLastChatPos = 0;
		return;
	}

	len = 0;

	p = cgs.teamChatMsgs[cgs.teamChatPos % chatHeight];
	*p = 0;

	lastcolor = '7';

	ls = NULL;
	while (*str) {
		if (len > TEAMCHAT_WIDTH - 1) {
			if (ls) {
				str -= (p - ls);
				str++;
				p -= (p - ls);
			}
			*p = 0;

			cgs.teamChatMsgTimes[cgs.teamChatPos % chatHeight] = cg.time;

			cgs.teamChatPos++;
			p = cgs.teamChatMsgs[cgs.teamChatPos % chatHeight];
			*p = 0;
			*p++ = Q_COLOR_ESCAPE;
			*p++ = lastcolor;
			len = 0;
			ls = NULL;
		}

		if ( Q_IsColorString( str ) ) {
			*p++ = *str++;
			lastcolor = *str;
			*p++ = *str++;
			continue;
		}
		if (*str == ' ') {
			ls = p;
		}
		*p++ = *str++;
		len++;
	}
	*p = 0;

	cgs.teamChatMsgTimes[cgs.teamChatPos % chatHeight] = cg.time;
	cgs.teamChatPos++;

	if (cgs.teamChatPos - cgs.teamLastChatPos > chatHeight)
		cgs.teamLastChatPos = cgs.teamChatPos - chatHeight;
}

/*
===============
CG_MapRestart

The server has issued a map_restart, so the next snapshot
is completely new and should not be interpolated to.

A tournement restart will clear everything, but doesn't
require a reload of all the media
===============
*/
static void CG_MapRestart( void )
{
    int i;

	if ( cg_showmiss.integer )
		CG_Printf( "CG_MapRestart\n" );

	CG_InitLocalEntities();
	CG_InitMarkPolys();
	CG_ClearParticles ();

	// SLK: clear all checkpoints
	for ( i = 0; i < MAX_CHECKPOINTS; i++ )
    {
		cg.bestCheckpoints[i] = 0;
	}

	// make sure the "3 frags left" warnings play again
	cg.fraglimitWarnings = 0;
	cg.rewardTime = 0;
	cg.rewardStack = 0;
	cg.intermissionStarted = qfalse;
	//cg.levelShot = qfalse;

	cgs.voteTime = 0;

	cg.mapRestart = qtrue;

	CG_StartMusic();

	trap_S_ClearLoopingSounds( qtrue );

	cg.allowPickupPrediction = cg.time + PICKUP_PREDICTION_DELAY;

	// we really should clear more parts of cg here and stop sounds

	// play the "fight" sound if this is a restart without warmup
	if ( cg.warmup == 0 ) {
		// force sound playback in CG_WarmupEvents()
		cg.warmup = cg.time;
		cg.warmupCount = -1;
	}

}


/*
=================
CG_RemoveChatEscapeChar
=================
*/
static void CG_RemoveChatEscapeChar( char *text ) {
	int i, l;

	l = 0;
	for ( i = 0; text[i]; i++ ) {
		if (text[i] == '\x19')
			continue;
		text[l] = text[i];
		l++;
	}
	text[l] = '\0';
}



void CG_AddCheckpoint( int time )
{
	if ( cg.currentCheckpoint < MAX_CHECKPOINTS )
    {
		cg.checkpoints[cg.currentCheckpoint] = time;

		if ( cg.bestCheckpoints[cg.currentCheckpoint] )
        {
			int delta = cg.bestCheckpoints[cg.currentCheckpoint] - time;
			if ( ( cgs.gametype == GT_DEFRAG || cgs.gametype == GT_TEAMRUN ) && cp_centerDraw.integer )
                CG_CenterPrint( FormatDFTimer( delta, qtrue ),  200, 10  );
		}
		else
        {
			int prev;

			if ( cg.currentCheckpoint > 0 )
            {
				prev = cg.checkpoints[cg.currentCheckpoint - 1];
			}
			else
            {
				prev = 0;
			}
            if ( ( cgs.gametype == GT_DEFRAG || cgs.gametype == GT_TEAMRUN ) && cp_centerDraw.integer )
                CG_CenterPrint( FormatDFTimer( time - prev, qfalse ),  200, 10  );
		}

		cg.currentCheckpoint++;
	}
}


/*
=================
CG_ServerCommand

The string has been tokenized and can be retrieved with
Cmd_Argc() / Cmd_Argv()
=================
*/
static void CG_ServerCommand( void )
{
	const char	*cmd, *id;
	char		text[MAX_SAY_TEXT];

	cmd = CG_Argv(0);

	if ( !cmd[0] )
    {
		// server claimed the command
		return;
	}

	if ( !strcmp( cmd, "cp" ) ) {
		CG_CenterPrint( CG_Argv(1), SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );
		return;
	}

	if ( !strcmp( cmd, "cpsm" ) ) {
		CG_CenterPrint( CG_Argv(1), 200, 10 );
		return;
	}

	if ( !strcmp( cmd, "cs" ) )
    {
		CG_ConfigStringModified();
		return;
	}

	if ( !strcmp( cmd, "print" ) )
    {
		CG_Printf( "%s", CG_Argv(1) );
		return;
	}

	if ( !strcmp( cmd, "chat" ) )
    {
        if ( !cg_noChatBeep.integer ) {
            trap_S_StartLocalSound( cgs.media.talkSound, CHAN_LOCAL_SOUND );
        }

        Q_strncpyz( text, CG_Argv(1), MAX_SAY_TEXT );
        CG_RemoveChatEscapeChar( text );
        id = CG_Argv( 2 );
        if ( *id >= '0' && *id <= '9' )
            CG_Printf( "(%i) %s\n", atoi( id ), text );
        else
            CG_Printf( "%s\n", text );

		return;
	}

	if ( !strcmp( cmd, "tchat" ) )
    {
		if ( !cg_noChatBeep.integer ) {
            trap_S_StartLocalSound( cgs.media.talkSound, CHAN_LOCAL_SOUND );
        }
		Q_strncpyz( text, CG_Argv(1), MAX_SAY_TEXT );
		CG_RemoveChatEscapeChar( text );
		CG_AddToTeamChat( text );
		id = CG_Argv( 2 );
		if ( *id >= '0' && *id <= '9' )
			CG_Printf( "(%i) %s\n", atoi( id ), text );
		else
			CG_Printf( "%s\n", text );
		return;
	}

	if ( !strcmp( cmd, "scores" ) )
    {
		CG_ParseScores();
		return;
	}

	if ( !strcmp( cmd, "map_restart" ) )
    {
		CG_MapRestart();
		return;
	}

	if ( Q_stricmp (cmd, "remapShader") == 0 )
	{
		if (trap_Argc() == 4)
		{
			char shader1[MAX_QPATH];
			char shader2[MAX_QPATH];
			char shader3[MAX_QPATH];

			Q_strncpyz(shader1, CG_Argv(1), sizeof(shader1));
			Q_strncpyz(shader2, CG_Argv(2), sizeof(shader2));
			Q_strncpyz(shader3, CG_Argv(3), sizeof(shader3));

			trap_R_RemapShader(shader1, shader2, shader3);
		}

		return;
	}

	// loaddeferred can be both a servercmd and a consolecmd
	if ( !strcmp( cmd, "loaddeferred" ) )
    {	// FIXME: spelled wrong, but not changing for demo
		CG_LoadDeferredPlayers();
		return;
	}

	if ( !strcmp( cmd, "checkpoint" ) )
	{
		int time = atoi( CG_Argv(1) );
        CG_AddCheckpoint( time );
		return;
	}

	if ( !strcmp( cmd, "starttimer" ) )
    {
		CG_ResetCheckpoints();
		return;
	}

	if ( !strcmp( cmd, "stoptimer" ) )
    {
		int time = atoi( CG_Argv(1) );

        CG_StopAutorecord();

		if ( atoi( CG_Argv(2) ) )
        {
			cg.newBestCheckpoints = qtrue;
		}
		CG_AddCheckpoint( time );
		return;
	}

    if ( !strcmp( cmd ,"file" ) ) {
		char filename[MAX_QPATH];
		const char *data_in;
		int chunknum, len;
		fileHandle_t f;

		Com_sprintf( filename, sizeof(filename), "dl/%s", CG_Argv(1) );
		chunknum = atoi( CG_Argv(2) );
		data_in = CG_Argv(3);

		trap_FS_FOpenFile( filename, &f, chunknum == 0 ? FS_WRITE : FS_APPEND );

		#define char2hex(c) (c >= '0' && c <= '9' ? c - '0' : 10 + tolower(c) - 'a')
		while ( data_in[0] && data_in[1] ) {
			char byte = (char2hex(data_in[0]) << 4) + char2hex(data_in[1]);
			trap_FS_Write( &byte, 1, f );
			data_in += 2;
		}
		trap_FS_FCloseFile( f );
		return;
    }


	CG_Printf( "Unknown client game command: %s\n", cmd );
}


/*
====================
CG_ExecuteNewServerCommands

Execute all of the server commands that were received along
with this this snapshot.
====================
*/
void CG_ExecuteNewServerCommands( int latestSequence )
{
	while ( cgs.serverCommandSequence < latestSequence )
    {
		if ( trap_GetServerCommand( ++cgs.serverCommandSequence ) )
        {
			CG_ServerCommand();
		}
	}
}
