/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2000-2013 Darklegion Development
Copyright (C) 2015-2019 GrangerHub

This file is part of Tremulous.

Tremulous is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

Tremulous is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tremulous; if not, see <https://www.gnu.org/licenses/>

===========================================================================
*/

// cg_servercmds.c -- reliably sequenced text commands sent by the server
// these are processed at snapshot transition time, so there will definately
// be a valid snapshot this frame


#include "cg_local.h"

/*
=================
CG_ParseScores

=================
*/
static void CG_ParseScores( void )
{
  int   i;

  cg.numScores = ( trap_Argc( ) - 3 ) / 6;

  if( cg.numScores > MAX_CLIENTS )
    cg.numScores = MAX_CLIENTS;

  cg.teamScores[ 0 ] = atoi( CG_Argv( 1 ) );
  cg.teamScores[ 1 ] = atoi( CG_Argv( 2 ) );

  memset( cg.scores, 0, sizeof( cg.scores ) );

  if( cg_debugRandom.integer )
    CG_Printf( "cg.numScores: %d\n", cg.numScores );

  for( i = 0; i < cg.numScores; i++ )
  {
    //
    cg.scores[ i ].client = atoi( CG_Argv( i * 6 + 3 ) );
    cg.scores[ i ].score = atoi( CG_Argv( i * 6 + 4 ) );
    cg.scores[ i ].ping = atoi( CG_Argv( i * 6 + 5 ) );
    cg.scores[ i ].time = atoi( CG_Argv( i * 6 + 6 ) );
    cg.scores[ i ].weapon = atoi( CG_Argv( i * 6 + 7 ) );
    cg.scores[ i ].upgrade = atoi( CG_Argv( i * 6 + 8 ) );

    if( cg.scores[ i ].client < 0 || cg.scores[ i ].client >= MAX_CLIENTS )
      cg.scores[ i ].client = 0;

    cgs.clientinfo[ cg.scores[ i ].client ].score = cg.scores[ i ].score;

    cg.scores[ i ].team = cgs.clientinfo[ cg.scores[ i ].client ].team;
  }
}

/*
=================
CG_Parse_Team_Status

=================
*/
static void CG_Parse_Team_Status(void) {
  int   i;
  int   total_buildables;

  total_buildables = ( trap_Argc( ) - 4 ) / 2;

  Com_Assert(total_buildables <= BA_NUM_BUILDABLES);

  cg.team_status.num_builders = atoi(CG_Argv(1));
  cg.team_status.core_buildable_health = atoi(CG_Argv(2));
  cg.team_status.core_buildable_constructing = atoi(CG_Argv(3));

  memset(
    cg.team_status.num_buildables, 0, sizeof(cg.team_status.num_buildables));

  for(i = 0; i < total_buildables; i++) {
    //
    cg.team_status.num_buildables[atoi(CG_Argv(i * 2 + 4))] =
      atoi(CG_Argv(i * 2 + 5));
  }
}

/*
=================
CG_ParseReplacableBuildables

=================
*/
static void CG_ParseReplacableBuildables(void) {
  int i;

  for(i = 0; i < MAX_REPLACABLE_BUILDABLES; i++) {
    cg.replacable_buildables[i] = atoi( CG_Argv( i ) );
  }
}

/*
=================
CG_ParseTeamInfo

=================
*/
static void CG_ParseTeamInfo( void )
{
  int   i;
  int   count;
  int   client;

  count = trap_Argc( );

  for( i = 1; i < count; i++ ) // i is also incremented when writing into cgs.clientinfo
  {
    client = atoi( CG_Argv( i ) );

    // wrong team? drop the remaining info
    if( cgs.clientinfo[ client ].team != cg.snap->ps.stats[ STAT_TEAM ] )
      return;

    if( client < 0 || client >= MAX_CLIENTS )
    {
      CG_Printf( "[skipnotify]CG_ParseTeamInfo: bad client number: %d\n", client );
      return;
    }

    cgs.clientinfo[ client ].location       = atoi( CG_Argv( ++i ) );
    cgs.clientinfo[ client ].health         = BG_HP2SU( atoi( CG_Argv( ++i ) ) );
    cgs.clientinfo[ client ].credits        = atoi( CG_Argv( ++i ) );
    cgs.clientinfo[ client ].curWeaponClass = atoi( CG_Argv( ++i ) );
    if( cg.snap->ps.stats[ STAT_TEAM ] == TEAM_HUMANS )
      cgs.clientinfo[ client ].upgrade      = atoi( CG_Argv( ++i ) );
  }

  cgs.teaminfoReceievedTime = cg.time;
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
  const char  *info;
  char  *mapname;

  info = CG_ConfigString( CS_SERVERINFO );
  cgs.timelimit = atoi( Info_ValueForKey( info, "timelimit" ) );
  cgs.humanBlackout = atoi (Info_ValueForKey ( info, "g_humanBlackout") );
  cgs.maxclients = atoi( Info_ValueForKey( info, "sv_maxclients" ) );
  cgs.markDeconstruct = atoi( Info_ValueForKey( info, "g_markDeconstruct" ) );
  mapname = Info_ValueForKey( info, "mapname" );
  Com_sprintf( cgs.mapname, sizeof( cgs.mapname ), "maps/%s.bsp", mapname );
}

/*
================
CG_ParsePhysics

This is called explicitly when the gamestate is first received,
and whenever the server updates CS_PHYSICS
================
*/
void CG_ParsePhysics( void )
{
  const char *info;
  
  info = CG_ConfigString( CS_PHYSICS );

  if(info[0]) {
    sscanf(info, "%i %i", &cgs.playerAccelMode, &cgs.humanStaminaMode);
  }
}

/*
==================
CG_ParseWarmup
==================
*/
static void CG_ParseWarmup( void )
{
  const char  *info;

  info = CG_ConfigString( CS_WARMUP );

  cgs.warmup = atoi( info );
  trap_Cvar_Set( "ui_warmup", va( "%d", cgs.warmup ) );
}

/*
==================
CG_ParseWarmupReady
==================
*/
static void CG_ParseWarmupReady( void )
{
  const char  *info;

  info = CG_ConfigString( CS_WARMUP_READY );

  if( info[0] )
  {
    sscanf( info, "%f %d %d %f %d %d",
        &cgs.percentAliensReady,
        &cgs.numAliensReady,
        &cgs.numAliens,
        &cgs.percentHumansReady,
        &cgs.numHumansReady,
        &cgs.numHumans);
  }
}

/*
==================
CG_ParseScrimTeamName
==================
*/
static void CG_ParseScrimTeamName(scrim_team_t scrim_team, const char *conStr) {
  Q_strncpyz(
    cgs.scrim.team[scrim_team].name,
    conStr, sizeof(cgs.scrim.team[scrim_team].name));
}

/*
==================
CG_ParseScrimTeam
==================
*/
static void CG_ParseScrimTeam(scrim_team_t scrim_team, const char *conStr) {
  int has_captain;

  sscanf(
    conStr, "%i %i %i %i %i %i",
    (int *)&cgs.scrim.team[scrim_team].current_team,
    &has_captain,
    &cgs.scrim.team[scrim_team].captain_num,
    &cgs.scrim.team[scrim_team].wins,
    &cgs.scrim.team[scrim_team].losses,
    &cgs.scrim.team[scrim_team].draws);

  cgs.scrim.team[scrim_team].has_captain = has_captain ? qtrue : qfalse;
}

/*
==================
CG_ParseScrim
==================
*/
static void CG_ParseScrim(const char *conStr) {
  int allowScims;
  int scrim_completed;

  sscanf(
    conStr, "%i %i %i %i %i %i %i %i %i %i %i %i %i",
    &allowScims,
    (int *)&cgs.scrim.mode,
    (int *)&cgs.scrim.win_condition,
    &scrim_completed,
    (int *)&cgs.scrim.scrim_winner,
    (int *)&cgs.scrim.scrim_forfeiter,
    (int *)&cgs.scrim.timed_income,
    (int *)&cgs.scrim.sudden_death_mode,
    &cgs.scrim.sudden_death_time,
    &cgs.scrim.time_limit,
    (int *)&cgs.scrim.previous_round_win,
    &cgs.scrim.rounds_completed,
    &cgs.scrim.max_rounds);

    cgs.allowScrims = allowScims ? qtrue : qfalse;
    cgs.scrim.scrim_completed = scrim_completed ? qtrue : qfalse;

    trap_Cvar_Set( "ui_scrim", va( "%d", IS_SCRIM ) );
    UI_Shared_Set_Is_Scrim( IS_SCRIM );
}

/*
==================
CG_ParseCountdown
==================
*/
static void CG_ParseCountdown( void )
{
  const char  *info;
  int         countdown;
  int         index;

  info = CG_ConfigString( CS_COUNTDOWN );
  sscanf( info, "%i %i",
              &countdown,
              &index);

  cg.countdownTime = countdown;

  if( cgs.warmup && countdown )
  {
    sfxHandle_t sound = cgs.media.warmupEndSound[ index ];

    if( !sound )
      sound = cgs.media.warmupEndSound[0];
    trap_S_StartLocalSound( sound, CHAN_LOCAL_SOUND );
  }
}

/*
=====================
CG_ParseVoteStrings
=====================
*/
void CG_ParseVoteStrings( int team, const char *conStr )
{
  int subStringIndex, rawStringChar, subStringChar;

  Q_strncpyz( cgs.voteString[ RAW_VOTE_STRING ][ team ], conStr,
              sizeof( cgs.voteString[ RAW_VOTE_STRING ][ team ] ) );

  cgs.voteString[ 0 ][ team ][ 0 ]= 0;

  for( subStringIndex = 0, rawStringChar = 0, subStringChar = 0;
       ( cgs.voteString[ RAW_VOTE_STRING ][ team ][rawStringChar] && ( subStringIndex < RAW_VOTE_STRING ) );
       ++rawStringChar )
  {
    if( cgs.voteString[ RAW_VOTE_STRING ][ team ][rawStringChar] == STRING_DELIMITER )
    {
      cgs.voteString[ subStringIndex ][ team ][subStringChar] = 0;
      ++subStringIndex;
      subStringChar = 0;
    }
    else
    {
      cgs.voteString[ subStringIndex ][ team ][subStringChar] = cgs.voteString[ RAW_VOTE_STRING ][ team ][rawStringChar];
      ++subStringChar;
    }
  }

  cgs.voteString[ subStringIndex ][ team ][subStringChar] = 0;
  ++subStringIndex;

  while( subStringIndex < RAW_VOTE_STRING )
  {
    cgs.voteString[ subStringIndex ][ team ][ 0 ] = 0;
    ++subStringIndex;
  }

  return;
}

/*
================
CG_SetConfigValues

Called on load to set the initial values from configure strings
================
*/
void CG_SetConfigValues( void )
{
  const char   *alienStages = CG_ConfigString( CS_ALIEN_STAGES );
  const char   *humanStages = CG_ConfigString( CS_HUMAN_STAGES );
  const char   *playersReady = CG_ConfigString( CS_WARMUP_READY );
  scrim_team_t scrim_team;

  if( alienStages[0] )
  {
    sscanf( alienStages, "%d %d %d", &cgs.alienStage, &cgs.alienCredits,
        &cgs.alienNextStageThreshold );
  }
  else
    cgs.alienStage = cgs.alienCredits = cgs.alienNextStageThreshold = 0;


  if( humanStages[0] )
  {
    sscanf( humanStages, "%d %d %d", &cgs.humanStage, &cgs.humanCredits,
        &cgs.humanNextStageThreshold );
  }
  else
    cgs.humanStage = cgs.humanCredits = cgs.humanNextStageThreshold = 0;

  if( playersReady[0] )
  {
    sscanf( playersReady, "%f %d %d %f %d %d",
        &cgs.percentAliensReady,
        &cgs.numAliensReady,
        &cgs.numAliens,
        &cgs.percentHumansReady,
        &cgs.numHumansReady,
        &cgs.numHumans);
  }
  else
  {
    cgs.percentAliensReady = cgs.numAliensReady = cgs.numAliens =
      cgs.percentHumansReady = cgs.numHumansReady = cgs.numHumans = 0;
  }

  cgs.levelStartTime = atoi( CG_ConfigString( CS_LEVEL_START_TIME ) );
  cg.countdownTime = atoi( CG_ConfigString( CS_COUNTDOWN ) );

  cgs.warmup = atoi( CG_ConfigString( CS_WARMUP ) );
  trap_Cvar_Set( "ui_warmup", va( "%d", cgs.warmup ) );

  cgs.devMode = atoi( CG_ConfigString( CS_DEVMODE ) );
  trap_Cvar_Set( "ui_devMode", va( "%d", cgs.devMode ) );

  // scrim settings
  CG_ParseScrim(CG_ConfigString( CS_SCRIM ));
  for(scrim_team = 0; scrim_team < NUM_SCRIM_TEAMS; scrim_team++) {
    CG_ParseScrimTeamName(
      scrim_team, CG_ConfigString( CS_SCRIM_TEAM_NAME + scrim_team ));
    CG_ParseScrimTeam(
      scrim_team, CG_ConfigString( CS_SCRIM_TEAM + scrim_team ));
  }
}


/*
=====================
CG_ShaderStateChanged
=====================
*/
void CG_ShaderStateChanged( void )
{
  char        originalShader[ MAX_QPATH ];
  char        newShader[ MAX_QPATH ];
  char        timeOffset[ 16 ];
  const char  *o;
  char        *n, *t;

  o = CG_ConfigString( CS_SHADERSTATE );

  while( o && *o )
  {
    n = strstr( o, "=" );

    if( n && *n )
    {
      strncpy( originalShader, o, n - o );
      originalShader[ n - o ] = 0;
      n++;
      t = strstr( n, ":" );

      if( t && *t )
      {
        strncpy( newShader, n, t - n );
        newShader[ t - n ] = 0;
      }
      else
        break;

      t++;
      o = strstr( t, "@" );

      if( o )
      {
        strncpy( timeOffset, t, o - t );
        timeOffset[ o - t ] = 0;
        o++;
        trap_R_RemapShader( originalShader, newShader, timeOffset );
      }
    }
    else
      break;
  }
}

/*
================
CG_AnnounceAlienStageTransistion
================
*/
static void CG_AnnounceAlienStageTransistion( stage_t from, stage_t to )
{
  if( cg.predictedPlayerState.stats[ STAT_TEAM ] != TEAM_ALIENS )
    return;

  trap_S_StartLocalSound( cgs.media.alienStageTransition, CHAN_ANNOUNCER );
  CG_CenterPrint( CP_STAGE_UP, "We have evolved!", GIANTCHAR_WIDTH * 4, -1 );
}

/*
================
CG_AnnounceHumanStageTransistion
================
*/
static void CG_AnnounceHumanStageTransistion( stage_t from, stage_t to )
{
  if( cg.predictedPlayerState.stats[ STAT_TEAM ] != TEAM_HUMANS )
    return;

  trap_S_StartLocalSound( cgs.media.humanStageTransition, CHAN_ANNOUNCER );
  CG_CenterPrint( CP_STAGE_UP, "Reinforcements have arrived!",
                  GIANTCHAR_WIDTH * 4, -1 );
}

/*
================
CG_PlayIntermissionSound

================
*/
static void CG_PlayIntermissionSound( team_t team, intermissionSound_t soundType, int index )
{
  int i = ( index > MAX_INTERMISSION_SOUND_SETS ) ? MAX_INTERMISSION_SOUND_SETS : index;
  sfxHandle_t sound = cgs.media.intermissionCustomSounds[ team ][ soundType ][ i ];

  if( !sound )
    sound = cgs.media.intermissionCustomSounds[ team ][ soundType ][ 0 ];

  if( !sound )
    return;

  trap_S_StartLocalSound( sound, CHAN_LOCAL_SOUND );
}

/*
================
CG_ConfigStringModified

================
*/
static void CG_ConfigStringModified( void )
{
  const char  *str;
  int         num;

  num = atoi( CG_Argv( 1 ) );

  // get the gamestate from the client system, which will have the
  // new configstring already integrated
  trap_GetGameState( &cgs.gameState );

  // look up the individual string that was modified
  str = CG_ConfigString( num );

  // do something with it if necessary
  if( num == CS_MUSIC )
    CG_StartMusic( );
  else if( num == CS_SERVERINFO )
    CG_ParseServerinfo( );
  else if( num == CS_COUNTDOWN )
    CG_ParseCountdown( );
  else if( num == CS_WARMUP )
    CG_ParseWarmup( );
  else if( num == CS_WARMUP_READY )
    CG_ParseWarmupReady( );
  else if(num >= CS_SCRIM_TEAM_NAME && num < CS_SCRIM_TEAM_NAME + NUM_SCRIM_TEAMS)
    CG_ParseScrimTeamName(num - CS_SCRIM_TEAM_NAME, str);
  else if(num >= CS_SCRIM_TEAM && num < CS_SCRIM_TEAM + NUM_SCRIM_TEAMS)
    CG_ParseScrimTeam(num - CS_SCRIM_TEAM, str);
  else if( num == CS_SCRIM )
    CG_ParseScrim(str);
  else if( num == CS_DEVMODE )
  {
    cgs.devMode = atoi( CG_ConfigString( CS_DEVMODE ) );
    trap_Cvar_Set( "ui_devMode", va( "%d", cgs.devMode ) );
  }
  else if( num == CS_PHYSICS )
    CG_ParsePhysics( );
  else if( num >= CS_HUMAN_PORTAL_CREATETIME && num < CS_HUMAN_PORTAL_CREATETIME + PORTAL_NUM )
    cgs.humanPortalCreateTime[ num - CS_HUMAN_PORTAL_CREATETIME ] = atoi( str );
  else if( num == CS_ALIEN_STAGES )
  {
    stage_t oldAlienStage = cgs.alienStage;

    if( str[0] )
    {
      sscanf( str, "%d %d %d", &cgs.alienStage, &cgs.alienCredits,
          &cgs.alienNextStageThreshold );

      if( ( cgs.alienStage > oldAlienStage ) && !( cgs.warmup ) )
        CG_AnnounceAlienStageTransistion( oldAlienStage, cgs.alienStage );
    }
    else
    {
      cgs.alienStage = cgs.alienCredits = cgs.alienNextStageThreshold = 0;
    }
  }
  else if( num == CS_HUMAN_STAGES )
  {
    stage_t oldHumanStage = cgs.humanStage;

    if( str[0] )
    {
      sscanf( str, "%d %d %d", &cgs.humanStage, &cgs.humanCredits,
          &cgs.humanNextStageThreshold );

      if( ( cgs.humanStage > oldHumanStage ) && !( cgs.warmup ) )
        CG_AnnounceHumanStageTransistion( oldHumanStage, cgs.humanStage );
    }
    else
    {
      cgs.humanStage = cgs.humanCredits = cgs.humanNextStageThreshold = 0;
    }
  }
  else if( num == CS_LEVEL_START_TIME )
    cgs.levelStartTime = atoi( str );
  else if( num >= CS_VOTE_TIME && num < CS_VOTE_TIME + NUM_TEAMS )
  {
    cgs.voteTime[ num - CS_VOTE_TIME ] = atoi( str );
    cgs.voteAlarmPlay[ num - CS_VOTE_TIME ] = qtrue;

    if( num - CS_VOTE_TIME == TEAM_NONE )
      trap_Cvar_Set( "ui_voteActive", cgs.voteTime[ TEAM_NONE ] ? "1" : "0" );
    else if( num - CS_VOTE_TIME == TEAM_ALIENS )
      trap_Cvar_Set( "ui_alienTeamVoteActive",
        cgs.voteTime[ TEAM_ALIENS ] ? "1" : "0" );
    else if( num - CS_VOTE_TIME == TEAM_HUMANS )
      trap_Cvar_Set( "ui_humanTeamVoteActive",
        cgs.voteTime[ TEAM_HUMANS ] ? "1" : "0" );
  }
  else if( num >= CS_VOTE_CAST && num < CS_VOTE_CAST + NUM_TEAMS )
  {
    cgs.voteCast[ num - CS_VOTE_CAST ] = atoi( str );
    cgs.voteModified[ num - CS_VOTE_CAST ] = qtrue;
  }
  else if( num >= CS_VOTE_ACTIVE && num < CS_VOTE_ACTIVE + NUM_TEAMS )
  {
    cgs.voteActive[ num - CS_VOTE_ACTIVE ] = atoi( str );
  }
  else if( num >= CS_VOTE_STRING && num < CS_VOTE_STRING + NUM_TEAMS )
  {
    CG_ParseVoteStrings( ( num - CS_VOTE_STRING ), str );
  }
  else if( num >= CS_VOTE_CALLER && num < CS_VOTE_CALLER + NUM_TEAMS )
    Q_strncpyz( cgs.voteCaller[ num - CS_VOTE_CALLER ], str,
      sizeof( cgs.voteCaller[ num - CS_VOTE_CALLER ] ) );
  else if( num == CS_INTERMISSION )
    cg.intermissionStarted = atoi( str );
  else if( num >= CS_MODELS && num < CS_MODELS+MAX_MODELS )
    cgs.gameModels[ num - CS_MODELS ] = trap_R_RegisterModel( str );
  else if( num >= CS_SHADERS && num < CS_SHADERS+MAX_GAME_SHADERS )
    cgs.gameShaders[ num - CS_SHADERS ] = trap_R_RegisterShader( str );
  else if( num >= CS_PARTICLE_SYSTEMS && num < CS_PARTICLE_SYSTEMS+MAX_GAME_PARTICLE_SYSTEMS )
    cgs.gameParticleSystems[ num - CS_PARTICLE_SYSTEMS ] = CG_RegisterParticleSystem( (char *)str );
  else if( num >= CS_SOUNDS && num < CS_SOUNDS+MAX_SOUNDS )
  {
    if( str[ 0 ] != '*' )
    {  // player specific sounds don't register here
      cgs.gameSounds[ num - CS_SOUNDS ] = trap_S_RegisterSound( str, qfalse );
    }
  }
  else if( num >= CS_PLAYERS && num < CS_PLAYERS+MAX_CLIENTS )
  {
    CG_NewClientInfo( num - CS_PLAYERS );
    CG_BuildSpectatorString( );
  }
  else if( num == CS_WINNER )
  {
    char winner[ MAX_CVAR_VALUE_STRING ] = "";

    //parse the CS_WINNER string
    if( str[0] )
    {
      int             index, intOutcome, intWinningTeam;
      matchOutcomes_t outcome;
      team_t          winningTeam;
      team_t          team = cg.snap->ps.stats[ STAT_TEAM ];
      scrim_team_t    winningScrimTeam = SCRIM_TEAM_NONE;

      sscanf( str, "%i %i %i",
              &intOutcome,
              &index,
              &intWinningTeam );

      outcome = (matchOutcomes_t)intOutcome;
      winningTeam =(team_t)intWinningTeam;

      if(IS_SCRIM) {
        winningScrimTeam = BG_Scrim_Team_From_Playing_Team(&cgs.scrim, winningTeam);
      }

      switch ( outcome )
      {
        case MATCHOUTCOME_WON:
          if(IS_SCRIM) {
            if(cgs.scrim.scrim_forfeiter != SCRIM_TEAM_NONE) {
              strcpy(
                winner,
                va(
                  "^7%s^7 forfeited the scrim. Scrim victory goes to ^7%s^7.",
                  cgs.scrim.team[cgs.scrim.scrim_forfeiter].name,
                  cgs.scrim.team[cgs.scrim.scrim_winner].name));
            }else if(cgs.scrim.scrim_completed) {
              if(cgs.scrim.scrim_winner == SCRIM_TEAM_NONE) {
                strcpy(
                  winner,
                  va(
                    "This round is won by ^7%s^7. The scrim is a draw.",
                    cgs.scrim.team[winningScrimTeam].name));
              } else {
                strcpy(
                  winner,
                  va(
                    "This round is won by ^7%s^7. Scrim victory goes to ^7%s^7.",
                    cgs.scrim.team[winningScrimTeam].name,
                    cgs.scrim.team[cgs.scrim.scrim_winner].name));
              }
            } else {
              strcpy(winner, va("This round is won by ^7%s^7.", cgs.scrim.team[winningScrimTeam].name));
            }
          } else {
            strcpy( winner, va( "^7%s^7 win.", BG_Team( winningTeam )->humanName ) );
          }
          if( cg_intermissionMusic.integer )
          {
            CG_PlayIntermissionSound( winningTeam, INTMSN_SND_WIN, index );
          }
          break;

        case MATCHOUTCOME_EVAC:
          if(IS_SCRIM) {
            if(cgs.scrim.scrim_completed) {
              if(cgs.scrim.scrim_winner == SCRIM_TEAM_NONE) {
                strcpy(
                  winner,
                  va(
                    "The round was evacuated. The scrim is a draw."));
              } else {
                strcpy(
                  winner,
                  va(
                    "The round was evacuated. Scrim victory goes to ^7%s^7.",
                    cgs.scrim.team[cgs.scrim.scrim_winner].name));
              }
            } else {
              strcpy(winner, va("The round was evacuated."));
            }
          } else {
            strcpy( winner, "Evacuation" );
          }
          if( cg_intermissionMusic.integer )
            CG_PlayIntermissionSound( team, INTMSN_SND_EVAC, index );
          break;

        case MATCHOUTCOME_MUTUAL:
          if(IS_SCRIM) {
            if(cgs.scrim.scrim_completed) {
              if(cgs.scrim.scrim_winner == SCRIM_TEAM_NONE) {
                strcpy(
                  winner,
                  va(
                    "The round underwent mutual destruction. The scrim is a draw"));
              } else {
                strcpy(
                  winner,
                  va(
                    "The round underwent mutual destruction. Scrim victory goes to ^7%s^7.",
                    cgs.scrim.team[cgs.scrim.scrim_winner].name));
              }
            } else {
              strcpy(winner, va("The round underwent mutual destruction."));
            }
          } else {
            strcpy( winner, "Mutual destruction" );
          }
          if( cg_intermissionMusic.integer )
            CG_PlayIntermissionSound( team, INTMSN_SND_EVAC, index );
          break;

        case MATCHOUTCOME_TIME:
          if(IS_SCRIM) {
            if(cgs.scrim.scrim_completed) {
              if(cgs.scrim.scrim_winner == SCRIM_TEAM_NONE) {
                strcpy(
                  winner,
                  va(
                    "The round is a stalemate. The scrim is a draw."));
              } else {
                strcpy(
                  winner,
                  va(
                    "The round is a stalemate. Scrim victory goes to ^7%s^7.",
                    cgs.scrim.team[cgs.scrim.scrim_winner].name));
              }
            } else {
              strcpy( winner, "Stalemate" );
            }
          } else {
            strcpy( winner, "Stalemate" );
          }
          if( cg_intermissionMusic.integer )
            CG_PlayIntermissionSound( team, INTMSN_SND_TIME, index );
          break;
      }
    }

    trap_Cvar_Set( "ui_winner", winner );
  }
  else if( num == CS_SHADERSTATE )
  {
    CG_ShaderStateChanged( );
  }
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
  if( cg_showmiss.integer )
    CG_Printf( "CG_MapRestart\n" );

  CG_InitMarkPolys( );

  // make sure the "3 frags left" warnings play again
  cg.fraglimitWarnings = 0;

  cg.timelimitWarnings = 0;

  cg.intermissionStarted = qfalse;

  cgs.voteTime[ TEAM_NONE ] = 0;

  cg.mapRestart = qtrue;

  CG_StartMusic( );

  trap_S_ClearLoopingSounds( qtrue );

  // we really should clear more parts of cg here and stop sounds

  trap_Cvar_Set( "cg_thirdPerson", "0" );
}

/*
==============
CG_Menu
==============
*/
void CG_Menu( int menu, int arg )
{
  const char *cmd;              // command to send
  const char *longMsg  = NULL; // command parameter
  const char *shortMsg = NULL; // non-modal version of message
  const char *dialog;
  dialogType_t type = 0; // controls which cg_disable var will switch it off
  
  switch( cg.snap->ps.stats[ STAT_TEAM ] )
  {
    case TEAM_ALIENS:
      dialog = "menu tremulous_alien_dialog\n";
      break;
    case TEAM_HUMANS:
      dialog = "menu tremulous_human_dialog\n";
      break;
    default:
      dialog = "menu tremulous_default_dialog\n";
  }
  cmd = dialog;

  switch( menu )
  {
    case MN_TEAM:
      cmd       = "menu tremulous_teamselect\n";
      type      = DT_INTERACTIVE;
      break;

    case MN_A_CLASS:
      cmd       = "menu tremulous_alienclass\n";
      type      = DT_INTERACTIVE;
      break;

    case MN_H_SPAWN:
      cmd       = "menu tremulous_humanitem\n";
      type      = DT_INTERACTIVE;
      break;

    case MN_A_BUILD:
      cmd       = "menu tremulous_alienbuild\n";
      type      = DT_INTERACTIVE;
      break;

    case MN_H_BUILD:
      cmd       = "menu tremulous_humanbuild\n";
      type      = DT_INTERACTIVE;
      break;

    case MN_H_ARMOURY:
      cmd       = "menu tremulous_humanarmoury\n";
      type      = DT_INTERACTIVE;
      break;

    case MN_H_UNKNOWNITEM:
      shortMsg  = "Unknown item";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_A_TEAMFULL:
      longMsg   = "The alien team has too many players. Please wait until slots "
                  "become available or join the human team.";
      shortMsg  = "The alien team has too many players";
      type      = DT_COMMAND;
      break;

    case MN_H_TEAMFULL:
      longMsg   = "The human team has too many players. Please wait until slots "
                  "become available or join the alien team.";
      shortMsg  = "The human team has too many players";
      type      = DT_COMMAND;
      break;

    case MN_A_TEAMLOCKED:
      longMsg   = "The alien team is locked. You cannot join the aliens "
                  "at this time.";
      shortMsg  = "The alien team is locked";
      type      = DT_COMMAND;
      break;

    case MN_H_TEAMLOCKED:
      longMsg   = "The human team is locked. You cannot join the humans "
                  "at this time.";
      shortMsg  = "The human team is locked";
      type      = DT_COMMAND;
      break;

    case MN_PLAYERLIMIT:
      longMsg   = "The maximum number of playing clients has been reached. "
                  "Please wait until slots become available.";
      shortMsg  = "No free player slots";
      type      = DT_COMMAND;
      break;

    //===============================

    // Since cheating commands have no default binds, they will often be done
    // via console. In light of this, perhaps opening a menu is 
    // counterintuitive
    case MN_CMD_CHEAT:
      //longMsg   = "This action is considered cheating. It can only be used "
      //            "in cheat mode, which is not enabled on this server.";
      shortMsg  = "Cheats are not enabled on this server";
      type      = DT_COMMAND;
      break;

    case MN_CMD_CHEAT_TEAM:
      shortMsg  = "Cheats are not enabled on this server, so "
                  "you may not use this command while on a team";
      type      = DT_COMMAND;
      break;

    case MN_CMD_TEAM:
      //longMsg   = "You must be on a team to perform this action. Join the alien"
      //            "or human team and try again.";
      shortMsg  = "Join a team first";
      type      = DT_COMMAND;
      break;

    case MN_CMD_SPEC:
      //longMsg   = "You may not perform this action while on a team. Become a "
      //            "spectator before trying again.";
      shortMsg  = "You can only use this command when spectating";
      type      = DT_COMMAND;
      break;

    case MN_CMD_ALIEN:
      //longMsg   = "You must be on the alien team to perform this action.";
      shortMsg  = "Must be alien to use this command";
      type      = DT_COMMAND;
      break;

    case MN_CMD_HUMAN:
      //longMsg   = "You must be on the human team to perform this action.";
      shortMsg  = "Must be human to use this command";
      type      = DT_COMMAND;
      break;

    case MN_CMD_ALIVE:
      //longMsg   = "You must be alive to perform this action.";
      shortMsg  = "Must be alive to use this command";
      type      = DT_COMMAND;
      break;


    //===============================
    
    case MN_B_NOROOM:
      longMsg   = "There is no room to build here. Move until the structure turns "
                  "translucent green, indicating a valid build location.";
      shortMsg  = "There is no room to build here";
      type      = DT_BUILD;
      break;

    case MN_B_NORMAL:
      longMsg   = "Cannot build on this surface. The surface is too steep or "
                  "unsuitable for building. Please choose another site for this "
                  "structure.";
      shortMsg  = "Cannot build on this surface";
      type      = DT_BUILD;
      break;

    case MN_B_CANNOT:
      longMsg   = NULL;
      shortMsg  = "You cannot build that structure";
      type      = DT_BUILD;
      break;

    // FIXME: MN_H_ and MN_A_?
    case MN_B_LASTSPAWN:
      longMsg   = "This action would remove your team's last spawn point, "
                  "which often quickly results in a loss. Try building more "
                  "spawns.";
      shortMsg  = "You may not deconstruct the last spawn";
      break;

    case MN_B_SUDDENDEATH:
      longMsg   = "Neither team has prevailed after a certain time and the "
                  "game has entered Sudden Death. During Sudden Death "
                  "building is not allowed.";
      shortMsg  = "Cannot build during Sudden Death";
      type      = DT_BUILD;
      break;

    case MN_B_REVOKED:
      longMsg   = "Your teammates have lost faith in your ability to build "
                  "for the team. You will not be allowed to build until your "
                  "team votes to reinstate your building rights.";
      shortMsg  = "Your building rights have been revoked";
      type      = DT_BUILD;
      break;

    case MN_B_BLOCKEDBYENEMY:
      switch( cg.snap->ps.stats[ STAT_TEAM ] )
      {
        case TEAM_ALIENS:
          longMsg   = "You cannot build within the range of a human reactor "
                      "during pre-game warmup.";
          shortMsg  = "A nearby human reactor disrupting creep here.";
          break;
        case TEAM_HUMANS:
          longMsg   = "You cannot build on alien creep, within the range of "
                      "an overmind during pre-game warmup.";
          shortMsg  = "Alien overmind creep is disrupting power here.";
          break;
      }
      break;

    case MN_B_SD_UNIQUE:
      longMsg   = "You can only rebuild one of each type of rebuildable "
                  "structure during Sudden Death.";
      shortMsg  = "You can only rebuild one of each type of rebuildable "
                  "structure during Sudden Death.";
      break;

    case MN_B_SD_IRREPLACEABLE:
      longMsg   = "Structures not built at the start of sudden death, spawns, "
                  "and defensive buildables can't be rebuilt during sudden death.";
      shortMsg  = "This building type cannot be rebuilt during Sudden Death";
      break;

    case MN_B_SD_NODECON:
      longMsg   = "This structure can't be deconstructed nor marked during "
                  "sudden death";
      shortMsg  = "This structure can't be deconstructed nor marked during "
                  "sudden death";
      break;

    case MN_B_SURRENDER:
      longMsg   = "Your team has decided to admit defeat and concede the game:"
                  "traitors and cowards are not allowed to build.";
                  // too harsh?
      shortMsg  = "Building is denied to traitorous cowards";
      break;

    //===============================

    case MN_H_NOBP:
      if( cgs.markDeconstruct )
        longMsg   = "There is no power remaining. Free up power by marking "
                    "existing buildable objects.";
      else
        longMsg   = "There is no power remaining. Free up power by deconstructing "
                    "existing buildable objects.";
      shortMsg  = "There is no power remaining";
      type      = DT_BUILD;
      break;

    case MN_H_NOTPOWERED:
      longMsg   = "This buildable is not powered. Build a Reactor and/or Repeater "
                  "in order to power it.";
      shortMsg  = "This buildable is not powered";
      type      = DT_BUILD;
      break;

    case MN_H_ONEREACTOR:
      longMsg   = "There can only be one Reactor. Deconstruct the existing one if you "
                  "wish to move it.";
      shortMsg  = "There can only be one Reactor";
      type      = DT_BUILD;
      break;

    case MN_H_NOPOWERHERE:
      longMsg   = "There is no power here. If available, a Repeater may be used to "
                  "transmit power to this location.";
      shortMsg  = "There is no power here";
      type      = DT_BUILD;
      break;

    case MN_H_NODCC:
      longMsg   = "There is no Defense Computer. A Defense Computer is needed to "
                  "build this.";
      shortMsg  = "There is no Defense Computer";
      type      = DT_BUILD;
      break;

    case MN_H_RPTPOWERHERE:
      longMsg   = "This area already has power. A Repeater is not required here.";
      shortMsg  = "This area already has power";
      type      = DT_BUILD;
      break;

    //===============================

    case MN_ACT_FAILED:
      longMsg   = "This object failed to activate";
      shortMsg  = "This object failed to activate";
      break;

    case MN_ACT_OCCUPIED:
      longMsg   = "This object is fully occupied at the "
                  "moment, and has no more room.";
      shortMsg  = "This object is fully occupied";
      type      = DT_COMMAND;
      break;

    case MN_ACT_OCCUPYING:
      longMsg   = "The targeted entity is already occupying "
                  "something else and thus can't occupy this "
                  "object you're trying to activate.";
      shortMsg  = "The targeted entity is preoccupied";
      type      = DT_COMMAND;
      break;
      
    case MN_ACT_NOOCCUPANTS:
      longMsg   = "There are no targets available to occupy this object.";
      shortMsg  = "There are no targets for occupying this object.";
      type      = DT_COMMAND;
      break;

    case MN_ACT_NOEXIT:
      longMsg   = "You can't exit this structure at this time.  Try again "
                  "later.";
      shortMsg  = "You can't exit this structure at this time.";
      type      = DT_COMMAND;
      break;

    case MN_ACT_NOTPOWERED:
      longMsg   = "This object requires power to be activated. Find a way "
                  "to power it.";
      shortMsg  = "This object is not powered";
      break;

    case MN_ACT_NOTCONTROLLED:
      longMsg   = "There is no Overmind. An Overmind must be built to control "
                  "this structure you are trying to activate.";
      shortMsg  = "There is no Overmind";
      type      = DT_BUILD;
      break;

    //===============================

    case MN_H_NOSLOTS:
      longMsg   = "You have no room to carry this. Please sell any conflicting "
                  "upgrades before purchasing this item.";
      shortMsg  = "You have no room to carry this";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_H_NOFUNDS:
      longMsg   = "Insufficient funds. You do not have enough credits to perform "
                  "this action.";
      shortMsg  = "Insufficient funds";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_H_ITEMHELD:
      longMsg   = "You already hold this item. It is not possible to carry multiple "
                  "items of the same type.";
      shortMsg  = "You already hold this item";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_H_UNEXPLODEDGRENADE:
      longMsg   = "You already have an unexploded grenade. It is not possible to buy "
                  "another until your grenade has detonated.";
      shortMsg  = "You already have an undetonated grenade.";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_H_NOARMOURYHERE:
      longMsg   = "You must be near a powered Armoury in order to purchase "
                  "weapons, upgrades or ammunition.";
      shortMsg  = "You must be near a powered Armoury";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_H_NOENERGYAMMOHERE:
      longMsg   = "You must be near a Reactor or a powered Armoury or Repeater "
                  "in order to purchase energy ammunition.";
      shortMsg  = "You must be near a Reactor or a powered Armoury or Repeater";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_H_NOROOMBSUITON:
      longMsg   = "There is not enough room here to put on a Battle Suit. "
                  "Make sure you have enough head room to climb in.";
      shortMsg  = "Not enough room here to put on a Battle Suit";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_H_NOROOMBSUITOFF:
      longMsg   = "There is not enough room here to take off your Battle Suit. "
                  "Make sure you have enough head room to climb out.";
      shortMsg  = "Not enough room here to take off your Battle Suit";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_H_ARMOURYBUILDTIMER:
      longMsg   = "You are not allowed to buy or sell weapons until your "
                  "build timer has expired.";
      shortMsg  = "You can not buy or sell weapons until your build timer "
                  "expires";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_H_DEADTOCLASS:
      shortMsg  = "You must be dead to use the class command";
      type      = DT_COMMAND;
      break;

    case MN_H_UNKNOWNSPAWNITEM:
      shortMsg  = "Unknown starting item";
      type      = DT_COMMAND;
      break;

    //===============================

    case MN_A_NOCREEP:
      longMsg   = "There is no creep here. You must build near existing Eggs or "
                  "the Overmind. Alien structures will not support themselves.";
      shortMsg  = "There is no creep here";
      type      = DT_BUILD;
      break;

    case MN_A_NOOVMND:
      longMsg   = "There is no Overmind. An Overmind must be built to control "
                  "the structure you tried to place.";
      shortMsg  = "There is no Overmind";
      type      = DT_BUILD;
      break;

    case MN_A_ONEOVERMIND:
      longMsg   = "There can only be one Overmind. Deconstruct the existing one if you "
                  "wish to move it.";
      shortMsg  = "There can only be one Overmind";
      type      = DT_BUILD;
      break;

    case MN_A_NOBP:
      longMsg   = "The Overmind cannot control any more structures. Deconstruct existing "
                  "structures to build more.";
      shortMsg  = "The Overmind cannot control any more structures";
      type      = DT_BUILD;
      break;

    case MN_A_NOEROOM:
      longMsg   = "There is no room to evolve here. Move away from walls or other "
                  "nearby objects and try again.";
      shortMsg  = "There is no room to evolve here";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_A_TOOCLOSE:
      longMsg   = "This location is too close to the enemy to evolve. Move away "
                  "from the enemy's presence and try again.";
      shortMsg  = "This location is too close to the enemy to evolve";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_A_NOOVMND_EVOLVE:
      longMsg   = "There is no Overmind. An Overmind must be built to allow "
                  "you to upgrade.";
      shortMsg  = "There is no Overmind";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_A_EVOLVEBUILDTIMER:
      longMsg   = "You cannot evolve until your build timer has expired.";
      shortMsg  = "You cannot evolve until your build timer expires";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_A_INFEST:
      trap_Cvar_Set( "ui_currentClass",
         va( "%d %d %d", cg.snap->ps.stats[ STAT_CLASS ],
                      cg.snap->ps.persistant[ PERS_CREDIT ],
                      (cg.snap->ps.stats[STAT_FLAGS] & SFL_CLASS_FORCED)) );

      cmd       = "menu tremulous_alienupgrade\n";
      type      = DT_INTERACTIVE;
      break;

    case MN_A_CANTEVOLVE:
      shortMsg  = va( "You cannot evolve into a %s", 
                      BG_ClassConfig( arg )->humanName );
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_A_EVOLVEWALLWALK:
      shortMsg  = "You cannot evolve to that class while wallwalking";
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_A_UNKNOWNCLASS:
      shortMsg  = "Unknown class";
      type      = DT_ARMOURYEVOLVE;
      break;
      
    case MN_A_CLASSNOTSPAWN:
      shortMsg  = va( "You cannot spawn as a %s", 
                      BG_ClassConfig( arg )->humanName );
      type      = DT_ARMOURYEVOLVE;
      break;
    
    case MN_A_CLASSNOTALLOWED:
      shortMsg  = va( "The %s is not allowed",
                      BG_ClassConfig( arg )->humanName );
      type      = DT_ARMOURYEVOLVE;
      break;

    case MN_A_CLASSNOTATSTAGE:
      shortMsg  = va( "The %s is not allowed at Stage %d",
                      BG_ClassConfig( arg )->humanName,
                      cgs.alienStage + 1 );
      type      = DT_ARMOURYEVOLVE;
      break;

    default:
      Com_Printf( "cgame: debug: no such menu %d\n", menu );
  }
  
  if( type == DT_ARMOURYEVOLVE && cg_disableUpgradeDialogs.integer )
    return;

  if( type == DT_BUILD && cg_disableBuildDialogs.integer )
    return;

  if( type == DT_COMMAND && cg_disableCommandDialogs.integer )
    return;

  if( cmd != dialog )
  {
    trap_SendConsoleCommand( cmd );
  }
  else if( longMsg && cg_disableWarningDialogs.integer == 0 )
  {
    trap_Cvar_Set( "ui_dialog", longMsg );
    trap_SendConsoleCommand( cmd );
  }
  else if( shortMsg && cg_disableWarningDialogs.integer < 2 )
  {
    CG_Printf( "%s\n", shortMsg );
  }
}

/*
=================
CG_Say
=================
*/
static void CG_Say( int clientNum, saymode_t mode, const char *text )
{
  char *name;
  char prefix[ 13 ] = "";
  char *ignore = "";
  char *location = "";
  char *color;
  char *maybeColon;
  char *tmsgcolor = S_COLOR_YELLOW;

  if( clientNum >= 0 && clientNum < MAX_CLIENTS )
  {
    clientInfo_t *ci = &cgs.clientinfo[ clientNum ];
    char         *tcolor = S_COLOR_WHITE;
    char         *tbcolor = S_COLOR_YELLOW;

    name = ci->name;

    if( !( ci->team == TEAM_NONE ) )
      tmsgcolor = S_COLOR_CYAN;

    if( ci->team == TEAM_ALIENS )
    {
      tcolor = S_COLOR_MAGENTA;
      tbcolor = S_COLOR_RED;
    }
    else if( ci->team == TEAM_HUMANS )
    {
      tcolor = S_COLOR_CYAN;
      tbcolor = S_COLOR_BLUE;
    }

    if( cg_chatTeamPrefix.integer )
      Com_sprintf( prefix, sizeof( prefix ), "%s[%s%c%s]" S_COLOR_WHITE " ",
                   tbcolor, tcolor,
                   toupper( *( BG_Team( ci->team )->name2 ) ),
                   tbcolor );

    if( ( mode == SAY_TEAM || mode == SAY_AREA ) &&
        cg.snap->ps.pm_type != PM_INTERMISSION )
    {
      int locationNum;

      if( clientNum == cg.snap->ps.clientNum )
      {
        centity_t     *locent;

        locent = CG_GetPlayerLocation( );
        if( locent )
          locationNum = locent->currentState.generic1;
        else
          locationNum = -1;
      }
      else
        locationNum = ci->location;

      if( locationNum >= 0 && locationNum < MAX_LOCATIONS )
      {
        const char *s = CG_ConfigString( CS_LOCATIONS + locationNum );

        if( *s )
          location = va( " (%s" S_COLOR_WHITE ")", s );
      }
    }
  }
  else
    name = "console";

  // IRC-like /me parsing
  if( mode != SAY_RAW && Q_stricmpn( text, "/me ", 4 ) == 0 )
  {
    text += 4;
    Q_strcat( prefix, sizeof( prefix ), "* " );
    maybeColon = "";
  }
  else
    maybeColon = ":";

  switch( mode )
  {
    case SAY_ALL:
      // might already be ignored but in that case no harm is done
      if( cg_teamChatsOnly.integer )
        ignore = "[skipnotify]";

#ifdef MODULE_INTERFACE_11
      CG_Printf( "%s%s%s" S_COLOR_WHITE "%s " S_COLOR_GREEN "%s\n",
                 ignore, prefix, name, maybeColon, text );
#else
      CG_Printf( "%s%s%s" S_COLOR_WHITE "%s %c" S_COLOR_GREEN "%s\n",
                 ignore, prefix, name, maybeColon, INDENT_MARKER, text );
#endif
      break;
    case SAY_TEAM:
#ifdef MODULE_INTERFACE_11
      CG_Printf( "%s%s(%s" S_COLOR_WHITE ")%s%s %s%s\n",
                 ignore, prefix, name, location, maybeColon, tmsgcolor, text );
#else
      CG_Printf( "%s%s(%s" S_COLOR_WHITE ")%s%s %c%s%s\n",
                 ignore, prefix, name, location, maybeColon, INDENT_MARKER, tmsgcolor, text );
#endif
      break;
    case SAY_ADMINS:
    case SAY_ADMINS_PUBLIC:
#ifdef MODULE_INTERFACE_11
      CG_Printf( "%s%s%s%s" S_COLOR_WHITE "%s " S_COLOR_MAGENTA "%s\n",
                 ignore, prefix,
                 ( mode == SAY_ADMINS ) ? "[ADMIN]" : "[PLAYER]",
                 name, maybeColon, text );
#else
      CG_Printf( "%s%s%s%s" S_COLOR_WHITE "%s %c" S_COLOR_MAGENTA "%s\n",
                 ignore, prefix,
                 ( mode == SAY_ADMINS ) ? "[ADMIN]" : "[PLAYER]",
                 name, maybeColon, INDENT_MARKER, text );
#endif
      break;
    case SAY_AREA:
#ifdef MODULE_INTERFACE_11
      CG_Printf( "%s%s<%s" S_COLOR_WHITE ">%s%s " S_COLOR_BLUE "%s\n",
                 ignore, prefix, name, location, maybeColon, text );
#else
      CG_Printf( "%s%s<%s" S_COLOR_WHITE ">%s%s %c" S_COLOR_BLUE "%s\n",
                 ignore, prefix, name, location, maybeColon, INDENT_MARKER, text );
#endif
      break;
    case SAY_PRIVMSG:
    case SAY_TPRIVMSG:
      color = ( mode == SAY_TPRIVMSG ) ? S_COLOR_CYAN : S_COLOR_GREEN;
#ifdef MODULE_INTERFACE_11
      CG_Printf( "%s%s[%s" S_COLOR_WHITE " -> %s" S_COLOR_WHITE "]%s %s%s\n",
                 ignore, prefix, name, cgs.clientinfo[ cg.clientNum ].name,
                 maybeColon, color, text );
#else
      CG_Printf( "%s%s[%s" S_COLOR_WHITE " -> %s" S_COLOR_WHITE "]%s %c%s%s\n",
                 ignore, prefix, name, cgs.clientinfo[ cg.clientNum ].name,
                 maybeColon, INDENT_MARKER, color, text );
#endif
      if( !ignore[0] )
      {
        CG_CenterPrint( CP_PRIVATE_MESSAGE,
                        va( "%sPrivate message from: " S_COLOR_WHITE "%s", 
                            color, name ), GIANTCHAR_WIDTH * 4, -1 );
        if( clientNum < 0 || clientNum >= MAX_CLIENTS )
          clientNum = cg.clientNum;
        CG_Printf( ">> to reply, say: /m %d [your message] <<\n", clientNum );
      }
      break;
    case SAY_RAW:
      CG_Printf( "%s\n", text );
      break;
  }

  switch( mode )
  {
    case SAY_TEAM:
    case SAY_AREA:
    case SAY_TPRIVMSG:
      if( cg.snap->ps.stats[ STAT_TEAM ] == TEAM_ALIENS )
      {
        trap_S_StartLocalSound( cgs.media.alienTalkSound, CHAN_LOCAL_SOUND );
        break;
      }
      else if( cg.snap->ps.stats[ STAT_TEAM ] == TEAM_HUMANS )
      {
        trap_S_StartLocalSound( cgs.media.humanTalkSound, CHAN_LOCAL_SOUND );
        break;
      }
    default: 
      trap_S_StartLocalSound( cgs.media.talkSound, CHAN_LOCAL_SOUND );
  }
}

/*
=================
CG_VoiceTrack

return the voice indexed voice track or print errors quietly to console
in case someone is on an unpure server and wants to know which voice pak
is missing or incomplete
=================
*/
static voiceTrack_t *CG_VoiceTrack( char *voice, int cmd, int track )
{
  voice_t *v;
  voiceCmd_t *c;
  voiceTrack_t *t;

  v = BG_VoiceByName( cgs.voices, voice );
  if( !v )
  {
    CG_Printf( "[skipnotify]WARNING: could not find voice \"%s\"\n", voice );
    return NULL;
  }
  c = BG_VoiceCmdByNum( v->cmds, cmd );
  if( !c )
  {
    CG_Printf( "[skipnotify]WARNING: could not find command %d "
      "in voice \"%s\"\n", cmd, voice );
    return NULL;
  }
  t = BG_VoiceTrackByNum( c->tracks, track );
  if( !t )
  {
    CG_Printf( "[skipnotify]WARNING: could not find track %d for command %d in "
      "voice \"%s\"\n", track, cmd, voice );
    return NULL;
  }
  return t;
}

/*
=================
CG_ParseVoice

voice clientNum vChan cmdNum trackNum [sayText]
=================
*/
static void CG_ParseVoice( void )
{
  int clientNum;
  voiceChannel_t vChan;
  char sayText[ MAX_SAY_TEXT] = {""};
  voiceTrack_t *track;
  clientInfo_t *ci;

  if( trap_Argc() < 5 || trap_Argc() > 6 )
    return;

  if( trap_Argc() == 6 )
    Q_strncpyz( sayText, CG_Argv( 5 ), sizeof( sayText ) );

  clientNum = atoi( CG_Argv( 1 ) );
  if( clientNum < 0 || clientNum >= MAX_CLIENTS )
    return;

  vChan = atoi( CG_Argv( 2 ) );
  if( (int)vChan < 0 || vChan >= VOICE_CHAN_NUM_CHANS )
    return;

  if( cg_teamChatsOnly.integer && vChan != VOICE_CHAN_TEAM )
    return;

  ci = &cgs.clientinfo[ clientNum ];

  // this joker is still talking
  if( ci->voiceTime > cg.time )
    return;

  track = CG_VoiceTrack( ci->voice, atoi( CG_Argv( 3 ) ), atoi( CG_Argv( 4 ) ) );

  // keep track of how long the player will be speaking
  // assume it takes 3s to say "*unintelligible gibberish*" 
  if( track )
    ci->voiceTime = cg.time + track->duration;
  else
    ci->voiceTime = cg.time + 3000;

  if( !sayText[ 0 ] )
  {
    if( track )
      Q_strncpyz( sayText, track->text, sizeof( sayText ) );
    else
      Q_strncpyz( sayText, "*unintelligible gibberish*", sizeof( sayText ) );
  }
 
  if( !cg_noVoiceText.integer ) 
  {
    switch( vChan )
    {
      case VOICE_CHAN_ALL:
        CG_Say( clientNum, SAY_ALL, sayText );
        break;
      case VOICE_CHAN_TEAM:
        CG_Say( clientNum, SAY_TEAM, sayText );
        break;
      default:
        break;
    }
  }

  // playing voice audio tracks disabled
  if( cg_noVoiceChats.integer )
    return;

  // no audio track to play
  if( !track )
    return;

  switch( vChan )
  {
    case VOICE_CHAN_ALL:
      trap_S_StartLocalSound( track->track, CHAN_VOICE );
      break;
    case VOICE_CHAN_TEAM:
      trap_S_StartLocalSound( track->track, CHAN_VOICE );
      break;
    case VOICE_CHAN_LOCAL:
      trap_S_StartSound( NULL, clientNum, CHAN_VOICE, track->track );
      break;
    default:
        break;
  } 
}

/*
=================
CG_CenterPrint_f
=================
*/
static void CG_CenterPrint_f( void ) {
  if(trap_Argc( ) >= 5) {
    char string[MAX_STRING_CHARS];

    Q_strncpyz(string, CG_Argv(1), sizeof(string));
    CG_CenterPrint(atoi(CG_Argv(2)), string, atoi(CG_Argv(3)), atoi(CG_Argv(3)));
  } else if(trap_Argc( ) >= 4) {
    char string[MAX_STRING_CHARS];

    Q_strncpyz(string, CG_Argv(1), sizeof(string));
    CG_CenterPrint(atoi(CG_Argv(2)), string, BIGCHAR_WIDTH, atoi(CG_Argv(3)));
  } else if(trap_Argc( ) >= 3) {
    char string[MAX_STRING_CHARS];

    Q_strncpyz(string, CG_Argv(1), sizeof(string));
    CG_CenterPrint(atoi(CG_Argv(2)), string, BIGCHAR_WIDTH, -1);
  } else {
    CG_CenterPrint( 0, CG_Argv( 1 ), BIGCHAR_WIDTH, -1 );
  }
}

/*
=================
CG_Print_f
=================
*/
static void CG_Print_f( void )
{
  CG_Printf( "%s", CG_Argv( 1 ) );
}

/*
=================
CG_Chat_f
=================
*/
static void CG_Chat_f( void )
{
  char     id[ 3 ];
  char     mode[ 3 ];

  trap_Argv( 1, id, sizeof( id ) );
  trap_Argv( 2, mode, sizeof( mode ) );

  CG_Say( atoi( id ), atoi( mode ), CG_Argv( 3 ) );
}

/*
=================
CG_ServerMenu_f
=================
*/
static void CG_ServerMenu_f( void )
{
  if( !cg.demoPlayback )
  {
    if( trap_Argc( ) == 2 )
      CG_Menu( atoi( CG_Argv( 1 ) ), 0 );
    else if( trap_Argc( ) == 3 )
      CG_Menu( atoi( CG_Argv( 1 ) ), atoi( CG_Argv( 2 ) ) );
  }
}

/*
=================
CG_ServerCloseMenus_f
=================
*/
static void CG_ServerCloseMenus_f( void )
{
  trap_SendConsoleCommand( "closemenus\n" );
}

/*
=================
CG_PlayMap_Pool_Json_f

Receive periodic updates from server to cache playmap pool locally.
=================
*/
static void CG_PlayMap_Pool_Json_f( void )
{
  if( cg_debugPlayMap.integer > 0 )
    trap_Print( va( "PLAYMAP: received broadcasted playmap pool contents: %s.\n",
		    CG_Argv( 1 ) ) );
  Q_strncpyz( cgs.playMapPoolJson, CG_Argv( 1 ),
              MAX_PLAYMAP_POOL_CHARS );
}

/*
=================
CG_PoisonCloud_f
=================
*/
static void CG_PoisonCloud_f( void )
{
  cg.poisonedTime = cg.time;

  if( CG_IsParticleSystemValid( &cg.poisonCloudPS ) )
  {
    cg.poisonCloudPS = CG_SpawnNewParticleSystem( cgs.media.poisonCloudPS );
    CG_SetAttachmentCent( &cg.poisonCloudPS->attachment, &cg.predictedPlayerEntity );
    CG_AttachToCent( &cg.poisonCloudPS->attachment );
  }
}

static char   registeredCmds[ 16384 ]; // cmd1\0cmd2\0cmdn\0\0
static size_t gcmdsOffset;
static void CG_GameCmds_f( void )
{
  int        i;
  int        c = trap_Argc( );
  const char *cmd;
  size_t     len;

  for( i = 1; i < c; i++ )
  {
    cmd = CG_Argv( i );
    len = strlen( cmd ) + 1;
    if( len + gcmdsOffset >= sizeof( registeredCmds ) - 1 )
    {
      CG_Printf( "AddCommand: too many commands (%d >= %d)\n",
        (int)( len + gcmdsOffset ), (int)( sizeof( registeredCmds ) - 1 ) );
      return;
    }
    trap_AddCommand( cmd );
    strcpy( registeredCmds + gcmdsOffset, cmd );
    gcmdsOffset += len;
  }
}

void CG_UnregisterCommands( void )
{
  size_t len, offset = 0;
  while( registeredCmds[ offset ] )
  {
    len = strlen( registeredCmds + offset );
    trap_RemoveCommand( registeredCmds + offset );
    offset += len + 1;
  }
  memset( registeredCmds, 0, 2 );
  gcmdsOffset = 0;
}

static consoleCommand_t svcommands[ ] =
{
  { "chat", CG_Chat_f },
  { "cmds", CG_GameCmds_f },
  { "cp", CG_CenterPrint_f },
  { "cs", CG_ConfigStringModified },
  { "map_restart", CG_MapRestart },
  { "playpool_json", CG_PlayMap_Pool_Json_f },
  { "poisoncloud", CG_PoisonCloud_f },
  { "print", CG_Print_f },
  { "replacable_buildables", CG_ParseReplacableBuildables },
  { "scores", CG_ParseScores },
  { "serverclosemenus", CG_ServerCloseMenus_f },
  { "servermenu", CG_ServerMenu_f },
  { "teamstatus", CG_Parse_Team_Status },
  { "tinfo", CG_ParseTeamInfo },
  { "voice", CG_ParseVoice }
};

/*
=================
CG_ServerCommand

The string has been tokenized and can be retrieved with
Cmd_Argc() / Cmd_Argv()
=================
*/
static void CG_ServerCommand( void )
{
  const char       *cmd;
  consoleCommand_t *command;

  cmd = CG_Argv( 0 );
  command = bsearch( cmd, svcommands, ARRAY_LEN( svcommands ),
                     sizeof( svcommands[ 0 ] ), cmdcmp );

  if( command )
  {
    command->function( );
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
  while( cgs.serverCommandSequence < latestSequence )
  {
    if( trap_GetServerCommand( ++cgs.serverCommandSequence ) )
      CG_ServerCommand( );
  }
}
