/*
**  	Copyright (C) 2004 by the Q3Rally Development team
**  	All rights reserved.
**
**  	ui_rally_tools.c
**
**  	Q3R menu utility functions.
**
**      Author: STONELANCE
**
*/


#include "ui_local.h"


float UI_Random( void ){
/* op stack corrupted
	int seed = (trap_RealTime() & 0x0fff);
	int seed2 = Q_rand ( &seed );

	Com_Printf("trap_Milliseconds %i\n", trap_Milliseconds());
	Com_Printf("trap_RealTime %i\n", trap_RealTime());
	Com_Printf("seed2 %i\n", seed2);

	return ( trap_RealTime() & 0xffff ) / (float)0x10000;
*/


	int seed = trap_Milliseconds();
	return Q_random( &seed );
//	return random();
}


int UI_BuildFileList( const char *directory, const char *extension, const char *prefix,
					 qboolean excludeDirectory, qboolean excludeFileNames,
					 int specialCases, int startIndex, char list[256][64]){
	int		numFiles;
	int		numDirectories;
	int		numListItems;

	int		maxListItems = 256;
	int		maxCharacters = 64;

	char	filelist[8162];
	char*	fileptr;
	int		filelen;

	char	dirlist[8162];
	char*	dirptr;
	int		dirlen;

	int		sizeofPrefix;
	char	p[MAX_QPATH];
	char	dirname[MAX_QPATH];

	char	name[MAX_QPATH];
	int		i, j;

	qboolean	excludePrefix = qtrue;

	if (prefix && prefix[0] == '*'){
		excludePrefix = qfalse;
		prefix++;
	}

	strncpy(p, prefix, sizeof(p));
	for (sizeofPrefix = 0; sizeofPrefix < sizeof(p); sizeofPrefix++){
		if (p[sizeofPrefix] == 0)
			break;
	}

//	Com_Printf("UI_BuildFileList: searching for '%s' in '%s' with an extention of '%s'\n", prefix, directory, extension);

	numListItems = startIndex;

	numDirectories = trap_FS_GetFileList( directory, "/", dirlist, sizeof(dirlist) );
	dirptr = dirlist;
	for (i = 0; i < numDirectories; i++, dirptr += dirlen+1)
	{
		dirlen = strlen(dirptr);
		
		if (dirlen && dirptr[dirlen-1]=='/')
			dirptr[dirlen-1] = '\0';

//		Com_Printf("directory '%s/%s' extension '%s'\n", directory, dirptr, extension);

		if (/*!strcmp(dirptr, "") || */!strcmp(dirptr, ".."))
			continue;

		if (specialCases == BL_EXCLUDE) {
//			if (!Q_stricmp(dirptr, "macdaddy"))
//				continue;
		}
		else if (specialCases == BL_ONLY){
//			if (Q_stricmp(dirptr, "macdaddy"))
//				continue;

			continue;
		}

		// iterate all files in directory with extension = extension
		Com_sprintf(dirname, sizeof(dirname), "%s/%s", directory, dirptr);

		numFiles = trap_FS_GetFileList( dirname, extension, filelist, sizeof(filelist) );

//		Com_Printf("Found %d matching files in the directory '%s'\n", numFiles, dirname);

		fileptr = filelist;
		for (j = 0; j < numFiles && numListItems < maxListItems; j++, fileptr += filelen+1)
		{
			filelen = strlen(fileptr);
			COM_StripExtension(fileptr, name);

//			Com_Printf("name %s, prefix %s, size %d\n", name, prefix, sizeofPrefix);

			// look for prefix
			if (!Q_stricmpn(name, prefix, sizeofPrefix)){

//				Com_Printf("name %s, prefix %s, size %d\n", name, prefix, sizeofPrefix);

				if (excludeFileNames){
					if (excludeDirectory)
						Com_sprintf( list[numListItems], maxCharacters, "%s", dirptr );
					else
						Com_sprintf( list[numListItems], maxCharacters, "%s/%s", directory, dirptr );

					numListItems++;
					break;
				}
				else {
					if (excludePrefix){
						if (excludeDirectory)
							Com_sprintf( list[numListItems], maxCharacters, "%s", name + sizeofPrefix );
						else if (!strcmp(dirptr, "."))
							Com_sprintf( list[numListItems], maxCharacters, "%s/%s", directory, name + sizeofPrefix );
						else
							Com_sprintf( list[numListItems], maxCharacters, "%s/%s/%s", directory, dirptr, name + sizeofPrefix );
					}
					else{
						if (excludeDirectory)
							Com_sprintf( list[numListItems], maxCharacters, "%s", name );
						else if (!strcmp(dirptr, "."))
							Com_sprintf( list[numListItems], maxCharacters, "%s/%s", directory, name );
						else
							Com_sprintf( list[numListItems], maxCharacters, "%s/%s/%s", directory, dirptr, name );
					}

					numListItems++;
				}
			}
		}
	}

	return numListItems;
}
