// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_effects.c -- these functions generate localentities, usually as a result
// of event processing

#include "cg_local.h"

trippyColors_t trippyColors[256];

/*
=================
CG_GetTrippyColor

exact trippy color
=================
*/
void CG_GetTrippyColor( int offset, int recycleTime, vec3_t color ) {
	int time, recycleTime2, recycleTime3;

	recycleTime2 = recycleTime / 3.0;
	recycleTime3 = recycleTime2 * 2.0;

	time = ( cg.time + offset) % recycleTime;

	if ( time < recycleTime2 ) {
		color[2] = time / (float)recycleTime2;
		color[1] = 0;
		color[0] = 1 - color[2];
	} else if ( time < recycleTime3 ) {
		color[1] = ( time - recycleTime2 ) / (float)recycleTime2;
		color[0] = 0;
		color[2] = 1 - color[1];
	} else {
		color[0] = ( time - recycleTime3 ) / (float)recycleTime2;
		color[2] = 0;
		color[1] = 1 - color[0];
	}
}

/*
================
CG_InitTrippyColors

fill the trippyColors structure, for fast acces by localentities
================
*/
void CG_InitTrippyColors( void ) {
	vec3_t color;
	int i;

	for( i = 0; i < 256; i++ ) {
		CG_GetTrippyColor( i, 256, color );
		trippyColors[i].red = color[0] * 255;
		trippyColors[i].green = color[1] * 255;
		trippyColors[i].blue = color[2] * 255;
	}
}

/*
=====================
CG_SmokePuff

Adds a smoke puff or blood trail localEntity.
=====================
*/
localEntity_t *CG_SmokePuff( const vec3_t p, const vec3_t vel,
				   float radius,
				   float r, float g, float b, float a,
				   float duration,
				   int startTime,
				   int fadeInTime,
				   int leFlags,
				   qhandle_t hShader ) {
	static int	seed = 0x92;
	localEntity_t	*le;
	refEntity_t		*re;
//	int fadeInTime = startTime + duration / 2;

	le = CG_AllocLocalEntity_2();
	if( !le ) return NULL;

	le->leFlags = leFlags;
	le->radius = radius;

	re = &le->refEntity;
	re->rotation = Q_random( &seed ) * 360;
	re->radius = radius;
	re->shaderTime = startTime / 1000.0f;

	le->leType = LE_MOVE_SCALE_FADE;
	le->startTime = startTime;
	le->fadeInTime = fadeInTime;
	le->endTime = startTime + duration;
	if ( fadeInTime > startTime ) {
		le->lifeRate = 1.0 / ( le->endTime - le->fadeInTime );
	}
	else {
		le->lifeRate = 1.0 / ( le->endTime - le->startTime );
	}
	le->color[0] = r;
	le->color[1] = g;
	le->color[2] = b;
	le->color[3] = a;


	le->pos.trType = TR_LINEAR;
	le->pos.trTime = startTime;
	VectorCopy( vel, le->pos.trDelta );
	VectorCopy( p, le->pos.trBase );

	VectorCopy( p, re->origin );
	re->customShader = hShader;

	// rage pro can't alpha fade, so use a different shader
	if ( cgs.glconfig.hardwareType == GLHW_RAGEPRO ) {
		re->customShader = cgs.media.smokePuffRageProShader;
		re->shaderRGBA[0] = 0xff;
		re->shaderRGBA[1] = 0xff;
		re->shaderRGBA[2] = 0xff;
		re->shaderRGBA[3] = 0xff;
	} else {
		re->shaderRGBA[0] = le->color[0] * 0xff;
		re->shaderRGBA[1] = le->color[1] * 0xff;
		re->shaderRGBA[2] = le->color[2] * 0xff;
		re->shaderRGBA[3] = 0xff;
	}

	re->reType = RT_SPRITE;
	re->radius = le->radius;

	return le;
}

/*
==================
CG_SpawnEffect

Player teleporting in or out
==================
*/
void CG_SpawnEffect( vec3_t org, int clientNum ) {
	localEntity_t	*le;
	refEntity_t		*re;
	clientInfo_t	*ci;

	ci = &cgs.clientinfo[ clientNum ];

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_FADE_RGB;
	le->startTime = cg.time;
	le->endTime = cg.time + 500;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	VectorCopy( ci->color, le->color );
	//le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;

	re = &le->refEntity;

	re->reType = RT_MODEL;
	re->shaderTime = cg.time / 1000.0f;

#ifndef MISSIONPACK
	re->customShader = cgs.media.teleportEffectShader;
#endif
	re->hModel = cgs.media.teleportEffectModel;
	AxisClear( re->axis );

	VectorCopy( org, re->origin );
#ifdef MISSIONPACK
	re->origin[2] += 16;
#else
	re->origin[2] -= 24;
#endif
}

/*
==================
CG_ScorePlum
==================
*/
void CG_ScorePlum( int client, vec3_t org, int score ) {
	localEntity_t	*le;
	refEntity_t		*re;
	vec3_t			angles;
	static vec3_t lastPos;

	// only visualize for the client that scored
	if (client != cg.predictedPlayerState.clientNum || cg_scorePlum.integer == 0) {
		return;
	}

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_SCOREPLUM;
	le->startTime = cg.time;
	le->endTime = cg.time + 4000;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );


	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;
	le->radius = score;

	VectorCopy( org, le->pos.trBase );
	if (org[2] >= lastPos[2] - 20 && org[2] <= lastPos[2] + 20) {
		le->pos.trBase[2] -= 20;
	}

	//CG_Printf( "Plum origin %i %i %i -- %i\n", (int)org[0], (int)org[1], (int)org[2], (int)Distance(org, lastPos));
	VectorCopy(org, lastPos);


	re = &le->refEntity;

	re->reType = RT_SPRITE;
	re->radius = 16;

	VectorClear(angles);
	AnglesToAxis( angles, re->axis );
}


/*
====================
CG_MakeExplosion
====================
*/
localEntity_t *CG_MakeExplosion( vec3_t origin, vec3_t dir,
								qhandle_t hModel, qhandle_t shader,
								int msec, qboolean isSprite ) {
	float			ang;
	localEntity_t	*ex;
	int				offset;
	vec3_t			tmpVec, newOrigin;

	if ( msec <= 0 ) {
		CG_Error( "CG_MakeExplosion: msec = %i", msec );
	}

	// skew the time a bit so they aren't all in sync
	offset = rand() & 63;

	ex = CG_AllocLocalEntity();
	if ( isSprite ) {
		ex->leType = LE_SPRITE_EXPLOSION;

		// randomly rotate sprite orientation
		ex->refEntity.rotation = rand() % 360;
		VectorScale( dir, 16, tmpVec );
		VectorAdd( tmpVec, origin, newOrigin );
	} else {
		ex->leType = LE_EXPLOSION;
		VectorCopy( origin, newOrigin );

		// set axis with random rotate
		if ( !dir ) {
			AxisClear( ex->refEntity.axis );
		} else {
			ang = rand() % 360;
			VectorCopy( dir, ex->refEntity.axis[0] );
			RotateAroundDirection( ex->refEntity.axis, ang );
		}
	}

	ex->startTime = cg.time - offset;
	ex->endTime = ex->startTime + msec;

	// bias the time so all shader effects start correctly
	ex->refEntity.shaderTime = ex->startTime / 1000.0f;

	ex->refEntity.hModel = hModel;
	ex->refEntity.customShader = shader;

	// set origin
	VectorCopy( newOrigin, ex->refEntity.origin );
	VectorCopy( newOrigin, ex->refEntity.oldorigin );

	ex->color[0] = ex->color[1] = ex->color[2] = 1.0;

	return ex;
}


/*
=================
CG_Bleed

this is the spurt of blood when a character gets hit
=================
*/
void CG_Bleed( vec3_t origin, int entityNum ) {
	localEntity_t	*ex;

	if ( !cg_blood.integer ) {
		return;
	}

	ex = CG_AllocLocalEntity();
	ex->leType = LE_EXPLOSION;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 500;

	VectorCopy ( origin, ex->refEntity.origin);
	ex->refEntity.reType = RT_SPRITE;
	ex->refEntity.rotation = rand() % 360;
	ex->refEntity.radius = 24;

	ex->refEntity.customShader = cgs.media.bloodExplosionShader;

	// don't show player's own blood in view
	if ( entityNum == cg.snap->ps.clientNum ) {
		ex->refEntity.renderfx |= RF_THIRD_PERSON;
	}
}



/*
==================
CG_LaunchGib
==================
*/
// scale bloodtrails and bloodmarks by size of the gibs
// i just made these number up :|
float gibSizes[10] = {
	0.9,
	0.9,
	1.0,
	0.9,
	1.0,
	0.7,
	0.7,
	0.8,
	0.9,
	0.9
};

void CG_LaunchGib( vec3_t origin, vec3_t velocity, int gib, int quad ) {
	localEntity_t	*le;
	refEntity_t		*re;

	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	if( gib > 9 ) gib = 9;

	le->leType = LE_FRAGMENT;
	le->leFlags = LEF_TUMBLE;
	le->startTime = cg.time;
	le->endTime = le->startTime + 6000 + random() * 2000;

	VectorCopy( origin, re->origin );
	AxisCopy( axisDefault, re->axis );
	re->hModel = cgs.media.gibs[gib];

	le->lifeRate = gibSizes[gib];

	le->pos.trType = TR_GRAVITY;
	VectorCopy( origin, le->pos.trBase );
	VectorCopy( velocity, le->pos.trDelta );
	le->pos.trTime = cg.time;

	le->bounceFactor = 0.6f;

	le->leBounceSoundType = LEBS_BLOOD;
	le->leMarkType = LEMT_BLOOD;

	le->quad = quad;

	le->trailTime = cg.time + (random() * 150.0);
}

/*
==================
CG_LaunchExplode
==================
*/
void CG_LaunchExplode( vec3_t origin, vec3_t velocity, qhandle_t hModel ) {
	localEntity_t	*le;
	refEntity_t		*re;

	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	le->leType = LE_FRAGMENT;
	le->leFlags = LEF_TUMBLE;
	le->startTime = cg.time;
	le->endTime = le->startTime + 10000 + random() * 6000;

	VectorCopy( origin, re->origin );
	AxisCopy( axisDefault, re->axis );
	re->hModel = hModel;

	le->pos.trType = TR_GRAVITY;
	VectorCopy( origin, le->pos.trBase );
	VectorCopy( velocity, le->pos.trDelta );
	le->pos.trTime = cg.time;

	le->bounceFactor = 0.1f;

	le->leBounceSoundType = LEBS_BRASS;
	le->leMarkType = LEMT_NONE;
}

#define	EXP_VELOCITY	100
#define	EXP_JUMP		150
/*
===================
CG_GibPlayer

Generated a bunch of gibs launching out from the bodies location
===================
*/
void CG_BigExplode( vec3_t playerOrigin ) {
	vec3_t	origin, velocity;

	if ( !cg_blood.integer ) {
		return;
	}

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY;
	velocity[1] = crandom()*EXP_VELOCITY;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY;
	velocity[1] = crandom()*EXP_VELOCITY;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY*1.5;
	velocity[1] = crandom()*EXP_VELOCITY*1.5;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY*2.0;
	velocity[1] = crandom()*EXP_VELOCITY*2.0;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*EXP_VELOCITY*2.5;
	velocity[1] = crandom()*EXP_VELOCITY*2.5;
	velocity[2] = EXP_JUMP + crandom()*EXP_VELOCITY;
	CG_LaunchExplode( origin, velocity, cgs.media.smoke2 );
}

/*
===================
CG_GibPlayer

Generated a bunch of gibs launching out from the bodies location
===================
*/
#define	GIB_VELOCITY	250
#define	GIB_JUMP		250
void CG_GibPlayer( vec3_t playerOrigin, vec3_t dir, int quad ) {
	vec3_t	origin, velocity;
	int i;

	if ( !cg_blood.integer ) {
		return;
	}

	if ( quad == 1 || quad == 2 ) {
		VectorScale( dir, 400, dir );
	} else {
		VectorScale( dir, 250, dir );
	}

	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	VectorAdd( dir, velocity, velocity );
	if ( rand() & 1 ) {
		CG_LaunchGib( origin, velocity, 0, quad );
	} else {
		CG_LaunchGib( origin, velocity, 1, quad );
	}

	// allow gibs to be turned off for speed
	if ( !cg_gibs.integer ) {
		return;
	}

	for ( i = 2; i < 10; i++ ) {
//	VectorCopy( playerOrigin, origin );
		velocity[0] = crandom()*GIB_VELOCITY;
		velocity[1] = crandom()*GIB_VELOCITY;
		velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
		VectorAdd( dir, velocity, velocity );
		CG_LaunchGib( origin, velocity, i, quad);
	}
/*
//	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	VectorAdd( dir, velocity, velocity );
	CG_LaunchGib( origin, velocity, cgs.media.gibArm, quad );

//	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	VectorAdd( dir, velocity, velocity );
	CG_LaunchGib( origin, velocity, cgs.media.gibChest, quad );

//	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	VectorAdd( dir, velocity, velocity );
	CG_LaunchGib( origin, velocity, cgs.media.gibFist, quad );

//	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	VectorAdd( dir, velocity, velocity );
	CG_LaunchGib( origin, velocity, cgs.media.gibFoot, quad );

//	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	VectorAdd( dir, velocity, velocity );
	CG_LaunchGib( origin, velocity, cgs.media.gibForearm, quad );

//	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	VectorAdd( dir, velocity, velocity );
	CG_LaunchGib( origin, velocity, cgs.media.gibIntestine, quad );

//	VectorCopy( playerOrigin, origin );
	velocity[0] = crandom()*GIB_VELOCITY;
	velocity[1] = crandom()*GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom()*GIB_VELOCITY;
	VectorAdd( dir, velocity, velocity );
	CG_LaunchGib( origin, velocity, cgs.media.gibLeg, quad );
	*/
}

/*
===================
CG_GibPlayerHead

create a gib with the players head
===================
*/
void CG_GibPlayerHead( centity_t *cent, vec3_t dir ) {
	vec3_t	origin;
	localEntity_t	*le;
	refEntity_t	*re;
	clientInfo_t	*ci;

	ci = &cgs.clientinfo[ cent->currentState.clientNum ];

	VectorScale( dir, 200, dir );

	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	le->leType = LE_FRAGMENT;
	le->leFlags = LEF_TUMBLE | LEF_HEAD;	// only do y-axis rotation
	le->startTime = cg.time;
	le->endTime =  le->startTime + 6000 + random() * 2000;

	VectorCopy( cent->lerpOrigin, re->origin );
	re->origin[2] += 24;
	re->hModel = ci->headModel;
	re->customSkin = ci->headSkin;

	le->lifeRate = 0.9;

	le->pos.trType = TR_GRAVITY;
	VectorCopy( re->origin, le->pos.trBase );
	VectorCopy( dir, le->pos.trDelta );
	le->pos.trTime = cg.time;
	le->pos.trDelta[2] += 100;

	le->bounceFactor = 0.5;
	le->leBounceSoundType = LEBS_BLOOD;
	le->leMarkType = LEMT_BLOOD;
}

/*
=================
CG_QuadBoom

uhuh, BOOM!
=================
*/
void CG_QuadBoom( vec3_t position, vec3_t dir, int team, int clientNum ) {
	localEntity_t	*le;
	refEntity_t		*re;
	vec3_t	angles;

	CG_MissileHitWall( WP_RAILGUN, clientNum, position, dir, 1 );
	trap_S_StartSound( position, ENTITYNUM_WORLD, CHAN_AUTO, cgs.media.quadSound );

	le = CG_AllocLocalEntity();

	re = &le->refEntity;

//	if( cgs.valkyrMode ) {
//		le->leType = LE_FADE_SCALE_MODEL_TRIPPY;
//		re->customShader = cgs.media.trippyQuad;
//	} else {
		le->leType = LE_FADE_SCALE_MODEL;
		if ( team == TEAM_RED ) re->customShader = cgs.media.quadboomRed;
			else re->customShader = cgs.media.quadboomBlue;
//	}

	le->startTime = cg.time;
	le->endTime = cg.time + 700;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	re->hModel = cgs.media.quadboom;

	position[2] -= 24;
	VectorCopy( position, re->origin );
	VectorCopy( position, re->oldorigin );
	VectorSet( le->color, 1, 1, 1 );
	VectorSet( angles, 0, 0, 0 );
	AnglesToAxis( angles, re->axis );
	VectorScale( re->axis[0], 8, re->axis[0] );
	VectorScale( re->axis[1], 8, re->axis[1] );
	VectorScale( re->axis[2], 8, re->axis[2] );
	re->nonNormalizedAxes = qtrue;

	le = CG_AllocLocalEntity();
	le->startTime = cg.time;
	le->endTime = cg.time + 1000;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	re = &le->refEntity;
	re->hModel = cgs.media.quadboom;

//	if( cgs.valkyrMode ) {
//		le->leType = LE_FADE_SCALE_MODEL_TRIPPY;
//		re->customShader = cgs.media.trippyQuad;
//	} else {
		le->leType = LE_FADE_SCALE_MODEL;
		if ( team == TEAM_RED ) re->customShader = cgs.media.quadboomRed;
			else re->customShader = cgs.media.quadboomBlue;
//	}

	position[2] += 12;
	VectorCopy( position, re->origin );
	VectorCopy( position, re->oldorigin );
	VectorSet( le->color, 1, 1, 1 );

	VectorSet( angles, 0, 0, 0 );
	AnglesToAxis( angles, re->axis );
	VectorScale( re->axis[0], 6, re->axis[0] );
	VectorScale( re->axis[1], 6, re->axis[1] );
	VectorScale( re->axis[2], 6, re->axis[2] );
	re->nonNormalizedAxes = qtrue;
}
