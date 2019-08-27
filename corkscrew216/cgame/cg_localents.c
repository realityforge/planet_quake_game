// Copyright (C) 1999-2000 Id Software, Inc.
//

// cg_localents.c -- every frame, generate renderer commands for locally
// processed entities, like smoke puffs, gibs, shells, etc.

#include "cg_local.h"

#define	MAX_LOCAL_ENTITIES	768
localEntity_t	cg_localEntities[MAX_LOCAL_ENTITIES];
localEntity_t	cg_activeLocalEntities;		// double linked list
localEntity_t	*cg_freeLocalEntities;		// single linked list

/*
===================
CG_InitLocalEntities

this is called at startup and for tournement restarts
===================
*/
void	CG_InitLocalEntities( void ) {
	int		i;

	memset( cg_localEntities, 0, sizeof( cg_localEntities ) );
	cg_activeLocalEntities.next = &cg_activeLocalEntities;
	cg_activeLocalEntities.prev = &cg_activeLocalEntities;
	cg_freeLocalEntities = cg_localEntities;
	for ( i = 0 ; i < MAX_LOCAL_ENTITIES - 1 ; i++ ) {
		cg_localEntities[i].next = &cg_localEntities[i+1];
	}
}


/*
==================
CG_FreeLocalEntity
==================
*/
void CG_FreeLocalEntity( localEntity_t *le ) {
	if ( !le->prev ) {
		CG_Error( "CG_FreeLocalEntity: not active" );
	}

	// remove from the doubly linked active list
	le->prev->next = le->next;
	le->next->prev = le->prev;

	// the free list is only singly linked
	le->next = cg_freeLocalEntities;
	cg_freeLocalEntities = le;
}

/*
===================
CG_AllocLocalEntity

Will allways succeed, even if it requires freeing an old active entity
===================
*/
localEntity_t	*CG_AllocLocalEntity( void ) {
	localEntity_t	*le;

	if ( !cg_freeLocalEntities ) {
		// no free entities, so free the one at the end of the chain
		// remove the oldest active entity
		CG_FreeLocalEntity( cg_activeLocalEntities.prev );
	}

	le = cg_freeLocalEntities;
	cg_freeLocalEntities = cg_freeLocalEntities->next;

	memset( le, 0, sizeof( *le ) );

	// link into the active list
	le->next = cg_activeLocalEntities.next;
	le->prev = &cg_activeLocalEntities;
	cg_activeLocalEntities.next->prev = le;
	cg_activeLocalEntities.next = le;
	return le;
}

localEntity_t	*CG_AllocLocalEntity_2( void ) {
	localEntity_t	*le;

	if ( !cg_freeLocalEntities ) {
		return NULL;
	}

	le = cg_freeLocalEntities;
	cg_freeLocalEntities = cg_freeLocalEntities->next;

	memset( le, 0, sizeof( *le ) );

	// link into the active list
	le->next = cg_activeLocalEntities.next;
	le->prev = &cg_activeLocalEntities;
	cg_activeLocalEntities.next->prev = le;
	cg_activeLocalEntities.next = le;
	return le;
}

/*
====================================================================================

FRAGMENT PROCESSING

A fragment localentity interacts with the environment in some way (hitting walls),
or generates more localentities along a trail.

====================================================================================
*/

/*
================
CG_BloodTrail

Leave expanding blood puffs behind gibs
================
*/
void CG_BloodTrail( localEntity_t *le ) {
	vec3_t			newOrigin;
	localEntity_t	*blood;

	if( cg.time > le->trailTime ) {
		BG_EvaluateTrajectory( &le->pos, le->trailTime, newOrigin );

		blood = CG_SmokePuff( newOrigin, vec3_origin,
					  20 * le->lifeRate,	// radius * size of the gib
					  1, 1, 1, 1,	// color
					  2000,		// trailTime
					  le->trailTime,		// startTime
					  0,		// fadeInTime
					  0,		// flags
					  cgs.media.bloodTrailShader );

		if( blood ) {	// CG_SmokePuff can return NULL
			if( le->quad == 3 || cgs.valkyrMode ) {
				// use the trippy version :)
				blood->leType = LE_TRIPPY_FALL_SCALE_FADE;
				blood->refEntity.customShader = cgs.media.trippyBloodTrail;
			} else {
				// use the optimized version
				blood->leType = LE_FALL_SCALE_FADE;
			}

			// drop a total of 40 units over its lifetime
			blood->pos.trDelta[2] = 40;
		}

		le->trailTime += 150;
	}
}


/*
================
CG_FragmentBounceMark
================
*/
void CG_FragmentBounceMark( localEntity_t *le, trace_t *trace ) {
	int			radius;
/*
	if ( le->leMarkType == LEMT_BLOOD ) {

		radius = 16 + (rand()&31);
		CG_ImpactMark( cgs.media.bloodMarkShader, trace->endpos, trace->plane.normal, random()*360,
			1,1,1,1, qtrue, radius, qfalse );
	} else if ( le->leMarkType == LEMT_BURN ) {

		radius = 8 + (rand()&15);
		CG_ImpactMark( cgs.media.burnMarkShader, trace->endpos, trace->plane.normal, random()*360,
			1,1,1,1, qtrue, radius, qfalse );
	}
*/

	if ( le->leMarkType == LEMT_BLOOD ) {
		float		speed;
		qhandle_t	shader;

		// le->quad == 2 means red quad, but red quad blood graphic is not needed
		if ( le->quad == 1 ) shader = cgs.media.quadBloodMarkShader;
			else shader = cgs.media.bloodMarkShader;

		speed = VectorLength( le->pos.trDelta );
		if ( trap_CM_PointContents( le->pos.trBase, 0 ) & CONTENTS_WATER ) speed *= 0.2;

		radius = 8 + speed / 20.0;
		if ( radius > 48 ) radius = 48;
		radius *= le->lifeRate;
		CG_ImpactMark( shader, trace->endpos, trace->plane.normal, random()*360,
			1,1,1,1, qtrue, radius, qfalse, qfalse );
	}

	// don't allow a fragment to make multiple marks, or they
	// pile up while settling
	//le->leMarkType = LEMT_NONE;
}

/*
================
CG_FragmentBounceSound
================
*/
void CG_FragmentBounceSound( localEntity_t *le, trace_t *trace ) {
	// decided not to change anything, i'm just too attached to the standard sound >:)
	if ( le->leBounceSoundType == LEBS_BLOOD ) {
		// half the gibs will make splat sounds
		if ( rand() & 1 ) {
			int r = rand()&3;
			sfxHandle_t	s;

			if ( r < 2 ) {
				s = cgs.media.gibBounce1Sound;
			} else if ( r == 2 ) {
				s = cgs.media.gibBounce2Sound;
			} else {
				s = cgs.media.gibBounce3Sound;
			}
			trap_S_StartSound( trace->endpos, ENTITYNUM_WORLD, CHAN_AUTO, s );
		}
	}

	le->leBounceSoundType = LEBS_NONE;
}


/*
================
CG_ReflectVelocity
================
*/
void CG_ReflectVelocity( localEntity_t *le, trace_t *trace ) {
	vec3_t	velocity;
	float	dot;
	int		hitTime;

	// reflect the velocity on the trace plane
	hitTime = cg.time - cg.frametime + cg.frametime * trace->fraction;
	BG_EvaluateTrajectoryDelta( &le->pos, hitTime, velocity );
	dot = DotProduct( velocity, trace->plane.normal );
	VectorMA( velocity, -2*dot, trace->plane.normal, le->pos.trDelta );

	// scale down bouncefactor at extreme speeds
	if( le->pos.trDelta[2] > 1000 ) {
		dot = ( 2000 - le->pos.trDelta[2] ) / 1000.0 * le->bounceFactor;
		VectorScale( le->pos.trDelta, dot, le->pos.trDelta );
	} else {
		VectorScale( le->pos.trDelta, le->bounceFactor, le->pos.trDelta );
	}

	VectorCopy( trace->endpos, le->pos.trBase );
	le->pos.trTime = cg.time;


	// check for stop, making sure that even on low FPS systems it doesn't bobble
	if ( trace->allsolid ||
		( trace->plane.normal[2] > 0 &&
		( le->pos.trDelta[2] < 40 || le->pos.trDelta[2] < -cg.frametime * le->pos.trDelta[2] ) ) ) {
		le->pos.trType = TR_STATIONARY;
	} else {

	}
}

void CG_AddFragmentWithQuad( localEntity_t *le ) {
	qhandle_t save;
	trap_R_AddRefEntityToScene( &le->refEntity );

	if ( le->quad ) {
		save = le->refEntity.customShader;

		if ( le->quad == 1 ) le->refEntity.customShader = cgs.media.quadWeaponShader;
		else if ( le->quad == 2 ) le->refEntity.customShader = cgs.media.redQuadWeaponShader;
		// we also use (quad == 3)

		trap_R_AddRefEntityToScene( &le->refEntity );
		le->refEntity.customShader = save;
	}
}

/*
================
CG_AddParticle
================
*/
static void CG_AddParticle( localEntity_t *le ) {
	vec3_t		newOrigin;
	trace_t	trace;
	int 		contents;
	vec3_t 	temp;

	BG_EvaluateTrajectory( &le->pos, cg.time, newOrigin );

	// no tracing, just get the contents and free it if it's in solid
	if ( !( trap_CM_PointContents( newOrigin, 0 ) & CONTENTS_SOLID ) ) {
		float	c;

		VectorCopy( newOrigin, le->refEntity.origin );

		c = 1.0 - ( le->endTime - cg.time ) * le->lifeRate;
		c = 1.0 - c*c;
		c *= 255;
		VectorSet( le->refEntity.shaderRGBA, c, c, c );
		le->refEntity.rotation = ( (cg.time + ((int)le) * 100 ) & 255) * 360.0 / 256.0;

		trap_R_AddRefEntityToScene( &le->refEntity );
	} else {
		CG_FreeLocalEntity( le );
	}
}

/*
================
CG_AddFragment
================
*/
void CG_AddFragment( localEntity_t *le ) {
	vec3_t	newOrigin, angles;
	trace_t	trace;

	if ( le->pos.trType == TR_STATIONARY ) {
		// sink into the ground if near the removal time
		int		t;
		float	oldZ;

		t = le->endTime - cg.time;
		if ( t < SINK_TIME ) {
			// we must use an explicit lighting origin, otherwise the
			// lighting would be lost as soon as the origin went
			// into the ground
			VectorCopy( le->refEntity.origin, le->refEntity.lightingOrigin );
			le->refEntity.renderfx |= RF_LIGHTING_ORIGIN;
			oldZ = le->refEntity.origin[2];
			le->refEntity.origin[2] -= 16 * ( 1.0 - (float)t / SINK_TIME );
			CG_AddFragmentWithQuad( le );
			le->refEntity.origin[2] = oldZ;
		} else {
			CG_AddFragmentWithQuad( le );
		}

		return;
	}

	// never free gibs while they're flying
	if ( le->endTime < cg.time + 2000 )
		le->endTime = cg.time + 2000;

	// calculate new position
	BG_EvaluateTrajectory( &le->pos, cg.time, newOrigin );

	// trace a line from previous position to new position
	CG_Trace( &trace, le->refEntity.origin, NULL, NULL, newOrigin, -1, CONTENTS_SOLID );
	if ( trace.fraction == 1.0 ) {
		// still in free fall
		VectorCopy( newOrigin, le->refEntity.origin );

		if ( le->leFlags & LEF_TUMBLE ) {
			VectorClear( angles );
			angles[1] = ( ( cg.time & 2047 ) * 360 / 2048.0 + 120 );
			if ( !(le->leFlags & LEF_HEAD ) ) {
				angles[0] = ( ( cg.time & 2047 ) * 360 / 2048.0 );
				angles[2] = ( ( cg.time & 2047 ) * 360 / 2048.0 + 240 );
			}
			AnglesToAxis( angles, le->refEntity.axis );
		}

		CG_AddFragmentWithQuad( le );

		// add a blood trail
		if ( le->leMarkType == LEMT_BLOOD ) {
			CG_BloodTrail( le );
		}

		return;
	}

	// if it is in a nodrop zone, remove it
	// this keeps gibs from waiting at the bottom of pits of death
	// and floating levels
	if ( trap_CM_PointContents( trace.endpos, 0 ) & CONTENTS_NODROP ) {
		CG_FreeLocalEntity( le );
		return;
	}

	// leave a mark
	CG_FragmentBounceMark( le, &trace );

	// do a bouncy sound
	CG_FragmentBounceSound( le, &trace );

	// reflect the velocity on the trace plane
	CG_ReflectVelocity( le, &trace );

	CG_AddFragmentWithQuad( le );
}

/*
=====================================================================

TRIVIAL LOCAL ENTITIES

These only do simple scaling or modulation before passing to the renderer
=====================================================================
*/

/*
====================
CG_AddFadeRGB
====================
*/
void CG_AddFadeRGB( localEntity_t *le ) {
	refEntity_t *re;
	float c;

	re = &le->refEntity;

	c = ( le->endTime - cg.time ) * le->lifeRate;
	c *= 0xff;

	re->shaderRGBA[0] = le->color[0] * c;
	re->shaderRGBA[1] = le->color[1] * c;
	re->shaderRGBA[2] = le->color[2] * c;
	re->shaderRGBA[3] = le->color[3] * c;

	trap_R_AddRefEntityToScene( re );
}

/*
====================
CG_AddTrippyFadeRGB
====================
*/
void CG_AddTrippyFadeRGB( localEntity_t *le ) {
	refEntity_t *re;
	float c;
	int i;

	re = &le->refEntity;

	c = ( le->endTime - cg.time ) * le->lifeRate;

	i = ( le->endTime / 2 - cg.time / 2 ) % 256;

	re->shaderRGBA[0] = trippyColors[i].red * c;
	re->shaderRGBA[1] = trippyColors[i].green * c;
	re->shaderRGBA[2] = trippyColors[i].blue * c;
	re->shaderRGBA[3] = 255 * c;

	trap_R_AddRefEntityToScene( re );
}

/*
====================
CG_AddFadePulseRGB
====================
*/
void CG_AddFadePulseRGB( localEntity_t *le ) {
	refEntity_t *re;
	float c, d;
	int i, j;
	polyVert_t polyverts[4];

	re = &le->refEntity;

	c = ( le->endTime - cg.time ) * le->lifeRate;
	d = ( le->endTime - cg.time ) % 500;
	if ( d < 50 ) {
		d -= 25;
		d = d < 0 ? -d : d;
		d = 25 - d;
		d = 1 + d / 25.0;
		c *= d;
		if ( c > 1.0 ) {
			c = 1.0;
		}
	}

	c *= 255;

	re->shaderRGBA[0] = le->color[0] * c;
	re->shaderRGBA[1] = le->color[1] * c;
	re->shaderRGBA[2] = le->color[2] * c;
	re->shaderRGBA[3] = 255;

	trap_R_AddRefEntityToScene( re );
}


//	for quad damage effect
void CG_AddFadeScaleModel( localEntity_t *le, qboolean trippy ) {
	refEntity_t *re;
	float c;
	vec3_t backup[3];
	int i;

	re = &le->refEntity;

	c = ( le->endTime - cg.time ) * le->lifeRate;

	if( trippy ) {
		i = ( le->endTime / 2 - cg.time / 2 ) % 256;
		re->shaderRGBA[0] = trippyColors[i].red * c;
		re->shaderRGBA[1] = trippyColors[i].green * c;
		re->shaderRGBA[2] = trippyColors[i].blue * c;
		re->shaderRGBA[3] = 255 * c;
	} else {
		c *= 0xff;
		re->shaderRGBA[0] = le->color[0] * c;
		re->shaderRGBA[1] = le->color[1] * c;
		re->shaderRGBA[2] = le->color[2] * c;
		re->shaderRGBA[3] = le->color[3] * c;
		c /= 255;
	}

	AxisCopy( re->axis, backup );

	VectorScale( re->axis[0], 0.5, re->axis[0] );
	VectorScale( re->axis[1], 0.5, re->axis[1] );
	VectorScale( re->axis[2], 0.5, re->axis[2] );
	VectorMA( re->axis[0], (1-c), re->axis[0], re->axis[0] );
	VectorMA( re->axis[1], (1-c), re->axis[1], re->axis[1] );
	VectorMA( re->axis[2], (1-c), re->axis[2], re->axis[2] );

	trap_R_AddRefEntityToScene( re );

	AxisCopy( backup, re->axis );
}

/*
==================
CG_AddMoveScaleFade
==================
*/
static void CG_AddMoveScaleFade( localEntity_t *le ) {
	refEntity_t	*re;
	float		c;
	vec3_t		delta;
	float		len;

	re = &le->refEntity;

	if ( le->fadeInTime > le->startTime && cg.time < le->fadeInTime ) {
		// fade / grow time
		c = 1.0 - (float) ( le->fadeInTime - cg.time ) / ( le->fadeInTime - le->startTime );
	}
	else {
		// fade / grow time
		c = ( le->endTime - cg.time ) * le->lifeRate;
	}

	re->shaderRGBA[3] = 0xff * c * le->color[3];

	if ( !( le->leFlags & LEF_PUFF_DONT_SCALE ) ) {
		re->radius = le->radius * ( 1.0 - c ) + 8;
	}

	BG_EvaluateTrajectory( &le->pos, cg.time, re->origin );

	// if the view would be "inside" the sprite, kill the sprite
	// so it doesn't add too much overdraw
	VectorSubtract( re->origin, cg.refdef.vieworg, delta );
	len = VectorLength( delta );
	if ( len < le->radius ) {
		CG_FreeLocalEntity( le );
		return;
	}

	trap_R_AddRefEntityToScene( re );
}


/*
===================
CG_AddScaleFade

for rocket smokes that hang in place, fade out, and are
removed if the view passes through them.
There are often many of these, so it needs to be simple.
===================
*/
static void CG_AddScaleFade( localEntity_t *le ) {
	refEntity_t	*re;
	float		c;
	vec3_t		delta;
	float		len;

	re = &le->refEntity;

	// fade / grow time
	c = ( le->endTime - cg.time ) * le->lifeRate;

	re->shaderRGBA[3] = 0xff * c * le->color[3];
	re->radius = le->radius * ( 1.0 - c ) + 8;

	// if the view would be "inside" the sprite, kill the sprite
	// so it doesn't add too much overdraw
	VectorSubtract( re->origin, cg.refdef.vieworg, delta );
	len = VectorLength( delta );
	if ( len < le->radius ) {
		CG_FreeLocalEntity( le );
		return;
	}

	trap_R_AddRefEntityToScene( re );
}


/*
=================
CG_AddFallScaleFade

this is just an optimized CG_AddMoveScaleFade
for blood mists that drift down, fade out, and are
removed if the view passes through them.
There are often 100+ of these, so it needs to be simple.
=================
*/
static void CG_AddFallScaleFade( localEntity_t *le, qboolean trippy ) {
	refEntity_t	*re;
	int i;
	float		c;
	vec3_t		delta;
	float		len;

	re = &le->refEntity;

	// fade time
	c = ( le->endTime - cg.time ) * le->lifeRate;

	if( trippy ) {
		i = ( le->startTime / 4 + cg.time / 4 ) % 256;
		re->shaderRGBA[0] = trippyColors[i].red * c;
		re->shaderRGBA[1] = trippyColors[i].green * c;
		re->shaderRGBA[2] = trippyColors[i].blue * c;
		re->shaderRGBA[3] = 1;
	} else {
		re->shaderRGBA[3] = 0xff * c * le->color[3];
	}

	re->origin[2] = le->pos.trBase[2] - ( 1.0 - c ) * le->pos.trDelta[2];

	re->radius = le->radius * ( 1.0 - c ) + 16;

	// if the view would be "inside" the sprite, don't draw the sprite
	// killing it will make it seem less realistic at times...
	VectorSubtract( re->origin, cg.refdef.vieworg, delta );
	len = VectorLength( delta );
	if ( len > le->radius ) {
		trap_R_AddRefEntityToScene( re );
	}
}

/*
================
CG_AddExplosion
================
*/
static void CG_AddExplosion( localEntity_t *ex ) {
	refEntity_t	*ent;

	ent = &ex->refEntity;

	// add the entity
	trap_R_AddRefEntityToScene(ent);

	// add the dlight
	if ( ex->light ) {
		float		light;

		light = (float)( cg.time - ex->startTime ) / ( ex->endTime - ex->startTime );
		if ( light < 0.5 ) {
			light = 1.0;
		} else {
			light = 1.0 - ( light - 0.5 ) * 2;
		}
		light = ex->light * light;
		trap_R_AddLightToScene(ent->origin, light, ex->lightColor[0], ex->lightColor[1], ex->lightColor[2] );
	}
}

/*
================
CG_AddSpriteExplosion
================
*/
static void CG_AddSpriteExplosion( localEntity_t *le ) {
	refEntity_t	re;
	float c;

	re = le->refEntity;

	c = ( le->endTime - cg.time ) / ( float ) ( le->endTime - le->startTime );
	if ( c > 1 ) {
		c = 1.0;	// can happen during connection problems
	}

	re.shaderRGBA[0] = 0xff;
	re.shaderRGBA[1] = 0xff;
	re.shaderRGBA[2] = 0xff;
	re.shaderRGBA[3] = 0xff * c * 0.33;

	re.reType = RT_SPRITE;
	re.radius = 42 * ( 1.0 - c ) + 30;

	trap_R_AddRefEntityToScene( &re );

	// add the dlight
	if ( le->light ) {
		float		light;

		light = (float)( cg.time - le->startTime ) / ( le->endTime - le->startTime );
		if ( light < 0.5 ) {
			light = 1.0;
		} else {
			light = 1.0 - ( light - 0.5 ) * 2;
		}
//		light = le->light * light;
		trap_R_AddLightToScene(re.origin, le->light, le->lightColor[0]*light, le->lightColor[1]*light, le->lightColor[2]*light );
	}
}


/*
===================
CG_AddScorePlum
===================
*/
#define NUMBER_SIZE		8

void CG_AddScorePlum( localEntity_t *le ) {
	refEntity_t	*re;
	vec3_t		origin, delta, dir, vec, up = {0, 0, 1};
	float		c, len;
	int			i, score, digits[10], numdigits, negative;

	re = &le->refEntity;

	c = ( le->endTime - cg.time ) * le->lifeRate;

	score = le->radius;
	if (score < 0) {
		re->shaderRGBA[0] = 0xff;
		re->shaderRGBA[1] = 0x11;
		re->shaderRGBA[2] = 0x11;
	}
	else {
		re->shaderRGBA[0] = 0xff;
		re->shaderRGBA[1] = 0xff;
		re->shaderRGBA[2] = 0xff;
		if (score >= 50) {
			re->shaderRGBA[1] = 0;
		} else if (score >= 20) {
			re->shaderRGBA[0] = re->shaderRGBA[1] = 0;
		} else if (score >= 10) {
			re->shaderRGBA[2] = 0;
		} else if (score >= 2) {
			re->shaderRGBA[0] = re->shaderRGBA[2] = 0;
		}

	}
	if (c < 0.25)
		re->shaderRGBA[3] = 0xff * 4 * c;
	else
		re->shaderRGBA[3] = 0xff;

	re->radius = NUMBER_SIZE / 2;

	VectorCopy(le->pos.trBase, origin);
	origin[2] += 110 - c * 100;

	VectorSubtract(cg.refdef.vieworg, origin, dir);
	CrossProduct(dir, up, vec);
	VectorNormalize(vec);

	VectorMA(origin, -10 + 20 * sin(c * 2 * M_PI), vec, origin);

	// if the view would be "inside" the sprite, kill the sprite
	// so it doesn't add too much overdraw
	VectorSubtract( origin, cg.refdef.vieworg, delta );
	len = VectorLength( delta );
	if ( len < 20 ) {
		CG_FreeLocalEntity( le );
		return;
	}

	negative = qfalse;
	if (score < 0) {
		negative = qtrue;
		score = -score;
	}

	for (numdigits = 0; !(numdigits && !score); numdigits++) {
		digits[numdigits] = score % 10;
		score = score / 10;
	}

	if (negative) {
		digits[numdigits] = 10;
		numdigits++;
	}

	for (i = 0; i < numdigits; i++) {
		VectorMA(origin, (float) (((float) numdigits / 2) - i) * NUMBER_SIZE, vec, re->origin);
		re->customShader = cgs.media.numberShaders[digits[numdigits-1-i]];
		trap_R_AddRefEntityToScene( re );
	}
}




//==============================================================================

/*
===================
CG_AddLocalEntities

===================
*/
void CG_AddLocalEntities( void ) {
	localEntity_t	*le, *next;

	// walk the list backwards, so any new local entities generated
	// (trails, marks, etc) will be present this frame
	le = cg_activeLocalEntities.prev;
	for ( ; le != &cg_activeLocalEntities ; le = next ) {
		// grab next now, so if the local entity is freed we
		// still have it
		next = le->prev;

		if ( cg.time >= le->endTime ) {
			CG_FreeLocalEntity( le );
			continue;
		}
		switch ( le->leType ) {
		default:
			CG_Error( "Bad leType: %i", le->leType );
			break;

		case LE_MARK:
			break;

		case LE_SPRITE_EXPLOSION:
			CG_AddSpriteExplosion( le );
			break;

		case LE_EXPLOSION:
			CG_AddExplosion( le );
			break;

		case LE_FRAGMENT:					// gibs and brass
			CG_AddFragment( le );
			break;

		case LE_PARTICLE:					// energy debris from railgun explosions
			CG_AddParticle( le );
			break;

		case LE_MOVE_SCALE_FADE:			// water bubbles
			CG_AddMoveScaleFade( le );
			break;

		case LE_TRIPPY_FADE_RGB:					// teleporters, railtrails
			CG_AddTrippyFadeRGB( le );
			break;

		case LE_FADE_RGB:					// teleporters, railtrails
			CG_AddFadeRGB( le );
			break;

		case LE_FADE_PULSE_RGB:				// railtrails ( screws )
			CG_AddFadePulseRGB( le );
			break;

		case LE_FADE_SCALE_MODEL:			// quad damage effect
			CG_AddFadeScaleModel( le, qfalse );
			break;

		case LE_FADE_SCALE_MODEL_TRIPPY:			// quad damage effect
			CG_AddFadeScaleModel( le, qtrue );
			break;

		case LE_FALL_SCALE_FADE: 			// gib blood trails
			CG_AddFallScaleFade( le, qfalse );
			break;

		case LE_TRIPPY_FALL_SCALE_FADE: 	//  trippy gib blood trails >:)
			CG_AddFallScaleFade( le, qtrue );
			break;

		case LE_SCALE_FADE:					// rocket trails
			CG_AddScaleFade( le );
			break;

		case LE_SCOREPLUM:
			CG_AddScorePlum( le );
			break;
#ifdef MISSIONPACK
		case LE_KAMIKAZE:
			CG_AddKamikaze( le );
			break;
		case LE_INVULIMPACT:
			CG_AddInvulnerabilityImpact( le );
			break;
		case LE_INVULJUICED:
			CG_AddInvulnerabilityJuiced( le );
			break;
		case LE_SHOWREFENTITY:
			CG_AddRefEntity( le );
			break;
#endif
		}
	}
}




