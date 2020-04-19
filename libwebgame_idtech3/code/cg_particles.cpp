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


typedef struct particle_s
{
	struct particle_s	*next;

	float		time;
	float		endtime;

	vec3_t		org;
	vec3_t		vel;
	vec3_t		accel;
	int			color;
	float		colorvel;
	float		alpha;
	float		alphavel;
	int			type;
	qhandle_t	pshader;
	
	float		height;
	float		width;
				
	float		endheight;
	float		endwidth;
	
	float		start;
	float		end;

	float		startfade;
	qboolean	rotate;
	int			snum;
	
	qboolean	link;

	// Ridah
	int			shaderAnim;
	int			roll;

	int			accumroll;

} cparticle_t;



#define	MAX_SHADER_ANIMS		32
#define	MAX_SHADER_ANIM_FRAMES	64

static char *shaderAnimNames[MAX_SHADER_ANIMS] = {
	"explode1",
	NULL
};
static qhandle_t shaderAnims[MAX_SHADER_ANIMS][MAX_SHADER_ANIM_FRAMES];
static int	shaderAnimCounts[MAX_SHADER_ANIMS] = {
	23
};
static float	shaderAnimSTRatio[MAX_SHADER_ANIMS] = {
	1.0f
};
static int	numShaderAnims;
// done.


void CG_ClearParticles (void)
{

}

void CG_AddParticleToScene (cparticle_t *p, vec3_t org, float alpha)
{
}

void CG_AddParticles (void)
{

}

void CG_ParticleSnowFlurry (qhandle_t pshader, centity_t *cent)
{


}

void CG_ParticleSnow (qhandle_t pshader, vec3_t origin, vec3_t origin2, int turb, float range, int snum)
{


}

void CG_ParticleBubble (qhandle_t pshader, vec3_t origin, vec3_t origin2, int turb, float range, int snum)
{
	

}

void CG_ParticleSmoke (qhandle_t pshader, centity_t *cent)
{


}


void CG_ParticleBulletDebris (vec3_t org, vec3_t vel, int duration)
{

	
}


void CG_ParticleExplosion (char *animStr, vec3_t origin, vec3_t vel, int duration, int sizeStart, int sizeEnd)
{
	
}

void CG_AddParticleShrapnel (localEntity_t *le)
{
}

int CG_NewParticleArea (int num)
{

	return (1);
}

void	CG_SnowLink (centity_t *cent, qboolean particleOn)
{

}

void CG_ParticleImpactSmokePuff (qhandle_t pshader, vec3_t origin)
{

}

void CG_Particle_Bleed (qhandle_t pshader, vec3_t start, vec3_t dir, int fleshEntityNum, int duration)
{

}

void CG_Particle_OilParticle (qhandle_t pshader, centity_t *cent)
{
	

}


void CG_Particle_OilSlick (qhandle_t pshader, centity_t *cent)
{
	

}

void CG_OilSlickRemove (centity_t *cent)
{
	
}

qboolean ValidBloodPool (vec3_t start)
{
	return qtrue;
}

void CG_BloodPool (localEntity_t *le, qhandle_t pshader, trace_t *tr)
{	

}

void CG_ParticleBloodCloud (centity_t *cent, vec3_t origin, vec3_t dir)
{


	
}

void CG_ParticleSparks (vec3_t org, vec3_t vel, int duration, float x, float y, float speed)
{

	
}

void CG_ParticleDust (centity_t *cent, vec3_t origin, vec3_t dir)
{


	
}

void CG_ParticleMisc (qhandle_t pshader, vec3_t origin, int size, int duration, float alpha)
{

}
