// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "../game/q_shared.h"
#include "tr_types.h"
#include "../game/bg_public.h"
#include "cg_public.h"


// The entire cgame module is unloaded and reloaded on each level change,
// so there is NO persistant data between levels on the client side.
// If you absolutely need something stored, it can either be kept
// by the server in the server stored userinfos, or stashed in a cvar.

#define BLACKOUT_TIME		2000.000	//time for the screen to remain black at start of game
#define	FADEIN_TIME			1200.000    //amount of time it takes for screen to fade in at start of game
#define TITLE_TIME			3500		//amount of time the level title stays on screen
#define TITLE_FADEIN_TIME	1000.000	//amount of time it takes for level title to fade in
#define TITLE_FADEOUT_TIME	1000.000	//amount of time it takes for level title to fade out

#define	POWERUP_BLINKS		5
#define	POWERUP_BLINK_TIME	1000
#define	FADE_TIME			200
#define	PULSE_TIME			200
#define	DAMAGE_DEFLECT_TIME	100
#define	DAMAGE_RETURN_TIME	400
#define DAMAGE_TIME			500
#define	LAND_DEFLECT_TIME	150
#define	LAND_RETURN_TIME	300
#define	STEP_TIME			200
#define	DUCK_TIME			100
#define	PAIN_TWITCH_TIME	200
#define	WEAPON_SELECT_TIME	1400
#define	ITEM_SCALEUP_TIME	1000
#define	ZOOM_TIME			150
#define	ITEM_BLOB_TIME		200
#define	MUZZLE_FLASH_TIME	20
#define	SINK_TIME			1000		// time for fragments to sink into ground before going away

#define	PULSE_SCALE			1.5			// amount to scale up the icons when activating
#define	MAX_STEP_CHANGE		32
#define	MAX_VERTS_ON_POLY	10
#define	MAX_MARK_POLYS		256
#define STAT_MINUS			10	        // num frame for '-' stats digit

#define	ICON_SIZE			20
#define	CHAR_WIDTH			12
#define	CHAR_HEIGHT			20
#define	TEXT_ICON_SPACE		4

#define	TEAMCHAT_WIDTH		80
#define TEAMCHAT_HEIGHT		8

// very large characters
#define	GIANT_WIDTH			32
#define	GIANT_HEIGHT		48

#define	NUM_CROSSHAIRS		19

#define	DEFAULT_MODEL			    "sarge"
#define	DEFAULT_TEAM_MODEL		    "sarge"
#define	DEFAULT_TEAM_HEAD		    "sarge"
#define DEFAULT_REDTEAM_NAME		"Stroggs"
#define DEFAULT_BLUETEAM_NAME		"Pagans"

// dynamic lights definitions
#define GL_EXPLOSION_RADIUS		300
#define RL_EXPLOSION_RADIUS		300
#define BFG_EXPLOSION_RADIUS	300

// holdable powerups
#define POWERUP_GLOW_RADIUS		200
#define POWERUP_GLOW_RADIUS_MOD	31

// running projectiles
#define MISSILE_GLOW_RADIUS		200

// on-hand firing weapons
#define MG_FLASH_RADIUS			200 // original: 300
#define WEAPON_FLASH_RADIUS		300
#define WEAPON_FLASH_RADIUS_MOD	31

typedef enum {
    FOOTSTEP_NORMAL,
	FOOTSTEP_BOOT,
	FOOTSTEP_FLESH,
	FOOTSTEP_MECH,
	FOOTSTEP_ENERGY,
	FOOTSTEP_METAL,
	FOOTSTEP_SPLASH,
// new ones below -------
	FOOTSTEP_STONE,
	FOOTSTEP_GRASS,
	FOOTSTEP_TALLGRASS,
	FOOTSTEP_WOOD,
	FOOTSTEP_SAND,
	FOOTSTEP_LAVA,
	FOOTSTEP_ICE,

	FOOTSTEP_TOTAL
} footstep_t;

// SLK custom new landing sounds according to the groundsurface
typedef enum {
    LANDSOUND_NORMAL,
    LANDSOUND_FLESH,
    LANDSOUND_METAL,
    LANDSOUND_SPLASH,
    LANDSOUND_STONE,
    LANDSOUND_GRASS,
    LANDSOUND_TALLGRASS,
    LANDSOUND_WOOD,
    LANDSOUND_SAND,
    LANDSOUND_LAVA,
    LANDSOUND_ICE,

    LANDSOUND_TOTAL
} landsound_t;

typedef enum {
	IMPACTSOUND_DEFAULT,
	IMPACTSOUND_METAL,
	IMPACTSOUND_FLESH
} impactSound_t;

//=================================================

// player entities need to track more information
// than any other type of entity.

// note that not every player entity is a client entity,
// because corpses after respawn are outside the normal
// client numbering range

// when changing animation, set animationTime to frameTime + lerping time
// The current lerp will finish out, then it will lerp to the new animation
typedef struct {
	int			oldFrame;
	int			oldFrameTime;		// time when ->oldFrame was exactly on

	int			frame;
	int			frameTime;			// time when ->frame will be exactly on

	float		backlerp;

	float		yawAngle;
	qbool	    yawing;
	float		pitchAngle;
	qbool	    pitching;

	int			animationNumber;	// may include ANIM_TOGGLEBIT
	animation_t	*animation;
	int			animationTime;		// time when the first frame of the animation will be exact
} lerpFrame_t;


typedef struct {
	lerpFrame_t		legs, torso, flag;
	int				painTime;
	int				painDirection;	// flip from 0 to 1
	qbool		    painIgnore;
	int				lightningFiring;

	// railgun trail spawning
	vec3_t			railgunImpact;
	qbool		    railgunFlash;

	// machinegun spinning
	float			barrelAngle;
	int				barrelTime;
	qbool		    barrelSpinning;
} playerEntity_t;

//=================================================



// centity_t have a direct corespondence with gentity_t in the game, but
// only the entityState_t is directly communicated to the cgame
typedef struct centity_s {
	entityState_t	currentState;	// from cg.frame
	entityState_t	nextState;		// from cg.nextFrame, if available
	qbool		    interpolate;	// true if next is valid to interpolate to
	qbool		    currentValid;	// true if cg.frame holds this entity

	int				muzzleFlashTime;	// move to playerEntity?
	int				previousEvent;
	int				teleportFlag;

	int				trailTime;		// so missile trails can handle dropped initial packets
	int				dustTrailTime;
	int				miscTime;
	int				delaySpawn;
	qbool		    delaySpawnPlayed;

	int				snapShotTime;	// last time this entity was found in a snapshot

	playerEntity_t	pe;

	int				errorTime;		// decay the error from this time
	vec3_t			errorOrigin;
	vec3_t			errorAngles;

	qbool		    extrapolated;	// false if origin / angles is an interpolation
	vec3_t			rawOrigin;
	vec3_t			rawAngles;

	vec3_t			beamEnd;

	// exact interpolated position of entity on this frame
	vec3_t			lerpOrigin;
	vec3_t			lerpAngles;
} centity_t;


//======================================================================

// local entities are created as a result of events or predicted actions,
// and live independantly from all server transmitted entities

typedef struct markPoly_s {
	struct markPoly_s	*prevMark, *nextMark;
	int			time;
	qhandle_t	markShader;
	qbool	    alphaFade;		// fade alpha instead of rgb
	float		color[4];
	poly_t		poly;
	polyVert_t	verts[MAX_VERTS_ON_POLY];
} markPoly_t;


typedef enum {
	LE_MARK,
	LE_EXPLOSION,
	LE_SPRITE_EXPLOSION,
	LE_FRAGMENT,
	LE_MOVE_SCALE_FADE,
	LE_FALL_SCALE_FADE,
	LE_FADE_RGB,
	LE_SCALE_FADE,
	LE_SCOREPLUM,       //unused
	LE_SHOWREFENTITY
} leType_t;

typedef enum {
	PT_GRAVITY,
	PT_LINEAR_UP,
	PT_LINEAR_DOWN,
	PT_LINEAR_BOTH
} particleType_t;

typedef enum {
	LEF_PUFF_DONT_SCALE  = 0x0001,			// do not scale size over time
	LEF_TUMBLE			 = 0x0002,			// tumble over time, used for ejecting shells
	LEF_SOUND1			 = 0x0004,			// unused
	LEF_SOUND2			 = 0x0008			// unused
} leFlag_t;

typedef enum {
	LEMT_NONE,
	LEMT_BURN,
	LEMT_BLOOD
} leMarkType_t;			// fragment local entities can leave marks on walls

typedef enum {
	LEBS_NONE,
	LEBS_BLOOD,
	LEBS_BRASS
} leBounceSoundType_t;	// fragment local entities can make sounds on impacts

typedef struct localEntity_s {
	struct localEntity_s	*prev, *next;
	leType_t		leType;
	int				leFlags;

	int				startTime;
	int				endTime;
	int				fadeInTime;

	float			lifeRate;			// 1.0 / (endTime - startTime)

	trajectory_t	pos;
	trajectory_t	angles;

	float			bounceFactor;		// 0.0 = no bounce, 1.0 = perfect

	float			color[4];

	float			radius;

	float			light;
	vec3_t			lightColor;

	leMarkType_t		leMarkType;		// mark to leave on fragment impact
	leBounceSoundType_t	leBounceSoundType;

	refEntity_t		refEntity;
} localEntity_t;

//======================================================================


typedef struct {
	int				client;
	int				score;
	int				ping;
	int				time;
	int				scoreFlags;
	int				powerUps;
	int				accuracy;
	int				impressiveCount;
	int				excellentCount;
	int				gauntletCount;
	int				defendCount;
	int				assistCount;
	//int				captures;
	int             keys;
	qbool		    perfect;
	team_t			team;

	int				minx, maxx;
	int				miny, maxy;
} score_t;

// each client has an associated clientInfo_t
// that contains media references necessary to present the
// client model and other color coded effects
// this is regenerated each time a client's configstring changes,
// usually as a result of a userinfo (name, model, etc) change
#define	MAX_CUSTOM_SOUNDS	32

typedef struct {
	qbool		    infoValid;

	char			name[MAX_NAME_LENGTH];
	team_t			team;

	int				botSkill;		// 0 = not bot, 1-5 = bot

	vec3_t			color1;
	vec3_t			color2;

	int				score;			// updated by score servercmds
	int				location;		// location index for team mode
	int				health;			// you only get this info about your teammates
	int				armor;
	int				curWeapon;

	int				handicap;
	int				wins, losses;	// in tourney mode

	int				teamTask;		// task in teamplay (offence/defence)
	qbool		    teamLeader;		// true when this is a team leader

	int				powerups;		// so can display quad/flag status

	int				medkitUsageTime;

	// when clientinfo is changed, the loading of models/skins/sounds
	// can be deferred until you are dead, to prevent hitches in
	// gameplay
	char			modelName[MAX_QPATH];
	char			skinName[MAX_QPATH];
	char			headModelName[MAX_QPATH];
	char			headSkinName[MAX_QPATH];
	qbool		    deferred;

	qbool		    newAnims;		// true if using the new mission pack animations
	qbool		    fixedlegs;		// true if legs yaw is always the same as torso yaw
	qbool		    fixedtorso;		// true if torso never changes yaw

	vec3_t			headOffset;		// move head in icon views
	footstep_t		footsteps;
	gender_t		gender;			// from model

	qhandle_t		legsModel;
	qhandle_t		legsSkin;

	qhandle_t		torsoModel;
	qhandle_t		torsoSkin;

	qhandle_t		headModel;
	qhandle_t		headSkin;

	qhandle_t		modelIcon;

	animation_t		animations[MAX_TOTALANIMATIONS];

	sfxHandle_t		sounds[MAX_CUSTOM_SOUNDS];

	qbool		    coloredSkin;
	vec3_t			headColor;
	vec3_t			bodyColor;
	vec3_t			legsColor;

} clientInfo_t;


// each WP_* weapon enum has an associated weaponInfo_t
// that contains media references necessary to present the
// weapon and its effects
typedef struct weaponInfo_s {
	qbool		    registered;
	gitem_t			*item;

	qhandle_t		handsModel;			// the hands don't actually draw, they just position the weapon
	qhandle_t		weaponModel;
	qhandle_t		barrelModel;
	qhandle_t		flashModel;

	vec3_t			weaponMidpoint;		// so it will rotate centered instead of by tag

	float			flashDlight;
	vec3_t			flashDlightColor;
	sfxHandle_t		flashSound[4];		// fast firing weapons randomly choose

	qhandle_t		weaponIcon;
	qhandle_t		ammoIcon;

	qhandle_t		ammoModel;

	qhandle_t		missileModel;
	sfxHandle_t		missileSound;
	void			(*missileTrailFunc)( centity_t *, const struct weaponInfo_s *wi );
	float			missileDlight;
	vec3_t			missileDlightColor;
	int				missileRenderfx;

	void			(*ejectBrassFunc)( centity_t * );

	float			trailRadius;
	float			wiTrailTime;

	sfxHandle_t		readySound;
	sfxHandle_t		firingSound;
	qbool		    loopFireSound;
} weaponInfo_t;


// each IT_* item has an associated itemInfo_t
// that constains media references necessary to present the
// item and its effects
typedef struct {
	qbool		    registered;
	qhandle_t		models[MAX_ITEM_MODELS];
	qhandle_t		icon;
	qhandle_t		icon_df;
} itemInfo_t;


typedef struct {
	int				itemNum;
} powerupInfo_t;


#define MAX_REWARDSTACK		10
#define MAX_SOUNDBUFFER		20
#define MAX_CHECKPOINTS		64

//======================================================================

// all cg.stepTime, cg.duckTime, cg.landTime, etc are set to cg.time when the action
// occurs, and they will have visible effects for #define STEP_TIME or whatever msec after

#define MAX_PREDICTED_EVENTS	16
#define PICKUP_PREDICTION_DELAY 200
#define NUM_SAVED_STATES ( CMD_BACKUP + 2 )

typedef struct {
	int			clientFrame;		    // incremented each frame
	int			clientNum;
	qbool	    demoPlayback;
	int			deferredPlayerLoading;
	qbool	    loading;			    // don't defer players at initial startup
	qbool	    intermissionStarted;	// don't play voice rewards, because game will end shortly
                                        // there are only one or two snapshot_t that are relevent at a time
	int			latestSnapshotNum;	    // the number of snapshots the client system has received
	int			latestSnapshotTime;	    // the time from latestSnapshotNum, so we don't need to read the snapshot yet
	snapshot_t	*snap;				    // cg.snap->serverTime <= cg.time
	snapshot_t	*nextSnap;			    // cg.nextSnap->serverTime > cg.time, or NULL
	snapshot_t	activeSnapshots[2];

	float		frameInterpolation;	    // (float)( cg.time - cg.frame->serverTime ) / (cg.nextFrame->serverTime - cg.frame->serverTime)

	qbool	    thisFrameTeleport;
	qbool	    nextFrameTeleport;

	int			frametime;		        // cg.time - cg.oldTime
	int			time;			        // this is the time value that the client
                                        // is rendering at.
	int			oldTime;		        // time at last frame, used for missile trails and prediction checking
	int			physicsTime;	        // either cg.snap->time or cg.nextSnap->time
	int			fraglimitWarnings;
	qbool	    mapRestart;			    // set on a map restart to set back the weapon

	qbool	    renderingThirdPerson;	// during deaths, chasecams, etc

	// prediction state
	qbool	    hyperspace;				// true if prediction has hit a trigger_teleport
	playerState_t	predictedPlayerState;
	centity_t	predictedPlayerEntity;
	qbool	    validPPS;				// clear until the first call to CG_PredictPlayerState
	int			predictedErrorTime;
	vec3_t		predictedError;

	int			eventSequence;
	int			predictableEvents[MAX_PREDICTED_EVENTS];

	float		stepChange;				// for stair up smoothing
	int			stepTime;
	float		duckChange;				// for duck viewheight smoothing
	int			duckTime;
	float		landChange;				// for landing hard
	int			landTime;

	// input state sent to server
	int			weaponSelect;
	// auto rotating items
	vec3_t		autoAngles;
	vec3_t		autoAxis[3];
	vec3_t		autoAnglesFast;
	vec3_t		autoAxisFast[3];
	// view rendering
	refdef_t	refdef;
	vec3_t		refdefViewAngles;		// will be converted to refdef.viewaxis
	// zoom key
	qbool	    zoomed;
	int			zoomTime;
	float		zoomSensitivity;
	// information screen text during loading
	char		infoScreenText[MAX_STRING_CHARS];

	// scoreboard
	int			scoresRequestTime;
	int			numScores;
	int			selectedScore;
	int			teamScores[2];
	score_t		scores[MAX_CLIENTS];
	qbool	    showScores;
	qbool	    scoreBoardShowing;
	int			scoreFadeTime;
	char		killerName[MAX_NAME_LENGTH+32];
	int			killerTime;
	char		spectatorList[MAX_STRING_CHARS];	// list of names
	int			spectatorLen;						// length of list
	float		spectatorWidth;						// width in device units
	int			spectatorTime;						// next time to offset
	int			spectatorPaintX;					// current paint x
	int			spectatorPaintX2;					// current paint x
	int			spectatorOffset;					// current offset from start
	int			spectatorPaintLen; 					// current offset from start

	// centerprinting
	int			centerPrintTime;
	int			centerPrintCharWidth;
	int			centerPrintY;
	char		centerPrint[1024];
	int			centerPrintLines;

	// low ammo warning state
	int			lowAmmoWarning;		// 1 = low, 2 = empty
	// kill timers for carnage reward
	int			lastKillTime;
	// crosshair client ID
	int			crosshairClientNum;
	int			crosshairClientTime;
	// powerup active flashing
	int			powerupActive;
	int			powerupTime;
	// attacking player
	char		attackerName[MAX_NAME_LENGTH];
	int			attackerClientNum;
	int			attackerTime;
	int			voiceTime;
	// reward medals
	int			rewardStack;
	int			rewardTime;
	// sound buffer mainly for announcer sounds
	int			soundBufferIn;
	int			soundBufferOut;
	int			soundTime;
	qhandle_t	soundBuffer[MAX_SOUNDBUFFER];
	qhandle_t	soundPlaying;
	// warmup countdown
	int			warmup;
	int			warmupCount;
	int			warmupFightSound;	    // last time "Fight!" sound was played

	//==========================
	int			itemPickup;
	int			itemPickupCount;
	int			itemPickupTime;
	int			itemPickupBlendTime;	// the pulse around the crosshair is timed seperately

	int			weaponSelectTime;
	int			weaponAnimation;
	int			weaponAnimationTime;
	// blend blobs
	int			damageTime;
	float		damageX, damageY, damageValue;
	// status bar head
	float		headYaw;
	float		headEndPitch;
	float		headEndYaw;
	int			headEndTime;
	float		headStartPitch;
	float		headStartYaw;
	int			headStartTime;
	// view movement
	float		v_dmg_time;
	float		v_dmg_pitch;
	float		v_dmg_roll;
	vec3_t		kick_angles;	// weapon kicks
	vec3_t		kick_origin;
	// temp working variables for player view
	float		bobfracsin;
	int			bobcycle;
	float		xyspeed;
	// development tool
	refEntity_t	testModelEntity;
	char		testModelName[MAX_QPATH];
	qbool		testGun;
	// optimized prediction
	int				lastPredictedCommand;
	int				lastServerTime;
	playerState_t	savedPmoveStates[ NUM_SAVED_STATES ];
	int				stateHead, stateTail;

	int				meanPing;
	int				timeResidual;
	int				allowPickupPrediction;

	int				followTime;
	int				followClient;

	qbool		    skipDFshaders;
	int             chainLength;

    int				fadeTime;		//for timing the fade in at start

    // SLK: for Checkpoints
    qbool           newBestCheckpoints;
    int     		checkpoints[MAX_CHECKPOINTS];
	int		        bestCheckpoints[MAX_CHECKPOINTS];
	int		        currentCheckpoint;
} cg_t;


// all of the model, shader, and sound references that are
// loaded at gamestate time are stored in cgMedia_t
// Other media that can be tied to clients, weapons, or items are
// stored in the clientInfo_t, itemInfo_t, weaponInfo_t, and powerupInfo_t
typedef struct {
	qhandle_t	charsetShader;
	qhandle_t	charsetProp;
	qhandle_t	charsetPropGlow;
	qhandle_t	charsetPropB;
	qhandle_t	whiteShader;

	qhandle_t	redFlagModel;
	qhandle_t	blueFlagModel;
	//qhandle_t	neutralFlagModel;
	//qhandle_t	redFlagShader[3];
	//qhandle_t	blueFlagShader[3];
	//qhandle_t	flagShader[4];

	qhandle_t	flagPoleModel;
	qhandle_t	flagFlapModel;

	qhandle_t	redFlagFlapSkin;
	qhandle_t	blueFlagFlapSkin;
	qhandle_t	neutralFlagFlapSkin;

	//qhandle_t	redFlagBaseModel;
	//qhandle_t	blueFlagBaseModel;
	//qhandle_t	neutralFlagBaseModel;

	qhandle_t	armorModel;
	qhandle_t	armorIcon;
	qhandle_t	deferShader;

	// gib explosions
	qhandle_t	gibAbdomen;
	qhandle_t	gibArm;
	qhandle_t	gibChest;
	qhandle_t	gibFist;
	qhandle_t	gibFoot;
	qhandle_t	gibForearm;
	qhandle_t	gibIntestine;
	qhandle_t	gibLeg;
	qhandle_t	gibSkull;
	qhandle_t	gibBrain;

	qhandle_t	smoke2;

	qhandle_t	machinegunBrassModel;
	qhandle_t	shotgunBrassModel;

	qhandle_t	railRingsShader;
	qhandle_t	railCoreShader;

	qhandle_t	lightningShader;
	qhandle_t	balloonShader;
	qhandle_t	connectionShader;

	qhandle_t	selectShader;
	qhandle_t	viewBloodShader;
	qhandle_t	tracerShader;
	qhandle_t	crosshairShader[NUM_CROSSHAIRS];
	qhandle_t	lagometerShader;
	qhandle_t	noammoShader;

	qhandle_t	smokePuffShader;
	qhandle_t	smokePuffRageProShader;
	qhandle_t	shotgunSmokePuffShader;
	qhandle_t	plasmaBallShader;
	qhandle_t	waterBubbleShader;
	qhandle_t	bloodTrailShader;
	qhandle_t	numberShaders[11];
	qhandle_t	shadowMarkShader;

	// wall mark shaders
	qhandle_t	wakeMarkShader;
	qhandle_t	bloodMarkShader;
	qhandle_t	bulletMarkShader;
	qhandle_t	burnMarkShader;
	qhandle_t	holeMarkShader;
	qhandle_t	energyMarkShader;

	// powerup shaders
	qhandle_t	quadShader;
	qhandle_t	redQuadShader;
	qhandle_t	quadWeaponShader;
	qhandle_t	invisShader;
	qhandle_t	regenShader;
	qhandle_t	battleSuitShader;
	qhandle_t	battleWeaponShader;
	qhandle_t	hastePuffShader;

	// weapon effect models
	qhandle_t	bulletFlashModel;
	qhandle_t	ringFlashModel;
	qhandle_t	dishFlashModel;
	qhandle_t	lightningExplosionModel;
    qhandle_t	grapplingChainShader;

	// weapon effect shaders
	qhandle_t	railExplosionShader;
	qhandle_t	plasmaExplosionShader;
	qhandle_t	bulletExplosionShader;
	qhandle_t	rocketExplosionShader;
	qhandle_t	grenadeExplosionShader;
	qhandle_t	bfgExplosionShader;
	qhandle_t	bloodExplosionShader;

	// special effects models
	qhandle_t	teleportEffectModel;
	qhandle_t	teleportEffectShader;
	qhandle_t	sparkShader;

	// HUD shaders
    qhandle_t	unavailableItem;
    qhandle_t   healthIcon;
	qhandle_t   clockIcon;
	qhandle_t   keyIcon;
	qhandle_t   timerFrame;
	qhandle_t   checkpointFrame;
		
	// SB shaders
	qhandle_t	sbback;
	qhandle_t	sbbottom;
	qhandle_t	sbheader;
	
	qhandle_t	sbrocket;
	qhandle_t	sbplasma;
	qhandle_t	sbgrenade;
	qhandle_t	sbslick;
	qhandle_t	sbtele;
	qhandle_t	sbpad;
	qhandle_t	sbdoor;
	qhandle_t	sboverbounce;
	qhandle_t	sbinterference;
	qhandle_t   sbreverse;

	// sounds
	sfxHandle_t	quadSound;
	sfxHandle_t	tracerSound;
	sfxHandle_t	selectSound;
	sfxHandle_t	useNothingSound;
	sfxHandle_t	wearOffSound;
	sfxHandle_t	footsteps[FOOTSTEP_TOTAL][4];
	sfxHandle_t	sfx_lghit1;
	sfxHandle_t	sfx_lghit2;
	sfxHandle_t	sfx_lghit3;
	sfxHandle_t	sfx_ric1;
	sfxHandle_t	sfx_ric2;
	sfxHandle_t	sfx_ric3;
	sfxHandle_t	sfx_railg;
	sfxHandle_t	sfx_rockexp;
	sfxHandle_t	sfx_plasmaexp;
    sfxHandle_t	sfx_hookhit;

	sfxHandle_t	gibSound;
	sfxHandle_t	gibBounce1Sound;
	sfxHandle_t	gibBounce2Sound;
	sfxHandle_t	gibBounce3Sound;
	sfxHandle_t	teleInSound;
	sfxHandle_t	teleOutSound;
	sfxHandle_t	noAmmoSound;
	sfxHandle_t	respawnSound;
	sfxHandle_t talkSound;
	sfxHandle_t landSound[LANDSOUND_TOTAL];
	sfxHandle_t jumpPadSound;

	sfxHandle_t takenLeadSound;
	sfxHandle_t tiedLeadSound;
	sfxHandle_t lostLeadSound;
	sfxHandle_t flagPickup;

	sfxHandle_t watrInSound;
	sfxHandle_t watrOutSound;
	sfxHandle_t watrUnSound;

	sfxHandle_t flightSound;
	sfxHandle_t medkitSound;

	sfxHandle_t	countSound;
	
	qhandle_t cursor;

	sfxHandle_t	regenSound;
	sfxHandle_t	protectSound;
	sfxHandle_t	n_healthSound;
	sfxHandle_t	hgrenb1aSound;
	sfxHandle_t	hgrenb2aSound;
	sfxHandle_t	wstbimplSound;
	sfxHandle_t	wstbimpmSound;
	sfxHandle_t	wstbimpdSound;
	sfxHandle_t	wstbactvSound;

} cgMedia_t;


// The client game static (cgs) structure hold everything
// loaded or calculated from the gamestate.  It will NOT
// be cleared when a tournament restart is done, allowing
// all clients to begin playing instantly
typedef struct {
	gameState_t		gameState;			// gamestate from server
	glconfig_t		glconfig;			// rendering configuration
	float			screenXScale;		// derived from glconfig
	float			screenYScale;

	float			cursorScaleR;		// clamped 1/screenXScale

	float			screenXBias;
	float			screenYBias;

	float			screenXmin;
	float			screenXmax;

	float			screenYmin;
	float			screenYmax;

	int				serverCommandSequence;	// reliable command stream counter
	int				processedSnapshotNum;   // the number of snapshots cgame has requested

	qbool		    localServer;		    // detected on startup by checking sv_running

	// parsed from serverinfo
	int				dmflags;
	int				teamflags;
	int				fraglimit;
	int				timelimit;
	int				maxclients;
	char			mapname[MAX_QPATH];
	//char			redTeam[MAX_QPATH];
	//char			blueTeam[MAX_QPATH];

	int				voteTime;
	int				voteYes;
	int				voteNo;
	qbool		    voteModified;			// beep whenever changed
	char			voteString[MAX_STRING_TOKENS];

	int				levelStartTime;

	int				scores1, scores2, scores3, scores4;		// from configstrings
	int				redflag, blueflag;		// flag status from configstrings
	int				flagStatus;


	//
	// locally derived information from gamestate
	//
	qhandle_t		gameModels[MAX_MODELS];
	sfxHandle_t		gameSounds[MAX_SOUNDS];

	int				numInlineModels;
	qhandle_t		inlineDrawModel[MAX_MODELS];
	vec3_t			inlineModelMidpoints[MAX_MODELS];

	clientInfo_t	clientinfo[MAX_CLIENTS];

	// teamchat width is *3 because of embedded color codes
	char			teamChatMsgs[TEAMCHAT_HEIGHT][TEAMCHAT_WIDTH*3+1];
	int				teamChatMsgTimes[TEAMCHAT_HEIGHT];
	int				teamChatPos;
	int				teamLastChatPos;

	// media
	cgMedia_t		media;

	float			fov;		// clamped cg_fov value
	float			zoomFov;	// clamped cg_zoomFov value

	qbool		    pmove_fixed;
	int				pmove_msec;

	qbool		    synchronousClients;

	//int			ospEnc;
	//qbool		    defrag;

	qbool		    filterKeyUpEvent;
	qbool		    score_catched;
	int				score_key;

	float			cursorX;
	float			cursorY;

    gametype_t		gametype;
	ruleset_t       ruleset;
	defragmode_t    dfmode;
	fastcapmode_t   fcmode;
	qbool           obs;
	qbool           interference;
	qbool           reversemap;
} cgs_t;

//==============================================================================

extern cgs_t		cgs;
extern cg_t			cg;
extern centity_t	cg_entities[MAX_GENTITIES];
extern weaponInfo_t cg_weapons[MAX_WEAPONS];
extern itemInfo_t	cg_items[MAX_ITEMS];
extern markPoly_t	cg_markPolys[MAX_MARK_POLYS];
extern int			pm_interference;                //needed for client side prediction of hitscan weapons
extern const char	*eventnames[EV_MAX];

extern vmCvar_t		cg_centertime;
extern vmCvar_t		cg_runpitch;
extern vmCvar_t		cg_runroll;
extern vmCvar_t		cg_bobup;
extern vmCvar_t		cg_bobpitch;
extern vmCvar_t		cg_bobroll;
extern vmCvar_t		cg_swingSpeed;
extern vmCvar_t		cg_shadows;
extern vmCvar_t		cg_gibs;
extern vmCvar_t		cg_drawCrosshair;
extern vmCvar_t		cg_drawCrosshairNames;
extern vmCvar_t		cg_drawWeaponSelect;
extern vmCvar_t		cg_crosshairSize;
extern vmCvar_t		cg_crosshairHealth;
extern vmCvar_t     cg_crosshairColor;
extern vmCvar_t		cg_draw2D;
extern vmCvar_t		cg_railTrailTime;
extern vmCvar_t		cg_railTrailRadius;
extern vmCvar_t		cg_errorDecay;
extern vmCvar_t		cg_nopredict;
extern vmCvar_t		cg_showmiss;
extern vmCvar_t		cg_footsteps;
extern vmCvar_t		cg_addMarks;
extern vmCvar_t		cg_brassTime;
extern vmCvar_t		cg_gun_frame;
extern vmCvar_t		cg_gun_x;
extern vmCvar_t		cg_gun_y;
extern vmCvar_t		cg_gun_z;
extern vmCvar_t		cg_drawGun;
extern vmCvar_t		cg_tracerChance;
extern vmCvar_t		cg_tracerWidth;
extern vmCvar_t		cg_tracerLength;
extern vmCvar_t		cg_autoswitch;
extern vmCvar_t		cg_simpleItems;
extern vmCvar_t		cg_fov;
extern vmCvar_t		cg_zoomFov;
extern vmCvar_t		cg_thirdPersonRange;
extern vmCvar_t		cg_thirdPersonAngle;
extern vmCvar_t		cg_thirdPerson;
extern vmCvar_t		cg_lagometer;
extern vmCvar_t		rate;
extern vmCvar_t		snaps;
#ifdef DEVBUILD
extern vmCvar_t		cg_stats;
extern vmCvar_t		cg_drawSnapshot;
extern vmCvar_t     cg_psf;
extern vmCvar_t		cg_debugAnim;
extern vmCvar_t		cg_debugEvents;
extern vmCvar_t     cg_debugAutorecord;
//extern vmCvar_t		cg_ignore;
#endif // DEVBUILD
extern vmCvar_t 	cg_forceModel;
extern vmCvar_t 	cg_buildScript;
extern vmCvar_t		cg_paused;
extern vmCvar_t		cg_blood;
extern vmCvar_t		cg_predictItems;
extern vmCvar_t		cg_deferPlayers;
extern vmCvar_t		cg_smoothClients;
extern vmCvar_t		cg_timescaleFadeEnd;
extern vmCvar_t		cg_timescaleFadeSpeed;
extern vmCvar_t		cg_timescale;
extern vmCvar_t		cg_smallFont;
extern vmCvar_t		cg_bigFont;
extern vmCvar_t		cg_noTaunt;
extern vmCvar_t		cg_oldRail;
extern vmCvar_t		cg_oldRocket;
extern vmCvar_t		cg_oldPlasma;
extern vmCvar_t		cg_trueLightning;
extern vmCvar_t		cg_enemyModel;
extern vmCvar_t		cg_enemyColors;
extern vmCvar_t		cg_teamModel;
extern vmCvar_t		cg_teamColors;
extern vmCvar_t		cg_fovAdjust;
extern vmCvar_t     cg_noChatBeep;
extern vmCvar_t     cg_showSpawns;
extern vmCvar_t     cg_autorecord;
extern vmCvar_t     cg_drawPickup;
extern vmCvar_t     cg_teleportEffect;
extern vmCvar_t     cg_gibEffect;
extern vmCvar_t     cg_landingSound;
extern vmCvar_t     cg_painSounds;
extern vmCvar_t     cg_alwaysDrawItems;

extern vmCvar_t     hud_fontSize;
extern vmCvar_t     hud_fontColor;
extern vmCvar_t     hud_frameColor;

extern vmCvar_t     hud_useHoldable;
extern vmCvar_t     hud_timer;
extern vmCvar_t     hud_timerX;
extern vmCvar_t     hud_timerY;
extern vmCvar_t     hud_cgaz;
extern vmCvar_t     hud_cgazAlpha;
extern vmCvar_t     hud_cgazY;
extern vmCvar_t     hud_bbox;
extern vmCvar_t     hud_bboxFPS;
extern vmCvar_t     hud_bboxColor;
extern vmCvar_t     hud_bboxMinsZ;
extern vmCvar_t     hud_snap;
extern vmCvar_t     hud_snapY;
extern vmCvar_t     hud_snapRGBA;

extern vmCvar_t     chs_fontSize;
extern vmCvar_t     chs_fontColor;
extern vmCvar_t     chs_fontHighlightColor;
extern vmCvar_t     chs_keypress;
extern vmCvar_t     chs_ob;
extern vmCvar_t     chs_obX;
extern vmCvar_t     chs_obY;
extern vmCvar_t     chs_speed;
extern vmCvar_t     chs_speedX;
extern vmCvar_t     chs_speedY;
extern vmCvar_t     chs_angles;
extern vmCvar_t     chs_anglesX;
extern vmCvar_t     chs_anglesY;
extern vmCvar_t		chs_levelTimer;
extern vmCvar_t		chs_levelTimerX;
extern vmCvar_t		chs_levelTimerY;
extern vmCvar_t		chs_fps;
extern vmCvar_t		chs_fpsX;
extern vmCvar_t		chs_fpsY;
extern vmCvar_t     chs_ping;
extern vmCvar_t     chs_pingX;
extern vmCvar_t     chs_pingY;

extern vmCvar_t		cp_centerDraw;


//
// cg_main.c
//
const char *CG_ConfigString( int index );
const char *CG_Argv( int arg );
int         CG_VariableIntegerValue( const char *cvar );

void QDECL CG_Printf( const char *msg, ... );
void QDECL CG_Error( const char *msg, ... );

void CG_StartMusic( void );
void CG_UpdateCvars( void );
int  CG_CrosshairPlayer( void );
int  CG_LastAttacker( void );
void CG_LoadMenus(const char *menuFile);
void CG_KeyEvent( int key, qbool down );
void CG_MouseEvent( int x, int y );
void CG_RankRunFrame( void );
void CG_SetScoreSelection(void *menu);
void CG_BuildSpectatorString( void );
void CG_SetScoreCatcher( qbool enable );

//
// cg_view.c
//
void CG_TestModel_f (void);
void CG_TestGun_f (void);
void CG_TestModelNextFrame_f (void);
void CG_TestModelPrevFrame_f (void);
void CG_TestModelNextSkin_f (void);
void CG_TestModelPrevSkin_f (void);
void CG_ZoomDown_f( void );
void CG_ZoomUp_f( void );
void CG_AddBufferedSound( sfxHandle_t sfx);
void CG_DrawActiveFrame( int serverTime, stereoFrame_t stereoView, qbool demoPlayback );

//
// cg_drawtools.c
//
void CG_AdjustFrom640( float *x, float *y, float *w, float *h );
void CG_FillRect( float x, float y, float width, float height, const float *color );
void CG_FillScreen( const float *color );
void CG_DrawPic( float x, float y, float width, float height, qhandle_t hShader );
void CG_DrawStringExt( int x, int y, const char *string, const float *setColor, qbool forceColor, qbool shadow, int charWidth, int charHeight, int maxChars );
int CG_DrawStrlen( const char *str );
float *CG_FadeColor( int startMsec, int totalMsec );
float *CG_FadeColorTime( int startMsec, int totalMsec, int fadeMsec );
const float *CG_TeamColor( team_t team );
void CG_ColorForHealth( vec4_t hcolor );
void CG_GetColorForHealth( int health, int armor, vec4_t hcolor );
void UI_DrawProportionalString( int x, int y, const char* str, int style, vec4_t color );
void CG_DrawRect( float x, float y, float width, float height, float size, const float *color );
void CG_DrawSides(float x, float y, float w, float h, float size);
void CG_DrawTopBottom(float x, float y, float w, float h, float size);

// flags for CG_DrawString
enum {
	DS_SHADOW       = 0x1,
	DS_FORCE_COLOR  = 0x2,
	DS_PROPORTIONAL = 0x4,
	DS_CENTER       = 0x8,	// alignment
	DS_RIGHT        = 0x10	// alignment
};
void CG_DrawString( float x, float y, const char *s, const vec4_t color, float charWidth, float charHeight, int maxChars, int flags );
void CG_LoadFonts( void );
void CG_SelectFont( int index );


//
// cg_draw.c
extern  char systemChat[256];

void CG_AddLagometerFrameInfo( void );
void CG_AddLagometerSnapshotInfo( snapshot_t *snap );
void CG_CenterPrint( const char *str, int y, int charWidth );
void CG_DrawActive( stereoFrame_t stereoView );
void CG_ForceModelChange( void );
void CG_TrackClientTeamChange( void );

//
// cg_player.c
//
sfxHandle_t	CG_CustomSound( int clientNum, const char *soundName );
void        CG_Player( centity_t *cent );
void        CG_ResetPlayerEntity( centity_t *cent );
void        CG_AddRefEntityWithPowerups( refEntity_t *ent, entityState_t *state, int team );
void        CG_NewClientInfo( int clientNum );
void        CG_Spawnpoint( centity_t *cent );

//
// cg_predict.c
//
void CG_BuildSolidList( void );
int	 CG_PointContents( const vec3_t point, int passEntityNum );
void CG_Trace( trace_t *result, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int skipNumber, int mask );
void CG_PredictPlayerState( void );
void CG_LoadDeferredPlayers( void );
void CG_PlayDroppedEvents( playerState_t *ps, playerState_t *ops );

//
// cg_events.c
//
void        CG_CheckEvents( centity_t *cent );
const char *CG_PlaceString( int rank );
void        CG_EntityEvent( centity_t *cent, vec3_t position, int entityNum );
void        CG_PainEvent( centity_t *cent, int health );


//
// cg_ents.c
//
void CG_SetEntitySoundPosition( const centity_t *cent );
void CG_AddPacketEntities( void );
void CG_Beam( const centity_t *cent );
void CG_AdjustPositionForMover( const vec3_t in, int moverNum, int fromTime, int toTime, vec3_t out, const vec3_t angles_in, vec3_t angles_out );
void CG_PositionEntityOnTag( refEntity_t *entity, const refEntity_t *parent, qhandle_t parentModel, const char *tagName );
void CG_PositionRotatedEntityOnTag( refEntity_t *entity, const refEntity_t *parent, qhandle_t parentModel, const char *tagName );


//
// cg_weapons.c
//
void CG_NextWeapon_f( void );
void CG_PrevWeapon_f( void );
void CG_Weapon_f( void );
void CG_RegisterWeapon( int weaponNum );
void CG_RegisterItemVisuals( int itemNum );
void CG_FireWeapon( centity_t *cent );
void CG_MissileHitWall( weapon_t weapon, int clientNum, vec3_t origin, vec3_t dir, impactSound_t soundType );
void CG_MissileHitPlayer( int weapon, vec3_t origin, vec3_t dir, int entityNum );
void CG_ShotgunFire( entityState_t *es );
void CG_Bullet( vec3_t origin, int sourceEntityNum, vec3_t normal, qbool flesh, int fleshEntityNum );
void CG_RailTrail( const clientInfo_t *ci, const vec3_t start, const vec3_t end );
void CG_GrappleTrail( centity_t *ent, const weaponInfo_t *wi );
void CG_AddViewWeapon (playerState_t *ps);
void CG_AddPlayerWeapon( refEntity_t *parent, playerState_t *ps, centity_t *cent, int team );
void CG_DrawWeaponSelect( void );
void CG_OutOfAmmoChange( void );	// should this be in pmove?

//
// cg_marks.c
//
void	CG_InitMarkPolys( void );
void	CG_AddMarks( void );
void	CG_ImpactMark( qhandle_t markShader, const vec3_t origin, const vec3_t dir,	float orientation, float r, float g, float b, float a, qbool alphaFade, float radius, qbool temporary );

//
// cg_localents.c
//
void	        CG_InitLocalEntities( void );
localEntity_t	*CG_AllocLocalEntity( void );
void	        CG_AddLocalEntities( void );

//
// cg_effects.c
//
localEntity_t *CG_SmokePuff( const vec3_t p, const vec3_t vel, float radius, float r, float g, float b, float a, float duration, int startTime, int fadeInTime, int leFlags, qhandle_t hShader );
void CG_BubbleTrail( const vec3_t start, const vec3_t end, float spacing );
void CG_SpawnEffect( const vec3_t origin );
void CG_GibEntity( const vec3_t playerOrigin );
void CG_GibPlayer( const vec3_t playerOrigin );
//void CG_BigExplode( vec3_t playerOrigin );
void CG_Bleed( const vec3_t origin, int entityNum );
localEntity_t *CG_MakeExplosion( const vec3_t origin, const vec3_t dir,	qhandle_t hModel, qhandle_t shader, int msec, qbool isSprite );


// cg_snapshot.c
//
void CG_ProcessSnapshots( void );


// cg_info.c
//
void CG_LoadingString( const char *s );
void CG_LoadingItem( int itemNum );
void CG_LoadingClient( int clientNum );
void CG_DrawInformation( void );

//
// cg_scoreboard.c
//
qbool CG_DrawScoreboard( void );
void  CG_ScoreboardClick( void );

//
// cg_consolecmds.c
//
qbool CG_ConsoleCommand( void );
void  CG_InitConsoleCommands( void );

//
// cg_servercmds.c
//
void CG_ExecuteNewServerCommands( int latestSequence );
void CG_ParseServerinfo( void );
void CG_ParseSysteminfo( void );
void CG_SetConfigValues( void );
void CG_ShaderStateChanged(void);

//
// cg_playerstate.c
//
void CG_ResetCheckpoints( void );
void CG_Respawn( void );
void CG_TransitionPlayerState( playerState_t *ps, playerState_t *ops );
void CG_CheckChangedPredictableEvents( playerState_t *ps );


//
// cg_defrag.c
//
void CG_StartAutorecord( void );
void CG_StopAutorecord( void );
void CG_CheckAutorecord( void );


//===============================================

//
// system traps
// These functions are how the cgame communicates with the main game system
//

// print message on the local console
void		trap_Print( const char *text );

// abort the game
void		trap_Error( const char *text );

// milliseconds should only be used for performance tuning, never
// for anything game related.  Get time from the CG_DrawActiveFrame parameter
int			trap_Milliseconds( void );

// console variable interaction
void		trap_Cvar_Register( vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags );
void		trap_Cvar_Update( vmCvar_t *vmCvar );
void		trap_Cvar_Set( const char *var_name, const char *value );
void		trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize );

// ServerCommand and ConsoleCommand parameter access
int			trap_Argc( void );
void		trap_Argv( int n, char *buffer, int bufferLength );
void		trap_Args( char *buffer, int bufferLength );

// filesystem access
// returns length of file
int			trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode );
void		trap_FS_Read( void *buffer, int len, fileHandle_t f );
void		trap_FS_Write( const void *buffer, int len, fileHandle_t f );
void		trap_FS_FCloseFile( fileHandle_t f );
int			trap_FS_Seek( fileHandle_t f, long offset, fsOrigin_t origin );

// add commands to the local console as if they were typed in
// for map changing, etc.  The command is not executed immediately,
// but will be executed in order the next time console commands
// are processed
void		trap_SendConsoleCommand( const char *text );

int			trap_RealTime(qtime_t *qtime);

// register a command name so the console can perform command completion.
// FIXME: replace this with a normal console command "defineCommand"?
void		trap_AddCommand( const char *cmdName );
void		trap_RemoveCommand( const char *cmdName );

// send a string to the server over the network
void		trap_SendClientCommand( const char *s );

// force a screen update, only used during gamestate load
void		trap_UpdateScreen( void );

// model collision
void		trap_CM_LoadMap( const char *mapname );
int			trap_CM_NumInlineModels( void );
clipHandle_t trap_CM_InlineModel( int index );		// 0 = world, 1+ = bmodels
clipHandle_t trap_CM_TempBoxModel( const vec3_t mins, const vec3_t maxs );
int			trap_CM_PointContents( const vec3_t p, clipHandle_t model );
int			trap_CM_TransformedPointContents( const vec3_t p, clipHandle_t model, const vec3_t origin, const vec3_t angles );
void		trap_CM_BoxTrace( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask );
void		trap_CM_CapsuleTrace( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask );
void		trap_CM_TransformedBoxTrace( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask, const vec3_t origin, const vec3_t angles );
void		trap_CM_TransformedCapsuleTrace( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask, const vec3_t origin, const vec3_t angles );

// Returns the projection of a polygon onto the solid brushes in the world
int			trap_CM_MarkFragments( int numPoints, const vec3_t *points,	const vec3_t projection, int maxPoints, vec3_t pointBuffer, int maxFragments, markFragment_t *fragmentBuffer );

// normal sounds will have their volume dynamically changed as their entity
// moves and the listener moves
void		trap_S_StartSound( const vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx );
void		trap_S_StopLoopingSound(int entnum);

// a local sound is always played full volume
void		trap_S_StartLocalSound( sfxHandle_t sfx, int channelNum );
void		trap_S_ClearLoopingSounds( qbool killall );
void		trap_S_AddLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx );
void		trap_S_AddRealLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx );
void		trap_S_UpdateEntityPosition( int entityNum, const vec3_t origin );

// respatialize recalculates the volumes of sound as they should be heard by the
// given entityNum and position
void		trap_S_Respatialize( int entityNum, const vec3_t origin, vec3_t axis[3], int inwater );
sfxHandle_t	trap_S_RegisterSound( const char *sample, qbool compressed );		// returns buzz if not found
void		trap_S_StartBackgroundTrack( const char *intro, const char *loop );	// empty name stops music
void	    trap_S_StopBackgroundTrack( void );


void		trap_R_LoadWorldMap( const char *mapname );

// all media should be registered during level startup to prevent
// hitches during gameplay
qhandle_t	trap_R_RegisterModel( const char *name );			// returns rgb axis if not found
qhandle_t	trap_R_RegisterSkin( const char *name );			// returns all white if not found
qhandle_t	trap_R_RegisterShader( const char *name );			// returns all white if not found
qhandle_t	trap_R_RegisterShaderNoMip( const char *name );			// returns all white if not found

// a scene is built up by calls to R_ClearScene and the various R_Add functions.
// Nothing is drawn until R_RenderScene is called.
void		trap_R_ClearScene( void );
void		trap_R_AddRefEntityToScene( const refEntity_t *re );

// polys are intended for simple wall marks, not really for doing
// significant construction
void		trap_R_AddPolyToScene( qhandle_t hShader , int numVerts, const polyVert_t *verts );
void		trap_R_AddPolysToScene( qhandle_t hShader , int numVerts, const polyVert_t *verts, int numPolys );
void		trap_R_AddLightToScene( const vec3_t org, float intensity, float r, float g, float b );
void		trap_R_AddAdditiveLightToScene( const vec3_t org, float intensity, float r, float g, float b );
int			trap_R_LightForPoint( vec3_t point, vec3_t ambientLight, vec3_t directedLight, vec3_t lightDir );
void		trap_R_RenderScene( const refdef_t *fd );
void		trap_R_SetColor( const float *rgba );	// NULL = 1,1,1,1
void		trap_R_DrawStretchPic( float x, float y, float w, float h,
			float s1, float t1, float s2, float t2, qhandle_t hShader );
void		trap_R_ModelBounds( clipHandle_t model, vec3_t mins, vec3_t maxs );
int			trap_R_LerpTag( orientation_t *tag, clipHandle_t mod, int startFrame, int endFrame, float frac, const char *tagName );
void		trap_R_RemapShader( const char *oldShader, const char *newShader, const char *timeOffset );
qbool	    trap_R_inPVS( const vec3_t p1, const vec3_t p2 );

// The glconfig_t will not change during the life of a cgame.
// If it needs to change, the entire cgame will be restarted, because
// all the qhandle_t are then invalid.
void		trap_GetGlconfig( glconfig_t *glconfig );

// the gamestate should be grabbed at startup, and whenever a
// configstring changes
void		trap_GetGameState( gameState_t *gamestate );

// cgame will poll each frame to see if a newer snapshot has arrived
// that it is interested in.  The time is returned seperately so that
// snapshot latency can be calculated.
void		trap_GetCurrentSnapshotNumber( int *snapshotNumber, int *serverTime );

// a snapshot get can fail if the snapshot (or the entties it holds) is so
// old that it has fallen out of the client system queue
qbool	    trap_GetSnapshot( int snapshotNumber, snapshot_t *snapshot );

// retrieve a text command from the server stream
// the current snapshot will hold the number of the most recent command
// qfalse can be returned if the client system handled the command
// argc() / argv() can be used to examine the parameters of the command
qbool	    trap_GetServerCommand( int serverCommandNumber );

// returns the most recent command number that can be passed to GetUserCmd
// this will always be at least one higher than the number in the current
// snapshot, and it may be quite a few higher if it is a fast computer on
// a lagged connection
int			trap_GetCurrentCmdNumber( void );

qbool	    trap_GetUserCmd( int cmdNumber, usercmd_t *ucmd );

// used for the weapon select and zoom
void		trap_SetUserCmdValue( int stateValue, float sensitivityScale );

// aids for VM testing
void		testPrintInt( char *string, int i );
void		testPrintFloat( char *string, float f );
int			trap_MemoryRemaining( void );
void		trap_R_RegisterFont(const char *fontName, int pointSize, fontInfo_t *font);
qbool	    trap_Key_IsDown( int keynum );
int			trap_Key_GetCatcher( void );
void		trap_Key_SetCatcher( int catcher );
int			trap_Key_GetKey( const char *binding );


typedef enum {
  SYSTEM_PRINT,
  CHAT_PRINT,
  TEAMCHAT_PRINT
} q3print_t;

e_status    trap_CIN_StopCinematic(int handle);
e_status    trap_CIN_RunCinematic (int handle);
int         trap_CIN_PlayCinematic( const char *arg0, int xpos, int ypos, int width, int height, int bits);
void        trap_CIN_DrawCinematic (int handle);
void        trap_CIN_SetExtents (int handle, int x, int y, int w, int h);
void		trap_SnapVector( float *v );
qbool	    trap_GetEntityToken( char *buffer, int bufferSize );

void	        CG_ClearParticles (void);
void	        CG_AddParticles (void);
void	        CG_ParticleSnow (qhandle_t pshader, vec3_t origin, vec3_t origin2, int turb, float range, int snum);
void	        CG_ParticleSmoke (qhandle_t pshader, centity_t *cent);
void	        CG_AddParticleShrapnel (localEntity_t *le);
void	        CG_ParticleSnowFlurry (qhandle_t pshader, centity_t *cent);
void	        CG_ParticleBulletDebris (vec3_t	org, vec3_t vel, int duration);
void	        CG_ParticleSparks (vec3_t org, vec3_t vel, int duration, float x, float y, float speed);
void	        CG_ParticleDust (centity_t *cent, vec3_t origin, vec3_t dir);
void	        CG_ParticleMisc (qhandle_t pshader, vec3_t origin, int size, int duration, float alpha);
void	        CG_ParticleExplosion( const char *animStr, const vec3_t origin, const vec3_t vel, int duration, int sizeStart, int sizeEnd );
extern qbool	initparticles;
int             CG_NewParticleArea ( int num );

// extension interface
extern qbool intShaderTime;
extern qbool linearLight;
extern void  (*trap_R_AddRefEntityToScene2)( const refEntity_t *re );
extern void	 (*trap_R_AddLinearLightToScene)( const vec3_t start, const vec3_t end, float intensity, float r, float g, float b );
