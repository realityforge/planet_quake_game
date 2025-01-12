/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2000-2013 Darklegion Development
Copyright (C) 2015-2019 GrangerHub

This file is part of Tremulous.

Tremulous is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

Tremulous is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tremulous; if not, see <https://www.gnu.org/licenses/>

===========================================================================
*/

#include <signal.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#ifndef DEDICATED
#include <SDL.h>
#include <SDL_cpuinfo.h>
#endif

#include "sys_local.h"
#include "sys_loadlib.h"

#include "../qcommon/q_shared.h"
#include "../qcommon/qcommon.h"

static char binaryPath[ MAX_OSPATH ] = { 0 };
static char installPath[ MAX_OSPATH ] = { 0 };

/*
=================
Sys_SetBinaryPath
=================
*/
void Sys_SetBinaryPath(const char *path)
{
	Q_strncpyz(binaryPath, path, sizeof(binaryPath));
}

/*
=================
Sys_BinaryPath
=================
*/
char *Sys_BinaryPath(void)
{
	return binaryPath;
}

/*
=================
Sys_SetDefaultInstallPath
=================
*/
void Sys_SetDefaultInstallPath(const char *path)
{
	Q_strncpyz(installPath, path, sizeof(installPath));
}

/*
=================
Sys_DefaultInstallPath
=================
*/
char *Sys_DefaultInstallPath(void)
{
	return installPath;
}

/*
=================
Sys_DefaultAppPath
=================
*/
char *Sys_DefaultAppPath(void)
{
	return Sys_BinaryPath();
}

/*
=================
Sys_In_Restart_f

Restart the input subsystem
=================
*/
void Sys_In_Restart_f( void )
{
	IN_Restart( );
}

/*
=================
Sys_ConsoleInput

Handle new console input
=================
*/
char *Sys_ConsoleInput(void)
{
	return CON_Input( );
}

/*
==================
Sys_GetClipboardData
==================
*/
char *Sys_GetClipboardData(void)
{
#ifdef DEDICATED
	return NULL;
#else
	char *data = NULL;
	char *cliptext;

	if ( ( cliptext = SDL_GetClipboardText() ) != NULL ) {
		if ( cliptext[0] != '\0' ) {
			size_t bufsize = strlen( cliptext ) + 1;

			data = Z_Malloc( bufsize );
			Q_strncpyz( data, cliptext, bufsize );

			// find first listed char and set to '\0'
			strtok( data, "\n\r\b" );
		}
		SDL_free( cliptext );
	}
	return data;
#endif
}

/*
=================
Sys_Exit

Single exit point (regular exit or in case of error)
=================
*/
static __attribute__ ((noreturn)) void Sys_Exit( int exitCode )
{
	CON_Shutdown( );

#ifndef DEDICATED
	SDL_Quit( );
#endif

	NET_Shutdown( );

	Sys_PlatformExit( );

	exit( exitCode );
}

/*
=================
Sys_Quit
=================
*/
void Sys_Quit( void )
{
	Sys_Exit( 0 );
}

/*
=================
Sys_GetProcessorFeatures
=================
*/
cpuFeatures_t Sys_GetProcessorFeatures( void )
{
	cpuFeatures_t features = 0;

#ifndef DEDICATED
	if( SDL_HasRDTSC( ) )    features |= CF_RDTSC;
	if( SDL_HasMMX( ) )      features |= CF_MMX;
	if( SDL_HasSSE( ) )      features |= CF_SSE;
	if( SDL_HasSSE2( ) )     features |= CF_SSE2;
#endif

	return features;
}

/*
=================
Sys_Init
=================
*/
void Sys_Init(void)
{
	Cmd_AddCommand( "in_restart", Sys_In_Restart_f );
}

/*
=================
Sys_AnsiColorPrint

Transform Q3 colour codes to ANSI escape sequences
=================
*/
void Sys_AnsiColorPrint( const char *msg )
{
	static char buffer[ MAXPRINTMSG ];
	int         length = 0;
	static int  q3ToAnsi[ 8 ] =
	{
		7, // COLOR_BLACK
		31, // COLOR_RED
		32, // COLOR_GREEN
		33, // COLOR_YELLOW
		34, // COLOR_BLUE
		36, // COLOR_CYAN
		35, // COLOR_MAGENTA
		0   // COLOR_WHITE
	};

	while( *msg )
	{
		if( Q_IsColorString( msg ) || *msg == '\n' )
		{
			// First empty the buffer
			if( length > 0 )
			{
				buffer[ length ] = '\0';
				fputs( buffer, stderr );
				length = 0;
			}

			if( *msg == '\n' )
			{
				// Issue a reset and then the newline
				fputs( "\033[0m\n", stderr );
				msg++;
			}
			else
			{
				vec4_t color;

				if(Q_IsHardcodedColor(msg)) {
					Vector4Copy(g_color_table[ColorIndex(*(msg+1))], color);
				} else {
					Q_GetVectFromHexColor(msg, color);
				}
				// Print the color code (reset first to clear potential inverse (black))
				Com_sprintf( buffer, sizeof( buffer ), "\033[0m\033[%dm",
						q3ToAnsi[Q_ApproxBasicColorIndexFromVectColor(color)] );
				fputs( buffer, stderr );
				msg += Q_ColorStringLength(msg);
			}
		}
		else
		{
			if( length >= MAXPRINTMSG - 1 )
				break;

			if(Q_IsColorEscapeEscape(msg)) {
				msg++;
			}

			buffer[ length ] = *msg;
			length++;
			msg++;
		}
	}

	// Empty anything still left in the buffer
	if( length > 0 )
	{
		buffer[ length ] = '\0';
		fputs( buffer, stderr );
	}
}

/*
=================
Sys_Print
=================
*/
void Sys_Print( const char *msg )
{
	CON_LogWrite( msg );
	CON_Print( msg );
}

/*
=================
Sys_Error
=================
*/
void Sys_Error( const char *error, ... )
{
	va_list argptr;
	char    string[1024];

	va_start (argptr,error);
	Q_vsnprintf (string, sizeof(string), error, argptr);
	va_end (argptr);

	Sys_ErrorDialog( string );

	Sys_Exit( 3 );
}

#if 0
/*
=================
Sys_Warn
=================
*/
static __attribute__ ((format (printf, 1, 2))) void Sys_Warn( char *warning, ... )
{
	va_list argptr;
	char    string[1024];

	va_start (argptr,warning);
	Q_vsnprintf (string, sizeof(string), warning, argptr);
	va_end (argptr);

	CON_Print( va( "Warning: %s", string ) );
}
#endif

/*
============
Sys_FileTime

returns -1 if not present
============
*/
int Sys_FileTime( char *path )
{
	struct stat buf;

	if (stat (path,&buf) == -1)
		return -1;

	return buf.st_mtime;
}

/*
=================
Sys_UnloadDll
=================
*/
void Sys_UnloadDll( void *dllHandle )
{
	if( !dllHandle )
	{
		Com_Printf("Sys_UnloadDll(NULL)\n");
		return;
	}

	Sys_UnloadLibrary(dllHandle);
}

/*
=================
Sys_LoadDll

First try to load library name from system library path,
from executable path, then fs_basepath.
=================
*/

void *Sys_LoadDll(const char *name, qboolean useSystemLib)
{
	void *dllhandle;
	
	if(useSystemLib)
		Com_Printf("Trying to load \"%s\"...\n", name);
	
	if(!useSystemLib || !(dllhandle = Sys_LoadLibrary(name)))
	{
		const char *overpath = Cvar_VariableString("fs_overpath");

		if(overpath[0])
		{
			char libPath[MAX_OSPATH];
			Com_Printf("Trying to load \"%s\" from \"%s\"...\n", name, overpath);
			Com_sprintf(libPath, sizeof(libPath), "%s%c%s", overpath, PATH_SEP, name);
			dllhandle = Sys_LoadLibrary(libPath);
		}
		else
			dllhandle = NULL;

		if(!dllhandle)
			Com_Printf("Loading \"%s\" failed\n", name);
	}
	
	return dllhandle;
}

/*
=================
Sys_LoadGameDll

Used to load a development dll instead of a virtual machine
=================
*/
void *Sys_LoadGameDll(const char *name,
	intptr_t (QDECL **entryPoint)(int, ...),
	intptr_t (*systemcalls)(intptr_t, ...))
{
	void *libHandle;
	void (*dllEntry)(intptr_t (*syscallptr)(intptr_t, ...));

	assert(name);

	Com_Printf( "Loading DLL file: %s\n", name);
	libHandle = Sys_LoadLibrary(name);

	if(!libHandle)
	{
		Com_Printf("Sys_LoadGameDll(%s) failed:\n\"%s\"\n", name, Sys_LibraryError());
		return NULL;
	}

	dllEntry = Sys_LoadFunction( libHandle, "dllEntry" );
	*entryPoint = Sys_LoadFunction( libHandle, "vmMain" );

	if ( !*entryPoint || !dllEntry )
	{
		Com_Printf ( "Sys_LoadGameDll(%s) failed to find vmMain function:\n\"%s\" !\n", name, Sys_LibraryError( ) );
		Sys_UnloadLibrary(libHandle);

		return NULL;
	}

	Com_Printf ( "Sys_LoadGameDll(%s) found vmMain function at %p\n", name, *entryPoint );
	dllEntry( systemcalls );

	return libHandle;
}

/*
=================
Sys_GetFunction

Grabs the symbol/function from the DLL
=================
*/
void *Sys_GetFunction( void *dllHandle, char *name )
{
	void *func = Sys_LoadFunction( dllHandle, name );
	if( !func )
	{
		fprintf( stdout, "Sys_GetFunction: %s failed to load: %s\n", name, Sys_LibraryError( ) );
		Sys_Exit( 0 );
	}
	return func;
}

/*
=================
Sys_ParseArgs
=================
*/
void Sys_ParseArgs( int argc, char **argv )
{
	if( argc == 2 )
	{
		if( !strcmp( argv[1], "--version" ) ||
				!strcmp( argv[1], "-v" ) )
		{
			const char* date = __DATE__;
#ifdef DEDICATED
			fprintf( stdout, Q3_VERSION " dedicated server (%s)\n", date );
#else
			fprintf( stdout, Q3_VERSION " client (%s)\n", date );
#endif
			Sys_Exit( 0 );
		}
	}
}

/*
=================
Sys_SigHandler
=================
*/
void Sys_SigHandler( int signal )
{
	static qboolean signalcaught = qfalse;

	if( signalcaught )
	{
		fprintf( stderr, "DOUBLE SIGNAL FAULT: Received signal %d, exiting...\n",
			signal );
	}
	else
	{
		signalcaught = qtrue;
		VM_Forced_Unload_Start();
#ifndef DEDICATED
		CL_Shutdown(va("Received signal %d", signal), qtrue, qtrue);
#endif
		SV_Shutdown(va("Received signal %d", signal) );
		VM_Forced_Unload_Done();
	}

	if( signal == SIGTERM || signal == SIGINT )
		Sys_Exit( 1 );
	else
		Sys_Exit( 2 );
}

/*
=================
main
=================
*/
int main( int argc, char **argv )
{
	int   i;
	char  commandLine[ MAX_STRING_CHARS ] = { 0 };

#ifndef DEDICATED
	// SDL version check

	// Compile time
#	if !SDL_VERSION_ATLEAST(MINSDL_MAJOR,MINSDL_MINOR,MINSDL_PATCH)
#		error A more recent version of SDL is required
#	endif

	// Run time
	SDL_version ver;
	SDL_GetVersion( &ver );

#define MINSDL_VERSION \
	XSTRING(MINSDL_MAJOR) "." \
	XSTRING(MINSDL_MINOR) "." \
	XSTRING(MINSDL_PATCH)

	if( SDL_VERSIONNUM( ver.major, ver.minor, ver.patch ) <
			SDL_VERSIONNUM( MINSDL_MAJOR, MINSDL_MINOR, MINSDL_PATCH ) )
	{
		Sys_Dialog( DT_ERROR, va( "SDL version " MINSDL_VERSION " or greater is required, "
			"but only version %d.%d.%d was found. You may be able to obtain a more recent copy "
			"from http://www.libsdl.org/.", ver.major, ver.minor, ver.patch ), "SDL Library Too Old" );

		Sys_Exit( 1 );
	}
#endif

	Sys_PlatformInit( );

	// Set the initial time base
	Sys_Milliseconds( );

#ifdef MACOS_X
	// This is passed if we are launched by double-clicking
	if ( argc >= 2 && Q_strncmp ( argv[1], "-psn", 4 ) == 0 )
		argc = 1;
#endif

	Sys_ParseArgs( argc, argv );
	if( strchr( argv[ 0 ], '/' )
#ifdef _WIN32
	    || strchr( argv[ 0 ], '\\' )
#endif
	)
	{
		Sys_SetBinaryPath( Sys_Dirname( argv[ 0 ] ) );
	}
#if defined DEFAULT_BASEDIR
	Sys_SetDefaultInstallPath( DEFAULT_BASEDIR );
#elif defined MACOS_X
	Sys_SetDefaultInstallPath( Sys_StripAppBundle( Sys_BinaryPath() ) );
#endif

	// Concatenate the command line for passing to Com_Init
	for( i = 1; i < argc; i++ )
	{
		const qboolean containsSpaces = strchr(argv[i], ' ') != NULL;
		if (containsSpaces)
			Q_strcat( commandLine, sizeof( commandLine ), "\"" );

		Q_strcat( commandLine, sizeof( commandLine ), argv[ i ] );

		if (containsSpaces)
			Q_strcat( commandLine, sizeof( commandLine ), "\"" );

		Q_strcat( commandLine, sizeof( commandLine ), " " );
	}

	Com_Init( commandLine );
	NET_Init( );

	CON_Init( );

	signal( SIGTERM, Sys_SigHandler );
	signal( SIGINT, Sys_SigHandler );

	while( 1 )
	{
		IN_Frame( );
		Com_Frame( );
	}

	return 0;
}
