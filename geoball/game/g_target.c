/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
#include "g_local.h"

//==========================================================

/*QUAKED target_give (1 0 0) (-8 -8 -8) (8 8 8)
Gives the activator all the items pointed to.
*/
void Use_Target_Give( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	gentity_t	*t;
	trace_t		trace;

	if ( !activator->client ) {
		return;
	}

	if ( !ent->target ) {
		return;
	}

	memset( &trace, 0, sizeof( trace ) );
	t = NULL;
	while ( (t = G_Find (t, FOFS(targetname), ent->target)) != NULL ) {
		if ( !t->item ) {
			continue;
		}
		Touch_Item( t, activator, &trace );

		// make sure it isn't going to respawn or show any events
		t->nextthink = 0;
		trap_UnlinkEntity( t );
	}
}

void SP_target_give( gentity_t *ent ) {
	ent->use = Use_Target_Give;
}


//==========================================================

/*QUAKED target_remove_powerups (1 0 0) (-8 -8 -8) (8 8 8)
takes away all the activators powerups.
Used to drop flight powerups into death puts.
*/
void Use_target_remove_powerups( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	if( !activator->client ) {
		return;
	}

	if( activator->client->ps.powerups[PW_REDFLAG] ) {
		Team_ReturnFlag( TEAM_RED );
	} else if( activator->client->ps.powerups[PW_BLUEFLAG] ) {
		Team_ReturnFlag( TEAM_BLUE );
	} else if( activator->client->ps.powerups[PW_NEUTRALFLAG] ) {
		Team_ReturnFlag( TEAM_FREE );
	}

	memset( activator->client->ps.powerups, 0, sizeof( activator->client->ps.powerups ) );
}

void SP_target_remove_powerups( gentity_t *ent ) {
	ent->use = Use_target_remove_powerups;
}


//==========================================================

/*QUAKED target_delay (1 0 0) (-8 -8 -8) (8 8 8)
"wait" seconds to pause before firing targets.
"random" delay variance, total delay = delay +/- random seconds
*/
void Think_Target_Delay( gentity_t *ent ) {
	G_UseTargets( ent, ent->activator );
}

void Use_Target_Delay( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	ent->nextthink = level.time + ( ent->wait + ent->random * crandom() ) * 1000;
	ent->think = Think_Target_Delay;
	ent->activator = activator;
}

void SP_target_delay( gentity_t *ent ) {
	// check delay for backwards compatability
	if ( !G_SpawnFloat( "delay", "0", &ent->wait ) ) {
		G_SpawnFloat( "wait", "1", &ent->wait );
	}

	if ( !ent->wait ) {
		ent->wait = 1;
	}
	ent->use = Use_Target_Delay;
}


//==========================================================

/*QUAKED target_score (1 0 0) (-8 -8 -8) (8 8 8)
"count" number of points to add, default 1

The activator is given this many points.
*/
void Use_Target_Score (gentity_t *ent, gentity_t *other, gentity_t *activator) {
	AddScore( activator, ent->r.currentOrigin, ent->count );
}

void SP_target_score( gentity_t *ent ) {
	if ( !ent->count ) {
		ent->count = 1;
	}
	ent->use = Use_Target_Score;
}


//==========================================================

/*QUAKED target_print (1 0 0) (-8 -8 -8) (8 8 8) redteam blueteam private
"message"	text to print
If "private", only the activator gets the message.  If no checks, all clients get the message.
*/
void Use_Target_Print (gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if ( activator->client && ( ent->spawnflags & 4 ) ) {
		trap_SendServerCommand( activator-g_entities, va("cp \"%s\"", ent->message ));
		return;
	}

	if ( ent->spawnflags & 3 ) {
		if ( ent->spawnflags & 1 ) {
			G_TeamCommand( TEAM_RED, va("cp \"%s\"", ent->message) );
		}
		if ( ent->spawnflags & 2 ) {
			G_TeamCommand( TEAM_BLUE, va("cp \"%s\"", ent->message) );
		}
		return;
	}

	trap_SendServerCommand( -1, va("cp \"%s\"", ent->message ));
}

void SP_target_print( gentity_t *ent ) {
	ent->use = Use_Target_Print;
}


//==========================================================


/*QUAKED target_speaker (1 0 0) (-8 -8 -8) (8 8 8) looped-on looped-off global activator
"noise"		wav file to play

A global sound will play full volume throughout the level.
Activator sounds will play on the player that activated the target.
Global and activator sounds can't be combined with looping.
Normal sounds play each time the target is used.
Looped sounds will be toggled by use functions.
Multiple identical looping sounds will just increase volume without any speed cost.
"wait" : Seconds between auto triggerings, 0 = don't auto trigger
"random"	wait variance, default is 0
*/
void Use_Target_Speaker (gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if (ent->spawnflags & 3) {	// looping sound toggles
		if (ent->s.loopSound)
			ent->s.loopSound = 0;	// turn it off
		else
			ent->s.loopSound = ent->noise_index;	// start it
	}else {	// normal sound
		if ( ent->spawnflags & 8 ) {
			G_AddEvent( activator, EV_GENERAL_SOUND, ent->noise_index );
		} else if (ent->spawnflags & 4) {
			G_AddEvent( ent, EV_GLOBAL_SOUND, ent->noise_index );
		} else {
			G_AddEvent( ent, EV_GENERAL_SOUND, ent->noise_index );
		}
	}
}

void SP_target_speaker( gentity_t *ent ) {
	char	buffer[MAX_QPATH];
	char	*s;

	G_SpawnFloat( "wait", "0", &ent->wait );
	G_SpawnFloat( "random", "0", &ent->random );

	if ( !G_SpawnString( "noise", "NOSOUND", &s ) ) {
		G_Error( "target_speaker without a noise key at %s", vtos( ent->s.origin ) );
	}

	// force all client reletive sounds to be "activator" speakers that
	// play on the entity that activates it
	if ( s[0] == '*' ) {
		ent->spawnflags |= 8;
	}

	if (!strstr( s, ".wav" )) {
		Com_sprintf (buffer, sizeof(buffer), "%s.wav", s );
	} else {
		Q_strncpyz( buffer, s, sizeof(buffer) );
	}
	ent->noise_index = G_SoundIndex(buffer);

	// a repeating speaker can be done completely client side
	ent->s.eType = ET_SPEAKER;
	ent->s.eventParm = ent->noise_index;
	ent->s.frame = ent->wait * 10;
	ent->s.clientNum = ent->random * 10;


	// check for prestarted looping sound
	if ( ent->spawnflags & 1 ) {
		ent->s.loopSound = ent->noise_index;
	}

	ent->use = Use_Target_Speaker;

	if (ent->spawnflags & 4) {
		ent->r.svFlags |= SVF_BROADCAST;
	}

	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	// must link the entity so we get areas and clusters so
	// the server can determine who to send updates to
	trap_LinkEntity( ent );
}



//==========================================================

/*QUAKED target_laser (0 .5 .8) (-8 -8 -8) (8 8 8) START_ON
When triggered, fires a laser.  You can either set a target or a direction.
*/
void target_laser_think (gentity_t *self) {
	vec3_t	end;
	trace_t	tr;
	vec3_t	point;

	// if pointed at another entity, set movedir to point at it
	if ( self->enemy ) {
		VectorMA (self->enemy->s.origin, 0.5, self->enemy->r.mins, point);
		VectorMA (point, 0.5, self->enemy->r.maxs, point);
		VectorSubtract (point, self->s.origin, self->movedir);
		VectorNormalize (self->movedir);
	}

	// fire forward and see what we hit
	VectorMA (self->s.origin, 2048, self->movedir, end);

	trap_Trace( &tr, self->s.origin, NULL, NULL, end, self->s.number, CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE);

	if ( tr.entityNum ) {
		// hurt it if we can
		G_Damage ( &g_entities[tr.entityNum], self, self->activator, self->movedir, 
			tr.endpos, self->damage, DAMAGE_NO_KNOCKBACK, MOD_TARGET_LASER);
	}

	VectorCopy (tr.endpos, self->s.origin2);

	trap_LinkEntity( self );
	self->nextthink = level.time + FRAMETIME;
}

void target_laser_on (gentity_t *self)
{
	if (!self->activator)
		self->activator = self;
	target_laser_think (self);
}

void target_laser_off (gentity_t *self)
{
	trap_UnlinkEntity( self );
	self->nextthink = 0;
}

void target_laser_use (gentity_t *self, gentity_t *other, gentity_t *activator)
{
	self->activator = activator;
	if ( self->nextthink > 0 )
		target_laser_off (self);
	else
		target_laser_on (self);
}

void target_laser_start (gentity_t *self)
{
	gentity_t *ent;

	self->s.eType = ET_BEAM;

	if (self->target) {
		ent = G_Find (NULL, FOFS(targetname), self->target);
		if (!ent) {
			G_Printf ("%s at %s: %s is a bad target\n", self->classname, vtos(self->s.origin), self->target);
		}
		self->enemy = ent;
	} else {
		G_SetMovedir (self->s.angles, self->movedir);
	}

	self->use = target_laser_use;
	self->think = target_laser_think;

	if ( !self->damage ) {
		self->damage = 1;
	}

	if (self->spawnflags & 1)
		target_laser_on (self);
	else
		target_laser_off (self);
}

void SP_target_laser (gentity_t *self)
{
	// let everything else get spawned before we start firing
	self->think = target_laser_start;
	self->nextthink = level.time + FRAMETIME;
}


//==========================================================

void target_teleporter_use( gentity_t *self, gentity_t *other, gentity_t *activator ) {
	gentity_t	*dest;

	if (!activator->client)
		return;
	dest = 	G_PickTarget( self->target );
	if (!dest) {
		G_Printf ("Couldn't find teleporter destination\n");
		return;
	}

	TeleportPlayer( activator, dest->s.origin, dest->s.angles );
}

/*QUAKED target_teleporter (1 0 0) (-8 -8 -8) (8 8 8)
The activator will be teleported away.
*/
void SP_target_teleporter( gentity_t *self ) {
	if (!self->targetname)
		G_Printf("untargeted %s at %s\n", self->classname, vtos(self->s.origin));

	self->use = target_teleporter_use;
}

//==========================================================


/*QUAKED target_relay (.5 .5 .5) (-8 -8 -8) (8 8 8) RED_ONLY BLUE_ONLY RANDOM
This doesn't perform any actions except fire its targets.
The activator can be forced to be from a certain team.
if RANDOM is checked, only one of the targets will be fired, not all of them
*/
/*void target_relay_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	if ( ( self->spawnflags & 1 ) && activator->client 
		&& activator->client->sess.sessionTeam != TEAM_RED ) {
		return;
	}
	if ( ( self->spawnflags & 2 ) && activator->client 
		&& activator->client->sess.sessionTeam != TEAM_BLUE ) {
		return;
	}
	if ( self->spawnflags & 4 ) {
		gentity_t	*ent;

		ent = G_PickTarget( self->target );
		if ( ent && ent->use ) {
			ent->use( ent, self, activator );
		}
		return;
	}
	G_UseTargets (self, activator);
}

void SP_target_relay (gentity_t *self) {
	self->use = target_relay_use;
}

*/
//newq3ball
/*QUAKED target_relay (.5 .5 .5) (-8 -8 -8) (8 8 8) RED_ONLY BLUE_ONLY RANDOM
This doesn't perform any actions except fire its targets.
The activator can be forced to be from a certain team.
if RANDOM is checked, only one of the targets will be fired, not all of them
*/

void target_relay_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	if ( ( self->spawnflags & 1 ) && activator->client 
		&& activator->client->sess.sessionTeam != TEAM_RED ) {
		return;
	}
	if ( ( self->spawnflags & 2 ) && activator->client 
		&& activator->client->sess.sessionTeam != TEAM_BLUE ) {
		return;
	}
	if ( self->spawnflags & 4 ) {
		gentity_t	*ent;

#if !ESCAPE_MODE	// JUHOX: G_PickTarget() also needs to know the segment
		ent = G_PickTarget( self->target );
#else
		ent = G_PickTarget(self->target, self->worldSegment - 1);
#endif
		if ( ent && ent->use ) {
			ent->use( ent, self, activator );
		}
		return;
	}
	G_UseTargets (self, activator);
}

#if 1	// JUHOX: target_relay_think
void target_relay_think(gentity_t* self)
{
	target_relay_use(self, NULL, self->activator);
}
#endif

#if 1	// JUHOX: target_relay_triggered
void target_relay_triggered(gentity_t *self, gentity_t *other, gentity_t *activator)
{
	self->think = target_relay_think;
	self->nextthink = level.time + (self->wait + self->random * crandom()) * 1000;
	self->activator = activator;
}
#endif

void SP_target_relay (gentity_t *self) {
#if 0	// JUHOX: target_relay now accepts "wait" and "random"
	self->use = target_relay_use;
#else
	if (G_SpawnFloat("wait", "0", &self->wait))
	{
		self->use = target_relay_triggered;
	}
	else
	{
		self->use = target_relay_use;
	}
#endif
#if ESCAPE_MODE	// JUHOX: set entity class
	self->entClass = GEC_target_relay;
#endif
}
//endnew

//==========================================================

/*QUAKED target_kill (.5 .5 .5) (-8 -8 -8) (8 8 8)
Kills the activator.
*/
void target_kill_use( gentity_t *self, gentity_t *other, gentity_t *activator ) {
	G_Damage ( activator, NULL, NULL, NULL, NULL, 100000, DAMAGE_NO_PROTECTION, MOD_TELEFRAG);
}

void SP_target_kill( gentity_t *self ) {
	self->use = target_kill_use;
}

/*QUAKED target_position (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for in-game calculation, like jumppad targets.
*/
void SP_target_position( gentity_t *self ){
	G_SetOrigin( self, self->s.origin );
}

#ifndef SINGLEPLAYER // static fixup
static void target_location_linkup(gentity_t *ent)
#else
void target_location_linkup(gentity_t *ent)
#endif
{
	int i;
	int n;

	if (level.locationLinked) 
		return;

	level.locationLinked = qtrue;

	level.locationHead = NULL;

	trap_SetConfigstring( CS_LOCATIONS, "unknown" );

	for (i = 0, ent = g_entities, n = 1;
			i < level.num_entities;
			i++, ent++) {
		if (ent->classname && !Q_stricmp(ent->classname, "target_location")) {
			// lets overload some variables!
			ent->health = n; // use for location marking
			trap_SetConfigstring( CS_LOCATIONS + n, ent->message );
			n++;
			ent->nextTrain = level.locationHead;
			level.locationHead = ent;
		}
	}

	// All linked together now
}

/*QUAKED target_location (0 0.5 0) (-8 -8 -8) (8 8 8)
Set "message" to the name of this location.
Set "count" to 0-7 for color.
0:white 1:red 2:green 3:yellow 4:blue 5:cyan 6:magenta 7:white

Closest target_location in sight used for the location, if none
in site, closest in distance
*/
void SP_target_location( gentity_t *self ){
	self->think = target_location_linkup;
	self->nextthink = level.time + 200;  // Let them all spawn first

	G_SetOrigin( self, self->s.origin );
}

#ifdef SINGLEPLAYER
/*QUAKED target_level_end (1 0 0) (-16 -16 -24) (16 16 32)
when triggered, the level ends
"message"	use it to set the name of the next level without any extension
targetname: To trigger the level end.
*/

char next_level[10][MAX_QPATH]; // max of 10 level change entities per level
int num_next_levels=0;

void target_use_end_level( gentity_t *self, gentity_t *other, gentity_t *activator ) {
	char buf[MAX_QPATH];
	char *nx= next_level[self->health];

	trap_Cvar_Set("Save_Loading", "2");
	trap_Cvar_Set("ui_LoadGame", "");
	G_SavePersistant(nx);
	Com_sprintf(buf,MAX_QPATH,"map %s",nx);;
	trap_Cvar_Set( "nextmap", buf );
	ExitLevel();
}

void SP_target_level_end( gentity_t *self ) {
 char *buf;
	char *nx=next_level[num_next_levels];

	self->health=num_next_levels;
	G_SpawnString( "message", "missinglevel", &buf);
	G_Printf("nextmap (%d): %s\n",num_next_levels,buf);
	Com_sprintf(nx,sizeof(next_level[0]),"%s",buf);
	self->use = target_use_end_level;
	num_next_levels++;
} 

//q3geoballnew
/*QUAKED target_objective (1.0 0 0) (-4 -4 -4) (4 4 4)
When used, the objective in the <mapname>.efo with this objective's "count" will be marked as completed

count - number of objective (as listed in the maps' <mapname>.efo)

NOTE: the objective with the lowest "count" will be considered the current objective
*/
/*
void target_objective_use( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	gentity_t *tent;

	tent = G_TempEntity( self->r.currentOrigin, EV_OBJECTIVE_COMPLETE );
	//Be sure to send the event to everyone
	tent->r.svFlags |= SVF_BROADCAST;
	tent->s.eventParm = self->count;
}

void SP_target_objective (gentity_t *self)
{
	if ( self->count <= 0 )
	{
		//FIXME: error msg
		G_FreeEntity( self );
		return;
	}
	if ( self->targetname )
	{
		self->use = target_objective_use;
	}
	level.numObjectives++;
}
//endnew
*/
//
// target_objective
//

#define MAX_OBJECTIVES 32
#define MAX_STATES_PER_OBJECTIVE 11
#define MAX_TARGETS_PER_OBJECTIVE 64

typedef struct
{
	int state;
	char* target;
} objectiveTarget_t;
typedef struct
{
	int priority;
	int state;
	int maxState;
	char* messages[MAX_STATES_PER_OBJECTIVE];
	objectiveTarget_t targets[MAX_TARGETS_PER_OBJECTIVE];
	int numTargets;
	gentity_t* entity;
} objective_t;

static objective_t objectives[MAX_OBJECTIVES];
static int numObjectives = 0;
static objective_t* sortedObjectives[MAX_OBJECTIVES];
static int numSortedObjectives = 0;
static qboolean objectivesRequireUpdate = qtrue;
static int objectivesUpdateId = 1;

static void AddObjectiveToSortedList(objective_t* objective)
{
	int i;
	int k;

	for (i = 0; i < numSortedObjectives; i++)
	{
		if (objective->priority < sortedObjectives[i]->priority) break;
		if (objective->priority == sortedObjectives[i]->priority)
		{
			G_Error("Duplicate objective priority %d", objective->priority);
		}
	}

	for (k = numSortedObjectives - 1; k >= i; k--)
	{
		sortedObjectives[k+1] = sortedObjectives[k];
	}
	sortedObjectives[i] = objective;
	numSortedObjectives++;
}

static void trigger_target_objective(gentity_t* obj, gentity_t* activator)
{
	objective_t* objective;
	int i;

	objective = &objectives[obj->count];
	if (objective->state >= objective->maxState) return;
	objective->state++;

	/*
	if (objective->state < MAX_STATES_PER_OBJECTIVE)
	{
		trap_SendServerCommand(-1, va("cp \"%s\"", objective->messages[objective->state]));
	}
	*/
	
	for (i = 0; i < objective->numTargets; i++)
	{
		objectiveTarget_t* target;

		target = &objective->targets[i];
		if (target->state != objective->state) continue;

		obj->target = target->target;
		G_UseTargets(obj, activator);
	}

	objectivesRequireUpdate = qtrue;
}

static void use_target_objective(gentity_t* ent, gentity_t* other, gentity_t* activator)
{
	trigger_target_objective(ent, activator);
}

static void touch_target_objective(gentity_t* ent, gentity_t* other, trace_t* trace)
{
	if (!other->client) return;
	trigger_target_objective(ent, other);
}

void SP_target_objective(gentity_t* self)
{
	objective_t* objective;
	int i;

	if (numObjectives >= MAX_OBJECTIVES)
	{
		G_Error("Too many objectives (max. %d)", MAX_OBJECTIVES);
		return;
	}

	self->use = use_target_objective;
	self->touch = touch_target_objective;

	self->count = numObjectives;
	objective = &objectives[numObjectives++];
	memset(objective, 0, sizeof(*objective));

	objective->entity = self;
	G_SpawnInt("priority", "0", &objective->priority);

	G_SpawnInt("maxState", "0", &objective->maxState);
	if (objective->maxState <= 0)
	{
		objective->maxState = MAX_STATES_PER_OBJECTIVE - 1;
	}

	for (i = 0; i < MAX_STATES_PER_OBJECTIVE; i++)
	{
		int varnum;
		char* message;
		char* keyPrefix;
		int keyPrefixSize;

		G_SpawnString(va("message%d", i), "", &message);
		objective->messages[i] = G_NewString(message);

		keyPrefix = va("target%d", i);
		keyPrefixSize = strlen(keyPrefix);
		for (varnum = 0; varnum < level.numSpawnVars; varnum++)
		{
			objectiveTarget_t* target;

			if (Q_stricmpn(level.spawnVars[varnum][0], keyPrefix, keyPrefixSize) != 0) continue;
			if
				(
				level.spawnVars[varnum][0][keyPrefixSize] != 0 &&
				level.spawnVars[varnum][0][keyPrefixSize] != '_'
				)
			{
				continue;
			}

			target = &objective->targets[objective->numTargets++];
			target->state = i;
			target->target = G_NewString(level.spawnVars[varnum][1]);
		}
	}

	AddObjectiveToSortedList(objective);
	objectivesRequireUpdate = qtrue;
}

void G_UpdateObjectives()
{
	int i;
	int o;

	if (!objectivesRequireUpdate) return;

	o = 0;
	for (i = 0; i < MAX_VIS_OBJECTIVES; i++)
	{
		for (; o < numSortedObjectives; o++)
		{
			if (sortedObjectives[o]->state >= MAX_STATES_PER_OBJECTIVE) continue;
			if (sortedObjectives[o]->messages[sortedObjectives[o]->state][0]) break;
		}
		if (o >= numSortedObjectives)
		{
			trap_SetConfigstring(CS_OBJECTIVES + i, "");
		}
		else
		{
			trap_SetConfigstring(CS_OBJECTIVES + i, sortedObjectives[o]->messages[sortedObjectives[o]->state]);
			o++;
		}
	}

	objectivesUpdateId++;
	trap_SetConfigstring(CS_OBJECTIVES_UPDATE_ID, va("%d", objectivesUpdateId));

	objectivesRequireUpdate = qfalse;
}

qboolean G_IsObjectiveFulfilled(const char* name, int minstate, int maxstate)
{
	int i;

	for (i = 0; i < numObjectives; i++)
	{
		gentity_t* ent;
		
		ent = objectives[i].entity;
		if (Q_stricmp(ent->targetname, name) != 0) continue;

		return objectives[i].state >= minstate && objectives[i].state <= maxstate;
	}
	G_Error("Undefined objective '%s'", name);
	return qfalse;
}

#endif

//newq3ball
/*QUAKED target_counter (1.0 0 0) (-4 -4 -4) (4 4 4) x x x x x x x x
Acts as an intermediary for an action that takes multiple inputs.

After the counter has been triggered "count" times (default 2), it will fire all of it's targets and remove itself.
*/

void target_counter_use( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	if ( self->count == 0 )
	{
		return;
	}
	
	self->count--;

	if ( self->count )
	{
		return;
	}
	
	self->activator = activator;
	G_UseTargets( self, activator );
}

void SP_target_counter (gentity_t *self)
{
	self->wait = -1;
	if (!self->count)
	{
		self->count = 2;
	}

	self->use = target_counter_use;
}
//endnew


static void target_timer_start_use( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	gentity_t *checks;
	int setMask;
	if (!activator->client )
		return;
	setMask = 1 << (activator - g_entities);
	activator->client->ps.stats[STAT_RACE_CHECKPOINT] = 1;
	//TODO Maybe use the pmove time but okay
	activator->client->ps.powerups[PW_RACE_TIME] = level.time;
	/* Clear all checkpoints */
	checks = level.raceTimeCheck;
	while ( checks ) {
		checks->raceMask |= setMask;
		checks = checks->chain;
	}
}

static void target_timer_end_use( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	int sec, msec;
	if (!activator->client )
		return;
	if (!activator->client->ps.powerups[PW_RACE_TIME])
		return;
	if (activator->client->ps.stats[STAT_RACE_CHECKPOINT] < 0)
		return;

	activator->client->ps.stats[STAT_RACE_CHECKPOINT] = -1;
	//TODO Maybe use the pmove time but okay
	sec = level.time - activator->client->ps.powerups[PW_RACE_TIME];
	activator->client->ps.powerups[PW_RACE_TIME] = sec;
	msec = sec % 1000;
	sec /= 1000;
	trap_SendServerCommand( activator-g_entities, 
		va("cp \"You finished in %d.%03d\"", sec, msec ));
}

static void target_timer_check_use( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	int clientMask;
	playerState_t *ps;
	if ( !activator->client )
		return;
	ps = &activator->client->ps;
	/* Check if we have a valid start time */
	if ( ps->powerups[PW_RACE_TIME] <= 0 )
		return;
	clientMask = 1 << (activator - g_entities);
	/* Check if we already triggered this check once, if so no go */
	if ( !( self->raceMask & clientMask ))
		return;
	self->raceMask &= ~clientMask;
	trap_SendServerCommand( activator-g_entities, 
		va("cp \"You reached a checkpoint\""));
	/* Check if this needs a counter check */
	if ( !self->count ) {
		/* Just increase the counter to signal the cgame of an event */
		ps->powerups[PW_RACE_TIME]++;
		return;
	}
	/* Check if we should already be at this checkpoint, else reset time */
	if ( ps->stats[STAT_RACE_CHECKPOINT] < self->count ) {
		trap_SendServerCommand( activator-g_entities, 
			va("cp \"You skipped a checkpoint!\""));
		ps->powerups[ PW_RACE_TIME ] = 0;
		ps->stats[ STAT_RACE_CHECKPOINT ] = 0;
		return;
	}
	/* If this is the current checkpoint, increase the counter for the next one */
	if ( ps->stats[STAT_RACE_CHECKPOINT] == self->count) {
		ps->stats[STAT_RACE_CHECKPOINT]++;
	}
}

void SP_target_timer_start(gentity_t *self) {
	self->use = target_timer_start_use;
}
void SP_target_timer_end (gentity_t *self) {
	self->use = target_timer_end_use;
	//Count should be read in by default
}
void SP_target_timer_check(gentity_t *self) {
	self->use = target_timer_check_use;
	//Count should be read in by default
	self->chain = level.raceTimeCheck;
	level.raceTimeCheck = self;
}