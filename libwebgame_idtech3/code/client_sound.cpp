/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2005 Stuart Dalton (badcdev@gmail.com)

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
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include "client.h"
#include "client_sound.h"

typedef struct alSfx_s
{
	char			filename[MAX_QPATH];
	int		buffer;					// OpenAL buffer

	qboolean	isDefault;				// Couldn't be loaded - use default FX
	qboolean	isDefaultChecked;		// Sound has been check if it isDefault
	qboolean	inMemory;				// Sound is stored in memory
	qboolean	isLocked;				// Sound is locked (can not be unloaded)
	int				lastUsedTime;		// Time last used

	int				loopCnt;		// number of loops using this sfx
	int				loopActiveCnt;		// number of playing loops using this sfx
	int				masterLoopSrc;		// All other sources looping this buffer are synced to this master src
} alSfx_t;

#define MAX_SFX 4096

EXTERNC qboolean alBuffersInitialised;
EXTERNC alSfx_t knownSfx[MAX_SFX];
EXTERNC sfxHandle_t numSfx;
EXTERNC sfxHandle_t default_sfx;


static qboolean enumeration_ext = qfalse;
static qboolean enumeration_all_ext = qfalse;

qboolean alBuffersInitialised = qfalse;
alSfx_t knownSfx[MAX_SFX];
sfxHandle_t numSfx = 0;
sfxHandle_t default_sfx;

typedef struct src_s
{
	int		alSource;		// OpenAL source object
	sfxHandle_t	sfx;			// Sound effect in use

	int		lastUsedTime;		// Last time used
	alSrcPriority_t	priority;		// Priority
	int		entity;			// Owning entity (-1 if none)
	int		channel;		// Associated channel (-1 if none)

	qboolean	isActive;		// Is this source currently in use?
	qboolean	isPlaying;		// Is this source currently playing, or stopped?
	qboolean	isLocked;		// This is locked (un-allocatable)
	qboolean	isLooping;		// Is this a looping effect (attached to an entity)
	qboolean	isTracking;		// Is this object tracking its owner
	qboolean	isStream;		// Is this source a stream

	float		curGain;		// gain employed if source is within maxdistance.
	float		scaleGain;		// Last gain value for this source. 0 if muted.
	
	float		lastTimePos;		// On stopped loops, the last position in the buffer
	int		lastSampleTime;		// Time when this was stopped
	vec3_t		loopSpeakerPos;		// Origin of the loop speaker
	
	qboolean	local;			// Is this local (relative to the cam)
} src_t;

#ifdef __APPLE__
	#define MAX_SRC 64
#else
	#define MAX_SRC 128
#endif
static src_t srcList[MAX_SRC];
static int srcCount = 0;
static int srcActiveCnt = 0;
static qboolean alSourcesInitialised = qfalse;
static int lastListenerNumber = -1;
static vec3_t lastListenerOrigin = { 0.0f, 0.0f, 0.0f };

typedef struct sentity_s
{
	vec3_t					origin;

	qboolean						srcAllocated; // If a src_t has been allocated to this entity
	int							srcIndex;

	qboolean				loopAddedThisFrame;
	alSrcPriority_t	loopPriority;
	sfxHandle_t			loopSfx;
	qboolean				startLoopingSound;
} sentity_t;

static sentity_t entityList[MAX_GENTITIES];

#define S_AL_SanitiseVector(v) S_AL_SanitiseVectorLine(v,__LINE__)
CCALL void S_AL_SanitiseVectorLine( vec3_t v, int line )
{
	if( Q_isnan( v[ 0 ] ) || Q_isnan( v[ 1 ] ) || Q_isnan( v[ 2 ] ) )
	{
		Com_Printf( S_COLOR_YELLOW "WARNING: vector with one or more NaN components "
				"being passed to OpenAL at %s:%d -- zeroing\n", __FILE__, line );
		VectorClear( v );
	}
}





cvar_t *s_volume;
cvar_t *s_muted;
cvar_t *s_musicVolume;
cvar_t *s_doppler;
cvar_t *s_backend;
cvar_t *s_muteWhenMinimized;
cvar_t *s_muteWhenUnfocused;

// Play a one-shot sound effect
// _S_AL_StartSound(0, entnum=0, entchannel=3, sfx=25)
// src = _S_AL_SrcAlloc(SRCPRI_ONESHOT=2, entnum, entchannel)
// _S_AL_StartSound(0, entnum=0, entchannel=0, sfx=112) // gun shot

void S_StartSound( vec3_t origin, int entnum, int entchannel, sfxHandle_t sfx )
{
	vec3_t sorigin;
	srcHandle_t src;
	src_t *curSource;




#ifdef EMSCRIPTEN
		EM_ASM_({
			callback_AL_StartSound($0, $1, $2, $3);
		}, origin, entnum, entchannel, sfx);
#endif

	//if(origin)
	//{
	//	if(S_AL_CheckInput(0, sfx))
	//		return;
	//	
	//	VectorCopy(origin, sorigin);
	//}
	//else
	//{
	//	if(S_AL_CheckInput(entnum, sfx))
	//		return;
	//
	//	if(S_AL_HearingThroughEntity(entnum))
	//	{
	//		S_AL_StartLocalSound(sfx, entchannel);
	//		return;
	//	}
	//	
	//	VectorCopy(entityList[entnum].origin, sorigin);
	//}
	//
	//S_AL_SanitiseVector(sorigin);
	//
	//if((srcActiveCnt > 5 * srcCount / 3) &&
	//	(DistanceSquared(sorigin, lastListenerOrigin) >=
	//	(s_alMaxDistance->value + s_alGraceDistance->value) * (s_alMaxDistance->value + s_alGraceDistance->value)))
	//{
	//	// We're getting tight on sources and source is not within hearing distance so don't add it
	//	return;
	//}
	//
	//// Try to grab a source
	//src = S_AL_SrcAlloc(SRCPRI_ONESHOT, entnum, entchannel);
	//if(src == -1)
	//	return;
	//
	//S_AL_SrcSetup(src, sfx, SRCPRI_ONESHOT, entnum, entchannel, qfalse);
	//
	//curSource = &srcList[src];
	//
	//if(!origin)
	//	curSource->isTracking = qtrue;
	//
	////imgui_log("AL_testsound(%u, %d, 1, %f, %f, %f)\n", curSource->alSource, S_AL_BufferGet(sfx), sorigin[0], sorigin[1], sorigin[2]);
	//
	//qalSourcefv(curSource->alSource, AL_POSITION, sorigin );
	//S_AL_ScaleGain(curSource, sorigin);
	//
	//// Start it playing
	//curSource->isPlaying = qtrue;
	//qalSourcePlay(curSource->alSource);
}

void S_StartLocalSound( sfxHandle_t sfx, int channelNum )
{

}

void S_StartBackgroundTrack( const char *intro, const char *loop )
{

}

void S_StopBackgroundTrack( void )
{

}

void S_RawSamples (int stream, int samples, int rate, int width, int channels, const byte *data, float volume, int entityNum) {
}

void S_StopAllSounds( void ) {
}

void S_ClearLoopingSounds( qboolean killall )
{

}

void S_AddLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx ) {
}

void S_AddRealLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx ) {
}

void S_StopLoopingSound( int entityNum ) {
}

void S_Respatialize( int entityNum, const vec3_t origin,
		vec3_t axis[3], int inwater )
{
//	float		orientation[6];
//	vec3_t	sorigin;
//
//	VectorCopy( origin, sorigin );
//	S_AL_SanitiseVector( sorigin );
//
//	S_AL_SanitiseVector( axis[ 0 ] );
//	S_AL_SanitiseVector( axis[ 1 ] );
//	S_AL_SanitiseVector( axis[ 2 ] );
//
//	// forward
//	orientation[0] = axis[0][0];
//	orientation[1] = axis[0][1];
//	orientation[2] = axis[0][2];
//	// up
//	orientation[3] = axis[2][0];
//	orientation[4] = axis[2][1];
//	orientation[5] = axis[2][2];
//
//	lastListenerNumber = entityNum;
//	VectorCopy( sorigin, lastListenerOrigin );
//
//// the openal emscripten implementation fucks this up, when i set pos of listener AND source, the source position ends up being 0,0,0
////#ifndef EMSCRIPTEN
//	// Set OpenAL listener paramaters
//	qalListenerfv(AL_POSITION, (ALfloat *)sorigin);
//	qalListenerfv(AL_VELOCITY, vec3_origin);
//	qalListenerfv(AL_ORIENTATION, orientation);
////#endif
}

/*
=================
S_UpdateEntityPosition
=================
*/
void S_UpdateEntityPosition( int entityNum, const vec3_t origin )
{
	//vec3_t sanOrigin;
	//
	//VectorCopy( origin, sanOrigin );
	//S_AL_SanitiseVector( sanOrigin );
	//if ( entityNum < 0 || entityNum >= MAX_GENTITIES )
	//	Com_Error( ERR_DROP, "S_UpdateEntityPosition: bad entitynum %i", entityNum );
	//VectorCopy( sanOrigin, entityList[entityNum].origin );
}

void S_Update( void )
{
	if(s_muted->integer)
	{
		if(!(s_muteWhenMinimized->integer && com_minimized->integer) &&
		   !(s_muteWhenUnfocused->integer && com_unfocused->integer))
		{
			s_muted->integer = qfalse;
			s_muted->modified = qtrue;
		}
	}
	else
	{
		if((s_muteWhenMinimized->integer && com_minimized->integer) ||
		   (s_muteWhenUnfocused->integer && com_unfocused->integer))
		{
			s_muted->integer = qtrue;
			s_muted->modified = qtrue;
		}
	}
	

}

void S_DisableSounds( void )
{
}


void S_BeginRegistration( void )
{
}

sfxHandle_t	S_RegisterSound( const char *sample, qboolean compressed ) {
	sfxHandle_t sfx;
#ifdef EMSCRIPTEN
		sfx = EM_ASM_INT({
			return callback_AL_RegisterSound($0);
		}, sample);
#endif
	return sfx;
}

void S_ClearSoundBuffer( void )
{
}


void S_SoundInfo( void )
{
	//qalGetString( 0 ); // for some reason my webassembly doesnt load anymore if this isnt called once...
	// LinkError: Import #75 module="env" function="_alGetString" error: function import requires a callable
}

/*
=================
S_SoundList
=================
*/
void S_SoundList( void )
{

}

void S_Play_f( void ) {

}

void S_Music_f( void ) {
}

void S_StopMusic_f( void )
{
}


void S_Init( void )
{
	cvar_t		*cv;
	qboolean	started = qfalse;

	Com_Printf( "------ Initializing Sound ------\n" );

	s_volume = Cvar_Get( "s_volume", "0.8", CVAR_ARCHIVE );
	s_musicVolume = Cvar_Get( "s_musicvolume", "0.25", CVAR_ARCHIVE );
	s_muted = Cvar_Get("s_muted", "0", CVAR_ROM);
	s_doppler = Cvar_Get( "s_doppler", "1", CVAR_ARCHIVE );
	s_backend = Cvar_Get( "s_backend", "", CVAR_ROM );
	s_muteWhenMinimized = Cvar_Get( "s_muteWhenMinimized", "0", CVAR_ARCHIVE );
	s_muteWhenUnfocused = Cvar_Get( "s_muteWhenUnfocused", "0", CVAR_ARCHIVE );

	S_SoundInfo();

}

void S_Shutdown( void )
{

}




//// Also see S_Base_HearingThroughEntity
//CCALL qboolean S_AL_HearingThroughEntity( int entityNum )
//{
//	//float	distanceSq;
//	//
//	//if( lastListenerNumber == entityNum )
//	//{
//	//	// This is an outrageous hack to detect
//	//	// whether or not the player is rendering in third person or not. We can't
//	//	// ask the renderer because the renderer has no notion of entities and we
//	//	// can't ask cgame since that would involve changing the API and hence mod
//	//	// compatibility. I don't think there is any way around this, but I'll leave
//	//	// the FIXME just in case anyone has a bright idea.
//	//	distanceSq = DistanceSquared(
//	//			entityList[ entityNum ].origin,
//	//			lastListenerOrigin );
//	//
//	//	if( distanceSq > THIRD_PERSON_THRESHOLD_SQ )
//	//		return qfalse; //we're the player, but third person
//	//	else
//	//		return qtrue;  //we're the player
//	//}
//	//else
//		return qfalse; //not the player
//}



//// Adapt the gain if necessary to get a quicker fadeout when the source is too far away.
//CCALL void S_AL_ScaleGain(src_t *chksrc, vec3_t origin)
//{
//	//float distance;
//	//
//	//if(!chksrc->local)
//	//	distance = Distance(origin, lastListenerOrigin);
//	//	
//	//// If we exceed a certain distance, scale the gain linearly until the sound
//	//// vanishes into nothingness.
//	//if(!chksrc->local/* && (distance -= s_alMaxDistance->value) > 0*/)
//	//{
//	//	//float scaleFactor;
//	//	//
//	//	//if(distance >= s_alGraceDistance->value)
//	//	//	scaleFactor = 0.0f;
//	//	//else
//	//	//	scaleFactor = 1.0f - distance / s_alGraceDistance->value;
//	//	//
//	//	//scaleFactor *= chksrc->curGain;
//	//	//
//	//	//if(chksrc->scaleGain != scaleFactor)
//	//	//{
//	//	//	chksrc->scaleGain = scaleFactor;
//	//	//	S_AL_Gain(chksrc->alSource, chksrc->scaleGain);
//	//	//}
//	//
//	//	// lets say max=1000, distance=800.... nearly at end... 0.2 is still loud as fuck tho...
//	//	// so we need a roll of curve
//	//	// the further away, the less gain i want
//	//	// so we multiply it again with 0.2 itself.... the smaller, the smaller its going to be again
//	//	float maxdistance = s_alMaxDistance->value;
//	//	float tmp = distance / maxdistance; // e.g. 3000 / 1000= 3
//	//	float gain = 1.0 - tmp; // so we end up at -2 gain, if distance is *3 of maxdistance
//	//	//gain /= gain;
//	//	if (gain < 0.0)
//	//		gain = 0.0;
//	//	else
//	//		gain *= gain; // todo: make some actual nice falloff curve + imgui menu for dynamic testing
//	//	S_AL_Gain(chksrc->alSource, gain);
//	//}
//	//else if(chksrc->scaleGain != chksrc->curGain)
//	//{
//	//	chksrc->scaleGain = chksrc->curGain;
//	//	S_AL_Gain(chksrc->alSource, chksrc->scaleGain);
//	//}
//}