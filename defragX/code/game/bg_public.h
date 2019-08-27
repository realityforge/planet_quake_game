// Copyright (C) 1999-2000 Id Software, Inc.
//
// bg_public.h -- definitions shared by both the server game and client game modules

// because games can change separately from the main system version, we need a
// second version that must match between game and cgame

#define	GAME_VERSION		    "defrag-5"//"dfx-1"
#define	DEFAULT_GRAVITY		    800
#define	GIB_HEALTH			    -40
#define	ARMOR_PROTECTION	    0.66
#define	HEALTH_SOFT_LIMIT	    100
#define	AMMO_HARD_LIMIT		    200
#define	MAX_ITEMS			    256
#define	RANK_TIED_FLAG		    0x4000
#define DEFAULT_SHOTGUN_SPREAD	700
#define DEFAULT_SHOTGUN_COUNT	11
#define	ITEM_RADIUS			    15		// item sizes are needed for client side pickup detection
#define	LIGHTNING_RANGE		    768
#define	SCORE_NOT_PRESENT	    -9999	// for the CS_SCORES[12] when only one player is present
#define	VOTE_TIME			    10000	// 30 seconds before vote times out
#define	MINS_Z				    -24
#define	DEFAULT_VIEWHEIGHT	    26
#define CROUCH_VIEWHEIGHT	    12
#define	DEAD_VIEWHEIGHT		    -16
#define STEPSIZE                 18

//
// config strings are a general means of communicating variable length strings
// from the server to all connected clients.
//

// CS_SERVERINFO and CS_SYSTEMINFO are defined in q_shared.h
#define	CS_MUSIC				2
#define	CS_MESSAGE				3		// from the map worldspawn's message field
#define	CS_MOTD					4		// g_motd string for server message of the day
#define	CS_WARMUP				5		// server time when the match will be restarted
#define	CS_SCORES1				6
#define	CS_SCORES2				7
#define	CS_SCORES3				8
#define	CS_SCORES4				9
#define CS_VOTE_TIME			10
#define CS_VOTE_STRING			11
#define	CS_VOTE_YES				12
#define	CS_VOTE_NO				13
// below new ones
#define CS_OVERBOUNCES          14
#define CS_INTERFERENCE         15
//--------------------------------
#define	CS_GAME_VERSION			20
#define	CS_LEVEL_START_TIME		21		// so the timer only shows the current level
#define	CS_INTERMISSION			22		// when 1, fraglimit/timelimit has been hit and intermission will start in a second or two
#define CS_FLAGSTATUS			23		// string indicating flag status in CTF
#define CS_SHADERSTATE			24
#define	CS_ITEMS				25		// string of 0's and 1's that tell which items are present

// below used for holding
// ruleset and gametype stuff
//[defined in bg_shared.h]
//
// CS_GAMETYPE                  26
// CS_RULESET                   27
// CS_DFMODE                    28
// CS_FCMODE                    29
// CS_REVERSEMAP                30

#define	CS_MODELS				32
#define	CS_SOUNDS				(CS_MODELS+MAX_MODELS)
#define	CS_PLAYERS				(CS_SOUNDS+MAX_SOUNDS)
#define CS_LOCATIONS			(CS_PLAYERS+MAX_CLIENTS)
#define CS_PARTICLES			(CS_LOCATIONS+MAX_LOCATIONS)
#define CS_MAX					(CS_PARTICLES+MAX_LOCATIONS)

#if (CS_MAX) > MAX_CONFIGSTRINGS
#error overflow: (CS_MAX) > MAX_CONFIGSTRINGS
#endif

typedef enum {
	GT_FASTCAP,         // fastcap mode

	GT_ADVENTURE,       // defrag 'single player' maps, only one client is allowed ingame
	GT_DEFRAG,          // classic 'defrag' gametype
	GT_TEAMRUN,         // like 'defrag' but in co-op style. everyone ingame can trigger the start/stoptimer. As soon as the stoptimer is triggered
                        // the game is over
	GT_FREESTYLE,       // spawn with all weapons and do trickjumps like you want
	GT_MAX_GAME_TYPE
} gametype_t;

typedef enum {
    RS_VANILLA,         // classic vq3 physics
    RS_CPM,             // classic defrag cpm physics
    RS_XVANILLA,        // extended vq3 physics featuring walljumping and crouchsliding
    RS_XCPM,            // extended defrag cpm physics featuring walljumping and crouchsliding
    RS_MAX_RULESET
} ruleset_t;

typedef enum {
    FC_DEFAULT,         // default fastcap strafe
    FC_NOTELES,         // strafe without teleporters
    FC_NOPADS,          // strafe, jump pads are killers
    FC_NODOORS,         // strafe, doors won't spawn
    FC_NOTELES_NOPADS,  // strafe, no teleporters, no jump pads
    FC_DISABLEALL,      // strafe, no teleporters, no jump pads, no doors
                        // *below weapon modes and settings from FC_DISABLEALL
    FC_ROCKET,          // rocket launcher
    FC_PLASMA,          // plasma gun
    FC_COMBO            // rocket launcher, plasma gun, grenade launcher
} fastcapmode_t;

typedef enum {
    DF_DEFAULT,         // default, like mapper made the map

    DF_STRAFE,          // strafe, no item spawnings, no selfdamage
    DF_ROCKET,          // rocket, no item spawnings, no selfdamage
    DF_PLASMA,          // plasma, no item spawnings, no selfdamage
    DF_COMBO,           // rocket, plasma and nade, no item spawnings, no selfdamage

    DF_ICEMODE          // strafe, no item spawnings, no selfdamage, all groundsurfaces are slick
} defragmode_t;

typedef enum { GENDER_MALE, GENDER_FEMALE, GENDER_NEUTER } gender_t;

/*
===================================================================================

PMOVE MODULE

The pmove code takes a player_state_t and a usercmd_t and generates a new player_state_t
and some other output data.  Used for local prediction on the client game and true
movement on the server game.
===================================================================================
*/

typedef enum {
	PM_NORMAL,		  // can accelerate and turn
	PM_NOCLIP,		  // noclip movement
	PM_SPECTATOR,	  // still run into walls
	PM_DEAD,		  // no acceleration or turning, but free falling
	PM_FREEZE,		  // stuck in place with no control
	PM_INTERMISSION,  // no movement or status bar
	PM_SPINTERMISSION // unused
} pmtype_t;

typedef enum {
	WEAPON_READY,
	WEAPON_RAISING,
	WEAPON_DROPPING,
	WEAPON_FIRING
} weaponstate_t;

// pmove->pm_flags
#define	PMF_DUCKED			1
#define	PMF_JUMP_HELD		2
#define	PMF_BACKWARDS_JUMP	8		// go into backwards land
#define	PMF_BACKWARDS_RUN	16		// coast down to backwards run
#define	PMF_TIME_LAND		32		// pm_time is time before rejump
#define	PMF_TIME_KNOCKBACK	64		// pm_time is an air-accelerate only time
#define	PMF_TIME_WATERJUMP	256		// pm_time is waterjump
#define	PMF_RESPAWNED		512		// clear after attack and jump buttons come up
#define	PMF_USE_ITEM_HELD	1024
#define PMF_GRAPPLE_PULL	2048	// pull towards grapple location
#define PMF_FOLLOW			4096	// spectate following another player
#define PMF_SCOREBOARD		8192	// spectate as a scoreboard
#define PMF_INVULEXPAND     16384   // unused
#define PMF_DUCK_HELD		32768   // SLK: for crouchsliding

#define	PMF_ALL_TIMES	    (PMF_TIME_WATERJUMP|PMF_TIME_LAND|PMF_TIME_KNOCKBACK)

#define	MAXTOUCH        	32
typedef struct {
	// state (in / out)
	playerState_t	*ps;

	// command (in)
	usercmd_t	cmd;
	int			tracemask;			// collide against these types of surfaces
	int			debugLevel;			// if set, diagnostic output will be printed
	qbool	    gauntletHit;		// true if a gauntlet attack would actually hit something

	int			framecount;

	// results (out)
	int			numtouch;
	int			touchents[MAXTOUCH];

	vec3_t		mins, maxs;			// bounding box size

	int			watertype;
	int			waterlevel;

	// for fixed msec Pmove
	int			pmove_fixed;
	int			pmove_msec;

	// callbacks to test the world
	// these will be different functions during game and cgame
	void		(*trace)( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentMask );
	int			(*pointcontents)( const vec3_t point, int passEntityNum );
} pmove_t;

// if a full pmove isn't done on the client, you can just update the angles
void PM_UpdateViewAngles( playerState_t *ps, const usercmd_t *cmd );
void Pmove (pmove_t *pmove);
void BG_SharedCvarsUpdate ( int gt, int rs, int dfmode, int fcmode, int obs, int interference, int rev );    // SLK: update shared game cvars/configstrings

//===================================================================================


// player_state->stats[] indexes
// NOTE: may not have more than 16
typedef enum {
	STAT_HEALTH,
	STAT_HOLDABLE_ITEM,
	STAT_WEAPONS,			// 16 bit fields
	STAT_ARMOR,
	STAT_DEAD_YAW,			// look this direction when dead (FIXME: get rid of?)
	STAT_CLIENTS_READY,		// bit mask of clients wishing to exit the intermission (FIXME: configstring?)
	STAT_MAX_HEALTH,    	// health / armor limit, changable by handicap
	STAT_JUMPTIME,          // for cpm movement
	STAT_DFX_FLAG,          //
    STAT_DFTIMERHI,         // for defrag timer
	STAT_DFTIMERLO,         // ""
    STAT_CROUCHTIME,        // for crouchsliding
    STAT_WJTIME,            // for walljumping
	STAT_WJCOUNT            // ""
} statIndex_t;

typedef enum {
    DFXF_STAIRJUMP  = 1,    // for cpm stair jumping
    DFXF_KEYFORWARD = 2,    // handling keypresses
    DFXF_KEYBACK    = 4,    // ***
    DFXF_KEYLEFT    = 8,
    DFXF_KEYRIGHT   = 16,
    DFXF_KEYJUMP    = 32,
    DFXF_KEYCROUCH  = 64,
} dfx_flagsIndex_t;


// player_state->persistant[] indexes
// these fields are the only part of player_state that isn't
// cleared on respawn
// NOTE: may not have more than 16
typedef enum {
	PERS_SCORE,						// !!! MUST NOT CHANGE, SERVER AND GAME BOTH REFERENCE !!!
	PERS_HITS,						// total points damage inflicted so damage beeps can sound on change
	PERS_RANK,						// player rank or team rank
	PERS_TEAM,						// player team
	PERS_SPAWN_COUNT,				// incremented every respawn
	PERS_PLAYEREVENTS,				// 16 bits that can be flipped for events
	PERS_ATTACKER,					// clientnum of last damage inflicter
	PERS_ATTACKEE_ARMOR,			// health/armor of last person we attacked
	PERS_KILLED,					// count of the number of times you died
	// player awards tracking
	PERS_IMPRESSIVE_COUNT,			// two railgun hits in a row
	PERS_EXCELLENT_COUNT,			// two successive kills in a short amount of time
	PERS_DEFEND_COUNT,				// defend awards
	PERS_ASSIST_COUNT,				// assist awards
	PERS_GAUNTLET_FRAG_COUNT,		// kills with the gauntlet
	/*PERS_CAPTURES,*/				// captures
	PERS_SECRETS,                   // levels secrets found
	PERS_KEYS
} persEnum_t;

// entityState_t->eFlags
#define	EF_DEAD				0x00000001		// don't draw a foe marker over players with EF_DEAD
#define	EF_TELEPORT_BIT		0x00000004		// toggled every time the origin abruptly changes
#define	EF_AWARD_EXCELLENT	0x00000008		// draw an excellent sprite
#define EF_PLAYER_EVENT		0x00000010
#define	EF_BOUNCE			0x00000010		// for missiles
#define	EF_BOUNCE_HALF		0x00000020		// for missiles
#define	EF_AWARD_GAUNTLET	0x00000040		// draw a gauntlet sprite
#define	EF_NODRAW			0x00000080		// may have an event, but no model (unspawned items)
#define	EF_FIRING			0x00000100		// for lightning gun
#define EF_KAMIKAZE         0x00000200      // unused
#define	EF_MOVER_STOP		0x00000400		// will push otherwise
#define EF_AWARD_CAP		0x00000800		// draw the capture sprite
#define	EF_TALK				0x00001000		// draw a talk balloon
#define	EF_CONNECTION		0x00002000		// draw a connection trouble sprite
#define	EF_VOTED			0x00004000		// already cast a vote
#define	EF_AWARD_IMPRESSIVE	0x00008000		// draw an impressive sprite
#define	EF_AWARD_DEFEND		0x00010000		// draw a defend sprite
#define	EF_AWARD_ASSIST		0x00020000		// draw a assist sprite
#define EF_AWARD_DENIED     0x00040000      // unused
#define EF_TEAMVOTED        0x00080000      // unused

#define EF_PERSISTANT       ( EF_CONNECTION | EF_VOTED )
#define EF_AWARDS           ( EF_AWARD_IMPRESSIVE | EF_AWARD_EXCELLENT | EF_AWARD_GAUNTLET | EF_AWARD_ASSIST | EF_AWARD_DEFEND | EF_AWARD_CAP )
#define EF_NOPREDICT        ( EF_AWARDS | EF_PERSISTANT | EF_TALK )

// NOTE: may not have more than 16
typedef enum {
	PW_NONE,

	PW_QUAD,
	PW_BATTLESUIT,
	PW_HASTE,
	PW_INVIS,
	PW_REGEN,
	PW_FLIGHT,

	PW_REDFLAG,
	PW_BLUEFLAG,
	PW_NEUTRALFLAG,

	PW_NUM_POWERUPS
} powerup_t;

typedef enum {
	HI_NONE,

	HI_TELEPORTER,
	HI_MEDKIT,

	HI_NUM_HOLDABLE
} holdable_t;


typedef enum {
	WP_NONE,

	WP_GAUNTLET,
	WP_MACHINEGUN,
	WP_SHOTGUN,
	WP_GRENADE_LAUNCHER,
	WP_ROCKET_LAUNCHER,
	WP_LIGHTNING,
	WP_RAILGUN,
	WP_PLASMAGUN,
	WP_BFG,
	WP_GRAPPLING_HOOK,

	WP_NUM_WEAPONS,
	WP_PENDING = WP_NUM_WEAPONS, // used in ui_players.c
	WP_MAX_WEAPONS = 16
} weapon_t;


// reward sounds (stored in ps->persistant[PERS_PLAYEREVENTS])
#define PLAYEREVENT_DENIEDREWARD		0x0001
#define PLAYEREVENT_GAUNTLETREWARD	    0x0002
#define PLAYEREVENT_HOLYSHIT			0x0004

// entityState_t->event values
// entity events are for effects that take place relative
// to an existing entities origin.  Very network efficient.

// two bits at the top of the entityState->event field
// will be incremented with each change in the event so
// that an identical event started twice in a row can
// be distinguished.  And off the value with ~EV_EVENT_BITS
// to retrieve the actual event number
#define	EV_EVENT_BIT1		0x00000100
#define	EV_EVENT_BIT2		0x00000200
#define	EV_EVENT_BITS		(EV_EVENT_BIT1|EV_EVENT_BIT2)
#define	EV_VALID_MSEC	    300

typedef enum {
	EV_NONE,

	EV_FOOTSTEP,
	EV_FOOTSTEP_METAL,
	EV_FOOTSTEP_FLESH,
	EV_FOOTSTEP_STONE,
	EV_FOOTSTEP_GRASS,
	EV_FOOTSTEP_TALLGRASS,
	EV_FOOTSTEP_WOOD,
	EV_FOOTSTEP_SAND,
	EV_FOOTSTEP_ENERGY,
	EV_FOOTSTEP_LAVA,
	EV_FOOTSTEP_ICE,

	EV_FOOTSPLASH,
	EV_FOOTWADE,
	EV_SWIM,

	EV_STEP_4,
	EV_STEP_8,
	EV_STEP_12,
	EV_STEP_16,

	EV_FALL_SHORT,
	EV_FALL_MEDIUM,
	EV_FALL_FAR,

	EV_JUMP_PAD,			// boing sound at origin, jump sound on player

	EV_JUMP,
	EV_WATER_TOUCH,	        // foot touches
	EV_WATER_LEAVE,     	// foot leaves
	EV_WATER_UNDER,	        // head touches
	EV_WATER_CLEAR,	        // head leaves

	EV_ITEM_PICKUP,			// normal item pickups are predictable
	EV_GLOBAL_ITEM_PICKUP,	// powerup / team sounds are broadcast to everyone

	EV_NOAMMO,
	EV_CHANGE_WEAPON,
	EV_FIRE_WEAPON,

	EV_USE_ITEM0,
	EV_USE_ITEM1,
	EV_USE_ITEM2,
	EV_USE_ITEM3,
	EV_USE_ITEM4,
	EV_USE_ITEM5,
	EV_USE_ITEM6,
	EV_USE_ITEM7,
	EV_USE_ITEM8,
	EV_USE_ITEM9,
	EV_USE_ITEM10,
	EV_USE_ITEM11,
	EV_USE_ITEM12,
	EV_USE_ITEM13,
	EV_USE_ITEM14,
	EV_USE_ITEM15,

	EV_ITEM_RESPAWN,
	EV_ITEM_POP,
	EV_PLAYER_TELEPORT_IN,
	EV_PLAYER_TELEPORT_OUT,

	EV_GRENADE_BOUNCE,		// eventParm will be the soundindex

	EV_GENERAL_SOUND,
	EV_GLOBAL_SOUND,		// no attenuation
	EV_GLOBAL_TEAM_SOUND,

	EV_BULLET_HIT_FLESH,
	EV_BULLET_HIT_WALL,

	EV_MISSILE_HIT,
	EV_MISSILE_MISS,
	EV_MISSILE_MISS_METAL,
	EV_RAILTRAIL,
	EV_SHOTGUN,
	EV_BULLET,				// otherEntity is the shooter

	EV_PAIN,
	EV_DEATH1,
	EV_DEATH2,
	EV_DEATH3,
	EV_OBITUARY,

	EV_POWERUP_QUAD,
	EV_POWERUP_BATTLESUIT,
	EV_POWERUP_REGEN,

	EV_GIB_PLAYER,			// gib a previously living player

	EV_DEBUG_LINE,
	EV_STOPLOOPINGSOUND,

	EV_TAUNT,
	EV_TAUNT_YES,
	EV_TAUNT_NO,
	EV_TAUNT_FOLLOWME,
	EV_TAUNT_GETFLAG,
	EV_TAUNT_GUARDBASE,
	EV_TAUNT_PATROL,

    EV_BREAKABLE,
    EV_EARTHQUAKE,
    EV_EXPLOSION,
    EV_PARTICLES_GRAVITY,
	EV_PARTICLES_LINEAR,
    EV_PARTICLES_LINEAR_UP,
	EV_PARTICLES_LINEAR_DOWN,

    EV_LANDSOUND,
	EV_LANDSOUND_METAL,
	EV_LANDSOUND_FLESH,
	EV_LANDSOUND_STONE,
	EV_LANDSOUND_GRASS,
	EV_LANDSOUND_TALLGRASS,
	EV_LANDSOUND_WOOD,
	EV_LANDSOUND_SAND,
	EV_LANDSOUND_LAVA,
	EV_LANDSOUND_ICE,

	EV_MAX

} entity_event_t;


typedef enum {
	GTS_RED_CAPTURE,
	GTS_BLUE_CAPTURE,
	GTS_RED_RETURN,
	GTS_BLUE_RETURN,
	GTS_RED_TAKEN,
	GTS_BLUE_TAKEN,
	GTS_REDOBELISK_ATTACKED,
	GTS_BLUEOBELISK_ATTACKED,
	GTS_REDTEAM_SCORED,
	GTS_BLUETEAM_SCORED,
	GTS_REDTEAM_TOOK_LEAD,
	GTS_BLUETEAM_TOOK_LEAD,
	GTS_TEAMS_ARE_TIED,
	GTS_KAMIKAZE
} global_team_sound_t;

// animations
typedef enum {
	BOTH_DEATH1,
	BOTH_DEAD1,
	BOTH_DEATH2,
	BOTH_DEAD2,
	BOTH_DEATH3,
	BOTH_DEAD3,

	TORSO_GESTURE,

	TORSO_ATTACK,
	TORSO_ATTACK2,

	TORSO_DROP,
	TORSO_RAISE,

	TORSO_STAND,
	TORSO_STAND2,

	LEGS_WALKCR,
	LEGS_WALK,
	LEGS_RUN,
	LEGS_BACK,
	LEGS_SWIM,

	LEGS_JUMP,
	LEGS_LAND,

	LEGS_JUMPB,
	LEGS_LANDB,

	LEGS_IDLE,
	LEGS_IDLECR,

	LEGS_TURN,

	TORSO_GETFLAG,
	TORSO_GUARDBASE,
	TORSO_PATROL,
	TORSO_FOLLOWME,
	TORSO_AFFIRMATIVE,
	TORSO_NEGATIVE,

	MAX_ANIMATIONS,

	LEGS_BACKCR,
	LEGS_BACKWALK,
	FLAG_RUN,
	FLAG_STAND,
	FLAG_STAND2RUN,

	MAX_TOTALANIMATIONS
} animNumber_t;


typedef struct animation_s {
	int		firstFrame;
	int		numFrames;
	int		loopFrames;			// 0 to numFrames
	int		frameLerp;			// msec between frames
	int		initialLerp;		// msec to get to first frame
	int		reversed;			// true if animation is reversed
	int		flipflop;			// true if animation should flipflop back to base
} animation_t;


// flip the togglebit every time an animation
// changes so a restart of the same anim can be detected
#define	ANIM_TOGGLEBIT		128

typedef enum {
	TEAM_FREE,
	TEAM_RED,
	TEAM_BLUE,
	TEAM_GREEN,
	TEAM_YELLOW,
	TEAM_SPECTATOR,
	
	TEAM_NUM_TEAMS
} team_t;

typedef enum {
	TAG_NONE = 0,
	TAG_DONTSPAWN,
} tag_t;

// means of death
typedef enum {
	MOD_UNKNOWN,
	MOD_SHOTGUN,
	MOD_GAUNTLET,
	MOD_MACHINEGUN,
	MOD_GRENADE,
	MOD_GRENADE_SPLASH,
	MOD_ROCKET,
	MOD_ROCKET_SPLASH,
	MOD_PLASMA,
	MOD_PLASMA_SPLASH,
	MOD_RAILGUN,
	MOD_LIGHTNING,
	MOD_BFG,
	MOD_BFG_SPLASH,
	MOD_WATER,
	MOD_SLIME,
	MOD_LAVA,
	MOD_CRUSH,
	MOD_TELEFRAG,
	MOD_FALLING,
	MOD_SUICIDE,
	MOD_TARGET_LASER,
	MOD_TRIGGER_HURT,
	MOD_GRAPPLE
} meansOfDeath_t;


//---------------------------------------------------------

// gitem_t->type
typedef enum {
	IT_BAD,
	IT_WEAPON,				// EFX: rotate + upscale + minlight
	IT_AMMO,				// EFX: rotate
	IT_ARMOR,				// EFX: rotate + minlight
	IT_HEALTH,				// EFX: static external sphere + rotating internal
	IT_POWERUP,				// instant on, timer based
							// EFX: rotate + external ring that rotates
	IT_HOLDABLE,			// single use, holdable item
							// EFX: rotate + bob
	IT_PERSISTANT_POWERUP,
	IT_TEAM,

	IT_KEY
} itemType_t;

#define MAX_ITEM_MODELS 4

typedef struct gitem_s {
	const char	*classname;	     // spawning name
	char		*pickup_sound;
	char		*world_model[MAX_ITEM_MODELS];
	char		*icon;
	char		*pickup_name;	// for printing on pickup
	int			quantity;		// for ammo how much, or duration of powerup
	itemType_t  giType;			// IT_* flags
	int			const giTag;
	char		const *precaches;	// string of all models and images this item will use
	char		const *sounds;		// string of all sounds this item will use
} gitem_t;

// included in both the game dll and the client
#define	ITEM_INDEX(x) ((x)-bg_itemlist)

gitem_t	    *BG_FindItem( const char *pickupName );
gitem_t	    *BG_FindItemForWeapon( weapon_t weapon );
gitem_t	    *BG_FindItemForPowerup( powerup_t pw );
gitem_t	    *BG_FindItemForHoldable( holdable_t pw );
gitem_t	    *BG_FindItemForClassname( const char *classname );
qbool	    BG_CanItemBeGrabbed( int gametype, const entityState_t *ent, const playerState_t *ps );
qbool   	BG_CopyFile( const char *srcFilename, const char *dstFilename );


extern gitem_t bg_itemlist[];
extern int	bg_numItems;

// g_dmflags->integer flags
#define	DF_NO_FALLING			8

// content masks
#define	MASK_ALL				(-1)
#define	MASK_SOLID				(CONTENTS_SOLID)
#define	MASK_PLAYERSOLID		(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_BODY)
#define	MASK_DEADSOLID			(CONTENTS_SOLID|CONTENTS_PLAYERCLIP)
#define	MASK_WATER				(CONTENTS_WATER|CONTENTS_LAVA|CONTENTS_SLIME)
#define	MASK_OPAQUE				(CONTENTS_SOLID|CONTENTS_SLIME|CONTENTS_LAVA)
#define	MASK_SHOT				(CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE)


//
// entityState_t->eType
//
typedef enum {
	ET_GENERAL,
	ET_PLAYER,
	ET_ITEM,
	ET_MISSILE,
	ET_MOVER,
    ET_BREAKABLE,
	ET_BEAM,
	ET_PORTAL,
	ET_SPEAKER,
	ET_PUSH_TRIGGER,
	ET_TELEPORT_TRIGGER,
	ET_PHYSICS_TRIGGER,
	ET_INVISIBLE,
	ET_GRAPPLE,				// grapple hooked on wall
    ET_SPAWNPOINT,
	ET_TEAM,

	ET_EVENTS				// any of the EV_* events can be added freestanding
							// by setting eType to ET_EVENTS + eventNum
							// this avoids having to set eFlags and eventNum
} entityType_t;

// for BG_TouchPhysicsTrigger
// ideally there would be a table of strings and variable addresses
// but certain things (e.g., ps->speed) depend on the playerstate
// passed to BG_TouchPhysicsTrigger
enum {
	PHYSICS_NONE,
	PHYSICS_GRAVITY,
	PHYSICS_SPEED,
	PHYSICS_FRICTION
};

// calculated by modulus.c for appropriate dividers:
#define TMOD_004  4272943
#define TMOD_075  2292106
#define TMOD_1000 5730265
#define TMOD_2000 5730265

void	    BG_EvaluateTrajectory( const trajectory_t *tr, int atTime, vec3_t result );
void	    BG_EvaluateTrajectoryDelta( const trajectory_t *tr, int atTime, vec3_t result );
void	    BG_AddPredictableEventToPlayerstate( entity_event_t newEvent, int eventParm, playerState_t *ps, int entityNum );
void	    BG_TouchJumpPad( playerState_t *ps, entityState_t *jumppad );
void	    BG_TouchVelocityJumpPad( playerState_t *ps, entityState_t *jumppad );
void	    BG_TouchPhysicsTrigger( playerState_t *ps, entityState_t *s );
void	    BG_PlayerStateToEntityState( playerState_t *ps, entityState_t *s, qbool snap );
void	    BG_PlayerStateToEntityStateExtraPolate( playerState_t *ps, entityState_t *s, int time, qbool snap );
qbool	    BG_PlayerTouchesItem( playerState_t *ps, entityState_t *item, int atTime );

char	    *FormatDFTimer( int time, qbool sign );
void	    SetItemAvailable( entityState_t *es, int clientNum, qbool available );
qbool       IsItemAvailable( entityState_t *es, int clientNum );

#define MAX_ARENAS			4096
#define	MAX_ARENAS_TEXT		8192


// custom functions
int         BG_sprintf( char *buf, const char *format, ... );
int         ED_vsprintf( char *buffer, const char *fmt, va_list argptr );
char        *Q_stristr( const char * str1, const char * str2 );
char        *strtok( char *strToken, const char *strDelimit );
char        *EncodedString( const char *str );
char        *DecodedString( const char *str );
void        BG_CleanName( const char *in, char *out, int outSize, const char *blankString );
char        *BG_StripColor( char *string );
void        Q_strcpy( char *dst, const char *src );
char        *Q_stradd( char *dst, const char *src );
int         Q_sscanf( const char *buffer, const char *fmt, ... );
qbool       replace1( const char match, const char replace, char *str );
qbool       BigEndian( void );
