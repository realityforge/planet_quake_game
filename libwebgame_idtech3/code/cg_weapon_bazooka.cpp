#include "cg_local.h"

void CG_RocketTrail( centity_t *ent, const weaponInfo_t *wi );

void bazooka_precache(weaponInfo_t *weaponInfo) {
	weaponInfo->missileModel = trap_R_RegisterModel( "kungmodels/sheep.kung1" );
	weaponInfo->missileSound = trap_S_RegisterSound( "sound/weapons/rocket/rockfly.wav", qfalse );
	weaponInfo->missileTrailFunc = CG_RocketTrail;
	weaponInfo->missileDlight = 200;
	weaponInfo->wiTrailTime = 2000;
	weaponInfo->trailRadius = 64;
		
	MAKERGB( weaponInfo->missileDlightColor, 1, 1, 1 );
	MAKERGB( weaponInfo->flashDlightColor, 1, 0.75f, 0 );
	
	FxHandleInit(&weaponInfo->shotsounds);
	FxHandleAddSound(&weaponInfo->shotsounds, "sound/sheep/sheep_2_baa_high_01.wav");
	FxHandleAddSound(&weaponInfo->shotsounds, "sound/sheep/sheep_2_baa_high_02.wav");
	FxHandleAddSound(&weaponInfo->shotsounds, "sound/sheep/sheep_2_baa_high_03.wav");
	FxHandleAddSound(&weaponInfo->shotsounds, "sound/sheep/sheep_2_baa_high_04.wav");
	FxHandleAddSound(&weaponInfo->shotsounds, "sound/sheep/sheep_2_baa_high_05.wav");
	FxHandleAddSound(&weaponInfo->shotsounds, "sound/sheep/sheep_2_baa_high_06.wav");

	cgs.media.rocketExplosionShader = trap_R_RegisterShader( "rocketExplosion" );
}


void bazooka_activate(weaponInfo_t *weapon, centity_t *cent) {
	trap_S_StartSound( NULL, cent->currentState.number, CHAN_WEAPON, FxHandleGetID(&weapon->shotsounds) );
}