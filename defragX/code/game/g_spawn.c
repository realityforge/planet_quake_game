// Copyright (C) 1999-2000 Id Software, Inc.
//

#include "g_local.h"

qbool	G_SpawnString( const char *key, const char *defaultString, char **out )
{
	int		i;

	if ( !level.spawning )
    {
		*out = (char *)defaultString;
	}

	for ( i = 0 ; i < level.numSpawnVars ; i++ )
    {
		if ( !Q_stricmp( key, level.spawnVars[i][0] ) )
		{
			*out = level.spawnVars[i][1];
			return qtrue;
		}
	}

	*out = (char *)defaultString;
	return qfalse;
}

qbool	G_SpawnFloat( const char *key, const char *defaultString, float *out )
{
	char		*s;
	qbool	present;

	present = G_SpawnString( key, defaultString, &s );
	*out = atof( s );
	return present;
}

qbool	G_SpawnInt( const char *key, const char *defaultString, int *out )
{
	char		*s;
	qbool	present;

	present = G_SpawnString( key, defaultString, &s );
	*out = atoi( s );
	return present;
}

qbool	G_SpawnVector( const char *key, const char *defaultString, float *out )
{
	char		*s;
	qbool	present;

	present = G_SpawnString( key, defaultString, &s );
	Q_sscanf( s, "%f %f %f", &out[0], &out[1], &out[2] );
	return present;
}



// fields are needed for spawning from the entity string
//
typedef enum {
	F_INT,
	F_FLOAT,
	F_LSTRING,			// string on disk, pointer in memory, TAG_LEVEL
	F_GSTRING,			// string on disk, pointer in memory, TAG_GAME
	F_VECTOR,
	F_ANGLEHACK,
	F_ENTITY,			// index on disk, pointer in memory
	F_ITEM,				// index on disk, pointer in memory
	F_CLIENT,			// index on disk, pointer in memory
	F_IGNORE
} fieldtype_t;

typedef struct
{
	const char *name;
	int		    ofs;
	fieldtype_t	type;
} field_t;

const field_t fields[] = {
	{"classname",           FOFS(classname),            F_LSTRING},
	{"origin",              FOFS(s.origin),             F_VECTOR},
	{"model",               FOFS(model),                F_LSTRING},
	{"model2",              FOFS(model2),               F_LSTRING},
	{"spawnflags",          FOFS(spawnflags),           F_INT},
	{"speed",               FOFS(speed),                F_FLOAT},
	{"target",              FOFS(target),               F_LSTRING},
	{"targetname",          FOFS(targetname),           F_LSTRING},
	{"message",             FOFS(message),              F_LSTRING},
	{"team",                FOFS(team),                 F_LSTRING},
	{"wait",                FOFS(wait),                 F_FLOAT},
	{"random",              FOFS(random),               F_FLOAT},
	{"count",               FOFS(count),                F_INT},
	{"health",              FOFS(health),               F_INT},
	{"light",               0,                          F_IGNORE},
	{"dmg",                 FOFS(damage),               F_INT},
	{"angles",              FOFS(s.angles),             F_VECTOR},
	{"angle",               FOFS(s.angles),             F_ANGLEHACK},
	{"targetShaderName",    FOFS(targetShaderName),     F_LSTRING},
	{"targetShaderNewName", FOFS(targetShaderNewName),  F_LSTRING},
	{"distance",            FOFS(distance),             F_FLOAT},
    {"music",               FOFS(music),                F_LSTRING},
	{NULL}
};


typedef struct {
	char	*name;
	void	(*spawn)(gentity_t *ent);
} spawn_t;

void SP_info_player_start (gentity_t *ent);
void SP_info_player_red (gentity_t *ent);
void SP_info_player_blue (gentity_t *ent);
void SP_info_player_green (gentity_t *ent);
void SP_info_player_yellow (gentity_t *ent);
void SP_info_player_intermission (gentity_t *ent);

void SP_func_plat (gentity_t *ent);
void SP_func_static (gentity_t *ent);
void SP_func_rotating (gentity_t *ent);
void SP_func_bobbing (gentity_t *ent);
void SP_func_pendulum( gentity_t *ent );
void SP_func_button (gentity_t *ent);
void SP_func_door (gentity_t *ent);
void SP_func_door_rotating( gentity_t *ent );
void SP_func_train (gentity_t *ent);
void SP_func_timer (gentity_t *self);
void SP_func_breakable (gentity_t *ent);

void SP_trigger_always (gentity_t *ent);
void SP_trigger_multiple (gentity_t *ent);
void SP_trigger_push (gentity_t *ent);
void SP_trigger_push_velocity (gentity_t *ent);
void SP_trigger_teleport (gentity_t *ent);
void SP_trigger_hurt (gentity_t *ent);
void SP_trigger_physics (gentity_t *ent);

void SP_target_remove_powerups( gentity_t *ent );
void SP_target_give (gentity_t *ent);
void SP_target_delay (gentity_t *ent);
void SP_target_speaker (gentity_t *ent);
void SP_target_print (gentity_t *ent);
void SP_target_laser (gentity_t *self);
void SP_target_score( gentity_t *ent );
void SP_target_teleporter( gentity_t *ent );
void SP_target_relay (gentity_t *ent);
void SP_target_kill (gentity_t *ent);
void SP_target_position (gentity_t *ent);
void SP_target_push (gentity_t *ent);
void SP_target_startTimer (gentity_t *ent);
void SP_target_stopTimer (gentity_t *ent);
void SP_target_checkpoint (gentity_t *ent);
void SP_target_speed (gentity_t *ent);
void SP_target_init (gentity_t *ent);
void SP_target_earthquake (gentity_t *ent);
void SP_target_effect (gentity_t *ent);
void SP_target_secret (gentity_t *ent);
void SP_target_music (gentity_t *ent);
void SP_target_fragsFilter (gentity_t *ent);

void SP_light (gentity_t *self);
void SP_info_null (gentity_t *self);
void SP_path_corner (gentity_t *self);

void SP_misc_teleporter_dest (gentity_t *self);
void SP_misc_model(gentity_t *ent);
void SP_misc_portal_camera(gentity_t *ent);
void SP_misc_portal_surface(gentity_t *ent);

void SP_shooter_rocket( gentity_t *ent );
void SP_shooter_plasma( gentity_t *ent );
void SP_shooter_grenade( gentity_t *ent );
void SP_shooter_bfg( gentity_t *ent );

spawn_t	spawns[] = {
	// info entities don't do anything at all, but provide positional
	// information for things controlled by other processes
	{"team_CTF_redplayer",          SP_info_player_start},
	{"team_CTF_blueplayer",         SP_info_player_start},
	{"team_CTF_redspawn",           SP_info_player_start},
	{"team_CTF_bluespawn",          SP_info_player_start},

	{"info_player_deathmatch",      SP_info_player_start},
	{"info_player_start",           SP_info_player_start},

	{"info_player_red",             SP_info_player_red},
	{"info_player_blue",            SP_info_player_blue},
	{"info_player_green",           SP_info_player_green},
	{"info_player_yellow",          SP_info_player_yellow},

	{"info_player_intermission",    SP_info_player_intermission},

	{"info_null",                   SP_info_null},
	{"info_notnull",                SP_target_position},//SP_info_notnull},		// use target_position instead
	{"info_camp",                   SP_target_position},//SP_info_camp},


	{"func_plat",                   SP_func_plat},
	{"func_button",                 SP_func_button},
	{"func_door",                   SP_func_door},
	{"func_door_rotating",          SP_func_door_rotating},
	{"func_static",                 SP_func_static},
	{"func_rotating",               SP_func_rotating},
	{"func_bobbing",                SP_func_bobbing},
	{"func_pendulum",               SP_func_pendulum},
	{"func_train",                  SP_func_train},
	{"func_group",                  SP_info_null},
	{"func_timer",                  SP_func_timer},
	{"func_breakable",              SP_func_breakable},

	{"trigger_always",              SP_trigger_always},
	{"trigger_multiple",            SP_trigger_multiple},
	{"trigger_push",                SP_trigger_push},
	{"trigger_push_velocity",       SP_trigger_push_velocity},
	{"trigger_teleport",            SP_trigger_teleport},
	{"trigger_hurt",                SP_trigger_hurt},
	{"trigger_physics",             SP_trigger_physics},

	{"target_give",                 SP_target_give},
	{"target_remove_powerups",      SP_target_remove_powerups},
	{"target_delay",                SP_target_delay},
	{"target_speaker",              SP_target_speaker},
	{"target_print",                SP_target_print},
	{"target_smallprint",           SP_target_print},
	{"target_laser",                SP_target_laser},
	{"target_score",                SP_target_score},
	{"target_teleporter",           SP_target_teleporter},
	{"target_relay",                SP_target_relay},
	{"target_kill",                 SP_target_kill},
	{"target_position",             SP_target_position},
	{"target_push",                 SP_target_push},
	{"target_startTimer",           SP_target_startTimer},
	{"target_stopTimer",            SP_target_stopTimer},
	{"target_checkpoint",           SP_target_checkpoint},
	{"target_speed",                SP_target_speed},
	{"target_init",                 SP_target_init},
	{"target_earthquake",           SP_target_earthquake},
	{"target_effect",               SP_target_effect},
	{"target_secret",               SP_target_secret},
	{"target_music",                SP_target_music},
	{"target_fragsFilter",          SP_target_fragsFilter},

	{"light",                       SP_light},
	{"path_corner",                 SP_path_corner},

	{"misc_teleporter_dest",        SP_misc_teleporter_dest},
	{"misc_model",                  SP_misc_model},
	{"misc_portal_surface",         SP_misc_portal_surface},
	{"misc_portal_camera",          SP_misc_portal_camera},

	{"shooter_rocket",              SP_shooter_rocket},
	{"shooter_grenade",             SP_shooter_grenade},
	{"shooter_plasma",              SP_shooter_plasma},
	{"shooter_bfg",                 SP_shooter_bfg},

	{0, 0}
};

/*
===============
G_CallSpawn

Finds the spawn function for the entity and calls it,
returning qfalse if not found
===============
*/
qbool G_CallSpawn( gentity_t *ent )
{
	spawn_t	*s;
	gitem_t	*item;

	if ( !ent->classname )
    {
		G_Printf ("G_CallSpawn: NULL classname\n");
		return qfalse;
	}

	// check item spawn functions
	for ( item = bg_itemlist + 1 ; item->classname ; item++ )
    {
		if ( !strcmp(item->classname, ent->classname) )
		{
			G_SpawnItem( ent, item );
			return qtrue;
		}
	}

	// check normal spawn functions
	for ( s = spawns ; s->name ; s++ )
    {
		if ( !strcmp(s->name, ent->classname) )
		{
			// found it
			s->spawn(ent);
			return qtrue;
		}
	}
#ifdef DEVBUILD	
	G_Printf ("%s doesn't have a spawn function\n", ent->classname);
#endif
	return qfalse;
}

/*
=============
G_NewString

Builds a copy of the string, translating \n to real linefeeds
so message texts can be multi-line
=============
*/
static char *G_NewString( const char *string )
{
	char	*newb, *new_p;
	int		i, l;

	l = (int)strlen(string) + 1;

	newb = G_Alloc( l );

	new_p = newb;

	// turn \n into a real linefeed
	for ( i=0 ; i< l ; i++ )
    {
		if (string[i] == '\\' && i < l-1)
		{
			i++;
			if (string[i] == 'n')
			{
				*new_p++ = '\n';
			}
            else
            {
				*new_p++ = '\\';
			}
		}
        else
        {
			*new_p++ = string[i];
		}
	}

	return newb;
}




/*
===============
G_ParseField

Takes a key/value pair and sets the binary values
in a gentity
===============
*/
static void G_ParseField( const char *key, const char *value, gentity_t *ent )
{
	const field_t *f;
	byte	*b;
	float	v;
	vec3_t	vec;

	for ( f=fields ; f->name ; f++ )
    {
		if ( !Q_stricmp(f->name, key) )
		{
			// found it
			b = (byte *)ent;

			switch( f->type ) {
			case F_LSTRING:
				*(char **)(b+f->ofs) = G_NewString (value);
				break;
			case F_VECTOR:
				Q_sscanf (value, "%f %f %f", &vec[0], &vec[1], &vec[2]);
				((float *)(b+f->ofs))[0] = vec[0];
				((float *)(b+f->ofs))[1] = vec[1];
				((float *)(b+f->ofs))[2] = vec[2];
				break;
			case F_INT:
				*(int *)(b+f->ofs) = atoi(value);
				break;
			case F_FLOAT:
				*(float *)(b+f->ofs) = atof(value);
				break;
			case F_ANGLEHACK:
				v = atof(value);
				((float *)(b+f->ofs))[0] = 0;
				((float *)(b+f->ofs))[1] = v;
				((float *)(b+f->ofs))[2] = 0;
				break;
			default:
			case F_IGNORE:
				break;
			}
			return;
		}
	}
}


/*
===================
G_SpawnGEntityFromSpawnVars

Spawn an entity and fill in all of the level fields from
level.spawnVars[], then call the class specfic spawn function
===================
*/
static void G_SpawnGEntityFromSpawnVars( void )
{
	int			i;
	extern int  pm_nodoors, pm_reversemap;
	gentity_t	*ent;
	char		*s, *value, *gametypeName;

	static char *gametypeNames[][4] = {
		{"fastcap", "ctf", "oneflag", "foobar"},
		{"adventure", "single", "foobar", "foobar"},
		{"defrag", "foobar", "foobar", "foobar"},
		{"team", "foobar", "foobar", "foobar"},
		{"freestyle", "tdm", "ffa", "tourney"}
	};

	// get the next free entity
	ent = G_Spawn();

	for ( i = 0 ; i < level.numSpawnVars ; i++ )
    {
		G_ParseField( level.spawnVars[i][0], level.spawnVars[i][1], ent );
	}

	if( G_SpawnString( "gametype", NULL, &value ) )
    {
		if( dfx_gametype.integer >= GT_FASTCAP && dfx_gametype.integer < GT_MAX_GAME_TYPE )
		{
			for(i = 0; i < 3; i++) {
				gametypeName = gametypeNames[dfx_gametype.integer][i];
				s = strstr( value, gametypeName );
				if( s ) {
					break;
				}
			}

			if( !s )
            {
                ADJUST_AREAPORTAL();
                G_FreeEntity( ent );
				return;
			}
		}
	}

	// check for "notcpm" flag
    G_SpawnInt( "notcpm", "0", &i );
    if ( i && ( dfx_ruleset.integer == RS_CPM) ) {
        ADJUST_AREAPORTAL();
        G_FreeEntity( ent );
        return;
    }

	// check for "notvq3" flag
    G_SpawnInt( "notvq3", "0", &i );
    if ( i && ( dfx_ruleset.integer == RS_VANILLA)) {
        ADJUST_AREAPORTAL();
        G_FreeEntity( ent );
        return;
    }

    // same for extended rulesets
    G_SpawnInt( "notxcpm", "0", &i );
    if ( i && ( dfx_ruleset.integer == RS_XCPM)) {
        ADJUST_AREAPORTAL();
        G_FreeEntity( ent );
        return;
    }

    G_SpawnInt( "notxvq3", "0", &i );
    if ( i && ( dfx_ruleset.integer == RS_XVANILLA)) {
        ADJUST_AREAPORTAL();
        G_FreeEntity( ent );
        return;
    }



//-------------------------------------------------------------------------------

    // SLK remove flags (also default q3ctf ones, when not in GT_FASTCAP
    if ( dfx_gametype.integer != GT_FASTCAP ) {
        if (!Q_stricmp(ent->classname, "start_flag")) {
            G_FreeEntity( ent );
            return;
        } else if (!Q_stricmp(ent->classname, "finish_flag")) {
            G_FreeEntity( ent );
            return;
        }

        if (!Q_stricmp(ent->classname, "team_CTF_redflag")) {
            G_FreeEntity( ent );
            return;
        } else if (!Q_stricmp(ent->classname, "team_CTF_blueflag")) {
            G_FreeEntity( ent );
            return;
        }
	}

	// SLK convert Teamrun spawns to normal spawns if not in teamrun
	if ( dfx_gametype.integer != GT_TEAMRUN ) {
		if (!Q_stricmp(ent->classname, "info_player_red")) {
			ent->classname = "info_player_start";
			return;
		} else if (!Q_stricmp(ent->classname, "info_player_blue")) {
			ent->classname = "info_player_start";
			return;
		} else if (!Q_stricmp(ent->classname, "info_player_green")) {
			ent->classname = "info_player_start";
			return;
		} else if (!Q_stricmp(ent->classname, "info_player_yellow")) {
			ent->classname = "info_player_start";
			return;
		}
	// otherwise let the game know theres a teamspawn, only add ONE and ignore others
	} else {
		
		
		// if we already found a spawn for red/blue/green/yellow free double spawns, they would mess up
		if (!Q_stricmp(ent->classname, "info_player_red") && level.redSpawnSpot) {
			G_FreeEntity( ent );
			return;
		} else if (!Q_stricmp(ent->classname, "info_player_blue") && level.blueSpawnSpot) {
			G_FreeEntity( ent );
			return;
		} else if (!Q_stricmp(ent->classname, "info_player_green") && level.greenSpawnSpot) {
			G_FreeEntity( ent );
			return;
		} else if (!Q_stricmp(ent->classname, "info_player_yellow") && level.yellowSpawnSpot) {
			G_FreeEntity( ent );
			return;
		}
		
		if (!Q_stricmp(ent->classname, "info_player_red") && !level.redSpawnSpot) {
			level.redSpawnSpot = qtrue;
			return;
		} else if (!Q_stricmp(ent->classname, "info_player_blue") && !level.blueSpawnSpot) {
			level.blueSpawnSpot = qtrue;
			return;
		} else if (!Q_stricmp(ent->classname, "info_player_green") && !level.greenSpawnSpot) {
			level.greenSpawnSpot = qtrue;
			return;
		} else if (!Q_stricmp(ent->classname, "info_player_yellow") && !level.yellowSpawnSpot) {
			level.yellowSpawnSpot = qtrue;
			return;
		}
	}

	// SLK remove start/stoptimer when in GT_FASTCAP or GT_FREESTYLE
    if ( dfx_gametype.integer == GT_FASTCAP || dfx_gametype.integer == GT_FREESTYLE ) {
        if (!Q_stricmp(ent->classname, "target_startTimer")) {
            G_FreeEntity( ent );
            return;
        } else if (!Q_stricmp(ent->classname, "target_stopTimer")) {
            G_FreeEntity( ent );
            return;
        }
    }

    // SLK also remove target_checkpoint when in GT_FASTCAP, GT_ADVENTURE or GT_FREESTYLE
    if ( dfx_gametype.integer == GT_FASTCAP || dfx_gametype.integer == GT_ADVENTURE || dfx_gametype.integer == GT_FREESTYLE ) {
        if (!Q_stricmp(ent->classname, "target_checkpoint")) {
            G_FreeEntity( ent );
            return;
        }
    }

	/*
    // SLK remove unneeded target_location entity
    if (!Q_stricmp(ent->classname, "target_location")) {
        G_FreeEntity( ent );
        return;
    }
	*/

    // SLK remove unneeded holdable_teleporter entity
    if (!Q_stricmp(ent->classname, "holdable_teleporter")) {
        G_FreeEntity( ent );
        return;
    }


	// SLK remove doors if in fastcap and doors are disabled
	if ( dfx_gametype.integer == GT_FASTCAP && pm_nodoors && (!Q_stricmp(ent->classname, "func_door") || !Q_stricmp(ent->classname, "func_door_rotating") ) ) {
		ent->s.eType = ET_MOVER;
		trap_SetBrushModel( ent, ent->model );
		ADJUST_AREAPORTAL();
		G_FreeEntity( ent );
		return;
	}
	
	//-OR- if in defrag and dfx_reversemap is active
	if ( dfx_gametype.integer == GT_DEFRAG && pm_nodoors && (!Q_stricmp(ent->classname, "func_door") || !Q_stricmp(ent->classname, "func_door_rotating") ) ) {
		ent->s.eType = ET_MOVER;
		trap_SetBrushModel( ent, ent->model );
		ADJUST_AREAPORTAL();
		G_FreeEntity( ent );
		return;
	}

	// move editor origin to pos
	VectorCopy( ent->s.origin, ent->s.pos.trBase );
	VectorCopy( ent->s.origin, ent->r.currentOrigin );

	// if we didn't get a classname, don't bother spawning anything
	if ( !G_CallSpawn( ent ) )
		G_FreeEntity( ent );

}



/*
====================
G_AddSpawnVarToken
====================
*/
static char *G_AddSpawnVarToken( const char *string )
{
	int		l;
	char	*dest;

	l = (int)strlen( string );
	if ( level.numSpawnVarChars + l + 1 > MAX_SPAWN_VARS_CHARS )
    {
		G_Error( "G_AddSpawnVarToken: MAX_SPAWN_VARS_CHARS" );
	}

	dest = level.spawnVarChars + level.numSpawnVarChars;
	memcpy( dest, string, l+1 );

	level.numSpawnVarChars += l + 1;

	return dest;
}

/*
====================
G_ParseSpawnVars

Parses a brace bounded set of key / value pairs out of the
level's entity strings into level.spawnVars[]

This does not actually spawn an entity.
====================
*/
static qbool G_ParseSpawnVars( void )
{
	char	keyname[MAX_TOKEN_CHARS];
	char	com_token[MAX_TOKEN_CHARS];

	level.numSpawnVars = 0;
	level.numSpawnVarChars = 0;

	// parse the opening brace
	if ( !trap_GetEntityToken( com_token, sizeof( com_token ) ) )
    {
		// end of spawn string
		return qfalse;
	}
	if ( com_token[0] != '{' )
	{
		G_Error( "G_ParseSpawnVars: found %s when expecting {",com_token );
	}

	// go through all the key / value pairs
	while ( 1 )
    {
		// parse key
		if ( !trap_GetEntityToken( keyname, sizeof( keyname ) ) )
		{
			G_Error( "G_ParseSpawnVars: EOF without closing brace" );
		}

		if ( keyname[0] == '}' )
        {
			break;
		}

		// parse value
		if ( !trap_GetEntityToken( com_token, sizeof( com_token ) ) )
        {
			G_Error( "G_ParseSpawnVars: EOF without closing brace" );
		}

		if ( com_token[0] == '}' )
        {
			G_Error( "G_ParseSpawnVars: closing brace without data" );
		}

		if ( level.numSpawnVars == MAX_SPAWN_VARS )
        {
			G_Error( "G_ParseSpawnVars: MAX_SPAWN_VARS" );
		}

		level.spawnVars[ level.numSpawnVars ][0] = G_AddSpawnVarToken( keyname );
		level.spawnVars[ level.numSpawnVars ][1] = G_AddSpawnVarToken( com_token );
		level.numSpawnVars++;
	}

	return qtrue;
}



/*QUAKED worldspawn (0 0 0) ?

Every map should have exactly one worldspawn.
"music"		music wav file
"gravity"	800 is default gravity
"message"	Text to print during connection process
*/
static void SP_worldspawn( void )
{
	char	*s;

	G_SpawnString( "classname", "", &s );
	if ( Q_stricmp( s, "worldspawn" ) )
    {
		G_Error( "SP_worldspawn: The first entity isn't 'worldspawn'" );
	}

	// make some data visible to connecting client
	trap_SetConfigstring( CS_GAME_VERSION, GAME_VERSION );

	trap_SetConfigstring( CS_LEVEL_START_TIME, va("%i", level.startTime ) );

	if (G_SpawnString( "music", "", &s ) )              // background music that is looping
	{
        trap_SetConfigstring( CS_MUSIC, s );
	}

	if (G_SpawnString( "message", "", &s ))
    {
        trap_SetConfigstring( CS_MESSAGE, s );			// map specific welcome message
    }


	trap_SetConfigstring( CS_MOTD, g_motd.string );		// message of the day

	if (G_SpawnString( "gravity", "800", &s ))          // set the worlds gravity value
    {
        trap_Cvar_Set( "g_gravity", s );
    } else {
		trap_Cvar_Set( "g_gravity", "800" );			// if not set, set to default to prevent altered gravity on mapchange
	}

	g_entities[ENTITYNUM_WORLD].s.number   = ENTITYNUM_WORLD;
	g_entities[ENTITYNUM_WORLD].r.ownerNum = ENTITYNUM_NONE;
	g_entities[ENTITYNUM_WORLD].classname  = "worldspawn";

	g_entities[ENTITYNUM_NONE].s.number   = ENTITYNUM_NONE;
	g_entities[ENTITYNUM_NONE].r.ownerNum = ENTITYNUM_NONE;
	g_entities[ENTITYNUM_NONE].classname  = "nothing";

	// see if we want a warmup time
	// SLK: always set warmup to 0
    if ( g_restarted.integer || dfx_gametype.integer == GT_TEAMRUN )
    {
		trap_Cvar_Set( "g_restarted", "0" );
		level.warmupTime = 0;
		trap_SetConfigstring( CS_WARMUP, "" );
	}
	else
    {
		// assume that g_doWarmup is always 1
		level.warmupTime = -1;
		trap_SetConfigstring( CS_WARMUP, "" );

		G_LogPrintf( "Warmup:\n" );
	}

}


/*
==============
CheckFragLimit

If a target_fragsFilter points to a target_stopTimer, set fraglimit to the fragsFilter's count
==============
*/
static void G_CheckFraglimit( void ) {
	gentity_t *stopTimer = NULL, *fragsFilter = NULL;

	while ( (stopTimer = G_Find (stopTimer, FOFS(classname), "target_stopTimer")) != NULL ) {
		while ( (fragsFilter = G_Find (fragsFilter, FOFS(target), stopTimer->targetname)) != NULL ) {
			if ( !Q_stricmp( fragsFilter->classname, "target_fragsFilter" ) ) {
				trap_Cvar_Set( "fraglimit", va("%d", fragsFilter->count) );
				return;
			}
		}
	}
}



/*
==============
G_SpawnEntitiesFromString

Parses textual entity definitions out of an entstring and spawns gentities.
==============
*/
void G_SpawnEntitiesFromString( void )
{
	// allow calls to G_Spawn*()
	level.spawning = qtrue;
	level.numSpawnVars = 0;

	// the worldspawn is not an actual entity, but it still
	// has a "spawn" function to perform any global setup
	// needed by a level (setting configstrings or cvars, etc)
	if ( !G_ParseSpawnVars() )
		G_Error( "SpawnEntities: no entities" );

	SP_worldspawn();


	// parse ents
	while( G_ParseSpawnVars() )
		G_SpawnGEntityFromSpawnVars();


	level.spawning = qfalse;	// any future calls to G_Spawn*() will be errors

	G_CheckFraglimit();
}
