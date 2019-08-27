// Copyright (C) 1999-2000 Id Software, Inc.
//
// g_local.h -- local definitions for game module

#include "q_shared.h"
#include "bg_public.h"
#include "g_public.h"

#define	GAMEVERSION	        "dfx"       // the "gameversion" client command will print this plus compile date
#define BODY_QUEUE_SIZE		8
#define INFINITE			1000000
#define	FRAMETIME			100				// msec
#define REWARD_SPRITE_TIME	2000
#define	INTERMISSION_DELAY	1000

// gentity->flags
#define	FL_GODMODE				0x00000010
#define	FL_NOTARGET				0x00000020
#define	FL_TEAMSLAVE			0x00000400	// not the first on the team
#define FL_NO_KNOCKBACK			0x00000800
#define FL_DROPPED_ITEM			0x00001000
#define FL_NO_BOTS				0x00002000	// spawn point not for bot use
#define FL_NO_HUMANS			0x00004000	// spawn point just for bots
#define FL_FORCE_GESTURE		0x00008000	// force gesture on client


#define ADJUST_AREAPORTAL() \
	if(ent->s.eType == ET_MOVER) \
	{ \
		trap_LinkEntity(ent); \
		trap_AdjustAreaPortalState(ent, qtrue); \
	}

// movers are things like doors, plats, buttons, etc
typedef enum {
	MOVER_POS1,
	MOVER_POS2,
	MOVER_1TO2,
	MOVER_2TO1,
	ROTATOR_POS1,
	ROTATOR_POS2,
	ROTATOR_1TO2,
	ROTATOR_2TO1
} moverState_t;

//============================================================================

typedef struct gentity_s gentity_t;
typedef struct gclient_s gclient_t;

struct gentity_s {
	entityState_t	s;				// communicated by server to clients
	entityShared_t	r;				// shared by both the server system and game

	// DO NOT MODIFY ANYTHING ABOVE THIS, THE SERVER
	// EXPECTS THE FIELDS IN THAT ORDER!
	//================================

	struct gclient_s	*client;	// NULL if not a client

	qbool	     inuse;
	const char	*classname;			// set in QuakeEd
	int			spawnflags;			// set in QuakeEd
	qbool	    neverFree;			// if true, FreeEntity will only unlink
									// bodyque uses this
	int			flags;				// FL_* variables
	const char	*model;
	const char	*model2;
	int			freetime;			// level.time when the object was freed
	int			eventTime;			// events will be cleared EV_VALID_MSEC after set
	qbool	    freeAfterEvent;
	qbool	    unlinkAfterEvent;
	qbool	    physicsObject;		// if true, it can be pushed by movers and fall off edges
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
	int			oldSoundLoop;
	gentity_t	*parent;
	gentity_t	*nextTrain;
	gentity_t	*prevTrain;
	vec3_t		pos1, pos2;
	char		*message;
	int			timestamp;		// body queue sinking, etc
	float		angle;			// set in editor, -1 = up, -2 = down
    float		distance;       // degrees to rotate rotator mover
	const char	*target;
	const char	*targetname;
	const char	*team;
	const char	*targetShaderName;
	const char	*targetShaderNewName;
	gentity_t	*target_ent;

	float		speed;
	vec3_t		movedir;

	qbool       needKey;        // for locked doors
	qbool       keyUsed;        // for locked doors

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

	qbool	    takedamage;
	int			damage;
	int			splashDamage;	// quad will increase this without increasing radius
	int			splashRadius;
	int			methodOfDeath;
	int			splashMethodOfDeath;
	int			count;

	char		music[MAX_QPATH]; //path to music file for target_music

	gentity_t	*chain;
	gentity_t	*enemy;
	gentity_t	*activator;
	gentity_t	*teamchain;		// next entity in team
	gentity_t	*teammaster;	// master of the team

	int			watertype;
	int			waterlevel;
	int			noise_index;

	// timing variables
	float		wait;
	float		random;
	gitem_t		*item;			// for bonus items
    float		waittable[MAX_CLIENTS];

    int			firedTime;		// despawn missiles in defrag mode if owner's respawn is more recent than this

	// team for spawn spot
	team_t		fteam;
	tag_t		tag;
	
	// for teamrun
	qbool		frozen;
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

// the auto following clients don't follow a specific client
// number, but instead follow the first two active players
#define	FOLLOW_ACTIVE1	-1
#define	FOLLOW_ACTIVE2	-2

// client data that stays across multiple levels or tournament restarts
// this is achieved by writing all the data to cvar strings at game shutdown
// time and reading them back at connection time.  Anything added here
// MUST be dealt with in G_InitSessionData() / G_ReadSessionData() / G_WriteSessionData()
typedef struct {
	team_t		sessionTeam;
	int			spectatorTime;		// for determining next-in-line to play
	spectatorState_t	spectatorState;
	int			spectatorClient;	// for chasecam and follow mode
	int			wins, losses;		// tournament stats
	qbool	    teamLeader;			// true when this client is a team leader
	int         secrets;            // holding levelsecrets
} clientSession_t;

#define MAX_NETNAME			36
#define	MAX_VOTE_COUNT		3

// client data that stays across multiple respawns, but is cleared
// on each level change or team change at ClientBegin()
typedef struct {
	clientConnected_t	connected;
	usercmd_t	cmd;				// we would lose angles if not persistant
	qbool	    localClient;		// true if "ip" info key is "localhost"
	qbool	    initialSpawn;		// the first spawn should be at a cool location
	qbool	    predictItemPickup;	// based on cg_predictItems userinfo
	char		netname[MAX_NETNAME];
	int			maxHealth;			// for handicapping
	int			enterTime;			// level.time the client entered the game

	playerTeamState_t teamState;	// status in teamplay games

	int			voteCount;			// to prevent people from constantly calling votes
	int			voted;
    int			dfBestTime;			// best defrag time

	qbool	    inGame;

    qbool	    useRespawnPoint;	// client wants to respawn in the same place every time (for defrag mode)
	vec3_t		respawnOrigin;		// where they want to respawn
	vec3_t		respawnAngles;
} clientPersistant_t;

// unlagged
#define NUM_CLIENT_HISTORY 18

typedef struct {
	vec3_t		mins, maxs;
	vec3_t		currentOrigin;
	int			leveltime;
} clientHistory_t;

// this structure is cleared on each ClientSpawn(),
// except for 'client->pers' and 'client->sess'
struct gclient_s {
	// ps MUST be the first element, because the server expects it
	playerState_t	ps;				// communicated by server to clients

	// the rest of the structure is private to game
	clientPersistant_t	pers;
	clientSession_t		sess;

	qbool	    readyToExit;		// wishes to leave the intermission
	qbool	    noclip;
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
	qbool	    damage_fromWorld;	// if true, don't use the damage_from vector

	int			accurateCount;		// for "impressive" reward sound
	int			accuracy_shots;		// total number of shots
	int			accuracy_hits;		// total number of hits
	int			lastkilled_client;	// last client that this client killed
	int			lasthurt_client;	// last client that damaged this client
	int			lasthurt_mod;		// type of damage the client did
	// timers
	int			respawnTime;		// can respawn when time > this, force after g_forcerespwan
	int			inactivityTime;		// kick players when time > this
	qbool	    inactivityWarning;	// qtrue if the five seoond warning has been given
	int			rewardTime;			// clear the EF_AWARD_IMPRESSIVE, etc when time > this
	int			airOutTime;
	int			lastKillTime;		// for multiple kill rewards

	qbool	    fireHeld;			// used for hook
	gentity_t	*hook;				// grapple hook if out
	int			switchTeamTime;		// time the player switched teams

	// timeResidual is used to handle events that happen every second
	// like health / armor countdowns and regeneration
	int			timeResidual;

	gentity_t	*persistantPowerup;

	char		*areabits;

    qbool	    dfTimerOn;			// timer for defrag mode
	int			dfTimer;

	int			hookTime;			// reload time for offhand hook

	// unlagged
	clientHistory_t	history[ NUM_CLIENT_HISTORY ];
	clientHistory_t	saved;

	int			historyHead;
	int			frameOffset;
	int			lastUpdateFrame;

	// hitsounds
	struct {
		int		team;
		int		enemy;
		int		amount;
	} damage;
};


//
// this structure is cleared as each map is entered
//
#define	MAX_SPAWN_VARS			64
#define	MAX_SPAWN_VARS_CHARS	4096

#define NUM_SPAWN_SPOTS MAX_GENTITIES
#define SPAWN_SPOT_INTERMISSION NUM_SPAWN_SPOTS-1
#define SPAWN_SPOT_TEAM_INTERMISSION NUM_SPAWN_SPOTS-2

typedef struct {
	struct gclient_s	*clients;	// [maxclients]

	struct gentity_s	*gentities;
	int			gentitySize;
	int			num_entities;		// current number, <= MAX_GENTITIES

	int			warmupTime;			// restart match at this time

	fileHandle_t	logFile;

	// store latched cvars here that we want to get at often
	int			maxclients;
	int			framenum;
	int			time;					// in msec
	int			previousTime;			// so movers can back up when blocked
	int			startTime;				// level.time the map was started
	int			msec;					// current frame duration
	int			teamScores[TEAM_NUM_TEAMS];
	int			lastTeamLocationTime;	// last time of client team location update

	qbool	    newSession;				// don't use any old session data, because
										// we changed gametype
	qbool	    restarted;				// waiting for a map_restart to fire

	int			numConnectedClients;
	int			numNonSpectatorClients;	// includes connecting clients
	int			numPlayingClients;		// connected, non-spectators
	int			sortedClients[MAX_CLIENTS];		// sorted by score
	int			follow1, follow2;		// clientNums for auto-follow spectators
	int			snd_fry;				// sound index for standing in lava
	int			warmupModificationCount;// for detecting if g_warmup is changed

	// voting state
	char		voteString[MAX_CVAR_VALUE_STRING];
	char		voteDisplayString[MAX_CVAR_VALUE_STRING];
	int			voteTime;				// level.time vote was called
	int			voteExecuteTime;		// time the vote is executed
	int			voteYes;
	int			voteNo;
	int			numVotingClients;		// set by CalculateRanks
	int			numteamVotingClients [TEAM_NUM_TEAMS];

	// spawn variables
	qbool	    spawning;				// the G_Spawn*() functions are valid
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
	qbool	    readyToExit;			// at least one client wants to exit
	int			exitTime;
	vec3_t		intermission_origin;	// also used for spectator spawns
	vec3_t		intermission_angle;
	qbool	    intermission_spot;
	qbool	    locationLinked;			// target_locations get linked
	gentity_t	*locationHead;			// head of the location list
	int			bodyQueIndex;			// dead bodies
	gentity_t	*bodyQue[BODY_QUEUE_SIZE];

	// spawn spots
	gentity_t	*spawnSpots[NUM_SPAWN_SPOTS];
	int			numSpawnSpots;
	int			numSpawnSpotsFFA;
	// if a info_player_start has spawnflag & 1 then theres a reverse map playerstart
	qbool		revStart;
	// team run spawn spots
	gentity_t	*TeamSpawnSpots[NUM_SPAWN_SPOTS];
	int			numTeamSpawnSpots;
	qbool		redSpawnSpot;
	qbool		blueSpawnSpot;
	qbool		greenSpawnSpot;
	qbool		yellowSpawnSpot;

	// unlagged
	int			frameStartTime;

	// target_Secret
	int			secretCount;			// number of target_secret entities in map
} level_locals_t;


//
// g_spawn.c
//
// spawn string returns a temporary reference, you must CopyString() if you want to keep it
qbool	G_SpawnString( const char *key, const char *defaultString, char **out );
qbool	G_SpawnFloat( const char *key, const char *defaultString, float *out );
qbool	G_SpawnInt( const char *key, const char *defaultString, int *out );
qbool	G_SpawnVector( const char *key, const char *defaultString, float *out );
void	G_SpawnEntitiesFromString( void );

//
// g_cmds.c
//
void Cmd_Score_f (gentity_t *ent);
void StopFollowing( gentity_t *ent, qbool release );
void BroadcastTeamChange( gclient_t *client, team_t oldTeam );
qbool SetTeam( gentity_t *ent, const char *s );
void Cmd_FollowCycle_f( gentity_t *ent, int dir );
void G_RevertVote( gclient_t *client );

//
// g_items.c
//
void G_RunItem( gentity_t *ent );
void RespawnItem( gentity_t *ent );
int SpawnTime( gentity_t *ent, qbool firstSpawn );

void UseHoldableItem( gentity_t *ent );
void PrecacheItem (gitem_t *it);
gentity_t *Drop_Item( gentity_t *ent, gitem_t *item, float angle );
gentity_t *LaunchItem( gitem_t *item, vec3_t origin, vec3_t velocity );
void SetRespawn (gentity_t *ent, float delay);
void G_SpawnItem (gentity_t *ent, gitem_t *item);
void FinishSpawningItem( gentity_t *ent );
void Think_Weapon (gentity_t *ent);
int ArmorIndex (gentity_t *ent);
void Touch_Item (gentity_t *ent, gentity_t *other, trace_t *trace);

void ClearRegisteredItems( void );
void RegisterItem( gitem_t *item );
void SaveRegisteredItems( void );

//
// g_utils.c
//
int		    G_ModelIndex( const char *name );
int		    G_SoundIndex( const char *name );
void	    G_KillBox (gentity_t *ent);
gentity_t   *G_Find (gentity_t *from, int fieldofs, const char *match);
gentity_t   *G_PickTarget (const char *targetname);
void	    G_UseTargets (gentity_t *ent, gentity_t *activator);
void	    G_SetMovedir ( vec3_t angles, vec3_t movedir);
void	    G_InitGentity( gentity_t *e );
gentity_t   *G_Spawn (void);
gentity_t   *G_TempEntity( vec3_t origin, int event );
void	    G_Sound( gentity_t *ent, int channel, int soundIndex );
void	    G_FreeEntity( gentity_t *e );
qbool	    G_EntitiesFree( void );
void	    G_TouchTriggers (gentity_t *ent);
void	    G_TouchSolids (gentity_t *ent);
float	    *tv (float x, float y, float z);
char	    *vtos( const vec3_t v );
float       vectoyaw( const vec3_t vec );
void        G_AddPredictableEvent( gentity_t *ent, entity_event_t event, int eventParm );
void        G_AddEvent( gentity_t *ent, int event, int eventParm );
void        G_SetOrigin( gentity_t *ent, vec3_t origin );
void        AddRemap(const char *oldShader, const char *newShader, float timeOffset);
const char *BuildShaderStateConfig( void );

//
// g_combat.c
//
qbool   CanDamage (gentity_t *targ, vec3_t origin);
void    G_Damage (gentity_t *targ, gentity_t *inflictor, gentity_t *attacker, vec3_t dir, vec3_t point, int damage, int dflags, int mod);
qbool   G_RadiusDamage (vec3_t origin, gentity_t *attacker, float damage, float radius, gentity_t *ignore, int mod);
int     G_InvulnerabilityEffect( gentity_t *targ, vec3_t dir, vec3_t point, vec3_t impactpoint, vec3_t bouncedir );
void    body_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath );

// damage flags
#define DAMAGE_RADIUS				0x00000001	// damage was indirect
#define DAMAGE_NO_ARMOR				0x00000002	// armor does not protect from this damage
#define DAMAGE_NO_KNOCKBACK			0x00000004	// do not affect velocity, just view angles
#define DAMAGE_NO_PROTECTION		0x00000008  // armor, shields, invulnerability, and godmode have no effect

//
// g_missile.c
//
void      G_RunMissile( gentity_t *ent );
gentity_t *fire_blaster (gentity_t *self, vec3_t start, vec3_t aimdir);
gentity_t *fire_plasma (gentity_t *self, vec3_t start, vec3_t aimdir);
gentity_t *fire_grenade (gentity_t *self, vec3_t start, vec3_t aimdir);
gentity_t *fire_rocket (gentity_t *self, vec3_t start, vec3_t dir);
gentity_t *fire_bfg (gentity_t *self, vec3_t start, vec3_t dir);
gentity_t *fire_grapple (gentity_t *self, vec3_t start, vec3_t dir);

//
// g_mover.c
//
void    G_RunMover( gentity_t *ent );
void    Touch_DoorTrigger( gentity_t *ent, gentity_t *other, trace_t *trace );

//
// g_trigger.c
//
void    trigger_teleporter_touch (gentity_t *self, gentity_t *other, trace_t *trace );


//
// g_misc.c
//
void    TeleportPlayer( gentity_t *player, vec3_t origin, vec3_t angles, qbool keepSpeed );
void    DF_Timer_Start( gentity_t *ent );
void    DF_Timer_Stop( gentity_t *ent );

//
// g_weapon.c
//
void    CalcMuzzlePoint ( gentity_t *ent, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint );
void    SnapVectorTowards( vec3_t v, vec3_t to );
qbool   CheckGauntletAttack( gentity_t *ent );
void    Weapon_HookFree (gentity_t *ent);
void    Weapon_HookThink (gentity_t *ent);

//
// g_client.c
//
int       TeamCount( int ignoreClientNum, team_t team );
int       TeamConnectedCount( int ignoreClientNum, team_t team );
void      SetClientViewAngle( gentity_t *ent, vec3_t angle );
gentity_t *SelectSpawnPoint( gentity_t *ent, vec3_t avoidPoint, vec3_t origin, vec3_t angles );
team_t 	  PickTeam( int ignoreClientNum );
void      CopyToBodyQue( gentity_t *ent );
void      respawn (gentity_t *ent);
void      InitBodyQue (void);
void      ClientSpawn( gentity_t *ent );
void      player_die (gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod);
void      AddScore( gentity_t *ent, vec3_t origin, int score );
void      AddKey( gentity_t *ent );
void      CalculateRanks( void );
qbool     SpotWouldTelefrag( gentity_t *spot );

//
// g_svcmds.c
//
qbool	ConsoleCommand( void );
void    G_ProcessIPBans(void);
qbool   G_FilterPacket (char *from);

//
// g_weapon.c
//
void FireWeapon( gentity_t *ent );

//
// g_cmds.c
//
void DeathmatchScoreboardMessage( gentity_t *ent );

//
// g_main.c
//
void MoveClientToIntermission( gentity_t *ent );
void FindIntermissionPoint( void );
void G_RunThink (gentity_t *ent);
void QDECL G_LogPrintf( const char *fmt, ... );
void QDECL G_Printf( const char *fmt, ... );
void QDECL G_Error( const char *fmt, ... );
void G_BroadcastServerCommand( int ignoreClient, const char *command );

//
// g_client.c
//
const char *ClientConnect( int clientNum, qbool firstTime, qbool isBot );
qbool       ClientUserinfoChanged( int clientNum );
void        ClientDisconnect( int clientNum );
void        ClientBegin( int clientNum );
void        ClientCommand( int clientNum );

//
// g_active.c
//
void ClientThink( int clientNum );
void ClientEndFrame( gentity_t *ent );
void G_RunClient( gentity_t *ent );

//
// g_team.c
//
qbool OnSameTeam( gentity_t *ent1, gentity_t *ent2 );
void  Team_CheckDroppedItem( gentity_t *dropped );
qbool CheckObeliskAttack( gentity_t *obelisk, gentity_t *attacker );
void  Team_ResetFlags( void );

//
// g_mem.c
//
void *G_Alloc( size_t size );
void G_InitMemory( void );
void Svcmd_GameMem_f( void );

//
// g_session.c
//
void G_InitWorldSession( void );
void G_WriteSessionData( void );
void G_InitSessionData( gclient_t *client, const char *team );
void G_ReadClientSessionData( gclient_t *client );
void G_WriteClientSessionData( gclient_t *client );
void G_ClearClientSessionData( gclient_t *client );

//
// g_unlagged.c
//
void G_ResetHistory( gentity_t *ent );
void G_StoreHistory( gentity_t *ent );
void G_TimeShiftAllClients( int time, gentity_t *skip );
void G_UnTimeShiftAllClients( gentity_t *skip );
void G_DoTimeShiftFor( gentity_t *ent );
void G_UndoTimeShiftFor( gentity_t *ent );
void G_UnTimeShiftClient( gentity_t *client );
void G_PredictPlayerMove( gentity_t *ent, float frametime );


#include "g_team.h" // teamplay specific stuff
extern	level_locals_t	level;
extern	gentity_t		g_entities[MAX_GENTITIES];

#define	FOFS(x) ((intptr_t)&(((gentity_t *)0)->x))

extern	vmCvar_t	g_gametype;
extern	vmCvar_t	g_mapname;
extern	vmCvar_t	sv_fps;
extern	vmCvar_t	sv_pure;
extern	vmCvar_t	g_dedicated;
extern	vmCvar_t	g_cheats;
extern	vmCvar_t	g_maxGameClients;
extern	vmCvar_t	g_restarted;
extern	vmCvar_t	g_dmflags;
extern	vmCvar_t	g_fraglimit;
extern	vmCvar_t	g_timelimit;
extern	vmCvar_t	g_password;
extern	vmCvar_t	g_needpass;
extern	vmCvar_t	g_gravity;
extern	vmCvar_t	g_speed;
extern	vmCvar_t	g_knockback;
extern	vmCvar_t	g_forcerespawn;
extern	vmCvar_t	g_inactivity;
extern	vmCvar_t	g_debugMove;
extern	vmCvar_t	g_debugAlloc;
extern	vmCvar_t	g_debugDamage;
extern	vmCvar_t	g_weaponRespawn;
extern	vmCvar_t	g_synchronousClients;
extern	vmCvar_t	g_motd;
extern	vmCvar_t	g_warmup;
extern	vmCvar_t	g_blood;
extern	vmCvar_t	g_allowVote;
extern	vmCvar_t	g_banIPs;
extern	vmCvar_t	g_filterBan;
extern	vmCvar_t	g_smoothClients;
extern	vmCvar_t	pmove_fixed;
extern	vmCvar_t	pmove_msec;
extern	vmCvar_t	g_unlagged;
extern	vmCvar_t	g_predictPVS;
extern	vmCvar_t	g_singlePlayer;
extern	vmCvar_t	g_quadfactor;

extern	vmCvar_t	dfx_gametype;
extern	vmCvar_t	dfx_obs;
extern	vmCvar_t	dfx_interference;
extern	vmCvar_t	dfx_reversemap;
extern	vmCvar_t	dfx_ruleset;
extern	vmCvar_t	dfx_mode;
extern	vmCvar_t	dfx_fastcapmode;


void	trap_Print( const char *text );
void	trap_Error( const char *text );
int		trap_Milliseconds( void );
int		trap_RealTime( qtime_t *qtime );
int		trap_Argc( void );
void	trap_Argv( int n, char *buffer, int bufferLength );
void	trap_Args( char *buffer, int bufferLength );
int		trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode );
void	trap_FS_Read( void *buffer, int len, fileHandle_t f );
void	trap_FS_Write( const void *buffer, int len, fileHandle_t f );
void	trap_FS_FCloseFile( fileHandle_t f );
int		trap_FS_GetFileList( const char *path, const char *extension, char *listbuf, int bufsize );
int		trap_FS_Seek( fileHandle_t f, long offset, fsOrigin_t origin );
void	trap_SendConsoleCommand( int exec_when, const char *text );
void	trap_Cvar_Register( vmCvar_t *cvar, const char *var_name, const char *value, int flags );
void	trap_Cvar_Update( vmCvar_t *cvar );
void	trap_Cvar_Set( const char *var_name, const char *value );
int		trap_Cvar_VariableIntegerValue( const char *var_name );
float	trap_Cvar_VariableValue( const char *var_name );
void	trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize );
void	trap_LocateGameData( gentity_t *gEnts, int numGEntities, int sizeofGEntity_t, playerState_t *gameClients, int sizeofGameClient );
void	trap_DropClient( int clientNum, const char *reason );
void	trap_SendServerCommand( int clientNum, const char *text );
void	trap_SetConfigstring( int num, const char *string );
void	trap_GetConfigstring( int num, char *buffer, int bufferSize );
void	trap_GetUserinfo( int num, char *buffer, int bufferSize );
void	trap_SetUserinfo( int num, const char *buffer );
void	trap_GetServerinfo( char *buffer, int bufferSize );
void	trap_SetBrushModel( gentity_t *ent, const char *name );
void	trap_Trace( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask );
void	trap_TraceCapsule( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask );
int		trap_PointContents( const vec3_t point, int passEntityNum );
qbool   trap_InPVS( const vec3_t p1, const vec3_t p2 );
qbool   trap_InPVSIgnorePortals( const vec3_t p1, const vec3_t p2 );
void	trap_AdjustAreaPortalState( gentity_t *ent, qbool open );
qbool   trap_AreasConnected( int area1, int area2 );
void	trap_LinkEntity( gentity_t *ent );
void	trap_UnlinkEntity( gentity_t *ent );
int		trap_EntitiesInBox( const vec3_t mins, const vec3_t maxs, int *entityList, int maxcount );
qbool   trap_EntityContact( const vec3_t mins, const vec3_t maxs, const gentity_t *ent );
void	trap_GetUsercmd( int clientNum, usercmd_t *cmd );
qbool	trap_GetEntityToken( char *buffer, int bufferSize );
int		trap_DebugPolygonCreate(int color, int numPoints, vec3_t *points);
void	trap_DebugPolygonDelete(int id);
void	trap_SnapVector( float *v );

extern	int svf_self_portal2;   // extension interface
