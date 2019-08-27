// Copyright (C) 1999-2000 Id Software, Inc.
//
// bg_local.h -- local definitions for the bg (both games) files

#define	TIMER_LAND		130
#define	TIMER_GESTURE	(34*66+50)
#define	OVERCLIP		1.001f
#define	JUMP_VELOCITY	270

// all of the locals will be zeroed before each
// pmove, just to make damn sure we don't have
// any differences when running on client or server
typedef struct {
	vec3_t		forward, right, up;
	float		frametime;
	int			msec;
	qbool	    walking;
	qbool       ladder;
	qbool	    groundPlane;
	trace_t		groundTrace;
	float		impactSpeed;
	vec3_t		previous_origin;
	vec3_t		previous_velocity;
	int			previous_waterlevel;
} pml_t;

extern	pmove_t		*pm;
extern	pml_t		pml;

extern int    c_pmove;        // used for debugging

void           PM_ClipVelocity( vec3_t in, vec3_t normal, vec3_t out, float overbounce );
void           PM_ClipVelocity2( vec3_t in, vec3_t normal, vec3_t out, float overbounce );
void           PM_AddTouchEnt( int entityNum );
void           PM_AddEvent( int newEvent );
void	       PM_StepSlideMove( qbool gravity );
qbool          PM_SlideMove( qbool gravity );
