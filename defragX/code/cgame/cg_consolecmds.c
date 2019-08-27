// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_consolecmds.c -- text commands typed in at the local console, or
// executed by a key binding

#include "cg_local.h"

/*
=================
CG_TargetCommand_f
=================
*/
static void CG_TargetCommand_f( void )
{
	int		targetNum;
	char	cmd[4];

	targetNum = CG_CrosshairPlayer();
	if ( targetNum == -1 )
    {
		return;
	}

	trap_Argv( 1, cmd, sizeof( cmd ) );
	trap_SendConsoleCommand( va( "gc %i %i", targetNum, atoi( cmd ) ) );
}

#ifdef DEVBUILD
/*
=============
CG_Viewpos_f

Debugging command to print the current position
=============
*/
static void CG_Viewpos_f (void)
{
	CG_Printf ("(%i %i %i) : %i\n", (int)cg.refdef.vieworg[0],
		(int)cg.refdef.vieworg[1], (int)cg.refdef.vieworg[2],
		(int)cg.refdefViewAngles[YAW]);
}
#endif

/*
=============
CG_SavePos_f

Save position into cvar for using with placeplayer and vstr
=============
*/
static void CG_SavePos_f (void)
{
	char	buffer[MAX_TOKEN_CHARS];
	playerState_t *ps = &cg.predictedPlayerState;

	if( trap_Argc() != 2 )
    {
		CG_Printf( "usage: savepos cvar\n" );
		return;
	}

	trap_Argv( 1, buffer, sizeof( buffer ) );
	trap_Cvar_Set( buffer, va("placeplayer %f %f %f %f %f %f %f %f %f",
                    ps->origin[0], ps->origin[1], ps->origin[2],
					cg.refdefViewAngles[0], cg.refdefViewAngles[1], cg.refdefViewAngles[2],
					ps->velocity[0], ps->velocity[1], ps->velocity[2]) );

	CG_Printf( "Position saved in %s\n", buffer );
}


static void CG_ScoresDown_f( void )
{

	if ( cg.scoresRequestTime + 2000 < cg.time && !cg.demoPlayback )
    {
		// the scores are more than two seconds out of data,
		// so request new ones
		cg.scoresRequestTime = cg.time;
		trap_SendClientCommand( "score" );

		// leave the current scores up if they were already
		// displayed, but if this is the first hit, clear them out
		if ( !cg.showScores )
        {
			cg.showScores = qtrue;
			cg.numScores = 0;
		}
	} else
	{
		// show the cached contents even if they just pressed if it
		// is within two seconds
		cg.showScores = qtrue;
	}

	CG_SetScoreCatcher( cg.showScores );
}


static void CG_ScoresUp_f( void )
{

	if ( cgs.filterKeyUpEvent )
    {
		cgs.filterKeyUpEvent = qfalse;
		return;
	}

	if ( cg.showScores )
    {
		cg.showScores = qfalse;
		cg.scoreFadeTime = cg.time;
	}

	CG_SetScoreCatcher( cg.showScores );
}


/*
==================
CG_TellTarget_f
==================
*/
static void CG_TellTarget_f( void )
{
	int		clientNum;
	char	command[128];
	char	message[128];

	clientNum = CG_CrosshairPlayer();
	if ( clientNum == -1 )
    {
		return;
	}

	trap_Args( message, sizeof( message ) );
	Com_sprintf( command, sizeof( command ), "tell %i %s", clientNum, message );
	trap_SendClientCommand( command );
}


/*
==================
CG_TellAttacker_f
==================
*/
static void CG_TellAttacker_f( void )
{
	int		clientNum;
	char	command[128];
	char	message[128];

	clientNum = CG_LastAttacker();
	if ( clientNum == -1 )
    {
		return;
	}

	trap_Args( message, sizeof( message ) );
	Com_sprintf( command, sizeof( command ), "tell %i %s", clientNum, message );
	trap_SendClientCommand( command );
}

/*
==================
CG_SaveDemo
==================
*/
static void CG_SaveDemo( void ) {
	static char buffer[MAX_STRING_CHARS];
	int protocol = CG_VariableIntegerValue( "protocol" );

	if ( trap_Argc() != 2 ) {
		Com_Printf( "Usage: savedemo [name]\n" );
		return;
	}

	trap_Argv( 1, buffer, sizeof(buffer) );
	if ( BG_CopyFile( va("demos/temp/prev.dm_%d", protocol), va("demos/%s.dm_%d", buffer, protocol) ) ) {
		Com_Printf( "Demo saved to demos/%s.dm_%d\n", buffer, protocol );
	} else {
		Com_Printf( "Unable to save demo\n" );
	}
}

#ifdef DEVBUILD
static void CG_PsAddr( void ) {
	Com_Printf("%u\n", &cg.predictedPlayerState);
}
#endif


typedef struct {
	const char *cmd;
	void	(*function)(void);
} consoleCommand_t;

static consoleCommand_t	commands[] = {
#ifdef DEVBUILD
	{ "testgun", CG_TestGun_f },
	{ "testmodel", CG_TestModel_f },
	{ "nextframe", CG_TestModelNextFrame_f },
	{ "prevframe", CG_TestModelPrevFrame_f },
	{ "nextskin", CG_TestModelNextSkin_f },
	{ "prevskin", CG_TestModelPrevSkin_f },
	{ "psaddr", CG_PsAddr },
	{ "viewpos", CG_Viewpos_f },
#endif // DEVBUILD
    { "savepos", CG_SavePos_f },
	{ "+scores", CG_ScoresDown_f },
	{ "-scores", CG_ScoresUp_f },
	{ "+zoom", CG_ZoomDown_f },
	{ "-zoom", CG_ZoomUp_f },
	{ "weapnext", CG_NextWeapon_f },
	{ "weapprev", CG_PrevWeapon_f },
	{ "weapon", CG_Weapon_f },
	{ "tcmd", CG_TargetCommand_f },
	{ "tell_target", CG_TellTarget_f },
	{ "tell_attacker", CG_TellAttacker_f },
	{ "loaddeferred", CG_LoadDeferredPlayers },
	{ "savedemo", CG_SaveDemo }
};


/*
=================
CG_ConsoleCommand

The string has been tokenized and can be retrieved with
Cmd_Argc() / Cmd_Argv()
=================
*/
qbool CG_ConsoleCommand( void )
{
	const char	*cmd;
	int		     i;

	cmd = CG_Argv(0);

	for ( i = 0 ; i < ARRAY_LEN( commands ) ; i++ )
    {
		if ( !Q_stricmp( cmd, commands[i].cmd ) )
		{
			commands[i].function();
			return qtrue;
		}
	}

	return qfalse;
}


/*
=================
CG_InitConsoleCommands

Let the client system know about all of our commands
so it can perform tab completion
=================
*/
void CG_InitConsoleCommands( void ) {
	int		i;

	for ( i = 0 ; i < ARRAY_LEN( commands ) ; i++ )
    {
		trap_AddCommand( commands[i].cmd );
	}

	// the game server will interpret these commands, which will be automatically
	// forwarded to the server after they are not recognized locally
	//
	trap_AddCommand ("kill");
	trap_AddCommand ("say");
	trap_AddCommand ("say_team");
	trap_AddCommand ("tell");
	trap_AddCommand ("give");
	trap_AddCommand ("god");
	trap_AddCommand ("noclip");
	trap_AddCommand ("team");
	trap_AddCommand ("placeplayer");
    trap_AddCommand ("respawnpoint");
	trap_AddCommand ("follow");
	trap_AddCommand ("setviewpos");
	trap_AddCommand ("callvote");
	trap_AddCommand ("cv");
	trap_AddCommand ("vote");
	trap_AddCommand ("stats");
	trap_AddCommand ("loaddefered");
}
