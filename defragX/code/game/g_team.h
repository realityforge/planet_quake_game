// Copyright (C) 1999-2000 Id Software, Inc.
//

int         OtherTeam( team_t team );
const char *TeamName( team_t team );
const char *OtherTeamName( team_t team );
const char *TeamColorString( team_t team );
void       Team_DroppedFlagThink(gentity_t *ent);
void       Team_FragBonuses(gentity_t *targ, gentity_t *inflictor, gentity_t *attacker);
void       Team_CheckHurtCarrier(gentity_t *targ, gentity_t *attacker);
void       Team_InitGame(void);
void       Team_ReturnFlag(team_t team);
void       Team_FreeEntity(gentity_t *ent);
gentity_t *SelectCTFSpawnPoint( gentity_t *ent, team_t team, int teamstate, vec3_t origin, vec3_t angles );
int        Pickup_Team( gentity_t *ent, gentity_t *other );
