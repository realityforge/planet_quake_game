#ifndef CG_HUDS_H
#define CG_HUDS_H

#include <client.h>
#include <include_ccall.h>
#include <server.h>
#include "cg_huds_structs.h"

// todo: allow more huds by special linked lists, so its fast to draw only the non-hidden, active huds e.g.
// or a clientvariable on server which says "huds dirty", so ONLY THEN the server checks if any hud needs to be resend.
// can then simply set that flag in the jl_ hud manipulation functions
#define MAX_HUDS 32

EXTERNC hudelem_t hudelems[128];

CCALL void CG_ParseHUD(msg_t *msg);
CCALL void CG_FreeHUD(msg_t *msg);
CCALL void CG_RenderHUDS();
CCALL void SV_WriteSnapshotHUDS(client_t *client, msg_t *msg);

#endif