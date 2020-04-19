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
#include "g_local.h"
#include <client.h>
#include <server.h>
#include "sv.h"

// this file is only included when building a dll
// g_syscalls.asm is included instead when building a qvm
#ifdef Q3_VM
#error "Do not use in VM build"
#endif








int trap_Cvar_VariableIntegerValue( const char *var_name ) {
	//return syscall( G_CVAR_VARIABLE_INTEGER_VALUE, var_name );
	return Cvar_VariableIntegerValue( var_name );
}



void trap_LocateGameData( gentity_t *gEnts, int numGEntities, int sizeofGEntity_t,
						 playerState_t *clients, int sizeofGClient ) {
	//syscall( G_LOCATE_GAME_DATA, gEnts, numGEntities, sizeofGEntity_t, clients, sizeofGClient );
	SV_LocateGameData( (sharedEntity_t *)gEnts, numGEntities, sizeofGEntity_t, clients, sizeofGClient );
}

void trap_DropClient( int clientNum, const char *reason ) {
	//syscall( G_DROP_CLIENT, clientNum, reason );
	SV_GameDropClient( clientNum, reason );
}

void trap_SendServerCommand( int clientNum, const char *text ) {
	//syscall( G_SEND_SERVER_COMMAND, clientNum, text );
	SV_GameSendServerCommand( clientNum, text );
}

void trap_SetConfigstring( int num, const char *string ) {
	//syscall( G_SET_CONFIGSTRING, num, string );
	SV_SetConfigstring( num, string );
}

void trap_GetConfigstring( int num, char *buffer, int bufferSize ) {
	//syscall( G_GET_CONFIGSTRING, num, buffer, bufferSize );
	SV_GetConfigstring( num, buffer, bufferSize );
}

void trap_GetUserinfo( int num, char *buffer, int bufferSize ) {
	//syscall( G_GET_USERINFO, num, buffer, bufferSize );
	SV_GetUserinfo( num, buffer, bufferSize );
}

void trap_SetUserinfo( int num, const char *buffer ) {
	//syscall( G_SET_USERINFO, num, buffer );
	SV_SetUserinfo( num, buffer );
}

void trap_GetServerinfo( char *buffer, int bufferSize ) {
	//syscall( G_GET_SERVERINFO, buffer, bufferSize );
	SV_GetServerinfo( buffer, bufferSize );
}

void trap_SetBrushModel( gentity_t *ent, const char *name ) {
	//syscall( G_SET_BRUSH_MODEL, ent, name );
	SV_SetBrushModel((sharedEntity_t*)ent, name );
}

void trap_Trace( trace_t *results, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int passEntityNum, int contentmask ) {
	//syscall( G_TRACE, results, start, mins, maxs, end, passEntityNum, contentmask );
	SV_Trace( results, start, mins, maxs, end, passEntityNum, contentmask, /*int capsule*/ qfalse );
}

void trap_TraceCapsule( trace_t *results, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int passEntityNum, int contentmask ) {
	//syscall( G_TRACECAPSULE, results, start, mins, maxs, end, passEntityNum, contentmask );
	SV_Trace( results, start, mins, maxs, end, passEntityNum, contentmask, /*int capsule*/ qtrue );
}

int trap_PointContents( const vec3_t point, int passEntityNum ) {
	//return syscall( G_POINT_CONTENTS, point, passEntityNum );
	return SV_PointContents( point, passEntityNum );
}

qboolean trap_InPVS( const vec3_t p1, const vec3_t p2 ) {
	//return syscall( G_IN_PVS, p1, p2 );
	return SV_inPVS( p1, p2 );
}

qboolean trap_InPVSIgnorePortals( const vec3_t p1, const vec3_t p2 ) {
	//return syscall( G_IN_PVS_IGNORE_PORTALS, p1, p2 );
	return SV_inPVSIgnorePortals( p1, p2 );
}

void trap_AdjustAreaPortalState( gentity_t *ent, qboolean open ) {
	//syscall( G_ADJUST_AREA_PORTAL_STATE, ent, open );
	SV_AdjustAreaPortalState((sharedEntity_t*)ent, open );
}

qboolean trap_AreasConnected( int area1, int area2 ) {
	//return syscall( G_AREAS_CONNECTED, area1, area2 );
	return CM_AreasConnected( area1, area2 );
}

void trap_LinkEntity( gentity_t *ent ) {
	//syscall( G_LINKENTITY, ent );
	SV_LinkEntity((sharedEntity_t*)ent );
}

void trap_UnlinkEntity( gentity_t *ent ) {
	//syscall( G_UNLINKENTITY, ent );
	SV_UnlinkEntity((sharedEntity_t*)ent );
}

int trap_EntitiesInBox( const vec3_t mins, const vec3_t maxs, int *list, int maxcount ) {
	//return syscall( G_ENTITIES_IN_BOX, mins, maxs, list, maxcount );
	return SV_AreaEntities( mins, maxs, list, maxcount );
}

#if 0
qboolean trap_EntityContact( const vec3_t mins, const vec3_t maxs, const gentity_t *ent ) {
	//return syscall( G_ENTITY_CONTACT, mins, maxs, ent );
	return SV_EntityContact( mins, maxs, (sharedEntity_t*)ent, /*int capsule*/ qfalse );
}
qboolean trap_EntityContactCapsule( const vec3_t mins, const vec3_t maxs, const gentity_t *ent ) {
	//return syscall( G_ENTITY_CONTACTCAPSULE, mins, maxs, ent );
	return SV_EntityContact( mins, maxs, (sharedEntity_t*)ent, /*int capsule*/ qtrue );
}
#endif

int trap_BotAllocateClient( void ) {
	//return syscall( G_BOT_ALLOCATE_CLIENT );
	return SV_BotAllocateClient();
}

void trap_BotFreeClient( int clientNum ) {
	//syscall( G_BOT_FREE_CLIENT, clientNum );
	SV_BotFreeClient( clientNum );
}

void trap_GetUsercmd( int clientNum, usercmd_t *cmd ) {
	//syscall( G_GET_USERCMD, clientNum, cmd );
	SV_GetUsercmd( clientNum, cmd );
}

qboolean trap_Game_GetEntityToken( char *buffer, int bufferSize ) {
	//return syscall( G_GET_ENTITY_TOKEN, buffer, bufferSize );
	const char	*s;
	s = COM_Parse( &sv.entityParsePoint );
	Q_strncpyz( buffer, s, bufferSize );
	if ( !sv.entityParsePoint && !s[0] ) {
	return qfalse;
	} else {
	return qtrue;
	}
}

int trap_DebugPolygonCreate(int color, int numPoints, vec3_t *points) {
	//return syscall( G_DEBUG_POLYGON_CREATE, color, numPoints, points );
	return BotImport_DebugPolygonCreate( color, numPoints, points );
}

void trap_DebugPolygonDelete(int id) {
	//syscall( G_DEBUG_POLYGON_DELETE, id );
	BotImport_DebugPolygonDelete( id );
}
