#include "cg_local.h"

void kar98_precache(weaponInfo_t *weapon) {
	
	FxHandleInit(&weapon->shotsounds);
	FxHandleAddSound(&weapon->shotsounds, "sound/gun_rifle_shot_02.wav");
	FxHandleAddSound(&weapon->shotsounds, "sound/gun_rifle_shot_03.wav");

	
	weapon->flashModel = trap_R_RegisterModel( "kungmodels/muzzleflash.kung1" );
	
}

void kar98_activate(weaponInfo_t *weapon, centity_t *cent) {
	trap_S_StartSound( NULL, cent->currentState.number, CHAN_WEAPON, FxHandleGetID(&weapon->shotsounds) );
}