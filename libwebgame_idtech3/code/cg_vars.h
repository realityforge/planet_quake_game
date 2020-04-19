#ifndef CG_VARS_H
#define CG_VARS_H

#include <include_ccall.h>
#include <client.h>

EXTERNC cvar_t *imgui_cg_runlerpframe;
EXTERNC cvar_t *imgui_cg_drawactiveframe;
EXTERNC cvar_t *imgui_ladder;
EXTERNC cvar_t *imgui_airmove;
EXTERNC cvar_t *imgui_pmovesingle;
EXTERNC cvar_t *imgui_rendercommands;

EXTERNC cvar_t *test_x;
EXTERNC cvar_t *test_y;
EXTERNC cvar_t *test_z;

EXTERNC cvar_t *cg_explosion_lights;

CCALL void cg_vars_init();

#endif