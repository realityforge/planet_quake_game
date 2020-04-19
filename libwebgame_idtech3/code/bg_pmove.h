#ifndef BG_PMOVE_H
#define BG_PMOVE_H

#include <q_shared.h>
#include "bg_public.h"
#include "bg_local.h"

#include <include_ccall.h>

CCALL void PM_Accelerate( vec3_t wishdir, float wishspeed, float accel );
CCALL float PM_CmdScale( usercmd_t *cmd );
CCALL void PM_Friction();
CCALL void PM_SetMovementDir();
CCALL void PM_ClipVelocity( vec3_t in, vec3_t normal, vec3_t out, float overbounce );
CCALL void PM_UpdateViewAngles( playerState_t *ps, const usercmd_t *cmd ); // if a full pmove isn't done on the client, you can just update the angles
CCALL void Pmove (pmove_t *pmove);

#endif