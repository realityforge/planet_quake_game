// Copyright (C) 1999-2000 Id Software, Inc.
//
#include "ui_local.h"

/*
===============================================================================

CONNECTION SCREEN

===============================================================================
*/

qboolean	passwordNeeded = qtrue;
menufield_s passwordField;

static connstate_t	lastConnState;
static char			lastLoadingText[MAX_INFO_VALUE];

static void UI_ReadableSize ( char *buf, int bufsize, int value )
{
	if (value > 1024*1024*1024 ) { // gigs
		Com_sprintf( buf, bufsize, "%d", value / (1024*1024*1024) );
		Com_sprintf( buf+strlen(buf), bufsize-strlen(buf), ".%02d GB", 
			(value % (1024*1024*1024))*100 / (1024*1024*1024) );
	} else if (value > 1024*1024 ) { // megs
		Com_sprintf( buf, bufsize, "%d", value / (1024*1024) );
		Com_sprintf( buf+strlen(buf), bufsize-strlen(buf), ".%02d MB", 
			(value % (1024*1024))*100 / (1024*1024) );
	} else if (value > 1024 ) { // kilos
		Com_sprintf( buf, bufsize, "%d KB", value / 1024 );
	} else { // bytes
		Com_sprintf( buf, bufsize, "%d bytes", value );
	}
}

// Assumes time is in msec
static void UI_PrintTime ( char *buf, int bufsize, int time ) {
	time /= 1000;  // change to seconds

	if (time > 3600) { // in the hours range
		Com_sprintf( buf, bufsize, "%d hr %d min", time / 3600, (time % 3600) / 60 );
	} else if (time > 60) { // mins
		Com_sprintf( buf, bufsize, "%d min %d sec", time / 60, time % 60 );
	} else  { // secs
		Com_sprintf( buf, bufsize, "%d sec", time );
	}
}

static void UI_DisplayDownloadInfo( const char *downloadName ) {
	static char dlText[]	= "Downloading:";
	static char etaText[]	= "Estimated time left:";
	static char xferText[]	= "Transfer rate:";

	int downloadSize, downloadCount, downloadTime;
	char dlSizeBuf[64], totalSizeBuf[64], xferRateBuf[64], dlTimeBuf[64];
	int xferRate;
	int width, leftWidth;
	int style = UI_LEFT|UI_SMALLFONT|UI_DROPSHADOW;
	const char *s;

	downloadSize = trap_Cvar_VariableValue( "cl_downloadSize" );
	downloadCount = trap_Cvar_VariableValue( "cl_downloadCount" );
	downloadTime = trap_Cvar_VariableValue( "cl_downloadTime" );

	leftWidth = width = UI_ProportionalStringWidth( dlText ) * UI_ProportionalSizeScale( style );
	width = UI_ProportionalStringWidth( etaText ) * UI_ProportionalSizeScale( style );
	if (width > leftWidth) leftWidth = width;
	width = UI_ProportionalStringWidth( xferText ) * UI_ProportionalSizeScale( style );
	if (width > leftWidth) leftWidth = width;
	leftWidth += 16;

	UI_DrawProportionalString( 8, 128, dlText, style, color_white );
	UI_DrawProportionalString( 8, 160, etaText, style, color_white );
	UI_DrawProportionalString( 8, 224, xferText, style, color_white );

	if (downloadSize > 0) {
		s = va( "%s (%d%%)", downloadName, downloadCount * 100 / downloadSize );
	} else {
		s = downloadName;
	}

	UI_DrawProportionalString( leftWidth, 128, s, style, color_white );

	UI_ReadableSize( dlSizeBuf,		sizeof dlSizeBuf,		downloadCount );
	UI_ReadableSize( totalSizeBuf,	sizeof totalSizeBuf,	downloadSize );

	if (downloadCount < 4096 || !downloadTime) {
		UI_DrawProportionalString( leftWidth, 160, "estimating", style, color_white );
		UI_DrawProportionalString( leftWidth, 192, 
			va("(%s of %s copied)", dlSizeBuf, totalSizeBuf), style, color_white );
	} else {
		if ((uis.realtime - downloadTime) / 1000) {
			xferRate = downloadCount / ((uis.realtime - downloadTime) / 1000);
		} else {
			xferRate = 0;
		}
		UI_ReadableSize( xferRateBuf, sizeof xferRateBuf, xferRate );

		// Extrapolate estimated completion time
		if (downloadSize && xferRate) {
			int n = downloadSize / xferRate; // estimated time for entire d/l in secs

			// We do it in K (/1024) because we'd overflow around 4MB
			UI_PrintTime ( dlTimeBuf, sizeof dlTimeBuf, 
				(n - (((downloadCount/1024) * n) / (downloadSize/1024))) * 1000);

			UI_DrawProportionalString( leftWidth, 160, 
				dlTimeBuf, style, color_white );
			UI_DrawProportionalString( leftWidth, 192, 
				va("(%s of %s copied)", dlSizeBuf, totalSizeBuf), style, color_white );
		} else {
			UI_DrawProportionalString( leftWidth, 160, 
				"estimating", style, color_white );
			if (downloadSize) {
				UI_DrawProportionalString( leftWidth, 192, 
					va("(%s of %s copied)", dlSizeBuf, totalSizeBuf), style, color_white );
			} else {
				UI_DrawProportionalString( leftWidth, 192, 
					va("(%s copied)", dlSizeBuf), style, color_white );
			}
		}

		if (xferRate) {
			UI_DrawProportionalString( leftWidth, 224, 
				va("%s/Sec", xferRateBuf), style, color_white );
		}
	}
}

/*
========================
UI_DrawConnectScreen

This will also be overlaid on the cgame info screen during loading
to prevent it from blinking away too rapidly on local or lan games.
========================
*/
void UI_DrawConnectScreen( qboolean overlay ) {
	char			*s;
	uiClientState_t	cstate;
	char			info[MAX_INFO_VALUE];

	Menu_Cache();

	// see what information we should display
	trap_GetClientState( &cstate );
	info[0] = '\0';

	if ( !overlay ) {
		// draw the dialog background
		UI_SetColor( color_white );
		UI_DrawHandlePic( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uis.menuBackLandscape );

		UI_DrawProportionalString( 320, 16, va("Connecting to %s", cstate.servername), UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, menu_text_color );

		// display global MOTD at bottom
		UI_DrawProportionalString( SCREEN_WIDTH/2, SCREEN_HEIGHT-32, 
			Info_ValueForKey( cstate.updateInfoString, "motd" ), UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, menu_text_color );
	
		// print any server info (server full, bad version, etc)
		if ( cstate.connState < CA_CONNECTED ) {
			UI_DrawProportionalString( 320, 192, cstate.messageString, UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, menu_text_color );
		}
	} else {
		UI_DrawProportionalString( 20, 16, va("Connecting to %s", cstate.servername), UI_SMALLFONT|UI_DROPSHADOW, menu_text_color );

		// display global MOTD at bottom
		UI_DrawProportionalString( 20, SCREEN_HEIGHT-32, 
			Info_ValueForKey( cstate.updateInfoString, "motd" ), UI_SMALLFONT|UI_DROPSHADOW, menu_text_color );
	
		// print any server info (server full, bad version, etc)
		if ( cstate.connState < CA_CONNECTED ) {
			UI_DrawProportionalString( 20, 192, cstate.messageString, UI_SMALLFONT|UI_DROPSHADOW, menu_text_color );
		}
	}
	
#if 0
	// display password field
	if ( passwordNeeded ) {
		s_ingame_menu.x = SCREEN_WIDTH * 0.50 - 128;
		s_ingame_menu.nitems = 0;
		s_ingame_menu.wrapAround = qtrue;

		passwordField.generic.type = MTYPE_FIELD;
		passwordField.generic.name = "Password:";
		passwordField.generic.callback = 0;
		passwordField.generic.x		= 10;
		passwordField.generic.y		= 180;
		Field_Clear( &passwordField.field );
		passwordField.width = 256;
		passwordField.field.widthInChars = 16;
		Q_strncpyz( passwordField.field.buffer, Cvar_VariableString("password"), 
			sizeof(passwordField.field.buffer) );

		Menu_AddItem( &s_ingame_menu, ( void * ) &s_customize_player_action );

		MField_Draw( &passwordField );
	}
#endif

	if ( lastConnState > cstate.connState ) {
		lastLoadingText[0] = '\0';
	}
	lastConnState = cstate.connState;

	switch ( cstate.connState ) {
	case CA_CONNECTING:
		s = va("Awaiting challenge...%i", cstate.connectPacketCount);
		break;
	case CA_CHALLENGING:
		s = va("Awaiting connection...%i", cstate.connectPacketCount);
		break;
	case CA_CONNECTED: {
		char downloadName[MAX_INFO_VALUE];

			trap_Cvar_VariableStringBuffer( "cl_downloadName", downloadName, sizeof(downloadName) );
			if (*downloadName) {
				UI_DisplayDownloadInfo( downloadName );
				return;
			}
		}
		s = "Awaiting gamestate...";

		// add to favorites now
		if ( uis.addToFavorites ) {
			int		res;

			res = trap_LAN_AddServer(AS_FAVORITES, cstate.servername, cstate.servername);
			if (res == 0) {
				// server already in the list
				Com_Printf("Favorite already in list\n");
				}
			else if (res == -1) {
				// list full
				Com_Printf("Favorite list full\n");
			}
			else {
				// successfully added
				Com_Printf("Added favorite server %s\n", cstate.servername);
			}
			uis.addToFavorites = qfalse;
		}
		break;
	case CA_LOADING:
		return;
	case CA_PRIMED:
		return;
	default:
		return;
	}

	UI_DrawProportionalString( 320, 128, s, UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, color_white );

	// password required / connection rejected information goes here
}


/*
===================
UI_KeyConnect
===================
*/
void UI_KeyConnect( int key ) {
	if ( key == K_ESCAPE ) {
		trap_Cmd_ExecuteText( EXEC_APPEND, "disconnect\n" );
		return;
	}
}
