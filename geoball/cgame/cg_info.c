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
// cg_info.c -- display information while data is being loading

#include "cg_local.h"

#define MAX_LOADING_PLAYER_ICONS	16
#define MAX_LOADING_ITEM_ICONS		26

static int			loadingPlayerIconCount;
static int			loadingItemIconCount;
static qhandle_t	loadingPlayerIcons[MAX_LOADING_PLAYER_ICONS];
static qhandle_t	loadingItemIcons[MAX_LOADING_ITEM_ICONS];


/*
===================
CG_DrawLoadingIcons
===================
*/
static void CG_DrawLoadingIcons( void ) {
	int		n;
	int		x, y;

	for( n = 0; n < loadingPlayerIconCount; n++ ) {
		x = 16 + n * 78;
		y = 324-40;
		CG_DrawPic( x, y, 64, 64, loadingPlayerIcons[n] );
	}

	for( n = 0; n < loadingItemIconCount; n++ ) {
		y = 400-40;
		if( n >= 13 ) {
			y += 40;
		}
		x = 16 + n % 13 * 48;
		CG_DrawPic( x, y, 32, 32, loadingItemIcons[n] );
	}
}


/*
======================
CG_LoadingString

======================
*/
void CG_LoadingString( const char *s ) {
	Q_strncpyz( cg.infoScreenText, s, sizeof( cg.infoScreenText ) );

	trap_UpdateScreen();
}

/*
===================
CG_LoadingItem
===================
*/
void CG_LoadingItem( int itemNum ) {
	gitem_t		*item;

	item = &bg_itemlist[itemNum];
	
	if ( item->icon && loadingItemIconCount < MAX_LOADING_ITEM_ICONS ) {
		loadingItemIcons[loadingItemIconCount++] = trap_R_RegisterShaderNoMip( item->icon );
	}

	CG_LoadingString( item->pickup_name );
}

/*
===================
CG_LoadingClient
===================
*/
void CG_LoadingClient( int clientNum ) {
	const char		*info;
	char			*skin;
	char			personality[MAX_QPATH];
	char			model[MAX_QPATH];
	char			iconName[MAX_QPATH];

	info = CG_ConfigString( CS_PLAYERS + clientNum );

	if ( loadingPlayerIconCount < MAX_LOADING_PLAYER_ICONS ) {
		Q_strncpyz( model, Info_ValueForKey( info, "model" ), sizeof( model ) );
		skin = Q_strrchr( model, '/' );
		if ( skin ) {
			*skin++ = '\0';
		} else {
			skin = "default";
		}

		Com_sprintf( iconName, MAX_QPATH, "models/players/%s/icon_%s.tga", model, skin );
		
		loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip( iconName );
		if ( !loadingPlayerIcons[loadingPlayerIconCount] ) {
			Com_sprintf( iconName, MAX_QPATH, "models/players/characters/%s/icon_%s.tga", model, skin );
			loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip( iconName );
		}
		if ( !loadingPlayerIcons[loadingPlayerIconCount] ) {
			Com_sprintf( iconName, MAX_QPATH, "models/players/%s/icon_%s.tga", DEFAULT_MODEL, "default" );
			loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip( iconName );
		}
		if ( loadingPlayerIcons[loadingPlayerIconCount] ) {
			loadingPlayerIconCount++;
		}
	}

	Q_strncpyz( personality, Info_ValueForKey( info, "n" ), sizeof(personality) );
	Q_CleanStr( personality );

	if( cgs.gametype == GT_SINGLE_PLAYER ) {
		trap_S_RegisterSound( va( "sound/player/announce/%s.wav", personality ), qtrue );
	}

	CG_LoadingString( personality );
}


/*
====================
CG_DrawInformation

Draw all the status / pacifier stuff during level loading
====================
*/
void CG_DrawInformation( void ) {
	const char	*s;
	const char	*info;
	const char	*sysInfo;
	int			y;
	int			value;
	qhandle_t	levelshot;
	qhandle_t	detail;
	char		buf[1024];

	info = CG_ConfigString( CS_SERVERINFO );
	sysInfo = CG_ConfigString( CS_SYSTEMINFO );

	s = Info_ValueForKey( info, "mapname" );
	levelshot = trap_R_RegisterShaderNoMip( va( "levelshots/%s.tga", s ) );
	if ( !levelshot ) {
		levelshot = trap_R_RegisterShaderNoMip( "menu/art/unknownmap" );
	}
	trap_R_SetColor( NULL );
	CG_DrawPic( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, levelshot );

	// blend a detail texture over it
	detail = trap_R_RegisterShader( "levelShotDetail" );
	trap_R_DrawStretchPic( 0, 0, cgs.glconfig.vidWidth, cgs.glconfig.vidHeight, 0, 0, 2.5, 2, detail );

	// draw the icons of things as they are loaded
	CG_DrawLoadingIcons();

	// the first 150 rows are reserved for the client connection
	// screen to write into
	if ( cg.infoScreenText[0] ) {
		UI_DrawProportionalString( 320, 128-32, va("Loading... %s", cg.infoScreenText),
			UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
	} else {
		UI_DrawProportionalString( 320, 128-32, "Awaiting snapshot...",
			UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
	}

	// draw info string information

	y = 180-32;

	// don't print server lines if playing a local game
	trap_Cvar_VariableStringBuffer( "sv_running", buf, sizeof( buf ) );
	if ( !atoi( buf ) ) {
		// server hostname
		Q_strncpyz(buf, Info_ValueForKey( info, "sv_hostname" ), 1024);
		Q_CleanStr(buf);
		UI_DrawProportionalString( 320, y, buf,
			UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
		y += PROP_HEIGHT;

		// pure server
		s = Info_ValueForKey( sysInfo, "sv_pure" );
		if ( s[0] == '1' ) {
			UI_DrawProportionalString( 320, y, "Pure Server",
				UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
			y += PROP_HEIGHT;
		}

		// server-specific message of the day
		s = CG_ConfigString( CS_MOTD );
		if ( s[0] ) {
			UI_DrawProportionalString( 320, y, s,
				UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
			y += PROP_HEIGHT;
		}

		// some extra space after hostname and motd
		y += 10;
	}

	// map-specific message (long map name)
	s = CG_ConfigString( CS_MESSAGE );
	if ( s[0] ) {
		UI_DrawProportionalString( 320, y, s,
			UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
		y += PROP_HEIGHT;
	}

	// cheats warning
	s = Info_ValueForKey( sysInfo, "sv_cheats" );
	if ( s[0] == '1' ) {
		UI_DrawProportionalString( 320, y, "CHEATS ARE ENABLED",
			UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
		y += PROP_HEIGHT;
	}

	// game type
	switch ( cgs.gametype ) {
	case GT_FFA:
		s = "Free For All";
		break;
	case GT_SINGLE_PLAYER:
		s = "Single Player";
		break;
	case GT_TOURNAMENT:
		s = "Tournament";
		break;
	case GT_TEAM:
		s = "Team Deathmatch";
		break;
	case GT_CTF:
		s = "Capture The Flag";
		break;
#ifdef MISSIONPACK
	case GT_1FCTF:
		s = "One Flag CTF";
		break;
	case GT_OBELISK:
		s = "Overload";
		break;
	case GT_HARVESTER:
		s = "Harvester";
		break;
#endif
	default:
		s = "Unknown Gametype";
		break;
	}
	UI_DrawProportionalString( 320, y, s,
		UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
	y += PROP_HEIGHT;
		
	value = atoi( Info_ValueForKey( info, "timelimit" ) );
	if ( value ) {
		UI_DrawProportionalString( 320, y, va( "timelimit %i", value ),
			UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
		y += PROP_HEIGHT;
	}

	if (cgs.gametype < GT_CTF ) {
		value = atoi( Info_ValueForKey( info, "fraglimit" ) );
		if ( value ) {
			UI_DrawProportionalString( 320, y, va( "fraglimit %i", value ),
				UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
			y += PROP_HEIGHT;
		}
	}

	if (cgs.gametype >= GT_CTF) {
		value = atoi( Info_ValueForKey( info, "capturelimit" ) );
		if ( value ) {
			UI_DrawProportionalString( 320, y, va( "capturelimit %i", value ),
				UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW, colorWhite );
			y += PROP_HEIGHT;
		}
	}
}
//newq3ball to the EOF!
/*
====================
ObjectivePrint_Line
====================
*/
/*static void ObjectivePrint_Line(int strIndex,int color,centity_t *cent)
{
	char *str,*strBegin;
	int y,pixelLen,charLen;
	char holdText[1024], holdText2[2];
	char finalText[MAX_OBJ_LENGTH];
	int len,len_s,maxPixLength,charHeight;

#ifndef Q3_VM
	assert(cgs.objectives[strIndex].text);
#endif
	str =  cgs.objectives[strIndex].text;

	len = strlen(str);
	len++;
	Q_strncpyz(finalText,str,len);
	len_s = strlen(str);

	pixelLen = UI_ProportionalStringWidth(finalText,UI_SMALLFONT);

	str = finalText;
	maxPixLength = SCREEN_WIDTH - (OBJ_TEXT_XPOS + OBJ_TEXT_X_BORDER_RIGHT + OBJ_CIRCLE_SIZE + OBJ_CIRCLE_TEXT_MARGIN);
	charHeight = OBJ_NORMAL_LINE_HEIGHT;

	if (missionYcnt)	// Not the very first objective to be printed?
	{
		missionYpos += (PROP_HEIGHT * 	OBJ_ADDITIONAL_LINE_HEIGHT); // Add a little space between objective lines
	}

	y =missionYpos + (charHeight * (missionYcnt));
	trap_R_SetColor( colorTable[color]);

	if (cgs.objectives[strIndex].complete)
	{
		CG_DrawPic( OBJ_TEXT_XPOS,y, OBJ_CIRCLE_SIZE,  OBJ_CIRCLE_SIZE,cgs.media.circle);
	}
	else
	{
		CG_DrawPic( OBJ_TEXT_XPOS,y, OBJ_CIRCLE_SIZE,  OBJ_CIRCLE_SIZE,cgs.media.circle2);
	}

	if (pixelLen < maxPixLength)	// One shot - small enough to print entirely on one line
	{
		UI_DrawProportionalString(OBJ_TEXT_XPOS + OBJ_CIRCLE_SIZE + OBJ_CIRCLE_TEXT_MARGIN, y,str, UI_SMALLFONT, colorTable[color] );
		++missionYcnt;
	}
	// Text is too long, break into lines.
	else
	{
		pixelLen = 0;
		charLen = 0;
		holdText2[1] = '\0';
		strBegin = str;

		while( *str ) 
		{
			holdText2[0] = *str;
			pixelLen += UI_ProportionalStringWidth(holdText2,UI_SMALLFONT);
			pixelLen += 2; // For kerning
			++charLen;

			if (pixelLen > maxPixLength ) 
			{	//Reached max length of this line
				//step back until we find a space
				while ((charLen) && (*str != ' ' ))
				{
					--str;
					--charLen;
				}

				if (*str==' ')
				{
					++str;	// To get past space
				}

				Q_strncpyz( holdText, strBegin, charLen);
				holdText[charLen] = '\0';
				strBegin = str;
				pixelLen = 0;
				charLen = 1;

				y = missionYpos + (charHeight * missionYcnt);

				UI_DrawProportionalString(OBJ_TEXT_XPOS + OBJ_CIRCLE_SIZE + OBJ_CIRCLE_TEXT_MARGIN, y, holdText, UI_SMALLFONT, colorTable[color] );
				++missionYcnt;
			} 
			else if (*(str+1) == '\0')
			{
				++charLen;

				y = missionYpos + (charHeight * missionYcnt);
				Q_strncpyz( holdText, strBegin, charLen);
				UI_DrawProportionalString(OBJ_TEXT_XPOS + OBJ_CIRCLE_SIZE + OBJ_CIRCLE_TEXT_MARGIN, y, holdText, UI_SMALLFONT, colorTable[color] );
				++missionYcnt;
				break;
			}
			++str; 
		} 
	}
}

static int Objective_LineCnt(int strIndex,centity_t *cent)
{
	char *str,*strBegin;
	int pixelLen,charLen;
	char holdText[1024], holdText2[2];
	char finalText[MAX_OBJ_LENGTH];
	int len,len_s,maxPixLength;
	int	lineCnt;

#ifndef Q3_VM
	assert(cgs.objectives[strIndex].text);
#endif
	str =  cgs.objectives[strIndex].text;

	len = strlen(str);
	len++;
	Q_strncpyz(finalText,str,len);
	len_s = strlen(str);

	pixelLen = UI_ProportionalStringWidth(finalText,UI_SMALLFONT);
	lineCnt = 0;

	maxPixLength = SCREEN_WIDTH - (OBJ_TEXT_XPOS + OBJ_TEXT_X_BORDER_RIGHT + OBJ_CIRCLE_SIZE + OBJ_CIRCLE_TEXT_MARGIN);

	str = finalText;

	if (pixelLen < maxPixLength)	// One shot - small enough to print entirely on one line
	{
		lineCnt = 1;
	}
	// Text is too long, break into lines.
	else
	{
		pixelLen = 0;
		charLen = 0;
		holdText2[1] = '\0';
		strBegin = str;

		while( *str ) 
		{
			holdText2[0] = *str;
			pixelLen += UI_ProportionalStringWidth(holdText2,UI_SMALLFONT);
			pixelLen += 2; // For kerning
			++charLen;

			if (pixelLen > maxPixLength ) 
			{	//Reached max length of this line
				//step back until we find a space
				while ((charLen) && (*str != ' ' ))
				{
					--str;
					--charLen;
				}

				if (*str==' ')
				{
					++str;	// To get past space
				}

				Q_strncpyz( holdText, strBegin, charLen);
				holdText[charLen] = '\0';
				strBegin = str;
				pixelLen = 0;
				charLen = 1;

				lineCnt++;
			} 
			else if (*(str+1) == '\0')
			{
				++charLen;
				lineCnt++;
				break;
			}
			++str; 
		} 
	}
	return (lineCnt);
}

*/
/*
====================
Objectives_Draw
====================
*/
/*
static void Objectives_Draw( centity_t *cent )
{
	int objCnt,i,lineCnt,maxLines;
	int	total,textYCnt,length,color;
	vec4_t newColor; 

	objCnt=0;
	for (i=0;i<MAX_OBJECTIVES;i++)
	{
		if (cgs.objectives[i].text[0])
		{
			objCnt++;
		}
	}

	lineCnt = 0;
	// Count the number of lines.
	for (i=0;i<objCnt;i++)
	{
		if (cgs.objectives[i].text[0])
		{
			lineCnt += Objective_LineCnt(i,cent);
		}
	}

	maxLines = OBJ_SCREEN_HEIGHT /OBJ_NORMAL_LINE_HEIGHT;
	if (lineCnt > maxLines)	// Too many lines?
	{
		lineCnt = maxLines;
	}

	if (lineCnt==0)	// Show there are no objectives
	{
		Q_strncpyz(cgs.objectives[0].text,ingame_text[IGT_NONETEXT],sizeof(cgs.objectives[0].text));
	}

	textYCnt = lineCnt * OBJ_NORMAL_LINE_HEIGHT;

	// For the space between objectives
	textYCnt +=	(objCnt-1) * (OBJ_ADDITIONAL_LINE_HEIGHT * PROP_HEIGHT);

	// Calc starting Y of text
	total = OBJ_SCREEN_YMARGIN + OBJ_SCREEN_YMARGIN + OBJ_SCREEN_Y2MARGIN + OBJ_SCREEN_Y2MARGIN +
		OBJ_SCREEN_YBORDERTOP + OBJ_SCREEN_YBORDERBOT + textYCnt;

	if (OBJ_SCREEN_HEIGHT < total)	// This should never happen (but just in case)
	{
		total = OBJ_SCREEN_HEIGHT;
	}

	missionInfoScreenY = ((OBJ_SCREEN_HEIGHT - total) /2) + (OBJ_SCREEN_YMARGIN + OBJ_SCREEN_YBORDERTOP);

	missionYcnt = 0;
	missionYpos = missionInfoScreenY;

	// Print top of frame
	trap_R_SetColor( colorTable[CT_DKPURPLE3]);
	CG_DrawPic( OBJ_HORIZONTAL_BORDER_X + 10,   missionInfoScreenY - (OBJ_SCREEN_YMARGIN + OBJ_SCREEN_YBORDERTOP) ,   
		SCREEN_WIDTH - (OBJ_HORIZONTAL_BORDER_X +  OBJ_TEXT_X_BORDER_RIGHT + 10), OBJ_SCREEN_YBORDERTOP, cgs.media.whiteShader);	// Middle column

	// Print bottom of frame
	CG_DrawPic( OBJ_HORIZONTAL_BORDER_X + 10,   missionInfoScreenY - OBJ_SCREEN_YMARGIN + textYCnt + (2 * OBJ_SCREEN_YMARGIN),   
		SCREEN_WIDTH - (OBJ_HORIZONTAL_BORDER_X +  OBJ_TEXT_X_BORDER_RIGHT + 10), OBJ_SCREEN_YBORDERBOT, cgs.media.whiteShader);	// Middle column

	length = (missionInfoScreenY - OBJ_SCREEN_YMARGIN + textYCnt + (2 * OBJ_SCREEN_YMARGIN)) - (missionInfoScreenY - (OBJ_SCREEN_YMARGIN + OBJ_SCREEN_YBORDERTOP)) - 15; 

	// Print left hand column of frame
	CG_DrawPic( OBJ_HORIZONTAL_BORDER_X,  (missionInfoScreenY - (OBJ_SCREEN_YMARGIN + OBJ_SCREEN_YBORDERTOP)) + 10,   
		OBJ_HORIZONTAL_BORDER_WIDTH, length, cgs.media.whiteShader);	// Middle column

	// Top corner
	trap_R_SetColor( colorTable[CT_DKPURPLE3]);
	CG_DrawPic( OBJ_HORIZONTAL_BORDER_X,  
		missionInfoScreenY - (OBJ_SCREEN_YMARGIN + OBJ_SCREEN_YBORDERTOP),   
		32, 32, cgs.media.corner_ul_20_30);	// Top corner

	// Bottom corner
	CG_DrawPic( OBJ_HORIZONTAL_BORDER_X,  
		(missionInfoScreenY - OBJ_SCREEN_YMARGIN + textYCnt + (2 * OBJ_SCREEN_YMARGIN))-5,   
		32, 32, cgs.media.corner_ll_8_30);	// Bottom corner

	UI_DrawProportionalString( OBJ_HORIZONTAL_BORDER_X + 30, missionInfoScreenY - (OBJ_SCREEN_YMARGIN + OBJ_SCREEN_YBORDERTOP) + 2, ingame_text[IGT_OBJECTIVES],UI_SMALLFONT, colorTable[CT_BLACK]);


	// Print the background
	newColor[0] = colorTable[CT_BLACK][0];
	newColor[1] = colorTable[CT_BLACK][1];
	newColor[2] = colorTable[CT_BLACK][2];
	newColor[3] = 0.5;
	trap_R_SetColor(newColor);
	CG_DrawPic( (OBJ_TEXT_XPOS - OBJ_TEXT_X_BORDER_LEFT), missionInfoScreenY - OBJ_SCREEN_YMARGIN, SCREEN_WIDTH - ((OBJ_TEXT_XPOS - OBJ_TEXT_X_BORDER_LEFT)+OBJ_TEXT_X_BORDER_RIGHT)  , textYCnt + (2 * OBJ_SCREEN_YMARGIN), cgs.media.whiteShader);	

	// Print the lines
	for (i=0;i<objCnt;i++)
	{
		if (cgs.objectives[i].text[0])
		{
			if (cgs.objectives[i].complete)
			{
				color = CT_DKGOLD1;
			}
			else
			{
				color = CT_VLTGOLD1;
			}

			ObjectivePrint_Line(i,color,cent);
		}
	}
}
*/
/*
====================
CG_DrawObjectiveInformation
====================
*/
/*void CG_DrawObjectiveInformation( void ) 
{
	centity_t *cent;

	// Don't show if dead
	if (cg.predictedPlayerState.pm_type == PM_DEAD)
	{
		return;
	}

	cent = &cg_entities[cg.snap->ps.clientNum];

	Objectives_Draw(cent);
}
*/
