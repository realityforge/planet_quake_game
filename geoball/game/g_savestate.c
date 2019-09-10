// g_savestate.c
//
// Save and Load game functions based on code from RTCW and ST:EF
//
// created by Dairyman
//

#include "g_local.h"
#include "q_shared.h"

#ifdef SINGLEPLAYER

#define	SAVE_VERSION	1 // version of save/load routines

//
// Function Conversion
//.......................................................................................
// this stores all functions in the game code
typedef struct {
	char *funcStr;
	byte *funcPtr;
} funcList_t;

//-----------------
// disable certain warnings temporarily
#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable : 4054 )
#endif
//-----------------

// declare all game functions used as pointers
// Think functions
extern void BodySink ( gentity_t * ent ) ;
extern void G_FreeEntity ( gentity_t * ed ) ;
extern void RespawnItem ( gentity_t * ent ) ;
extern void FinishSpawningItem( gentity_t *ent );
extern void locateCamera ( gentity_t * ent ) ;
extern void InitShooter_Finish( gentity_t *ent );
extern void Weapon_HookThink ( gentity_t * ent ) ;
extern void G_ExplodeMissile ( gentity_t * ent ) ;
extern void ReturnToPos1 ( gentity_t * ent ) ;
extern void Think_MatchTeam ( gentity_t * ent ) ;
extern void Think_SpawnNewDoorTrigger ( gentity_t * ent ) ;
extern void Think_BeginMoving ( gentity_t * ent ) ;
extern void Think_SetupTrainTargets ( gentity_t * ent ) ;
extern void Think_Target_Delay ( gentity_t * ent ) ;
extern void target_laser_think ( gentity_t * self ) ;
extern void target_laser_start ( gentity_t * self ) ;
extern void target_location_linkup(gentity_t *ent) ;
extern void multi_wait ( gentity_t * ent ) ;
extern void trigger_always_think ( gentity_t * ent ) ;
extern void AimAtTarget ( gentity_t * self ) ;
extern void func_timer_think ( gentity_t * self ) ;
/* uncomment for npc and entities
#ifdef SINGLEPLAYER // npc
extern void ank_shot_think ( gentity_t * ent ) ;
extern void FinishSpawningNPC ( gentity_t * ent ) ;
#endif
#ifdef SINGLEPLAYER // entity
extern void FinishSpawningMD3Anim ( gentity_t * ent ) ;
#endif
*/

// reached functions
extern void Reached_BinaryMover ( gentity_t * ent ) ;
extern void Reached_Train ( gentity_t * ent ) ;

// blocked functions
extern void Blocked_Door ( gentity_t * ent , gentity_t * other ) ;

// touch functions
extern void Touch_Item ( gentity_t * ent , gentity_t * other , trace_t * trace ) ;
extern void Touch_DoorTrigger ( gentity_t * ent , gentity_t * other , trace_t * trace ) ;
extern void Touch_PlatCenterTrigger ( gentity_t * ent , gentity_t * other , trace_t * trace ) ;
extern void Touch_Plat ( gentity_t * ent , gentity_t * other , trace_t * trace ) ;
extern void Touch_Button ( gentity_t * ent , gentity_t * other , trace_t * trace ) ;
extern void Touch_Multi ( gentity_t * self , gentity_t * other , trace_t * trace ) ;
extern void trigger_push_touch ( gentity_t * self , gentity_t * other , trace_t * trace ) ;
extern void trigger_teleporter_touch ( gentity_t * self , gentity_t * other , trace_t * trace ) ;
extern void hurt_touch ( gentity_t * self , gentity_t * other , trace_t * trace ) ;

// use functions
extern void Use_Item( gentity_t *ent, gentity_t *other, gentity_t *activator );
extern void Use_Shooter ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_BinaryMover ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_Target_Give ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_target_remove_powerups ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_Target_Delay ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_Target_Speaker ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_Target_Print ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_Target_Score ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void target_laser_use ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void target_teleporter_use ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void target_use_end_level ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void target_kill_use ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void target_relay_use ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void Use_Multi ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
extern void Use_target_push ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void hurt_use ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void func_timer_use ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void target_use_end_level ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
/* uncomment for npc and entities
#ifdef SINGLEPLAYER // npc
extern void trig_FinishSpawningNPC ( gentity_t * ent , gentity_t * a , gentity_t * b ) ;
#endif
#ifdef SINGLEPLAYER // entity
extern void target_player_stop ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void target_earthquake ( gentity_t * self , gentity_t * other , gentity_t * activator ) ;
extern void ActivateAnimModel ( gentity_t * ent , gentity_t * other , gentity_t * activator ) ;
#endif
*/

// pain functions
/* uncomment for npc and entities
#ifdef SINGLEPLAYER // npc
extern void PainNPC ( gentity_t * self , gentity_t * attacker , int damage ) ;
#endif
*/

// die functions
extern void body_die ( gentity_t * self , gentity_t * inflictor , gentity_t * attacker , int damage , int meansOfDeath ) ;
extern void player_die ( gentity_t * self , gentity_t * inflictor , gentity_t * attacker , int damage , int meansOfDeath ) ;
/* uncomment for npc and entities
#ifdef SINGLEPLAYER // npc
extern void DieNPC ( gentity_t * self , gentity_t * inflictor , gentity_t * attacker , int damage , int mod ) ;
#endif
*/


// conversion table for functions
funcList_t funcList[] = {
// Think functions
	{"BodySink", (byte *)BodySink},
	{"G_FreeEntity", (byte *)G_FreeEntity},
	{"RespawnItem", (byte *)RespawnItem},
	{"FinishSpawningItem", (byte *)FinishSpawningItem},
	{"locateCamera", (byte *)locateCamera},
	{"InitShooter_Finish", (byte *)InitShooter_Finish},
	{"Weapon_HookThink", (byte *)Weapon_HookThink},
	{"G_ExplodeMissile", (byte *)G_ExplodeMissile},
	{"ReturnToPos1", (byte *)ReturnToPos1},
	{"Think_MatchTeam", (byte *)Think_MatchTeam},
	{"Think_SpawnNewDoorTrigger", (byte *)Think_SpawnNewDoorTrigger},
	{"Think_BeginMoving", (byte *)Think_BeginMoving},
	{"Think_SetupTrainTargets", (byte *)Think_SetupTrainTargets},
	{"Think_Target_Delay", (byte *)Think_Target_Delay},
	{"target_laser_think", (byte *)target_laser_think},
	{"target_laser_start", (byte *)target_laser_start},
	{"target_location_linkup", (byte *)target_location_linkup},
	{"multi_wait", (byte *)multi_wait},
	{"trigger_always_think", (byte *)trigger_always_think},
	{"AimAtTarget", (byte *)AimAtTarget},
	{"func_timer_think", (byte *)func_timer_think},
/* uncomment for npc and entities
#ifdef SINGLEPLAYER // npc
	{"ank_shot_think", (byte *)ank_shot_think},
	{"FinishSpawningNPC", (byte *)FinishSpawningNPC},
#endif
#ifdef SINGLEPLAYER // entity
	{"FinishSpawningMD3Anim", (byte *)FinishSpawningMD3Anim},
#endif
*/

// reached functions
	{"Reached_BinaryMover", (byte *)Reached_BinaryMover},
	{"Reached_Train", (byte *)Reached_Train},

// blocked functions
	{"Blocked_Door", (byte *)Blocked_Door},

// touch functions
	{"Touch_Item", (byte *)Touch_Item},
	{"Touch_DoorTrigger", (byte *)Touch_DoorTrigger},
	{"Touch_PlatCenterTrigger", (byte *)Touch_PlatCenterTrigger},
	{"Touch_Plat", (byte *)Touch_Plat},
	{"Touch_Button", (byte *)Touch_Button},
	{"Touch_Multi", (byte *)Touch_Multi},
	{"trigger_push_touch", (byte *)trigger_push_touch},
	{"trigger_teleporter_touch", (byte *)trigger_teleporter_touch},
	{"hurt_touch", (byte *)hurt_touch},

// use functions
	{"Use_Item", (byte *)Use_Item},
	{"Use_Shooter", (byte *)Use_Shooter},
	{"Use_BinaryMover", (byte *)Use_BinaryMover},
	{"Use_Target_Give", (byte *)Use_Target_Give},
	{"Use_target_remove_powerups", (byte *)Use_target_remove_powerups},
	{"Use_Target_Delay", (byte *)Use_Target_Delay},
	{"Use_Target_Speaker", (byte *)Use_Target_Speaker},
	{"Use_Target_Print", (byte *)Use_Target_Print},
	{"Use_Target_Score", (byte *)Use_Target_Score},
	{"target_laser_use", (byte *)target_laser_use},
	{"target_teleporter_use", (byte *)target_teleporter_use},
	{"target_use_end_level", (byte *)target_use_end_level},
	{"target_kill_use", (byte *)target_kill_use},
	{"target_relay_use", (byte *)target_relay_use},
	{"Use_Multi", (byte *)Use_Multi},
	{"Use_target_push", (byte *)Use_target_push},
	{"hurt_use", (byte *)hurt_use},
	{"func_timer_use", (byte *)func_timer_use},
	{"target_use_end_level", (byte *)target_use_end_level},
/* uncomment for npc and entities
#ifdef SINGLEPLAYER // npc
	{"trig_FinishSpawningNPC", (byte *)trig_FinishSpawningNPC},
#endif
#ifdef SINGLEPLAYER // entity
	{"target_player_stop", (byte *)target_player_stop},
	{"target_earthquake", (byte *)target_earthquake},
	{"ActivateAnimModel", (byte *)ActivateAnimModel},
#endif
	*/

// pain functions
/* uncomment for npc and entities
#ifdef SINGLEPLAYER // npc
	{"PainNPC", (byte *)PainNPC},
#endif
*/

// die functions
	{"body_die", (byte *)body_die},
	{"player_die", (byte *)player_die},
/* uncomment for npc and entities
#ifdef SINGLEPLAYER // npc
	{"DieNPC", (byte *)DieNPC},
#endif
*/

	{0, 0}
};

//-----------------
#ifdef _WIN32
#pragma warning( pop )	// return previous warning state
#endif
//-----------------

//
// types of pointers we must convert
//
typedef enum {
	F_NONE,
	F_STRING,
	F_ENTITY,
	F_ITEM,
	F_CLIENT,
	F_FUNCTION
} saveFieldtype_t;

typedef struct {
	int		ofs;
	saveFieldtype_t	type;
} saveField_t;

//....................................................
// these are the fields that cannot be saved directly,
// so they need to be converted
//
// Entity pointers
//
static saveField_t gentityFields[] = {
	{FOFS(client),		F_CLIENT},
	{FOFS(classname),	F_STRING},
	{FOFS(model),		F_STRING},
	{FOFS(model2),		F_STRING},
	{FOFS(parent),		F_ENTITY},
	{FOFS(nextTrain),	F_ENTITY},
	{FOFS(prevTrain),	F_ENTITY},
	{FOFS(message),		F_STRING},
	{FOFS(target),		F_STRING},
	{FOFS(targetname),	F_STRING},
	{FOFS(team),		F_STRING},
	{FOFS(target_ent),	F_ENTITY},
	{FOFS(think),		F_FUNCTION},
	{FOFS(reached),		F_FUNCTION}, 
	{FOFS(blocked),		F_FUNCTION},
	{FOFS(touch),		F_FUNCTION},
	{FOFS(use),			F_FUNCTION},
	{FOFS(pain),		F_FUNCTION},
	{FOFS(die),			F_FUNCTION},
	{FOFS(chain),		F_ENTITY},
	{FOFS(enemy),		F_ENTITY},
	{FOFS(activator),	F_ENTITY},
	{FOFS(teamchain),	F_ENTITY},
	{FOFS(teammaster),	F_ENTITY},
	{FOFS(item),		F_ITEM},
/* uncomment for npc and entities
#ifdef SINGLEPLAYER // npc
	{FOFS(ns.enemy),	F_ENTITY},
#endif
*/
	
	{0, 0}
};

//
// Client pointers
//
static saveField_t gclientFields[] = {
	{CFOFS(hook),		F_ENTITY},

	{0, 0}
};

//
// pointers that are not overwritten when loading a saved game
//
typedef struct {
	int		ofs;
	int		len;
} ignoreField_t;

//
// Entity ignore pointers
//
static ignoreField_t gentityIgnoreFields[] = {
/* uncomment for npc and entities
#ifdef SINGLEPLAYER // npc
	{FOFS(npc),	sizeof(gnpc_t *)},
#endif
*/

	{0, 0}
};

//
// Client ignore pointers
//
static ignoreField_t gclientIgnoreFields[] = {

	{0, 0}
};

/****************************************************************/
//
// find a funcList entry by function address
//
funcList_t *G_FindFuncAtAddress( byte *adr )
{
	int i;

	for (i=0; funcList[i].funcStr; i++) {
		if (funcList[i].funcPtr == adr) {
			return &funcList[i];
		}
	}
	return NULL;
}

//
// find a funcList entry by function name
//
byte *G_FindFuncByName( char *name )
{
	int i;

	for (i=0; funcList[i].funcStr; i++) {
		if (!strcmp(name, funcList[i].funcStr)) {
			return funcList[i].funcPtr;
		}
	}
	return NULL;
}

/****************************************************************/
//
// convert a pointer when saving
//
void WriteField1 (saveField_t *field, byte *base)
{
	void		*p;
	int			len = 0;
	int			index;
	funcList_t	*func;

	p = (void *)(base + field->ofs);
	switch (field->type)
	{
	case F_STRING:
		if ( *(char **)p )
			len = strlen(*(char **)p) + 1;
		else
			len = 0;
		*(int *)p = len;
		break;
	case F_ENTITY:
		if ( *(gentity_t **)p == NULL)
			index = -1;
		else
			index = *(gentity_t **)p - g_entities;
		if (index >= MAX_GENTITIES || index < -1)
			G_Error( "WriteField1: entity out of range (%i)", index );
		*(int *)p = index;
		break;
	case F_CLIENT:
		if ( *(gclient_t **)p == NULL)
			index = -1;
		else
			index = *(gclient_t **)p - level.clients;
		if (index >= MAX_CLIENTS || index < -1)
			G_Error( "WriteField1: client out of range (%i)", index );
		*(int *)p = index;
		break;
	case F_ITEM:
		if ( *(gitem_t **)p == NULL)
			index = -1;
		else
			index = *(gitem_t **)p - bg_itemlist;
		*(int *)p = index;
		break;
	//	match this with a function address in the function list
	//	We then save the string equivalent of the function name.
	case F_FUNCTION:
		if (*(byte **)p == NULL)
			len = 0;
		else {
			func = G_FindFuncAtAddress( *(byte **)p );
			if (!func) {
				G_Error ("WriteField1: unknown function, cannot save game");
			}
			else
			    len = strlen(func->funcStr) + 1;
		}
		*(int *)p = len;
		break;

	default:
		G_Error ("WriteField1: unknown field type");
	}
}

//
// write out the string data
//
void WriteField2 (fileHandle_t f, saveField_t *field, byte *base)
{
	int			len;
	void		*p;
	funcList_t	*func;

	p = (void *)(base + field->ofs);
	switch (field->type)
	{
	case F_STRING:
		if ( *(char **)p > 0)
		{
			trap_FS_Write ("STRG", 4, f);
			len = strlen(*(char **)p) + 1;
			trap_FS_Write (*(char **)p, len, f);
		}
		break;
	case F_FUNCTION:
		if ( *(byte **)p )
		{
			func = G_FindFuncAtAddress( *(byte **)p );
			if (!func) {
				G_Error ("WriteField2: unknown function, cannot save game");
			}
			else
			{
				trap_FS_Write ("STRG", 4, f);
				len = strlen(func->funcStr) + 1;
				trap_FS_Write (func->funcStr, len, f);
			}
		}
		break;
  default:
    break;
	}
}

/*
===============
WriteClient
===============
*/
void WriteClient(fileHandle_t f, gclient_t *cl)
{
	saveField_t	*field;
	gclient_t	temp;
	int length;

	// copy the structure across, then process the fields
	temp = *cl;

	// first, kill all events (assume they have been processed)
	memset( temp.ps.events, 0, sizeof(temp.ps.events) );
	memset( temp.ps.eventParms, 0, sizeof(temp.ps.eventParms) );
	temp.ps.eventSequence = 0;
	temp.ps.entityEventSequence = 0;

	// change the pointers to lengths or indexes
	for (field=gclientFields ; field->type ; field++)
	{
		WriteField1 (field, (byte *)&temp);
	}

	trap_FS_Write ("CLEN", 4, f);
	// write the block
	length = sizeof( temp );
	trap_FS_Write (&length, sizeof(length), f);
	trap_FS_Write (&temp, length, f);

	// now write any allocated data following the edict
	for (field=gclientFields ; field->type ; field++)
	{
		WriteField2 (f, field, (byte *)cl);
	}
	
}

/*
===============
WriteEntity
===============
*/
void WriteEntity(fileHandle_t f, gentity_t *ent)
{
	saveField_t	*field;
	gentity_t	temp;
	int length;

	// copy the structure across, then process the fields
	temp = *ent;

	// first, kill all events (assume they have been processed)
	temp.s.event = 0;
	temp.s.eventParm = 0;

	// change the pointers to lengths or indexes
	for (field=gentityFields ; field->type ; field++)
	{
		WriteField1 (field, (byte *)&temp);
	}

	trap_FS_Write ("GENT", 4, f);
	// write the block
	length = sizeof( temp);
	trap_FS_Write (&length, sizeof(length), f);
	trap_FS_Write (&temp, length, f);

	// now write any allocated data following the edict
	for (field=gentityFields ; field->type ; field++)
	{
		WriteField2 (f, field, (byte *)ent);
	}
}

/*
===============
G_SaveGame

  returns qtrue if successful

===============
*/
qboolean G_SaveGame(char *username)
{
	char filename[MAX_QPATH];
	char	mapstr[MAX_QPATH];
	fileHandle_t f;
	int	i;
	int skill;
	gentity_t	*ent;
	gclient_t	*cl;

	if(g_entities[0].health <= 0)	// no save when dead
		return qtrue;

	if(g_gametype.integer != GT_SINGLE_PLAYER) {
		return qtrue;
	}

	if (!username) {
		username = "current";
	}

	G_Printf("Saving game as %s\n", username);

	// validate the filename
	for (i=0; i<strlen(username); i++) {
		if (!Q_isforfilename( username[i] ) && username[i] != '\\') {	// (allow '\\' so games can be saved in subdirs)
			G_Printf( "G_SaveGame: '%s'.  Invalid character (%c) in filename. Must use alphanumeric characters only.\n", username, username[i]);
			return qtrue;
		}
	}

	// open the file
	Com_sprintf( filename, MAX_QPATH, "save\\%s.svg", username );
	if (trap_FS_FOpenFile( filename, &f, FS_WRITE ) < 0) {
		G_Error( "G_SaveGame: cannot open file for saving\n" );
	}

	// write the version
	i = SAVE_VERSION;
	trap_FS_Write (&i, sizeof(i), f);

	// write the mapname
	trap_Cvar_VariableStringBuffer( "mapname", mapstr, sizeof(mapstr) );
	trap_FS_Write (mapstr, MAX_QPATH, f);

	// write out the level time
	trap_FS_Write (&level.time, sizeof(level.time), f);

	// save the skill level
	skill = trap_Cvar_VariableValue( "g_spSkill" );
	trap_FS_Write (&skill, sizeof(skill), f);

	// write out the entity structures
	i = sizeof(gentity_t);
	trap_FS_Write (&i, sizeof(i), f);

	for (i=0 ; i<level.num_entities ; i++)
	{
		ent = &g_entities[i];
		if (!ent->inuse || ent->s.number == ENTITYNUM_WORLD)
			continue;
		trap_FS_Write (&i, sizeof(i), f);
		WriteEntity (f, ent);
	}
	i = -1;
	trap_FS_Write (&i, sizeof(i), f);

	// write out the client structures
	i = sizeof(gclient_t);
	trap_FS_Write (&i, sizeof(i), f);
	for (i=0 ; i<MAX_CLIENTS ; i++)
	{
		cl = &level.clients[i];
		if (cl->pers.connected != CON_CONNECTED)
			continue;
		trap_FS_Write (&i, sizeof(i), f);
		WriteClient (f, cl);
	}
	i = -1;
	trap_FS_Write (&i, sizeof(i), f);

	trap_FS_FCloseFile( f );

	return qtrue;
}

/****************************************************************/
//
// Read saved file
//

void ReadField (fileHandle_t f, saveField_t *field, byte *base)
{
	void		*p;
	int			len;
	int			index;
	char		funcStr[512];
	char		strhdr[5];

	p = (void *)(base + field->ofs);
	switch (field->type)
	{
	case F_STRING:
		len = *(int *)p;
		if (!len)
			*(char **)p = NULL;
		else
		{
			trap_FS_Read (strhdr, 4, f);
			strhdr[4] = 0;
			if (Q_stricmp (strhdr, "STRG") != 0)
				G_Error ("ReadField: unknown string '%s'\ncannot load game", strhdr);
			*(char **)p = G_Alloc (len);
			trap_FS_Read (*(char **)p, len, f);
		}
		break;
	case F_ENTITY:
		index = *(int *)p;
		if (index >= MAX_GENTITIES || index < -1)
			G_Error( "ReadField: entity out of range (%i)", index );
		if ( index == -1 )
			*(gentity_t **)p = NULL;
		else
			*(gentity_t **)p = &g_entities[index];
		break;
	case F_CLIENT:
		index = *(int *)p;
		if (index >= MAX_CLIENTS || index < -1)
			G_Error( "ReadField: client out of range (%i)", index );
		if ( index == -1 )
			*(gclient_t **)p = NULL;
		else
			*(gclient_t **)p = &level.clients[index];
		break;
	case F_ITEM:
		index = *(int *)p;
		if ( index == -1 )
			*(gitem_t **)p = NULL;
		else
			*(gitem_t **)p = &bg_itemlist[index];
		break;

	//relative to code segment
	case F_FUNCTION:
		len = *(int *)p;
		if (!len)
			*(byte **)p = NULL;
		else
		{
			if (len > sizeof(funcStr)) {
				G_Error ("ReadField: function name is greater than buffer (%i chars)", sizeof(funcStr));
			}
			trap_FS_Read (strhdr, 4, f);
			strhdr[4] = 0;
			if (Q_stricmp (strhdr, "STRG") != 0)
				G_Error ("ReadField: unknown string '%s'\ncannot load game", strhdr);
			trap_FS_Read (funcStr, len, f);
			*(byte **)p = G_FindFuncByName( funcStr );
			if (!(*(byte **)p)) {
				G_Error ("ReadField: unknown function '%s'\ncannot load game", funcStr);
			}
		}
		break;

	default:
		G_Error ("ReadField: unknown field type");
	}
}

/*
===============
ReadClient
===============
*/
void ReadClient (fileHandle_t f, gclient_t *client, int size)
{
	saveField_t	*field;
	ignoreField_t *ifield;
	gclient_t	temp;
	gentity_t	*ent;
	int decodedSize;

	// read the encoded chunk
	trap_FS_Read (&decodedSize, sizeof(int), f);
	trap_FS_Read ( (byte *)&temp, decodedSize, f );\

	// convert any feilds back to the correct data
	for (field=gclientFields ; field->type ; field++)
	{
		ReadField (f, field, (byte *)&temp);
	}

	// backup any fields that we don't want to read in
	for (ifield=gclientIgnoreFields ; ifield->len ; ifield++)
	{
		memcpy( ((byte *)&temp) + ifield->ofs, ((byte *)client) + ifield->ofs, ifield->len );
	}

	// now copy the temp structure into the existing structure
	memcpy( client, &temp, size );

	ent = &g_entities[client->ps.clientNum];

	trap_GetUsercmd( ent->client - level.clients, &ent->client->pers.cmd );
	SetClientViewAngle( ent, ent->client->ps.viewangles );

	// run a client frame to drop exactly to the floor,
	// initialize animations and other things
	trap_GetUsercmd( ent-g_entities, &ent->client->pers.cmd );
	ent->client->ps.commandTime = ent->client->pers.cmd.serverTime - 100;
	ClientThink( ent-g_entities );

}

/* uncomment for npc and entities
#ifdef SINGLEPLAYER // npc
//
// NPC time data fixup
//
void NPCTime (gentity_t *ent, int leveltime)
{
	if(ent->ns.checkTime!=0)
		ent->ns.checkTime = ent->ns.checkTime - leveltime + level.time ;
	if(ent->ns.dontMoveTime!=0)
		ent->ns.dontMoveTime = ent->ns.dontMoveTime - leveltime + level.time ;
	if(ent->ns.attackTime!=0)
		ent->ns.attackTime = ent->ns.attackTime - leveltime + level.time ;
	if(ent->ns.fireTime!=0)
		ent->ns.fireTime = ent->ns.fireTime - leveltime + level.time ;
	if(ent->ns.meleeTime!=0)
		ent->ns.meleeTime = ent->ns.meleeTime - leveltime + level.time ;
	if(ent->ns.quakeTime!=0)
		ent->ns.quakeTime = ent->ns.quakeTime - leveltime + level.time ;
	if(ent->ns.ps.commandTime!=0)
		ent->ns.ps.commandTime = ent->ns.ps.commandTime - leveltime + level.time ;
}
#endif
*/

//
// Entity time fixup
//
void EntTime (gentity_t *ent, int leveltime)
{
	if(ent->timestamp!=0)
		ent->timestamp = ent->timestamp - leveltime + level.time ;
	if(ent->pain_debounce_time!=0)
		ent->pain_debounce_time = ent->pain_debounce_time - leveltime + level.time ;
	if(ent->nextthink!=0)
		ent->nextthink = ent->nextthink - leveltime + level.time ;
	if(ent->s.pos.trTime!=0)
		ent->s.pos.trTime = level.time ;
}

/*
===============
ReadEntity
===============
*/
void ReadEntity (fileHandle_t f, gentity_t *ent, int size, int leveltime)
{
	saveField_t	*field;
	ignoreField_t *ifield;
	gentity_t	temp, backup, backup2;
	int decodedSize;

	backup = *ent;

	// read the encoded chunk
	trap_FS_Read (&decodedSize, sizeof(int), f);
	trap_FS_Read ( (byte *)&temp, decodedSize, f );

	// convert any fields back to the correct data
	for (field=gentityFields ; field->type ; field++)
	{
		ReadField (f, field, (byte *)&temp);
	}

	// backup any fields that we don't want to read in
	for (ifield=gentityIgnoreFields ; ifield->len ; ifield++)
	{
		memcpy( ((byte *)&temp) + ifield->ofs, ((byte *)ent) + ifield->ofs, ifield->len );
	}

	// kill all events (assume they have been processed)
	if (!temp.freeAfterEvent) {
		temp.s.event = 0;
		temp.s.eventParm = 0;
	}

	// now copy the temp structure into the existing structure
	memcpy( ent, &temp, size );

/* uncomment for npc and entities
#ifdef SINGLEPLAYER // npc
//
// Fixup NPCs that are dead to show only last frame of animation
//
	if(ent->npc && ent->health<=0) 
	{
		ent->ns.ps.eFlags |= EF_FORCE_END_FRAME;
	}
#endif

#ifdef SINGLEPLAYER // entity
//
// Fixup misc_model_anim with hidden end to not display again
//
	if(Q_stricmp(ent->classname, "misc_model_anim")==0)
	{
		if(ent->s.modelindex2>0 && (ent->spawnflags & 2 ))
		{
			ent->s.eFlags |= EF_FORCE_END_FRAME;
		}
	}
#endif
*/

//
// Fixup entity times
//
	EntTime (ent, leveltime);
/* uncomment for npc and entities
#ifdef SINGLEPLAYER // npc
	NPCTime (ent, leveltime);
#endif
*/

	// notify server of changes in position/orientation
	if (ent->r.linked) {
		trap_LinkEntity( ent );
	} else {
		trap_UnlinkEntity( ent );
	}

	// if this is a mover, check areaportals
	if (ent->s.eType == ET_MOVER && ent->moverState != backup.moverState) {
		if ( ent->teammaster == ent || !ent->teammaster ) {
			if (ent->moverState == MOVER_POS1 ) {
				// closed areaportal
				trap_AdjustAreaPortalState( ent, qfalse );
			} else {	// must be open
				// portals are always opened before the mover starts to open, so we must move
				// it back to the start position, link, set portals, then move it back
				backup2 = *ent;
				*ent = backup;
				// link it at original position
				trap_LinkEntity( ent );
				// set portals
				trap_AdjustAreaPortalState( ent, qtrue );
				// put it back
				*ent = backup2;
				trap_LinkEntity( ent );
			}
		}
	}

}

/*
===============
G_LoadGame

===============
*/
void G_LoadGame(char *username)
{
	char mapname[MAX_QPATH];
	char filename[MAX_QPATH];
	fileHandle_t f;
	int	i, leveltime, size, last;
	gentity_t	*ent;
	gclient_t	*cl;
	qboolean	serverEntityUpdate = qfalse;
	char strhdr[5];

	if(g_gametype.integer != GT_SINGLE_PLAYER) {
		return;
	}

	Com_sprintf( filename, MAX_QPATH, "save\\%s.svg", username );

	// open the file
	if (trap_FS_FOpenFile( filename, &f, FS_READ ) < 0) {
		G_Error( "G_LoadGame: savegame '%s' not found\n", filename );
	}

	// read the version
	trap_FS_Read (&i, sizeof(i), f);

	if (i != SAVE_VERSION) {
		trap_FS_FCloseFile( f );
		G_Error( "G_LoadGame: savegame '%s' is wrong version (%i, should be %i)\n", filename, i, SAVE_VERSION );
	}

	// discard the mapname 
	trap_FS_Read (mapname, MAX_QPATH, f);

	// read the level time
	trap_FS_Read (&i, sizeof(i), f);	
	leveltime = i;

	// read the game skill
	trap_FS_Read (&i, sizeof(i), f);
	// set the skill level
	trap_Cvar_Set( "g_spSkill", va("%i",i) );

	// read the entity structures
	trap_FS_Read (&i, sizeof(i), f);
	size = i;
	last = 0;
	while (1)
	{
		trap_FS_Read (&i, sizeof(i), f);
		if (i < 0)
			break;
		if (i >= MAX_GENTITIES) {
			trap_FS_FCloseFile( f );
			G_Error( "G_LoadGame: entitynum out of range (%i, MAX = %i)\n", i, MAX_GENTITIES );
		}
		if (i >= level.num_entities) {	// notify server
			level.num_entities = i + 1;
			serverEntityUpdate = qtrue;
		}
		trap_FS_Read (strhdr, 4, f);
		strhdr[4] = 0;
		if (Q_stricmp (strhdr, "GENT") != 0)
			G_Error ("Load Entity : Unknown header '%s'\ncannot load game", strhdr);
		ent = &g_entities[i];
		ReadEntity (f, ent, size, leveltime);


		// free all entities that we skipped
		for ( ; last < i; last++) {
			if (g_entities[last].inuse && i != ENTITYNUM_WORLD) {
				if (last < MAX_CLIENTS) {
					trap_DropClient( last, "" );
				} else {
					G_FreeEntity( &g_entities[last] );
				}
			}
		} 
		last = i+1; 
	}

	// clear all remaining entities
	for (ent=&g_entities[last] ; last < MAX_GENTITIES ; last++, ent++) {
		memset (ent, 0, sizeof(*ent));
		ent->classname = "freed";
		ent->freetime = level.time;
		ent->inuse = qfalse;
	}

	// read the client structures
	trap_FS_Read (&i, sizeof(i), f);
	size = i;
	while (1)
	{
		trap_FS_Read (&i, sizeof(i), f);
		if (i < 0)
			break;
		if (i > MAX_CLIENTS) {
			trap_FS_FCloseFile( f );
			G_Error( "G_LoadGame: clientnum out of range\n" );
		}
		cl = &level.clients[i];
		if (cl->pers.connected == CON_DISCONNECTED) {
			trap_FS_FCloseFile( f );
			G_Error( "G_LoadGame: client mis-match in savegame" );
		}
		trap_FS_Read (strhdr, 4, f);
		strhdr[4] = 0;
		if (Q_stricmp (strhdr, "CLEN") != 0)
			G_Error ("Load Client : Unknown header '%s'\ncannot load game", strhdr);
		ReadClient (f, cl, size);
	}

	// inform server of entity count if it has increased
	if (serverEntityUpdate) {
		// let the server system know that there are more entities
		trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ), 
			&level.clients[0].ps, sizeof( level.clients[0] ) );
	}

	trap_FS_FCloseFile( f );

}

#endif