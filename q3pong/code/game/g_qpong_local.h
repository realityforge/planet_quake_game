#undef GAMEVERSION
#define	GAMEVERSION	"qpong arena"

int		G_SkinIndex( char *name );

extern	vmCvar_t	g_gametype;

extern	vmCvar_t	g_startPoints;
extern	vmCvar_t	g_balls;
extern	vmCvar_t	g_playerDmgScale;
extern	vmCvar_t	g_announcer;
extern	vmCvar_t	g_grapple;
extern	vmCvar_t	g_ballScale;

extern	vmCvar_t	g_weapon;
extern	vmCvar_t	g_speedy;
extern	vmCvar_t	g_aqua;

void Q3P_Use_StopSign(gentity_t *player);
void Q3P_Use_UTurn(gentity_t *player);
void Q3P_Use_Spring(gentity_t *player);
