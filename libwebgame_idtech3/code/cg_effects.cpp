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

#include "cg_local.h"

void CG_BubbleTrail( vec3_t start, vec3_t end, float spacing ) {

}

localEntity_t *CG_SmokePuff( const vec3_t p, const vec3_t vel, 
				   float radius,
				   float r, float g, float b, float a,
				   float duration,
				   int startTime,
				   int fadeInTime,
				   int leFlags,
				   qhandle_t hShader ) {
	localEntity_t	*le;
	le = CG_AllocLocalEntity();
	return le;
}

void CG_SpawnEffect( vec3_t org ) {

}

void CG_ScorePlum( int client, vec3_t org, int score ) {
}

localEntity_t *CG_MakeExplosion( vec3_t origin, vec3_t dir, 
								qhandle_t hModel, qhandle_t shader,
								int msec, qboolean isSprite ) {
	return CG_AllocLocalEntity();
}

void CG_Bleed( vec3_t origin, int entityNum ) {
}

void CG_LaunchGib( vec3_t origin, vec3_t velocity, qhandle_t hModel ) {

}

void CG_GibPlayer( vec3_t playerOrigin ) {

}

void CG_LaunchExplode( vec3_t origin, vec3_t velocity, qhandle_t hModel ) {
	
}

void CG_BigExplode( vec3_t playerOrigin ) {
	
}
