void InitTrigger( gentity_t *self );

void Q3P_Init(void);
void Q3P_Check(void);
void Q3P_SpawnBall(void);
void Q3P_RegisterTeamChange(int team, int oldteam);
qboolean Q3P_CheckStartingConditions(void);
void Q3P_CheckExit(void);
void Q3P_BloodyBall(gentity_t *ball);

void G_RunBall(gentity_t *ent);
void G_BounceMissile(gentity_t *ent, trace_t *trace);
void Ball_Touch(gentity_t *self, gentity_t *other, trace_t *trace);

#define MAX_BALL_INFOS 25

typedef struct {
	int bluePlayers;
	int redPlayers;
	int greenPlayers;
	int yellowPlayers;
	
	int bluePoints;
	int redPoints;
	int greenPoints;
	int yellowPoints;

	int firstTime;

	int balls;

	float spawnBallTime;

	int numBallInfos;
	char* ballInfo[MAX_BALL_INFOS];

} levelData_t;

extern levelData_t q3p_levelData;
