// Copyright (C) 1999-2000 Id Software, Inc.
//

#include "g_local.h"

level_locals_t	level;

typedef struct {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
	int			modificationCount;  // for tracking changes
	qboolean	trackChange;	    // track this variable, and announce if changed
  qboolean teamShader;        // track and if changed, update shader state
} cvarTable_t;

gentity_t		g_entities[MAX_GENTITIES];
gclient_t		g_clients[MAX_CLIENTS];

/****************************************/
//	Accuracy Challenge - back!!!
vmCvar_t	g_accuracyChallenge;
// g_worldDamage
vmCvar_t	g_worldDamage;
// log analyzer friendly mode
vmCvar_t	g_logCompatibilityMode;
vmCvar_t	g_sendStats;
//	Headhunting
vmCvar_t	g_headHunt;
vmCvar_t	g_headFromCorpse;
//	1 for accuracy*hits scoring, 0 for normal == default
vmCvar_t	g_scoring;
//	delayed fire alla quake2 BFG
vmCvar_t	g_railgunDelayedFire;
vmCvar_t	g_railgunFireDelay;
//	overal firerate modifier, effects all weapons
vmCvar_t	g_firerate;
//	spawn protection
vmCvar_t	g_spawnProtection;
vmCvar_t	g_spawnProtectionNoFire;
//	space protection
vmCvar_t	g_spaceProtection;
//	railgun explosions
vmCvar_t	g_railgunExplosions;
vmCvar_t	g_railgunBounces;
vmCvar_t	g_railgunSuicide;
//	offhand grapple
vmCvar_t	g_allowGrapple;
vmCvar_t	g_grappleSpeed;
vmCvar_t	g_grapplePull;
//	powerup spawn control
vmCvar_t	g_allowQuad;
vmCvar_t	g_allowHaste;
vmCvar_t	g_allowInvis;
//	always gib
vmCvar_t	g_alwaysGib;
//	invis option
vmCvar_t	g_invis;
vmCvar_t	g_invisFire;
vmCvar_t	g_invisFlicker;
//	anti camper
vmCvar_t	g_anticampTime;
vmCvar_t	g_anticampRadius;
vmCvar_t	g_anticamp;
//	improve bot ai hack
vmCvar_t	g_botHack;
// trippy corkscrew!
vmCvar_t	g_valkyrMode;
/****************************************/

vmCvar_t	g_gametype;
vmCvar_t	g_dmflags;
vmCvar_t	g_fraglimit;
vmCvar_t	g_timelimit;
vmCvar_t	g_capturelimit;
vmCvar_t	g_friendlyFire;
vmCvar_t	g_password;
vmCvar_t	g_needpass;
vmCvar_t	g_maxclients;
vmCvar_t	g_maxGameClients;
vmCvar_t	g_dedicated;
vmCvar_t	g_speed;
vmCvar_t	g_gravity;
vmCvar_t	g_cheats;
vmCvar_t	g_knockback;
vmCvar_t	g_quadfactor;
vmCvar_t	g_forcerespawn;
vmCvar_t	g_inactivity;
vmCvar_t	g_debugMove;
vmCvar_t	g_debugDamage;
vmCvar_t	g_debugAlloc;
vmCvar_t	g_weaponRespawn;
vmCvar_t	g_weaponTeamRespawn;
vmCvar_t	g_motd;
vmCvar_t	g_synchronousClients;
vmCvar_t	g_warmup;
vmCvar_t	g_doWarmup;
vmCvar_t	g_restarted;
vmCvar_t	g_log;
vmCvar_t	g_logSync;
vmCvar_t	g_blood;
vmCvar_t	g_podiumDist;
vmCvar_t	g_podiumDrop;
vmCvar_t	g_allowVote;
vmCvar_t	g_teamAutoJoin;
vmCvar_t	g_teamForceBalance;
vmCvar_t	g_banIPs;
vmCvar_t	g_filterBan;
vmCvar_t	g_smoothClients;
vmCvar_t	pmove_fixed;
vmCvar_t	pmove_msec;
vmCvar_t	g_rankings;
vmCvar_t	g_listEntity;
#ifdef MISSIONPACK
vmCvar_t	g_obeliskHealth;
vmCvar_t	g_obeliskRegenPeriod;
vmCvar_t	g_obeliskRegenAmount;
vmCvar_t	g_obeliskRespawnDelay;
vmCvar_t	g_cubeTimeout;
vmCvar_t	g_redteam;
vmCvar_t	g_blueteam;
vmCvar_t	g_singlePlayer;
vmCvar_t	g_enableDust;
vmCvar_t	g_enableBreath;
vmCvar_t	g_proxMineTimeout;
#endif

cvarTable_t		gameCvarTable[] = {

/**************************************************************************/
//	accuracy challenge
	{ &g_accuracyChallenge, "g_accuracyChallenge", "0", CVAR_SERVERINFO | CVAR_ROM, 0, qfalse  },
//	g_worldDamage
	{ &g_worldDamage, "g_worldDamage", "0", 0, 0, qtrue },
// log analyzer friendly mode
	{ &g_logCompatibilityMode, "g_logCompatibilityMode", "1", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_sendStats, "g_sendStats", "0", CVAR_ARCHIVE, 0, qfalse  },
//	head hunting
	{ &g_headHunt, "g_headHunt", "0", CVAR_SERVERINFO | CVAR_LATCH, 0, qtrue },
	{ &g_headFromCorpse, "g_headFromCorpse", "1", 0, 0, qtrue },
//	g_scoring, accuracy scoring
	{ &g_scoring, "g_scoring", "0", CVAR_LATCH, 0, qtrue  },
//	spawn protection
	{ &g_spawnProtectionNoFire, "g_spawnProtectionNoFire", "1", 0, 0, qtrue  },
	{ &g_spawnProtection, "g_spawnProtection", "2", 0, 0, qtrue  },
//	space protection
	{ &g_spaceProtection, "g_spaceProtection", "1", 0, 0, qtrue  },
//	railgun explosions
	{ &g_railgunExplosions, "g_railgunExplosions", "1", 0, 0, qtrue  },
//	railgun bounces
	{ &g_railgunBounces, "g_railgunBounces", "0", 0, 0, qtrue  },
	{ &g_railgunSuicide, "g_railgunSuicide", "0", 0, 0, qtrue  },
//	delayed fire alla quake2 BFG
	{ &g_railgunDelayedFire, "g_railgunDelayedFire", "0", 0, 0, qtrue  },
	{ &g_railgunFireDelay, "g_railgunFireDelay", "800", 0, 0, qtrue  },
//	overal firerate modifier
	{ &g_firerate, "g_firerate", "1", 0, 0, qtrue  },
//	offhand grapple
	{ &g_allowGrapple, "g_allowGrapple", "1", 0, 0, qtrue  },
	{ &g_grappleSpeed, "g_grappleSpeed", "1600", 0, 0, qtrue  },
	{ &g_grapplePull, "g_grapplePull", "800", 0, 0, qtrue  },
//	powerup spawn control
	{ &g_allowQuad, "g_allowQuad", "1", 0, 0, qtrue  },
	{ &g_allowHaste, "g_allowHaste", "1", 0, 0, qtrue  },
	{ &g_allowInvis, "g_allowInvis", "1", 0, 0, qtrue  },
//	always gib
	{ &g_alwaysGib, "g_alwaysGib", "0", 0, 0, qtrue  },
//	invis option
	{ &g_invisFire, "g_invisFire", "0", 0, 0, qtrue  },
	{ &g_invisFlicker, "g_invisFlicker", "1", 0, 0, qtrue  },
	{ &g_invis, "g_invis", "0", 0, 0, qtrue  },
//	anti camper
	{ &g_anticampTime, "g_anticampTime", "10", 0, 0, qtrue  },
	{ &g_anticampRadius, "g_anticampRadius", "192", 0, 0, qtrue  },
	{ &g_anticamp, "g_anticamp", "0", 0, 0, qtrue  },
//	improve bot ai hack
	{ &g_botHack, "g_botHack", "1", CVAR_LATCH, 0, qtrue  },
// trippy corkscrew
	{ &g_valkyrMode, "g_valkyrMode", "0", CVAR_SERVERINFO, 0, qtrue  },
/**************************************************************************/

	// don't override the cheat state set by the system
	{ &g_cheats, "sv_cheats", "", 0, 0, qfalse },

	// noset vars
	{ NULL, "gamename", GAMEVERSION , CVAR_SERVERINFO | CVAR_ROM, 0, qfalse  },
	{ NULL, "gamedate", __DATE__ , CVAR_ROM, 0, qfalse  },
	{ &g_restarted, "g_restarted", "0", CVAR_ROM, 0, qfalse  },
	{ NULL, "sv_mapname", "", CVAR_SERVERINFO | CVAR_ROM, 0, qfalse  },

	// latched vars
	{ &g_gametype, "g_gametype", "0", CVAR_SERVERINFO | CVAR_LATCH, 0, qfalse  },

	{ &g_maxclients, "sv_maxclients", "8", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },
	{ &g_maxGameClients, "g_maxGameClients", "0", CVAR_SERVERINFO | CVAR_LATCH | CVAR_ARCHIVE, 0, qfalse  },

	// change anytime vars
	{ &g_dmflags, "dmflags", "0", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },
	{ &g_fraglimit, "fraglimit", "20", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_timelimit, "timelimit", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_capturelimit, "capturelimit", "8", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },

	{ &g_synchronousClients, "g_synchronousClients", "0", CVAR_SYSTEMINFO, 0, qfalse  },

	{ &g_friendlyFire, "g_friendlyFire", "0", CVAR_ARCHIVE, 0, qtrue  },

	{ &g_teamAutoJoin, "g_teamAutoJoin", "0", CVAR_ARCHIVE  },
	{ &g_teamForceBalance, "g_teamForceBalance", "0", CVAR_ARCHIVE  },

	{ &g_warmup, "g_warmup", "20", CVAR_ARCHIVE, 0, qtrue  },
	{ &g_doWarmup, "g_doWarmup", "0", 0, 0, qtrue  },
	{ &g_log, "g_log", "games.log", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_logSync, "g_logSync", "0", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_password, "g_password", "", CVAR_USERINFO, 0, qfalse  },

	{ &g_banIPs, "g_banIPs", "", CVAR_ARCHIVE, 0, qfalse  },
	{ &g_filterBan, "g_filterBan", "1", CVAR_ARCHIVE, 0, qfalse  },

	{ &g_needpass, "g_needpass", "0", CVAR_SERVERINFO | CVAR_ROM, 0, qfalse },

	{ &g_dedicated, "dedicated", "0", 0, 0, qfalse  },

	{ &g_speed, "g_speed", "320", 0, 0, qtrue  },
	{ &g_gravity, "g_gravity", "800", 0, 0, qtrue  },
	{ &g_knockback, "g_knockback", "1000", 0, 0, qtrue  },
	{ &g_quadfactor, "g_quadfactor", "3", 0, 0, qtrue  },
	{ &g_weaponRespawn, "g_weaponrespawn", "5", 0, 0, qtrue  },
	{ &g_weaponTeamRespawn, "g_weaponTeamRespawn", "30", 0, 0, qtrue },
	{ &g_forcerespawn, "g_forcerespawn", "20", 0, 0, qtrue },
	{ &g_inactivity, "g_inactivity", "0", 0, 0, qtrue },
	{ &g_debugMove, "g_debugMove", "0", 0, 0, qfalse },
	{ &g_debugDamage, "g_debugDamage", "0", 0, 0, qfalse },
	{ &g_debugAlloc, "g_debugAlloc", "0", 0, 0, qfalse },
	{ &g_motd, "g_motd", "", 0, 0, qfalse },
	{ &g_blood, "com_blood", "1", 0, 0, qfalse },

	{ &g_podiumDist, "g_podiumDist", "80", 0, 0, qfalse },
	{ &g_podiumDrop, "g_podiumDrop", "70", 0, 0, qfalse },

	{ &g_allowVote, "g_allowVote", "1", CVAR_ARCHIVE, 0, qfalse },
	{ &g_listEntity, "g_listEntity", "0", 0, 0, qfalse },

#ifdef MISSIONPACK
	{ &g_obeliskHealth, "g_obeliskHealth", "2500", 0, 0, qfalse },
	{ &g_obeliskRegenPeriod, "g_obeliskRegenPeriod", "1", 0, 0, qfalse },
	{ &g_obeliskRegenAmount, "g_obeliskRegenAmount", "15", 0, 0, qfalse },
	{ &g_obeliskRespawnDelay, "g_obeliskRespawnDelay", "10", CVAR_SERVERINFO, 0, qfalse },

	{ &g_cubeTimeout, "g_cubeTimeout", "30", 0, 0, qfalse },
	{ &g_redteam, "g_redteam", "Stroggs", CVAR_ARCHIVE | CVAR_SERVERINFO | CVAR_USERINFO , 0, qtrue, qtrue },
	{ &g_blueteam, "g_blueteam", "Pagans", CVAR_ARCHIVE | CVAR_SERVERINFO | CVAR_USERINFO , 0, qtrue, qtrue  },
	{ &g_singlePlayer, "ui_singlePlayerActive", "", 0, 0, qfalse, qfalse  },

	{ &g_enableDust, "g_enableDust", "0", CVAR_SERVERINFO, 0, qtrue, qfalse },
	{ &g_enableBreath, "g_enableBreath", "0", CVAR_SERVERINFO, 0, qtrue, qfalse },
	{ &g_proxMineTimeout, "g_proxMineTimeout", "20000", 0, 0, qfalse },
#endif
	{ &g_smoothClients, "g_smoothClients", "1", 0, 0, qfalse},
	{ &pmove_fixed, "pmove_fixed", "0", CVAR_SYSTEMINFO, 0, qfalse},
	{ &pmove_msec, "pmove_msec", "8", CVAR_SYSTEMINFO, 0, qfalse},

	{ &g_rankings, "g_rankings", "0", 0, 0, qfalse}

};

int		gameCvarTableSize = sizeof( gameCvarTable ) / sizeof( gameCvarTable[0] );


void G_InitGame( int levelTime, int randomSeed, int restart );
void G_RunFrame( int levelTime );
void G_ShutdownGame( int restart );
void CheckExitRules( void );


/*
================
vmMain

this is the only way control passes into the module.
this must be the very first function compiled into the .q3vm file
================
*/
int vmMain( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11  ) {
	switch ( command ) {
	case GAME_INIT:
		G_InitGame( arg0, arg1, arg2 );
		return 0;
	case GAME_SHUTDOWN:
		G_ShutdownGame( arg0 );
		return 0;
	case GAME_CLIENT_CONNECT:
		return (int)ClientConnect( arg0, arg1, arg2 );
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
		return BotAIStartFrame( arg0 );
	}

	return -1;
}


void QDECL G_Printf( const char *fmt, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, fmt);
	vsprintf (text, fmt, argptr);
	va_end (argptr);

	trap_Printf( text );
}

void QDECL G_Error( const char *fmt, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, fmt);
	vsprintf (text, fmt, argptr);
	va_end (argptr);

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
void G_FindTeams( void ) {
	gentity_t	*e, *e2;
	int		i, j;
	int		c, c2;

	c = 0;
	c2 = 0;
	for ( i=1, e=g_entities+i ; i < level.num_entities ; i++,e++ ){
		if (!e->inuse)
			continue;
		if (!e->team)
			continue;
		if (e->flags & FL_TEAMSLAVE)
			continue;
		e->teammaster = e;
		c++;
		c2++;
		for (j=i+1, e2=e+1 ; j < level.num_entities ; j++,e2++)
		{
			if (!e2->inuse)
				continue;
			if (!e2->team)
				continue;
			if (e2->flags & FL_TEAMSLAVE)
				continue;
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

void G_RemapTeamShaders() {
#ifdef MISSIONPACK
	char string[1024];
	float f = level.time * 0.001;
	Com_sprintf( string, sizeof(string), "team_icon/%s_red", g_redteam.string );
	AddRemap("textures/ctf2/redteam01", string, f);
	AddRemap("textures/ctf2/redteam02", string, f);
	Com_sprintf( string, sizeof(string), "team_icon/%s_blue", g_blueteam.string );
	AddRemap("textures/ctf2/blueteam01", string, f);
	AddRemap("textures/ctf2/blueteam02", string, f);
	trap_SetConfigstring(CS_SHADERSTATE, BuildShaderStateConfig());
#endif
}


/*
=================
G_RegisterCvars
=================
*/
void G_RegisterCvars( void ) {
	int			i;
	cvarTable_t	*cv;
	qboolean remapped = qfalse;

	for ( i = 0, cv = gameCvarTable ; i < gameCvarTableSize ; i++, cv++ ) {
		trap_Cvar_Register( cv->vmCvar, cv->cvarName,
			cv->defaultString, cv->cvarFlags );
		if ( cv->vmCvar )
			cv->modificationCount = cv->vmCvar->modificationCount;

		if (cv->teamShader) {
			remapped = qtrue;
		}
	}

	if (remapped) {
		G_RemapTeamShaders();
	}

	// check some things
	if ( g_gametype.integer < 0 || g_gametype.integer >= GT_MAX_GAME_TYPE ) {
		G_Printf( "g_gametype %i is out of range, defaulting to 0\n", g_gametype.integer );
		trap_Cvar_Set( "g_gametype", "0" );
	}

	if ( g_headHunt.integer && !( g_gametype.integer == GT_FFA || g_gametype.integer == GT_TEAM ) ) {
		G_Printf( "^2Head Hunting is only available in FFA or Team Deathmatch^7\n" );
		if( g_gametype.integer > GT_TEAM ) trap_Cvar_Set( "g_gametype", "3" );
			else trap_Cvar_Set( "g_gametype", "0" );
	}

	level.warmupModificationCount = g_warmup.modificationCount;
}

/*
=================
G_UpdateCvars
=================
*/
void G_UpdateCvars( void ) {
	int			i;
	cvarTable_t	*cv;
	qboolean remapped = qfalse;

	for ( i = 0, cv = gameCvarTable ; i < gameCvarTableSize ; i++, cv++ ) {
		if ( cv->vmCvar ) {
			trap_Cvar_Update( cv->vmCvar );

			if ( cv->modificationCount != cv->vmCvar->modificationCount ) {
				cv->modificationCount = cv->vmCvar->modificationCount;

				if ( cv->trackChange ) {
					trap_SendServerCommand( -1, va("print \"Server: %s changed to %s\n\"",
						cv->cvarName, cv->vmCvar->string ) );
				}

				if (cv->teamShader) {
					remapped = qtrue;
				}
			}
		}
	}

	if (remapped) {
		G_RemapTeamShaders();
	}
}

/*
============
G_InitGame

============
*/
void G_InitGame( int levelTime, int randomSeed, int restart ) {
	int					i;

	G_Printf ("------- Game Initialization -------\n");
	G_Printf ("gamename: %s\n", GAMEVERSION);
	G_Printf ("gamedate: %s\n", __DATE__);

	srand( randomSeed );

	G_RegisterCvars();

	G_ProcessIPBans();

	G_InitMemory();

	// set some level globals
	memset( &level, 0, sizeof( level ) );
	level.time = levelTime;
	level.startTime = levelTime;

	level.snd_fry = G_SoundIndex("sound/player/fry.wav");	// FIXME standing in lava / slime

	if ( g_gametype.integer != GT_SINGLE_PLAYER && g_log.string[0] ) {
		if ( g_logSync.integer ) {
			trap_FS_FOpenFile( g_log.string, &level.logFile, FS_APPEND_SYNC );
		} else {
			trap_FS_FOpenFile( g_log.string, &level.logFile, FS_APPEND );
		}
		if ( !level.logFile ) {
			G_Printf( "WARNING: Couldn't open logfile: %s\n", g_log.string );
		} else {
			char	serverinfo[MAX_INFO_STRING];

			trap_GetServerinfo( serverinfo, sizeof( serverinfo ) );

			G_LogPrintf("------------------------------------------------------------\n" );
			G_LogPrintf("InitGame: %s\n", serverinfo );
		}
	} else {
		G_Printf( "Not logging to disk.\n" );
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
	for ( i=0 ; i<level.maxclients ; i++ ) {
		g_entities[i].client = level.clients + i;
	}

	// always leave room for the max number of clients,
	// even if they aren't all used, so numbers inside that
	// range are NEVER anything but clients
	level.num_entities = MAX_CLIENTS;

	// let the server system know where the entites are
	trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ),
		&level.clients[0].ps, sizeof( level.clients[0] ) );

	// reserve some spots for dead player bodies
	InitBodyQue();

	ClearRegisteredItems();

	// parse the key/value pairs and spawn gentities
	G_SpawnEntitiesFromString();

	// general initialization
	G_FindTeams();

	// make sure we have flags for CTF, etc
	if( g_gametype.integer >= GT_TEAM ) {
		G_CheckTeamItems();
	}

	SaveRegisteredItems();

	G_Printf ("-----------------------------------\n");

	if( g_gametype.integer == GT_SINGLE_PLAYER || trap_Cvar_VariableIntegerValue( "com_buildScript" ) ) {
		G_ModelIndex( SP_PODIUM_MODEL );
		G_SoundIndex( "sound/player/gurp1.wav" );
		G_SoundIndex( "sound/player/gurp2.wav" );
	}

	if ( trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
		BotAISetup( restart );
		BotAILoadMap( restart );
		G_InitBots( restart );
	}

	G_RemapTeamShaders();

}



/*
=================
G_ShutdownGame
=================
*/
void G_ShutdownGame( int restart ) {
	G_Printf ("==== ShutdownGame ====\n");

	if ( level.logFile ) {
		G_LogPrintf("ShutdownGame:\n" );
		G_LogPrintf("------------------------------------------------------------\n" );
		trap_FS_FCloseFile( level.logFile );
	}

	// write all the client session data so we can get it back
	G_WriteSessionData();

	if ( trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
		BotAIShutdown( restart );
	}
}



//===================================================================

#ifndef GAME_HARD_LINKED
// this is only here so the functions in q_shared.c and bg_*.c can link

void QDECL Com_Error ( int level, const char *error, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, error);
	vsprintf (text, error, argptr);
	va_end (argptr);

	G_Error( "%s", text);
}

void QDECL Com_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	vsprintf (text, msg, argptr);
	va_end (argptr);

	G_Printf ("%s", text);
}

#endif

/*
========================================================================

PLAYER COUNTING / SCORE SORTING

========================================================================
*/

/*
=============
AddTournamentPlayer

if there are less than two tournament players, put a
spectator in the game and restart
=============
*/
void AddTournamentPlayer( void ) {
	int			i;
	gclient_t	*client;
	gclient_t	*nextInLine;

	if ( level.numPlayingClients >= 2 ) {
		return;
	}

	// never change during intermission
	if ( level.intermissiontime ) {
		return;
	}

	nextInLine = NULL;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		client = &level.clients[i];
		if ( client->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
			continue;
		}
		// never select the dedicated follow or scoreboard clients
		if ( client->sess.spectatorState == SPECTATOR_SCOREBOARD ||
			client->sess.spectatorClient < 0  ) {
			continue;
		}

		if ( !nextInLine || client->sess.spectatorTime < nextInLine->sess.spectatorTime ) {
			nextInLine = client;
		}
	}

	if ( !nextInLine ) {
		return;
	}

	level.warmupTime = -1;

	// set them to free-for-all team
	SetTeam( &g_entities[ nextInLine - level.clients ], "f" );
}

/*
=======================
RemoveTournamentLoser

Make the loser a spectator at the back of the line
=======================
*/
void RemoveTournamentLoser( void ) {
	int			clientNum;

	if ( level.numPlayingClients != 2 ) {
		return;
	}

	clientNum = level.sortedClients[1];

	if ( level.clients[ clientNum ].pers.connected != CON_CONNECTED ) {
		return;
	}

	// make them a spectator
	SetTeam( &g_entities[ clientNum ], "s" );
}

/*
=======================
RemoveTournamentWinner
=======================
*/
void RemoveTournamentWinner( void ) {
	int			clientNum;

	if ( level.numPlayingClients != 2 ) {
		return;
	}

	clientNum = level.sortedClients[0];

	if ( level.clients[ clientNum ].pers.connected != CON_CONNECTED ) {
		return;
	}

	// make them a spectator
	SetTeam( &g_entities[ clientNum ], "s" );
}

/*
=======================
AdjustTournamentScores
=======================
*/
void AdjustTournamentScores( void ) {
	int			clientNum;

	clientNum = level.sortedClients[0];
	if ( level.clients[ clientNum ].pers.connected == CON_CONNECTED ) {
		level.clients[ clientNum ].sess.wins++;
		ClientUserinfoChanged( clientNum );
	}

	clientNum = level.sortedClients[1];
	if ( level.clients[ clientNum ].pers.connected == CON_CONNECTED ) {
		level.clients[ clientNum ].sess.losses++;
		ClientUserinfoChanged( clientNum );
	}

}

/*
=============
SortRanks

=============
*/
int QDECL SortRanks( const void *a, const void *b ) {
	gclient_t	*ca, *cb;

	ca = &level.clients[*(int *)a];
	cb = &level.clients[*(int *)b];

	// sort special clients last
	if ( ca->sess.spectatorState == SPECTATOR_SCOREBOARD || ca->sess.spectatorClient < 0 ) {
		return 1;
	}
	if ( cb->sess.spectatorState == SPECTATOR_SCOREBOARD || cb->sess.spectatorClient < 0  ) {
		return -1;
	}

	// then connecting clients
	if ( ca->pers.connected == CON_CONNECTING ) {
		return 1;
	}
	if ( cb->pers.connected == CON_CONNECTING ) {
		return -1;
	}


	// then spectators
	if ( ca->sess.sessionTeam == TEAM_SPECTATOR && cb->sess.sessionTeam == TEAM_SPECTATOR ) {
		if ( ca->sess.spectatorTime < cb->sess.spectatorTime ) {
			return -1;
		}
		if ( ca->sess.spectatorTime > cb->sess.spectatorTime ) {
			return 1;
		}
		return 0;
	}
	if ( ca->sess.sessionTeam == TEAM_SPECTATOR ) {
		return 1;
	}
	if ( cb->sess.sessionTeam == TEAM_SPECTATOR ) {
		return -1;
	}

	// then sort by score
	if ( ca->ps.persistant[PERS_SCORE]
		> cb->ps.persistant[PERS_SCORE] ) {
		return -1;
	}
	if ( ca->ps.persistant[PERS_SCORE]
		< cb->ps.persistant[PERS_SCORE] ) {
		return 1;
	}
	return 0;
}

/*
============
CalculateRanks

Recalculates the score ranks of all players
this will be called on every client connect, begin, disconnect, death,
and team change.
============
*/
void CalculateRanks( void ) {
	int		i;
	int		rank;
	int		score;
	int		newScore;
	gclient_t	*cl;

	level.follow1 = -1;
	level.follow2 = -1;
	level.numConnectedClients = 0;
	level.numNonSpectatorClients = 0;
	level.numPlayingClients = 0;
	level.numVotingClients = 0;		// don't count bots
	for ( i = 0; i < TEAM_NUM_TEAMS; i++ ) {
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
					if ( !(g_entities[i].r.svFlags & SVF_BOT) ) {
						level.numVotingClients++;
						if ( level.clients[i].sess.sessionTeam == TEAM_RED )
							level.numteamVotingClients[0]++;
						else if ( level.clients[i].sess.sessionTeam == TEAM_BLUE )
							level.numteamVotingClients[1]++;
					}
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
	if ( g_gametype.integer >= GT_TEAM ) {
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
		score = 0;
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
			if ( g_gametype.integer == GT_SINGLE_PLAYER && level.numPlayingClients == 1 ) {
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
			}
		}
	}

	// set the CS_SCORES1/2 configstrings, which will be visible to everyone
	if ( g_gametype.integer >= GT_TEAM ) {
		trap_SetConfigstring( CS_SCORES1, va("%i", level.teamScores[TEAM_RED] ) );
		trap_SetConfigstring( CS_SCORES2, va("%i", level.teamScores[TEAM_BLUE] ) );
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
	CheckExitRules();

	// if we are at the intermission, send the new info to everyone
	if ( level.intermissiontime ) {
		SendScoreboardMessageToAllClients();
	}
}


/*
========================================================================

MAP CHANGING

========================================================================
*/

/*
========================
SendChallenge

The client must draw the accuracy challenge info, and take a screenshot
========================
*/

void SendChallenge( void ) {
	int		i;

	SendScoreboardMessageToAllClients();

	// only send to client 0, since this is always a local game
	trap_SendServerCommand( 0, "drawchallengeinfo" );
}


/*
================
SendGameStatsXXX

CORKSCREW: send info 'bout awards
================
*/
static const char *MonthAbbrev[] = {
	"Jan","Feb","Mar",
	"Apr","May","Jun",
	"Jul","Aug","Sep",
	"Oct","Nov","Dec"
};

void SendGameStatsDM( void ) {
	char			string[4096], string2[1024], filename[MAX_QPATH] = "stats/", serverinfo[MAX_INFO_STRING], userinfo[MAX_INFO_STRING];
	char			*date;
	fileHandle_t	f;
	gclient_t		*client;
	int				i, ping, gametime, mins, tens, secs, spectator, accuracy, players;
	qtime_t			q;

	gametime = level.time - level.startTime;
	mins = gametime / 60000;
	tens = (gametime / 60000.0 - mins) * 60;
	secs = tens%10;
	tens /= 10;

	trap_RealTime( &q );
	date = va("%s-%i, %i at %i:%i%i", MonthAbbrev[q.tm_mon],q.tm_mday, 1900+q.tm_year,q.tm_hour, (q.tm_min/10), (q.tm_min%10) );

	if( level.accuracyChallenge ) {
		strcpy( string2, "Accuracy Challenge" );
	} else if( g_headHunt.integer ) {
		strcpy( string2, "Head Hunting" );
	} else {
		strcpy( string2, "Deathmatch" );
	}

	if( g_sendStats.integer ) {

		Com_sprintf( string, sizeof(string), "\n%s %s statistics\nDate: %s, game time: %i:%i%i\n\n"
		S_COLOR_YELLOW "rnk   fra  dea   time  lag  hea  exc  imp  hit  sho  acc | player\n" S_COLOR_WHITE ""
		, GAMEVERSION, string2, date, mins, tens, secs );

		for( i = 0; i < level.numConnectedClients; i++ ) {
			client = &level.clients[level.sortedClients[i]];
			if( !client ) continue;
			if ( client->sess.sessionTeam == TEAM_SPECTATOR ) continue;
			if ( client->pers.connected == CON_CONNECTING ) continue;

			players++;
			if( players > 5 ) {
				players = 1;

				Com_sprintf( string, sizeof(string), "\nprint \"%s\"\n", string );
				trap_SendServerCommand( -1, string );
				Com_sprintf( string, sizeof(string), "" );
			}

			ping = client->ps.ping < 999 ? client->ps.ping : 999;

			gametime = level.time - client->pers.enterTime;
			mins = gametime / 60000;
			tens = (gametime / 60000.0 - mins) * 60;
			secs = tens%10;
			tens /= 10;

			if( client->ps.persistant[PERS_ACCURACY_SHOTS] == 0 ) {
				accuracy = 0;
			} else {
				accuracy = client->ps.persistant[PERS_ACCURACY_HITS] * 100 / client->ps.persistant[PERS_ACCURACY_SHOTS];
			}

			Com_sprintf( string, sizeof(string), "%s"

				" %2i" S_COLOR_YELLOW "  %4i" S_COLOR_WHITE " %4i %3i:%i%i %4i %4i %4i %4i %4i %4i  %3i " S_COLOR_YELLOW "|" S_COLOR_WHITE " %s" S_COLOR_WHITE "",
				string,
				i+1,
				client->ps.persistant[PERS_SCORE],
				client->ps.persistant[PERS_KILLED],
				mins, tens, secs,
				ping,
				client->ps.persistant[PERS_DECAPITATIONS],
				client->ps.persistant[PERS_EXCELLENT_COUNT],
				client->ps.persistant[PERS_IMPRESSIVE_COUNT],
				client->ps.persistant[PERS_ACCURACY_HITS],
				client->ps.persistant[PERS_ACCURACY_SHOTS],
				accuracy,
				client->pers.netname );

			if ( g_entities[client->ps.clientNum].r.svFlags & SVF_BOT ) {
				trap_GetUserinfo( client->ps.clientNum, userinfo, sizeof( userinfo ) );
				Com_sprintf( string, sizeof(string), "%s (bot %i)", string, atoi( Info_ValueForKey( userinfo, "skill" ) ) );
			}
			Com_sprintf( string, sizeof(string), "%s\n", string );
		}

		Com_sprintf( string, sizeof(string), "\nprint \"%s\n\n\"\n", string );

		// send to all the clients, with color codes
		trap_SendServerCommand( -1, string );
	}

	// write to stats/mapname.txt
	// dang... doing it all over again :(
	date = va("%s-%i, %i at %i:%i%i", MonthAbbrev[q.tm_mon],q.tm_mday, 1900+q.tm_year,q.tm_hour, (q.tm_min/10), (q.tm_min%10) );

	mins = gametime / 60000;
	tens = (gametime / 60000.0 - mins) * 60;
	secs = tens%10;
	tens /= 10;

	Com_sprintf( string, sizeof(string), "\n*********************************************\n%s %s statistics\nDate: %s, game time: %i:%i%i\n*********************************************\n"
	"rnk   fra  dea   time  lag  hea  exc  imp  hit  sho  acc | player\n"
	, GAMEVERSION, string2, date, mins, tens, secs );

	for( i = 0; i < level.numConnectedClients; i++ ) {
		client = &level.clients[level.sortedClients[i]];
		if( !client ) continue;
		if ( client->sess.sessionTeam == TEAM_SPECTATOR ) continue;
		if ( client->pers.connected == CON_CONNECTING ) continue;

		ping = client->ps.ping < 999 ? client->ps.ping : 999;
		Q_strncpyz( string2, client->pers.netname, sizeof(string2) );
		Q_CleanStr( string2 );

		gametime = level.time - client->pers.enterTime;
		mins = gametime / 60000;
		tens = (gametime / 60000.0 - mins) * 60;
		secs = tens%10;
		tens /= 10;

// lesson 2 in how to avoid a FRIGGIN DIVIDE BY ZERO!
		if( client->ps.persistant[PERS_ACCURACY_SHOTS] == 0 ) {
			accuracy = 0;
		} else {
			accuracy = client->ps.persistant[PERS_ACCURACY_HITS] * 100 / client->ps.persistant[PERS_ACCURACY_SHOTS];
		}

		Com_sprintf( string, sizeof(string), "%s"
//			"rnk   fra   dea   lag   hea   exc   imp   hit   sho   acc | name\n"
			" %2i  %4i %4i %3i:%i%i %4i %4i %4i %4i %4i %4i %3i%s | %s",
			string,
			i+1,
			client->ps.persistant[PERS_SCORE],
			client->ps.persistant[PERS_KILLED],
			mins, tens, secs,
			ping,
			client->ps.persistant[PERS_DECAPITATIONS],
			client->ps.persistant[PERS_EXCELLENT_COUNT],
			client->ps.persistant[PERS_IMPRESSIVE_COUNT],
			client->ps.persistant[PERS_ACCURACY_HITS],
			client->ps.persistant[PERS_ACCURACY_SHOTS],
			accuracy,
			"%",
			string2 );

		if ( g_entities[client->ps.clientNum].r.svFlags & SVF_BOT ) {
			trap_GetUserinfo( client->ps.clientNum, userinfo, sizeof( userinfo ) );
			Com_sprintf( string, sizeof(string), "%s (bot %i)", string, atoi( Info_ValueForKey( userinfo, "skill" ) ) );
		}
		Com_sprintf( string, sizeof(string), "%s\n", string );
	}

	spectator = 0;
	for( i = 0; i < level.numConnectedClients; i++ ) {
		client = &level.clients[level.sortedClients[i]];
		if ( client->sess.sessionTeam != TEAM_SPECTATOR ) continue;
		if ( client->pers.connected == CON_CONNECTING ) continue;

		Q_strncpyz( string2, client->pers.netname, sizeof(string2) );
		Q_CleanStr( string2 );

		if ( !spectator ) {
			Com_sprintf( string, sizeof(string), "%s\nSpectators: %s", string, string2 );
			spectator = 1;
		} else {
			Com_sprintf( string, sizeof(string), "%s, %s", string, string2 );
		}
	}

	if ( spectator ) {
		Com_sprintf( string, sizeof(string), "%s\n\n", string );
	} else {
		Com_sprintf( string, sizeof(string), "%s\n", string );
	}

	trap_GetServerinfo( serverinfo, sizeof(serverinfo) );
	Q_strncpyz( string2, Info_ValueForKey( serverinfo, "mapname" ), sizeof(string2) );

	strcat(filename, string2);
	strcat(filename, ".txt");
	Q_strlwr( filename );

	trap_FS_FOpenFile( filename, &f, FS_APPEND );
	trap_FS_Write( string, strlen( string ), f );
	trap_FS_FCloseFile( f );
}

/*****/

void SendGameStatsTEAM( void ) {
	char			string[4096], string2[1024], filename[MAX_QPATH] = "stats/", serverinfo[MAX_INFO_STRING], userinfo[MAX_INFO_STRING];
	char			*date;
	fileHandle_t	f;
	gclient_t		*client;
	int				i, ping, gametime, mins, tens, secs, team, spectator, accuracy, players;
	qtime_t			q;

	gametime = level.time - level.startTime;
	mins = gametime / 60000;
	tens = (gametime / 60000.0 - mins) * 60;
	secs = tens%10;
	tens /= 10;

	trap_RealTime( &q );
	date = va("%s-%i, %i at %i:%i%i", MonthAbbrev[q.tm_mon],q.tm_mday, 1900+q.tm_year,q.tm_hour, (q.tm_min/10), (q.tm_min%10) );

	if ( g_gametype.integer == GT_CTF ) {
		strcpy( string2, "Capture The Flag" );
	} else if( g_headHunt.integer ) {
		strcpy( string2, "Team Head Hunting" );
	} else {
		strcpy( string2, "Team Deathmatch" );
	}

	if( g_sendStats.integer ) {

		Com_sprintf( string, sizeof(string), "\n%s %s statistics\nDate: %s, game time: %i:%i%i\n", GAMEVERSION, string2, date, mins, tens, secs );

		for ( team = TEAM_RED; team < TEAM_SPECTATOR; team++ ) {
			if ( g_gametype.integer == GT_CTF ) {
				if ( team == TEAM_RED ) {
					Com_sprintf( string, sizeof(string), "%s\nRed team: %i captures\n", string, level.teamScores[team] );
				} else {
					Com_sprintf( string, sizeof(string), "%s\nBlue team: %i captures\n", string, level.teamScores[team] );
				}
			} else {
				if ( team == TEAM_RED ) {
					Com_sprintf( string, sizeof(string), "%s\nRed team: %i frags\n", string, level.teamScores[team] );
				} else {
					Com_sprintf( string, sizeof(string), "%s\nBlue team: %i frags\n", string, level.teamScores[team] );
				}
			}

			if ( g_gametype.integer == GT_CTF ) {
				Com_sprintf( string, sizeof(string), "%s" S_COLOR_YELLOW "points  dea   time  lag  cap  def  ass  hit  sho  acc | player\n" S_COLOR_WHITE, string );
			} else {
				Com_sprintf( string, sizeof(string), "%s" S_COLOR_YELLOW "  fra  dea   time  lag  hea  exc  imp  hit  sho  acc | player\n" S_COLOR_WHITE, string );
			}

			for( i = 0; i < level.numConnectedClients; i++ ) {
				client = &level.clients[level.sortedClients[i]];
				if( !client ) continue;
				if( client->sess.sessionTeam != team ) continue;
				if( client->pers.connected != CON_CONNECTED ) continue;

				players++;
				if( players > 5 ) {
					players = 1;

					Com_sprintf( string, sizeof(string), "\nprint \"%s\"\n", string );
					trap_SendServerCommand( -1, string );
					Com_sprintf( string, sizeof(string), "" );
				}

				ping = client->ps.ping < 999 ? client->ps.ping : 999;

				gametime = level.time - client->pers.enterTime;
				mins = gametime / 60000;
				tens = (gametime / 60000.0 - mins) * 60;
				secs = tens%10;
				tens /= 10;

				if( client->ps.persistant[PERS_ACCURACY_SHOTS] == 0 ) {
					accuracy = 0;
				} else {
					accuracy = client->ps.persistant[PERS_ACCURACY_HITS] * 100 / client->ps.persistant[PERS_ACCURACY_SHOTS];
				}

				if ( g_gametype.integer == GT_CTF ) {
					Com_sprintf( string, sizeof(string), "%s"
		//			"points   kll   lag   cap   def   ass   hit   sho   acc | name\n"
						S_COLOR_YELLOW "  %4i" S_COLOR_WHITE " %4i %3i:%i%i %4i %4i %4i %4i %4i %4i  %3i " S_COLOR_YELLOW "|" S_COLOR_WHITE " %s" S_COLOR_WHITE "",
						string,
						client->ps.persistant[PERS_SCORE],
						client->ps.persistant[PERS_KILLED],
						mins, tens, secs,
						ping,
						client->ps.persistant[PERS_CAPTURES],
						client->ps.persistant[PERS_DEFEND_COUNT],
						client->ps.persistant[PERS_ASSIST_COUNT],
						client->ps.persistant[PERS_ACCURACY_HITS],
						client->ps.persistant[PERS_ACCURACY_SHOTS],
						accuracy,
						client->pers.netname );
				} else {
					Com_sprintf( string, sizeof(string), "%s"
			//			" fra   kll   lag   hea   exc   imp   hit   sho   acc | name\n"
						" %4i" S_COLOR_WHITE " %4i %3i:%i%i %4i %4i %4i %4i %4i %4i  %3i " S_COLOR_YELLOW "|" S_COLOR_WHITE " %s" S_COLOR_WHITE "",
						string,
						client->ps.persistant[PERS_SCORE],
						client->ps.persistant[PERS_KILLED],
						mins, tens, secs,
						ping,
						client->ps.persistant[PERS_DECAPITATIONS],
						client->ps.persistant[PERS_EXCELLENT_COUNT],
						client->ps.persistant[PERS_IMPRESSIVE_COUNT],
						client->ps.persistant[PERS_ACCURACY_HITS],
						client->ps.persistant[PERS_ACCURACY_SHOTS],
						accuracy,
						client->pers.netname );
				}

				if ( g_entities[client->ps.clientNum].r.svFlags & SVF_BOT ) {
					trap_GetUserinfo( client->ps.clientNum, userinfo, sizeof( userinfo ) );
					Com_sprintf( string, sizeof(string), "%s (bot %i)", string, atoi( Info_ValueForKey( userinfo, "skill" ) ) );
				}
				Com_sprintf( string, sizeof(string), "%s\n", string );
			}
		}

		Com_sprintf( string, sizeof(string), "\nprint \"%s\n\n\"\n", string );

		// send to all the clients, with color codes
		trap_SendServerCommand( -1, string );
	}

	// cleanup color codes and write to stats/mapname.txt
	// dang... doing it all over again :(
	date = va("%s-%i, %i at %i:%i%i", MonthAbbrev[q.tm_mon],q.tm_mday, 1900+q.tm_year,q.tm_hour, (q.tm_min/10), (q.tm_min%10) );

	gametime = level.time - level.startTime;
	mins = gametime / 60000;
	tens = (gametime / 60000.0 - mins) * 60;
	secs = tens%10;
	tens /= 10;

	Com_sprintf( string, sizeof(string), "\n*********************************************\n%s %s statistics\nDate: %s, game time: %i:%i%i\n*********************************************\n", GAMEVERSION, string2, date, mins, tens, secs );

	for ( team = TEAM_RED; team < TEAM_SPECTATOR; team++ ) {
		if ( g_gametype.integer == GT_CTF ) {
			if ( team == TEAM_RED ) {
				Com_sprintf( string, sizeof(string), "%s\nRed team: %i captures\n", string, level.teamScores[team] );
			} else {
				Com_sprintf( string, sizeof(string), "%s\nBlue team: %i captures\n", string, level.teamScores[team] );
			}
		} else {
			if ( team == TEAM_RED ) {
				Com_sprintf( string, sizeof(string), "%s\nRed team: %i frags\n", string, level.teamScores[team] );
			} else {
				Com_sprintf( string, sizeof(string), "%s\nBlue team: %i frags\n", string, level.teamScores[team] );
			}
		}

		if ( g_gametype.integer == GT_CTF ) {
			Com_sprintf( string, sizeof(string), "%spoints  dea   time  lag  cap  def  ass  hit  sho   acc | player\n", string );
		} else {
			Com_sprintf( string, sizeof(string), "%s  fra  dea   time  lag  hea  exc  imp  hit  sho   acc | player\n", string );
		}

		for( i = 0; i < level.numConnectedClients; i++ ) {
			client = &level.clients[level.sortedClients[i]];
			if( !client ) continue;
			if ( client->sess.sessionTeam != team ) continue;
			if ( client->pers.connected == CON_CONNECTING ) continue;

			ping = client->ps.ping < 999 ? client->ps.ping : 999;
			Q_strncpyz( string2, client->pers.netname, sizeof(string2) );
			Q_CleanStr( string2 );

			gametime = level.time - client->pers.enterTime;
			mins = gametime / 60000;
			tens = (gametime / 60000.0 - mins) * 60;
			secs = tens%10;
			tens /= 10;

// lesson 4 in how to avoid a FRIGGIN DIVIDE BY ZERO!
			if( client->ps.persistant[PERS_ACCURACY_SHOTS] == 0 ) {
				accuracy = 0;
			} else {
				accuracy = client->ps.persistant[PERS_ACCURACY_HITS] * 100 / client->ps.persistant[PERS_ACCURACY_SHOTS];
			}

			if ( g_gametype.integer == GT_CTF ) {
				Com_sprintf( string, sizeof(string), "%s"
	//			"points   kll   lag   cap   def   ass   hit   sho   acc | name\n"
					"  %4i %4i %3i:%i%i %4i %4i %4i %4i %4i %4i  %3i%s | %s",
					string,
					client->ps.persistant[PERS_SCORE],
					client->ps.persistant[PERS_KILLED],
					mins, tens, secs,
					ping,
					client->ps.persistant[PERS_CAPTURES],
					client->ps.persistant[PERS_DEFEND_COUNT],
					client->ps.persistant[PERS_ASSIST_COUNT],
					client->ps.persistant[PERS_ACCURACY_HITS],
					client->ps.persistant[PERS_ACCURACY_SHOTS],
					accuracy,
					"%",
					string2 );
			} else {
				Com_sprintf( string, sizeof(string), "%s"
		//			" fra   kll   lag   hea   exc   imp   hit   sho   acc | name\n"
					" %4i %4i %3i:%i%i %4i %4i %4i %4i %4i %4i  %3i%s | %s",
					string,
					client->ps.persistant[PERS_SCORE],
					client->ps.persistant[PERS_KILLED],
					mins, tens, secs,
					ping,
					client->ps.persistant[PERS_DECAPITATIONS],
					client->ps.persistant[PERS_EXCELLENT_COUNT],
					client->ps.persistant[PERS_IMPRESSIVE_COUNT],
					client->ps.persistant[PERS_ACCURACY_HITS],
					client->ps.persistant[PERS_ACCURACY_SHOTS],
					accuracy,
					"%",
					string2 );
			}

			if ( g_entities[client->ps.clientNum].r.svFlags & SVF_BOT ) {
				trap_GetUserinfo( client->ps.clientNum, userinfo, sizeof( userinfo ) );
				Com_sprintf( string, sizeof(string), "%s (bot %i)", string, atoi( Info_ValueForKey( userinfo, "skill" ) ) );
			}
			Com_sprintf( string, sizeof(string), "%s\n", string );
		}
	}

	spectator = 0;
	for( i = 0; i < level.numConnectedClients; i++ ) {
		client = &level.clients[level.sortedClients[i]];
		if ( client->sess.sessionTeam != TEAM_SPECTATOR ) continue;
		if ( client->pers.connected == CON_CONNECTING ) continue;

		Q_strncpyz( string2, client->pers.netname, sizeof(string2) );
		Q_CleanStr( string2 );

		if ( !spectator ) {
			Com_sprintf( string, sizeof(string), "%s\nSpectators: %s", string, string2 );
			spectator = 1;
		} else {
			Com_sprintf( string, sizeof(string), "%s, %s", string, string2 );
		}
	}

	if ( spectator ) {
		Com_sprintf( string, sizeof(string), "%s\n\n", string );
	} else {
		Com_sprintf( string, sizeof(string), "%s\n", string );
	}

	trap_GetServerinfo( serverinfo, sizeof(serverinfo) );
	Q_strncpyz( string2, Info_ValueForKey( serverinfo, "mapname" ), sizeof(string2) );

	strcat(filename, string2);
	strcat(filename, ".txt");
	Q_strlwr( filename );

	trap_FS_FOpenFile( filename, &f, FS_APPEND );
	trap_FS_Write( string, strlen( string ), f );
	trap_FS_FCloseFile( f );

}

/*
========================
SendScoreboardMessageToAllClients

do this at BeginIntermission time and whenever ranks are recalculated
due to enters/exits/forced team changes
========================
*/
void SendScoreboardMessageToAllClients( void ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) {
			DeathmatchScoreboardMessage( g_entities + i );
		}
	}
}

/*
========================
MoveClientToIntermission

When the intermission starts, this will be called for all players.
if a new client connects, this will be called after the spawn function.
========================
*/
void MoveClientToIntermission( gentity_t *ent ) {
	// take out of follow mode if needed
	if ( ent->client->sess.spectatorState == SPECTATOR_FOLLOW ) {
		StopFollowing( ent );
	}


	// move to the spot
	VectorCopy( level.intermission_origin, ent->s.origin );
	VectorCopy( level.intermission_origin, ent->client->ps.origin );
	VectorCopy (level.intermission_angle, ent->client->ps.viewangles);
	ent->client->ps.pm_type = PM_INTERMISSION;

	// clean up powerup info
	memset( ent->client->ps.powerups, 0, sizeof(ent->client->ps.powerups) );

	ent->client->ps.eFlags = 0;
	ent->s.eFlags = 0;
	ent->s.eType = ET_GENERAL;
	ent->s.modelindex = 0;
	ent->s.loopSound = 0;
	ent->s.event = 0;
	ent->r.contents = 0;
}

/*
==================
FindIntermissionPoint

this is also used for spectator spawns
==================
*/
void FindIntermissionPoint( void ) {
	gentity_t	*ent, *target;
	vec3_t		dir;

	// find the intermission spot
	ent = G_Find (NULL, FOFS(classname), "info_player_intermission");
	if ( !ent ) {	// the map creator forgot to put in an intermission point...
		SelectSpawnPoint ( vec3_origin, level.intermission_origin, level.intermission_angle );
	} else {
		VectorCopy (ent->s.origin, level.intermission_origin);
		VectorCopy (ent->s.angles, level.intermission_angle);
		// if it has a target, look towards it
		if ( ent->target ) {
			target = G_PickTarget( ent->target );
			if ( target ) {
				VectorSubtract( target->s.origin, level.intermission_origin, dir );
				vectoangles( dir, level.intermission_angle );
			}
		}
	}

}

/*
==================
BeginIntermission
==================
*/
void BeginIntermission( void ) {
	int			i;
	gentity_t	*client;

	if ( level.intermissiontime ) {
		return;		// already active
	}

	// CORKSCREW send stats to all clients
	if( g_gametype.integer >= GT_TEAM ) {
		SendGameStatsTEAM();
	} else if ( g_gametype.integer == GT_FFA ) {
		SendGameStatsDM();
	}

	// if in tournement mode, change the wins / losses
	if ( g_gametype.integer == GT_TOURNAMENT ) {
		AdjustTournamentScores();
	}

	level.intermissiontime = level.time;
	FindIntermissionPoint();

#ifdef MISSIONPACK
	if (g_singlePlayer.integer) {
		trap_Cvar_Set("ui_singlePlayerActive", "0");
		UpdateTournamentInfo();
	}
#else
	// if single player game
	if ( g_gametype.integer == GT_SINGLE_PLAYER ) {
		UpdateTournamentInfo();
		SpawnModelsOnVictoryPads();
	}
#endif

	// move all clients to the intermission point
	for (i=0 ; i< level.maxclients ; i++) {
		client = g_entities + i;
		if (!client->inuse)
			continue;
		// respawn if dead
		if (client->health <= 0) {
			respawn(client);
		}
		MoveClientToIntermission( client );
	}

	// send the current scoring to all clients
	SendScoreboardMessageToAllClients();
}


/*
=============
ExitLevel

When the intermission has been exited, the server is either killed
or moved to a new level based on the "nextmap" cvar

=============
*/
void ExitLevel (void) {
	int		i;
	gclient_t *cl;

	//bot interbreeding
	BotInterbreedEndMatch();

	// if we are running a tournement map, kick the loser to spectator status,
	// which will automatically grab the next spectator and restart
	if ( g_gametype.integer == GT_TOURNAMENT  ) {
		if ( !level.restarted ) {
			RemoveTournamentLoser();
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			level.changemap = NULL;
			level.intermissiontime = 0;
		}
		return;
	}

	if( level.accuracyChallenge ) {
		trap_Cvar_Set( "ui_accuracychallengestartup", "1" );
		trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
	} else if ( trap_Cvar_VariableValue( "ui_randomskirmish" ) ) trap_SendConsoleCommand( EXEC_APPEND, "randomskirmish\n" );
				else trap_SendConsoleCommand( EXEC_APPEND, "vstr nextmap\n" );

	level.changemap = NULL;
	level.intermissiontime = 0;

	// reset all the scores so we don't enter the intermission again
	level.teamScores[TEAM_RED] = 0;
	level.teamScores[TEAM_BLUE] = 0;
	for ( i=0 ; i< g_maxclients.integer ; i++ ) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		cl->ps.persistant[PERS_SCORE] = 0;
	}

	// we need to do this here before chaning to CON_CONNECTING
	G_WriteSessionData();

	// change all client states to connecting, so the early players into the
	// next level will know the others aren't done reconnecting
	for (i=0 ; i< g_maxclients.integer ; i++) {
		if ( level.clients[i].pers.connected == CON_CONNECTED ) {
			level.clients[i].pers.connected = CON_CONNECTING;
		}
	}

}

/*
=================
G_LogPrintf

Print to the logfile with a time stamp if it is open
=================
*/
void QDECL G_LogPrintf( const char *fmt, ... ) {
	va_list		argptr;
	char		string[1024];
	int			min, tens, sec;

	sec = level.time / 1000;

	min = sec / 60;
	sec -= min * 60;
	tens = sec / 10;
	sec -= tens * 10;

	Com_sprintf( string, sizeof(string), "%3i:%i%i ", min, tens, sec );

	va_start( argptr, fmt );
	vsprintf( string +7 , fmt,argptr );
	va_end( argptr );

	if ( g_dedicated.integer ) {
		G_Printf( "%s", string + 7 );
	}

	if ( !level.logFile ) {
		return;
	}

	trap_FS_Write( string, strlen( string ), level.logFile );
}

/*
================
LogExit

Append information about this game to the log file
================
*/
void LogExit( const char *string ) {
	int				i, numSorted;
	gclient_t		*cl;
	qboolean won = qtrue;

	G_LogPrintf( "Exit: %s\n", string );

	level.intermissionQueued = level.time;

	// this will keep the clients from playing any voice sounds
	// that will get cut off when the queued intermission starts
	trap_SetConfigstring( CS_INTERMISSION, "1" );

	// don't send more than 32 scores (FIXME?)
	numSorted = level.numConnectedClients;
	if ( numSorted > 32 ) {
		numSorted = 32;
	}

	if ( g_gametype.integer >= GT_TEAM ) {
		G_LogPrintf( "red:%i  blue:%i\n",
			level.teamScores[TEAM_RED], level.teamScores[TEAM_BLUE] );
	}

	for (i=0 ; i < numSorted ; i++) {
		int		ping;

		cl = &level.clients[level.sortedClients[i]];

		if ( cl->sess.sessionTeam == TEAM_SPECTATOR ) {
			continue;
		}
		if ( cl->pers.connected == CON_CONNECTING ) {
			continue;
		}

		ping = cl->ps.ping < 999 ? cl->ps.ping : 999;

		G_LogPrintf( "score: %i  ping: %i  client: %i %s\n", cl->ps.persistant[PERS_SCORE], ping, level.sortedClients[i],	cl->pers.netname );
#ifdef MISSIONPACK
		if (g_singlePlayer.integer && g_gametype.integer == GT_TOURNAMENT) {
			if (g_entities[cl - level.clients].r.svFlags & SVF_BOT && cl->ps.persistant[PERS_RANK] == 0) {
				won = qfalse;
			}
		}
#endif

	}

#ifdef MISSIONPACK
	if (g_singlePlayer.integer) {
		if (g_gametype.integer >= GT_CTF) {
			won = level.teamScores[TEAM_RED] > level.teamScores[TEAM_BLUE];
		}
		trap_SendConsoleCommand( EXEC_APPEND, (won) ? "spWin\n" : "spLose\n" );
	}
#endif
}


/*
=================
CheckIntermissionExit

The level will stay at the intermission for a minimum of 5 seconds
if all players wish to continue, the level will then exit.
if one or more players have not acknowledged the continue, the game will
wait 10 seconds before going on.
=================
*/
void CheckIntermissionExit( void ) {
	int			ready, notReady;
	int			i;
	gclient_t	*cl;
	int			readyMask;

	if ( g_gametype.integer == GT_SINGLE_PLAYER ) {
		return;
	}

	// see which players are ready
	ready = 0;
	notReady = 0;
	readyMask = 0;
	for (i=0 ; i< g_maxclients.integer ; i++) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT ) {
			continue;
		}

		if ( cl->readyToExit ) {
			ready++;
			if ( i < 16 ) {
				readyMask |= 1 << i;
			}
		} else {
			notReady++;
		}
	}

	// copy the readyMask to each player's stats so
	// it can be displayed on the scoreboard
	for (i=0 ; i< g_maxclients.integer ; i++) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		cl->ps.stats[STAT_CLIENTS_READY] = readyMask;
	}

	// never exit in less than five seconds
	if ( level.time < level.intermissiontime + 5000 ) {
		return;
	}

	// if nobody wants to go, clear timer
	if ( !ready ) {
		level.readyToExit = qfalse;
		return;
	}

	// if everyone wants to go, go now
	if ( !notReady ) {
		ExitLevel();
		return;
	}

	// the first person to ready starts the ten second timeout
	if ( !level.readyToExit ) {
		level.readyToExit = qtrue;
		level.exitTime = level.time;
	}

	// if we have waited ten seconds since at least one player
	// wanted to exit, go ahead
	if ( level.time < level.exitTime + 10000 ) {
		return;
	}

	ExitLevel();
}

/*
=============
ScoreIsTied
=============
*/
qboolean ScoreIsTied( void ) {
	int		a, b;

	if ( level.numPlayingClients < 2 ) {
		return qfalse;
	}

	if ( g_gametype.integer >= GT_TEAM ) {
		return level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE];
	}

	a = level.clients[level.sortedClients[0]].ps.persistant[PERS_SCORE];
	b = level.clients[level.sortedClients[1]].ps.persistant[PERS_SCORE];

	return a == b;
}

/*
=================
CheckExitRules

There will be a delay between the time the exit is qualified for
and the time everyone is moved to the intermission spot, so you
can see the last frag.
=================
*/
void CheckExitRules( void ) {
 	int			i;
	gclient_t	*cl;
	// if at the intermission, wait for all non-bots to
	// signal ready, then go to next level
	if ( level.intermissiontime ) {
		CheckIntermissionExit ();
		return;
	}

	if ( level.intermissionQueued ) {
#ifdef MISSIONPACK
		int time = (g_singlePlayer.integer) ? SP_INTERMISSION_DELAY_TIME : INTERMISSION_DELAY_TIME;
		if ( level.time - level.intermissionQueued >= time ) {
			level.intermissionQueued = 0;
			BeginIntermission();
		}
#else
		if ( level.time - level.intermissionQueued >= INTERMISSION_DELAY_TIME ) {
			level.intermissionQueued = 0;
			BeginIntermission();
		}
#endif
		return;
	}

	// check for sudden death
	if ( ScoreIsTied() ) {
		// always wait for sudden death
		return;
	}

	if ( g_timelimit.integer && !level.warmupTime ) {
		if ( level.time - level.startTime >= g_timelimit.integer*60000 ) {
			trap_SendServerCommand( -1, "print \"Timelimit hit.\n\"");
			LogExit( "Timelimit hit." );
			return;
		}
	}

	if ( level.numPlayingClients < 2 ) {
		return;
	}

	if ( g_gametype.integer < GT_CTF && g_fraglimit.integer ) {
		if ( level.teamScores[TEAM_RED] >= g_fraglimit.integer ) {
			trap_SendServerCommand( -1, "print \"Red hit the fraglimit.\n\"" );
			LogExit( "Fraglimit hit." );
			return;
		}

		if ( level.teamScores[TEAM_BLUE] >= g_fraglimit.integer ) {
			trap_SendServerCommand( -1, "print \"Blue hit the fraglimit.\n\"" );
			LogExit( "Fraglimit hit." );
			return;
		}

		for ( i=0 ; i< g_maxclients.integer ; i++ ) {
			cl = level.clients + i;
			if ( cl->pers.connected != CON_CONNECTED ) {
				continue;
			}
			if ( cl->sess.sessionTeam != TEAM_FREE ) {
				continue;
			}

			if ( cl->ps.persistant[PERS_SCORE] >= g_fraglimit.integer ) {
				// accuracy challenge, only sent command if not cheated or something
				if ( level.accuracyChallenge && !g_cheats.integer && g_fraglimit.integer == 150 )
					SendChallenge();

				LogExit( "Fraglimit hit." );
				trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " hit the fraglimit.\n\"",
					cl->pers.netname ) );
				return;
			}
		}
	}

	if ( g_gametype.integer >= GT_CTF && g_capturelimit.integer ) {

		if ( level.teamScores[TEAM_RED] >= g_capturelimit.integer ) {
			trap_SendServerCommand( -1, "print \"Red hit the capturelimit.\n\"" );
			LogExit( "Capturelimit hit." );
			return;
		}

		if ( level.teamScores[TEAM_BLUE] >= g_capturelimit.integer ) {
			trap_SendServerCommand( -1, "print \"Blue hit the capturelimit.\n\"" );
			LogExit( "Capturelimit hit." );
			return;
		}
	}
}



/*
========================================================================

FUNCTIONS CALLED EVERY FRAME

========================================================================
*/


/*
=============
CheckTournament

Once a frame, check for changes in tournement player state
=============
*/
void CheckTournament( void ) {
	// check because we run 3 game frames before calling Connect and/or ClientBegin
	// for clients on a map_restart
	if ( level.numPlayingClients == 0 ) {
		return;
	}

	if ( g_gametype.integer == GT_TOURNAMENT ) {

		// pull in a spectator if needed
		if ( level.numPlayingClients < 2 ) {
			AddTournamentPlayer();
		}

		// if we don't have two players, go back to "waiting for players"
		if ( level.numPlayingClients != 2 ) {
			if ( level.warmupTime != -1 ) {
				level.warmupTime = -1;
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
				G_LogPrintf( "Warmup:\n" );
			}
			return;
		}

		if ( level.warmupTime == 0 ) {
			return;
		}

		// if the warmup is changed at the console, restart it
		if ( g_warmup.modificationCount != level.warmupModificationCount ) {
			level.warmupModificationCount = g_warmup.modificationCount;
			level.warmupTime = -1;
		}

		// if all players have arrived, start the countdown
		if ( level.warmupTime < 0 ) {
			if ( level.numPlayingClients == 2 ) {
				// fudge by -1 to account for extra delays
				level.warmupTime = level.time + ( g_warmup.integer - 1 ) * 1000;
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
			}
			return;
		}

		// if the warmup time has counted down, restart
		if ( level.time > level.warmupTime ) {
			level.warmupTime += 10000;
			trap_Cvar_Set( "g_restarted", "1" );
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			return;
		}
	} else if ( g_gametype.integer != GT_SINGLE_PLAYER && level.warmupTime != 0 ) {
		int		counts[TEAM_NUM_TEAMS];
		qboolean	notEnough = qfalse;

		if ( g_gametype.integer > GT_TEAM ) {
			counts[TEAM_BLUE] = TeamCount( -1, TEAM_BLUE );
			counts[TEAM_RED] = TeamCount( -1, TEAM_RED );

			if (counts[TEAM_RED] < 1 || counts[TEAM_BLUE] < 1) {
				notEnough = qtrue;
			}
		} else if ( level.numPlayingClients < 2 ) {
			notEnough = qtrue;
		}

		if ( notEnough ) {
			if ( level.warmupTime != -1 ) {
				level.warmupTime = -1;
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
				G_LogPrintf( "Warmup:\n" );
			}
			return; // still waiting for team members
		}

		if ( level.warmupTime == 0 ) {
			return;
		}

		// if the warmup is changed at the console, restart it
		if ( g_warmup.modificationCount != level.warmupModificationCount ) {
			level.warmupModificationCount = g_warmup.modificationCount;
			level.warmupTime = -1;
		}

		// if all players have arrived, start the countdown
		if ( level.warmupTime < 0 ) {
			// fudge by -1 to account for extra delays
			level.warmupTime = level.time + ( g_warmup.integer - 1 ) * 1000;
			trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
			return;
		}

		// if the warmup time has counted down, restart
		if ( level.time > level.warmupTime ) {
			level.warmupTime += 10000;
			trap_Cvar_Set( "g_restarted", "1" );
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			return;
		}
	}
}


/*
==================
CheckVote
==================
*/
void CheckVote( void ) {
	if ( level.voteExecuteTime && level.voteExecuteTime < level.time ) {
		level.voteExecuteTime = 0;
		trap_SendConsoleCommand( EXEC_APPEND, va("%s\n", level.voteString ) );
	}
	if ( !level.voteTime ) {
		return;
	}
	if ( level.time - level.voteTime >= VOTE_TIME ) {
		trap_SendServerCommand( -1, "print \"Vote failed.\n\"" );
	} else {
		if ( level.voteYes > level.numVotingClients/2 ) {
			// execute the command, then remove the vote
			trap_SendServerCommand( -1, "print \"Vote passed.\n\"" );
			level.voteExecuteTime = level.time + 3000;
		} else if ( level.voteNo >= level.numVotingClients/2 ) {
			// same behavior as a timeout
			trap_SendServerCommand( -1, "print \"Vote failed.\n\"" );
		} else {
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
void PrintTeam(int team, char *message) {
	int i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if (level.clients[i].sess.sessionTeam != team)
			continue;
		trap_SendServerCommand( i, message );
	}
}

/*
==================
SetLeader
==================
*/
void SetLeader(int team, int client) {
	int i;

	if ( level.clients[client].pers.connected == CON_DISCONNECTED ) {
		PrintTeam(team, va("print \"%s is not connected\n\"", level.clients[client].pers.netname) );
		return;
	}
	if (level.clients[client].sess.sessionTeam != team) {
		PrintTeam(team, va("print \"%s is not on the team anymore\n\"", level.clients[client].pers.netname) );
		return;
	}
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if (level.clients[i].sess.sessionTeam != team)
			continue;
		if (level.clients[i].sess.teamLeader) {
			level.clients[i].sess.teamLeader = qfalse;
			ClientUserinfoChanged(i);
		}
	}
	level.clients[client].sess.teamLeader = qtrue;
	ClientUserinfoChanged( client );
	PrintTeam(team, va("print \"%s is the new team leader\n\"", level.clients[client].pers.netname) );
}

/*
==================
CheckTeamLeader
==================
*/
void CheckTeamLeader( int team ) {
	int i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if (level.clients[i].sess.sessionTeam != team)
			continue;
		if (level.clients[i].sess.teamLeader)
			break;
	}
	if (i >= level.maxclients) {
		for ( i = 0 ; i < level.maxclients ; i++ ) {
			if (level.clients[i].sess.sessionTeam != team)
				continue;
			if (!(g_entities[i].r.svFlags & SVF_BOT)) {
				level.clients[i].sess.teamLeader = qtrue;
				break;
			}
		}
		for ( i = 0 ; i < level.maxclients ; i++ ) {
			if (level.clients[i].sess.sessionTeam != team)
				continue;
			level.clients[i].sess.teamLeader = qtrue;
			break;
		}
	}
}

/*
==================
CheckTeamVote
==================
*/
void CheckTeamVote( int team ) {
	int cs_offset;

	if ( team == TEAM_RED )
		cs_offset = 0;
	else if ( team == TEAM_BLUE )
		cs_offset = 1;
	else
		return;

	if ( !level.teamVoteTime[cs_offset] ) {
		return;
	}
	if ( level.time - level.teamVoteTime[cs_offset] >= VOTE_TIME ) {
		trap_SendServerCommand( -1, "print \"Team vote failed.\n\"" );
	} else {
		if ( level.teamVoteYes[cs_offset] > level.numteamVotingClients[cs_offset]/2 ) {
			// execute the command, then remove the vote
			trap_SendServerCommand( -1, "print \"Team vote passed.\n\"" );
			//
			if ( !Q_strncmp( "leader", level.teamVoteString[cs_offset], 6) ) {
				//set the team leader
				SetLeader(team, atoi(level.teamVoteString[cs_offset] + 7));
			}
			else {
				trap_SendConsoleCommand( EXEC_APPEND, va("%s\n", level.teamVoteString[cs_offset] ) );
			}
		} else if ( level.teamVoteNo[cs_offset] >= level.numteamVotingClients[cs_offset]/2 ) {
			// same behavior as a timeout
			trap_SendServerCommand( -1, "print \"Team vote failed.\n\"" );
		} else {
			// still waiting for a majority
			return;
		}
	}
	level.teamVoteTime[cs_offset] = 0;
	trap_SetConfigstring( CS_TEAMVOTE_TIME + cs_offset, "" );

}


/*
==================
CheckCvars
==================
*/
void CheckCvars( void ) {
	static int lastMod = -1;

	if ( g_password.modificationCount != lastMod ) {
		lastMod = g_password.modificationCount;
		if ( *g_password.string && Q_stricmp( g_password.string, "none" ) ) {
			trap_Cvar_Set( "g_needpass", "1" );
		} else {
			trap_Cvar_Set( "g_needpass", "0" );
		}
	}
}

/*
=============
G_RunThink

Runs thinking code for this frame if necessary
=============
*/
void G_RunThink (gentity_t *ent) {
	float	thinktime;

	thinktime = ent->nextthink;
	if (thinktime <= 0) {
		return;
	}
	if (thinktime > level.time) {
		return;
	}

	ent->nextthink = 0;
	if (!ent->think) {
		G_Error ( "NULL ent->think");
	}
	ent->think (ent);
}

/*
================
G_RunFrame

Advances the non-player objects in the world
================
*/
void G_RunFrame( int levelTime ) {
	int			i;
	gentity_t	*ent;
	int			msec;
int start, end;

	// if we are waiting for the level to restart, do nothing
	if ( level.restarted ) {
		return;
	}

	level.framenum++;
	level.previousTime = level.time;
	level.time = levelTime;
	msec = level.time - level.previousTime;

	// get any cvar changes
	G_UpdateCvars();

	//
	// go through all allocated objects
	//
	start = trap_Milliseconds();
	ent = &g_entities[0];
	for (i=0 ; i<level.num_entities ; i++, ent++) {
		if ( !ent->inuse ) {
			continue;
		}

		// clear events that are too old
		if ( level.time - ent->eventTime > EVENT_VALID_MSEC ) {
			if ( ent->s.event ) {
				ent->s.event = 0;	// &= EV_EVENT_BITS;
				if ( ent->client ) {
					ent->client->ps.externalEvent = 0;
					// predicted events should never be set to zero
					//ent->client->ps.events[0] = 0;
					//ent->client->ps.events[1] = 0;
				}
			}
			if ( ent->freeAfterEvent ) {
				// tempEntities or dropped items completely go away after their event
				G_FreeEntity( ent );
				continue;
			} else if ( ent->unlinkAfterEvent ) {
				// items that will respawn will hide themselves after their pickup event
				ent->unlinkAfterEvent = qfalse;
				trap_UnlinkEntity( ent );
			}
		}

		// temporary entities don't think
		if ( ent->freeAfterEvent ) {
			continue;
		}

		if ( !ent->r.linked && ent->neverFree ) {
			continue;
		}

		if ( ent->s.eType == ET_MISSILE ) {
			G_RunMissile( ent );
			continue;
		}

		if ( ent->s.eType == ET_ITEM || ent->physicsObject ) {
			G_RunItem( ent );
			continue;
		}

		if ( ent->s.eType == ET_MOVER ) {
			G_RunMover( ent );
			continue;
		}

		if ( i < MAX_CLIENTS ) {
			G_RunClient( ent );
			continue;
		}

		G_RunThink( ent );
	}
end = trap_Milliseconds();

start = trap_Milliseconds();
	// perform final fixups on the players
	ent = &g_entities[0];
	for (i=0 ; i < level.maxclients ; i++, ent++ ) {
		if ( ent->inuse ) {
			ClientEndFrame( ent );
		}
	}
end = trap_Milliseconds();

	// see if it is time to do a tournement restart
	CheckTournament();

	// see if it is time to end the level
	CheckExitRules();

	// update to team status?
	CheckTeamStatus();

	// cancel vote if timed out
	CheckVote();

	// check team votes
	CheckTeamVote( TEAM_RED );
	CheckTeamVote( TEAM_BLUE );

	// for tracking changes
	CheckCvars();

	if (g_listEntity.integer) {
		for (i = 0; i < MAX_GENTITIES; i++) {
			G_Printf("%4i: %s\n", i, g_entities[i].classname);
		}
		trap_Cvar_Set("g_listEntity", "0");
	}
}
