/*
 *	aibsmod.h -- This header file is shared by all 3 game components.
 *	It's here for version info.
 *
 */

#ifndef __AIBSMOD_H__
#define __AIBSMOD_H__

#define GAMEVERSION		"aibsmod_test10"
#define	GAME_VERSION	"aibsmod-1"

#define VERSION_STRING	"aibsmod v1.00rc1"

//Couldn't find a suitable place for these, didn't want to modify q_shared.h
#define MIN(x,y) ((x < y) ? (x) : (y))
#define MAX(x,y) ((x > y) ? (x) : (y))

//buttonstate bitmask for the EV_CURRENT_BUTTONS event
#define BTNFLAG_BUTTON_UP			1
#define BTNFLAG_BUTTON_DOWN			2
#define BTNFLAG_BUTTON_LEFT			4
#define BTNFLAG_BUTTON_RIGHT		8
#define BTNFLAG_BUTTON_JUMP			16
#define BTNFLAG_BUTTON_FIRE			64

//Model coloring parts
typedef enum {
	AM_COLORPART_NONE = -1,

	AM_COLORPART_HEAD,
	AM_COLORPART_TORSO,
	AM_COLORPART_LEGS,
	AM_COLORPART_1,
	AM_COLORPART_2,

	AM_MAX_COLORPARTS
} amColorpart_t;

//Demo fast-forwarding
typedef enum {
	AM_DEMOFF_OFF,
	AM_DEMOFF_LEVELTIME,	//Fast-forward until level time
	AM_DEMOFF_TIMEOFFSET,	//Fast-forward n seconds
} amDemoFF_t;

//The constants below are used by game mechanics

#define WEAPON_REPICK_TIME				5000
#define TRAININGMODE_TELEPORT_DELAY		200

//g_aibsmod.c
#define MASK_TELEPORT				(CONTENTS_SOLID | CONTENTS_PLAYERCLIP)
#define MAX_TELEPORT_DISTANCE		8192.0f

//how many unit vectors to move back if teleport would put us in a solid
#define TELEPORT_BACKTRACE_FRACTION	8.0f

//how many times to initially backtrace using the normal of the solid we hit
#define TELEPORT_ANTINORMAL_COUNT	5

//how many times to backtrace using the inverse forward vector
#define TELEPORT_BACKTRACE_COUNT	100

//g_football.c
#define BALL_RADIUS				8.0f
#define BALL_SHOOT_SPEED		1000

#define BALL_BOUNCE_FACTOR		0.90f
#define BALL_BOUNCE_GOAL		0.50f
#define BALL_AIR_FRICTION		0.995f

#define BALL_IDLE_RESET_TIME	30000
#define BALL_GOAL_RESET_TIME	10000
#define BALL_RECATCH_TIME		500

//how much the ball should turn per unit traveled
#define BALL_ANGLE_SPEED		0.1

#define CONTENTS_GOALPOST		(CONTENTS_SOLID | CONTENTS_PLAYERCLIP)
#define CONTENTS_GOALCLIP		CONTENTS_PLAYERCLIP

#define GOAL_SCORE				10
#define OWN_GOAL_SCORE			-10
#define GOAL_ASSIST_SCORE		5

//g_tripmine.c
#define TRIPMINE_MASK			(CONTENTS_SOLID|CONTENTS_BODY)
#define TRIPMINE_PLACE_RANGE	40.0f
#define TRIPMINE_THINK_DELAY	50
#define TRIPMINE_RANGE			8192
#define TRIPMINE_ARM_TIME		2500

//g_redeemer.c
#define REDEEMER_MASK			(CONTENTS_SOLID|CONTENTS_BODY)

//bg_pmove.c
//Speed of the redeemer missile
#define REDEEMER_VELOCITY		300.0f
//How much of player's forward vector affects the missile trajectory
#define REDEEMER_MOVE_CONSTANT	0.9f

//explode if laser length changes this much in TRIPMINE_THINK_DELAY
//to allow crawling away at 50ups, for example, this should be 50 * (1000 / TRIPMINE_THINK_DELAY)
#define TRIPMINE_TRIP_DELTA		3.0f

#define TRIPMINE_HEALTH			60
#define TRIPMINE_DAMAGE			150
#define TRIPMINE_SPLASH			360

//alive for this long
#define TRIPMINE_LIFETIME		(300000 + TRIPMINE_ARM_TIME)
#define TRIPMINE_EXPLODE_DELAY	200

#endif /*__AIBSMOD_H__*/
