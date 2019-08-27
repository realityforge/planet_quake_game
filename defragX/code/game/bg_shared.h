// shared vars relevant for handling ruleset/gametype/modes...


#define CS_GAMETYPE             26      // below used for holding ruleset and gametype stuff
#define CS_RULESET              27
#define CS_DFMODE               28
#define CS_FCMODE               29
#define CS_REVERSEMAP           30      // [0, 1] in defrag mode can play the map backwards

extern int    pm_doublejump;
extern int    pm_rampjump;
extern int    pm_stairjump;
extern int    pm_cpmstep;
extern int    pm_stairfix;

extern float  pm_knockback_z;
extern int    pm_rocketspeed;
extern int    pm_grenadereload;
extern int    pm_nodoors;
extern int    pm_noteles;
extern int    pm_jumppad;

extern float  pm_stopspeed;
extern float  pm_duckScale;
extern float  pm_swimScale;
extern float  pm_wadeScale;

extern float  pm_accelerate;
extern float  pm_airaccelerate;
extern float  pm_airstopaccelerate;
extern float  pm_strafeaccelerate;
extern float  pm_wateraccelerate;
extern float  pm_flyaccelerate;

extern float  pm_friction;
extern float  pm_waterfriction;
extern float  pm_flightfriction;

extern float  pm_aircontrol;
extern float  pm_wishspeed;

extern int    g_weaponraisetime;
extern int    g_weapondroptime;

extern int    pm_minrespawntime;

extern int    pm_reversemap;        // if set to 1 swap start/stop timer in defrag mode
