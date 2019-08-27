// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "g_local.h"
#include "bg_shared.h"

level_locals_t	level;
static void G_ResetCvars (void);

typedef struct {
	vmCvar_t	*vmCvar;
	const char	*cvarName;
	const char	*defaultString;
	int			cvarFlags;
	int			modificationCount;	// for tracking changes
	qbool		trackChange;		// track this variable, and announce if changed
	qbool		teamShader;			// track and if changed, update shader state
} cvarTable_t;

gentity_t	g_entities[MAX_GENTITIES];
gclient_t	g_clients[MAX_CLIENTS];

vmCvar_t	g_gametype;
vmCvar_t	g_dmflags;
vmCvar_t	g_fraglimit;
vmCvar_t	g_timelimit;
vmCvar_t	g_password;
vmCvar_t	g_needpass;
vmCvar_t	g_mapname;
vmCvar_t	sv_fps;
vmCvar_t	sv_pure;
vmCvar_t	g_maxclients;
vmCvar_t	g_maxGameClients;
vmCvar_t	g_dedicated;
vmCvar_t	g_speed;
vmCvar_t	g_gravity;
vmCvar_t	g_cheats;
vmCvar_t	g_knockback;
vmCvar_t	g_forcerespawn;
vmCvar_t	g_inactivity;
vmCvar_t	g_debugMove;
vmCvar_t	g_debugDamage;
vmCvar_t	g_debugAlloc;
vmCvar_t	g_weaponRespawn;
vmCvar_t	g_motd;
vmCvar_t	g_synchronousClients;
vmCvar_t	g_warmup;
vmCvar_t	g_predictPVS;
vmCvar_t	g_restarted;
vmCvar_t	g_log;
vmCvar_t	g_logSync;
vmCvar_t	g_blood;
vmCvar_t	g_allowVote;
vmCvar_t	g_banIPs;
vmCvar_t	g_filterBan;
vmCvar_t	g_smoothClients;
vmCvar_t	g_unlagged;
vmCvar_t	pmove_fixed;
vmCvar_t	pmove_msec;
vmCvar_t	g_listEntity;
vmCvar_t	g_quadfactor;

vmCvar_t	dfx_gametype;
vmCvar_t	dfx_obs;
vmCvar_t	dfx_interference;
vmCvar_t	dfx_reversemap;
vmCvar_t	dfx_ruleset;
vmCvar_t	dfx_mode;
vmCvar_t	dfx_fastcapmode;

static cvarTable_t gameCvarTable[] = {
	// don't override the cheat state set by the system
    { &g_cheats, "sv_cheats", "", 0, 0, qfalse },

    // noset vars
	{ NULL, "gamename", GAMEVERSION , CVAR_SERVERINFO | CVAR_ROM, 0, qfalse  },
	{ NULL, "gamedate", __DATE__ , CVAR_ROM, 0, qfalse  },

	{ &g_restarted, "g_restarted", "0", CVAR_ROM, 0, qfalse  },
	{ &g_mapname, "mapname", "", CVAR_SERVERINFO | CVAR_ROM, 0, qfalse  },
	{ &sv_fps, "sv_fps", "125", CVAR_SERVERINFO|CVAR_SYSTEMINFO|CVAR_ROM, 0, qfalse  },
	{ &sv_pure, "sv_pure", "0", CVAR_SERVERINFO|CVAR_SYSTEMINFO|CVAR_ROM, 0, qfalse  },

	// latched vars
	//{ &g_gametype, "g_gametype", "0", CVAR_SERVERINFO | CVAR_USERINFO | CVAR_LATCH, 0, qfalse  }, // q3e
	{ &g_gametype,               "g_gametype",              "0",    CVAR_SERVERINFO|CVAR_USERINFO|CVAR_LATCH|CVAR_ROM, 0, qfalse }, // we use dfx_gametype instead for changes

    { &dfx_gametype,             "dfx_gametype",            "0",    CVAR_SERVERINFO|CVAR_LATCH, 0, qfalse },
    { &dfx_obs,                  "dfx_obs",                 "1",    CVAR_SERVERINFO|CVAR_LATCH, 0, qfalse },
    { &dfx_interference,         "dfx_interference",        "0",    CVAR_SERVERINFO|CVAR_LATCH, 0, qfalse },
    { &dfx_reversemap,           "dfx_reversemap",          "0",    CVAR_SERVERINFO|CVAR_LATCH, 0, qfalse },
    { &dfx_ruleset,              "dfx_ruleset",             "0",    CVAR_SERVERINFO|CVAR_LATCH, 0, qfalse },
	{ &dfx_mode,                 "dfx_mode",                "0",    CVAR_SERVERINFO|CVAR_LATCH, 0, qfalse },
	{ &dfx_fastcapmode,          "dfx_fastcapmode",         "0",    CVAR_SERVERINFO|CVAR_LATCH, 0, qfalse },

	{ &g_maxclients, "sv_maxclients", "8", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },
	{ &g_maxGameClients, "g_maxGameClients", "0", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },

	// change anytime vars
  //{ &g_dmflags,   "dmflags", "0",  CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue },    // q3e
	{ &g_fraglimit, "fraglimit", "20", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_timelimit, "timelimit", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
  //{ &g_capturelimit, "capturelimit", "8", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue }, //q3e

  //{ &g_synchronousClients, "g_synchronousClients", "0", CVAR_SYSTEMINFO, 0, qfalse  },    // q3e
  { &g_synchronousClients, "g_synchronousClients", "0", CVAR_SERVERINFO,   0, qfalse }, //dfx

/* qe3
  { &g_friendlyFire, "g_friendlyFire", "0", CVAR_ARCHIVE, 0, qtrue  },
  { &g_autoJoin, "g_autoJoin", "1", CVAR_ARCHIVE  },
  { &g_teamForceBalance, "g_teamForceBalance", "0", CVAR_ARCHIVE  },
*/

	{ &g_warmup, "g_warmup", "20", CVAR_ARCHIVE, 0, qtrue  },
	{ &g_log, "g_log", "games.log", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_logSync, "g_logSync", "0", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_password, "g_password", "", CVAR_USERINFO, 0, qfalse  },

	{ &g_banIPs, "g_banIPs", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_filterBan, "g_filterBan", "1", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_needpass, "g_needpass", "0", CVAR_SERVERINFO | CVAR_ROM, 0, qfalse },
	{ &g_dmflags,   "dmflags", "0",  CVAR_SERVERINFO | CVAR_ROM,     0, qtrue },

	{ &g_dedicated, "dedicated", "0", 0, 0, qfalse  },

	{ &g_speed, "g_speed", "320", 0, 0, qtrue  },
	{ &g_gravity, "g_gravity", "800", 0, 0, qtrue  },
	{ &g_knockback, "g_knockback", "1000", 0, 0, qtrue  },
	{ &g_quadfactor, "g_quadfactor", "3", 0, 0, qtrue  },
	{ &g_weaponRespawn, "g_weaponrespawn", "5", 0, 0, qtrue  },
	//{ &g_weaponTeamRespawn, "g_weaponTeamRespawn", "30", 0, 0, qtrue },
	{ &g_forcerespawn, "g_forcerespawn", "20", 0, 0, qtrue },
	{ &g_inactivity, "g_inactivity", "0", 0, 0, qtrue },
	{ &g_debugMove, "g_debugMove", "0", 0, 0, qfalse },
	{ &g_debugDamage, "g_debugDamage", "0", 0, 0, qfalse },
	{ &g_debugAlloc, "g_debugAlloc", "0", 0, 0, qfalse },
	{ &g_motd, "g_motd", "", 0, 0, qfalse },
	{ &g_blood, "com_blood", "1", 0, 0, qfalse },

	{ &g_allowVote, "g_allowVote", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &g_listEntity, "g_listEntity", "0", 0, 0, qfalse },

	{ &g_unlagged, "g_unlagged", "1", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qfalse },
	{ &g_predictPVS, "g_predictPVS", "0", CVAR_ARCHIVE, 0, qfalse },

	{ &g_smoothClients, "g_smoothClients", "1", 0, 0, qfalse},
	//{ &pmove_fixed, "pmove_fixed", "1", CVAR_SYSTEMINFO, 0, qfalse},
	//{ &pmove_msec, "pmove_msec", "8", CVAR_SYSTEMINFO, 0, qfalse}
	{ &pmove_fixed, "pmove_fixed", "1", CVAR_SERVERINFO|CVAR_SYSTEMINFO|CVAR_ROM, 0, qfalse},
	{ &pmove_msec, "pmove_msec", "8", CVAR_SERVERINFO|CVAR_SYSTEMINFO|CVAR_ROM, 0, qfalse}
};

static void G_InitGame( int levelTime, int randomSeed, int restart );
static void G_RunFrame( int levelTime );
static void G_ShutdownGame( int restart );
static void SendScoreboardMessageToAllClients( void );
// extension interface
qbool		(*trap_GetValue)( char *value, int valueSize, const char *key );
int			svf_self_portal2;

/*
================
vmMain

This is the only way control passes into the module.
This must be the very first function compiled into the .q3vm file
================
*/
DLLEXPORT intptr_t vmMain( int command, int arg0, int arg1, int arg2 )
{
	switch ( command ) {
	case GAME_INIT:
		G_InitGame( arg0, arg1, arg2 );
		return 0;
	case GAME_SHUTDOWN:
		G_ShutdownGame( arg0 );
		return 0;
	case GAME_CLIENT_CONNECT:
		return (intptr_t)ClientConnect( arg0, arg1, arg2 );
	case GAME_CLIENT_THINK:
		ClientThink( arg0 );
		return 0;
	case GAME_CLIENT_USERINFO_CHANGED:
		ClientUserinfoChanged( arg0 );
		return 0;
	case GAME_CLIENT_DISCONNECT:
		ClientDisconnect( arg0 );
		return 0;
	case GAME_CLIENT_BEGIN:
		ClientBegin( arg0 );
		return 0;
	case GAME_CLIENT_COMMAND:
		ClientCommand( arg0 );
		return 0;
	case GAME_RUN_FRAME:
		G_RunFrame( arg0 );
		return 0;
	case GAME_CONSOLE_COMMAND:
		return ConsoleCommand();
	case BOTAI_START_FRAME:
		 return 0;
	}
	return -1;
}

float trap_Cvar_VariableValue( const char *var_name )
{
	char buf[128];

	trap_Cvar_VariableStringBuffer(var_name, buf, sizeof(buf));
	return atof(buf);
}


void QDECL G_Printf( const char *fmt, ... )
{
	va_list		argptr;
	char		text[BIG_INFO_STRING];
	int			len;

	va_start (argptr, fmt);
	len = ED_vsprintf( text, fmt, argptr );
	va_end (argptr);

	if ( len <= 4095 ) trap_Print( text );
}


void G_BroadcastServerCommand( int ignoreClient, const char *command )
{
	int i;
	for ( i = 0; i < level.maxclients; i++ )
	{
		if ( i == ignoreClient ) continue;
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) trap_SendServerCommand( i, command );
	}
}


void QDECL G_Error( const char *fmt, ... )
{
	va_list	argptr;
	char	text[1024];

	va_start( argptr, fmt );
	ED_vsprintf( text, fmt, argptr );
	va_end( argptr );

	trap_Error( text );
}


/*
================
G_FindTeams

Chain together all entities with a matching team field.
Entity teams are used for item groups and multi-entity mover groups.

All but the first will have the FL_TEAMSLAVE flag set and teammaster field set
All but the last will have the teamchain field set to the next one
================
*/
void G_FindTeams( void )
{
	gentity_t	*e, *e2;
	int			i, j;
	int			c=0, c2=0;

	for ( i=MAX_CLIENTS, e=g_entities+i ; i < level.num_entities ; i++,e++ ){
		if ((!e->inuse)||(!e->team)||(e->flags&FL_TEAMSLAVE)) continue;

		e->teammaster = e;
		c++;
		c2++;

		for (j=i+1, e2=e+1 ; j < level.num_entities ; j++,e2++)
		{
			if ((!e2->inuse)||(!e2->team)||(e2->flags & FL_TEAMSLAVE)) continue;

			if (!strcmp(e->team, e2->team))
			{
				c2++;
				e2->teamchain = e->teamchain;
				e->teamchain = e2;
				e2->teammaster = e;
				e2->flags |= FL_TEAMSLAVE;

				// make sure that targets only point at the master
				if ( e2->targetname ) {
					e->targetname = e2->targetname;
					e2->targetname = NULL;
				}
			}
		}
	}

	G_Printf ("%i teams with %i entities\n", c, c2);
}


/*
=================
G_RegisterCvars
=================
*/
void G_RegisterCvars( void )
{
	cvarTable_t *cv;
	int         i;

	for ( i = 0, cv = gameCvarTable ; i < ARRAY_LEN( gameCvarTable ) ; i++, cv++ ) {
		trap_Cvar_Register( cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags );

		if ( cv->vmCvar )
			cv->modificationCount = cv->vmCvar->modificationCount;
	}

	// check some things
	if (dfx_gametype.integer < 0 || dfx_gametype.integer >= GT_MAX_GAME_TYPE)
	{
		G_Printf( "dfx_gametype %i is out of range, defaulting to GT_DEFRAG\n", dfx_gametype.integer );
		trap_Cvar_Set ( "dfx_gametype", "2" ); // defrag as default
		trap_Cvar_Update( &dfx_gametype );
	}
	
	if (dfx_gametype.integer != GT_DEFRAG && dfx_reversemap.integer) {
		G_Printf( "Setting dfx_reversemap to 0, since it has no effect in dfx_gametype %i\n", dfx_gametype.integer );
		trap_Cvar_Set ( "dfx_reversemap", "0" );
		trap_Cvar_Update ( &dfx_gametype );
	}

	if (dfx_ruleset.integer < 0 || dfx_ruleset.integer >= RS_MAX_RULESET)
	{
		G_Printf( "dfx_ruleset %i is out of range, defaulting to RS_VANILLA\n", dfx_ruleset.integer );
		trap_Cvar_Set ( "dfx_ruleset", "0" ); // vanilla as default
		trap_Cvar_Update( &dfx_ruleset );
	}

	if (dfx_mode.integer < 0 || dfx_mode.integer > DF_ICEMODE)
	{
		G_Printf( "dfx_mode %i is out of range, defaulting to DF_DEFAULT\n", dfx_mode.integer );
		trap_Cvar_Set ( "dfx_mode", "0" ); // default
		trap_Cvar_Update( &dfx_mode );
	}

	if (dfx_fastcapmode.integer < 0 || dfx_fastcapmode.integer > FC_COMBO)
	{
		G_Printf( "dfx_fastcapmode %i is out of range, defaulting to FC_DEFAULT\n", dfx_fastcapmode.integer );
		trap_Cvar_Set ( "dfx_fastcapmode", "0" ); // default
		trap_Cvar_Update( &dfx_fastcapmode );
	}

	// SLK: dirty fix :)
	trap_Cvar_Set( "sv_pure", "0" );
	trap_Cvar_Update( &sv_pure );

	level.warmupModificationCount = g_warmup.modificationCount;
}

/*
===================
G_HandleTeleporters
disable/enable teleporters in fastcap
===================
*/
static void G_HandleTeleporters( void ) {

	gentity_t *ent = NULL;
	extern int pm_noteles;

	while( (ent = G_Find( ent, FOFS(classname), "trigger_teleport" )) != NULL ) {
		if ( pm_noteles == 1 ) {
			trap_UnlinkEntity( ent );
		} else {
			trap_LinkEntity( ent );
		}
	}

}


/*
===========================================================
G_FixTimersForMapreverse
Set up the start/stop timer wait values for dfx_mapinvert
===========================================================
*/
static void G_FixTimersForMapreverse( void ) {
	gentity_t *target = NULL;
	gentity_t *ent	  = NULL;
	extern int pm_reversemap;

	while( ( ent = G_Find( ent, FOFS(classname), "trigger_multiple" ) ) != NULL )
	{
		while( ( target = G_Find( target, FOFS(targetname), ent->target ) ) != NULL )
		{
			if (!Q_stricmp(target->classname, "target_starttimer")) {
				if ( pm_reversemap == 1 ) {
					ent->wait = 0.5;
				} else {
					ent->wait = -1;
				}
			} else if (!Q_stricmp(target->classname, "target_stoptimer")) {
				if ( pm_reversemap == 1 ) {
					ent->wait = -1;
				} else {
					ent->wait = 0.5;
				}
			}
		}
	}
}


/*
=================
G_UpdateCvars
=================
*/
static void G_UpdateCvars( void ) {
	int			i;
	cvarTable_t	*cv;
	qbool		updateShared = qfalse;

	for ( i = 0, cv = gameCvarTable ; i < ARRAY_LEN( gameCvarTable ) ; i++, cv++ ) {

		if (cv->vmCvar) {

			trap_Cvar_Update(cv->vmCvar);

			if (cv->modificationCount != cv->vmCvar->modificationCount) {
				cv->modificationCount = cv->vmCvar->modificationCount;

				if (cv->trackChange)
					G_BroadcastServerCommand(-1, va("print \"^7Server: ^2%s ^7: %s\n\"", cv->cvarName, cv->vmCvar->string));


				if (!strcmp(cv->cvarName,"dfx_gametype"))
				{
					trap_SetConfigstring(CS_GAMETYPE, va("%d", dfx_gametype.integer));
					updateShared = qtrue;
				} else if(!strcmp(cv->cvarName, "dfx_ruleset"))
				{
					trap_SetConfigstring(CS_RULESET, va("%d", dfx_ruleset.integer));
					updateShared = qtrue;
				} else if(!strcmp(cv->cvarName, "dfx_mode"))
				{
					if ( dfx_gametype.integer == GT_DEFRAG ) {
						trap_SetConfigstring(CS_DFMODE, va("%d", dfx_mode.integer));
						updateShared = qtrue;
					} else {
						G_BroadcastServerCommand(-1, va("print \"^7dfx_mode can only be set in GT_DEFRAG\n\""));
					}
				} else if(!strcmp(cv->cvarName, "dfx_fastcapmode"))
				{
					if ( dfx_gametype.integer == GT_FASTCAP ) {
						trap_SetConfigstring(CS_FCMODE, va("%d", dfx_fastcapmode.integer));
						updateShared = qtrue;
					} else {
						G_BroadcastServerCommand(-1, va("print \"^7dfx_fastcapmode can only be set in GT_FASTCAP\n\""));
					}
				} else if(!strcmp(cv->cvarName, "dfx_obs")) {
					trap_SetConfigstring(CS_OVERBOUNCES, va("%d", dfx_obs.integer));
					updateShared = qtrue;
				} else if(!strcmp(cv->cvarName, "dfx_interference")) {
					trap_SetConfigstring(CS_INTERFERENCE, va("%d", dfx_interference.integer));
					updateShared = qtrue;
				} else if(!strcmp(cv->cvarName, "dfx_reversemap")) {
					trap_SetConfigstring(CS_REVERSEMAP, va("%d", dfx_reversemap.integer));
					updateShared = qtrue;
				}
			} // if (cv->modificationCount != cv->vmCvar->modificationCount)
		} //if (cv->vmCvar)
	} // for

	if(updateShared) {
            // set a bunch of cvars here after dfx_ruleset has changed
            if ( dfx_ruleset.integer == RS_CPM || dfx_ruleset.integer == RS_XCPM ) {
                trap_Cvar_Set( "g_quadfactor", "4" );
                trap_Cvar_Set( "g_forcerespawn", "3" );
                trap_Cvar_Set( "g_weaponrespawn", "15" );
            } else {
                trap_Cvar_Set( "g_quadfactor", "3" );
                trap_Cvar_Set( "g_forcerespawn", "20" );
                trap_Cvar_Set( "g_weaponrespawn", "5" );
            }

            // switch dfx_mode to 0 when we are not playing defrag
            if ( dfx_mode.integer != DF_DEFAULT && dfx_gametype.integer != GT_DEFRAG )
                trap_Cvar_Set( "dfx_mode", "0" );
            // switch dfx_fastcapmode to 0 when we are not playing fastcap
            if ( dfx_fastcapmode.integer != FC_DEFAULT && dfx_gametype.integer != GT_FASTCAP )
                trap_Cvar_Set( "dfx_fastcapmode", "0" );
            // switch dfx_mapreverse to 0 if we are not playing defrag
            if ( dfx_gametype.integer != GT_DEFRAG && dfx_reversemap.integer == 1)
                trap_Cvar_Set( "dfx_reversemap", "0" );

            // update all shared cvars
            BG_SharedCvarsUpdate( dfx_gametype.integer, dfx_ruleset.integer,
                                  dfx_mode.integer, dfx_fastcapmode.integer,
                                  dfx_obs.integer, dfx_interference.integer,
                                  dfx_reversemap.integer );


            // link/unlink all teleporters
            if ( dfx_gametype.integer == GT_FASTCAP ) {
                if ( dfx_fastcapmode.integer == FC_NOTELES || dfx_fastcapmode.integer == FC_NOTELES_NOPADS || dfx_fastcapmode.integer >= FC_DISABLEALL ) {
                    G_HandleTeleporters ();
                }
            }

            // finally perform a map_restart
            trap_SendConsoleCommand( EXEC_APPEND, "map_restart 3\n" );
    }

}

/*----------------------
 * G_LocateTeamSpawnSpots
 * --------------------*/
static void G_LocateTeamSpawnSpots( void )
{
	gentity_t	*ent;
	int			i, n=0;
	
	level.redSpawnSpot = qfalse;
	level.blueSpawnSpot = qfalse;
	level.greenSpawnSpot = qfalse;
	level.yellowSpawnSpot = qfalse;
	
	level.TeamSpawnSpots[ SPAWN_SPOT_TEAM_INTERMISSION ] = NULL;

	// locate all spawn spots
	ent = g_entities + MAX_CLIENTS;
	for ( i = MAX_CLIENTS; i < MAX_GENTITIES; i++, ent++ )
	{

		if (!ent->inuse || !ent->classname)
			continue;

		// intermission/player spots
		if ( !Q_stricmpn( ent->classname, "info_player_", 12 ) )
		{
			if ( !Q_stricmp( ent->classname + 12, "intermission" ) )
			{
				if ( level.TeamSpawnSpots[ SPAWN_SPOT_TEAM_INTERMISSION ] == NULL )
				{
					level.TeamSpawnSpots[ SPAWN_SPOT_TEAM_INTERMISSION ] = ent; // put in the PRE-last slot
					ent->fteam = TEAM_FREE;
				}
				continue;
			}
			
			if (!Q_stricmp( ent->classname + 12, "red") ) {
				level.numTeamSpawnSpots++;
				level.TeamSpawnSpots[n]=ent; n++;
				ent->fteam = TEAM_RED;
				ent->count = 1;
			}
			
			if (!Q_stricmp( ent->classname + 12, "blue") ) {
				level.numTeamSpawnSpots++;
				level.TeamSpawnSpots[n]=ent; n++;
				ent->fteam = TEAM_BLUE;
				ent->count = 1;
			}
				
			if (!Q_stricmp( ent->classname + 12, "green") ) {
				level.numTeamSpawnSpots++;
				level.TeamSpawnSpots[n]=ent; n++;
				ent->fteam = TEAM_GREEN;
				ent->count = 1;
			}
			
			if (!Q_stricmp( ent->classname + 12, "yellow") ) {
				level.numTeamSpawnSpots++;
				level.TeamSpawnSpots[n]=ent; n++;
				ent->fteam = TEAM_YELLOW;
				ent->count = 1;
			}
			
			continue;
		}
	}
	
	level.numTeamSpawnSpots = n;
}


/*----------------------
 * G_LocateSpawnSpots
 * --------------------*/

static void G_LocateSpawnSpots( void )
{
	gentity_t	*ent;
	int			i, n=0;
	extern int	pm_reversemap;

	level.spawnSpots[ SPAWN_SPOT_INTERMISSION ] = NULL;

	// locate all spawn spots
	ent = g_entities + MAX_CLIENTS;
	for ( i = MAX_CLIENTS; i < MAX_GENTITIES; i++, ent++ )
	{

		if (!ent->inuse || !ent->classname)
			continue;

		// intermission/player spots
		if ( !Q_stricmpn( ent->classname, "info_player_", 12 ) )
		{
			if ( !Q_stricmp( ent->classname + 12, "intermission" ) )
			{
				if ( level.spawnSpots[ SPAWN_SPOT_INTERMISSION ] == NULL )
				{
					level.spawnSpots[ SPAWN_SPOT_INTERMISSION ] = ent; // put in the last slot
					ent->fteam = TEAM_FREE;
				}
				continue;
			}

			if ( !Q_stricmp( ent->classname + 12, "start" ) || !Q_stricmp( ent->classname + 12, "deathmatch" ) )
			{
				if(pm_reversemap&&(ent->spawnflags&1) && dfx_gametype.integer==GT_DEFRAG)
				{

					if (!level.revStart)
					{
						level.revStart=qtrue; // we found at least one reverse playerstart
					}

					level.numSpawnSpotsFFA++;
					level.spawnSpots[n]=ent; n++;
					ent->fteam = TEAM_FREE;
					ent->count = 1;
				}
				else if(!pm_reversemap&& !(ent->spawnflags&1))
				{
					level.numSpawnSpotsFFA++;
					level.spawnSpots[n] = ent; n++;
					ent->fteam = TEAM_FREE;
					ent->count = 1;
				}
				continue;
			}
			continue;
		}

	}
	
	level.numSpawnSpots = n;
}

/*
=========================================================================
G_CreateReverseSpawnpoint
creates a emergency Spawnpoint by a copy of the target_stopTimer entity
=========================================================================
*/
static void G_CreateReverseSpawnpoint( void ) {
	gentity_t *ent = G_Find( NULL, FOFS(classname), "target_stopTimer");
	gentity_t *spawnpoint = G_Spawn();

	if(ent)
	{
		VectorCopy(ent->s.origin, spawnpoint->s.origin);

		spawnpoint->classname = "info_player_start";
		spawnpoint->spawnflags = 1;
		spawnpoint->s.eType = ET_SPAWNPOINT;
		spawnpoint->s.legsAnim = LEGS_IDLE;
		spawnpoint->s.torsoAnim = TORSO_STAND;
		spawnpoint->fteam = TEAM_FREE;
		spawnpoint->count = 1;

		level.spawnSpots[level.numSpawnSpots] = spawnpoint;
		level.numSpawnSpots++;

		trap_LinkEntity( spawnpoint );
	}
}


/*
============
G_InitGame

============
*/
static void G_InitGame( int levelTime, int randomSeed, int restart )
{
	char value[ MAX_CVAR_VALUE_STRING ];
	int	i;

	G_Printf ("------- Game Initialization -------\n");
	G_Printf ("gamename: %s\n", GAMEVERSION);
	G_Printf ("gamedate: %s\n", __DATE__);

	// extension interface
	trap_Cvar_VariableStringBuffer( "//trap_GetValue", value, sizeof( value ) );
	if ( value[0] )
	{
		trap_GetValue = (void*)~atoi( value );

		if ( trap_GetValue( value, sizeof( value ), "SVF_SELF_PORTAL2_Q3E" ) )
		{
			svf_self_portal2 = atoi( value );
		}
		else
		{
			svf_self_portal2 = 0;
		}
	}

	srand( randomSeed );
	G_RegisterCvars();
	G_ProcessIPBans();
	G_InitMemory();

	// set all the gametype/mode relevant vars
	BG_SharedCvarsUpdate( dfx_gametype.integer, dfx_ruleset.integer, dfx_mode.integer,
						  dfx_fastcapmode.integer, dfx_obs.integer, dfx_interference.integer, dfx_reversemap.integer );

	//set configstrings
	trap_SetConfigstring( CS_GAMETYPE,     va("%d", dfx_gametype.integer ) );
	trap_SetConfigstring( CS_RULESET,      va("%d", dfx_ruleset.integer  ) );
	trap_SetConfigstring( CS_DFMODE,       va("%d", dfx_mode.integer  ) );
	trap_SetConfigstring( CS_FCMODE,       va("%d", dfx_fastcapmode.integer ) );
	trap_SetConfigstring( CS_REVERSEMAP,   va("%d", dfx_reversemap.integer ) );
	trap_SetConfigstring( CS_OVERBOUNCES,  va("%d", dfx_obs.integer ) );
	trap_SetConfigstring( CS_INTERFERENCE, va("%d", dfx_interference.integer ) );

	//reset music configstring, else a triggered target_music keeps active on maprestart!
	trap_SetConfigstring( CS_MUSIC, "" );

	// set some level globals
	memset( &level, 0, sizeof( level ) );
	level.time = levelTime;
	level.startTime = levelTime;
	level.previousTime = levelTime;
	level.msec = FRAMETIME;
	level.snd_fry = G_SoundIndex("sound/player/fry.wav");	// FIXME standing in lava / slime

	if(g_logSync.integer)
	{
		trap_FS_FOpenFile(g_log.string, &level.logFile, FS_APPEND_SYNC);
	}
	else
	{
		trap_FS_FOpenFile(g_log.string, &level.logFile, FS_APPEND);
	}

	if(level.logFile==FS_INVALID_HANDLE)
	{
		G_Printf("WARNING: Couldn't open logfile: %s\n", g_log.string);
	}
	else
	{
		char	serverinfo[MAX_INFO_STRING];
		trap_GetServerinfo(serverinfo, sizeof(serverinfo));
		G_LogPrintf("------------------------------------------------------------\n");
		G_LogPrintf("InitGame: %s\n", serverinfo);
	}

	G_InitWorldSession();

	// initialize all entities for this game
	memset( g_entities, 0, MAX_GENTITIES * sizeof(g_entities[0]) );
	level.gentities = g_entities;
	// initialize all clients for this game
	level.maxclients = g_maxclients.integer;
	memset( g_clients, 0, MAX_CLIENTS * sizeof(g_clients[0]) );
	level.clients = g_clients;

	// set client fields on player ents
	for(i=0 ; i<level.maxclients ; i++)
	{
		g_entities[i].client = level.clients+i;
	}
	// always leave room for the max number of clients,
	// even if they aren't all used, so numbers inside that
	// range are NEVER anything but clients
	level.num_entities = MAX_CLIENTS;
	for(i = 0 ; i<MAX_CLIENTS ; i++)
	{
		g_entities[i].classname="clientslot";
	}
	// let the server system know where the entites are
	trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ), &level.clients[0].ps, sizeof( level.clients[0] ) );
	// reserve some spots for dead player bodies
	InitBodyQue();
	ClearRegisteredItems();
	// parse the key/value pairs and spawn gentities
	G_SpawnEntitiesFromString();

	// fix start/stop timers wait values for reversemap
	if(dfx_gametype.integer==GT_DEFRAG && dfx_reversemap.integer)
	{
		G_FixTimersForMapreverse();
	}

	// general initialization
	G_FindTeams();

	SaveRegisteredItems();
	if (dfx_gametype.integer == GT_TEAMRUN) {
		G_LocateTeamSpawnSpots();
	} else {
		G_LocateSpawnSpots();
	}

	// if we want to play a map backwards in defrag and theres no playerspawn for this mode, we need
	// to create one somewhere next to the target_stopTimer entity
	if(!level.revStart && dfx_gametype.integer == GT_DEFRAG && dfx_reversemap.integer)
	{
		G_CreateReverseSpawnpoint ();
	}
	G_Printf("-----------------------------------\n");

	// don't forget to reset times
	trap_SetConfigstring(CS_INTERMISSION, "");
	// must be, else non 8 framed times get produced
	trap_Cvar_Set("sv_fps", "125");
}


/*
=================
G_ShutdownGame
=================
*/
static void G_ShutdownGame( int restart )
{
	G_Printf ("==== ShutdownGame ====\n");
	if(level.logFile != FS_INVALID_HANDLE)
	{
		G_LogPrintf("ShutdownGame:\n" );
		G_LogPrintf("------------------------------------------------------------\n" );
		trap_FS_FCloseFile( level.logFile );
		level.logFile = FS_INVALID_HANDLE;
	}
	// write all the client session data so we can get it back
	G_WriteSessionData();
}



//===================================================================

#ifndef GAME_HARD_LINKED
// this is only here so the functions in q_shared.c and bg_*.c can link

void QDECL Com_Error( int level, const char *fmt, ... )
{
	va_list		argptr;
	char		text[4096];

	va_start( argptr, fmt );
	ED_vsprintf( text, fmt, argptr );
	va_end( argptr );

	trap_Error( text );
}

void QDECL Com_Printf( const char *fmt, ... )
{
	va_list		argptr;
	char		text[4096];

	va_start( argptr, fmt );
	ED_vsprintf( text, fmt, argptr );
	va_end( argptr );

	trap_Print( text );
}

#endif

/*
========================================================================
PLAYER COUNTING / SCORE SORTING
========================================================================
*/


/*
=============
SortRanks
=============
*/
static int QDECL SortRanks( const void *a, const void *b )
{
	gclient_t	*ca, *cb;

	ca = &level.clients[*(int *)a];
	cb = &level.clients[*(int *)b];

	// sort special clients last
	if ( ca->sess.spectatorState == SPECTATOR_SCOREBOARD || ca->sess.spectatorClient < 0 )  return 1;
	if ( cb->sess.spectatorState == SPECTATOR_SCOREBOARD || cb->sess.spectatorClient < 0  ) return -1;

	// then connecting clients
	if ( ca->pers.connected == CON_CONNECTING ) return 1;
	if ( cb->pers.connected == CON_CONNECTING ) return -1;

	//check defrag times first so switching to spec doesnt make taken the lead messages play
	if ( dfx_gametype.integer != GT_FREESTYLE ) {
		// sort by time, lowest to highest

		// 0 means no time yet, not first place
		if ( ca->pers.dfBestTime == 0 ) {
			if ( cb->pers.dfBestTime == 0 )	return 0;
			return 1;
		}

		if ( cb->pers.dfBestTime == 0 ) {
			if ( ca->pers.dfBestTime == 0 ) return 0;
			return -1;
		}

		if ( ca->pers.dfBestTime > cb->pers.dfBestTime ) return 1;
		if ( ca->pers.dfBestTime < cb->pers.dfBestTime ) return -1;
	}

    // then spectators
    if ( ca->sess.sessionTeam == TEAM_SPECTATOR && cb->sess.sessionTeam == TEAM_SPECTATOR ) {
		if ( ca->sess.spectatorClient > cb->sess.spectatorClient ) return -1;
		if ( ca->sess.spectatorClient < cb->sess.spectatorClient ) return 1;
		return 0;
	}
	if ( ca->sess.sessionTeam == TEAM_SPECTATOR ) return 1;
	if ( cb->sess.sessionTeam == TEAM_SPECTATOR ) return -1;

	// then sort by score
	if ( ca->ps.persistant[PERS_SCORE] > cb->ps.persistant[PERS_SCORE] ) return -1;
	if ( ca->ps.persistant[PERS_SCORE] < cb->ps.persistant[PERS_SCORE] ) return 1;

	return 0;
}


/*
============
CalculateRanks

Recalculates the score ranks of all players
This will be called on every client connect, begin, disconnect, death,
and team change.
============
*/
void CalculateRanks( void ) {
	int		i;
	int		rank;
	int		score;
	int		newScore;
	gclient_t	*cl;

	if ( level.restarted )
		return;

	level.follow1 = -1;
	level.follow2 = -1;
	level.numConnectedClients = 0;
	level.numNonSpectatorClients = 0;
	level.numPlayingClients = 0;
	level.numVotingClients = 0;		// don't count bots
	for (i = 0; i < ARRAY_LEN(level.numteamVotingClients); i++) {
		level.numteamVotingClients[i] = 0;
	}
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].pers.connected != CON_DISCONNECTED ) {
			level.sortedClients[level.numConnectedClients] = i;
			level.numConnectedClients++;

			if ( level.clients[i].sess.sessionTeam != TEAM_SPECTATOR ) {
				level.numNonSpectatorClients++;
			
				// decide if this should be auto-followed
				if ( level.clients[i].pers.connected == CON_CONNECTED ) {
					level.numPlayingClients++;
					//if ( !(g_entities[i].r.svFlags & SVF_BOT) ) {
						level.numVotingClients++;
						if ( level.clients[i].sess.sessionTeam == TEAM_RED )
							level.numteamVotingClients[0]++;
						else if ( level.clients[i].sess.sessionTeam == TEAM_BLUE )
							level.numteamVotingClients[1]++;
					//}
					if ( level.follow1 == -1 ) {
						level.follow1 = i;
					} else if ( level.follow2 == -1 ) {
						level.follow2 = i;
					}
				}
			}
		}
	}

	qsort( level.sortedClients, level.numConnectedClients, 
		sizeof(level.sortedClients[0]), SortRanks );

	// set the rank value for all clients that are connected and not spectators
	if ( g_gametype.integer == GT_TEAMRUN ) {
		// in team games, rank is just the order of the teams, 0=red, 1=blue, 2=tied
		for ( i = 0;  i < level.numConnectedClients; i++ ) {
			cl = &level.clients[ level.sortedClients[i] ];
			if ( level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE] ) {
				cl->ps.persistant[PERS_RANK] = 2;
			} else if ( level.teamScores[TEAM_RED] > level.teamScores[TEAM_BLUE] ) {
				cl->ps.persistant[PERS_RANK] = 0;
			} else {
				cl->ps.persistant[PERS_RANK] = 1;
			}
		}
	} else {	
		rank = -1;
		score = MAX_QINT;
		for ( i = 0;  i < level.numPlayingClients; i++ ) {
			cl = &level.clients[ level.sortedClients[i] ];
			newScore = cl->ps.persistant[PERS_SCORE];
			if ( i == 0 || newScore != score ) {
				rank = i;
				// assume we aren't tied until the next client is checked
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank;
			} else {
				// we are tied with the previous client
				level.clients[ level.sortedClients[i-1] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
			}
			score = newScore;
			//if ( g_gametype.integer == GT_SINGLE_PLAYER && level.numPlayingClients == 1 ) {
			//	level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
			//}
		}
	}

	// set the CS_SCORES1/2 configstrings, which will be visible to everyone
	if ( g_gametype.integer == GT_TEAMRUN ) {
		trap_SetConfigstring( CS_SCORES1, va("%i", level.teamScores[TEAM_RED] ) );
		trap_SetConfigstring( CS_SCORES2, va("%i", level.teamScores[TEAM_BLUE] ) );
		trap_SetConfigstring( CS_SCORES3, va("%i", level.teamScores[TEAM_GREEN] ) );
		trap_SetConfigstring( CS_SCORES4, va("%i", level.teamScores[TEAM_YELLOW] ) );
	} else {
		if ( level.numConnectedClients == 0 ) {
			trap_SetConfigstring( CS_SCORES1, va("%i", SCORE_NOT_PRESENT) );
			trap_SetConfigstring( CS_SCORES2, va("%i", SCORE_NOT_PRESENT) );
		} else if ( level.numConnectedClients == 1 ) {
			trap_SetConfigstring( CS_SCORES1, va("%i", level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE] ) );
			trap_SetConfigstring( CS_SCORES2, va("%i", SCORE_NOT_PRESENT) );
		} else {
			trap_SetConfigstring( CS_SCORES1, va("%i", level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE] ) );
			trap_SetConfigstring( CS_SCORES2, va("%i", level.clients[ level.sortedClients[1] ].ps.persistant[PERS_SCORE] ) );
		}
	}

	// see if it is time to end the level
	//CheckExitRules();

	// if we are at the intermission, send the new info to everyone
	if ( level.intermissiontime ) 
		SendScoreboardMessageToAllClients();

}

/*
========================================================================
MAP CHANGING
========================================================================
*/

/*
========================
SendScoreboardMessageToAllClients

Do this at BeginIntermission time and whenever ranks are recalculated
due to enters/exits/forced team changes
========================
*/
static void SendScoreboardMessageToAllClients(void)
{
	int	i;

	for(i=0 ; i<level.maxclients ; i++)
	{
		if(level.clients[i].pers.connected == CON_CONNECTED)
		{
			DeathmatchScoreboardMessage(g_entities+i);
		}
	}
}


/*
========================
MoveClientToIntermission

When the intermission starts, this will be called for all players.
If a new client connects, this will be called after the spawn function.
========================
*/
void MoveClientToIntermission( gentity_t *ent )
{

	gclient_t * client;
	client = ent->client;

	// take out of follow mode if needed
	if ( client->sess.spectatorState == SPECTATOR_FOLLOW ) StopFollowing( ent, qtrue );

	// move to the spot
	VectorCopy( level.intermission_origin, ent->s.origin );
	VectorCopy( level.intermission_origin, client->ps.origin );
	SetClientViewAngle( ent, level.intermission_angle );
	client->ps.pm_type = PM_INTERMISSION;

	// clean up powerup info
	memset( client->ps.powerups, 0, sizeof( client->ps.powerups ) );

	client->ps.eFlags = ( client->ps.eFlags & ~EF_PERSISTANT ) | ( client->ps.eFlags & EF_PERSISTANT );

	ent->s.eFlags = client->ps.eFlags;
	ent->s.eType = ET_GENERAL;
	ent->s.modelindex = 0;
	ent->s.loopSound = 0;
	ent->s.event = 0;
	ent->r.contents = 0;

	ent->s.legsAnim = LEGS_IDLE;
	ent->s.torsoAnim = TORSO_STAND;
}


/*
=================================
FindIntermissionPoint

This is used for spectator spawns
=================================
*/
void FindIntermissionPoint( void )
{
	gentity_t	*ent, *target;
	vec3_t		dir;

	// search only once
	if ( level.intermission_spot )
		return;

	// find the intermission spot
	if ( dfx_gametype.integer != GT_TEAMRUN ) {
		ent = level.spawnSpots[ SPAWN_SPOT_INTERMISSION ];
	} else {
		ent = level.TeamSpawnSpots[ SPAWN_SPOT_TEAM_INTERMISSION ];
	}
	
	if(!ent)
	{ // the map creator forgot to put in an intermission point...
		if ( dfx_gametype.integer != GT_TEAMRUN )
			SelectSpawnPoint( NULL, vec3_origin, level.intermission_origin, level.intermission_angle );
		else
			SelectTeamSpawnPoint( NULL, level.intermission_origin, level.intermission_angle);
	}
	else
	{
		VectorCopy (ent->s.origin, level.intermission_origin);
		VectorCopy (ent->s.angles, level.intermission_angle);
		// if it has a target, look towards it
		if ( ent->target )
		{
			target = G_PickTarget( ent->target );
			if ( target )
			{
				VectorSubtract( target->s.origin, level.intermission_origin, dir );
				vectoangles( dir, level.intermission_angle );
			}
		}
	}

	level.intermission_spot = qtrue;
}

/*
=================
G_LogPrintf

Print to the logfile with a time stamp if it is open
=================
*/
void QDECL G_LogPrintf( const char *fmt, ... )
{
	va_list		argptr;
	char		string[BIG_INFO_STRING];
	int			min, tens, sec, len, n;

	sec = level.time / 1000;

	min = sec / 60;
	sec -= min * 60;
	tens = sec / 10;
	sec -= tens * 10;

	Com_sprintf( string, sizeof(string), "%3i:%i%i ", min, tens, sec );
	len = (int)strlen( string );

	va_start( argptr, fmt );
	ED_vsprintf( string + len , fmt,argptr );
	va_end( argptr );

	n = (int)strlen( string );

	if ( g_dedicated.integer ) G_Printf( "%s", string + len );
	if ( level.logFile == FS_INVALID_HANDLE ) return;

	trap_FS_Write( string, n, level.logFile );
}


/*
================
LogExit

Append information about this game to the log file
================
*/
void LogExit( const char *string )
{
	int				i, numSorted;
	gclient_t		*cl;

	G_LogPrintf( "Exit: %s\n", string );

	level.intermissionQueued = level.time;

	// this will keep the clients from playing any voice sounds
	// that will get cut off when the queued intermission starts
	trap_SetConfigstring( CS_INTERMISSION, "1" );

	// don't send more than 32 scores (FIXME?)
	numSorted = level.numConnectedClients;
	if ( numSorted > 32 ) numSorted = 32;

	for ( i = 0 ; i < numSorted ; i++ )
    {
		int	ping;
		cl = &level.clients[level.sortedClients[i]];
		if ((cl->sess.sessionTeam == TEAM_SPECTATOR) || (cl->pers.connected == CON_CONNECTING)) continue;
		ping = cl->ps.ping < 999 ? cl->ps.ping : 999;
		G_LogPrintf( "score: %i  ping: %i  client: %i %s\n", cl->ps.persistant[PERS_SCORE], ping, level.sortedClients[i], cl->pers.netname );
	}

}


/*
=============
ScoreIsTied
=============
*/
static qbool ScoreIsTied( void )
{
	int	a, b;

	if ( level.numPlayingClients < 2 ) return qfalse;

	a = level.clients[level.sortedClients[0]].ps.persistant[PERS_SCORE];
	b = level.clients[level.sortedClients[1]].ps.persistant[PERS_SCORE];

	return a == b;
}


static void ClearBodyQue( void )
{
	int	i;
	gentity_t	*ent;

	for ( i = 0 ; i < BODY_QUEUE_SIZE ; i++ )
    {
		ent = level.bodyQue[i];
		if ( ent->r.linked || ent->physicsObject )
		{
			trap_UnlinkEntity( ent );
			ent->physicsObject = qfalse;
		}
	}
}


static void G_WarmupEnd( void )
{
	gclient_t *client;
	gentity_t *ent;
	int i, t;

	// remove corpses
	ClearBodyQue();

	// return flags
	//Team_ResetFlags();

	memset( level.teamScores, 0, sizeof( level.teamScores ) );

	level.warmupTime = 0;
	level.startTime = level.time;

	trap_SetConfigstring( CS_SCORES1, "0" );
	trap_SetConfigstring( CS_SCORES2, "0" );
	trap_SetConfigstring( CS_WARMUP,  ""  );
	trap_SetConfigstring( CS_LEVEL_START_TIME, va( "%i", level.startTime ) );

	client = level.clients;
	for ( i = 0; i < level.maxclients; i++, client++ )
	{
		if ( client->pers.connected != CON_CONNECTED )
			continue;

		client->ps.persistant[PERS_ATTACKER] = ENTITYNUM_NONE;
		client->ps.persistant[PERS_ATTACKEE_ARMOR] = 0;
		client->damage.enemy = client->damage.team = 0;

		client->ps.stats[STAT_CLIENTS_READY] = 0;
		client->ps.stats[STAT_HOLDABLE_ITEM] = 0;

		memset( &client->ps.powerups, 0, sizeof( client->ps.powerups ) );

		ClientUserinfoChanged( i ); // set max.health etc.

		if ( client->sess.sessionTeam != TEAM_SPECTATOR )
			ClientSpawn( level.gentities + i );

		//trap_SendServerCommand( i, "map_restart" );
	}

	// respawn items, remove projectiles, etc.
	ent = level.gentities + MAX_CLIENTS;
	for ( i = MAX_CLIENTS; i < level.num_entities ; i++, ent++ )
	{

		if ( !ent->inuse || ent->freeAfterEvent ) continue;

		if ( ent->tag == TAG_DONTSPAWN )
		{
			ent->nextthink = 0;
			continue;
		}

		if ( ent->s.eType == ET_ITEM && ent->item )
        {

			// already processed in Team_ResetFlags()
			if ( ent->item->giTag == PW_NEUTRALFLAG || ent->item->giTag == PW_REDFLAG || ent->item->giTag == PW_BLUEFLAG )
				continue;

			// remove dropped items
			if ( ent->flags & FL_DROPPED_ITEM )
			{
				ent->nextthink = level.time;
				continue;
			}

			// respawn picked up items
			t = SpawnTime( ent, qtrue );
			if ( t != 0 )
			{
				// hide items with defined spawn time
				ent->s.eFlags |= EF_NODRAW;
				ent->r.svFlags |= SVF_NOCLIENT;
				ent->r.contents = 0;
				ent->activator = NULL;
				ent->think = RespawnItem;
				ent->nextthink = level.time + t;
			}
			else
			{
				if ( ent->activator )
				{
					ent->activator = NULL;
					ent->think = RespawnItem;
				}
				ent->nextthink = level.time + FRAMETIME;
			}

		}
		else if ( ent->s.eType == ET_MISSILE )
		{
			// remove all launched missiles
			G_FreeEntity( ent );
		}
	}
}


/*
========================================================================
FUNCTIONS CALLED EVERY FRAME
========================================================================
*/


/*
==================
CheckVote
==================
*/
static void CheckVote( void )
{

	if ( level.voteExecuteTime )
	{
		 if ( level.voteExecuteTime < level.time )
		 {
			level.voteExecuteTime = 0;
			trap_SendConsoleCommand( EXEC_APPEND, va( "%s\n", level.voteString ) );
		 }
		 return;
	}

	if ( !level.voteTime ) return;

	if ( level.time - level.voteTime >= VOTE_TIME )
	{
		G_BroadcastServerCommand( -1, "print \"Vote failed.\n\"" );
	}
	else
	{
		if ( level.voteYes == 0 && level.voteNo == 0 )
		{
			return; //don't fail if spec called a vote
		} else if ( level.voteYes > level.numVotingClients/2 )
		{
			// execute the command, then remove the vote
			G_BroadcastServerCommand( -1, "print \"Vote passed.\n\"" );
			level.voteExecuteTime = level.time + 3000;
		} else if ( level.voteNo >= level.numVotingClients/2 )
		{
			// same behavior as a timeout
			G_BroadcastServerCommand( -1, "print \"Vote failed.\n\"" );
		} else
		{
			// still waiting for a majority
			return;
		}
	}

	level.voteTime = 0;
	trap_SetConfigstring( CS_VOTE_TIME, "" );
}


/*
==================
PrintTeam
==================
*/
static void PrintTeam( team_t team, const char *message )
{
	int i;

	for ( i = 0 ; i < level.maxclients ; i++ )
	{
		if ( level.clients[i].sess.sessionTeam != team )
			continue;
		if ( level.clients[i].pers.connected != CON_CONNECTED )
			continue;
		trap_SendServerCommand( i, message );
	}
}

/*
==================
CheckCvars
==================
*/
void CheckCvars( void )
{
	static int lastMod = -1;

	if ( lastMod != g_password.modificationCount )
	{
		lastMod = g_password.modificationCount;
		if ( g_password.string[0] && Q_stricmp( g_password.string, "none" ) != 0 )
			trap_Cvar_Set( "g_needpass", "1" );
		else
			trap_Cvar_Set( "g_needpass", "0" );
	}
}


/*
=============
G_RunThink

Runs thinking code for this frame if necessary
=============
*/
void G_RunThink( gentity_t *ent )
{
	int	thinktime;

	thinktime = ent->nextthink;
	if ( (thinktime <= 0) || (thinktime > level.time) ) return;

	ent->nextthink = 0;
	if ( !ent->think )
		G_Error ( "NULL ent->think");
	else
		ent->think (ent);
}


/*
================
G_RunFrame

Advances the non-player objects in the world
================
*/
static void G_RunFrame( int levelTime )
{
	int			i;
	gentity_t	*ent;
	gclient_t	*client;
	static	gentity_t *missiles[ MAX_GENTITIES - MAX_CLIENTS ];
	int		numMissiles;

	// if we are waiting for the level to restart, do nothing
	if ( level.restarted ) return;

	level.framenum++;
	level.previousTime = level.time;
	level.time = levelTime;
	level.msec = level.time - level.previousTime;

	// get any cvar changes
	G_UpdateCvars();
	numMissiles = 0;

	//
	// go through all allocated objects
	//
	ent = &g_entities[0];
	for (i=0 ; i<level.num_entities ; i++, ent++)
	{
		if ( !ent->inuse )
			continue;

		// clear events that are too old
		if ( level.time - ent->eventTime > EV_VALID_MSEC )
		{
			if ( ent->s.event )
			{
				ent->s.event = 0;
				if ( ent->client )
					ent->client->ps.externalEvent = 0;

			}

			if ( ent->freeAfterEvent )
			{
				// tempEntities or dropped items completely go away after their event
				G_FreeEntity( ent );
				continue;
			} else if ( ent->unlinkAfterEvent )
			{
				// items that will respawn will hide themselves after their pickup event
				ent->unlinkAfterEvent = qfalse;
				trap_UnlinkEntity( ent );
			}
		}

		// temporary entities don't think
		if ( ent->freeAfterEvent )
			continue;

		if ( !ent->r.linked && ent->neverFree )
			continue;

		if ( ent->s.eType == ET_MISSILE )
		{
			// queue for unlagged pass
			missiles[ numMissiles ] = ent;
			numMissiles++;
			continue;
		}

		if ( ent->s.eType == ET_ITEM || ent->physicsObject )
		{
			G_RunItem( ent );
			continue;
		}

		if ( ent->s.eType == ET_MOVER )
		{
			G_RunMover( ent );
			continue;
		}

		if ( i < MAX_CLIENTS )
		{
			client = ent->client;
			client->sess.spectatorTime += level.msec;
			if ( client->pers.connected == CON_CONNECTED )
				G_RunClient( ent );

			continue;
		}

		G_RunThink( ent );
	}

	if ( numMissiles )
	{
		// unlagged
		G_TimeShiftAllClients( level.previousTime, NULL );
		// run missiles
		for ( i = 0; i < numMissiles; i++ )
			G_RunMissile( missiles[ i ] );
		// unlagged
		G_UnTimeShiftAllClients( NULL );
	}

	// perform final fixups on the players
	ent = &g_entities[0];
	for (i = 0 ; i < level.maxclients ; i++, ent++ )
	{
		if ( ent->inuse )
			ClientEndFrame( ent );
	}

	// cancel vote if timed out
	CheckVote();
	// for tracking changes
	CheckCvars();

	if (g_listEntity.integer) {
		for (i = 0; i < MAX_GENTITIES; i++) {
			G_Printf("%4i: %s\n", i, g_entities[i].classname);
		}

		trap_Cvar_Set("g_listEntity", "0");
	}

	//unlagged
	level.frameStartTime = trap_Milliseconds();
}
