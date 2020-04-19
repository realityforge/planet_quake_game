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
// bg_local.h -- local definitions for the bg (both games) files


#ifndef BG_LOCAL_H
#define BG_LOCAL_H

//#define	MIN_WALK_NORMAL	0.7f		// can't walk on very steep slopes

#define	STEPSIZE		18

//#define	JUMP_VELOCITY	270

#define	TIMER_LAND		130
#define	TIMER_GESTURE	(34*66+50)

#define	OVERCLIP		1.001f

// all of the locals will be zeroed before each
// pmove, just to make damn sure we don't have
// any differences when running on client or server
typedef struct pml_s {
	vec3_t		forward, right, up;
	float		frametime;

	int			msec;

	qboolean	walking;
	qboolean	groundPlane;
	trace_t		groundTrace;

	float		impactSpeed;

	vec3_t		previous_origin;
	vec3_t		previous_velocity;
	int			previous_waterlevel;

	qboolean ladder; // ripped from et:legacy
} pml_t;

EXTERNC	pmove_t		*pm;
EXTERNC	pml_t		pml;

// movement parameters
EXTERNC float	pm_stopspeed;
EXTERNC float	pm_duckScale;
EXTERNC float	pm_swimScale;
EXTERNC float	pm_accelerate;
EXTERNC float	pm_airaccelerate;
EXTERNC float	pm_wateraccelerate;
EXTERNC float	pm_flyaccelerate;
EXTERNC float	pm_friction;
EXTERNC float	pm_waterfriction;
EXTERNC float	pm_flightfriction;

EXTERNC	int		c_pmove;

CCALL void PM_ClipVelocity( vec3_t in, vec3_t normal, vec3_t out, float overbounce );
CCALL void PM_AddTouchEnt( int entityNum );
CCALL void PM_AddEvent( int newEvent );
CCALL qboolean	PM_SlideMove(qboolean gravity);
CCALL void PM_StepSlideMove(qboolean gravity);

EXTERNC int clientframe; // is 1 when the code is executed as part of CL_Frame

#endif