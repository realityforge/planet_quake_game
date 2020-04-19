/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
// g_local.h -- local definitions for game module

#ifndef G_LOCAL_H
#define G_LOCAL_H

#include <q_shared.h>
#include "bg_public.h"
#include "g_public.h"


#include "client.h"

//==================================================================

// the "gameversion" client command will print this plus compile date
#define	GAMEVERSION	BASEGAME

#define BODY_QUEUE_SIZE		8

#define INFINITE			1000000

#define	FRAMETIME			100					// msec
#define	CARNAGE_REWARD_TIME	3000
#define REWARD_SPRITE_TIME	2000

#define	INTERMISSION_DELAY_TIME	1000
#define	SP_INTERMISSION_DELAY_TIME	5000

// gentity->flags
#define	FL_GODMODE				0x00000010
#define	FL_NOTARGET				0x00000020
#define	FL_TEAMSLAVE			0x00000400	// not the first on the team
#define FL_NO_KNOCKBACK			0x00000800
#define FL_DROPPED_ITEM			0x00001000
#define FL_NO_BOTS				0x00002000	// spawn point not for bot use
#define FL_NO_HUMANS			0x00004000	// spawn point just for bots
#define FL_FORCE_GESTURE		0x00008000	// force gesture on client

// movers are things like doors, plats, buttons, etc
typedef enum {
	MOVER_POS1,
	MOVER_POS2,
	MOVER_1TO2,
	MOVER_2TO1
} moverState_t;

#define SP_PODIUM_MODEL		"models/mapobjects/podium/podium4.md3"

//============================================================================

typedef struct gentity_s gentity_t;
typedef struct gclient_s gclient_t;

struct gentity_s {
	entityState_t	s;				// communicated by server to clients
	entityShared_t	r;				// shared by both the server system and game

	// DO NOT MODIFY ANYTHING ABOVE THIS, THE SERVER
	// EXPECTS THE FIELDS IN THAT ORDER!
	//================================

	struct gclient_s	*client;			// NULL if not a client

	qboolean	inuse;

	char		*classname;			// set in QuakeEd
	int			spawnflags;			// set in QuakeEd

	qboolean	neverFree;			// if true, FreeEntity will only unlink
									// bodyque uses this

	int			flags;				// FL_* variables

	char		*model;
	char		*model2;
	int			freetime;			// level.time when the object was freed
	
	int			eventTime;			// events will be cleared EVENT_VALID_MSEC after set
	qboolean	freeAfterEvent;
	qboolean	unlinkAfterEvent;

	qboolean	physicsObject;		// if true, it can be pushed by movers and fall off edges
									// all game items are physicsObjects, 
	float		physicsBounce;		// 1.0 = continuous bounce, 0.0 = no bounce
	int			clipmask;			// brushes with this content value will be collided against
									// when moving.  items and corpses do not collide against
									// players, for instance

	// movers
	moverState_t moverState;
	int			soundPos1;
	int			sound1to2;
	int			sound2to1;
	int			soundPos2;
	int			soundLoop;
	gentity_t	*parent;
	gentity_t	*nextTrain;
	gentity_t	*prevTrain;
	vec3_t		pos1, pos2;

	char		*message;

	int			timestamp;		// body queue sinking, etc

	char		*target;
	char		*targetname;
	char		*team;
	char		*targetShaderName;
	char		*targetShaderNewName;
	gentity_t	*target_ent;

	float		speed;
	vec3_t		movedir;

	int			nextthink;
	void		(*think)(gentity_t *self);
	void		(*reached)(gentity_t *self);	// movers call this when hitting endpoint
	void		(*blocked)(gentity_t *self, gentity_t *other);
	void		(*touch)(gentity_t *self, gentity_t *other, trace_t *trace);
	void		(*use)(gentity_t *self, gentity_t *other, gentity_t *activator);
	void		(*pain)(gentity_t *self, gentity_t *attacker, int damage);
	void		(*die)(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod);

	int			pain_debounce_time;
	int			fly_sound_debounce_time;	// wind tunnel
	int			last_move_time;

	int			health;

	qboolean	takedamage;

	int			damage;
	int			splashDamage;	// quad will increase this without increasing radius
	int			splashRadius;
	int			methodOfDeath;
	int			splashMethodOfDeath;

	int			count;

	gentity_t	*chain;
	gentity_t	*enemy;
	gentity_t	*activator;
	gentity_t	*teamchain;		// next entity in team
	gentity_t	*teammaster;	// master of the team

#ifdef MISSIONPACK
	int			kamikazeTime;
	int			kamikazeShockTime;
#endif

	int			watertype;
	int			waterlevel;

	int			noise_index;

	// timing variables
	float		wait;
	float		random;

	gitem_t		*item;			// for bonus items
};


typedef enum {
	CON_DISCONNECTED,
	CON_CONNECTING,
	CON_CONNECTED
} clientConnected_t;

typedef enum {
	SPECTATOR_NOT,
	SPECTATOR_FREE,
	SPECTATOR_FOLLOW,
	SPECTATOR_SCOREBOARD
} spectatorState_t;

typedef enum {
	TEAM_BEGIN,		// Beginning a team game, spawn at base
	TEAM_ACTIVE		// Now actively playing
} playerTeamStateState_t;

typedef struct {
	playerTeamStateState_t	state;

	int			location;

	int			captures;
	int			basedefense;
	int			carrierdefense;
	int			flagrecovery;
	int			fragcarrier;
	int			assists;

	float		lasthurtcarrier;
	float		lastreturnedflag;
	float		flagsince;
	float		lastfraggedcarrier;
} playerTeamState_t;

// client data that stays across multiple levels or tournament restarts
// this is achieved by writing all the data to cvar strings at game shutdown
// time and reading them back at connection time.  Anything added here
// MUST be dealt with in G_InitSessionData() / G_ReadSessionData() / G_WriteSessionData()
typedef struct {
	team_t		sessionTeam;
	int			spectatorNum;		// for determining next-in-line to play
	spectatorState_t	spectatorState;
	int			spectatorClient;	// for chasecam and follow mode
	int			wins, losses;		// tournament stats
	qboolean	teamLeader;			// true when this client is a team leader
} clientSession_t;

//
#define MAX_NETNAME			36
#define	MAX_VOTE_COUNT		3

// client data that stays across multiple respawns, but is cleared
// on each level change or team change at ClientBegin()
typedef struct {
	clientConnected_t	connected;	
	usercmd_t	cmd;				// we would lose angles if not persistant
	qboolean	localClient;		// true if "ip" info key is "localhost"
	qboolean	initialSpawn;		// the first spawn should be at a cool location
	qboolean	predictItemPickup;	// based on cg_predictItems userinfo
	qboolean	pmoveFixed;			//
	char		netname[MAX_NETNAME];
	int			maxHealth;			// for handicapping
	int			enterTime;			// level.time the client entered the game
	playerTeamState_t teamState;	// status in teamplay games
	int			voteCount;			// to prevent people from constantly calling votes
	int			teamVoteCount;		// to prevent people from constantly calling votes
	qboolean	teamInfo;			// send team overlay updates?
} clientPersistant_t;


// this structure is cleared on each ClientSpawn(),
// except for 'client->pers' and 'client->sess'
struct gclient_s {
	// ps MUST be the first element, because the server expects it
	playerState_t	ps;				// communicated by server to clients

	// the rest of the structure is private to game
	clientPersistant_t	pers;
	clientSession_t		sess;

	qboolean	readyToExit;		// wishes to leave the intermission

	qboolean	noclip;

	int			lastCmdTime;		// level.time of last usercmd_t, for EF_CONNECTION
									// we can't just use pers.lastCommand.time, because
									// of the g_sycronousclients case
	int			buttons;
	int			oldbuttons;
	int			latched_buttons;

	vec3_t		oldOrigin;

	// sum up damage over an entire frame, so
	// shotgun blasts give a single big kick
	int			damage_armor;		// damage absorbed by armor
	int			damage_blood;		// damage taken out of health
	int			damage_knockback;	// impact damage
	vec3_t		damage_from;		// origin for vector calculation
	qboolean	damage_fromWorld;	// if true, don't use the damage_from vector

	int			accurateCount;		// for "impressive" reward sound

	int			accuracy_shots;		// total number of shots
	int			accuracy_hits;		// total number of hits

	//
	int			lastkilled_client;	// last client that this client killed
	int			lasthurt_client;	// last client that damaged this client
	int			lasthurt_mod;		// type of damage the client did

	// timers
	int			respawnTime;		// can respawn when time > this, force after g_forcerespwan
	int			inactivityTime;		// kick players when time > this
	qboolean	inactivityWarning;	// qtrue if the five seoond warning has been given
	int			rewardTime;			// clear the EF_AWARD_IMPRESSIVE, etc when time > this

	int			airOutTime;

	int			lastKillTime;		// for multiple kill rewards

	qboolean	fireHeld;			// used for hook
	gentity_t	*hook;				// grapple hook if out

	int			switchTeamTime;		// time the player switched teams

	// timeResidual is used to handle events that happen every second
	// like health / armor countdowns and regeneration
	int			timeResidual;

#ifdef MISSIONPACK
	gentity_t	*persistantPowerup;
	int			portalID;
	int			ammoTimes[WP_NUM_WEAPONS];
	int			invulnerabilityTime;
#endif

	char		*areabits;
};


//
// this structure is cleared as each map is entered
//
#define	MAX_SPAWN_VARS			64
#define	MAX_SPAWN_VARS_CHARS	4096

typedef struct {
	struct gclient_s	*clients;		// [maxclients]

	struct gentity_s	*gentities;
	int			gentitySize;
	int			num_entities;		// MAX_CLIENTS <= num_entities <= ENTITYNUM_MAX_NORMAL

	int			warmupTime;			// restart match at this time

	fileHandle_t	logFile;

	// store latched cvars here that we want to get at often
	int			maxclients;

	int			framenum;
	int			time;					// in msec
	int			previousTime;			// so movers can back up when blocked

	int			startTime;				// level.time the map was started

	int			teamScores[TEAM_NUM_TEAMS];
	int			lastTeamLocationTime;		// last time of client team location update

	qboolean	newSession;				// don't use any old session data, because
										// we changed gametype

	qboolean	restarted;				// waiting for a map_restart to fire

	int			numConnectedClients;
	int			numNonSpectatorClients;	// includes connecting clients
	int			numPlayingClients;		// connected, non-spectators
	int			sortedClients[MAX_CLIENTS];		// sorted by score
	int			follow1, follow2;		// clientNums for auto-follow spectators

	int			snd_fry;				// sound index for standing in lava

	int			warmupModificationCount;	// for detecting if g_warmup is changed

	// voting state
	char		voteString[MAX_STRING_CHARS];
	char		voteDisplayString[MAX_STRING_CHARS];
	int			voteTime;				// level.time vote was called
	int			voteExecuteTime;		// time the vote is executed
	int			voteYes;
	int			voteNo;
	int			numVotingClients;		// set by CalculateRanks

	// team voting state
	char		teamVoteString[2][MAX_STRING_CHARS];
	int			teamVoteTime[2];		// level.time vote was called
	int			teamVoteYes[2];
	int			teamVoteNo[2];
	int			numteamVotingClients[2];// set by CalculateRanks

	// spawn variables
	qboolean	spawning;				// the G_Spawn*() functions are valid
	int			numSpawnVars;
	char		*spawnVars[MAX_SPAWN_VARS][2];	// key / value pairs
	int			numSpawnVarChars;
	char		spawnVarChars[MAX_SPAWN_VARS_CHARS];

	// intermission state
	int			intermissionQueued;		// intermission was qualified, but
										// wait INTERMISSION_DELAY_TIME before
										// actually going there so the last
										// frag can be watched.  Disable future
										// kills during this delay
	int			intermissiontime;		// time the intermission was started
	char		*changemap;
	qboolean	readyToExit;			// at least one client wants to exit
	int			exitTime;
	vec3_t		intermission_origin;	// also used for spectator spawns
	vec3_t		intermission_angle;

	qboolean	locationLinked;			// target_locations get linked
	gentity_t	*locationHead;			// head of the location list
	int			bodyQueIndex;			// dead bodies
	gentity_t	*bodyQue[BODY_QUEUE_SIZE];
#ifdef MISSIONPACK
	int			portalSequence;
#endif
} level_locals_t;


//
// g_spawn.c
//
CCALL qboolean	G_SpawnString( const char *key, const char *defaultString, char **out );
// spawn string returns a temporary reference, you must CopyString() if you want to keep it
CCALL qboolean	G_SpawnFloat( const char *key, const char *defaultString, float *out );
CCALL qboolean	G_SpawnInt( const char *key, const char *defaultString, int *out );
CCALL qboolean	G_SpawnVector( const char *key, const char *defaultString, float *out );
CCALL void		G_SpawnEntitiesFromString( void );
CCALL char *G_NewString( const char *string );

//
// g_cmds.c
//
CCALL void Cmd_Score_f (gentity_t *ent);
CCALL void StopFollowing( gentity_t *ent );
CCALL void BroadcastTeamChange( gclient_t *client, int oldTeam );
CCALL void SetTeam( gentity_t *ent, char *s );
CCALL void Cmd_FollowCycle_f( gentity_t *ent, int dir );

//
// g_items.c
//
CCALL void G_CheckTeamItems( void );
CCALL void G_RunItem( gentity_t *ent );
CCALL void RespawnItem( gentity_t *ent );
CCALL gentity_t *Drop_Item( gentity_t *ent, gitem_t *item, float angle );
CCALL gentity_t *LaunchItem( gitem_t *item, vec3_t origin, vec3_t velocity );
CCALL void G_SpawnItem (gentity_t *ent, gitem_t *item);
CCALL void FinishSpawningItem( gentity_t *ent );
CCALL void	Add_Ammo (gentity_t *ent, int weapon, int count);
CCALL void Touch_Item (gentity_t *ent, gentity_t *other, trace_t *trace);
CCALL void ClearRegisteredItems( void );
CCALL void RegisterItem( gitem_t *item );
CCALL void SaveRegisteredItems( void );

//
// g_utils.c
//
CCALL int G_ModelIndex( char *name );
CCALL int		G_SoundIndex( char *name );
CCALL void	G_TeamCommand( team_t team, char *cmd );
CCALL void	G_KillBox (gentity_t *ent);
CCALL gentity_t *G_Find (gentity_t *from, int fieldofs, const char *match);
CCALL gentity_t *G_PickTarget (char *targetname);
CCALL void	G_UseTargets (gentity_t *ent, gentity_t *activator);
CCALL void	G_SetMovedir ( vec3_t angles, vec3_t movedir);
CCALL void	G_InitGentity( gentity_t *e );
CCALL gentity_t	*G_Spawn (void);
CCALL gentity_t *G_TempEntity( vec3_t origin, int event );
CCALL void	G_Sound( gentity_t *ent, int channel, int soundIndex );
CCALL void	G_FreeEntity( gentity_t *e );
CCALL qboolean	G_EntitiesFree( void );
CCALL void	G_TouchTriggers (gentity_t *ent);
CCALL float	*tv (float x, float y, float z);
CCALL char	*vtos( const vec3_t v );
CCALL float vectoyaw( const vec3_t vec );
CCALL void G_AddPredictableEvent( gentity_t *ent, int event, int eventParm );
CCALL void G_AddEvent( gentity_t *ent, int event, int eventParm );
CCALL void G_SetOrigin( gentity_t *ent, vec3_t origin );
CCALL void AddRemap(const char *oldShader, const char *newShader, float timeOffset);
CCALL const char *BuildShaderStateConfig( void );

//
// g_combat.c
//
CCALL qboolean CanDamage (gentity_t *targ, vec3_t origin);
CCALL void G_Damage (gentity_t *targ, gentity_t *inflictor, gentity_t *attacker, vec3_t dir, vec3_t point, int damage, int dflags, int mod);
CCALL qboolean G_RadiusDamage (vec3_t origin, gentity_t *attacker, float damage, float radius, gentity_t *ignore, int mod);
CCALL int G_InvulnerabilityEffect( gentity_t *targ, vec3_t dir, vec3_t point, vec3_t impactpoint, vec3_t bouncedir );
CCALL void body_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath );
CCALL void TossClientItems( gentity_t *self );
#ifdef MISSIONPACK
CCALL void TossClientPersistantPowerups( gentity_t *self );
#endif
CCALL void TossClientCubes( gentity_t *self );

// damage flags
#define DAMAGE_RADIUS				0x00000001	// damage was indirect
#define DAMAGE_NO_ARMOR				0x00000002	// armour does not protect from this damage
#define DAMAGE_NO_KNOCKBACK			0x00000004	// do not affect velocity, just view angles
#define DAMAGE_NO_PROTECTION		0x00000008  // armor, shields, invulnerability, and godmode have no effect
#ifdef MISSIONPACK
#define DAMAGE_NO_TEAM_PROTECTION	0x00000010  // armor, shields, invulnerability, and godmode have no effect
#endif

//
// g_missile.c
//
CCALL void G_RunMissile( gentity_t *ent );
CCALL gentity_t *fire_plasma (gentity_t *self, vec3_t start, vec3_t aimdir);
CCALL gentity_t *fire_grenade (gentity_t *self, vec3_t start, vec3_t aimdir);
CCALL gentity_t *fire_rocket (gentity_t *self, vec3_t start, vec3_t dir);
CCALL gentity_t *fire_bfg (gentity_t *self, vec3_t start, vec3_t dir);
CCALL gentity_t *fire_grapple (gentity_t *self, vec3_t start, vec3_t dir);
#ifdef MISSIONPACK
gentity_t *fire_nail( gentity_t *self, vec3_t start, vec3_t forward, vec3_t right, vec3_t up );
gentity_t *fire_prox( gentity_t *self, vec3_t start, vec3_t aimdir );
#endif


//
// g_mover.c
//
CCALL void G_RunMover( gentity_t *ent );
CCALL void Touch_DoorTrigger( gentity_t *ent, gentity_t *other, trace_t *trace );

//
// g_trigger.c
//
CCALL void trigger_teleporter_touch (gentity_t *self, gentity_t *other, trace_t *trace );


//
// g_misc.c
//
CCALL void TeleportPlayer( gentity_t *player, vec3_t origin, vec3_t angles );
#ifdef MISSIONPACK
CCALL void DropPortalSource( gentity_t *ent );
CCALL void DropPortalDestination( gentity_t *ent );
#endif


//
// g_weapon.c
//
CCALL qboolean LogAccuracyHit( gentity_t *target, gentity_t *attacker );
CCALL void CalcMuzzlePoint ( gentity_t *ent, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint );
CCALL void SnapVectorTowards( vec3_t v, vec3_t to );
CCALL qboolean CheckGauntletAttack( gentity_t *ent );
CCALL void Weapon_HookFree (gentity_t *ent);
CCALL void Weapon_HookThink (gentity_t *ent);


//
// g_client.c
//
CCALL int TeamCount( int ignoreClientNum, team_t team );
CCALL int TeamLeader( int team );
CCALL team_t PickTeam( int ignoreClientNum );
CCALL void SetClientViewAngle( gentity_t *ent, vec3_t angle );
CCALL gentity_t *SelectSpawnPoint (vec3_t avoidPoint, vec3_t origin, vec3_t angles, qboolean isbot);
CCALL void CopyToBodyQue( gentity_t *ent );
CCALL void ClientRespawn(gentity_t *ent);
CCALL void BeginIntermission (void);
CCALL void InitBodyQue (void);
CCALL void ClientSpawn( gentity_t *ent );
CCALL void player_die (gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod);
CCALL void AddScore( gentity_t *ent, vec3_t origin, int score );
CCALL void CalculateRanks( void );
CCALL qboolean SpotWouldTelefrag( gentity_t *spot );

//
// g_svcmds.c
//
CCALL qboolean	ConsoleCommand( void );
CCALL void G_ProcessIPBans(void);
CCALL qboolean G_FilterPacket (char *from);

//
// g_weapon.c
//
CCALL void FireWeapon( gentity_t *ent );
#ifdef MISSIONPACK
void G_StartKamikaze( gentity_t *ent );
#endif

//
// g_cmds.c
//
CCALL void DeathmatchScoreboardMessage( gentity_t *ent );

//
// g_main.c
//
CCALL void MoveClientToIntermission( gentity_t *ent );
CCALL void FindIntermissionPoint( void );
CCALL void SetLeader(int team, int client);
CCALL void CheckTeamLeader( int team );
CCALL void G_RunThink (gentity_t *ent);
CCALL void AddTournamentQueue(gclient_t *client);
CCALL void G_LogPrintf( const char *fmt, ... ) __attribute__ ((format (printf, 1, 2)));
CCALL void SendScoreboardMessageToAllClients( void );
CCALL void G_Printf( const char *fmt, ... ) __attribute__ ((format (printf, 1, 2)));
CCALL void G_Error( const char *fmt, ... ) __attribute__ ((noreturn, format (printf, 1, 2)));

//
// g_client.c
//
CCALL char *ClientConnect( int clientNum, qboolean firstTime, qboolean isBot );
CCALL void ClientUserinfoChanged( int clientNum );
CCALL void ClientDisconnect( int clientNum );
CCALL void ClientBegin( int clientNum );
CCALL void ClientCommand( int clientNum );

//
// g_active.c
//
CCALL void ClientThink( int clientNum );
CCALL void ClientEndFrame( gentity_t *ent );
CCALL void G_RunClient( gentity_t *ent );

//
// g_team.c
//
CCALL qboolean OnSameTeam( gentity_t *ent1, gentity_t *ent2 );
CCALL qboolean CheckObeliskAttack( gentity_t *obelisk, gentity_t *attacker );

//
// g_mem.c
//
CCALL void *G_Alloc( int size );
CCALL void G_InitMemory( void );
CCALL void Svcmd_GameMem_f( void );

//
// g_arenas.c
//
CCALL void UpdateTournamentInfo( void );
CCALL void SpawnModelsOnVictoryPads( void );
CCALL void Svcmd_AbortPodium_f( void );

//
// g_bot.c
//
CCALL void G_InitBots( qboolean restart );
CCALL char *G_GetBotInfoByNumber( int num );
CCALL char *G_GetBotInfoByName( const char *name );
CCALL void G_CheckBotSpawn( void );
CCALL void G_RemoveQueuedBotBegin( int clientNum );
CCALL qboolean G_BotConnect( int clientNum, qboolean restart );
CCALL void Svcmd_AddBot_f( void );
CCALL void Svcmd_BotList_f( void );

// ai_main.c
#define MAX_FILEPATH			144

//bot settings
typedef struct bot_settings_s
{
	char characterfile[MAX_FILEPATH];
	float skill;
	char team[MAX_FILEPATH];
} bot_settings_t;

#include "g_team.h" // teamplay specific stuff


EXTERNC	level_locals_t	level;
EXTERNC gentity_t		g_entities[MAX_GENTITIES];

#define	FOFS(x) ((size_t)&(((gentity_t *)0)->x))

EXTERNC vmCvar_t	g_gametype;
EXTERNC vmCvar_t	g_dedicated;
EXTERNC vmCvar_t	g_cheats;
EXTERNC vmCvar_t	g_maxclients;			// allow this many total, including spectators
EXTERNC vmCvar_t	g_maxGameClients;		// allow this many active
EXTERNC vmCvar_t	g_restarted;

EXTERNC vmCvar_t	g_dmflags;
EXTERNC vmCvar_t	g_fraglimit;
EXTERNC vmCvar_t	g_timelimit;
EXTERNC vmCvar_t	g_capturelimit;
EXTERNC vmCvar_t	g_friendlyFire;
EXTERNC vmCvar_t	g_password;
EXTERNC vmCvar_t	g_needpass;
EXTERNC vmCvar_t	g_gravity;
EXTERNC vmCvar_t	g_minwalknormal;
EXTERNC vmCvar_t	g_aircontrol;
EXTERNC vmCvar_t	g_speed;
EXTERNC vmCvar_t	g_jumpheight;
EXTERNC vmCvar_t	g_knockback;
EXTERNC vmCvar_t	g_quadfactor;
EXTERNC vmCvar_t	g_forcerespawn;
EXTERNC vmCvar_t	g_inactivity;
EXTERNC vmCvar_t	g_debugMove;
EXTERNC vmCvar_t	g_debugAlloc;
EXTERNC vmCvar_t	g_debugDamage;
EXTERNC vmCvar_t	g_weaponRespawn;
EXTERNC vmCvar_t	g_weaponTeamRespawn;
EXTERNC vmCvar_t	g_synchronousClients;
EXTERNC vmCvar_t	g_motd;
EXTERNC vmCvar_t	g_warmup;
EXTERNC vmCvar_t	g_doWarmup;
EXTERNC vmCvar_t	g_blood;
EXTERNC vmCvar_t	g_allowVote;
EXTERNC vmCvar_t	g_teamAutoJoin;
EXTERNC vmCvar_t	g_teamForceBalance;
EXTERNC vmCvar_t	g_banIPs;
EXTERNC vmCvar_t	g_filterBan;
EXTERNC vmCvar_t	g_obeliskHealth;
EXTERNC vmCvar_t	g_obeliskRegenPeriod;
EXTERNC vmCvar_t	g_obeliskRegenAmount;
EXTERNC vmCvar_t	g_obeliskRespawnDelay;
EXTERNC vmCvar_t	g_cubeTimeout;
EXTERNC vmCvar_t	g_redteam;
EXTERNC vmCvar_t	g_blueteam;
EXTERNC vmCvar_t	g_smoothClients;
EXTERNC vmCvar_t	pmove_fixed;
EXTERNC vmCvar_t	pmove_msec;
EXTERNC vmCvar_t	g_rankings;
EXTERNC vmCvar_t	g_enableDust;
EXTERNC vmCvar_t	g_enableBreath;
EXTERNC vmCvar_t	g_singlePlayer;
EXTERNC vmCvar_t	g_proxMineTimeout;

// todo fixme that g and cg both use the real functions... no need for the trap bs anymore
#ifndef IGNORE_TRAPS
CCALL void	trap_Print( const char *text );
CCALL void	trap_Error( const char *text ) __attribute__((noreturn));
CCALL int		trap_Milliseconds( void );
CCALL int	trap_RealTime( qtime_t *qtime );
CCALL int		trap_Argc( void );
CCALL void	trap_Argv( int n, char *buffer, int bufferLength );
CCALL void	trap_Args( char *buffer, int bufferLength );
CCALL int		trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode );
CCALL int		trap_Cvar_VariableIntegerValue( const char *var_name );
CCALL float	trap_Cvar_VariableValue( const char *var_name );
CCALL void	trap_LocateGameData( gentity_t *gEnts, int numGEntities, int sizeofGEntity_t, playerState_t *gameClients, int sizeofGameClient );
CCALL void	trap_DropClient( int clientNum, const char *reason );
CCALL void	trap_SendServerCommand( int clientNum, const char *text );
CCALL void	trap_SetConfigstring( int num, const char *string );
CCALL void	trap_GetConfigstring( int num, char *buffer, int bufferSize );
CCALL void	trap_GetUserinfo( int num, char *buffer, int bufferSize );
CCALL void	trap_SetUserinfo( int num, const char *buffer );
CCALL void	trap_GetServerinfo( char *buffer, int bufferSize );
CCALL void	trap_SetBrushModel( gentity_t *ent, const char *name );
CCALL void	trap_Trace( trace_t *results, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int passEntityNum, int contentmask );
CCALL int		trap_PointContents( const vec3_t point, int passEntityNum );
CCALL qboolean trap_InPVS( const vec3_t p1, const vec3_t p2 );
CCALL qboolean trap_InPVSIgnorePortals( const vec3_t p1, const vec3_t p2 );
CCALL void	trap_AdjustAreaPortalState( gentity_t *ent, qboolean open );
CCALL qboolean trap_AreasConnected( int area1, int area2 );
CCALL void	trap_LinkEntity( gentity_t *ent );
CCALL void	trap_UnlinkEntity( gentity_t *ent );
CCALL int		trap_EntitiesInBox( const vec3_t mins, const vec3_t maxs, int *entityList, int maxcount );
CCALL qboolean trap_EntityContact( const vec3_t mins, const vec3_t maxs, const gentity_t *ent );
CCALL int		trap_BotAllocateClient( void );
CCALL void	trap_BotFreeClient( int clientNum );
CCALL void	trap_GetUsercmd( int clientNum, usercmd_t *cmd );
CCALL qboolean	trap_Game_GetEntityToken( char *buffer, int bufferSize );
CCALL int		trap_DebugPolygonCreate(int color, int numPoints, vec3_t *points);
CCALL void	trap_DebugPolygonDelete(int id);
CCALL void	trap_SnapVector( float *v );

#endif

#endif