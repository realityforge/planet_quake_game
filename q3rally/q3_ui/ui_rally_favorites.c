/*
**  	Copyright (C) 2004 by the Q3Rally Development team
**  	All rights reserved.
**
**  	ui_rally_favorites.c
**
**  	Main support function for q3r favorites menu feature.
**
**      Author: STONELANCE
**
*/

#include "ui_local.h"

/*
=================
GetValuesFromFavorite

=================
*/
qboolean GetValuesFromFavorite( const char *favorite, char *modelName, char *skinName, char *rimName, char *headName ) {
	char		buf[MAX_QPATH];
	char		buf2[MAX_QPATH];
	char		*slash;

	trap_Cvar_VariableStringBuffer( favorite, buf, sizeof( buf ) );

	slash = strchr( buf, '/' );
	if ( slash ) {
		Q_strncpyz(buf2, slash + 1, sizeof(buf2));
		*slash = 0;
		if (modelName){
			strcpy( modelName, buf );
		}
	}
	else{
		modelName = NULL;
		return qtrue;
	}

	slash = strchr( buf2, '/' );
	if ( slash ) {
		Q_strncpyz( buf, slash + 1, sizeof( buf ) );
		*slash = 0;
		if (skinName){
			strcpy( skinName, buf2 );
		}
	}
	else{
		skinName = NULL;
		return qtrue;
	}

	slash = strchr( buf, '/' );
	if ( slash ) {
		Q_strncpyz( buf2, slash + 1, sizeof( buf2 ) );
		*slash = 0;
		if (rimName){
			strcpy( rimName, buf );
		}
	}
	else{
		rimName = NULL;
		return qtrue;
	}

	if (buf2){
		if (headName){
			strcpy(headName, buf2);
		}
	}
	else{
		headName = NULL;
		return qtrue;
	}

	return qfalse;
}
