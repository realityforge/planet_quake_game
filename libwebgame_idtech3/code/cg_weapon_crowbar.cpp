#include "cg_local.h"

void crowbar_precache(weaponInfo_t *weapon) {

	
}

void crowbar_activate(weaponInfo_t *weapon, centity_t *cent) {
	trap_S_StartSound( NULL, cent->currentState.number, CHAN_WEAPON, FxHandleGetID(&cgs.media.whoosh) );
}