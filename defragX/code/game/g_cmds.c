// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "g_local.h"

/*
==================
DeathmatchScoreboardMessage
==================
*/
void DeathmatchScoreboardMessage( gentity_t *ent ) {
	char		entry[256]; // enough to hold 14 integers
	char		string[MAX_STRING_CHARS-1];
	int			stringlength;
	int			i, j, ping, prefix;
	gclient_t	*cl;
	int			numSorted, scoreFlags, accuracy, perfect;

	// send the latest information on all clients
	string[0] = '\0';
	stringlength = 0;
	scoreFlags = 0;

	numSorted = level.numConnectedClients;

	// estimate prefix length to avoid oversize of final string
	prefix = BG_sprintf( entry, "scores %i %i %i", level.teamScores[TEAM_RED], level.teamScores[TEAM_BLUE], numSorted );

	for ( i = 0 ; i < numSorted ; i++ ) {

		cl = &level.clients[level.sortedClients[i]];

		if ( cl->pers.connected == CON_CONNECTING ) {
			ping = -1;
		} else {
			ping = cl->ps.ping < 999 ? cl->ps.ping : 999;
		}

		if( cl->accuracy_shots ) {
			accuracy = cl->accuracy_hits * 100 / cl->accuracy_shots;
		} else {
			accuracy = 0;
		}

		perfect = ( cl->ps.persistant[PERS_RANK] == 0 && cl->ps.persistant[PERS_KILLED] == 0 ) ? 1 : 0;
		
		//						 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 
		j = BG_sprintf( entry, " %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i",
			level.sortedClients[i],
			(dfx_gametype.integer != GT_FREESTYLE ? cl->pers.dfBestTime : cl->ps.persistant[PERS_SCORE]),
			ping,
			(level.time - cl->pers.enterTime)/60000,
			scoreFlags,
			g_entities[level.sortedClients[i]].s.powerups,
			accuracy,
			cl->ps.persistant[PERS_IMPRESSIVE_COUNT],
			cl->ps.persistant[PERS_EXCELLENT_COUNT],
			cl->ps.persistant[PERS_GAUNTLET_FRAG_COUNT],
			cl->ps.persistant[PERS_DEFEND_COUNT],
			cl->ps.persistant[PERS_ASSIST_COUNT],
			perfect,
			//cl->ps.persistant[PERS_CAPTURES],
			cl->ps.persistant[PERS_SECRETS],
			cl->ps.persistant[PERS_KEYS]
            );

		if ( stringlength + j + prefix >= sizeof( string ) )
			break;

		strcpy( string + stringlength, entry );
		stringlength += j;
	}

	trap_SendServerCommand( ent-g_entities, va( "scores %i %i %i%s",
                                                         i,
                                                         level.teamScores[TEAM_RED],
                                                         level.teamScores[TEAM_BLUE],
                                                         string ) );
}


/*
==================
Cmd_Score_f

Request current scoreboard information
==================
*/
void Cmd_Score_f( gentity_t *ent ) {
	DeathmatchScoreboardMessage( ent );
}


/*
==================
CheatsOk
==================
*/
qbool CheatsOk( gentity_t *ent ) {
	if ( !g_cheats.integer ) {
		trap_SendServerCommand( ent-g_entities, "print \"Cheats are not enabled on this server.\n\"");
		return qfalse;
	}
	if ( ent->health <= 0 ) {
		trap_SendServerCommand( ent-g_entities, "print \"You must be alive to use this command.\n\"");
		return qfalse;
	}
	return qtrue;
}


/*
==================
ConcatArgs
==================
*/
char *ConcatArgs( int start ) {
	static char line[MAX_STRING_CHARS];
	char	arg[MAX_STRING_CHARS];
	int		i, c, tlen;
	int		len;

	len = 0;
	c = trap_Argc();
	for ( i = start ; i < c ; i++ ) {
		trap_Argv( i, arg, sizeof( arg ) );
		tlen = (int)strlen( arg );
		if ( len + tlen >= sizeof( line )-1 )
			break;

		memcpy( line + len, arg, tlen );
		len += tlen;
		if ( i != c - 1 ) {
			line[len] = ' ';
			len++;
		}
	}

	line[len] = '\0';

	return line;
}


/*
==================
SanitizeString

Remove case and control characters
==================
*/
void SanitizeString( const char *in, char *out ) {
	while ( *in ) {
		if ( *in == 27 ) {
			in += 2;		// skip color code
			continue;
		}
		if ( *in < ' ' ) {
			in++;
			continue;
		}
		*out = tolower( *in );
		out++;
		in++;
	}

	*out = '\0';
}


/*
==================
ClientNumberFromString

Returns a player number for either a number or name string
Returns -1 if invalid
==================
*/
int ClientNumberFromString( gentity_t *to, char *s ) {
	gclient_t	*cl;
	int			idnum;
	char		s2[MAX_STRING_CHARS];
	char		n2[MAX_STRING_CHARS];

	// numeric values are just slot numbers
	if (s[0] >= '0' && s[0] <= '9') {
		idnum = atoi( s );
		if ( (unsigned) idnum >= (unsigned)level.maxclients ) {
			trap_SendServerCommand( to-g_entities, va("print \"Bad client slot: %i\n\"", idnum));
			return -1;
		}

		cl = &level.clients[idnum];
		if ( cl->pers.connected != CON_CONNECTED ) {
			trap_SendServerCommand( to-g_entities, va("print \"Client %i is not active\n\"", idnum));
			return -1;
		}
		return idnum;
	}

	// check for a name match
	SanitizeString( s, s2 );
	for ( idnum=0,cl=level.clients ; idnum < level.maxclients ; idnum++,cl++ ) {
		if ( cl->pers.connected != CON_CONNECTED )
			continue;

		SanitizeString( cl->pers.netname, n2 );
		if ( !strcmp( n2, s2 ) )
			return idnum;
	}

	trap_SendServerCommand( to-g_entities, va("print \"User %s is not on the server\n\"", s));
	return -1;
}


/*
==================
Cmd_Give_f

Give items to a client
==================
*/
void Cmd_Give_f( gentity_t *ent )
{
	char		*name;
	gitem_t		*it;
	int			i;
	qbool	give_all;
	gentity_t	*it_ent;
	trace_t		trace;

	if ( !CheatsOk( ent ) )
		return;

	name = ConcatArgs( 1 );

	if (Q_stricmp(name, "all") == 0)
		give_all = qtrue;
	else
		give_all = qfalse;

	if (give_all || Q_stricmp( name, "health") == 0)
	{
		ent->health = ent->client->ps.stats[STAT_MAX_HEALTH];
		if (!give_all)
			return;
	}

	if (give_all || Q_stricmp(name, "weapons") == 0)
	{
		ent->client->ps.stats[STAT_WEAPONS] = (1 << WP_NUM_WEAPONS) - 1 -
			( 1 << WP_GRAPPLING_HOOK ) - ( 1 << WP_NONE );
		if (!give_all)
			return;
	}

	if (give_all || Q_stricmp(name, "ammo") == 0)
	{
		for ( i = 0 ; i < MAX_WEAPONS ; i++ ) {
			ent->client->ps.ammo[i] = 999;
		}
		if (!give_all)
			return;
	}

	if (give_all || Q_stricmp(name, "armor") == 0)
	{
		ent->client->ps.stats[STAT_ARMOR] = 200;

		if (!give_all)
			return;
	}

	// spawn a specific item right on the player
	if ( !give_all ) {
		it = BG_FindItem (name);
		if (!it)
			return;

		it_ent = G_Spawn();
		VectorCopy( ent->r.currentOrigin, it_ent->s.origin );
		it_ent->classname = it->classname;
		G_SpawnItem (it_ent, it);
		FinishSpawningItem(it_ent );
		memset( &trace, 0, sizeof( trace ) );
		Touch_Item (it_ent, ent, &trace);
		if (it_ent->inuse)
			G_FreeEntity( it_ent );
	}
}


/*
==================
Cmd_God_f

Sets client to godmode

argv(0) god
==================
*/
void Cmd_God_f( gentity_t *ent )
{
	const char *msg;

	if ( !CheatsOk( ent ) )
		return;

	ent->flags ^= FL_GODMODE;
	if (!(ent->flags & FL_GODMODE) )
		msg = "godmode OFF\n";
	else
		msg = "godmode ON\n";

	trap_SendServerCommand( ent-g_entities, va( "print \"%s\"", msg ) );
}

/*
=================
Cmd_PlacePlayer_f
=================
*/
void Cmd_PlacePlayer_f( gentity_t *ent ) {
	char	buffer[MAX_TOKEN_CHARS];
	int 	i;
	vec3_t	angles;

	if (dfx_gametype.integer == GT_ADVENTURE ) {
        trap_SendServerCommand( ent-g_entities, "print \"not allowed in GT_ADVENTURE\n");
        return;
	}

	if (!ent->client)
        return;

	if ( dfx_gametype.integer != GT_FREESTYLE || (!CheatsOk(ent)))
        return;

	if( trap_Argc() != 10 ) {
		trap_SendServerCommand( ent-g_entities, "print \"usage: placeplayer x y z p y r v0 v1 v2\n\"");
		return;
	}

	//origin
	for ( i = 0 ; i < 3 ; i++ ) {
		trap_Argv( i + 1, buffer, sizeof( buffer ) );
		ent->client->ps.origin[i] = atof(buffer);
	}

	//angles
	for ( i = 3 ; i < 6 ; i++ ) {
		trap_Argv( i + 1, buffer, sizeof( buffer ) );
		angles[i - 3] = atof(buffer);
	}

	//velocity
	for ( i = 6 ; i < 9 ; i++ ) {
		trap_Argv( i + 1, buffer, sizeof( buffer ) );
		ent->client->ps.velocity[i - 6] = atof(buffer);
	}

	SetClientViewAngle( ent, angles );
	ent->client->ps.eFlags ^= EF_TELEPORT_BIT;
	BG_PlayerStateToEntityState( &ent->client->ps, &ent->s, qtrue );
	VectorCopy( ent->client->ps.origin, ent->r.currentOrigin );
}

/*
==================
Cmd_Noclip_f

argv(0) noclip
==================
*/
void Cmd_Noclip_f( gentity_t *ent ) {
	const char *msg;

	if ( !CheatsOk( ent ) )
		return;

	if ( ent->client->noclip )
		msg = "noclip OFF\n";
	else
		msg = "noclip ON\n";

	ent->client->noclip = !ent->client->noclip;
	trap_SendServerCommand( ent-g_entities, va("print \"%s\"", msg));
}

/*
=================
Cmd_Kill_f
=================
*/
void Cmd_Kill_f( gentity_t *ent ) {
	if ( (ent->client->sess.sessionTeam == TEAM_SPECTATOR) || (ent->health <= 0) )
		return;

	ent->flags &= ~FL_GODMODE;
	ent->client->ps.stats[STAT_HEALTH] = ent->health = -999;
	player_die (ent, ent, ent, 100000, MOD_SUICIDE);
}


/*
=================
BroadcastTeamChange

Let everyone know about a team change
=================
*/
void BroadcastTeamChange( gclient_t *client, team_t oldTeam )
{
	int clientNum = client - level.clients;

    if ( client->sess.sessionTeam == TEAM_SPECTATOR && oldTeam != TEAM_SPECTATOR ) {
		G_BroadcastServerCommand( clientNum, va("cp \"%s" S_COLOR_WHITE " joined the spectators.\n\"",		client->pers.netname));
	} else if ( client->sess.sessionTeam == TEAM_FREE  && oldTeam != TEAM_FREE ) {
		G_BroadcastServerCommand( clientNum, va("cp \"%s" S_COLOR_WHITE " entered the game.\n\"",			client->pers.netname));
	} else if ( client->sess.sessionTeam == TEAM_RED  && oldTeam != TEAM_RED ) {
		G_BroadcastServerCommand( clientNum, va("cp \"%s" S_COLOR_WHITE " entered as player red.\n\"",		client->pers.netname));
	} else if ( client->sess.sessionTeam == TEAM_BLUE  && oldTeam != TEAM_BLUE ) {
		G_BroadcastServerCommand( clientNum, va("cp \"%s" S_COLOR_WHITE " entered as player blue.\n\"",		client->pers.netname));
	} else if ( client->sess.sessionTeam == TEAM_GREEN  && oldTeam != TEAM_GREEN ) {
		G_BroadcastServerCommand( clientNum, va("cp \"%s" S_COLOR_WHITE " entered as player green.\n\"",	client->pers.netname));
	} else if ( client->sess.sessionTeam == TEAM_YELLOW  && oldTeam != TEAM_YELLOW) {
		G_BroadcastServerCommand( clientNum, va("cp \"%s" S_COLOR_WHITE " entered as player yellow.\n\"",	client->pers.netname));
	}
}


static qbool AllowTeamSwitch( int clientNum, team_t newTeam ) {

	int		counts[TEAM_NUM_TEAMS];

	counts[TEAM_BLUE] 	= TeamCount( clientNum, TEAM_BLUE );
	counts[TEAM_RED] 	= TeamCount( clientNum, TEAM_RED );
	counts[TEAM_GREEN] 	= TeamCount( clientNum, TEAM_GREEN );
	counts[TEAM_YELLOW] = TeamCount( clientNum, TEAM_YELLOW );

	if ( newTeam == TEAM_BLUE && counts[TEAM_BLUE] > 0) {
		trap_SendServerCommand( clientNum, "cp \"Blue spawn is already used.\n\"" );
		return qfalse;
	} else if ( newTeam == TEAM_RED && counts[TEAM_RED] > 0) {
		trap_SendServerCommand( clientNum, "cp \"Red spawn is already used.\n\"" );
		return qfalse;
	} else if ( newTeam == TEAM_GREEN && counts[TEAM_GREEN] > 0) {
		trap_SendServerCommand( clientNum, "cp \"Green spawn is already used.\n\"" );
		return qfalse;
	} else if ( newTeam == TEAM_YELLOW && counts[TEAM_YELLOW] > 0) {
		trap_SendServerCommand( clientNum, "cp \"Yellow spawn is already used.\n\"" );
		return qfalse;
	}

	return qtrue;
}


/*
=================
SetTeam
=================
*/
qbool SetTeam( gentity_t *ent, const char *s ) {
	team_t				team, oldTeam;
	gclient_t			*client;
	int					clientNum;
	spectatorState_t	specState;
	int					specClient;
	int					teamLeader;
	qbool   			checkTeamLeader;

	//
	// see what change is requested
	//

	clientNum = ent - g_entities;
	client = level.clients + clientNum;

	// early team override
	if ( client->pers.connected == CON_CONNECTING && dfx_gametype.integer == GT_TEAMRUN ) {
		if ( !Q_stricmp( s, "red" ) || !Q_stricmp( s, "r" ) ) {
			team = TEAM_RED;
		} else if ( !Q_stricmp( s, "blue" ) || !Q_stricmp( s, "b" ) ) {
			team = TEAM_BLUE;
		} else if ( !Q_stricmp( s, "green" ) || !Q_stricmp( s, "g" ) ) {
			team = TEAM_GREEN;
		} else if ( !Q_stricmp( s, "yellow" ) || !Q_stricmp( s, "y" ) ) {
			team = TEAM_YELLOW;
		} else {
			team = -1;
		}
		
		if ( team != -1 && AllowTeamSwitch( clientNum, team ) ) {
			client->sess.sessionTeam = team;
			client->pers.teamState.state = TEAM_BEGIN;
			G_WriteClientSessionData( client );
			// count current clients and rank for scoreboard
			CalculateRanks();
		}
		return qfalse; // bypass flood protection
	}

	specClient = clientNum;
	specState = SPECTATOR_NOT;
	if ( !Q_stricmp( s, "scoreboard" ) || !Q_stricmp( s, "score" )  ) {
		team = TEAM_SPECTATOR;
		specState = SPECTATOR_SCOREBOARD;
	} else if ( !Q_stricmp( s, "follow1" ) ) {
		team = TEAM_SPECTATOR;
		specState = SPECTATOR_FOLLOW;
		specClient = -1;
	} else if ( !Q_stricmp( s, "follow2" ) ) {
		team = TEAM_SPECTATOR;
		specState = SPECTATOR_FOLLOW;
		specClient = -2;
	} else if ( !Q_stricmp( s, "spectator" ) || !Q_stricmp( s, "s" ) ) {
		team = TEAM_SPECTATOR;
		specState = SPECTATOR_FREE;
	} else if ( dfx_gametype.integer == GT_TEAMRUN ) {
		// if running a team game, assign player to one of the teams
		specState = SPECTATOR_NOT;
		if ( !Q_stricmp( s, "red" ) || !Q_stricmp( s, "r" ) ) {
			team = TEAM_RED;
		} else if ( !Q_stricmp( s, "blue" ) || !Q_stricmp( s, "b" ) ) {
			team = TEAM_BLUE;
		} else if ( !Q_stricmp( s, "green" ) || !Q_stricmp( s, "g" ) ) {
			team = TEAM_GREEN;
		} else if ( !Q_stricmp( s, "yellow" ) || !Q_stricmp( s, "y" ) ) {
			team = TEAM_YELLOW;
		} else {
			// pick the team with the least number of players
			team = PickTeam ( clientNum ); 	// team = TEAM_SPECTATOR;
		}
		
		if ( !AllowTeamSwitch( clientNum, team ) )
			return qtrue;
		
	} else {
		// force them to spectators if there aren't any spots free
		team = TEAM_FREE;
	}
			
	
	// override decision if limiting the players
	if ( ( dfx_gametype.integer == GT_ADVENTURE) && level.numNonSpectatorClients >= 1 ) {
		team = TEAM_SPECTATOR;
	} else if ( g_maxGameClients.integer > 0 && level.numNonSpectatorClients >= g_maxGameClients.integer ) {
		team = TEAM_SPECTATOR;
	}
	
	//
	// decide if we will allow the change
	//
	oldTeam = client->sess.sessionTeam;
	if ( team == oldTeam ) {
		
		if ( team != TEAM_SPECTATOR )
			return qfalse;

		// do soft release if possible
		if ( ( client->ps.pm_flags & PMF_FOLLOW ) && client->sess.spectatorState == SPECTATOR_FOLLOW ) {
			StopFollowing( ent, qtrue );
			return qfalse;
		}

		// second spectator team request will move player to intermission point
		if ( client->ps.persistant[ PERS_TEAM ] == TEAM_SPECTATOR && !( client->ps.pm_flags & PMF_FOLLOW ) && client->sess.spectatorState == SPECTATOR_FREE ) {
			VectorCopy( level.intermission_origin, ent->s.origin );
			VectorCopy( level.intermission_origin, client->ps.origin );
			SetClientViewAngle( ent, level.intermission_angle );
			return qfalse;
		}
	}

	//
	// execute the team change
	//

	// if the player was dead leave the body
	if ( ent->health <= 0 ) 
		CopyToBodyQue( ent );

	// he starts at 'base'
	client->pers.teamState.state = TEAM_BEGIN;

	if ( oldTeam != TEAM_SPECTATOR ) {

		// revert any casted votes
		if ( oldTeam != team )
			G_RevertVote( ent->client );

		// Kill him (makes sure he loses flags, etc)
		ent->flags &= ~FL_GODMODE;
		ent->client->ps.stats[STAT_HEALTH] = ent->health = 0;
		player_die (ent, ent, ent, 100000, MOD_SUICIDE);
	}

	if ( team == TEAM_SPECTATOR ) 
		client->sess.spectatorTime = 0;
		
	client->sess.sessionTeam = team;
	client->sess.spectatorState = specState;
	client->sess.spectatorClient = specClient;

	checkTeamLeader = client->sess.teamLeader;
	client->sess.teamLeader = qfalse;
	/*
	if ( team == TEAM_RED || team == TEAM_BLUE || team == TEAM_GREEN || team == TEAM_YELLOW) {
		teamLeader = TeamLeader( team );
		// if there is no team leader or the team leader is a bot and this client is not a bot
		if ( teamLeader == -1 || ( !(g_entities[clientNum].r.svFlags & SVF_BOT) && (g_entities[teamLeader].r.svFlags & SVF_BOT) ) ) {
			SetLeader( team, clientNum );
		}
	}

	// make sure there is a team leader on the team the player came from
	if ( oldTeam == TEAM_RED || oldTeam == TEAM_BLUE || oldTeam == TEAM_GREEN || oldTeam == TEAM_YELLOW ) {
		if ( checkTeamLeader ) {
			CheckTeamLeader( oldTeam );
		}
	}
	*/
	G_WriteClientSessionData( client );

	BroadcastTeamChange( client, oldTeam );

	// get and distribute relevent paramters
	ClientUserinfoChanged( clientNum );

	ClientBegin( clientNum );

	return qtrue;
}

/*
=================
StopFollowing

If the client being followed leaves the game, or you just want to drop
to free floating spectator mode
=================
*/
void StopFollowing( gentity_t *ent, qbool release ) {
	gclient_t *client;

	if ( !ent->inuse )
		return;

	client = ent->client;

	client->ps.persistant[ PERS_TEAM ] = TEAM_SPECTATOR;
	client->sess.sessionTeam = TEAM_SPECTATOR;
	if ( release ) {
		client->ps.stats[STAT_HEALTH] = ent->health = 1;
		memset( client->ps.powerups, 0, sizeof ( client->ps.powerups ) );
	}
	SetClientViewAngle( ent, client->ps.viewangles );

	client->sess.spectatorState = SPECTATOR_FREE;
	client->ps.pm_flags &= ~PMF_FOLLOW;

	client->ps.clientNum = ent - g_entities;
}


/*
=================
Cmd_Team_f
=================
*/
void Cmd_Team_f( gentity_t *ent ) {
	char	s[MAX_TOKEN_CHARS];

	if ( trap_Argc() != 2 ) {
		switch ( ent->client->sess.sessionTeam ) {
		case TEAM_BLUE:
			trap_SendServerCommand( ent-g_entities, "print \"Blue team\n\"" );
			break;
		case TEAM_RED:
			trap_SendServerCommand( ent-g_entities, "print \"Red team\n\"" );
			break;
		case TEAM_GREEN:
			trap_SendServerCommand( ent-g_entities, "print \"Green team\n\"" );
			break;
		case TEAM_YELLOW:
			trap_SendServerCommand( ent-g_entities, "print \"Yellow team\n\"" );
			break;
		case TEAM_FREE:
			trap_SendServerCommand( ent-g_entities, "print \"Free team\n\"" );
			break;
		case TEAM_SPECTATOR:
			trap_SendServerCommand( ent-g_entities, "print \"Spectator team\n\"" );
			break;
		}
		return;
	}

	if ( ent->client->switchTeamTime > level.time ) {
		trap_SendServerCommand( ent-g_entities, "print \"May not switch teams more than once a seconds.\n\"" );
		return;
	}

	trap_Argv( 1, s, sizeof( s ) );

	if ( SetTeam( ent, s ) ) {
		ent->client->switchTeamTime = level.time + 1000;
	}
}

void ResetTimers( void )
{
	gentity_t *ent = &g_entities[0];
	int i;

	for( i = 0; i < MAX_CLIENTS; i++, ent++ )
    {
		if ( !ent->client ){
			continue;
		};

		ent->client->dfTimerOn = qfalse;
		ent->client->dfTimer = 0;
		ent->client->pers.dfBestTime = 0;
	}
}

void Cmd_StartTimer_f( gentity_t *ent )
{
	static gentity_t *itemFlag1 = NULL;
	static gentity_t *startTimer = NULL;

	ResetTimers();

	if ( !itemFlag1 ) {
		itemFlag1 = G_Spawn();
		itemFlag1->classname = "start_flag";
		itemFlag1->spawnflags = 1; // suspended
		itemFlag1->target = "InternalStartTimer";
		VectorCopy( ent->client->ps.origin, itemFlag1->s.origin );
		G_CallSpawn( itemFlag1 );
		itemFlag1->wait = 1;

		startTimer = G_Spawn();
		startTimer->targetname = "InternalStartTimer";
		SP_target_startTimer( startTimer );
	} else {
		VectorCopy( ent->client->ps.origin, itemFlag1->s.pos.trBase );
		VectorCopy( itemFlag1->s.pos.trBase, itemFlag1->r.currentOrigin );
		trap_LinkEntity( itemFlag1 );
	}
}

void Cmd_StopTimer_f( gentity_t *ent )
{
	static gentity_t *itemFlag2 = NULL;
	static gentity_t *stopTimer = NULL;

	ResetTimers();

	if ( !itemFlag2 ) {
		itemFlag2 = G_Spawn();
		itemFlag2->classname = "finish_flag";
		itemFlag2->spawnflags = 1; // suspended
		itemFlag2->target = "InternalStopTimer";
		VectorCopy( ent->client->ps.origin, itemFlag2->s.origin );
		G_CallSpawn( itemFlag2 );
		itemFlag2->wait = 1;

		stopTimer = G_Spawn();
		stopTimer->targetname = "InternalStopTimer";
		SP_target_stopTimer( stopTimer );
	} else {
		VectorCopy( ent->client->ps.origin, itemFlag2->s.pos.trBase );
		VectorCopy( itemFlag2->s.pos.trBase, itemFlag2->r.currentOrigin );
		trap_LinkEntity( itemFlag2 );
	}
}


/*
=================
Cmd_Follow_f
=================
*/
void Cmd_Follow_f( gentity_t *ent ) {
	int		i;
	char	arg[MAX_TOKEN_CHARS];

	if ( trap_Argc() != 2 ) {
		if ( ent->client->sess.spectatorState == SPECTATOR_FOLLOW )
			StopFollowing( ent, qtrue );

		return;
	}

	trap_Argv( 1, arg, sizeof( arg ) );
	i = ClientNumberFromString( ent, arg );
	if ( i == -1 )
		return;

	// can't follow self
	if ( &level.clients[ i ] == ent->client )
		return;

	// can't follow another spectator
	if ( level.clients[ i ].sess.sessionTeam == TEAM_SPECTATOR )
		return;

	// first set them to spectator
	if ( ent->client->sess.sessionTeam != TEAM_SPECTATOR )
		SetTeam( ent, "spectator" );

	ent->client->sess.spectatorState = SPECTATOR_FOLLOW;
	ent->client->sess.spectatorClient = i;
}


/*
=================
Cmd_FollowCycle_f
=================
*/
void Cmd_FollowCycle_f( gentity_t *ent, int dir ) {
	int		clientnum;
	int		original;
	gclient_t	*client;

	client = ent->client;

	// first set them to spectator
	if ( client->sess.spectatorState == SPECTATOR_NOT )
		SetTeam( ent, "spectator" );

	if ( dir != 1 && dir != -1 )
		G_Error( "Cmd_FollowCycle_f: bad dir %i", dir );

	clientnum = client->sess.spectatorClient;
	original = clientnum;
	do {
		clientnum += dir;
		if ( clientnum >= level.maxclients )
			clientnum = 0;

		if ( clientnum < 0 )
			clientnum = level.maxclients - 1;

		// can only follow connected clients
		if ( level.clients[ clientnum ].pers.connected != CON_CONNECTED )
			continue;

		// can't follow another spectator
		if ( level.clients[ clientnum ].sess.sessionTeam == TEAM_SPECTATOR )
			continue;

		// this is good, we can use it
		ent->client->sess.spectatorClient = clientnum;
		ent->client->sess.spectatorState = SPECTATOR_FOLLOW;
		return;

	} while ( clientnum != original );

	// leave it where it was
}


/*
==================
G_Say
==================
*/
static void G_SayTo( gentity_t *ent, gentity_t *other, int mode, int color, const char *name, const char *message ) {
	if (!other)
		return;

	if (!other->inuse)
		return;

	if (!other->client)
		return;

	if ( other->client->pers.connected != CON_CONNECTED )
		return;

	if ( mode == SAY_TEAM  && !OnSameTeam(ent, other) )
		return;

	trap_SendServerCommand( other-g_entities, va( "%s \"%s%c%c%s\" %i", mode == SAY_TEAM ? "tchat" : "chat", name, Q_COLOR_ESCAPE, color, message, ent - g_entities ) );
}

#define EC "\x19"

void G_Say( gentity_t *ent, gentity_t *target, int mode, const char *chatText ) {
	int			j;
	gentity_t	*other;
	int			color;
	char		name[64];
	char		text[MAX_SAY_TEXT];
	char		location[64];

	switch ( mode ) {
	default:
	case SAY_ALL:
		G_LogPrintf( "say: %s: %s\n", ent->client->pers.netname, chatText );
		Com_sprintf (name, sizeof(name), "%s%c%c"EC": ", ent->client->pers.netname, Q_COLOR_ESCAPE, COLOR_WHITE );
		color = COLOR_GREEN;
		break;
	case SAY_TEAM:
		G_LogPrintf( "sayteam: %s: %s\n", ent->client->pers.netname, chatText );
		Com_sprintf (name, sizeof(name), EC"(%s%c%c"EC")"EC": ", ent->client->pers.netname, Q_COLOR_ESCAPE, COLOR_WHITE );
		color = COLOR_CYAN;
		break;
	case SAY_TELL:
		if (target && target->inuse && target->client && target->client->sess.sessionTeam == ent->client->sess.sessionTeam)
			Com_sprintf (name, sizeof(name), EC"[%s%c%c"EC"]"EC": ", ent->client->pers.netname, Q_COLOR_ESCAPE, COLOR_WHITE );

		color = COLOR_MAGENTA;
		break;
	}

	Q_strncpyz( text, chatText, sizeof(text) );

	if ( target ) {
		G_SayTo( ent, target, mode, color, name, text );
		return;
	}

	// echo the text to the console
	if ( g_dedicated.integer )
		G_Printf( "%s%s\n", name, text);

	// send it to all the appropriate clients
	for (j = 0; j < level.maxclients; j++) {
		other = &g_entities[j];
		G_SayTo( ent, other, mode, color, name, text );
	}
}


/*
==================
Cmd_Say_f
==================
*/
static void Cmd_Say_f( gentity_t *ent, int mode, qbool arg0 ) {
	char		*p;

	if ( trap_Argc () < 2 && !arg0 )
		return;

	if (arg0)
		p = ConcatArgs( 0 );
	else
		p = ConcatArgs( 1 );

	G_Say( ent, NULL, mode, p );
}


/*
==================
Cmd_Tell_f
==================
*/
static void Cmd_Tell_f( gentity_t *ent ) {
	int			targetNum;
	gentity_t	*target;
	char		*p;
	char		arg[MAX_TOKEN_CHARS];

	if ( trap_Argc () < 2 )
		return;

	trap_Argv( 1, arg, sizeof( arg ) );
	targetNum = atoi( arg );
	if ( (unsigned)targetNum >= (unsigned)level.maxclients )
		return;

	target = &g_entities[targetNum];
	if ( !target->inuse || !target->client )
		return;

	p = ConcatArgs( 2 );

	G_LogPrintf( "tell: %s to %s: %s\n", ent->client->pers.netname, target->client->pers.netname, p );
	G_Say( ent, target, SAY_TELL, p );
	// don't tell to the player self if it was already directed to this player
	if ( ent != target )
		G_Say( ent, ent, SAY_TELL, p );

}

/*
==================
Cmd_Where_f
==================
*/
void Cmd_Where_f( gentity_t *ent ) {
	trap_SendServerCommand( ent-g_entities, va("print \"%s\n\"", vtos( ent->s.origin ) ) );
}


/*
=================
Cmd_RespawnPoint_f
=================
*/
gentity_t *SelectNearestDeathmatchSpawnPoint( vec3_t );

void Cmd_RespawnPoint_f( gentity_t *ent ) {
	int i, j;
	char buffer[MAX_STRING_CHARS];
	gentity_t *spawn;
	gclient_t *client = ent->client;

	if ( trap_Argc() >= 2 ) {
		trap_Argv( 1, buffer, sizeof(buffer) );
		if ( !Q_stricmp( buffer, "set" ) ) {
			if ( trap_PointContents( client->ps.origin, -1 ) & CONTENTS_SPAWNAREA ) {
				VectorCopy( client->ps.origin, client->pers.respawnOrigin );
				VectorCopy( client->ps.viewangles, client->pers.respawnAngles );
			} else {
				spawn = SelectNearestDeathmatchSpawnPoint( client->ps.origin );
				VectorCopy( spawn->s.origin, client->pers.respawnOrigin );
				VectorCopy( spawn->s.angles, client->pers.respawnAngles );
			}
			client->pers.useRespawnPoint = qtrue;

			Com_Printf( "Respawn point set to %f %f %f\n",
				client->pers.respawnAngles[0], client->pers.respawnAngles[1], client->pers.respawnAngles[2] );
			return;
		} else if ( !Q_stricmp( buffer, "clear" ) ) {
			client->pers.useRespawnPoint = qfalse;
			Com_Printf( "Respawn point cleared\n" );
			return;
		}
	}

	Com_Printf( "respawnpoint set\nrespawnpoint clear\n" );

}


static qbool G_MapExist( const char *map )
{
	fileHandle_t fh;
	int len;

	if ( !map || !*map )
		return qfalse;

	len = trap_FS_FOpenFile( va( "maps/%s.bsp", map ), &fh, FS_READ );

	if ( len < 0 )
		return qfalse;

	trap_FS_FCloseFile( fh );

	return ( len >= 144 ) ? qtrue : qfalse ;
}

/*
==================
ValidVoteCommand
Input string can be modified by overwriting gametype number instead of text value, for example
==================
*/

static const char *voteCommands[] = {
	"dfx_gametype",
	"dfx_obs",
	"dfx_interference",
	"dfx_reversemap",
	"dfx_ruleset",
	"dfx_mode",
	"dfx_fastcapmode",
	"map",
	"devmap",
	"kick",
	"clientkick"
};


static qbool ValidVoteCommand( int clientNum, char *command )
{
	char      buf[ MAX_CVAR_VALUE_STRING ];
	char      *base;
	char      *s;
	int       i;

	if ( strchr( command, ';' ) || strchr( command, '\n' ) || strchr( command, '\r' ) ) {
		trap_SendServerCommand( clientNum, "print \"^1Invalid vote command.\n\n\"" );
		return qfalse;
	}

	base = command;

	s = buf; // extract command name
	while ( *command != '\0' && *command != ' ' ) {
		*s = *command; s++; command++;
	}
	*s = '\0';
	// point cmd on first argument
	while ( *command == ' ' || *command == '\t' )
		command++;

	for ( i = 0; i < ARRAY_LEN( voteCommands ); i++ ) {
		if ( !Q_stricmp( buf, voteCommands[i] ) ) {
			break;
		}
	}


	if ( i == ARRAY_LEN( voteCommands )) {
		trap_SendServerCommand( clientNum, "print \"^1Invalid vote command.\n\n^2Vote commands are:\n"
			"    ^7dfx_gametype     ^2<fastcap,adventure,defrag,teamrun,freestyle>\n"
			"    ^7dfx_ruleset      ^2<vq3,cpm,xvq3,xcpm>\n"
			"    ^7dfx_mode         ^2<default,strafe,rocket,plasma,combo,ice>\n"
			"    ^7dfx_fastcapmode  ^2<default,noteles,nopads,nodoors,noteles_nopads,disableall,rocket,plasma,combo>\n"
			"    ^7dfx_obs          ^2<0,1>\n"
			"    ^7dfx_interference ^2<0,1>\n"
			"    ^7dfx_reversemap   ^2<0,1>\n\n"
			
			"    ^7map              ^2<mapname>\n"
			"    ^7devmap           ^2<mapname>\n"
			"    ^7kick             ^2<player>\n"
			"    ^7clientkick       ^2<clientnum>\n" ) ;
		return qfalse;
	}

	if ( !Q_stricmp( buf, "dfx_fastcapmode" )) {
		if (dfx_gametype.integer != GT_FASTCAP) {
			trap_SendServerCommand( clientNum, va( "print \"^1dfx_fastcapmode can only be set in GT_FASTCAP\n\"" ) );
			
		}
		if ( !Q_stricmp( command, "default" )) {
            i = FC_DEFAULT;
		} else if ( !Q_stricmp( command, "noteles" )) {
            i = FC_NOTELES;
		} else if ( !Q_stricmp( command, "nopads" )) {
            i = FC_NOPADS;
		} else if ( !Q_stricmp( command, "nodoors" )) {
            i = FC_NODOORS;
		} else if ( !Q_stricmp( command, "noteles_nopads" )) {
            i = FC_NOTELES_NOPADS;
		} else if ( !Q_stricmp( command, "disableall" )) {
            i = FC_DISABLEALL;
		} else if ( !Q_stricmp( command, "rocket" )) {
            i = FC_ROCKET;
		} else if ( !Q_stricmp( command, "plasma" )) {
            i = FC_PLASMA;
		} else if ( !Q_stricmp( command, "combo" )) {
            i = FC_COMBO;
		} else {
			i = atoi( command );
			if( i < FC_DEFAULT || i > FC_COMBO ) {
				trap_SendServerCommand( clientNum, va( "print \"^1Invalid fastcap mode %i.\n\"", i ) );
				return qfalse;
			}
		}
		
		if (i == dfx_fastcapmode.integer) {
			trap_SendServerCommand( clientNum, va( "print \"^1dfx_fastcapmode already set to %i.\n\"", i ) );
			return qfalse;	
		}

		// handle string values
		BG_sprintf( base, "dfx_fastcapmode %i", i );
		return qtrue;
	}


	if ( !Q_stricmp( buf, "dfx_mode" )) {
		if (dfx_gametype.integer != GT_DEFRAG) {
			trap_SendServerCommand( clientNum, va( "print \"^1dfx_mode can only be set in GT_DEFRAG\n\"" ) );
			return qfalse;
		}
		if ( !Q_stricmp( command, "default" )) {
            i = DF_DEFAULT;
		} else if ( !Q_stricmp( command, "strafe" )) {
            i = DF_STRAFE;
		} else if ( !Q_stricmp( command, "rocket" )) {
            i = DF_ROCKET;
		} else if ( !Q_stricmp( command, "plasma" )) {
            i = DF_PLASMA;
		} else if ( !Q_stricmp( command, "combo" )) {
            i = DF_COMBO;
		} else if ( !Q_stricmp( command, "ice" )) {
            i = DF_ICEMODE;
		} else {
			i = atoi( command );
			if( i < DF_DEFAULT || i > DF_ICEMODE ) {
				trap_SendServerCommand( clientNum, va( "print \"^1Invalid defrag mode %i.\n\"", i ) );
				return qfalse;
			}
		}
		
		if (i == dfx_mode.integer) {
			trap_SendServerCommand( clientNum, va( "print \"^1dfx_mode already set to %i.\n\"", i ) );
			return qfalse;	
		}

		// handle string values
		BG_sprintf( base, "dfx_mode %i", i );
		return qtrue;
	}

	if ( !Q_stricmp( buf, "dfx_ruleset" )) {

		if ( !Q_stricmp( command, "vq3" )) {
            i = RS_VANILLA;
		} else if ( !Q_stricmp( command, "cpm" )) {
            i = RS_CPM;
		} else if ( !Q_stricmp( command, "xvq3" )) {
            i = RS_XVANILLA;
		} else if ( !Q_stricmp( command, "xcpm" )) {
            i = RS_XCPM;
		} else {
			i = atoi( command );
			if( i < RS_VANILLA || i >= RS_MAX_RULESET ) {
				trap_SendServerCommand( clientNum, va( "print \"^1Invalid ruleset %i.\n\"", i ) );
				return qfalse;
			}
		}
		
		if (i == dfx_ruleset.integer) {
			trap_SendServerCommand( clientNum, va( "print \"^1dfx_ruleset already set to %i.\n\"", i ) );
			return qfalse;	
		}

		// handle string values
		BG_sprintf( base, "dfx_ruleset %i", i );
		return qtrue;
	}

	if ( !Q_stricmp( buf, "dfx_gametype" )) {

		if ( !Q_stricmp( command, "fastcap" )) {
            i = GT_FASTCAP;
		} else if ( !Q_stricmp( command, "adventure" )) {
            i = GT_ADVENTURE;
		} else if ( !Q_stricmp( command, "defrag" )) {
            i = GT_DEFRAG;
		} else if ( !Q_stricmp( command, "teamrun" )) {
            i = GT_TEAMRUN;
		} else if ( !Q_stricmp( command, "freestyle" )) {
            i = GT_FREESTYLE;
		} else {
			i = atoi( command );
			if( i < GT_FASTCAP || i >= GT_MAX_GAME_TYPE ) {
				trap_SendServerCommand( clientNum, va( "print \"^1Invalid gametype %i.\n\"", i ) );
				return qfalse;
			}
		}
		
		if (i == dfx_gametype.integer) {
			trap_SendServerCommand( clientNum, va( "print \"^1dfx_gametype already set to %i.\n\"", i ) );
			return qfalse;	
		}

		// handle string values
		BG_sprintf( base, "dfx_gametype %i", i );
		return qtrue;
	}

	if ( !Q_stricmp( buf, "dfx_obs" )) {
        i = atoi( command );
        if (i < 0 || i > 1){
            trap_SendServerCommand( clientNum, va( "print \"^1Invalid option %i.\n\"", i ));
            return qfalse;
        }
		
		if (i == dfx_obs.integer) {
			trap_SendServerCommand( clientNum, va( "print \"^1dfx_obs already set to %i.\n\"", i ) );
			return qfalse;	
		}

        // handle string values
		BG_sprintf( base, "dfx_obs %i", i );
		return qtrue;
	}

    if ( !Q_stricmp( buf, "dfx_interference" )) {
		if (dfx_gametype.integer != GT_FREESTYLE) {
			trap_SendServerCommand( clientNum, va( "print \"^1dfx_interference can only be set in GT_FREESTYLE\n\"" ) );
			return qfalse;
		}
		
        i = atoi( command );
        if (i < 0 || i > 1){
            trap_SendServerCommand( clientNum, va( "print \"^1Invalid option %i.\n\"", i ));
            return qfalse;
        }
		
		if (i == dfx_interference.integer) {
			trap_SendServerCommand( clientNum, va( "print \"^1dfx_interference already set to %i.\n\"", i ) );
			return qfalse;	
		}

        // handle string values
		BG_sprintf( base, "dfx_interference %i", i );
		return qtrue;
	}

	if ( !Q_stricmp( buf, "dfx_reversemap" )) {
		if (dfx_gametype.integer != GT_DEFRAG) {
			trap_SendServerCommand( clientNum, va( "print \"^1dfx_reversemap can only be set in GT_DEFRAG\n\"" ) );
			return qfalse;
		}
		
        i = atoi( command );
        if (i < 0 || i > 1){
            trap_SendServerCommand( clientNum, va( "print \"^1Invalid option %i.\n\"", i ));
            return qfalse;
        }
		
		if (i == dfx_reversemap.integer) {
			trap_SendServerCommand( clientNum, va( "print \"^1dfx_reversemap already set to %i.\n\"", i ) );
			return qfalse;	
		}

        // handle string values
		BG_sprintf( base, "dfx_reversemap %i", i );
		return qtrue;
	}

	if ( (!Q_stricmp( buf, "map" )) || ( !Q_stricmp( buf, "devmap" )) ) {
		if ( !G_MapExist( command ) ) {
			trap_SendServerCommand( clientNum, va( "print \"^1No such map on server: %s.\n\"", command ) );
			trap_SendConsoleCommand( EXEC_APPEND, va("download %s\n", command) ); // SLK: try DL map from dl_source if not existing
			return qfalse;
		}
		return qtrue;
	}
/*
	if ( (!Q_stricmp( buf, "map_restart")) && dfx_gametype.integer != GT_ADVENTURE ) {
        trap_SendServerCommand( clientNum, va( "print \"map_restart only allowed in GT_ADVENTURE\n\"" ) );
        return qfalse;
	}
*/
	return qtrue;
}


/*
==================
Cmd_CallVote_f
==================
*/
void Cmd_CallVote_f( gentity_t *ent ) {
	int		i, n;
	char	arg[MAX_STRING_TOKENS], *argn[4];
	char	cmd[MAX_STRING_TOKENS], *s;

	if (ent->client->pers.localClient){
        trap_SendServerCommand( ent-g_entities, "print \"Cannot perform a callvote as localhost.\n\"" );
		return;
	}

	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ){
        trap_SendServerCommand( ent-g_entities, "print \"Cannot perform a callvote as spectator.\n\"" );
		return;
	}

	if ( !g_allowVote.integer ) {
		trap_SendServerCommand( ent-g_entities, "print \"Voting not allowed here.\n\"" );
		return;
	}

	if ( level.voteTime ) {
		trap_SendServerCommand( ent-g_entities, "print \"A vote is already in progress.\n\"" );
		return;
	}

	// if there is still a vote to be executed
	if ( level.voteExecuteTime || level.restarted ) {
		trap_SendServerCommand( ent-g_entities, "print \"Previous vote command is waiting execution^1.^7\n\"" );
		return;
	}

	// build command buffer
	arg[ 0 ] = '\0'; s = arg;
	for ( i = 1; i < trap_Argc(); i++ ) {
		if ( arg[ 0 ] )
			s = Q_stradd( s, " " );
		trap_Argv( i, cmd, sizeof( cmd ) );
		s = Q_stradd( s, cmd );
	}

	// split by ';' seperators
	n = Com_Split( arg, argn, ARRAY_LEN( argn ), ';' );
	if ( n == 0 || *argn[0] == '\0' )
		return; // empty callvote command?

	// validate all split commands
	for ( i = 0; i < n; i++ ) {
		// make sure it is a valid command to vote on
		if ( !ValidVoteCommand( ent - g_entities, argn[i] ) ) {
			return;
		}
	}

	// rebuild command buffer
	cmd[0] = '\0';
	for ( s = cmd, i = 0; i < n; i++ ) {
		if ( cmd[0] )
			s = Q_stradd( s, ";" );
		s = Q_stradd( s, argn[ i ] );
	}

	Com_sprintf( level.voteString, sizeof( level.voteString ), cmd );
	Com_sprintf( level.voteDisplayString, sizeof( level.voteDisplayString ), "%s", level.voteString );

	trap_SendServerCommand( -1, va( "print \"%s called a vote(%s).\n\"", ent->client->pers.netname, cmd ) );

	// start the voting, the caller automatically votes yes
	level.voteTime = level.time;
	level.voteYes = ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) ? 0 : 1;
	level.voteNo = 0;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		level.clients[i].ps.eFlags &= ~EF_VOTED;
		level.clients[i].pers.voted = 0;
	}

	ent->client->ps.eFlags |= EF_VOTED;
	ent->client->pers.voted = 1;

	ent->client->pers.voteCount++;

	trap_SetConfigstring( CS_VOTE_TIME, va("%i", level.voteTime ) );
	trap_SetConfigstring( CS_VOTE_STRING, level.voteDisplayString );
	trap_SetConfigstring( CS_VOTE_YES, va("%i", level.voteYes ) );
	trap_SetConfigstring( CS_VOTE_NO, va("%i", level.voteNo ) );
}


/*
==================
Cmd_Vote_f
==================
*/
void Cmd_Vote_f( gentity_t *ent ) {
	char    msg[64];

	if ( !level.voteTime ) {
		trap_SendServerCommand( ent-g_entities, "print \"No vote in progress.\n\"" );
		return;
	}

	if ( ent->client->pers.voted != 0 ) {
		trap_SendServerCommand( ent-g_entities, "print \"Vote already cast.\n\"" );
		return;
	}

	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
		trap_SendServerCommand( ent-g_entities, "print \"Not allowed to vote as spectator.\n\"" );
		return;
	}

	trap_SendServerCommand( ent-g_entities, "print \"Vote cast.\n\"" );

	ent->client->ps.eFlags |= EF_VOTED;

	trap_Argv( 1, msg, sizeof( msg ) );

	if ( msg[0] == 'y' || msg[0] == 'Y' || msg[0] == '1' ) {
		level.voteYes++;
		ent->client->pers.voted = 1;
		trap_SetConfigstring( CS_VOTE_YES, va( "%i", level.voteYes ) );
	} else {
		level.voteNo++;
		ent->client->pers.voted = -1;
		trap_SetConfigstring( CS_VOTE_NO, va( "%i", level.voteNo ) );
	}

	// a majority will be determined in CheckVote, which will also account
	// for players entering or leaving
}


void G_RevertVote( gclient_t *client ) {
	if ( level.voteTime ) {
		if ( client->pers.voted == 1 ) {
			level.voteYes--;
			client->pers.voted = 0;
			client->ps.eFlags &= ~EF_VOTED;
			trap_SetConfigstring( CS_VOTE_YES, va( "%i", level.voteYes ) );
		} else if ( client->pers.voted == -1 ) {
			level.voteNo--;
			client->pers.voted = 0;
			client->ps.eFlags &= ~EF_VOTED;
			trap_SetConfigstring( CS_VOTE_NO, va( "%i", level.voteNo ) );
		}
	}

}

/*
=================
ClientCommand
=================
*/
void ClientCommand( int clientNum ) {
	gentity_t *ent;
	gclient_t *client;

	char	cmd[MAX_TOKEN_CHARS];

	ent = g_entities + clientNum;
	client = ent->client;

	if ( !ent->client ) {
		return;
	}

	trap_Argv( 0, cmd, sizeof( cmd ) );

	if ( ent->client->pers.connected != CON_CONNECTED ) {

		if ( ent->client->pers.connected == CON_CONNECTING && g_gametype.integer == GT_TEAMRUN ) {
			if ( Q_stricmp( cmd, "team" ) == 0 && !level.restarted ) {
				Cmd_Team_f( ent ); // early team override
			}
		}
		return;	// not fully in game yet
	}

	if (Q_stricmp (cmd, "say") == 0) {
		Cmd_Say_f (ent, SAY_ALL, qfalse);
		return;
	}

	if (Q_stricmp (cmd, "say_team") == 0) {
		if (!client->pers.localClient){
            Cmd_Say_f (ent, SAY_TEAM, qfalse);
		}
		return;
	}

	if (Q_stricmp (cmd, "tell") == 0) {
		if (!client->pers.localClient){
            Cmd_Tell_f ( ent );
		}
		return;
	}

	if (Q_stricmp (cmd, "score") == 0) {
		Cmd_Score_f (ent);
		return;
	}

	// ignore all other commands when at intermission
	if (level.intermissiontime) {
		Cmd_Say_f (ent, qfalse, qtrue);
		return;
	}

	if (Q_stricmp (cmd, "give") == 0) {
		Cmd_Give_f (ent);
	} else if (Q_stricmp (cmd, "god") == 0) {
		Cmd_God_f (ent);
	} else if (Q_stricmp (cmd, "noclip") == 0) {
		Cmd_Noclip_f (ent);
    } else if (Q_stricmp (cmd, "placeplayer") == 0) {
		Cmd_PlacePlayer_f( ent );
    } else if (Q_stricmp (cmd, "respawnpoint") == 0) {
		Cmd_RespawnPoint_f( ent );
	} else if (Q_stricmp (cmd, "kill") == 0) {

            if (dfx_gametype.integer != GT_ADVENTURE) {
                Cmd_Kill_f (ent);
            } else {
                trap_SendConsoleCommand( EXEC_APPEND, "map_restart 1\n" );
            }
	} else if (Q_stricmp (cmd, "follow") == 0) {
            if (!client->pers.localClient) {
                Cmd_Follow_f (ent);
            }
	} else if (Q_stricmp (cmd, "follownext") == 0) {
            if (!client->pers.localClient) {
                Cmd_FollowCycle_f (ent, 1);
            }
	} else if (Q_stricmp (cmd, "followprev") == 0) {
		Cmd_FollowCycle_f (ent, -1);
	} else if (Q_stricmp (cmd, "team") == 0) {
		Cmd_Team_f (ent);
	} else if (Q_stricmp (cmd, "where") == 0) {
		Cmd_Where_f (ent);
	} else if ( (Q_stricmp (cmd, "callvote") == 0) || (Q_stricmp (cmd, "cv") == 0)) {
            if (!client->pers.localClient) {
                Cmd_CallVote_f (ent);
            }
	} else if (Q_stricmp (cmd, "vote") == 0) {
            if (!client->pers.localClient) {
                Cmd_Vote_f (ent);
            }
	} else if (Q_stricmp (cmd, "starttimer") == 0) {
		Cmd_StartTimer_f( ent );
	} else if (Q_stricmp (cmd, "stoptimer") == 0) {
		Cmd_StopTimer_f( ent );
	} else {
		trap_SendServerCommand( clientNum, va( "print \"unknown cmd %s\n\"", cmd ) );
	}
}
