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

#ifndef CG_LOCAL_H
#define CG_LOCAL_H

#include "q_shared.h"
#include "client_render.h"
#include "bg_public.h"
#include "cg_public.h"
#include "qcommon.h"

// todo: put in own file
typedef struct fxhandle_s {
	int count;
	sfxHandle_t ids[16];
} FxHandle;
CCALL void FxHandleInit(FxHandle *handle);
CCALL void FxHandleAddSound(FxHandle *handle, char *filename);
void FxHandleAddShader(FxHandle *handle, char *filename);
CCALL sfxHandle_t FxHandleGetID(FxHandle *handle);


// The entire cgame module is unloaded and reloaded on each level change,
// so there is NO persistant data between levels on the client side.
// If you absolutely need something stored, it can either be kept
// by the server in the server stored userinfos, or stashed in a cvar.

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
#define	ATTACKER_HEAD_TIME	10000
#define	REWARD_TIME			3000

#define	PULSE_SCALE			1.5			// amount to scale up the icons when activating

#define	MAX_STEP_CHANGE		32

#define	MAX_VERTS_ON_POLY	10
#define	MAX_MARK_POLYS		256

#define STAT_MINUS			10	// num frame for '-' stats digit

#define	ICON_SIZE			48
#define	CHAR_WIDTH			32
#define	CHAR_HEIGHT			48
#define	TEXT_ICON_SPACE		4

#define	TEAMCHAT_WIDTH		80
#define TEAMCHAT_HEIGHT		8

// very large characters
#define	GIANT_WIDTH			32
#define	GIANT_HEIGHT		48

#define	NUM_CROSSHAIRS		10

#define TEAM_OVERLAY_MAXNAME_WIDTH	12
#define TEAM_OVERLAY_MAXLOCATION_WIDTH	16

#define	DEFAULT_MODEL			"sarge"
#ifdef MISSIONPACK
#define	DEFAULT_TEAM_MODEL		"james"
#define	DEFAULT_TEAM_HEAD		"*james"
#else
#define	DEFAULT_TEAM_MODEL		"sarge"
#define	DEFAULT_TEAM_HEAD		"sarge"
#endif

#define DEFAULT_REDTEAM_NAME		"Stroggs"
#define DEFAULT_BLUETEAM_NAME		"Pagans"

typedef enum {
	FOOTSTEP_NORMAL,
	FOOTSTEP_BOOT,
	FOOTSTEP_FLESH,
	FOOTSTEP_MECH,
	FOOTSTEP_ENERGY,
	FOOTSTEP_METAL,
	FOOTSTEP_SPLASH,

	FOOTSTEP_TOTAL
} footstep_t;

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
	qboolean	yawing;
	float		pitchAngle;
	qboolean	pitching;

	int			animationNumber;	// may include ANIM_TOGGLEBIT
	animation_t	*animation;
	int			animationTime;		// time when the first frame of the animation will be exact
} lerpFrame_t;


typedef struct {
	lerpFrame_t		legs, torso, flag;
	int				painTime;
	int				painDirection;	// flip from 0 to 1
	int				lightningFiring;

	int				railFireTime;

	// machinegun spinning
	float			barrelAngle;
	int				barrelTime;
	qboolean		barrelSpinning;
} playerEntity_t;

//=================================================



// centity_t have a direct corespondence with gentity_t in the game, but
// only the entityState_t is directly communicated to the cgame
typedef struct centity_s {
	entityState_t	currentState;	// from cg.frame
	entityState_t	nextState;		// from cg.nextFrame, if available
	qboolean		interpolate;	// true if next is valid to interpolate to
	qboolean		currentValid;	// true if cg.frame holds this entity

	int				muzzleFlashTime;	// move to playerEntity?
	int				previousEvent;
	int				teleportFlag;

	int				trailTime;		// so missile trails can handle dropped initial packets
	int				dustTrailTime;
	int				miscTime;

	int				snapShotTime;	// last time this entity was found in a snapshot

	playerEntity_t	pe;

	int				errorTime;		// decay the error from this time
	vec3_t			errorOrigin;
	vec3_t			errorAngles;
	
	qboolean		extrapolated;	// false if origin / angles is an interpolation
	vec3_t			rawOrigin;
	vec3_t			rawAngles;

	vec3_t			beamEnd;

	// exact interpolated position of entity on this frame
	vec3_t			lerpOrigin;
	vec3_t			lerpAngles;
} centity_t;

typedef enum {
	LE_MARK,
	LE_EXPLOSION,
	LE_SPRITE_EXPLOSION,
	LE_FRAGMENT,
	LE_MOVE_SCALE_FADE,
	LE_FALL_SCALE_FADE,
	LE_FADE_RGB,
	LE_SCALE_FADE,
	LE_SCOREPLUM,
#ifdef MISSIONPACK
	LE_KAMIKAZE,
	LE_INVULIMPACT,
	LE_INVULJUICED,
	LE_SHOWREFENTITY
#endif
} leType_t;

typedef enum {
	LEF_PUFF_DONT_SCALE  = 0x0001,			// do not scale size over time
	LEF_TUMBLE			 = 0x0002,			// tumble over time, used for ejecting shells
	LEF_SOUND1			 = 0x0004,			// sound 1 for kamikaze
	LEF_SOUND2			 = 0x0008			// sound 2 for kamikaze
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
	int				guantletCount;
	int				defendCount;
	int				assistCount;
	int				captures;
	qboolean	perfect;
	int				team;
} score_t;

// each client has an associated clientInfo_t
// that contains media references necessary to present the
// client model and other color coded effects
// this is regenerated each time a client's configstring changes,
// usually as a result of a userinfo (name, model, etc) change
#define	MAX_CUSTOM_SOUNDS	32

typedef struct {
	qboolean		infoValid;

	char			name[MAX_QPATH];
	team_t			team;

	int				botSkill;		// 0 = not bot, 1-5 = bot

	vec3_t			color1;
	vec3_t			color2;
	
	byte c1RGBA[4];
	byte c2RGBA[4];

	int				score;			// updated by score servercmds
	int				location;		// location index for team mode
	int				health;			// you only get this info about your teammates
	int				armor;
	int				curWeapon;

	int				handicap;
	int				wins, losses;	// in tourney mode

	int				teamTask;		// task in teamplay (offence/defence)
	qboolean		teamLeader;		// true when this is a team leader

	int				powerups;		// so can display quad/flag status

	int				medkitUsageTime;
	int				invulnerabilityStartTime;
	int				invulnerabilityStopTime;

	int				breathPuffTime;

	// when clientinfo is changed, the loading of models/skins/sounds
	// can be deferred until you are dead, to prevent hitches in
	// gameplay
	char			modelName[MAX_QPATH];
	char			skinName[MAX_QPATH];
	char			headModelName[MAX_QPATH];
	char			headSkinName[MAX_QPATH];
	char			redTeam[MAX_TEAMNAME];
	char			blueTeam[MAX_TEAMNAME];
	qboolean		deferred;

	qboolean		newAnims;		// true if using the new mission pack animations
	qboolean		fixedlegs;		// true if legs yaw is always the same as torso yaw
	qboolean		fixedtorso;		// true if torso never changes yaw

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
} clientInfo_t;


// each WP_* weapon enum has an associated weaponInfo_t
// that contains media references necessary to present the
// weapon and its effects
typedef struct weaponInfo_s {
	qboolean		registered;
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


	int id;
	FxHandle shotsounds;
	void (*precache)  (struct weaponInfo_s *weapon);
	void (*shot)      (struct weaponInfo_s *weapon);
	void (*reload)    (struct weaponInfo_s *weapon);
	void (*activate)  (struct weaponInfo_s *weapon, centity_t *cent);
	void (*deactivate)(struct weaponInfo_s *weapon);

	float			trailRadius;
	float			wiTrailTime;

	sfxHandle_t		readySound;
	sfxHandle_t		firingSound;
} weaponInfo_t;


// each IT_* item has an associated itemInfo_t
// that constains media references necessary to present the
// item and its effects
typedef struct {
	qboolean		registered;
	qhandle_t		models[MAX_ITEM_MODELS];
	qhandle_t		icon;
} itemInfo_t;


typedef struct {
	int				itemNum;
} powerupInfo_t;


#define MAX_SKULLTRAIL		10

typedef struct {
	vec3_t positions[MAX_SKULLTRAIL];
	int numpositions;
} skulltrail_t;


#define MAX_REWARDSTACK		10
#define MAX_SOUNDBUFFER		20

//======================================================================

// all cg.stepTime, cg.duckTime, cg.landTime, etc are set to cg.time when the action
// occurs, and they will have visible effects for #define STEP_TIME or whatever msec after

#define MAX_PREDICTED_EVENTS	16
 
typedef struct {
	int			clientFrame;		// incremented each frame

	int			clientNum;
	
	qboolean	demoPlayback;
	qboolean	levelShot;			// taking a level menu screenshot
	int			deferredPlayerLoading;
	qboolean	loading;			// don't defer players at initial startup
	qboolean	intermissionStarted;	// don't play voice rewards, because game will end shortly

	// there are only one or two snapshot_t that are relevent at a time
	int			latestSnapshotNum;	// the number of snapshots the client system has received
	int			latestSnapshotTime;	// the time from latestSnapshotNum, so we don't need to read the snapshot yet

	snapshot_t	*snap;				// cg.snap->serverTime <= cg.time
	snapshot_t	*nextSnap;			// cg.nextSnap->serverTime > cg.time, or NULL
	snapshot_t	activeSnapshots[2];

	float		frameInterpolation;	// (float)( cg.time - cg.frame->serverTime ) / (cg.nextFrame->serverTime - cg.frame->serverTime)

	qboolean	thisFrameTeleport;
	qboolean	nextFrameTeleport;

	int			frametime;		// cg.time - cg.oldTime

	int			time;			// this is the time value that the client
								// is rendering at.
	int			oldTime;		// time at last frame, used for missile trails and prediction checking

	int			physicsTime;	// either cg.snap->time or cg.nextSnap->time

	int			timelimitWarnings;	// 5 min, 1 min, overtime
	int			fraglimitWarnings;

	qboolean	mapRestart;			// set on a map restart to set back the weapon

	qboolean	renderingThirdPerson;		// during deaths, chasecams, etc

	// prediction state
	qboolean	hyperspace;				// true if prediction has hit a trigger_teleport
	playerState_t	predictedPlayerState;
	centity_t		predictedPlayerEntity;
	qboolean	validPPS;				// clear until the first call to CG_PredictPlayerState
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

	qboolean	showToolgunMenu;

	// zoom key
	qboolean	zoomed;
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
	qboolean	showScores;
	qboolean	scoreBoardShowing;
	int			scoreFadeTime;
	char		killerName[MAX_NAME_LENGTH];
	char			spectatorList[MAX_STRING_CHARS];		// list of names
	int				spectatorLen;												// length of list
	float			spectatorWidth;											// width in device units
	int				spectatorTime;											// next time to offset
	int				spectatorPaintX;										// current paint x
	int				spectatorPaintX2;										// current paint x
	int				spectatorOffset;										// current offset from start
	int				spectatorPaintLen; 									// current offset from start

#ifdef MISSIONPACK
	// skull trails
	skulltrail_t	skulltrails[MAX_CLIENTS];
#endif

	// centerprinting
	int			centerPrintTime;
	int			centerPrintCharWidth;
	int			centerPrintY;
	char		centerPrint[1024];
	int			centerPrintLines;

	// low ammo warning state
	int			lowAmmoWarning;		// 1 = low, 2 = empty

	// crosshair client ID
	int			crosshairClientNum;
	int			crosshairClientTime;

	// powerup active flashing
	int			powerupActive;
	int			powerupTime;

	// attacking player
	int			attackerTime;
	int			voiceTime;

	// reward medals
	int			rewardStack;
	int			rewardTime;
	int			rewardCount[MAX_REWARDSTACK];
	qhandle_t	rewardShader[MAX_REWARDSTACK];
	qhandle_t	rewardSound[MAX_REWARDSTACK];

	// sound buffer mainly for announcer sounds
	int			soundBufferIn;
	int			soundBufferOut;
	int			soundTime;
	qhandle_t	soundBuffer[MAX_SOUNDBUFFER];

	// warmup countdown
	int			warmup;
	int			warmupCount;

	//==========================

	int			itemPickup;
	int			itemPickupTime;
	int			itemPickupBlendTime;	// the pulse around the crosshair is timed seperately

	int			weaponSelectTime;
	int			weaponAnimation;
	int			weaponAnimationTime;

	// blend blobs
	float		damageTime;
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

	// temp working variables for player view
	float		bobfracsin;
	int			bobcycle;
	float		xyspeed;
	int     nextOrbitTime;

	//qboolean cameraMode;		// if rendering from a loaded camera


	// development tool
	refEntity_t		testModelEntity;
	char			testModelName[MAX_QPATH];
	qboolean		testGun;

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


	qhandle_t	armorModel;
	qhandle_t	armorIcon;

	qhandle_t	teamStatusBar;

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

	qhandle_t	friendShader;

	qhandle_t	balloonShader;
	qhandle_t	connectionShader;

	qhandle_t	selectShader;
	qhandle_t	viewBloodShader;
	qhandle_t	tracerShader;
	qhandle_t	crosshairShader[NUM_CROSSHAIRS];
	qhandle_t	lagometerShader;
	qhandle_t	backTileShader;
	qhandle_t	noammoShader;

	qhandle_t	smokePuffShader;
	qhandle_t	smokePuffRageProShader;
	qhandle_t	shotgunSmokePuffShader;
	qhandle_t	plasmaBallShader;
	qhandle_t	waterBubbleShader;
	qhandle_t	bloodTrailShader;
#ifdef MISSIONPACK
	qhandle_t	nailPuffShader;
	qhandle_t	blueProxMine;
#endif

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

	// scoreboard headers
	qhandle_t	scoreboardName;
	qhandle_t	scoreboardPing;
	qhandle_t	scoreboardScore;
	qhandle_t	scoreboardTime;



	// sounds
	sfxHandle_t	quadSound;
	sfxHandle_t	tracerSound;
	sfxHandle_t	selectSound;
	sfxHandle_t	useNothingSound;
	sfxHandle_t	wearOffSound;
	sfxHandle_t	sfx_lghit1;
	sfxHandle_t	sfx_lghit2;
	sfxHandle_t	sfx_lghit3;
	sfxHandle_t	sfx_ric1;
	sfxHandle_t	sfx_ric2;
	sfxHandle_t	sfx_ric3;
	//sfxHandle_t	sfx_railg;
	sfxHandle_t	sfx_rockexp;
	sfxHandle_t	sfx_plasmaexp;

	sfxHandle_t	gibSound;
	sfxHandle_t	gibBounce1Sound;
	sfxHandle_t	gibBounce2Sound;
	sfxHandle_t	gibBounce3Sound;
	sfxHandle_t	teleInSound;
	sfxHandle_t	teleOutSound;
	sfxHandle_t	noAmmoSound;
	sfxHandle_t	respawnSound;
	sfxHandle_t talkSound;
	sfxHandle_t landSound;
	sfxHandle_t fallSound;
	sfxHandle_t jumpPadSound;

	sfxHandle_t hitSound;
	sfxHandle_t hitSoundHighArmor;
	sfxHandle_t hitSoundLowArmor;
	sfxHandle_t hitTeamSound;
	
	sfxHandle_t deniedSound;
	sfxHandle_t assistSound;
	sfxHandle_t defendSound;
	
	FxHandle footstep_default;
	FxHandle footstep_metal;
	FxHandle footstep_slime;
	FxHandle footstep_flesh;
	FxHandle footstep_snow;
	FxHandle footstep_ice;
	FxHandle footstep_water;
	FxHandle footstep_splash;
	FxHandle footstep_energy;
	FxHandle footstep_mech;
	FxHandle footstep_boot;
	FxHandle footwade;
	FxHandle swim;
	FxHandle waterIn;
	FxHandle waterSlowIn;
	FxHandle waterOut;
	FxHandle waterDrown;
	FxHandle waterAppear;
	FxHandle jump;
	FxHandle pain;
	FxHandle laugh;
	FxHandle sigh;
	FxHandle death;
	FxHandle whoosh;
	FxHandle bulletmark;
	FxHandle rocketexplosion;

	sfxHandle_t flightSound;
	sfxHandle_t medkitSound;

#ifdef MISSIONPACK
	sfxHandle_t weaponHoverSound;
#endif



	sfxHandle_t	regenSound;
	sfxHandle_t	protectSound;
	sfxHandle_t	n_healthSound;
	sfxHandle_t	hgrenb1aSound;
	sfxHandle_t	hgrenb2aSound;
	sfxHandle_t	wstbimplSound;
	sfxHandle_t	wstbimpmSound;
	sfxHandle_t	wstbimpdSound;
	sfxHandle_t	wstbactvSound;
	
	FxHandle ev_bullet_hit_flesh;
	FxHandle ev_bullet_hit_wall;
	FxHandle gun_rifle_shot;

} cgMedia_t;


// The client game static (cgs) structure hold everything
// loaded or calculated from the gamestate.  It will NOT
// be cleared when a tournement restart is done, allowing
// all clients to begin playing instantly
typedef struct {
	gameState_t		gameState;			// gamestate from server
	glconfig_t		glconfig;			// rendering configuration
	float			screenXScale;		// derived from glconfig
	float			screenYScale;
	float			screenXBias;

	int				serverCommandSequence;	// reliable command stream counter
	int				processedSnapshotNum;// the number of snapshots cgame has requested

	qboolean		localServer;		// detected on startup by checking sv_running

	// parsed from serverinfo
	gametype_t		gametype;
	int				dmflags;
	int				teamflags;
	int				fraglimit;
	int				capturelimit;
	int				timelimit;
	int				maxclients;
	char			mapname[MAX_QPATH];
	char			redTeam[MAX_QPATH];
	char			blueTeam[MAX_QPATH];

	int				voteTime;
	int				voteYes;
	int				voteNo;
	qboolean		voteModified;			// beep whenever changed
	char			voteString[MAX_STRING_TOKENS];

	int				teamVoteTime[2];
	int				teamVoteYes[2];
	int				teamVoteNo[2];
	qboolean		teamVoteModified[2];	// beep whenever changed
	char			teamVoteString[2][MAX_STRING_TOKENS];

	int				levelStartTime;

	int				scores1, scores2;		// from configstrings
	int				redflag, blueflag;		// flag status from configstrings
	int				flagStatus;

	qboolean  newHud;

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

	int cursorX;
	int cursorY;
	qboolean eventHandling;
	qboolean mouseCaptured;
	qboolean sizingHud;
	void *capturedItem;
	qhandle_t activeCursor;

	// orders
	int currentOrder;
	qboolean orderPending;
	int orderTime;
	int currentVoiceClient;
	int acceptOrderTime;
	int acceptTask;
	int acceptLeader;
	char acceptVoice[MAX_NAME_LENGTH];

	// media
	cgMedia_t		media;

} cgs_t;

//==============================================================================

EXTERNC cgs_t			cgs;
EXTERNC cg_t			cg;
EXTERNC centity_t		cg_entities[MAX_GENTITIES];
EXTERNC weaponInfo_t	cg_weapons[MAX_WEAPONS];
EXTERNC itemInfo_t		cg_items[MAX_ITEMS];

EXTERNC	vmCvar_t		cg_centertime;
EXTERNC	vmCvar_t		cg_runpitch;
EXTERNC	vmCvar_t		cg_runroll;
EXTERNC	vmCvar_t		cg_bobup;
EXTERNC	vmCvar_t		cg_bobpitch;
EXTERNC	vmCvar_t		cg_bobroll;
EXTERNC	vmCvar_t		cg_swingSpeed;
EXTERNC	vmCvar_t		cg_shadows;
EXTERNC	vmCvar_t		cg_gibs;
EXTERNC	vmCvar_t		cg_drawTimer;
EXTERNC	vmCvar_t		cg_drawFPS;
EXTERNC	vmCvar_t		cg_drawSnapshot;
EXTERNC	vmCvar_t		cg_draw3dIcons;
EXTERNC	vmCvar_t		cg_drawIcons;
EXTERNC	vmCvar_t		cg_drawAmmoWarning;
EXTERNC	vmCvar_t		cg_drawCrosshair;
EXTERNC	vmCvar_t		cg_drawCrosshairNames;
EXTERNC	vmCvar_t		cg_drawRewards;
EXTERNC	vmCvar_t		cg_drawTeamOverlay;
EXTERNC	vmCvar_t		cg_teamOverlayUserinfo;
EXTERNC	vmCvar_t		cg_crosshairX;
EXTERNC	vmCvar_t		cg_crosshairY;
EXTERNC	vmCvar_t		cg_crosshairSize;
EXTERNC	vmCvar_t		cg_crosshairHealth;
EXTERNC	vmCvar_t		cg_drawStatus;
EXTERNC	vmCvar_t		cg_draw2D;
EXTERNC	vmCvar_t		cg_animSpeed;
EXTERNC	vmCvar_t		cg_debugAnim;
EXTERNC	vmCvar_t		cg_debugPosition;
EXTERNC	vmCvar_t		cg_debugEvents;
EXTERNC	vmCvar_t		cg_railTrailTime;
EXTERNC	vmCvar_t		cg_errorDecay;
EXTERNC	vmCvar_t		cg_nopredict;
EXTERNC	vmCvar_t		cg_noPlayerAnims;
EXTERNC	vmCvar_t		cg_showmiss;
EXTERNC	vmCvar_t		cg_footsteps;
EXTERNC	vmCvar_t		cg_addMarks;
EXTERNC	vmCvar_t		cg_brassTime;
EXTERNC	vmCvar_t		cg_gun_frame;
EXTERNC	vmCvar_t		cg_gun_x;
EXTERNC	vmCvar_t		cg_gun_y;
EXTERNC	vmCvar_t		cg_gun_z;
EXTERNC	vmCvar_t		cg_drawGun;
EXTERNC	vmCvar_t		cg_viewsize;
EXTERNC	vmCvar_t		cg_tracerChance;
EXTERNC	vmCvar_t		cg_tracerWidth;
EXTERNC	vmCvar_t		cg_tracerLength;
EXTERNC	vmCvar_t		cg_autoswitch;
EXTERNC	vmCvar_t		cg_ignore;
EXTERNC	vmCvar_t		cg_simpleItems;
EXTERNC	vmCvar_t		cg_fov;
EXTERNC	vmCvar_t		cg_zoomFov;
EXTERNC	vmCvar_t		cg_thirdPersonRange;
EXTERNC	vmCvar_t		cg_thirdPersonAngle;
EXTERNC	vmCvar_t		cg_thirdPerson;
EXTERNC	vmCvar_t		cg_lagometer;
EXTERNC	vmCvar_t		cg_drawAttacker;
EXTERNC	vmCvar_t		cg_synchronousClients;
EXTERNC	vmCvar_t		cg_teamChatTime;
EXTERNC	vmCvar_t		cg_teamChatHeight;
EXTERNC	vmCvar_t		cg_stats;
EXTERNC	vmCvar_t 		cg_forceModel;
EXTERNC	vmCvar_t 		cg_buildScript;
EXTERNC	vmCvar_t		cg_paused;
EXTERNC	vmCvar_t		cg_blood;
EXTERNC	vmCvar_t		cg_predictItems;
EXTERNC	vmCvar_t		cg_deferPlayers;
EXTERNC	vmCvar_t		cg_drawFriend;
EXTERNC	vmCvar_t		cg_teamChatsOnly;
#ifdef MISSIONPACK
EXTERNC	vmCvar_t		cg_noVoiceChats;
EXTERNC	vmCvar_t		cg_noVoiceText;
#endif
EXTERNC  vmCvar_t		cg_scorePlum;
EXTERNC	vmCvar_t		cg_smoothClients;
EXTERNC	vmCvar_t		cg_pmove_fixed;
EXTERNC	vmCvar_t		cg_pmove_msec;
//extern	vmCvar_t		cg_pmove_fixed;
EXTERNC	vmCvar_t		cg_cameraOrbit;
EXTERNC	vmCvar_t		cg_cameraOrbitDelay;
EXTERNC	vmCvar_t		cg_timescaleFadeEnd;
EXTERNC	vmCvar_t		cg_timescaleFadeSpeed;
EXTERNC	vmCvar_t		cg_timescale;
EXTERNC	vmCvar_t		cg_cameraMode;
EXTERNC  vmCvar_t		cg_smallFont;
EXTERNC  vmCvar_t		cg_bigFont;
EXTERNC	vmCvar_t		cg_noTaunt;
EXTERNC	vmCvar_t		cg_noProjectileTrail;
EXTERNC	vmCvar_t		cg_oldRail;
EXTERNC	vmCvar_t		cg_oldRocket;
EXTERNC	vmCvar_t		cg_oldPlasma;
EXTERNC	vmCvar_t		cg_trueLightning;

//
// cg_main.c
//

CCALL void CG_Init( int serverMessageNum, int serverCommandSequence, int clientNum );
CCALL const char *CG_ConfigString( int index );
CCALL const char *CG_Argv( int arg );

CCALL void CG_Printf( const char *msg, ... ) __attribute__ ((format (printf, 1, 2)));
CCALL void CG_Error( const char *msg, ... ) __attribute__ ((noreturn, format (printf, 1, 2)));

CCALL void CG_StartMusic( void );
CCALL void CG_UpdateCvars( void );
CCALL int CG_CrosshairPlayer( void );
CCALL int CG_LastAttacker( void );
CCALL void CG_MouseEvent(int x, int y);
CCALL void CG_BuildSpectatorString( void );

//
// cg_view.c
//
CCALL void CG_TestModel_f (void);
CCALL void CG_TestGun_f (void);
CCALL void CG_TestModelNextFrame_f (void);
CCALL void CG_TestModelPrevFrame_f (void);
CCALL void CG_TestModelNextSkin_f (void);
CCALL void CG_TestModelPrevSkin_f (void);
CCALL void CG_ZoomDown_f( void );
CCALL void CG_ZoomUp_f( void );
CCALL void CG_AddBufferedSound( sfxHandle_t sfx);

//
// cg_drawtools.c
//
CCALL void CG_AdjustFrom640( float *x, float *y, float *w, float *h );
CCALL void CG_FillRect( float x, float y, float width, float height, const float *color );
CCALL void CG_DrawPic( float x, float y, float width, float height, qhandle_t hShader );
CCALL void CG_DrawStringExt( int x, int y, const char *string, const float *setColor, qboolean forceColor, qboolean shadow, int charWidth, int charHeight, int maxChars );
CCALL void CG_DrawBigString( int x, int y, const char *s, float alpha );
CCALL void CG_DrawBigStringColor( int x, int y, const char *s, vec4_t color );
CCALL void CG_DrawSmallString( int x, int y, const char *s, float alpha );
CCALL void CG_DrawSmallStringColor( int x, int y, const char *s, vec4_t color );
CCALL int CG_DrawStrlen( const char *str );
CCALL float	*CG_FadeColor( int startMsec, int totalMsec );
CCALL float *CG_TeamColor( int team );
CCALL void CG_TileClear( void );
CCALL void CG_ColorForHealth( vec4_t hcolor );
CCALL void CG_GetColorForHealth( int health, int armor, vec4_t hcolor );
CCALL void UI_DrawProportionalString( int x, int y, const char* str, int style, vec4_t color );
CCALL void CG_DrawRect( float x, float y, float width, float height, float size, const float *color );
CCALL void CG_DrawSides(float x, float y, float w, float h, float size);
CCALL void CG_DrawTopBottom(float x, float y, float w, float h, float size);


//
// cg_draw.c, cg_newDraw.c
//
extern	int sortedTeamPlayers[TEAM_MAXOVERLAY];
extern	int	numSortedTeamPlayers;
extern	int drawTeamOverlayModificationCount;
extern  char systemChat[256];
extern  char teamChat1[256];
extern  char teamChat2[256];

CCALL void CG_AddLagometerFrameInfo( void );
CCALL void CG_AddLagometerSnapshotInfo( snapshot_t *snap );
CCALL void CG_CenterPrint( const char *str, int y, int charWidth );
CCALL void CG_DrawHead( float x, float y, float w, float h, int clientNum, vec3_t headAngles );
CCALL void CG_DrawFlagModel( float x, float y, float w, float h, int team, qboolean force2D );
CCALL void CG_DrawTeamBackground( int x, int y, int w, int h, float alpha, int team );
CCALL void CG_Draw3DModel(float x, float y, float w, float h, qhandle_t model, qhandle_t skin, vec3_t origin, vec3_t angles);


//
// cg_player.c
//
CCALL void CG_Player( centity_t *cent );
CCALL void CG_ResetPlayerEntity( centity_t *cent );
CCALL void CG_AddRefEntityWithPowerups( refEntity_t *ent, entityState_t *state, int team );
CCALL void CG_NewClientInfo( int clientNum );
CCALL sfxHandle_t	CG_CustomSound( int clientNum, const char *soundName );

//
// cg_predict.c
//
CCALL void CG_BuildSolidList( void );
CCALL int	CG_PointContents( const vec3_t point, int passEntityNum );
CCALL void CG_Trace( trace_t *result, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int skipNumber, int mask );
CCALL void CG_PredictPlayerState( void );
CCALL void CG_LoadDeferredPlayers( void );


//
// cg_events.c
//
CCALL void CG_CheckEvents( centity_t *cent );
CCALL const char	*CG_PlaceString( int rank );
CCALL void CG_EntityEvent( centity_t *cent, vec3_t position );
CCALL void CG_PainEvent( centity_t *cent, int health );


//
// cg_ents.c
//
CCALL void CG_SetEntitySoundPosition( centity_t *cent );
CCALL void CG_AddPacketEntities( void );
CCALL void CG_Beam( centity_t *cent );
CCALL void CG_AdjustPositionForMover(const vec3_t in, int moverNum, int fromTime, int toTime, vec3_t out, vec3_t angles_in, vec3_t angles_out);
CCALL void CG_PositionEntityOnTag( refEntity_t *entity, const refEntity_t *parent,  qhandle_t parentModel, char *tagName );
CCALL void CG_PositionRotatedEntityOnTag( refEntity_t *entity, const refEntity_t *parent,  qhandle_t parentModel, char *tagName );



//
// cg_weapons.c
//
CCALL void CG_NextWeapon_f( void );
CCALL void CG_PrevWeapon_f( void );
CCALL void CG_Weapon_f( void );
CCALL void CG_RegisterWeapon( int weaponNum );
CCALL void CG_RegisterItemVisuals( int itemNum );
CCALL void CG_FireWeapon( centity_t *cent );
CCALL void CG_MissileHitWall( int weapon, int clientNum, vec3_t origin, vec3_t dir, impactSound_t soundType );
CCALL void CG_MissileHitPlayer( int weapon, vec3_t origin, vec3_t dir, int entityNum );
CCALL void CG_ShotgunFire( entityState_t *es );
CCALL void CG_Bullet( vec3_t origin, int sourceEntityNum, vec3_t normal, qboolean flesh, int fleshEntityNum );
CCALL void CG_RailTrail( clientInfo_t *ci, vec3_t start, vec3_t end );
CCALL void CG_GrappleTrail( centity_t *ent, const weaponInfo_t *wi );
CCALL void CG_AddViewWeapon (playerState_t *ps);
CCALL void CG_AddPlayerWeapon( refEntity_t *parent, playerState_t *ps, centity_t *cent, int team );
CCALL void CG_DrawWeaponSelect( void );
CCALL void CG_OutOfAmmoChange( void );	// should this be in pmove?

//
// cg_localents.c
//
CCALL void	CG_InitLocalEntities( void );
CCALL localEntity_t	*CG_AllocLocalEntity( void );
CCALL void	CG_AddLocalEntities( void );

//
// cg_effects.c
//
CCALL localEntity_t *CG_SmokePuff( const vec3_t p,  const vec3_t vel,  float radius, float r, float g, float b, float a, float duration, int startTime, int fadeInTime, int leFlags, qhandle_t hShader );
CCALL void CG_BubbleTrail( vec3_t start, vec3_t end, float spacing );
CCALL void CG_SpawnEffect( vec3_t org );
#ifdef MISSIONPACK
CCALL void CG_KamikazeEffect( vec3_t org );
CCALL void CG_ObeliskExplode( vec3_t org, int entityNum );
CCALL void CG_ObeliskPain( vec3_t org );
CCALL void CG_InvulnerabilityImpact( vec3_t org, vec3_t angles );
CCALL void CG_InvulnerabilityJuiced( vec3_t org );
CCALL void CG_LightningBoltBeam( vec3_t start, vec3_t end );
#endif
CCALL void CG_ScorePlum( int client, vec3_t org, int score );

CCALL void CG_GibPlayer( vec3_t playerOrigin );
CCALL void CG_BigExplode( vec3_t playerOrigin );

CCALL void CG_Bleed( vec3_t origin, int entityNum );

CCALL localEntity_t *CG_MakeExplosion( vec3_t origin, vec3_t dir, qhandle_t hModel, qhandle_t shader, int msec, qboolean isSprite );

//
// cg_snapshot.c
//
CCALL void CG_ProcessSnapshots( void );

//
// cg_info.c
//
CCALL void CG_LoadingString( const char *s );
CCALL void CG_LoadingItem( int itemNum );
CCALL void CG_LoadingClient( int clientNum );
CCALL void CG_DrawInformation( void );

//
// cg_scoreboard.c
//
CCALL qboolean CG_DrawOldScoreboard( void );
CCALL void CG_DrawOldTourneyScoreboard( void );

//
// cg_consolecmds.c
//
CCALL qboolean CG_ConsoleCommand( void );
CCALL void CG_InitConsoleCommands( void );

//
// cg_servercmds.c
//
CCALL void CG_ExecuteNewServerCommands( int latestSequence );
CCALL void CG_ParseServerinfo( void );
CCALL void CG_SetConfigValues( void );
CCALL void CG_ShaderStateChanged(void);
#ifdef MISSIONPACK
CCALL void CG_LoadVoiceChats( void );
CCALL void CG_VoiceChatLocal( int mode, qboolean voiceOnly, int clientNum, int color, const char *cmd );
CCALL void CG_PlayBufferedVoiceChats( void );
#endif

//
// cg_playerstate.c
//
CCALL void CG_Respawn( void );
CCALL void CG_TransitionPlayerState( playerState_t *ps, playerState_t *ops );
CCALL void CG_CheckChangedPredictableEvents( playerState_t *ps );


//===============================================

//
// system traps
// These functions are how the cgame communicates with the main game system
//

// print message on the local console
CCALL void		trap_Print( const char *fmt );

// abort the game
CCALL void		trap_Error(const char *fmt) __attribute__((noreturn));

// milliseconds should only be used for performance tuning, never
// for anything game related.  Get time from the CG_DrawActiveFrame parameter
CCALL int			trap_Milliseconds( void );

// ServerCommand and ConsoleCommand parameter access
CCALL int			trap_Argc( void );
CCALL void		trap_Argv( int n, char *buffer, int bufferLength );
CCALL void		trap_Args( char *buffer, int bufferLength );

// filesystem access
// returns length of file
CCALL int			trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode );

// add commands to the local console as if they were typed in
// for map changing, etc.  The command is not executed immediately,
// but will be executed in order the next time console commands
// are processed
CCALL void		trap_SendConsoleCommand( const char *text );

// register a command name so the console can perform command completion.
// FIXME: replace this with a normal console command "defineCommand"?
CCALL void		trap_AddCommand( const char *cmdName );
CCALL void		trap_RemoveCommand( const char *cmdName );

// send a string to the server over the network
CCALL void		trap_SendClientCommand( const char *s );

// force a screen update, only used during gamestate load
CCALL void		trap_UpdateScreen( void );

// model collision
CCALL void		trap_CM_LoadMap( const char *mapname );
CCALL int			trap_CM_NumInlineModels( void );
CCALL clipHandle_t trap_CM_InlineModel( int index );		// 0 = world, 1+ = bmodels
CCALL clipHandle_t trap_CM_TempBoxModel( const vec3_t mins, const vec3_t maxs );
CCALL int			trap_CM_PointContents( const vec3_t p, clipHandle_t model );
CCALL int			trap_CM_TransformedPointContents( const vec3_t p, clipHandle_t model, const vec3_t origin, const vec3_t angles );
CCALL void		trap_CM_BoxTrace( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask );
CCALL void		trap_CM_CapsuleTrace( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask );
CCALL void		trap_CM_TransformedBoxTrace( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask, const vec3_t origin, const vec3_t angles );
CCALL void		trap_CM_TransformedCapsuleTrace( trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask, const vec3_t origin, const vec3_t angles );

// Returns the projection of a polygon onto the solid brushes in the world
CCALL int			trap_CM_MarkFragments( int numPoints, const vec3_t *points, 
			const vec3_t projection,
			int maxPoints, vec3_t pointBuffer,
			int maxFragments, markFragment_t *fragmentBuffer );

// normal sounds will have their volume dynamically changed as their entity
// moves and the listener moves
CCALL void		trap_S_StartSound( vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx );
CCALL void		trap_S_StopLoopingSound(int entnum);

// a local sound is always played full volume
CCALL void		trap_S_StartLocalSound( sfxHandle_t sfx, int channelNum );
CCALL void		trap_S_ClearLoopingSounds( qboolean killall );
CCALL void		trap_S_AddLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx );
CCALL void		trap_S_AddRealLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx );
CCALL void		trap_S_UpdateEntityPosition( int entityNum, const vec3_t origin );

// respatialize recalculates the volumes of sound as they should be heard by the
// given entityNum and position
CCALL void		trap_S_Respatialize( int entityNum, const vec3_t origin, vec3_t axis[3], int inwater );
CCALL sfxHandle_t	trap_S_RegisterSound( const char *sample, qboolean compressed );		// returns buzz if not found
CCALL void		trap_S_StartBackgroundTrack( const char *intro, const char *loop );	// empty name stops music
CCALL void	trap_S_StopBackgroundTrack( void );


CCALL void		trap_R_LoadWorldMap( const char *mapname );

// all media should be registered during level startup to prevent
// hitches during gameplay
CCALL qhandle_t	trap_R_RegisterModel( const char *name );			// returns rgb axis if not found
CCALL qhandle_t	trap_R_RegisterSkin( const char *name );			// returns all white if not found
CCALL qhandle_t	trap_R_RegisterShader( const char *name );			// returns all white if not found
CCALL qhandle_t	trap_R_RegisterShaderNoMip( const char *name );			// returns all white if not found

// polys are intended for simple wall marks, not really for doing
// significant construction
CCALL void		trap_R_AddLightToScene( const vec3_t org, float intensity, float r, float g, float b );
CCALL void		trap_R_AddAdditiveLightToScene( const vec3_t org, float intensity, float r, float g, float b );
CCALL void		trap_R_SetColor( const float *rgba );	// NULL = 1,1,1,1
CCALL void		trap_R_DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader );
CCALL qboolean	trap_R_inPVS( const vec3_t p1, const vec3_t p2 );

// the gamestate should be grabbed at startup, and whenever a
// configstring changes
CCALL void		trap_GetGameState( gameState_t *gamestate );

// cgame will poll each frame to see if a newer snapshot has arrived
// that it is interested in.  The time is returned seperately so that
// snapshot latency can be calculated.
CCALL void		trap_GetCurrentSnapshotNumber( int *snapshotNumber, int *serverTime );

// a snapshot get can fail if the snapshot (or the entties it holds) is so
// old that it has fallen out of the client system queue
CCALL qboolean	trap_GetSnapshot( int snapshotNumber, snapshot_t *snapshot );

// retrieve a text command from the server stream
// the current snapshot will hold the number of the most recent command
// qfalse can be returned if the client system handled the command
// argc() / argv() can be used to examine the parameters of the command
CCALL qboolean	trap_GetServerCommand( int serverCommandNumber );

// returns the most recent command number that can be passed to GetUserCmd
// this will always be at least one higher than the number in the current
// snapshot, and it may be quite a few higher if it is a fast computer on
// a lagged connection
CCALL int			trap_GetCurrentCmdNumber( void );	

CCALL qboolean	trap_GetUserCmd( int cmdNumber, usercmd_t *ucmd );

// used for the weapon select and zoom
CCALL void		trap_SetUserCmdValue( int stateValue, float sensitivityScale );
CCALL int			trap_MemoryRemaining( void );
CCALL qboolean	trap_Key_IsDown( int keynum );
CCALL int			trap_Key_GetCatcher( void );
CCALL void		trap_Key_SetCatcher( int catcher );
CCALL int			trap_Key_GetKey( const char *binding );


typedef enum {
  SYSTEM_PRINT,
  CHAT_PRINT,
  TEAMCHAT_PRINT
} q3print_t;

CCALL int			trap_RealTime(qtime_t *qtime);
CCALL void		trap_SnapVector( float *v );

CCALL qboolean	trap_GetEntityToken( char *buffer, int bufferSize );
CCALL void	CG_ClearParticles (void);
CCALL void	CG_AddParticles (void);
CCALL void	CG_ParticleSnow (qhandle_t pshader, vec3_t origin, vec3_t origin2, int turb, float range, int snum);
CCALL void	CG_ParticleSmoke (qhandle_t pshader, centity_t *cent);
CCALL void	CG_AddParticleShrapnel (localEntity_t *le);
CCALL void	CG_ParticleSnowFlurry (qhandle_t pshader, centity_t *cent);
CCALL void	CG_ParticleBulletDebris (vec3_t	org, vec3_t vel, int duration);
CCALL void	CG_ParticleSparks (vec3_t org, vec3_t vel, int duration, float x, float y, float speed);
CCALL void	CG_ParticleDust (centity_t *cent, vec3_t origin, vec3_t dir);
CCALL void	CG_ParticleMisc (qhandle_t pshader, vec3_t origin, int size, int duration, float alpha);
CCALL void	CG_ParticleExplosion (char *animStr, vec3_t origin, vec3_t vel, int duration, int sizeStart, int sizeEnd);
EXTERNC qboolean		initparticles;
CCALL int CG_NewParticleArea ( int num );


// cl_cgame.c
CCALL void     CL_GetGameState(gameState_t *gs);
CCALL qboolean CL_GetUserCmd  (int cmdNumber, usercmd_t *ucmd);
CCALL int      CL_GetCurrentCmdNumber();
CCALL qboolean CL_GetParseEntityState( int parseEntityNumber, entityState_t *state );
CCALL void     CL_GetCurrentSnapshotNumber( int *snapshotNumber, int *serverTime );
CCALL qboolean CL_GetSnapshot( int snapshotNumber, snapshot_t *snapshot );
CCALL void     CL_SetUserCmdValue( int userCmdValue, float sensitivityScale );
CCALL void     CL_AddCgameCommand( const char *cmdName );
CCALL void     CL_ConfigstringModified();
CCALL qboolean CL_GetServerCommand( int serverCommandNumber );
CCALL void     CL_CM_LoadMap( const char *mapname );
CCALL void     CL_InitCGame();
CCALL qboolean CL_GameCommand();
CCALL void     CL_AdjustTimeDelta();
CCALL void     CL_FirstSnapshot();
CCALL void     CL_SetCGameTime();

#endif