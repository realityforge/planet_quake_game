// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_weapons.c -- events and effects dealing with weapons
#include "cg_local.h"

vec3_t grapple_origins[MAX_CLIENTS];

//***********************************************************************************
//*********************** START SCREWY RAILTRAIL BY FIRESTARTER**********************
//***********************************************************************************

#define M_TWOPI 			(M_PI*2.0)
#define M_PI_2				1.57079632679489661923
#define ROTATION_PER_SEG 	(M_PI*0.25)

void CG_RailTrail( clientInfo_t *ci, vec3_t start, vec3_t end ) {
	localEntity_t	*le;
	refEntity_t		*re;
	vec3_t			forward, right, up, tempVector;
	int				segments, i, j, trailTime;
	float			length, rotation, temp1, temp2;

	VectorSubtract( end, start, forward );
	length = VectorNormalize( forward );
	PerpendicularVector( right, forward );
	CrossProduct( forward, right, up );

	segments = length / 64.0;

	if( cgs.valkyrMode && cg_railTrailTime.integer < 800 ) trailTime = 800;
		else trailTime = cg_railTrailTime.integer;

	if( segments > 2 && cg_railscrew.integer ) {
		for( i = 0; i < cg_railscrewCount.integer; i++ ) {

			VectorCopy( start, tempVector );
			rotation = (M_TWOPI / cg_railscrewCount.integer) * i;

			for( j = 1; j < segments; j++ ) {
				le = CG_AllocLocalEntity_2();
				if( !le ) goto endRailScrew;
				re = &le->refEntity;

				if( cgs.valkyrMode ) le->leType = LE_TRIPPY_FADE_RGB;
					else  le->leType = LE_FADE_RGB;

				le->startTime = cg.time;
				le->endTime = cg.time + trailTime + ( i + j ) * 25;
				le->lifeRate = 1.0 / ( le->endTime - le->startTime );

				re->shaderTime = cg.time / 1000.0;
				re->reType = RT_RAIL_CORE;

				re->customShader = cgs.media.railCoreShader;
				VectorCopy( ci->color2, le->color );

				VectorCopy( tempVector, re->origin );
				VectorMA( start, ( 64.0 * j ), forward, tempVector);

				if ( j == ( segments - 1 ) ) {
					VectorCopy( end, re->oldorigin );
				} else {
					temp1 = cos( rotation ) * 8;
					temp2 = sin( rotation ) * 8;

					tempVector[0] += right[0] * temp1 + up[0] * temp2;
					tempVector[1] += right[1] * temp1 + up[1] * temp2;
					tempVector[2] += right[2] * temp1 + up[2] * temp2;

					rotation += ROTATION_PER_SEG;

					VectorCopy( tempVector, re->oldorigin );
				}

				le->color[3] = 1.0f;

				AxisClear( re->axis );
			}
		}
	}

endRailScrew:;

	//
	// rings
	//
	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	le->leType = LE_FADE_RGB;
	le->startTime = cg.time;
	le->endTime = cg.time + cg_railTrailTime.value;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	re->shaderTime = cg.time / 1000.0f;
	re->reType = RT_RAIL_RINGS;
	re->customShader = cgs.media.railRingsShader;

	VectorCopy( start, re->origin );
	VectorCopy( end, re->oldorigin );

	le->color[0] = ci->color1[0];
	le->color[1] = ci->color1[1] * 0.75;
	le->color[2] = ci->color1[2] * 0.75;
	le->color[3] = 1.0f;

	AxisClear( re->axis );

	//
	// core
	//
	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	le->leType = LE_FADE_RGB;
	le->startTime = cg.time;
	le->endTime = cg.time + cg_railTrailTime.value;
	le->lifeRate = 1.0 / ( le->endTime - le->startTime );

	re->shaderTime = cg.time / 1000.0f;
	re->reType = RT_RAIL_CORE;
	re->customShader = cgs.media.railCoreShader;

	VectorCopy( start, re->origin );
	VectorCopy( end, re->oldorigin );

	le->color[0] = ci->color[0];
	le->color[1] = ci->color[1] * 0.75;
	le->color[2] = ci->color[2] * 0.75;
	le->color[3] = 1.0f;

	AxisClear( re->axis );
}

//***********************************************************************************
//*********************** END SCREWY RAILTRAIL BY FIRESTARTER************************
//***********************************************************************************

/*
==========================
CG_RocketTrail
==========================
*/
static void CG_RocketTrail( centity_t *ent, const weaponInfo_t *wi ) {
	int		step;
	vec3_t	origin, lastPos;
	int		t;
	int		startTime, contents;
	int		lastContents;
	entityState_t	*es;
	vec3_t	up;
	localEntity_t	*smoke;

	up[0] = 0;
	up[1] = 0;
	up[2] = 0;

	step = 50;

	es = &ent->currentState;
	startTime = ent->trailTime;
	t = step * ( (startTime + step) / step );

	BG_EvaluateTrajectory( &es->pos, cg.time, origin );
	contents = CG_PointContents( origin, -1 );

	// if object (e.g. grenade) is stationary, don't toss up smoke
	if ( es->pos.trType == TR_STATIONARY ) {
		ent->trailTime = cg.time;
		return;
	}

	BG_EvaluateTrajectory( &es->pos, ent->trailTime, lastPos );
	lastContents = CG_PointContents( lastPos, -1 );

	ent->trailTime = cg.time;

	if ( contents & ( CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) ) {
		if ( contents & lastContents & CONTENTS_WATER ) {
//			CG_BubbleTrail( lastPos, origin, 8 );
		}
		return;
	}

	for ( ; t <= ent->trailTime ; t += step ) {
		BG_EvaluateTrajectory( &es->pos, t, lastPos );

		smoke = CG_SmokePuff( lastPos, up,
					  wi->trailRadius,
					  1, 1, 1, 0.33f,
					  wi->wiTrailTime,
					  t,
					  0,
					  0,
					  cgs.media.smokePuffShader );

		if( smoke ) {
			// use the optimized local entity add
			smoke->leType = LE_SCALE_FADE;
		}
	}

}

/*
==========================
CG_GrappleTrail
==========================
*/
void CG_GrappleTrail( centity_t *ent, const weaponInfo_t *wi ) {
	vec3_t			color, angles;
	entityState_t	*es, *ees;
	centity_t		*enemy;
	vec3_t			*temp, temp2, temp3;
	refEntity_t		beam;
	clientInfo_t	*ci;

	es = &ent->currentState;

	memset( &beam, 0, sizeof( beam ) );

	if ( es->otherEntityNum >= 0 && es->otherEntityNum < cg.snap->numEntities ) {
		if ( es->otherEntityNum == cg.predictedPlayerState.clientNum ) {
			VectorCopy( cg.predictedPlayerState.origin, beam.oldorigin );
		} else {
			enemy = &cg_entities[ es->otherEntityNum ];
			ees = &enemy->currentState;
			BG_EvaluateTrajectory( &ees->pos, cg.time, beam.oldorigin );
		}
	} else VectorCopy( ent->lerpOrigin, beam.oldorigin );

	ent->trailTime = cg.time;

	temp = &grapple_origins[ es->clientNum ];
	VectorCopy( temp, beam.origin );
	beam.reType = RT_RAIL_CORE;
	beam.customShader = cgs.media.railCoreShader;

	AxisClear( beam.axis );

	ci = &cgs.clientinfo[ es->clientNum ];

	if( cgs.valkyrMode ) CG_GetTrippyColor( es->clientNum * 700, 3000, color );
	else {
		VectorCopy( ci->color, color );
	}

	beam.shaderRGBA[0] = color[0] * 255;
	beam.shaderRGBA[1] = color[1] * 192;
	beam.shaderRGBA[2] = color[2] * 192;
	beam.shaderRGBA[3] = 0xff;
	trap_R_AddRefEntityToScene( &beam );

	VectorCopy( beam.oldorigin, temp2 );

	memset (&beam, 0, sizeof(beam));
	beam.reType = RT_SPRITE;
	beam.radius = 10 + crandom();

	angles[YAW] = 0;
	angles[PITCH] = 0;
	angles[ROLL] = crandom() * 10;
	AnglesToAxis( angles, beam.axis );

	beam.customShader = cgs.media.grapple_flare;

	VectorSubtract( temp2, cg.refdef.vieworg, temp3 );
	VectorNormalize( temp3 );
	VectorScale( temp3, 16, temp3 );
	VectorSubtract( temp2, temp3, beam.origin );
	VectorCopy( beam.origin, beam.oldorigin);

	trap_R_AddRefEntityToScene( &beam );

	// add dynamic light
	trap_R_AddLightToScene( temp2, 50 + crandom()*2, color[0], color[1] * .5, color[2] );
}

/*
==========================
CG_GrenadeTrail
==========================
*/
static void CG_GrenadeTrail( centity_t *ent, const weaponInfo_t *wi ) {
	CG_RocketTrail( ent, wi );
}


/*
=================
CG_RegisterWeapon

The server says this item is used on this level
=================
*/
void CG_RegisterWeapon( int weaponNum ) {
	weaponInfo_t	*weaponInfo;
	gitem_t			*item, *ammo;
	char			path[MAX_QPATH];
	vec3_t			mins, maxs;
	int				i;

	weaponInfo = &cg_weapons[weaponNum];

	if ( weaponNum == 0 ) {
		return;
	}

	if ( weaponInfo->registered ) {
		return;
	}

	memset( weaponInfo, 0, sizeof( *weaponInfo ) );
	weaponInfo->registered = qtrue;

	for ( item = bg_itemlist + 1 ; item->classname ; item++ ) {
		if ( item->giType == IT_WEAPON && item->giTag == weaponNum ) {
			weaponInfo->item = item;
			break;
		}
	}
	if ( !item->classname ) {
		CG_Error( "Couldn't find weapon %i", weaponNum );
	}
	CG_RegisterItemVisuals( item - bg_itemlist );

	// load cmodel before model so filecache works
	weaponInfo->weaponModel = trap_R_RegisterModel( item->world_model[0] );

	// calc midpoint for rotation
	trap_R_ModelBounds( weaponInfo->weaponModel, mins, maxs );
	for ( i = 0 ; i < 3 ; i++ ) {
		weaponInfo->weaponMidpoint[i] = mins[i] + 0.5 * ( maxs[i] - mins[i] );
	}

	weaponInfo->weaponIcon = trap_R_RegisterShader( item->icon );
	weaponInfo->ammoIcon = trap_R_RegisterShader( item->icon );

	for ( ammo = bg_itemlist + 1 ; ammo->classname ; ammo++ ) {
		if ( ammo->giType == IT_AMMO && ammo->giTag == weaponNum ) {
			break;
		}
	}
	if ( ammo->classname && ammo->world_model[0] ) {
		weaponInfo->ammoModel = trap_R_RegisterModel( ammo->world_model[0] );
	}

	strcpy( path, item->world_model[0] );
	COM_StripExtension( path, path );
	strcat( path, "_flash.md3" );
	weaponInfo->flashModel = trap_R_RegisterModel( path );

	strcpy( path, item->world_model[0] );
	COM_StripExtension( path, path );
	strcat( path, "_barrel.md3" );
	weaponInfo->barrelModel = trap_R_RegisterModel( path );

	strcpy( path, item->world_model[0] );
	COM_StripExtension( path, path );
	strcat( path, "_hand.md3" );
	weaponInfo->handsModel = trap_R_RegisterModel( path );

	if ( !weaponInfo->handsModel ) {
		weaponInfo->handsModel = trap_R_RegisterModel( "models/weapons2/shotgun/shotgun_hand.md3" );
	}

	weaponInfo->loopFireSound = qfalse;

	switch ( weaponNum ) {
	case WP_GAUNTLET:
		MAKERGB( weaponInfo->flashDlightColor, 0.6f, 0.6f, 1.0f );
		weaponInfo->firingSound = trap_S_RegisterSound( "sound/weapons/melee/fstrun.wav", qfalse );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/melee/fstatck.wav", qfalse );
		break;

	case WP_LIGHTNING:
		MAKERGB( weaponInfo->flashDlightColor, 0.6f, 0.6f, 1.0f );
		weaponInfo->readySound = trap_S_RegisterSound( "sound/weapons/melee/fsthum.wav", qfalse );
		weaponInfo->firingSound = trap_S_RegisterSound( "sound/weapons/lightning/lg_hum.wav", qfalse );

		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/lightning/lg_fire.wav", qfalse );
		cgs.media.lightningShader = trap_R_RegisterShader( "lightningBolt");
		cgs.media.lightningExplosionModel = trap_R_RegisterModel( "models/weaphits/crackle.md3" );
		cgs.media.sfx_lghit1 = trap_S_RegisterSound( "sound/weapons/lightning/lg_hit.wav", qfalse );
		cgs.media.sfx_lghit2 = trap_S_RegisterSound( "sound/weapons/lightning/lg_hit2.wav", qfalse );
		cgs.media.sfx_lghit3 = trap_S_RegisterSound( "sound/weapons/lightning/lg_hit3.wav", qfalse );

		break;

	case WP_GRAPPLING_HOOK:
		weaponInfo->missileTrailFunc = CG_GrappleTrail;
		cgs.media.grapple_flare = trap_R_RegisterShader("models/mapobjects/gratelamp/gratelamp_flare");
		cgs.media.sfx_grapplehit = trap_S_RegisterSound( "corkscrew_sfx/hit.wav", qfalse );
		break;

#ifdef MISSIONPACK
	case WP_CHAINGUN:
		weaponInfo->firingSound = trap_S_RegisterSound( "sound/weapons/vulcan/wvulfire.wav", qfalse );
		weaponInfo->loopFireSound = qtrue;
		MAKERGB( weaponInfo->flashDlightColor, 1, 1, 0 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/vulcan/vulcanf1b.wav", qfalse );
		weaponInfo->flashSound[1] = trap_S_RegisterSound( "sound/weapons/vulcan/vulcanf2b.wav", qfalse );
		weaponInfo->flashSound[2] = trap_S_RegisterSound( "sound/weapons/vulcan/vulcanf3b.wav", qfalse );
		weaponInfo->flashSound[3] = trap_S_RegisterSound( "sound/weapons/vulcan/vulcanf4b.wav", qfalse );
		weaponInfo->ejectBrassFunc = CG_MachineGunEjectBrass;
		cgs.media.bulletExplosionShader = trap_R_RegisterShader( "bulletExplosion" );
		break;
#endif

	case WP_MACHINEGUN:
		MAKERGB( weaponInfo->flashDlightColor, 1, 1, 0 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/machinegun/machgf1b.wav", qfalse );
		weaponInfo->flashSound[1] = trap_S_RegisterSound( "sound/weapons/machinegun/machgf2b.wav", qfalse );
		weaponInfo->flashSound[2] = trap_S_RegisterSound( "sound/weapons/machinegun/machgf3b.wav", qfalse );
		weaponInfo->flashSound[3] = trap_S_RegisterSound( "sound/weapons/machinegun/machgf4b.wav", qfalse );
		cgs.media.bulletExplosionShader = trap_R_RegisterShader( "bulletExplosion" );
		break;

	case WP_SHOTGUN:
		MAKERGB( weaponInfo->flashDlightColor, 1, 1, 0 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/shotgun/sshotf1b.wav", qfalse );
		break;

	case WP_ROCKET_LAUNCHER:
		weaponInfo->missileModel = trap_R_RegisterModel( "models/ammo/rocket/rocket.md3" );
		weaponInfo->missileSound = trap_S_RegisterSound( "sound/weapons/rocket/rockfly.wav", qfalse );
		weaponInfo->missileTrailFunc = CG_RocketTrail;
		weaponInfo->missileDlight = 200;
		weaponInfo->wiTrailTime = 2000;
		weaponInfo->trailRadius = 64;

		MAKERGB( weaponInfo->missileDlightColor, 1, 0.75f, 0 );
		MAKERGB( weaponInfo->flashDlightColor, 1, 0.75f, 0 );

		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/rocket/rocklf1a.wav", qfalse );
		cgs.media.rocketExplosionShader = trap_R_RegisterShader( "rocketExplosion" );
		break;

#ifdef MISSIONPACK
	case WP_PROX_LAUNCHER:
		weaponInfo->missileModel = trap_R_RegisterModel( "models/weaphits/proxmine.md3" );
		weaponInfo->missileTrailFunc = CG_GrenadeTrail;
		weaponInfo->wiTrailTime = 700;
		weaponInfo->trailRadius = 32;
		MAKERGB( weaponInfo->flashDlightColor, 1, 0.70f, 0 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/proxmine/wstbfire.wav", qfalse );
		cgs.media.grenadeExplosionShader = trap_R_RegisterShader( "grenadeExplosion" );
		break;
#endif

	case WP_GRENADE_LAUNCHER:
		weaponInfo->missileModel = trap_R_RegisterModel( "models/ammo/grenade1.md3" );
		weaponInfo->missileTrailFunc = CG_GrenadeTrail;
		weaponInfo->wiTrailTime = 700;
		weaponInfo->trailRadius = 32;
		MAKERGB( weaponInfo->flashDlightColor, 1, 0.70f, 0 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/grenade/grenlf1a.wav", qfalse );
		cgs.media.grenadeExplosionShader = trap_R_RegisterShader( "grenadeExplosion" );
		break;

#ifdef MISSIONPACK
	case WP_NAILGUN:
		weaponInfo->missileTrailFunc = CG_NailTrail;
		weaponInfo->missileSound = trap_S_RegisterSound( "sound/weapons/nailgun/wnalflit.wav", qfalse );
		weaponInfo->trailRadius = 16;
		weaponInfo->wiTrailTime = 250;
		weaponInfo->missileModel = trap_R_RegisterModel( "models/weaphits/nail.md3" );
		MAKERGB( weaponInfo->flashDlightColor, 1, 0.75f, 0 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/nailgun/wnalfire.wav", qfalse );
		break;
#endif

	case WP_PLASMAGUN:
		weaponInfo->missileSound = trap_S_RegisterSound( "sound/weapons/plasma/lasfly.wav", qfalse );
		MAKERGB( weaponInfo->flashDlightColor, 0.6f, 0.6f, 1.0f );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/plasma/hyprbf1a.wav", qfalse );
		cgs.media.plasmaExplosionShader = trap_R_RegisterShader( "plasmaExplosion" );
		break;

	case WP_RAILGUN:
		weaponInfo->readySound = trap_S_RegisterSound( "sound/weapons/railgun/rg_hum.wav", qfalse );
		MAKERGB( weaponInfo->flashDlightColor, 1, 0.5f, 0 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "corkscrew_sfx/railgun.wav", qfalse );
		weaponInfo->flashSound[1] = trap_S_RegisterSound( "corkscrew_sfx/railgun2.wav", qfalse );
		cgs.media.railExplosionShader = trap_R_RegisterShader( "railExplosion" );
		cgs.media.plasmaExplosionShader = trap_R_RegisterShader( "plasmaExplosion" );
		cgs.media.railRingsShader = trap_R_RegisterShader( "railDisc" );
		cgs.media.railCoreShader = trap_R_RegisterShader( "railCore" );
		break;

	case WP_BFG:
		weaponInfo->readySound = trap_S_RegisterSound( "sound/weapons/bfg/bfg_hum.wav", qfalse );
		MAKERGB( weaponInfo->flashDlightColor, 1, 0.7f, 1 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/bfg/bfg_fire.wav", qfalse );
		cgs.media.bfgExplosionShader = trap_R_RegisterShader( "bfgExplosion" );
		weaponInfo->missileModel = trap_R_RegisterModel( "models/weaphits/bfg.md3" );
		weaponInfo->missileSound = trap_S_RegisterSound( "sound/weapons/rocket/rockfly.wav", qfalse );
		break;

	 default:
		MAKERGB( weaponInfo->flashDlightColor, 1, 1, 1 );
		weaponInfo->flashSound[0] = trap_S_RegisterSound( "sound/weapons/rocket/rocklf1a.wav", qfalse );
		break;
	}
}

/*
=================
CG_RegisterItemVisuals

The server says this item is used on this level
=================
*/
void CG_RegisterItemVisuals( int itemNum ) {
	itemInfo_t		*itemInfo;
	gitem_t			*item;

	itemInfo = &cg_items[ itemNum ];
	if ( itemInfo->registered ) {
		return;
	}

	item = &bg_itemlist[ itemNum ];

	memset( itemInfo, 0, sizeof( &itemInfo ) );
	itemInfo->registered = qtrue;

	itemInfo->models[0] = trap_R_RegisterModel( item->world_model[0] );

	itemInfo->icon = trap_R_RegisterShader( item->icon );

	if ( item->giType == IT_WEAPON ) {
		CG_RegisterWeapon( item->giTag );
	}

	//
	// powerups have an accompanying ring or sphere
	//
	if ( item->giType == IT_POWERUP || item->giType == IT_HEALTH ||
		item->giType == IT_ARMOR || item->giType == IT_HOLDABLE ) {
		if ( item->world_model[1] ) {
			itemInfo->models[1] = trap_R_RegisterModel( item->world_model[1] );
		}
	}
}


/*
========================================================================================

VIEW WEAPON

========================================================================================
*/

/*
=================
CG_MapTorsoToWeaponFrame

=================
*/
static int CG_MapTorsoToWeaponFrame( clientInfo_t *ci, int frame ) {

	// change weapon
	if ( frame >= ci->animations[TORSO_DROP].firstFrame
		&& frame < ci->animations[TORSO_DROP].firstFrame + 9 ) {
		return frame - ci->animations[TORSO_DROP].firstFrame + 6;
	}

	// stand attack
	if ( frame >= ci->animations[TORSO_ATTACK].firstFrame
		&& frame < ci->animations[TORSO_ATTACK].firstFrame + 6 ) {
		return 1 + frame - ci->animations[TORSO_ATTACK].firstFrame;
	}

	// stand attack 2
	if ( frame >= ci->animations[TORSO_ATTACK2].firstFrame
		&& frame < ci->animations[TORSO_ATTACK2].firstFrame + 6 ) {
		return 1 + frame - ci->animations[TORSO_ATTACK2].firstFrame;
	}

	return 0;
}


/*
==============
CG_CalculateWeaponPosition
==============
*/
static void CG_CalculateWeaponPosition( vec3_t origin, vec3_t angles ) {
	float	scale;
	int		delta;
	float	fracsin;

	VectorCopy( cg.refdef.vieworg, origin );
	VectorCopy( cg.refdefViewAngles, angles );

	// on odd legs, invert some angles
	if ( cg.bobcycle & 1 ) {
		scale = -cg.xyspeed;
	} else {
		scale = cg.xyspeed;
	}

	// gun angles from bobbing
	angles[ROLL] += scale * cg.bobfracsin * 0.005;
	angles[YAW] += scale * cg.bobfracsin * 0.01;
	angles[PITCH] += cg.xyspeed * cg.bobfracsin * 0.005;

	// drop the weapon when landing
	delta = cg.time - cg.landTime;
	if ( delta < LAND_DEFLECT_TIME ) {
		origin[2] += cg.landChange*0.25 * delta / LAND_DEFLECT_TIME;
	} else if ( delta < LAND_DEFLECT_TIME + LAND_RETURN_TIME ) {
		origin[2] += cg.landChange*0.25 *
			(LAND_DEFLECT_TIME + LAND_RETURN_TIME - delta) / LAND_RETURN_TIME;
	}

#if 0
	// drop the weapon when stair climbing
	delta = cg.time - cg.stepTime;
	if ( delta < STEP_TIME/2 ) {
		origin[2] -= cg.stepChange*0.25 * delta / (STEP_TIME/2);
	} else if ( delta < STEP_TIME ) {
		origin[2] -= cg.stepChange*0.25 * (STEP_TIME - delta) / (STEP_TIME/2);
	}
#endif

	// idle drift
	scale = cg.xyspeed + 40;
	fracsin = sin( cg.time * 0.001 );
	angles[ROLL] += scale * fracsin * 0.01;
	angles[YAW] += scale * fracsin * 0.01;
	angles[PITCH] += scale * fracsin * 0.01;
}

/*
========================
CG_AddWeaponWithPowerups
========================
*/
static void CG_AddWeaponWithPowerups( refEntity_t *gun, int powerups, int team ) {
	// add powerup effects
	if ( powerups & ( 1 << PW_INVIS ) ) {
		gun->customShader = cgs.media.invisShader;
		trap_R_AddRefEntityToScene( gun );
	} else {
		trap_R_AddRefEntityToScene( gun );

		if ( powerups & ( 1 << PW_BATTLESUIT ) ) {
			gun->customShader = cgs.media.battleWeaponShader;
			trap_R_AddRefEntityToScene( gun );
		}
		if ( powerups & ( 1 << PW_QUAD ) ) {
			if ( team == TEAM_RED ) gun->customShader = cgs.media.redQuadWeaponShader;
				else gun->customShader = cgs.media.quadWeaponShader;
			trap_R_AddRefEntityToScene( gun );
		}
	}
}


/*
=============
CG_AddPlayerWeapon

Used for both the view weapon (ps is valid) and the world modelother character models (ps is NULL)
The main player will have this called for BOTH cases, so effects like light and
sound should only be done on the world model case.
=============
*/
void CG_AddPlayerWeapon( refEntity_t *parent, playerState_t *ps, centity_t *cent, int team ) {
	refEntity_t		gun;
	refEntity_t		barrel;
	refEntity_t		flash;
	vec3_t			angles, color;
	weapon_t		weaponNum;
	weaponInfo_t	*weapon;
	centity_t		*nonPredictedCent;
	clientInfo_t	*ci;
	float f, g;
//	int	col;
	ci = &cgs.clientinfo[ cent->currentState.clientNum ];

	weaponNum = cent->currentState.weapon;

	CG_RegisterWeapon( weaponNum );
	weapon = &cg_weapons[weaponNum];

	// add the weapon
	memset( &gun, 0, sizeof( gun ) );
	VectorCopy( parent->lightingOrigin, gun.lightingOrigin );
	gun.shadowPlane = parent->shadowPlane;
	gun.renderfx = parent->renderfx;

	// set custom shading for railgun refire rate
	// the firerate is the same for everybody, it's in the ps for the pmove module
	// if we have no STAT_FIRERATE, then we are a spectator.... use the default
	if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR ) {
		f = 1500.0 * 100.0 / (float)ci->handicap;
	} else {
		f = 1500.0 * ( 1000.0 / cg.snap->ps.stats[STAT_FIRERATE] )  * 100.0 / (float)ci->handicap;
	}

	// make it a progressive increase, not linear, looks better due to gamma
	if( cgs.valkyrMode ) CG_GetTrippyColor( cent->currentState.clientNum * 700, 3000, color );
	else {
		VectorCopy( ci->color, color );
	}

	if ( cg.time - cent->muzzleFlashTime < f ) {
		g = (float)( cg.time - cent->muzzleFlashTime ) / f;

		f = g * g * 176 + 16;
		VectorScale( color, f, gun.shaderRGBA );
	} else {
		VectorScale( color, 192, gun.shaderRGBA );
	}


	gun.hModel = weapon->weaponModel;
	if (!gun.hModel) {
		return;
	}

	if ( !ps ) {
		// add weapon ready sound
		cent->pe.lightningFiring = qfalse;
		if ( ( cent->currentState.eFlags & EF_FIRING ) && weapon->firingSound ) {
			// lightning gun and guantlet make a different sound when fire is held down
			trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, weapon->firingSound );
			cent->pe.lightningFiring = qtrue;
		} else if ( weapon->readySound ) {
			trap_S_AddLoopingSound( cent->currentState.number, cent->lerpOrigin, vec3_origin, weapon->readySound );
		}
	}

	CG_PositionEntityOnTag( &gun, parent, parent->hModel, "tag_weapon");

	CG_AddWeaponWithPowerups( &gun, cent->currentState.powerups, ci->team );

	// make sure we aren't looking at cg.predictedPlayerEntity for LG
	nonPredictedCent = &cg_entities[cent->currentState.clientNum];

	// if the index of the nonPredictedCent is not the same as the clientNum
	// then this is a fake player (like on teh single player podiums), so
	// go ahead and use the cent
	if( ( nonPredictedCent - cg_entities ) != cent->currentState.clientNum ) {
		nonPredictedCent = cent;
	}

	memset( &flash, 0, sizeof( flash ) );
	VectorCopy( parent->lightingOrigin, flash.lightingOrigin );
	flash.shadowPlane = parent->shadowPlane;
	flash.renderfx = parent->renderfx;

	flash.hModel = weapon->flashModel;
	if (!flash.hModel) {
		return;
	}
	angles[YAW] = 0;
	angles[PITCH] = 0;
	angles[ROLL] = crandom() * 10;
	AnglesToAxis( angles, flash.axis );
	CG_PositionRotatedEntityOnTag( &flash, &gun, weapon->weaponModel, "tag_flash");

	// save info for grapple trail
	if ( ( cent->currentState.number != cg.predictedPlayerState.clientNum ||
				parent->renderfx == (RF_DEPTHHACK | RF_FIRST_PERSON | RF_MINLIGHT) ||
			cg.renderingThirdPerson ) && !( cent->currentState.eFlags & EF_DEAD ) ) {
		float *temp;
		temp = &grapple_origins[ cent->currentState.clientNum ][0];
		VectorCopy( flash.origin, temp );
	}

	// add the flash
	if ( ( weaponNum == WP_LIGHTNING || weaponNum == WP_GAUNTLET || weaponNum == WP_GRAPPLING_HOOK )
		&& ( nonPredictedCent->currentState.eFlags & EF_FIRING ) )
	{
		// continuous flash
	} else {
		// impulse flash
		if ( cg.time - cent->muzzleFlashTime > MUZZLE_FLASH_TIME && !cent->pe.railgunFlash ) {
			return;
		}
	}

	// colorize the railgun blast
	if ( weaponNum == WP_RAILGUN ) {
		flash.shaderRGBA[0] = 255 * ci->color[0];
		flash.shaderRGBA[1] = 192 * ci->color[1];
		flash.shaderRGBA[2] = 192 * ci->color[2];
	}

	trap_R_AddRefEntityToScene( &flash );

	if ( ps || cg.renderingThirdPerson || cent->currentState.number != cg.predictedPlayerState.clientNum ) {
		trap_R_AddLightToScene( flash.origin, 300 + (rand()&31), ci->color[0], ci->color[1] * 0.75, ci->color[2] * 0.75 );
	}
}

/*
==============
CG_AddViewWeapon

Add the weapon, and flash for the player's view
==============
*/
void CG_AddViewWeapon( playerState_t *ps ) {
	refEntity_t	hand;
	centity_t	*cent;
	clientInfo_t	*ci;
	float		fovOffset;
	vec3_t		angles;
	weaponInfo_t	*weapon;

	if( !ps ) return;

	if ( ps->persistant[PERS_TEAM] == TEAM_SPECTATOR ) {
		return;
	}

	if ( ps->pm_type == PM_INTERMISSION ) {
		return;
	}

	// no gun if in third person view
	if ( cg.renderingThirdPerson ) {
		return;
	}

	// allow the gun to be completely removed
	if ( !cg_drawGun.integer ) {
		vec3_t		origin;
		float			*temp;

		temp = &grapple_origins[ cg.predictedPlayerState.clientNum ][0];
		VectorCopy( cg.refdef.vieworg, origin );
		VectorMA( origin, -16, cg.refdef.viewaxis[2], origin );
		VectorCopy( origin, temp );
		return;
	}

	// don't draw if testing a gun model
	if ( cg.testGun ) {
		return;
	}

	// drop gun lower at higher fov
	if ( cg_fov.integer > 90 ) {
		fovOffset = -0.2 * ( cg_fov.integer - 90 );
	} else {
		fovOffset = 0;
	}

	if( !ps->weapon ) return;

	cent = &cg.predictedPlayerEntity;	// &cg_entities[cg.snap->ps.clientNum];
	CG_RegisterWeapon( ps->weapon );
	weapon = &cg_weapons[ ps->weapon ];

	memset (&hand, 0, sizeof(hand));

	// set up gun position
	CG_CalculateWeaponPosition( hand.origin, angles );

	VectorMA( hand.origin, cg_gun_x.value, cg.refdef.viewaxis[0], hand.origin );
	VectorMA( hand.origin, cg_gun_y.value, cg.refdef.viewaxis[1], hand.origin );
	VectorMA( hand.origin, (cg_gun_z.value+fovOffset), cg.refdef.viewaxis[2], hand.origin );

	AnglesToAxis( angles, hand.axis );

	// map torso animations to weapon animations
	if ( cg_gun_frame.integer ) {
		// development tool
		hand.frame = hand.oldframe = cg_gun_frame.integer;
		hand.backlerp = 0;
	} else {
		// get clientinfo for animation map
		ci = &cgs.clientinfo[ cent->currentState.clientNum ];
		hand.frame = CG_MapTorsoToWeaponFrame( ci, cent->pe.torso.frame );
		hand.oldframe = CG_MapTorsoToWeaponFrame( ci, cent->pe.torso.oldFrame );
		hand.backlerp = cent->pe.torso.backlerp;
	}

	hand.hModel = weapon->handsModel;
	hand.renderfx = RF_DEPTHHACK | RF_FIRST_PERSON | RF_MINLIGHT;

	// add everything onto the hand
	CG_AddPlayerWeapon( &hand, ps, &cg.predictedPlayerEntity, ps->persistant[PERS_TEAM] );
}

/*
==============================================================================

WEAPON SELECTION

==============================================================================
*/

// uhuh, WHAT weapon selection???

/*
===================================================================================================

WEAPON EVENTS

===================================================================================================
*/

/*
================
CG_FireWeapon

Caused by an EV_FIRE_WEAPON event
================
*/
void CG_FireWeapon( centity_t *cent ) {
	entityState_t *ent;
	int				c;
	weaponInfo_t	*weap;
	clientInfo_t	*ci;
	float f;

	if( !cent ) return;			// sanity check
	ent = &cent->currentState;
	if( !ent ) return;			// sanity check

	if ( ent->weapon == WP_NONE || (ent->eFlags & EF_DEAD) ) {	// spawn protection
		return;
	}
	if ( ent->weapon >= WP_NUM_WEAPONS ) {
		CG_Error( "CG_FireWeapon: ent->weapon >= WP_NUM_WEAPONS" );
		return;
	}
	weap = &cg_weapons[ ent->weapon ];

	// mark the entity as muzzle flashing, so when it is added it will
	// append the flash to the weapon model
	cent->muzzleFlashTime = cg.time;

	// lightning gun only does this this on initial press
	if ( ent->weapon == WP_LIGHTNING ) {
		if ( cent->pe.lightningFiring ) {
			return;
		}
	}

	// play quad sound if needed
	if ( cent->currentState.powerups & ( 1 << PW_QUAD ) ) {
		trap_S_StartSound (NULL, cent->currentState.number, CHAN_ITEM, cgs.media.quadSound );
	}

	ci = &cgs.clientinfo[ cent->currentState.clientNum ];

	f = 1500.0 * ( 1000.0 / cg.snap->ps.stats[STAT_FIRERATE] )  * 100.0 / (float)ci->handicap;

	// only play standard 1.5 second sound if reload is exactly 1.5 seconds
	if ( f == 1500.0 ) {
		trap_S_StartSound( NULL, ent->number, CHAN_WEAPON, weap->flashSound[0] );
	} else {
		trap_S_StartSound( NULL, ent->number, CHAN_WEAPON, weap->flashSound[1] );
	}
}


/*
=================
CG_MissileHitWall

Caused by an EV_MISSILE_MISS event, or directly by local bullet tracing
=================
*/

void CG_MissileHitWall( int weapon, int clientNum, vec3_t origin, vec3_t dir, impactSound_t soundType ) {
	qhandle_t		mod;
	qhandle_t		mark;
	qhandle_t		shader, particleShader;
	sfxHandle_t		sfx;
	float			radius;
	float			light;
	vec3_t			lightColor;
	localEntity_t	*le;
	int				r;
	qboolean		alphaFade;
	qboolean		isSprite;
	int				duration;

	mark = 0;
	radius = 32;
	sfx = 0;
	mod = 0;
	shader = 0;
	light = 0;
	lightColor[0] = 1;
	lightColor[1] = 1;
	lightColor[2] = 0;

	// set defaults
	isSprite = qfalse;
	duration = 600;

	switch ( weapon ) {
	case WP_RAILGUN:
		if ( cg_railgunExplosions.integer || soundType ) {
			mod = cgs.media.dishFlashModel;
			shader = cgs.media.rocketExplosionShader;
			sfx = cgs.media.sfx_rockexp;
			mark = cgs.media.burnMarkShader;
			radius = 64;
			light = 300;
			isSprite = qtrue;
			duration = 1000;
			break;
		} else {
			mod = cgs.media.ringFlashModel;
			shader = cgs.media.railExplosionShader;
			sfx = cgs.media.sfx_plasmaexp;
			mark = cgs.media.energyMarkShader;
			radius = 24;
			break;
		}
		break;
	case WP_PLASMAGUN:	// just a bounced railgun shot
		mod = cgs.media.ringFlashModel;
		shader = cgs.media.railExplosionShader;
		sfx = cgs.media.sfx_plasmaexp;
		mark = cgs.media.energyMarkShader;
		radius = 24;
		break;
	case WP_GRAPPLING_HOOK:
		sfx = cgs.media.sfx_grapplehit;
		mark = cgs.media.holeMarkShader;
		radius = 8;
		break;
	}

	if ( sfx ) {
		trap_S_StartSound( origin, ENTITYNUM_WORLD, CHAN_AUTO, sfx );
	}

	//
	// create the explosion
	//

	if ( weapon == WP_RAILGUN && ( cg_railgunExplosions.integer == 1 || soundType ) && shader == cgs.media.rocketExplosionShader ) {
		int i, partNum, colorIndex;
		VectorCopy( cgs.clientinfo[clientNum].color, lightColor );
		partNum = 0;

		if ( soundType ) {	// if quad damage :/
			if ( cgs.clientinfo[clientNum].team == TEAM_RED ) {
				shader = cgs.media.railgunShaders[3].explosionShader;
				mark = cgs.media.redQuadBurnMarkShader;
			} else {
				shader = cgs.media.railgunShaders[0].explosionShader;
				mark = cgs.media.quadBurnMarkShader;
			}
		} else {
			colorIndex = cgs.clientinfo[clientNum].c1 - 1;
			shader = cgs.media.railgunShaders[colorIndex].explosionShader;
			colorIndex = cgs.clientinfo[clientNum].c2 - 1;
			particleShader = cgs.media.railgunShaders[colorIndex].particleShader;
			partNum = cg_explosionParticles.integer + crandom() * 0.25 * cg_explosionParticles.integer;
		}

		if ( !soundType )
		for ( i = 0; i < partNum; i++ ) {
			localEntity_t	*le;
			refEntity_t		*re;
			vec3_t			tempvec;

			le = CG_AllocLocalEntity_2();
			if (!le) break;

			re = &le->refEntity;

			le->leType = LE_PARTICLE;
			le->startTime = cg.time;
			le->endTime = cg.time + 1000 + crandom() * 200;
			le->lifeRate = 1.0 / ( le->endTime - le->startTime );

			re->customShader = particleShader;

			le->pos.trType = TR_GRAVITY;
			VectorMA( origin, 20, dir, le->pos.trBase );

			le->pos.trBase[0] += crandom()*(20+(crandom()*4));
			le->pos.trBase[1] += crandom()*(20+(crandom()*4));
			le->pos.trBase[2] += crandom()*(20+(crandom()*4));

			VectorSubtract( le->pos.trBase, origin, tempvec );
			VectorNormalizeFast( tempvec );
			VectorScale( tempvec, 300, le->pos.trDelta );
			le->pos.trTime = cg.time;

			VectorCopy( le->pos.trBase, re->origin );
			VectorCopy( le->pos.trBase, re->oldorigin );

			re->reType = RT_SPRITE;
			re->radius = 3;
			re->shaderRGBA[3] = 255;
		}
	}

	if ( mod ) {
		le = CG_MakeExplosion( origin, dir,
							   mod,	shader,
							   duration, isSprite );
		le->light = light;
		if ( weapon == WP_RAILGUN ) {	// if quad damage :/
			if ( soundType ) le->light = 450;
			if ( soundType && cgs.clientinfo[clientNum].team == TEAM_FREE ) {			// force blue explosion if quad
				VectorCopy( colorBlue, le->lightColor );
			} else {
			VectorCopy( lightColor, le->lightColor );
			}
		}
	}

	//
	// impact mark
	//
	// quad fades alpha too
	alphaFade = (mark == cgs.media.energyMarkShader) || soundType;
	if ( weapon == WP_RAILGUN && alphaFade ) {
		float	*color;

		// colorize with client color
		color = cgs.clientinfo[clientNum].color;
		CG_ImpactMark( mark, origin, dir, random()*360, color[0], color[1], color[2], 1, alphaFade, radius, qfalse, qtrue );
	} else {
		CG_ImpactMark( mark, origin, dir, random()*360, 1,1,1,1, alphaFade, radius, qfalse, qtrue );
	}
}


/*
=================
CG_MissileHitPlayer
=================
*/
void CG_MissileHitPlayer( int weapon, vec3_t origin, vec3_t dir, int entityNum ) {
	CG_Bleed( origin, entityNum );

	// some weapons will make an explosion with the blood, while
	// others will just make the blood
	switch ( weapon ) {
	case WP_GRENADE_LAUNCHER:
		CG_MissileHitWall( weapon, 0, origin, dir, IMPACTSOUND_FLESH );
		break;
	case WP_ROCKET_LAUNCHER:
		CG_MissileHitWall( weapon, 0, origin, dir, IMPACTSOUND_FLESH );
		break;
	default:
		break;
	}
}

/*
============================================================================

SHOTGUN TRACING

============================================================================
*/

/*
============================================================================

BULLETS

============================================================================
*/

/*
======================
CG_CalcMuzzlePoint
======================
*/
static qboolean	CG_CalcMuzzlePoint( int entityNum, vec3_t muzzle ) {
	vec3_t		forward;
	centity_t	*cent;
	int			anim;

	if ( entityNum == cg.snap->ps.clientNum ) {
		VectorCopy( cg.snap->ps.origin, muzzle );
		muzzle[2] += cg.snap->ps.viewheight;
		AngleVectors( cg.snap->ps.viewangles, forward, NULL, NULL );
		VectorMA( muzzle, 14, forward, muzzle );
		return qtrue;
	}

	cent = &cg_entities[entityNum];
	if ( !cent->currentValid ) {
		return qfalse;
	}

	VectorCopy( cent->currentState.pos.trBase, muzzle );

	AngleVectors( cent->currentState.apos.trBase, forward, NULL, NULL );
	anim = cent->currentState.legsAnim & ~ANIM_TOGGLEBIT;
	if ( anim == LEGS_WALKCR || anim == LEGS_IDLECR ) {
		muzzle[2] += CROUCH_VIEWHEIGHT;
	} else {
		muzzle[2] += DEFAULT_VIEWHEIGHT;
	}

	VectorMA( muzzle, 14, forward, muzzle );

	return qtrue;

}
