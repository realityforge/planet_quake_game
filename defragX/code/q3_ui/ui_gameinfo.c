// Copyright (C) 1999-2000 Id Software, Inc.
// gameinfo.c
//

#include "ui_local.h"
#define POOLSIZE	256 * 1024

static int		ui_numArenas;
static char		*ui_arenaInfos[MAX_ARENAS];
static char		memoryPool[POOLSIZE];
static int		allocPoint, outOfMemory;


/*
===============
UI_Alloc
===============
*/
void *UI_Alloc( int size ) {
	char	*p;

	if ( allocPoint + size > POOLSIZE ) {
		outOfMemory = qtrue;
		return NULL;
	}

	p = &memoryPool[allocPoint];

	allocPoint += ( size + 31 ) & ~31;

	return p;
}

/*
===============
UI_InitMemory
===============
*/
void UI_InitMemory( void ) {
	allocPoint = 0;
	outOfMemory = qfalse;
}

/*
===============
UI_ParseInfos
===============
*/
int UI_ParseInfos( char *buf, int max, char *infos[] ) {
	char	*token;
	int		count = 0;
	char	key[MAX_TOKEN_CHARS];
	char	info[MAX_INFO_STRING];

	while ( 1 ) {
		token = COM_Parse( &buf );
		if ( !token[0] )
			break;

		if ( strcmp( token, "{" ) ) {
			Com_Printf( "Missing { in info file\n" );
			break;
		}

		if ( count == max ) {
			Com_Printf( "Max infos exceeded\n" );
			break;
		}

		info[0] = '\0';
		while ( 1 ) {
			token = COM_ParseExt( &buf, qtrue );
			if ( !token[0] ) {
				Com_Printf( "Unexpected end of info file\n" );
				break;
			}
			if ( !strcmp( token, "}" ) )
				break;

			Q_strncpyz( key, token, sizeof( key ) );

			token = COM_ParseExt( &buf, qfalse );
			if ( !token[0] )
				strcpy( token, "<NULL>" );

			Info_SetValueForKey( info, key, token );
		}

		//NOTE: extra space for arena number
		infos[count] = UI_Alloc(strlen(info) + strlen("\\num\\") + strlen(va("%d", MAX_ARENAS)) + 1);
		if (infos[count]) {
			strcpy(infos[count], info);
			count++;
		}
	}
	return count;
}

/*
===============
UI_LoadArenasFromFile
===============
*/
static void UI_LoadArenasFromFile( const char *filename ) {
	int				len;
	fileHandle_t	f;
	char			buf[MAX_ARENAS_TEXT];

	len = trap_FS_FOpenFile( filename, &f, FS_READ );
	if ( f == FS_INVALID_HANDLE ) {
		trap_Print( va( S_COLOR_RED "file not found: %s\n", filename ) );
		return;
	}
	if ( len >= sizeof( buf ) ) {
		trap_Print( va( S_COLOR_RED "file too large: %s is %i, max allowed is %i", filename, len, sizeof( buf ) ) );
		trap_FS_FCloseFile( f );
		return;
	}

	trap_FS_Read( buf, len, f );
	buf[len] = '\0';
	trap_FS_FCloseFile( f );

	ui_numArenas += UI_ParseInfos( buf, MAX_ARENAS - ui_numArenas, &ui_arenaInfos[ui_numArenas] );
}


/*
===============
UI_LoadMapinfos
===============
*/
static void UI_LoadMapinfos( void ) {
	int			numdirs;
	char		filename[128];
	char		dirlist[8192];
	char		dirlist2[8192];
	char		dirlist3[8192];
	char*		dirptr;
	int			i, n;
	int			dirlen;
	char		*type;
	char		*tag;

	ui_numArenas = 0;

	numdirs = trap_FS_GetFileList( "scripts", ".dfx",   dirlist, sizeof( dirlist ) );    // get all maps from .dfx files
    numdirs+= trap_FS_GetFileList( "scripts", ".defi",  dirlist2, sizeof( dirlist2 ) );  // get all maps from .defi files
    numdirs+= trap_FS_GetFileList( "scripts", ".arena", dirlist3, sizeof( dirlist3 ) );  // get all maps from .arena files

	dirptr  = dirlist;
	for (i = 0; i < numdirs; i++, dirptr += dirlen+1) {
		dirlen = strlen(dirptr);
		strcpy(filename, "scripts/");
		strcat(filename, dirptr);
		UI_LoadArenasFromFile(filename);
	}

	dirptr  = dirlist2;
	for (i = 0; i < numdirs; i++, dirptr += dirlen+1) {
		dirlen = strlen(dirptr);
		strcpy(filename, "scripts/");
		strcat(filename, dirptr);
		UI_LoadArenasFromFile(filename);
	}

	dirptr  = dirlist3;
	for (i = 0; i < numdirs; i++, dirptr += dirlen+1) {
		dirlen = strlen(dirptr);
		strcpy(filename, "scripts/");
		strcat(filename, dirptr);
		UI_LoadArenasFromFile(filename);
	}

	trap_Print( va( "%i maps parsed\n", ui_numArenas ) );

	if (outOfMemory)
        trap_Print( S_COLOR_YELLOW "WARNING: not anough memory in pool to load all maps\n" );

	// set initial numbers
	for( n = 0; n < ui_numArenas; n++ ) {
		Info_SetValueForKey( ui_arenaInfos[n], "num", va( "%i", n ) );
	}
}


/*
===============
UI_GetArenaInfoByNumber
===============
*/
const char *UI_GetArenaInfoByNumber( int num )
{
	int		n;
	char	*value;

	if( num < 0 || num >= ui_numArenas ) {
		trap_Print( va( S_COLOR_RED "Invalid arena number: %i\n", num ) );
		return NULL;
	}

	for( n = 0; n < ui_numArenas; n++ ) {
		value = Info_ValueForKey( ui_arenaInfos[n], "num" );
		if( *value && atoi(value) == num )
			return ui_arenaInfos[n];
	}

	return NULL;
}


/*
===============
UI_GetNumArenas
===============
*/
int UI_GetNumArenas( void )
{
	return ui_numArenas;
}

/*
===============
UI_InitGameinfo
===============
*/
void UI_InitGameinfo( void )
{
	UI_InitMemory();
	UI_LoadMapinfos();
}
