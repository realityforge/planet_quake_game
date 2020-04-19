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
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
// cg_syscalls.c -- this file is only included when building a dll
// cg_syscalls.asm is included instead when building a qvm

#include "q_shared.h"
#include "cg_local.h"
#include "client.h"
#include "client_render.h"

void	trap_Print( const char *fmt ) {
	//syscall( CG_PRINT, fmt );
	Com_Printf( "%s", fmt );
}

void trap_Error(const char *fmt)
{
	//syscall(CG_ERROR, fmt);
	Com_Error( ERR_DROP, "%s", fmt );
	// shut up GCC warning about returning functions, because we know better
	exit(1);
}

int trap_Milliseconds() {
	//return syscall( CG_MILLISECONDS ); 
	return Sys_Milliseconds();
}

int trap_Argc( void ) {
	//return syscall( CG_ARGC );
	return Cmd_Argc();
}

void	trap_Argv( int n, char *buffer, int bufferLength ) {
	//syscall( CG_ARGV, n, buffer, bufferLength );
	Cmd_ArgvBuffer( n, buffer, bufferLength );
}

void	trap_Args( char *buffer, int bufferLength ) {
	//syscall( CG_ARGS, buffer, bufferLength );
	Cmd_ArgsBuffer( buffer, bufferLength );
}

int		trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode ) {
	return 0;
}

void	trap_SendConsoleCommand( const char *text ) {
	//syscall( CG_SENDCONSOLECOMMAND, text );
	Cbuf_AddText( text );
}

void	trap_AddCommand( const char *cmdName ) {
	//syscall( CG_ADDCOMMAND, cmdName );
	CL_AddCgameCommand( cmdName );
}

void	trap_RemoveCommand( const char *cmdName ) {
	//syscall( CG_REMOVECOMMAND, cmdName );
	Cmd_RemoveCommandSafe( cmdName );
}

void	trap_SendClientCommand( const char *s ) {
	//syscall( CG_SENDCLIENTCOMMAND, s );
	CL_AddReliableCommand(s, qfalse);
}

void	trap_UpdateScreen( void ) {
	//syscall( CG_UPDATESCREEN );
		// this is used during lengthy level loading, so pump message loop
//		Com_EventLoop();	// FIXME: if a server restarts here, BAD THINGS HAPPEN!
// We can't call Com_EventLoop here, a restart will crash and this _does_ happen
// if there is a map change while we are downloading at pk3.
// ZOID
	SCR_UpdateScreen();
}

void	trap_CM_LoadMap( const char *mapname ) {
	//syscall( CG_CM_LOADMAP, mapname );
	CL_CM_LoadMap( mapname );
}

int		trap_CM_NumInlineModels( void ) {
	//return syscall( CG_CM_NUMINLINEMODELS );
	return CM_NumInlineModels();
}

clipHandle_t trap_CM_InlineModel( int index ) {
	//return syscall( CG_CM_INLINEMODEL, index );
	return CM_InlineModel( index );
}

clipHandle_t trap_CM_TempBoxModel( const vec3_t mins, const vec3_t maxs ) {
	//return syscall( CG_CM_TEMPBOXMODEL, mins, maxs );
	return CM_TempBoxModel( mins, maxs, /*int capsule*/ qfalse );
}

clipHandle_t trap_CM_TempCapsuleModel( const vec3_t mins, const vec3_t maxs ) {
	//return syscall( CG_CM_TEMPCAPSULEMODEL, mins, maxs );
	return CM_TempBoxModel( mins, maxs, /*int capsule*/ qtrue );
}

int		trap_CM_PointContents( const vec3_t p, clipHandle_t model ) {
	//return syscall( CG_CM_POINTCONTENTS, p, model );
	return CM_PointContents( p, model );
}

int		trap_CM_TransformedPointContents( const vec3_t p, clipHandle_t model, const vec3_t origin, const vec3_t angles ) {
	//return syscall( CG_CM_TRANSFORMEDPOINTCONTENTS, p, model, origin, angles );
	return CM_TransformedPointContents( p, model, origin, angles );
}

void	trap_CM_BoxTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask ) {
	//syscall( CG_CM_BOXTRACE, results, start, end, mins, maxs, model, brushmask );
	CM_BoxTrace( results, start, end, (vec_t*)mins, (vec_t*)maxs, model, brushmask, /*int capsule*/ qfalse );
}

void	trap_CM_CapsuleTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask ) {
	//syscall( CG_CM_CAPSULETRACE, results, start, end, mins, maxs, model, brushmask );
	CM_BoxTrace( results, start, end, (vec_t*)mins, (vec_t*)maxs, model, brushmask, /*int capsule*/ qtrue );
}

void trap_CM_TransformedBoxTrace( trace_t *results, const vec3_t start, const vec3_t end,
						  const vec3_t mins, const vec3_t maxs,
						  clipHandle_t model, int brushmask,
						  const vec3_t origin, const vec3_t angles ) {
	//syscall( CG_CM_TRANSFORMEDBOXTRACE, results, start, end, mins, maxs, model, brushmask, origin, angles );
	CM_TransformedBoxTrace( results, start, end, (vec_t*)mins, (vec_t*)maxs, model, brushmask, origin, angles, /*int capsule*/ qfalse );
}

void trap_CM_TransformedCapsuleTrace(
	trace_t *results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs, clipHandle_t model, int brushmask, const vec3_t origin, const vec3_t angles
) {
	//syscall( CG_CM_TRANSFORMEDCAPSULETRACE, results, start, end, mins, maxs, model, brushmask, origin, angles );
	CM_TransformedBoxTrace( results, start, end, (vec_t*)mins, (vec_t*)maxs, model, brushmask, origin, angles, /*int capsule*/ qtrue );
}

int	trap_CM_MarkFragments( int numPoints, const vec3_t *points, const vec3_t projection, int maxPoints, vec3_t pointBuffer, int maxFragments, markFragment_t *fragmentBuffer ) {
	return 0;
}

void	trap_S_StartSound( vec3_t origin, int entityNum, int entchannel, sfxHandle_t sfx ) {
	S_StartSound( origin, entityNum, entchannel, sfx );
}

void	trap_S_StartLocalSound( sfxHandle_t sfx, int channelNum ) {
	S_StartLocalSound( sfx, channelNum );
}

void	trap_S_ClearLoopingSounds( qboolean killall ) {
	S_ClearLoopingSounds(killall);
}

void	trap_S_AddLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx ) {
	S_AddLoopingSound( entityNum, origin, velocity, sfx );
}

void	trap_S_AddRealLoopingSound( int entityNum, const vec3_t origin, const vec3_t velocity, sfxHandle_t sfx ) {
	S_AddRealLoopingSound( entityNum, origin, velocity, sfx );
}

void	trap_S_StopLoopingSound( int entityNum ) {
	S_StopLoopingSound( entityNum );
}

void	trap_S_UpdateEntityPosition( int entityNum, const vec3_t origin ) {
	S_UpdateEntityPosition( entityNum, origin );
}

void	trap_S_Respatialize( int entityNum, const vec3_t origin, vec3_t axis[3], int inwater ) {
	S_Respatialize( entityNum, origin, axis, inwater );
}

sfxHandle_t	trap_S_RegisterSound( const char *sample, qboolean compressed ) {
	sfxHandle_t ret = S_RegisterSound( sample, compressed );
	return ret;
}

void	trap_S_StartBackgroundTrack( const char *intro, const char *loop ) {
	S_StartBackgroundTrack( intro, loop );
}

void	trap_R_LoadWorldMap( const char *mapname ) {

}

qhandle_t RE_RegisterModel( const char *name );
qhandle_t trap_R_RegisterModel( const char *name ) {
	//return syscall( CG_R_REGISTERMODEL, name );
	return RE_RegisterModel( name );
}

qhandle_t trap_R_RegisterSkin( const char *name ) {
	return 0;
}

qhandle_t trap_R_RegisterShader( const char *name ) {
	//return syscall( CG_R_REGISTERSHADER, name );
	return 0;
}

qhandle_t trap_R_RegisterShaderNoMip( const char *name ) {
	return 0;
}

void	trap_R_AddLightToScene( const vec3_t org, float intensity, float r, float g, float b ) {

}

void	trap_R_AddAdditiveLightToScene( const vec3_t org, float intensity, float r, float g, float b ) {

}

void	trap_R_SetColor( const float *rgba ) {
}

void	trap_R_DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader ) {

}

void		trap_GetGameState( gameState_t *gamestate ) {
	//syscall( CG_GETGAMESTATE, gamestate );
	CL_GetGameState( gamestate );
}

void		trap_GetCurrentSnapshotNumber( int *snapshotNumber, int *serverTime ) {
	//syscall( CG_GETCURRENTSNAPSHOTNUMBER, snapshotNumber, serverTime );
	CL_GetCurrentSnapshotNumber( snapshotNumber, serverTime );
}

qboolean	trap_GetSnapshot( int snapshotNumber, snapshot_t *snapshot ) {
	//return syscall( CG_GETSNAPSHOT, snapshotNumber, snapshot );
	return CL_GetSnapshot( snapshotNumber, snapshot );
}

qboolean	trap_GetServerCommand( int serverCommandNumber ) {
	//return syscall( CG_GETSERVERCOMMAND, serverCommandNumber );
	return CL_GetServerCommand( serverCommandNumber );
}

int trap_GetCurrentCmdNumber( void ) {
	//return syscall( CG_GETCURRENTCMDNUMBER );
	return CL_GetCurrentCmdNumber();
}

qboolean	trap_GetUserCmd( int cmdNumber, usercmd_t *ucmd ) {
	//return syscall( CG_GETUSERCMD, cmdNumber, ucmd );
	return CL_GetUserCmd( cmdNumber, ucmd );
}


// LMAO, the reason why I couldn't move the mouse when merging cgame with engine was here
// There was no function definition for CL_SetUserCmdValue, but it compiled nonetheless without problems
// And the missing funcdef fucked up the parameters, the sensitivityScale==1.0 value was converted always to 0
// And this 0 was multiplied with the mouse movement, which caused no mouse movement at all...
// looks like the other functions got no defs aswell.... why the fuck does it even work? Added all defs to cl_local.h now
void trap_SetUserCmdValue( int stateValue, float sensitivityScale ) {
	//syscall( CG_SETUSERCMDVALUE, stateValue, PASSFLOAT(sensitivityScale) );
	CL_SetUserCmdValue( stateValue,  sensitivityScale );
}

int trap_MemoryRemaining( void ) {
	//return syscall( CG_MEMORY_REMAINING );
	return Hunk_MemoryRemaining();
}

qboolean trap_Key_IsDown( int keynum ) {
	//return syscall( CG_KEY_ISDOWN, keynum );
	return Key_IsDown( keynum );
}

int trap_Key_GetCatcher( void ) {
	//return syscall( CG_KEY_GETCATCHER );
	return Key_GetCatcher();
}

void trap_Key_SetCatcher( int catcher ) {
	//syscall( CG_KEY_SETCATCHER, catcher );
	// Don't allow the cgame module to close the console
	Key_SetCatcher( catcher | ( Key_GetCatcher( ) & KEYCATCH_CONSOLE ) );
}

int trap_Key_GetKey( const char *binding ) {
	//return syscall( CG_KEY_GETKEY, binding );
	return Key_GetKey( binding );
}

//int trap_PC_AddGlobalDefine( char *define ) {
//	return syscall( CG_PC_ADD_GLOBAL_DEFINE, define );
//}

void	trap_S_StopBackgroundTrack( void ) {
	//syscall( CG_S_STOPBACKGROUNDTRACK );
	S_StopBackgroundTrack();
}

void trap_SnapVector( float *v ) {
	//syscall( CG_SNAPVECTOR, v );
	Q_SnapVector(v);
}

qboolean trap_GetEntityToken( char *buffer, int bufferSize ) {
	return qfalse;
}

qboolean trap_R_inPVS( const vec3_t p1, const vec3_t p2 ) {
	return qfalse;
}
