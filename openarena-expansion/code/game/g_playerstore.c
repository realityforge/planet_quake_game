/*
===========================================================================
Copyright (C) 2008-2009 Poul Sander

This file is part of Open Arena source code.

Open Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Open Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Open Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "g_local.h"

#define MAX_PLAYERS_STORED 32

#define GUID_SIZE 32

typedef struct {
    char guid[GUID_SIZE+1]; //The guid is 32 chars long
    int age; //int that grows each time a new player is stored. The lowest number is always replaced. Reset to 0 then retrieved.
    int	persistant[MAX_PERSISTANT]; //This is the only information we need to save
    int timePlayed;
    int	accuracy[WP_NUM_WEAPONS][2];
} playerstore_t;

static playerstore_t playerstore[MAX_PLAYERS_STORED];

static int nextAge;

/**
 * Logs all accuracy values for a given client. This should be called just before the game ends and then a client leaves.
 * @param clientNum
 */
void LogAcc(int clientNum) {
	char buffer[1024];
	int i;
	qboolean first = qtrue;
	memset(buffer,0,sizeof(buffer));
	Com_sprintf(buffer,sizeof(buffer),"Accuracy: %i ",clientNum);
	for(i=0;i<WP_NUM_WEAPONS;++i) {
		char *tmp = va("%sf%i\\%i\\h%i\\%i",first ? "" : "\\",i,level.clients[clientNum].accuracy[i][0],i,level.clients[clientNum].accuracy[i][1]);
		if(strlen(tmp)+strlen(buffer)+1+1<sizeof(buffer)) /* +1 \n +1 \0 */
			strcat(buffer,tmp);
		first = qfalse;
	}
	strcat(buffer,"\n");
	G_LogPrintf(buffer);
}

/**
 *Resets the player store. Should be called everytime game.qvm is loaded.
 */
void PlayerStoreInit( void ) {
    memset(playerstore,0,sizeof(playerstore));
    nextAge = 1;
}

void PlayerStore_store(char* guid, playerState_t ps) {
    int place2store = -1;
    int lowestAge = 32000;
    int i;
	LogAcc(ps.clientNum);
    if(strlen(guid)<32)
    {
        G_LogPrintf("Playerstore: Failed to store player. Invalid guid: %s\n",guid);
        return;
    }
    for(i=0;i<MAX_PLAYERS_STORED;i++) {
        if(!Q_stricmp(guid,playerstore[i].guid)) {
            place2store=i;
        }
    }

    if(place2store<0)
    for(i=0;i<MAX_PLAYERS_STORED;i++) {
        if(playerstore[i].age < lowestAge) {
            place2store = i;
            lowestAge = playerstore[i].age;
        }
    }

    //Make sure we don't store in -1
    if(place2store<0)
        place2store = 0;
    //place2store is now the place to store to.
    playerstore[place2store].age = nextAge++;
    Q_strncpyz(playerstore[place2store].guid,guid,GUID_SIZE+1);
    memcpy(playerstore[place2store].persistant,ps.persistant,sizeof(int[MAX_PERSISTANT]));
    memcpy(playerstore[place2store].accuracy,level.clients[ps.clientNum].accuracy, sizeof(playerstore[0].accuracy) );
    playerstore[place2store].timePlayed = level.time - level.clients[ps.clientNum].pers.enterTime;
    G_LogPrintf("Playerstore: Stored player with guid: %s in %u\n", playerstore[place2store].guid,place2store);
}

void PlayerStore_restore(char* guid, playerState_t *ps)  {
    int i;
    if(strlen(guid)<32)
    {
        G_LogPrintf("Playerstore: Failed to restore player. Invalid guid: %s\n",guid);
        return;
    }
    for(i=0;i<MAX_PLAYERS_STORED;i++) {
        if(!Q_stricmpn(guid,playerstore[i].guid,GUID_SIZE) && playerstore[i].age != -1) {
            memcpy(ps->persistant,playerstore[i].persistant,sizeof(int[MAX_PERSISTANT]));
            memcpy(level.clients[ps->clientNum].accuracy, playerstore[i].accuracy,sizeof(playerstore[0].accuracy) );
            level.clients[ps->clientNum].pers.enterTime = level.time - playerstore[i].timePlayed;
            //Never ever restore a player with negative score
            if(ps->persistant[PERS_SCORE]<0)
                ps->persistant[PERS_SCORE]=0;
            playerstore[i].age = -1;
            G_LogPrintf("Restored player with guid: %s\n",guid);
            return;
        }
    }
    G_LogPrintf("Playerstore: Nothing to restore. Guid: %s\n",guid);
}
