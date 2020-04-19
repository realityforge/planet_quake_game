#ifndef BG_SLIDEMOVE_H
#define BG_SLIDEMOVE_H

#include <q_shared.h>
#include "bg_public.h"
#include "bg_local.h"
#include <include_ccall.h>

CCALL qboolean PM_SlideMove( qboolean gravity );
CCALL void PM_StepSlideMove( qboolean gravity );

#endif