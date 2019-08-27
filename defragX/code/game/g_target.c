// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "g_local.h"

//==========================================================

/*QUAKED target_startTimer (1 0 0) (-8 -8 -8) (8 8 8)
Start timer
*/
void Use_Target_StartTimer( gentity_t *ent, gentity_t *other, gentity_t *activator )
{
    extern int pm_reversemap;
    int secretFound, secretCount;

	if ( !activator->client)
		return;

    if ( pm_reversemap == 1 ) {

        if ( !activator->client->dfTimerOn )
            return;

        //set number of secrets to persistant so it can be displayed in the client side scoreboard. If user persistant secretcount already
        //contains a secretcount from a previous level, add that to the secretcount of this level.
        secretFound = (activator->client->ps.persistant[PERS_SECRETS] & 0x7F);
        secretCount = ((activator->client->ps.persistant[PERS_SECRETS] >> 7) & 0x7F) + level.secretCount;
        activator->client->ps.persistant[PERS_SECRETS] = secretFound + (secretCount << 7);

        DF_Timer_Stop( activator );
    } else {
        DF_Timer_Start( activator );
    }
}

void SP_target_startTimer( gentity_t *ent )
{
   ent->use = Use_Target_StartTimer;
}


// set fraglimit if a fragsfilter points to a stoptimer
void Init_Target_StopTimer( gentity_t *ent ) {
	gentity_t *t;

	while ( (t = G_Find (t, FOFS(target), ent->targetname)) != NULL ) {
		if ( !Q_stricmp( t->classname, "target_fragsFilter" ) ) {
			trap_Cvar_Set( "fraglimit", va("%d", t->count) );
			break;
		}
	}
}

/*QUAKED target_stopTimer (1 0 0) (-8 -8 -8) (8 8 8)
Stop timer
*/
void Use_Target_StopTimer( gentity_t *ent, gentity_t *other, gentity_t *activator )
{
    extern int pm_reversemap;
    int secretFound, secretCount;

	if ( !activator->client )
		return;

    if ( pm_reversemap == 1 ) {

        DF_Timer_Start( activator );

    } else {

        if ( !activator->client->dfTimerOn )
            return;

        //set number of secrets to persistant so it can be displayed in the client side scoreboard. If user persistant secretcount already
        //contains a secretcount from a previous level, add that to the secretcount of this level.
        secretFound = (activator->client->ps.persistant[PERS_SECRETS] & 0x7F);
        secretCount = ((activator->client->ps.persistant[PERS_SECRETS] >> 7) & 0x7F) + level.secretCount;
        activator->client->ps.persistant[PERS_SECRETS] = secretFound + (secretCount << 7);

        DF_Timer_Stop( activator );
    }
}

void SP_target_stopTimer( gentity_t *ent )
{
	ent->use = Use_Target_StopTimer;
    ent->think = Init_Target_StopTimer;
	ent->nextthink = level.time+200;
}

/*QUAKED target_checkpoint (1 0 0) (-8 -8 -8) (8 8 8)
Checkpoint
*/
void Use_Target_Checkpoint( gentity_t *ent, gentity_t *other, gentity_t *activator )
{
	int time;

	if ( !activator->client || !activator->client->dfTimerOn )
		return;

	time = level.time - activator->client->dfTimer;
	trap_SendServerCommand( activator - g_entities, va("checkpoint %d", time) );
}

void SP_target_checkpoint( gentity_t *ent )
{
	ent->use = Use_Target_Checkpoint;
}


/*QUAKED target_init (1 0 0) (-8 -8 -8) (8 8 8) KEEPARMOR KEEPHEALTH KEEPWEAPONS KEEPPOWERUPS KEEPHOLDABLE REMOVEMACHINEGUN
Removes various items from player.
-------- SPAWNFLAGS --------
KEEPARMOR
KEEPHEALTH
KEEPWEAPONS
KEEPPOWERUPS
KEEPHOLDABLE
REMOVEMACHINEGUN
-------- NOTES --------
REMOVEMACHINEGUN has no effect if KEEPWEAPONS is ON.
This target was originally settled for teleporters that lead back to the map starting point.
target_init fires its target when activated.
*/
void Use_Target_Init( gentity_t *ent, gentity_t *other, gentity_t *activator )
{
	gclient_t *client;

	if ( !activator->client )
		return;

	client = activator->client;

	if ( !(ent->spawnflags & 1) )
		client->ps.stats[STAT_ARMOR] = 0;

	if ( !(ent->spawnflags & 2) )
		activator->health = client->ps.stats[STAT_HEALTH] =	client->ps.stats[STAT_MAX_HEALTH] + 25;

	if ( !(ent->spawnflags & 4) ) {
		if ( dfx_gametype.integer == GT_ADVENTURE ) {
			client->ps.stats[STAT_WEAPONS]       = (1<<WP_GAUNTLET);
			client->ps.ammo[WP_MACHINEGUN]       = 0;
		} else {
			client->ps.stats[STAT_WEAPONS]       = 6;
			client->ps.ammo[WP_MACHINEGUN]       = 100;
		}
		client->ps.ammo[WP_GAUNTLET]         = -1;
		client->ps.ammo[WP_GRAPPLING_HOOK]   = -1;
		client->ps.ammo[WP_SHOTGUN]          = 0;
		client->ps.ammo[WP_GRENADE_LAUNCHER] = 0;
		client->ps.ammo[WP_ROCKET_LAUNCHER]  = 0;
		client->ps.ammo[WP_LIGHTNING]        = 0;
		client->ps.ammo[WP_RAILGUN]          = 0;
		client->ps.ammo[WP_PLASMAGUN]        = 0;
		client->ps.ammo[WP_BFG]              = 0;
		
	}

	if ( !(ent->spawnflags & 8) )
		memset( client->ps.powerups, 0, sizeof(client->ps.powerups) );

	if ( !(ent->spawnflags & 16) )
		client->ps.stats[STAT_HOLDABLE_ITEM] = 0;

	if ( ent->spawnflags & 32 && !(ent->spawnflags & 4) ){
		client->ps.stats[STAT_WEAPONS] &= ~(1 << WP_MACHINEGUN);
		client->ps.ammo[WP_MACHINEGUN] = 0;
	}

	G_UseTargets( ent, activator );
}

void SP_target_init( gentity_t *ent )
{
	ent->use = Use_Target_Init;
}


//==========================================================

/*QUAKED target_earthquake (.5 .5 .5) (-8 -8 -8) (8 8 8)
starts earthquake
"length" - length in  seconds (2-32, in steps of 2)
"intensity" - strength of earthquake (1-16)
*/

void Use_Target_Earthquake (gentity_t *self, gentity_t *other, gentity_t *activator) {
	G_AddEvent(self, EV_EARTHQUAKE, self->s.generic1);
}

void SP_target_earthquake (gentity_t *self) {
	int param;
	float length;		// length in seconds (2 to 32)
	float intensity;	// intensity (1 to 16)
	int length_;
	int intensity_;

	// read parameters
	G_SpawnFloat( "length", "1000", &length );
	G_SpawnFloat( "intensity", "50", &intensity );
	if ( length < 2 ) length = 2;
	if ( length > 32 ) length = 32;
	if ( intensity < 1 ) intensity = 1;
	if ( intensity > 16 ) intensity = 16;

	// adjust parameters
	length_ = ((int)(length) - 2) / 2;
	intensity_ = (int)intensity - 1;
	param = ( intensity_ | (length_ << 4) );
	self->s.generic1 = param;
	self->use = Use_Target_Earthquake;
	self->s.eType = ET_EVENTS;
	trap_LinkEntity (self);
}

//==========================================================

/*QUAKED target_effect (.5 .5 .5) (-8 -8 -8) (8 8 8) EXPLOSION PARTICLES_GRAVITY PARTICLES_LINEAR PARTICLES_LINEAR_UP PARTICLES_LINEAR_DOWN
shows animated environmental effect
The EXPLOSION spawnflag will cause the entity to show an explosion
The PARTICLES_GRAVITY spawnflag will cause the entity to show particles which are affected by gravity and drop to the ground
The PARTICLES_LINEAR spawnflag will cause the entity to show particles which are not affected by gravity and move in a straight line
*/

void Use_Target_Effect (gentity_t *self, gentity_t *other, gentity_t *activator) {
	gentity_t	*ent;
	gentity_t	*ent2;
	gentity_t	*ent3;
	gentity_t	*ent4;
	gentity_t	*ent5;

	//set default values
	if ( !self->count )
		self->count = 100;
	else if (self->count > 255)
		self->count = 255;

	if ( !self->speed )
		self->speed = 100;

	//explosion
	if ( self->spawnflags & 1 )
		ent = G_TempEntity( self->s.origin, EV_EXPLOSION );

	//particles_gravity
	if ( self->spawnflags & 2 ) {
		ent2 = G_TempEntity( self->s.origin, EV_PARTICLES_GRAVITY );
		ent2->s.constantLight = self->s.constantLight;	//constantLight is used to determine particle color
		ent2->s.eventParm = self->count; //eventParm is used to determine the number of particles
		ent2->s.generic1 = self->speed; //generic1 is used to determine the speed of the particles
	}

	//particles_linear
	if ( self->spawnflags & 4 ) {
		ent3 = G_TempEntity( self->s.origin, EV_PARTICLES_LINEAR );
		ent3->s.constantLight = self->s.constantLight;	//constantLight is used to determine particle color
		ent3->s.eventParm = self->count; //eventParm is used to determine the number of particles
		ent3->s.generic1 = self->speed; //generic1 is used to determine the speed of the particles
	}

	//particles_linear_up
	if ( self->spawnflags & 8 ) {
		ent4 = G_TempEntity( self->s.origin, EV_PARTICLES_LINEAR_UP );
		ent4->s.constantLight = self->s.constantLight;	//constantLight is used to determine particle color
		ent4->s.eventParm = self->count; //eventParm is used to determine the number of particles
		ent4->s.generic1 = self->speed; //generic1 is used to determine the speed of the particles
	}

	//particles_linear_down
	if ( self->spawnflags & 16 ) {
		ent5 = G_TempEntity( self->s.origin, EV_PARTICLES_LINEAR_DOWN );
		ent5->s.constantLight = self->s.constantLight;	//constantLight is used to determine particle color
		ent5->s.eventParm = self->count; //eventParm is used to determine the number of particles
		ent5->s.generic1 = self->speed; //generic1 is used to determine the speed of the particles
	}

}

void SP_target_effect (gentity_t *self) {
	vec3_t		color;
	int			r, g, b;

    //check if effects are selected
	if ( !self->spawnflags ) {
		G_Printf( va( S_COLOR_YELLOW "WARNING: target_effect without selected effects at %s\n", vtos(self->s.origin) ) );
		G_FreeEntity( self );
	}

	// particle color
	G_SpawnVector( "color", "1 1 1", color );

	r = color[0] * 255;
	if (r > 255) r = 255;

	g = color[1] * 255;
	if (g > 255) g = 255;

	b = color[2] * 255;
	if (b > 255) b = 255;

	self->s.constantLight = r + (g << 8) + (b << 16);

	//preload assets if necessary
	if ( self->spawnflags & 1 )
		RegisterItem( BG_FindItemForWeapon( WP_ROCKET_LAUNCHER ) );	//uses RL gfx so we must register the RL

	self->use = Use_Target_Effect;
}

//==========================================================
/*QUAKED target_speed (1 1 0) (-8 -8 -8) (8 8 8) PERCENTAGE ADD +X -X +Y -Y +Z -Z LAUNCHER
Sets the speed of the player.
If ADD is speficied, negative values will reduce the speed.
-------- KEYS --------
targetname: Activating trigger points to this.
speed: Speed value to set (default: 100).
-------- SPAWNFLAGS --------
PERCENTAGE: value is treated as percentage of current speed
ADD: adds the speed instead of setting it
+X: takes positive X-direction into account
-X: takes negative X-direction into account
+Y: takes positive Y-direction into account
-Y: takes negative Y-direction into account
+Z: takes positive Z-direction (up) into account
-Z: takes negative Z-direction (down) into account
LAUNCHER: will accelerate the player into the given direction (spawnflags)
-------- NOTES --------
If LAUNCHER isn't specified the player will only be accelerated if he is moving
while the target_speed is triggered, as the player needs a moving direction and this is
only given when he moves. LAUNCHER will split the given speed value to the given
directions. Note that only one of the spawnflags for each coordinate must be set. For
example if +X and -X is active the x-direction will be ignored. The same is the case
if whether +X nor -X is active. So if you want to accelerate the player up with 900u
you have to set the speed value to 900 and activate the spawnflags +Z and LAUNCHER.
*/

#ifndef MAX
#define MAX(x,y) ((x)>(y)?(x):(y))
#endif

void Use_Target_Speed( gentity_t *ent, gentity_t *other, gentity_t *activator )
{
	qbool plus[3];
	qbool minus[3];

	vec3_t var_28 = { 1.0f, 1.0f, 1.0f };
	vec3_t var_34 = { 1.0f, 1.0f, 1.0f };
	vec3_t speeds;
	float mag;

	vec3_t velocity, signs;
	int i;

	if ( !activator->client )
		return;

	VectorCopy( activator->client->ps.velocity, velocity );

	for ( i = 0; i < 3; i++ ) {
		plus[i] = (ent->spawnflags & (1 << (i*2 + 2))) != 0;  // spawnflags & 4, 16, 64
		minus[i] = (ent->spawnflags & (1 << (i*2 + 3))) != 0; // spawnflags & 8, 32, 128

		if ( !(plus[i] || minus[i]) )
			velocity[i] = 0.0f;

		if ( fabs( velocity[i] ) >= 0.00001 )
			signs[i] = velocity[i] / fabs( velocity[i] );
		 else
			signs[i] = 0.0f;

		if ( plus[i] && minus[i] ) {
			var_28[i] = 1.0f;
			var_34[i] = signs[i];
		} else if ( plus[i] ) {
			var_28[i] = signs[i];
			var_34[i] = 1.0f;
		} else if ( minus[i] ) {
			var_28[i] = -signs[i];
			var_34[i] = -1.0f;
		}
	}

	mag = VectorLength( velocity );
	speeds[0] = speeds[1] = speeds[2] = mag;

	for ( i = 0; i < 3; i++ ) {
		if ( ent->spawnflags & 2 ) { // add
			if ( ent->spawnflags & 256 )  // launcher
				var_28[i] = 1.0f;


			if ( ent->spawnflags & 1 )  // percentage
				speeds[i] += var_28[i] * speeds[i] * ent->speed * 0.01f;
            else
				speeds[i] += var_28[i] * ent->speed;

		} else {
			if ( ent->speed < 0 )
				ent->speed = 0;

			if ( ent->spawnflags & 1 )  // percentage
				speeds[i] *= ent->speed * 0.01f;
			 else
				speeds[i] = ent->speed;

			speeds[i] *= var_34[i];

			if ( fabs( velocity[i] ) > 0.00001 )
				velocity[i] = fabs( velocity[i] );
		}
	}

	if ( fabs( mag ) > 0.00001 ) {
		velocity[0] /= fabs( mag );
		velocity[1] /= fabs( mag );
		velocity[2] /= fabs( mag );
	}

	if ( ent->spawnflags & 256 ) { // launcher
		int num_adds = 0;

		if ( plus[0] ^ minus[0] )
			num_adds++;

		if ( plus[1] ^ minus[1] )
			num_adds++;

		if ( plus[2] ^ minus[2] )
			num_adds++;

		if ( num_adds != 0 ) {
			mag = MAX( MAX( fabs(speeds[0]), fabs(speeds[1]) ), fabs(speeds[2]) );
			mag = sqrt( mag * mag * num_adds );
			for ( i = 0; i < 3; i++ ) {
				if ( plus[i] ^ minus[i] ) {
					if ( plus[i] )
						activator->client->ps.velocity[i] = mag;
					 else
						activator->client->ps.velocity[i] = -mag;

				}
			}
		}
	} else {
		for ( i = 0; i < 3; i++ ) {
			if ( plus[i] || minus[i] )
				activator->client->ps.velocity[i] = velocity[i] * speeds[i];

		}
	}
}

void SP_target_speed( gentity_t *ent )
{
	if ( !ent->speed )
		ent->speed = 100;

	ent->use = Use_Target_Speed;
}


/*QUAKED target_fragsFilter (1 0 0) (-8 -8 -8) (8 8 8) REMOVER RUNONCE SILENT RESET MATCH
Frags Filter
-------- KEYS --------
frags: (default is 1) number of frags required to trigger the targeted entity.
target: targeted entity.
-------- SPAWNFLAGS --------
REMOVER: removes from player's score the number of frags that was required to trigger the targeted entity.
RUNONCE: no longer used, kept for compatibility.
SILENT: disables player warnings. ("x more frags needed" messages)
RESET: resets player's score to 0 after the targeted entity is triggered.
MATCH: the player's score must be exactly equal to the frags value.
-------- NOTES --------
If the Frags Filter is not bound from a trigger, it becomes independant and is so always active.
Defrag is limited to 10 independant target_fragsFilter.
*/

gentity_t *global_fragsFilters[10] = {0};

void Use_Target_FragsFilter( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	int score;

	if ( !activator->client ) {
		return;
	}

	score = activator->client->ps.persistant[PERS_SCORE];

	// MATCH
	if ( (ent->spawnflags & 16) && score != ent->count ) {
		return;
	}

	if ( score >= ent->count ) {
		G_UseTargets( ent, activator );

		if ( ent->spawnflags & 1 ) {            // REMOVER
			AddScore( activator, activator->r.currentOrigin, -ent->count );
		} else if ( ent->spawnflags & 8 ) {     // RESET
			AddScore( activator, activator->r.currentOrigin, -score );
		}
	} else {
		if ( !(ent->spawnflags & 4) ) {         //!SILENT
			int left = ent->count - score;
			trap_SendServerCommand( activator-g_entities, va("cp \"%d frag%s left\"", left, left == 1 ? "" : "s") );
		}
	}
}

void SP_target_fragsFilter( gentity_t *ent ) {
	G_SpawnInt( "frags", "1", &ent->count );
	ent->use = Use_Target_FragsFilter;

	// untargeted target_fragsFilters are global
	if ( !ent->targetname ) {
		int i;

		ent->spawnflags |= 4; //global fragsfilters are silent

		for( i = 0; i < ARRAY_LEN(global_fragsFilters); i++ ) {
			if ( !global_fragsFilters[i] ) {
				global_fragsFilters[i] = ent;
				break;
			}
		}
	}
}




/*QUAKED target_give (1 0 0) (-8 -8 -8) (8 8 8)
Gives the activator all the items pointed to.
*/
void Use_Target_Give( gentity_t *ent, gentity_t *other, gentity_t *activator )
{
	gentity_t	*t;
	trace_t		trace;

	if ( (!activator->client) || (!ent->target) )
		return;

	memset( &trace, 0, sizeof( trace ) );
	t = NULL;
	while ( (t = G_Find (t, FOFS(targetname), ent->target)) != NULL )
    {
		if ( !t->item )
			continue;

		t->waittable[activator->client->ps.clientNum] = 0;

		Touch_Item( t, activator, &trace );

		// make sure it isn't going to respawn or show any events
		t->tag = TAG_DONTSPAWN;
		t->nextthink = 0;
		trap_UnlinkEntity( t );
	}
}

void SP_target_give( gentity_t *ent )
{
	ent->use = Use_Target_Give;
}


//==========================================================

/*QUAKED target_remove_powerups (1 0 0) (-8 -8 -8) (8 8 8)
takes away all the activators powerups.
Used to drop flight powerups into death puts.
*/
void Use_Target_Remove_Powerups( gentity_t *ent, gentity_t *other, gentity_t *activator )
{
	if( !activator->client )
		return;

	memset( activator->client->ps.powerups, 0, sizeof( activator->client->ps.powerups ) );
}

void SP_target_remove_powerups( gentity_t *ent )
{
	ent->use = Use_Target_Remove_Powerups;
}


//==========================================================

/*QUAKED target_delay (1 0 0) (-8 -8 -8) (8 8 8)
"wait" seconds to pause before firing targets.
"random" delay variance, total delay = delay +/- random seconds
*/
void Think_Target_Delay( gentity_t *ent )
{
	G_UseTargets( ent, ent->activator );
}

void Use_Target_Delay( gentity_t *ent, gentity_t *other, gentity_t *activator )
{
	ent->nextthink = level.time + ( ent->wait + ent->random * crandom() ) * 1000;
	ent->think = Think_Target_Delay;
	ent->activator = activator;
}

void SP_target_delay( gentity_t *ent )
{
	// check delay for backwards compatability
	if ( !G_SpawnFloat( "delay", "0", &ent->wait ) )
		G_SpawnFloat( "wait", "1", &ent->wait );

	if ( !ent->wait )
		ent->wait = 1;

	ent->use = Use_Target_Delay;
}


//==========================================================

/*QUAKED target_score (1 0 0) (-8 -8 -8) (8 8 8)
"count" number of points to add, default 1

The activator is given this many points.
*/
void Use_Target_Score (gentity_t *ent, gentity_t *other, gentity_t *activator) {

    int i;

	if ( !activator )
        return;

	AddScore( activator, ent->r.currentOrigin, ent->count );

	for ( i = 0; i < ARRAY_LEN(global_fragsFilters); i++ ) {
		if ( global_fragsFilters[i] ) {
			global_fragsFilters[i]->use( global_fragsFilters[i], NULL, activator );
		}
	}

}

void SP_target_score( gentity_t *ent ) {

	if ( !ent->count )
		ent->count = 1;

	ent->use = Use_Target_Score;
}


//==========================================================

/*QUAKED target_secret (.5 .5 .5) (-8 -8 -8) (8 8 8) SILENT
When triggered, marks the secret as 'found'
*/

void Use_Target_Secret (gentity_t *self, gentity_t *other, gentity_t *activator) {
	activator->client->ps.persistant[PERS_SECRETS]++;

	if ( !(self->spawnflags & 1) ) {
		if ( self->message )
			trap_SendServerCommand( -1, va("cp \"%s\"", self->message ));
		else
			trap_SendServerCommand( -1, va("cp \"%s\"", "You found a secret!" ));
	}

	//remove the entity so it cannot be triggered again
	G_FreeEntity( self );
}

void SP_target_secret (gentity_t *self) {

	//register the secret
	level.secretCount++;

	self->use = Use_Target_Secret;
}


//==========================================================

void Use_Target_Print (gentity_t *ent, gentity_t *other, gentity_t *activator)
{
	if ( dfx_gametype.integer == GT_DEFRAG || dfx_gametype.integer == GT_ADVENTURE ) {
		if ( ent->spawnflags & 8 ) {
			trap_SendServerCommand( -1, va("cpsm \"%s\"", ent->message ));
		} else {
			trap_SendServerCommand( activator-g_entities, va("cpsm \"%s\"", ent->message ));
		}
		return;
	}

	if ( activator->client && ( ent->spawnflags & 4 ) ) {
		trap_SendServerCommand( activator-g_entities, va("cp \"%s\"", ent->message ));
		return;
	}

	trap_SendServerCommand( -1, va("cp \"%s\"", ent->message ));
}

void SP_target_print( gentity_t *ent )
{
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

void Use_Target_Speaker (gentity_t *ent, gentity_t *other, gentity_t *activator)
{
	if (ent->spawnflags & 3)
    {	// looping sound toggles

		if (ent->s.loopSound)
        {
            ent->s.loopSound = 0;	// turn it off
        }

		else

        {
            ent->s.loopSound = ent->noise_index;	// start it
        }
	}

	else

    {	// normal sound
		if ( ent->spawnflags & 8 && activator )
		{
			G_AddEvent( activator, EV_GENERAL_SOUND, ent->noise_index );
		}

        else if (ent->spawnflags & 4)

        {
			G_AddEvent( ent, EV_GLOBAL_SOUND, ent->noise_index );
		}

        else

        {
			G_AddEvent( ent, EV_GENERAL_SOUND, ent->noise_index );
		}

	}
}

void SP_target_speaker( gentity_t *ent )
{
	char	buffer[MAX_QPATH];
	char	*s;

	G_SpawnFloat( "wait", "0", &ent->wait );
	G_SpawnFloat( "random", "0", &ent->random );

	if ( !G_SpawnString( "noise", "NOSOUND", &s ) )
    {
		G_Error( "target_speaker without a noise key at %s", vtos( ent->s.origin ) );
	}

	// force all client relative sounds to be "activator" speakers that
	// play on the entity that activates it
	if ( s[0] == '*' )
    {
		ent->spawnflags |= 8;
	}

	if (!strstr( s, ".wav" ))
    {
		Com_sprintf (buffer, sizeof(buffer), "%s.wav", s );
	}
	else
    {
		Q_strncpyz( buffer, s, sizeof(buffer) );
	}
	ent->noise_index = G_SoundIndex(buffer);

	// a repeating speaker can be done completely client side
	ent->s.eType = ET_SPEAKER;
	ent->s.eventParm = ent->noise_index;
	ent->s.frame = ent->wait * 10;
	ent->s.clientNum = ent->random * 10;


	// check for prestarted looping sound
	if ( ent->spawnflags & 1 )
    {
        ent->s.loopSound = ent->noise_index;
    }
	else
    {
        ent->s.loopSound = 0;
    }

	ent->use = Use_Target_Speaker;

	if (ent->spawnflags & 4)
    {
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

void Think_Target_Laser (gentity_t *self)
{
	vec3_t	start, end;
	trace_t	tr;
	vec3_t	point;
	int     tracenum = 0;

	// if pointed at another entity, set movedir to point at it
	if ( self->enemy )
    {
		VectorMA (self->enemy->s.origin, 0.5, self->enemy->r.mins, point);
		VectorMA (point, 0.5, self->enemy->r.maxs, point);
		VectorSubtract (point, self->s.origin, self->movedir);
		VectorNormalize (self->movedir);
	}

	// fire forward and see what we hit
	VectorCopy (self->s.origin, start);
	VectorMA (self->s.origin, 2048, self->movedir, end);

	do
    {
		trap_Trace( &tr, start, NULL, NULL, end, self->s.number, CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE);
		VectorMA( tr.endpos, 1, self->movedir, start );
	}
	while ( tracenum++ < 32 && tr.contents & CONTENTS_TRANSLUCENT ); // lasers go through transparent objects

	if ( tr.entityNum != ENTITYNUM_NONE )
    {
		// hurt it if we can
		G_Damage ( &g_entities[tr.entityNum], self, self, self->movedir,
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

	Think_Target_Laser (self);
}

void target_laser_off (gentity_t *self)
{
	trap_UnlinkEntity( self );
	self->nextthink = 0;
}

void Use_Target_Laser (gentity_t *self, gentity_t *other, gentity_t *activator)
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

	if (self->target)
    {
		ent = G_Find (NULL, FOFS(targetname), self->target);
		if (!ent)
		{
			G_Printf ("%s at %s: %s is a bad target\n", self->classname, vtos(self->s.origin), self->target);
		}

		self->enemy = ent;

	}
	else
    {
		G_SetMovedir (self->s.angles, self->movedir);
	}

	self->use = Use_Target_Laser;
	self->think = Think_Target_Laser;

	if ( !self->damage )
		self->damage = 1;

	if (self->spawnflags & 1)
        target_laser_on (self);
	else
        target_laser_off (self);
}

void SP_target_laser (gentity_t *self)
{
     char   *sound;
     vec3_t	color;
	 int	r, g, b;

     // SLK: if the "noise" key is set, use a constant looping sound when active
	if (G_SpawnString("noise", "100", &sound))
		self->s.loopSound = G_SoundIndex(sound);

	// let everything else get spawned before we start firing
	self->think = target_laser_start;
	self->nextthink = level.time + FRAMETIME;

	//beam color
	G_SpawnVector("color", "1 0 0", color);

	r = color[0] * 255;
	if (r > 255)
		r = 255;

	g = color[1] * 255;
	if (g > 255)
		g = 255;

	b = color[2] * 255;
	if (b > 255)
		b = 255;

	self->s.constantLight = r | (g << 8) | (b << 16);
}


//==========================================================


/*QUAKED target_teleporter (1 0 0) (-8 -8 -8) (8 8 8)
The activator will be teleported away.
*/

void Use_Target_Teleporter( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	gentity_t	*dest;
	extern int pm_noteles;

	if ( (!activator) || (!activator->client) || pm_noteles == 1 )
        return;

	dest = 	G_PickTarget( self->target );
	if (!dest)
    {
		G_Printf ("Couldn't find teleporter destination\n");
		return;
	}

	TeleportPlayer( activator, dest->s.origin, dest->s.angles, self->spawnflags & 1 );
}

void SP_target_teleporter( gentity_t *self )
{
	if (!self->targetname)
    {
        G_Printf("untargeted %s at %s\n", self->classname, vtos(self->s.origin));
    }

	self->use = Use_Target_Teleporter;
}

//==========================================================


/*QUAKED target_relay (.5 .5 .5) (-8 -8 -8) (8 8 8) RED_ONLY BLUE_ONLY RANDOM
This doesn't perform any actions except fire its targets.
The activator can be forced to be from a certain team.
if RANDOM is checked, only one of the targets will be fired, not all of them
*/
void Use_Target_Relay (gentity_t *self, gentity_t *other, gentity_t *activator)
{
	if ( self->spawnflags & 4 )
    {
		gentity_t	*ent;

		ent = G_PickTarget( self->target );
		if ( ent && ent->use )
			ent->use( ent, self, activator );

		return;
	}

	G_UseTargets (self, activator);
}

void SP_target_relay (gentity_t *self)
{
	self->use = Use_Target_Relay;
}


//==========================================================

/*QUAKED target_kill (.5 .5 .5) (-8 -8 -8) (8 8 8)
Kills the activator.
*/
void Use_Target_Kill( gentity_t *self, gentity_t *other, gentity_t *activator )
{
	if ( !activator )
        return;

	G_Damage ( activator, NULL, NULL, NULL, NULL, 100000, DAMAGE_NO_PROTECTION, MOD_TELEFRAG);
}

void SP_target_kill( gentity_t *self )
{
	self->use = Use_Target_Kill;
}

/*QUAKED target_position (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for in-game calculation, like jump pad targets.
*/
void SP_target_position( gentity_t *self )
{
	G_SetOrigin( self, self->s.origin );
}

/*QUAKED target_music (0 .7 .7) (-8 -8 -8) (8 8 8)
When triggered, starts playing specified music track
*/
void Use_Target_Music (gentity_t *self, gentity_t *other, gentity_t *activator) {
    trap_SetConfigstring( CS_MUSIC, self->music );
}

void SP_target_music (gentity_t *self) {
    char	*s;
	char	buffer[MAX_QPATH];

	G_SpawnString( "music", "", &s );
	Q_strncpyz( self->music, s, sizeof(self->music) );

	self->use = Use_Target_Music;
}
