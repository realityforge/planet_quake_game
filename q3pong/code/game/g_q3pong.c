#include "g_local.h"
#include "g_q3pong.h"

levelData_t q3p_levelData;

void LogExit( const char *string );
int G_ParseInfos( char *buf, int max, char *infos[] );

/*
#define S_COLOR_BLACK	"^0"
#define S_COLOR_RED		"^1"
#define S_COLOR_GREEN	"^2"
#define S_COLOR_YELLOW	"^3"
#define S_COLOR_BLUE	"^4"
#define S_COLOR_CYAN	"^5"
#define S_COLOR_MAGENTA	"^6"
#define S_COLOR_WHITE	"^7"
*/

enum AnnouncerSounds {
   ANNOUNCER_WELCOME,
   ANNOUNCER_BLUE_SCORE,
   ANNOUNCER_BLUE_SCORE_SELF,
   ANNOUNCER_RED_SCORE,
   ANNOUNCER_RED_SCORE_SELF,
   ANNOUNCER_BLUE_WINS,
   ANNOUNCER_RED_WINS,
   ANNOUNCER_GIBBED, // killed, and gibbed
   ANNOUNCER_KILLED, // killed, but not gibbed
   ANNOUNCER_NUM_TYPES
};

#define NUM_KILLED_SOUNDS 11
#define NUM_GIBBED_SOUNDS 7
#define NUM_GENERIC_GOAL_SOUNDS 1
#define NUM_BLUE_GOAL_SOUNDS 4
#define NUM_RED_GOAL_SOUNDS 4
#define NUM_OTHER_GOAL_SOUNDS 3
#define NUM_OTHER_GOAL_RED_SOUNDS 1
#define NUM_OTHER_GOAL_BLUE_SOUNDS 1

#define ANNOUNCER_PATH "sound/announcer/"

/*
   the higher g_announcer is, the more "splat" announcements you get
   set it to 0 to disable all sounds
   1 enables all sounds except the death ones
   2 enables all sounds and plays death ones 25% of the time
   3 enables all sounds and plays death ones 50% of the time
   4 enables all sounds and plays death ones 75% of the time
   5 enables all sounds and plays a death announcement everytime someone gets killed by the ball
*/

void Q3P_Announce(int sound){
   char filename[256] = {0};
   int n;

   if (!g_announcer.value){
      return;
   }

   if (sound == ANNOUNCER_RED_WINS){
      strcpy(filename, ANNOUNCER_PATH "RedWins.wav");
   }else if (sound == ANNOUNCER_BLUE_WINS){
      strcpy(filename, ANNOUNCER_PATH "BlueWins.wav");
   }else{
      // don't play the sound if a team has no points
      // this stops problems with the end game announcer overlapping other announcer sounds
      if (!q3p_levelData.bluePoints || !q3p_levelData.redPoints){
         return;
      }
   }

   if (sound == ANNOUNCER_WELCOME){
      strcpy(filename, "sound/announcer/QpongIntro.wav");
   }
   else if ((sound == ANNOUNCER_BLUE_SCORE) || (sound == ANNOUNCER_RED_SCORE)){
      // decide if we want to play a generic score sound, or a team-specific one
      n = ceil(random() * (NUM_GENERIC_GOAL_SOUNDS + NUM_BLUE_GOAL_SOUNDS + NUM_RED_GOAL_SOUNDS));

      if (n <= NUM_GENERIC_GOAL_SOUNDS){
         n = ceil(random() * NUM_GENERIC_GOAL_SOUNDS);
         Com_sprintf(filename, sizeof(filename), ANNOUNCER_PATH "GenericGoal%02i.wav", n);
      }else{
         if (sound == ANNOUNCER_BLUE_SCORE){
            n = ceil(random() * NUM_BLUE_GOAL_SOUNDS);
            Com_sprintf(filename, sizeof(filename), ANNOUNCER_PATH "Bluegoal%02i.wav", n);
         }else if (sound == ANNOUNCER_RED_SCORE){
            n = ceil(random() * NUM_RED_GOAL_SOUNDS);
            Com_sprintf(filename, sizeof(filename), ANNOUNCER_PATH "Redgoal%02i.wav", n);
         }
      }
   }
   else if ((sound == ANNOUNCER_BLUE_SCORE_SELF) || (sound == ANNOUNCER_RED_SCORE_SELF)){
      // decide if we want to play a generic self-score sound, or a team-specific one
      n = ceil(random() * (NUM_OTHER_GOAL_SOUNDS + NUM_OTHER_GOAL_BLUE_SOUNDS + NUM_OTHER_GOAL_RED_SOUNDS));

      if (n <= NUM_OTHER_GOAL_SOUNDS){
         n = ceil(random() * NUM_OTHER_GOAL_SOUNDS);
         Com_sprintf(filename, sizeof(filename), ANNOUNCER_PATH "OtherGoal%02i.wav", n);
      }else{
         if (sound == ANNOUNCER_BLUE_SCORE_SELF){
            n = ceil(random() * NUM_OTHER_GOAL_BLUE_SOUNDS);
            Com_sprintf(filename, sizeof(filename), ANNOUNCER_PATH "OtherGoalB%02i.wav", n);
         }else if (sound == ANNOUNCER_RED_SCORE_SELF){
            n = ceil(random() * NUM_OTHER_GOAL_RED_SOUNDS);
            Com_sprintf(filename, sizeof(filename), ANNOUNCER_PATH "OtherGoalR%02i.wav", n);
         }
      }
   }
   else if (sound == ANNOUNCER_KILLED){
      // this first random decides if we're even going to play a sound
      n = ceil(random() * 4);
      if (n < g_announcer.value){
         // this random decides what sound to play
         n = ceil(random() * NUM_KILLED_SOUNDS);
         Com_sprintf(filename, sizeof(filename), ANNOUNCER_PATH "Killed%02i.wav", n);
      }
   }
   else if (sound == ANNOUNCER_GIBBED){
      // this first random decides if we're even going to play a sound
      n = ceil(random() * 4);
      if (n < g_announcer.value){
         // this random decides what sound to play
         n = ceil(random() * NUM_GIBBED_SOUNDS);
         Com_sprintf(filename, sizeof(filename), ANNOUNCER_PATH "Gibbed%02i.wav", n);
      }
   }

	if (filename[0]){
      gentity_t *te;
		te = G_TempEntity( vec3_origin, EV_ANNOUNCER_SOUND );
		te->s.eventParm = G_SoundIndex(filename);
		te->r.svFlags |= SVF_BROADCAST;
	}
}

void Q3P_CheckExit(void){
  if (g_gametype.integer == GT_REDROVER){
     // rr has a special set of rules
     int redPlayers, bluePlayers;
     int i;
     gentity_t *ent;
     redPlayers = 0;
     bluePlayers = 0;
     for ( i=0 ; i<MAX_CLIENTS; i++ ) {
        ent = &g_entities[i];

        if (ent->client->sess.sessionTeam == TEAM_RED){
           redPlayers++;
        }else if (ent->client->sess.sessionTeam == TEAM_BLUE){
           bluePlayers++;
        }

        if (g_fraglimit.integer && ent->client->ps.persistant[PERS_SCORE] >= g_fraglimit.integer){
           LogExit("%s Hit the Frag Limit!\n");
        }
     }
     if ( (!redPlayers || !bluePlayers) && ((redPlayers + bluePlayers) > 1) ){
        //int players[MAX_CLIENTS];
        int maxPlayers;
        int fromTeam;
        int toTeam;
        if (!redPlayers){
           trap_SendServerCommand(-1, "print \"^1Red^3 has no more players!  Restarting round.\n\"");
           maxPlayers = bluePlayers;
           fromTeam = TEAM_BLUE;
           toTeam = TEAM_RED;
        }else if (!bluePlayers){
           trap_SendServerCommand(-1, "print \"^5Blue^3 has no more players!  Restarting round.\n\"");
           maxPlayers = redPlayers;
           fromTeam = TEAM_RED;
           toTeam = TEAM_BLUE;
        }

        for ( i=0 ; i<(int)(maxPlayers/2); i++ ) {
           ent = &g_entities[rand() % MAX_CLIENTS];
           if (ent->client->sess.sessionTeam == fromTeam){
              ent->client->sess.sessionTeam = toTeam;
           }else{
              while(1){
                 ent++;
                 if (ent > (g_entities + MAX_CLIENTS) ){
                    ent = g_entities;
                 }
                 if (ent->client->sess.sessionTeam == fromTeam){
                    ent->client->sess.sessionTeam = toTeam;
                 }
              } // while
           } // if
        } // for

        for ( i=0 ; i<MAX_CLIENTS; i++ ) {
           ent = &g_entities[i];

           // force them to respawn
           ent->client->ps.pm_type = PM_DEAD;
           ent->health = 0;
           ent->s.weapon = WP_NONE;
           ent->s.powerups = 0;
           ent->r.contents = CONTENTS_CORPSE;

           ent->client->respawnTime = level.time + rand() % 1000;
        }
     }
  }else{
     //only check rules if at least one team's score is 0
     if(q3p_levelData.bluePoints > 0 && q3p_levelData.redPoints > 0)
        return;

     if(q3p_levelData.bluePoints <= 0){
        trap_SendServerCommand(-1, "print \"^5Blue^3 team eliminated, ^1Red^3 wins!^7\n\"");

        Q3P_Announce(ANNOUNCER_RED_WINS);

        LogExit("Blue Eliminated, Red Wins.\n");
        return;
     }

     if(q3p_levelData.redPoints <= 0){
        trap_SendServerCommand(-1, "print \"^1Red^3 team eliminated, ^5Blue^3 wins!^7\n\"");

		  Q3P_Announce(ANNOUNCER_BLUE_WINS);

		  LogExit("Red Eliminated, Blue Wins.\n");
		  return;
	  }
  }
}

void Q3P_RegisterTeamChange(int team, int oldteam){
	switch(team){
		case TEAM_RED:
			q3p_levelData.redPlayers++;
			break;
		case TEAM_BLUE:
			q3p_levelData.bluePlayers++;
			break;
	}

	switch(oldteam){
		case TEAM_RED:
			q3p_levelData.redPlayers--;
			break;
		case TEAM_BLUE:
			q3p_levelData.bluePlayers--;
			break;
	}

	//sanity check
	if(q3p_levelData.redPlayers < 0 || q3p_levelData.bluePlayers < 0 /* q3p_levelData.greenPlayers < 0 || q3p_levelData.yellowPlayers < 0*/)
		G_Error("Q3P_RegisterTeamChange: team counter less than 0!\n");

	return;
}

void Q3P_BloodyBall(gentity_t *ball){
	int n;
	char *blood = Info_ValueForKey(q3p_levelData.ballInfo[ball->watertype], "bloodSkin");
	if (*blood){
		ball->s.otherEntityNum = G_SkinIndex(blood);
	}
   n = ceil(random() * 2);
   if (n == 1){
      Q3P_Announce(ANNOUNCER_KILLED);
   }else{
      Q3P_Announce(ANNOUNCER_GIBBED);
   }
}

void Q3P_SpawnBall(void){
	gentity_t *spawnpoints[MAX_GENTITIES];
	int spawn_c;
	gentity_t *ball, *spawnpoint;
//	vec3_t dir;

	static int lastBalln;
	int balln;

	if (q3p_levelData.balls >= g_balls.value)
		return;

	if (q3p_levelData.spawnBallTime > level.time)
		return;

	q3p_levelData.spawnBallTime = level.time + 1000;

	spawnpoint = NULL;
	spawn_c = 0;

	while ( spawnpoint = G_Find(spawnpoint, FOFS(classname), "info_gameball_start")){
		spawnpoints[spawn_c] = spawnpoint;
		spawn_c ++;
	}
	if (!spawn_c)
		G_Error("Q3P_SpawnBall: ball start point not found!\n");

	spawnpoint = spawnpoints[ rand() % spawn_c ];

	if (trap_PointContents(spawnpoint->s.origin, spawnpoint->s.number) & CONTENTS_SOLID)
		return;

	q3p_levelData.balls ++;

	if (q3p_levelData.numBallInfos > 1){
		balln = lastBalln;
		while (balln == lastBalln)
			balln = floor(random() * q3p_levelData.numBallInfos);
		lastBalln = balln;
	}else{
		balln = 0;
	}

	//General Init
	ball = G_Spawn();
	ball->classname = "ball";
	ball->target = NULL;

	VectorSet(ball->s.angles, 0, 1, 0);
    VectorSet(ball->s.angles2, 0, 0, 1);
	VectorCopy(spawnpoint->s.origin, ball->s.origin);

	ball->s.eType = ET_BALL;
	ball->r.svFlags = SVF_USE_CURRENT_ORIGIN;
	ball->s.eFlags = EF_BOUNCE | EF_BOUNCE_HALF;
	ball->damage = 1000;
	ball->takedamage = qtrue;

	ball->flags &= ~FL_NO_KNOCKBACK;
	ball->touch = Ball_Touch;
	ball->methodOfDeath = MOD_BALL;
	ball->clipmask = CONTENTS_BOTCLIP | CONTENTS_MONSTERCLIP | CONTENTS_SOLID;
	ball->r.contents = CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_BODY;

	ball->watertype = balln;
	{
		float radius;
        radius = atof(Info_ValueForKey(q3p_levelData.ballInfo[ball->watertype], "radius"));

		if (radius == 0)
			radius = 32;

		radius *= g_ballScale.value;

		VectorSet (ball->r.mins, -radius, -radius, -radius);

		VectorSet (ball->r.maxs, radius, radius, radius);

		ball->s.torsoAnim = radius;
		ball->s.legsAnim = -radius;
	}

	ball->s.generic1 = (int)(atof(Info_ValueForKey(q3p_levelData.ballInfo[ball->watertype], "scale")) * 100);
	if (ball->s.generic1 == 0){
		ball->s.generic1 = 100;
	}
	ball->s.generic1 *= g_ballScale.value;

	ball->speed = atof(Info_ValueForKey(q3p_levelData.ballInfo[ball->watertype], "bounceScale"));
	if (ball->speed == 0){
		ball->speed = 1.60f;
	}

	ball->model = Info_ValueForKey(q3p_levelData.ballInfo[ball->watertype], "model");
	ball->s.modelindex = G_ModelIndex( ball->model );

	ball->s.otherEntityNum = G_SkinIndex(Info_ValueForKey(q3p_levelData.ballInfo[ball->watertype], "skin"));

	ball->s.pos.trType = TR_BALL;
	ball->s.pos.trTime = level.time;
	VectorCopy(ball->s.origin, ball->s.pos.trBase);
	VectorCopy(ball->s.origin, ball->r.currentOrigin);

	/*
   this will set the ball moving in the direction specified by the mapper
   it's commented out now because it doesn't seem to work right

	G_SetMovedir(spawnpoint->s.angles, ball->s.pos.trDelta);
	VectorMA(tv(random() * 40 - 20, random() * 40 - 20, random() * 40 - 20), spawnpoint->speed, ball->s.pos.trDelta, ball->s.pos.trDelta);
	*/
	VectorCopy(tv(random() * 40 - 20, random() * 40 - 20, random() * 40 - 20), ball->s.pos.trDelta);

   ball->count = atoi(Info_ValueForKey(q3p_levelData.ballInfo[ball->watertype], "mass"));

   // this isn't used right now, but eventually it can be used to remove "dead" balls (balls that got trapped in a corner
   // and no one can find them, or balls that went out of the map)  after "wait" time, it should delete the ball and respawn
   // a new one.  "wait" is reset every time someone shoots it
	ball->wait = level.time + 30000; // 30 second timeout period

	trap_LinkEntity(ball);

   G_Printf("Spawning ball %d, %s - %s", balln, ball->model, Info_ValueForKey(q3p_levelData.ballInfo[ball->watertype], "skin"));
}

/* Q3P_Init
Initializes Q3Pong, spawns the ball, resets the leveldata
Called on initial map load only
*/
void Q3P_Init(void){
	memset(&q3p_levelData, 0, sizeof(levelData_t));
	q3p_levelData.bluePoints = g_startPoints.value;
	q3p_levelData.redPoints = g_startPoints.value;
	q3p_levelData.greenPoints = g_startPoints.value;
	q3p_levelData.yellowPoints = g_startPoints.value;

	q3p_levelData.spawnBallTime = level.time;

	q3p_levelData.firstTime = 1;

   G_Printf("Parsing Balls\n");
   G_Printf("----------\n");
	{
		char buffer[10001];
		int ballFileSize;
		fileHandle_t ballf;

		ballFileSize = trap_FS_FOpenFile("balls.cfg", &ballf, FS_READ);
		if (ballFileSize > 10000)
			ballFileSize = 10000;
		trap_FS_Read(buffer, ballFileSize, ballf);
		buffer[ballFileSize] = 0;
		trap_FS_FCloseFile(ballf);

		q3p_levelData.numBallInfos = G_ParseInfos(buffer, MAX_BALL_INFOS, q3p_levelData.ballInfo);

		G_Printf("%i balls parsed\n", q3p_levelData.numBallInfos);
		{
         int useSmallBalls = trap_Cvar_VariableIntegerValue("g_useSmallBalls");
			int i;
         for (i=0; i<q3p_levelData.numBallInfos; i++) {
            float radius;

            radius = atof(Info_ValueForKey(q3p_levelData.ballInfo[i], "radius"));
            if (!useSmallBalls && radius < 32)
            {
               char *t = q3p_levelData.ballInfo[i];
               int j;

               G_Printf("Ball (%s : %s) is too small, tossing out\n", i, Info_ValueForKey(q3p_levelData.ballInfo[i], "model"), Info_ValueForKey(q3p_levelData.ballInfo[i], "skin"));

               for (j=i; j<q3p_levelData.numBallInfos-1; j++) {
                  q3p_levelData.ballInfo[j] = q3p_levelData.ballInfo[j+1];
               }
               q3p_levelData.ballInfo[q3p_levelData.numBallInfos-1] = t;
               q3p_levelData.numBallInfos --;
            }
         }
		}
      G_Printf("%i balls usable\n", q3p_levelData.numBallInfos);

      if (q3p_levelData.numBallInfos <= 0) {
         G_Error("No balls!\n");
      }
	}
   G_Printf("Balls:\n");

   {int i;
   for (i=0; i<q3p_levelData.numBallInfos; i++) {
      G_Printf("%d: %s - %s\n", i, Info_ValueForKey(q3p_levelData.ballInfo[i], "model"), Info_ValueForKey(q3p_levelData.ballInfo[i], "skin"));
   }
   }

   G_Printf("----------\n");

	return;
}

qboolean Q3P_CheckStartingConditions(void){
	int players;

	players = q3p_levelData.redPlayers + q3p_levelData.bluePlayers /* + q3p_levelData.greenPlayers + q3p_levelData.yellowPlayers*/;

	if(players > trap_Cvar_VariableIntegerValue("g_startPlayers"))
		return qtrue;
	else
		return qfalse;
}

void Q3P_Check(void){
	if (q3p_levelData.balls < g_balls.value && q3p_levelData.spawnBallTime < level.time && Q3P_CheckStartingConditions()){
		if (q3p_levelData.balls < g_balls.value){
			Q3P_SpawnBall();
		}
	}else if (q3p_levelData.firstTime && (q3p_levelData.spawnBallTime + 1000) < level.time && Q3P_CheckStartingConditions()){
		// x seconds after spawning the last ball for the first time

		Q3P_Announce(ANNOUNCER_WELCOME);

		q3p_levelData.firstTime = 0;
	}
}

#define GOALIE_GIVE_TIME 5000 // time in msecs between gives
#define GOALIE_HURT_TIME 1000 // time in msecs between hurts

void Give_Item(gentity_t *ent, char *name){
   gitem_t *it;
   gentity_t *it_ent;
   trace_t trace;

   it = BG_FindItem (name);
	if (!it) {
		return;
	}

	it_ent = G_Spawn();
	VectorCopy( ent->r.currentOrigin, it_ent->s.origin );
	it_ent->classname = it->classname;
	G_SpawnItem (it_ent, it);
	FinishSpawningItem(it_ent );
	memset( &trace, 0, sizeof( trace ) );
	Touch_Item (it_ent, ent, &trace);
	if (it_ent->inuse) {
		G_FreeEntity( it_ent );
	}
}

void Goal_Touch(gentity_t *self, gentity_t *other, trace_t *trace){
	gentity_t *te;

	int n;

	if (other->client){
		if (other->client->sess.sessionTeam == TEAM_RED){
			if (self->spawnflags & 1){
            if (other->client->goalieTime){
               // this person is already a goalie
               other->client->goalieTime = level.time;
            }else{
               // this person is not yet goalie
				   int i;
				   gentity_t *ent;
				   for ( i=0 ; i<MAX_CLIENTS; i++ ) {
					   ent = &g_entities[i];

					   if ( ent->client->sess.sessionTeam != other->client->sess.sessionTeam ) {
						   continue;
					   }

					   if ( ent == other ){
						   continue;
					   }

					   if ( ent->client->goalieTime ){
						   // already a goalie on the team
						   return;
					   }
				   }
               other->client->ps.powerups[PW_BATTLESUIT] = INT_MAX;
				   other->client->goalieTime = level.time;
            }
         }else{
            // standing in the wrong goal is bad
            if (!(level.time % GOALIE_HURT_TIME)){
               G_Damage(other, NULL, NULL, vec3_origin, vec3_origin, 10, DAMAGE_NO_KNOCKBACK, MOD_UNKNOWN);
            }
         }
		}else if (other->client->sess.sessionTeam == TEAM_BLUE){
			if (self->spawnflags & 2){
            if (other->client->goalieTime){
               // this person is already a goalie
               other->client->goalieTime = level.time;
            }else{
				   int i;
				   gentity_t *ent;
				   for ( i=0 ; i<MAX_CLIENTS; i++ ) {
					   ent = &g_entities[i];

					   if ( ent->client->sess.sessionTeam != other->client->sess.sessionTeam ) {
						   continue;
					   }

					   if ( ent == other ){
						   continue;
					   }

					   if ( ent->client->goalieTime ){
						   // already a goalie on the team
						   return;
					   }
				   }
               other->client->ps.powerups[PW_BATTLESUIT] = INT_MAX;
				   other->client->goalieTime = level.time;
            }
         }else{
            // standing in the wrong goal is bad
            if (!(level.time % GOALIE_HURT_TIME)){
               G_Damage(other, NULL, NULL, vec3_origin, vec3_origin, 10, DAMAGE_NO_KNOCKBACK, MOD_UNKNOWN);
            }
			}
		}
		return;
	}

	if (other->s.eType != ET_BALL)
		return;

	n = ceil(random() * 4);

	// Spawn Flags for trigger_goal
	// 1 = red team's goal
	// 2 = blue team's goal
	// 4 = yellow team's goal
	// 8 = green team's goal
	if (self->spawnflags & 1){
      if (g_gametype.integer == GT_REDROVER) {
         int i;
         gentity_t *ent;
         for ( i=0 ; i<MAX_CLIENTS; i++ ) {
            ent = &g_entities[i];

            if ( ent->client->sess.sessionTeam != TEAM_BLUE ) {
               continue;
            }
            AddScore(ent, other->r.currentOrigin, 5);
         }
      }else{
         q3p_levelData.redPoints--;
      }
      if (other->enemy && other->enemy->client){
			if (other->enemy->client->sess.sessionTeam != TEAM_RED){
				Q3P_Announce(ANNOUNCER_BLUE_SCORE);

				trap_SendServerCommand(-1, va("print \"%s Scored on ^1Red^7!\n\"", other->enemy->client->pers.netname));
				AddScore(other->enemy, other->r.currentOrigin, 10);
			}else{
				Q3P_Announce(ANNOUNCER_RED_SCORE_SELF);

				trap_SendServerCommand(-1, va("print \"%s Scored on his ^1own team^7!\n\"", other->enemy->client->pers.netname));
				AddScore(other->enemy, other->r.currentOrigin, -10);
			}
		}else{
			trap_SendServerCommand(-1, va("print \"The ball magically went into the ^1Red^7 goal...\n\"", other->enemy->client->pers.netname));
		}
	}else if (self->spawnflags & 2){
      if (g_gametype.integer == GT_REDROVER) {
         int i;
         gentity_t *ent;
         for ( i=0 ; i<MAX_CLIENTS; i++ ) {
            ent = &g_entities[i];

            if ( ent->client->sess.sessionTeam != TEAM_RED ) {
               continue;
            }
            AddScore(ent, other->r.currentOrigin, 5);
         }
      }else{
         q3p_levelData.bluePoints--;
      }
      if (other->enemy && other->enemy->client){
			if (other->enemy->client->sess.sessionTeam != TEAM_BLUE){

            // don't play the sound if this is the last point
            // this stops problems with the end game and score announcers overlapping
            if (q3p_levelData.bluePoints){
               Q3P_Announce(ANNOUNCER_RED_SCORE);
            }

				trap_SendServerCommand(-1, va("print \"%s Scored on ^5Blue^7!\n\"", other->enemy->client->pers.netname));
				AddScore(other->enemy, other->r.currentOrigin, 10);
			}else{
				Q3P_Announce(ANNOUNCER_BLUE_SCORE_SELF);

				trap_SendServerCommand(-1, va("print \"%s Scored on his ^5own team^7!\n\"", other->enemy->client->pers.netname));
				AddScore(other->enemy, other->r.currentOrigin, -10);
			}
		}else{
			trap_SendServerCommand(-1, va("print \"The ball magically went into the ^5Blue^7 goal...\n\"", other->enemy->client->pers.netname));
		}
/*	else if (self->spawnflags & 4)
		q3p_levelData.yellowPoints--;
	else if (self->spawnflags & 8)
		q3p_levelData.greenPoints--;*/
	}else{
		//G_Error("Goal_Touch: trigger_goal has wrong or invalid spawnflags\n");
	}

	te = G_TempEntity( other->r.currentOrigin, EV_MISSILE_MISS );
	te->s.weapon = WP_ROCKET_LAUNCHER;
	te->s.eventParm = 10;
	te->r.svFlags |= SVF_BROADCAST;

	G_FreeEntity(other);

	// splash damage
	if ( other->splashDamage ) {
		G_RadiusDamage( other->r.currentOrigin, other->parent, other->splashDamage, other->splashRadius, NULL, other->splashMethodOfDeath );
	}

	CalculateRanks();

	q3p_levelData.balls --;
	Q3P_SpawnBall();
}

void Ball_Touch(gentity_t *self, gentity_t *other, trace_t *trace){
	vec3_t velocity, delta;
	float speed, hitTime;
	int damage;

	if(!other->takedamage)
		return;

	hitTime = level.previousTime + ( ( level.time - level.previousTime ) * trace->fraction );

	BG_EvaluateTrajectoryDelta( &self->s.pos, hitTime, velocity );
	speed = VectorLength(velocity);

	damage = (self->count * speed) / 500;
	damage *= g_ballScale.value;

	if(other->client){
		if (damage > 5 && other->client->ps.groundEntityNum != self->s.number){
			damage -= 5;

			VectorSubtract(self->r.currentOrigin, other->client->ps.origin, delta);
			if (DotProduct(delta, velocity) >= 0)
				return;

			if (self->enemy && self->enemy->client)
				G_Damage (other, self, self->enemy, velocity, trace->endpos, damage, 0, MOD_BALL);
			else
				G_Damage (other, self, NULL, velocity, trace->endpos, damage, 0, MOD_BALL);
		}else{
			vec3_t kvel;
			int mass;

			VectorSubtract(self->r.currentOrigin, other->r.currentOrigin, delta);
			damage = VectorLength(other->client->ps.velocity);

			mass = self->count;

			VectorNormalize(delta);
			VectorScale (delta, (g_knockback.value * 5.0f) / mass, kvel);

			VectorAdd (self->s.pos.trDelta, kvel, self->s.pos.trDelta);
			SnapVector(self->s.pos.trDelta);

			self->enemy = other;
			self->health = 25;
		}
	}else if( other->s.eType == ET_BALL){
		int s;

		vec3_t selfVi, selfVf;
		vec3_t otherVi, otherVf;
		vec3_t temp1, temp2;
		float totalMass;

		hitTime = level.previousTime + (( level.time - level.previousTime ) * (trace->fraction/2.0f));

		VectorSubtract( other->s.pos.trBase, self->s.pos.trBase, selfVi);
		VectorNormalize( selfVi );
		VectorScale( selfVi, VectorLength(self->s.pos.trDelta), selfVi );

		VectorSubtract( self->s.pos.trBase, other->s.pos.trBase, otherVi);
		VectorNormalize( otherVi );
		VectorScale( otherVi, VectorLength(other->s.pos.trDelta), otherVi );

		/*
		VectorCopy(self->s.pos.trDelta, selfVi);
		VectorCopy(other->s.pos.trDelta, otherVi);
		*/
      BG_EvaluateTrajectoryDelta(&other->s.pos, hitTime, otherVi);

		totalMass = self->count + other->count;

		VectorScale(selfVi, (self->count - other->count)/totalMass, temp1);
		VectorScale(otherVi, (2 * other->count)/totalMass, temp2);
		VectorAdd(temp1, temp2, selfVf);
		VectorScale(selfVf, 0.6, selfVf);

		VectorScale(otherVi, (other->count - self->count)/totalMass, temp1);
		VectorScale(selfVi, (2 * self->count)/totalMass, temp2);
		VectorAdd(temp1, temp2, otherVf);
		VectorScale(otherVf, 0.6, otherVf);

		VectorCopy(selfVf, self->s.pos.trDelta);
		SnapVector(self->s.pos.trDelta);

		VectorCopy(otherVf, other->s.pos.trDelta);
		SnapVector(other->s.pos.trDelta);

		if (VectorLength(self->s.pos.trDelta) > 50){
			s = G_SoundIndex(Info_ValueForKey(q3p_levelData.ballInfo[self->watertype], "collideSound"));
			if (s){
				G_Sound(self, CHAN_AUTO, s);
			}else{
				G_AddEvent( self, EV_GRENADE_BOUNCE, 0 );
			}
		}

		if (VectorLength(other->s.pos.trDelta) > 50){
			s = G_SoundIndex(Info_ValueForKey(q3p_levelData.ballInfo[other->watertype], "collideSound"));
			if (s){
				G_Sound(other, CHAN_AUTO, s);
			}else{
				G_AddEvent( self, EV_GRENADE_BOUNCE, 0 );
			}
		}

	}else{
		G_Damage (other, self, NULL, velocity, trace->endpos, damage, 0, MOD_BALL);
	}
}

/*
==================
ClipVelocity

Slide off of the impacting object
returns the blocked flags (1 = floor, 2 = step / wall)
==================
*/
#define	STOP_EPSILON	0.1

int ClipVelocity (vec3_t in, vec3_t normal, vec3_t out, float overbounce){
	float	backoff;
	float	change;
	int		i, blocked;

	blocked = 0;
	if (normal[2] > 0)
		blocked |= 1;		// floor
	if (!normal[2])
		blocked |= 2;		// step

	backoff = DotProduct (in, normal) * overbounce;

	for (i=0 ; i<3 ; i++){
		change = normal[i]*backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
			out[i] = 0;
	}

	return blocked;
}

#define FLY_MASK  MASK_SHOT

/*
============
SV_FlyMove

The basic solid body movement clip that slides along multiple planes
Returns the clipflags if the velocity was modified (hit something solid)
1 = floor
2 = wall / step
4 = dead stop
============
*/
#define	MAX_CLIP_PLANES	5
int O3P_FlyMove (gentity_t *ent, float overbounce)
{
	gentity_t		*hit;
	int			bumpcount, numbumps;
	vec3_t		dir;
	float		d;
	int			numplanes;
	vec3_t		planes[MAX_CLIP_PLANES];
	vec3_t		primal_velocity, original_velocity, new_velocity;
	int			i, j;
	trace_t		trace;
	vec3_t		end;
	float		   time_left;	
	int			blocked;
	
	numbumps = 4;
	
	blocked = 0;
	VectorCopy (ent->s.pos.trDelta, original_velocity);
	VectorCopy (ent->s.pos.trDelta, primal_velocity);
	numplanes = 0;
	
	time_left = 0.1f;

	ent->s.groundEntityNum = -1;
	for (bumpcount=0 ; bumpcount<numbumps ; bumpcount++)
	{
		for (i=0 ; i<3 ; i++)
			end[i] = ent->s.origin[i] + time_left * ent->s.pos.trDelta[i];

		trap_Trace (&trace, ent->s.origin, ent->r.mins, ent->r.maxs, end, ent->s.number, FLY_MASK);

		if (trace.allsolid)
		{	// entity is trapped in another solid
			VectorCopy (vec3_origin, ent->s.pos.trDelta);
			return 3;
		}

		if (trace.fraction > 0)
		{	// actually covered some distance
			VectorCopy (trace.endpos, ent->s.origin);
			VectorCopy (ent->s.pos.trDelta, original_velocity);
			numplanes = 0;
		}

		if (trace.fraction == 1.0)
			 break;		// moved the entire distance

		hit = &g_entities[trace.entityNum];

		if (trace.plane.normal[2] > 0.7)
		{
			blocked |= 1;		// floor
			if ( hit->r.contents & CONTENTS_SOLID)
			{
				ent->s.groundEntityNum = hit->s.number;
				//ent->groundentity_linkcount = hit->linkcount;
			}
		}
		if (!trace.plane.normal[2])
		{
			blocked |= 2;		// step
		}

//
// run the impact function
//
		Ball_Touch (ent, hit, &trace);
		if (!ent->inuse)
			break;		// removed by the impact function

		
		time_left -= time_left * trace.fraction;
		
	// cliped to another plane
		if (numplanes >= MAX_CLIP_PLANES)
		{	// this shouldn't really happen
			VectorCopy (vec3_origin, ent->s.pos.trDelta);
			return 3;
		}

		VectorCopy (trace.plane.normal, planes[numplanes]);
		numplanes++;

//
// modify original_velocity so it parallels all of the clip planes
//
			for (i=0 ; i<numplanes ; i++) {
				ClipVelocity (original_velocity, planes[i], new_velocity, overbounce);

				for (j=0 ; j<numplanes ; j++)
					if (j != i)
					{
						if (DotProduct (new_velocity, planes[j]) < 0)
							break;	// not ok
					}	

				if (j == numplanes)
					break;
			}
		
			if (i != numplanes)
			{	// go along this plane
				VectorCopy (new_velocity, ent->s.pos.trDelta);
			}
			else
			{	// go along the crease
				if (numplanes != 2)
				{
	//				gi.dprintf ("clip velocity, numplanes == %i\n",numplanes);
					VectorCopy (vec3_origin, ent->s.pos.trDelta);
					return 7;
				}
				CrossProduct (planes[0], planes[1], dir);
				d = DotProduct (dir, ent->s.pos.trDelta);
				VectorScale (dir, d, ent->s.pos.trDelta);
			}

	}
	
	return blocked;
}

int InWater(vec3_t point, int entNum){
	return ( (trap_PointContents(point, entNum) & (CONTENTS_WATER | CONTENTS_LAVA | CONTENTS_SLIME)) );
}

void Q3P_AddGravity (gentity_t *ent){
   vec3_t   gravity = {0, 0, 0};
 
   if ((g_aqua.integer) || InWater(ent->s.origin, ent->s.number)){
		gravity[2] -= 0.05f * g_gravity.value * sin(level.time) * (( ent->count - 100.0f ) / 100.0f);
	}else{
		gravity[2] -= 0.05f * g_gravity.value;
	}

   if (ent->s.groundEntityNum != -1) {
      trace_t  trace;
      vec3_t   end;

      VectorAdd(ent->s.origin, gravity, end);

      trap_Trace (&trace, ent->s.origin, ent->r.mins, ent->r.maxs, end, ent->s.number, FLY_MASK);

      ClipVelocity(gravity, trace.plane.normal, gravity, 1.01f);
   }

   VectorAdd(ent->s.pos.trDelta, gravity, ent->s.pos.trDelta);

   return;
}

void Q3P_SetGroundEntity(gentity_t *ent, vec3_t origin){
	trace_t		tr;
	vec3_t		down;

	VectorCopy(origin, down);
	down[2] -= 2;

	// trace a line down
	trap_Trace( &tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, down, 
		ent->s.number, ent->clipmask );

	if (tr.fraction != 1.0){
		ent->s.groundEntityNum = tr.entityNum;
	}else{
		ent->s.groundEntityNum = -1;
	}
}

void Q3P_TouchTriggers( gentity_t *ent){
	int touch[MAX_GENTITIES];
	vec3_t mins, maxs;
	gentity_t *hit;
	trace_t trace;
	int num;

	int i;

	VectorAdd( ent->s.origin, ent->r.mins, mins );
	VectorAdd( ent->s.origin, ent->r.maxs, maxs );

	num = trap_EntitiesInBox( mins, maxs, touch, MAX_GENTITIES );

	for ( i=0 ; i<num ; i++ ) {
		hit = &g_entities[touch[i]];

		if ( !hit->touch && !ent->touch ) {
			continue;
		}
		if ( !( hit->r.contents & CONTENTS_TRIGGER ) ) {
			continue;
		}

		memset( &trace, 0, sizeof(trace) );

		hit->touch(hit, ent, &trace);
		ent->touch(ent, hit, &trace);
	}
}

void RotateBall(vec3_t dst, vec3_t axis[3], vec3_t point, float degrees)
{
	float	m[3][3];
	float	im[3][3];
	float	zrot[3][3];
	float	tmpmat[3][3];
	float	rot[3][3];
	int	i;
	float	rad;

	m[0][0] = axis[0][0];
	m[1][0] = axis[0][1];
	m[2][0] = axis[0][2];

	m[0][1] = axis[1][0];
	m[1][1] = axis[1][1];
	m[2][1] = axis[1][2];

	m[0][2] = axis[2][0];
	m[1][2] = axis[2][1];
	m[2][2] = axis[2][2];

   // create inverse matrix
	memcpy( im, m, sizeof( im ) );

	im[0][1] = m[1][0];
	im[0][2] = m[2][0];
	im[1][0] = m[0][1];
	im[1][2] = m[2][1];
	im[2][0] = m[0][2];
	im[2][1] = m[1][2];

   // create the z rotation matrix
	memset( zrot, 0, sizeof( zrot ) );
	zrot[0][0] = zrot[1][1] = zrot[2][2] = 1.0F;

	rad = DEG2RAD( degrees );
	zrot[0][0] = cos( rad );
	zrot[0][1] = sin( rad );
	zrot[1][0] = -sin( rad );
	zrot[1][1] = cos( rad );

	MatrixMultiply( m, zrot, tmpmat );
	MatrixMultiply( tmpmat, im, rot );

	for ( i = 0; i < 3; i++ ) {
		dst[i] = rot[i][0] * point[0] + rot[i][1] * point[1] + rot[i][2] * point[2];
	}
}

void G_RunBall(gentity_t *ent){
	float		friction;
	vec_t		oldspeed, speed;
   vec3_t   worldup = {0, 0, 1};

	if (ent->s.pos.trDelta[2] > 0)
		ent->s.groundEntityNum = -1;

// check for the groundentity going away
	if (ent->s.groundEntityNum != -1)
		if (!g_entities[ent->s.groundEntityNum].inuse)
			ent->s.groundEntityNum = -1;

   // slowly eat away the health, this will make it so that a ball that's
   // sitting there will eventually be able to take control of easily
   ent->health--;
	if (ent->health <= 1){
		ent->health = 1;
	}

	oldspeed = VectorLength(ent->s.pos.trDelta);

	if (ent->s.pos.trDelta[2] || ent->s.pos.trDelta[1] || ent->s.pos.trDelta[0]){
      if (InWater(ent->s.origin, ent->s.number)){
         // in water, friction is a constant 85%
         // ie, the ball will roll at 85% less than it did the frame before
         friction = 0.85f;
         ent->s.pos.trDelta[0] *= friction;
         ent->s.pos.trDelta[1] *= friction;
      }else{
         // on dry land, the ball will lose 7 'units' of speed per frame
         friction = VectorLength(ent->s.pos.trDelta);
         VectorNormalize(ent->s.pos.trDelta);
         friction -= 7.0f;
         if (friction < 0)
            friction = 0;
         VectorScale(ent->s.pos.trDelta, friction, ent->s.pos.trDelta);
      }
	}

   // move origin
	O3P_FlyMove (ent, ent->speed);

   Q3P_SetGroundEntity(ent, ent->s.origin);

   // add the gravity
   Q3P_AddGravity (ent);

   //G_Printf("Ball: %i, Velocity: (%f %f %f), Gravity: (%f %f %f)\n", ent->s.number, ent->s.pos.trDelta[0], ent->s.pos.trDelta[1], ent->s.pos.trDelta[2], gravity[0], gravity[1], gravity[2]);

	VectorCopy(ent->s.origin, ent->r.currentOrigin);
	VectorCopy(ent->s.origin, ent->s.pos.trBase);

	ent->s.pos.trTime = level.time;

	trap_LinkEntity(ent);

	Q3P_TouchTriggers (ent);

	speed = VectorLength(ent->s.pos.trDelta);

	if((speed - oldspeed) < -50){
		int s = G_SoundIndex(Info_ValueForKey(q3p_levelData.ballInfo[ent->watertype], "bounceSound"));
		if (s){
			G_Sound(ent, CHAN_AUTO, s);
		}else{
			G_AddEvent( ent, EV_GRENADE_BOUNCE, 0 );
		}
	}

   if (ent->s.pos.trDelta[0] != 0 && ent->s.pos.trDelta[1] != 0 && speed > 0) {
      vec3_t rotaxis[3];
      vec3_t fw, up;
      VectorCopy(ent->s.pos.trDelta, rotaxis[1]);
      CrossProduct(rotaxis[1], worldup , rotaxis[2]);
      CrossProduct(rotaxis[1], rotaxis[2], rotaxis[0]);

      VectorNormalize(rotaxis[0]);
      VectorNormalize(rotaxis[1]);
      VectorNormalize(rotaxis[2]);

      //G_Printf("(%.4f %.4f %.4f)(%.4f %.4f %.4f)(%.4f %.4f %.4f)\n", rotaxis[0][0], rotaxis[0][1], rotaxis[0][2], rotaxis[1][0], rotaxis[1][1], rotaxis[1][2], rotaxis[2][0], rotaxis[2][1], rotaxis[2][2]);

      // I am storing the 'forward' axis in the 'angles' vector and the 'up' axis in the 'angles2' vector
      RotateBall(fw, rotaxis, ent->s.angles, speed/10.0f);
      RotateBall(up, rotaxis, ent->s.angles2, speed/10.0f);

      VectorNormalize(fw);
      VectorNormalize(up);

      VectorCopy(fw, ent->s.angles);
      VectorCopy(up, ent->s.angles2);
   }
   //G_Printf("F(%f, %f, %f) U(%f, %f, %f)\n", ent->s.angles[0], ent->s.angles[1], ent->s.angles[2], ent->s.angles2[0], ent->s.angles2[1], ent->s.angles2[2]);
}

/*QUAKED info_team_start (1 0 0) (-16 -16 -16) (16 16 32)
Q3Pong Player Start Entity. spawnflags controls who spawns
1 = red 2 = blue 4 = yellow 8 = green
*/
void SP_info_team_start (gentity_t *ent){
}

void SP_trigger_goal( gentity_t *self ) {
	InitTrigger (self);

	self->touch = Goal_Touch;
	self->r.contents = CONTENTS_TRIGGER;
	trap_LinkEntity (self);
}

void SP_info_gameball_start (gentity_t *ent){
}

// called trigger_killball in qerad
void SP_trigger_ball_kill( gentity_t *self){
	InitTrigger (self);

	self->spawnflags = 16;
	self->touch = Goal_Touch;
	self->r.contents = CONTENTS_TRIGGER;
	trap_LinkEntity (self);
}


void Q3P_Use_StopSign(gentity_t *player)
{
   gentity_t *ent;

   trap_SendServerCommand(-1, va("print \"^7%s^3 Stopped the balls!\n\"", player->client->pers.netname));

   // find the balls, and shoot them up
   ent = NULL;
   while ( ent = G_Find (ent, FOFS(classname), "ball") ) {
      BG_EvaluateTrajectory( &ent->s.pos, level.time, ent->s.pos.trBase );
      VectorSet( ent->s.pos.trDelta, 0, 0, 0);
   }
}

void Q3P_Use_UTurn(gentity_t *player)
{
   gentity_t *ent;

   trap_SendServerCommand(-1, va("print \"^7%s^3 Turned the balls around!\n\"", player->client->pers.netname));

   // find the balls, and shoot them up
   ent = NULL;
   while ( ent = G_Find (ent, FOFS(classname), "ball") ) {
      BG_EvaluateTrajectory( &ent->s.pos, level.time, ent->s.pos.trBase );
      VectorScale( ent->s.pos.trDelta, -1, ent->s.pos.trDelta);
   }
}

void Q3P_Use_Spring(gentity_t *player)
{
   vec3_t up = {0, 0, 1};
   gentity_t *ent;

   trap_SendServerCommand(-1, va("print \"^7%s^3 Made the balls Bounce!\n\"", player->client->pers.netname));

   // find the balls, and shoot them up
   ent = NULL;
   while ( ent = G_Find (ent, FOFS(classname), "ball") ) {
      G_Damage(ent, NULL, player, up, NULL, 200, 0, MOD_SHOTGUN);
   }
}

