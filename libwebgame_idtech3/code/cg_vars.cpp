#include "cg_vars.h"

cvar_t *imgui_cg_runlerpframe        = NULL;
cvar_t *imgui_cg_drawactiveframe     = NULL;
cvar_t *imgui_ladder                 = NULL;
cvar_t *imgui_airmove                = NULL;
cvar_t *imgui_pmovesingle            = NULL;
cvar_t *imgui_rendercommands         = NULL;
cvar_t *test_x                       = NULL;
cvar_t *test_y                       = NULL;
cvar_t *test_z                       = NULL;
cvar_t *cg_explosion_lights          = NULL;

void cg_vars_init() {
	imgui_cg_runlerpframe    = Cvar_Get("imgui_cg_runlerpframe"   , "0", 0);
	imgui_cg_drawactiveframe = Cvar_Get("imgui_cg_drawactiveframe", "0", 0);
	imgui_ladder             = Cvar_Get("imgui_ladder"            , "0", 0);
	imgui_airmove            = Cvar_Get("imgui_airmove"           , "0", 0);
	imgui_pmovesingle        = Cvar_Get("imgui_pmovesingle"       , "0", 0);
	imgui_rendercommands     = Cvar_Get("imgui_rendercommands"    , "0", 0);
	test_x                   = Cvar_Get("test_x"                  , "0", 0);
	test_y                   = Cvar_Get("test_y"                  , "0", 0);
	test_z                   = Cvar_Get("test_z"                  , "0", 0);
	cg_explosion_lights      = Cvar_Get("cg_explosion_lights"     , "1", 0);
}
