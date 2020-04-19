#include "cg_local.h"

void mp44_precache(weaponInfo_t *weapon) {

	
	weaponInfo_t *weaponInfo = &cg_weapons[weapon->id]; // doesnt work, why?

	MAKERGB( weaponInfo->flashDlightColor, 1, 1, 0 );

	FxHandleInit(&weapon->shotsounds);
	FxHandleAddSound(&weapon->shotsounds, "sound/gun_rifle_shot_02.wav");
	FxHandleAddSound(&weapon->shotsounds, "sound/gun_rifle_shot_03.wav");

	//weaponInfo->ejectBrassFunc = CG_MachineGunEjectBrass;
	cgs.media.bulletExplosionShader = trap_R_RegisterShader( "bulletExplosion" );

	weapon->flashModel = trap_R_RegisterModel( "kungmodels/muzzleflash.kung1" );
}


void mp44_activate(weaponInfo_t *weapon, centity_t *cent) {
	trap_S_StartSound( NULL, cent->currentState.number, CHAN_WEAPON, FxHandleGetID(&weapon->shotsounds) );
}