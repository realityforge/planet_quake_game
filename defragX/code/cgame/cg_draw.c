// Copyright (C) 1999-2000 Id Software, Inc.
//
// cg_draw.c -- draw all of the graphical elements during
// active (after loading) gameplay

#include "cg_local.h"

char systemChat[256];
static void CG_DrawChs_OBs( void );

/*
==============
CG_DrawField
Draws numbers for status bar and powerups
==============
*/
static void CG_DrawField (int x, int y, int width, int value)
{
	char	num[16], *ptr;
	int		l;
	int		frame;

	if ( width < 1 ) {
		return;
	}

	// draw number string
	if ( width > 5 ) {
		width = 5;
	}

	switch ( width ) {
	case 1:
		value = value > 9 ? 9 : value;
		value = value < 0 ? 0 : value;
		break;
	case 2:
		value = value > 99 ? 99 : value;
		value = value < -9 ? -9 : value;
		break;
	case 3:
		value = value > 999 ? 999 : value;
		value = value < -99 ? -99 : value;
		break;
	case 4:
		value = value > 9999 ? 9999 : value;
		value = value < -999 ? -999 : value;
		break;
	}

	Com_sprintf (num, sizeof(num), "%i", value);
	l = strlen(num);
	if (l > width)
		l = width;
	x += 2 + CHAR_WIDTH*(width - l);

	ptr = num;
	while (*ptr && l)
	{
		if (*ptr == '-')
			frame = STAT_MINUS;
		else
			frame = *ptr -'0';

		CG_DrawPic( x,y, CHAR_WIDTH, CHAR_HEIGHT, cgs.media.numberShaders[frame] );
		x += CHAR_WIDTH;
		ptr++;
		l--;
	}
}

/*
================
CG_DrawStatusBar
================
*/
#define STATUSBAR_HEIGHT   30
#define ST_AMMO_POS         8
#define ST_HITEM_POS       48
#define ST_HEALTH_POS     128
#define ST_ARMOR_POS      256
#define ST_PICKUP_POS     480

static void CG_DrawStatusBar( void )
{
	int			color;
	centity_t	*cent;
	playerState_t	*ps;
	int			value;
	vec4_t		hcolor;
	vec3_t		angles;
	vec3_t		origin;
	float		y;

	static float colors[4][4] = {
		{ 1.0f, 1.0f, 1.0f, 1.0f },     // normal
		{ 1.0f, 0.0f, 0.0f, 1.0f },     // low health
		{ 0.5f, 0.5f, 0.5f, 1.0f },     // weapon firing
		{ 0.7f, 0.7f, 1.0f, 1.0f } };   // health > 100

	CG_FillRect(0, 448, SCREEN_WIDTH, STATUSBAR_HEIGHT, colorBlackTrans);

	y = cgs.screenYmax + 1 - ICON_SIZE - 8;

	cent = &cg_entities[cg.snap->ps.clientNum];
	ps = &cg.snap->ps;

	VectorClear( angles );

	//
	// ammo
	//
	if ( cent->currentState.weapon )
    {
        qhandle_t icon;
		value = ps->ammo[cent->currentState.weapon];
		if ( value != -1 ) {
			if ( cg.predictedPlayerState.weaponstate == WEAPON_FIRING && cg.predictedPlayerState.weaponTime > 100 )
            {
				// draw as dark grey when reloading
				color = 2;
			} else {
				if ( value >= 0 ) {
					color = 0;
				} else {
					color = 1;
				}
			}

			CG_SelectFont( 1 );
			CG_DrawString( ST_AMMO_POS + CHAR_WIDTH*3, y, va( "%i", value ), colors[ color ], CHAR_WIDTH, CHAR_HEIGHT, 0, DS_RIGHT | DS_PROPORTIONAL | DS_SHADOW );
			CG_SelectFont( 0 );

			trap_R_SetColor( NULL );

			// draw a 2D icon for ammo
            icon = cg_weapons[ cg.predictedPlayerState.weapon ].ammoIcon;
            if ( icon ) {
                CG_DrawPic( ST_AMMO_POS + CHAR_WIDTH*3 + TEXT_ICON_SPACE, y, ICON_SIZE, ICON_SIZE, icon );
            }

		}
	}

	//
	// health
	//
	value = ps->stats[STAT_HEALTH];
	if ( value > 100 ) {
		color = 3;
	} else if ( value > 25 ) {
		color = 0;
	} else if (value > 0) {
		color = (cg.time >> 8) & 1;	// flashing
	} else {
		color = 1;
	}

	CG_SelectFont( 1 );
	CG_DrawString( ST_HEALTH_POS + CHAR_WIDTH*3, y, va( "%i", value ), colors[ color ], CHAR_WIDTH, CHAR_HEIGHT, 0, DS_RIGHT | DS_PROPORTIONAL | DS_SHADOW );
	CG_SelectFont( 0 );

	CG_ColorForHealth( hcolor );
	trap_R_SetColor( hcolor );
    CG_DrawPic( ST_HEALTH_POS + CHAR_WIDTH*3 + TEXT_ICON_SPACE, y, ICON_SIZE + (ICON_SIZE/3), ICON_SIZE, cgs.media.healthIcon );

	//
	// armor
	//
	value = ps->stats[STAT_ARMOR];
    CG_SelectFont( 1 );
    CG_DrawString( ST_ARMOR_POS + CHAR_WIDTH*3, y, va( "%i", value ), colors[ color ], CHAR_WIDTH, CHAR_HEIGHT, 0, DS_RIGHT | DS_PROPORTIONAL );
    CG_SelectFont( 0 );

    trap_R_SetColor( NULL );
    CG_DrawPic( ST_ARMOR_POS + CHAR_WIDTH*3 + TEXT_ICON_SPACE, y, ICON_SIZE, ICON_SIZE, cgs.media.armorIcon );

    //
    // holdable
    //
	value = cg.snap->ps.stats[STAT_HOLDABLE_ITEM];
	if ( value )
    {
		CG_RegisterItemVisuals( value );
		CG_DrawPic( ST_HITEM_POS + CHAR_WIDTH*3 + TEXT_ICON_SPACE, y, ICON_SIZE, ICON_SIZE, cg_items[ value ].icon );
	}
}


/*
===========================================================================================
  UPPER RIGHT CORNER
===========================================================================================
*/


/*
==================
CG_DrawSnapshot
==================
*/
#ifdef DEVBUILD
static float CG_DrawSnapshot( float y )
{
	const char *s;

	s = va( "time:%i snap:%i cmd:%i", cg.snap->serverTime, cg.latestSnapshotNum, cgs.serverCommandSequence );

	CG_DrawString( cgs.screenXmax - 4, y + 2, s, colorWhite, BIGCHAR_WIDTH, BIGCHAR_HEIGHT, 0, DS_SHADOW | DS_RIGHT );

	return y + BIGCHAR_HEIGHT + 4;
}
#endif


/*
==================
CG_DrawChs_FPS
==================
*/
#define	FPS_FRAMES	4
static void CG_DrawChs_FPS( void )
{
	const char	*s;
	static int	previousTimes[FPS_FRAMES];
	static int	index;
	int		i, total;
	int		fps;
	static	int	previous;
	int		t, frameTime;
	int     x, y, col, fs;

	col = chs_fontColor.integer;
	fs = chs_fontSize.integer;
	x = chs_fpsX.integer;
	y = chs_fpsY.integer;
    if (col < 0)
        col = 0;

    if (col > 9)
        col = 9;

    if (fs < 4)
        fs = 4;

    if (fs > 16)
        fs = 16;

	// don't use serverTime, because that will be drifting to
	// correct for internet lag changes, timescales, timedemos, etc
	t = trap_Milliseconds();
	frameTime = t - previous;
	previous = t;

	previousTimes[index % FPS_FRAMES] = frameTime;
	index++;
	if ( index > FPS_FRAMES )
    {
		// average multiple frames together to smooth changes out a bit
		total = 0;
		for ( i = 0 ; i < FPS_FRAMES ; i++ ) {
			total += previousTimes[i];
		}

		if ( !total )
			total = 1;

		fps = 1000 * FPS_FRAMES / total;

		s = va( "%ifps", fps );
		CG_DrawString( x, y, s, g_color_table[col], fs, fs, 0, DS_SHADOW | DS_RIGHT | DS_PROPORTIONAL );
	}
}


/*
=================
CG_DrawChs_LevelTimer
=================
*/
static void CG_DrawChs_LevelTimer( void )
{
	const char	*s;
	int			mins, seconds;
	int			msec;
    int         x, y, col, fs;

    col = chs_fontColor.integer;
    fs = chs_fontSize.integer;
    x = chs_levelTimerX.integer;
    y = chs_levelTimerY.integer;

    if (col < 0)
        col = 0;
    if (col > 9)
        col = 9;

    if (fs < 4)
        fs = 4;
    if (fs > 16)
        fs = 16;

	msec = cg.time - cgs.levelStartTime;

	seconds = msec / 1000;
	mins = seconds / 60;
	seconds -= mins * 60;

	s = va( "%i:%02i", mins, seconds );
	CG_DrawString( x, y, s, g_color_table[col], fs, fs, 0, DS_SHADOW | DS_RIGHT | DS_PROPORTIONAL );
}

/*
=====================
CG_DrawUpperRight
=====================
*/
static void CG_DrawUpperRight(stereoFrame_t stereoFrame)
{
	float	y = cgs.screenYmin;

#ifdef DEVBUILD
	if ( cg_drawSnapshot.integer )
		y = CG_DrawSnapshot( y );
#endif

	if (chs_fps.integer && (stereoFrame == STEREO_CENTER || stereoFrame == STEREO_RIGHT))
		CG_DrawChs_FPS();

	if ( chs_levelTimer.integer && cgs.gametype != GT_ADVENTURE)
		CG_DrawChs_LevelTimer();
}


/*
===========================================================================================
  LOWER RIGHT CORNER
===========================================================================================
*/


/*
================
CG_DrawPowerups
================
*/
static float CG_DrawPowerups( float y )
{
	int		        sorted[MAX_POWERUPS];
	int		        sortedTime[MAX_POWERUPS];
	int		        i, j, k, t, x, active;
	float	        size, f;
	playerState_t	*ps;
	gitem_t	        *item;

	if (cgs.gametype == GT_FASTCAP)
		return y;

	ps = &cg.snap->ps;
	if ( ps->stats[STAT_HEALTH] <= 0 )
		return y;

	// sort the list by time remaining
	active = 0;
	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
		if ( !ps->powerups[ i ] )
			continue;

		t = ps->powerups[ i ] - cg.time;
		if ( t < 0 )
			continue;

		// insert into the list
		for ( j = 0 ; j < active ; j++ ) {
			if ( sortedTime[j] >= t ) {
				for ( k = active - 1 ; k >= j ; k-- ) {
					sorted[k+1] = sorted[k];
					sortedTime[k+1] = sortedTime[k];
				}
				break;
			}
		}
		sorted[j] = i;
		sortedTime[j] = t;
		active++;
	}

	//y += 0;
	y -= ICON_SIZE;
	// draw the icons and timers
	x = cgs.screenXmax + 1 - ICON_SIZE - (CHAR_WIDTH * 2);
	for ( i = 0 ; i < active ; i++ ) {
		item = BG_FindItemForPowerup( sorted[i] );

		if ( item )
        {
			y -= ICON_SIZE;
            CG_FillRect(x, y, cgs.screenXmax - x, ICON_SIZE, colorBlackTrans );

			trap_R_SetColor( colorWhite );
			CG_DrawField( x, y, 2, sortedTime[ i ] / 1000 );

			trap_R_SetColor( NULL );

			size = ICON_SIZE;

			CG_DrawPic( cgs.screenXmax + 1 - size, y + ICON_SIZE / 2 - size / 2, size, size, trap_R_RegisterShader( item->icon ) );
		}

	}
	trap_R_SetColor( NULL );

	return y;
}



/*
=====================
CG_DrawLowerRight

=====================
*/
static void CG_DrawLowerRight( void )
{
	float	y;
	y = cgs.screenYmax + 1 - STATUSBAR_HEIGHT;
	y = CG_DrawPowerups( y );
}


/*
===================
CG_DrawPickupItem
===================
*/
static int CG_DrawPickupItem( int y )
{
	int		value;
	float	*fadeColor;
	const char *text;

	if ( cg.snap->ps.stats[STAT_HEALTH] <= 0 )
		return y;

	if ( !cg_drawPickup.integer )
        return y;

	y = cgs.screenYmax + 1 - ICON_SIZE - 8;

	value = cg.itemPickup;
	if ( value ) {
		fadeColor = CG_FadeColorTime( cg.itemPickupTime, 5000, 500 );
		if ( fadeColor ) {
			CG_RegisterItemVisuals( value );
			trap_R_SetColor( fadeColor );
			CG_DrawPic( ST_PICKUP_POS + CHAR_WIDTH*3 + TEXT_ICON_SPACE, y, ICON_SIZE, ICON_SIZE, cg_items[ value ].icon );
			text = bg_itemlist[ value ].pickup_name;
			CG_DrawString( ST_PICKUP_POS + CHAR_WIDTH*3 , y, text, fadeColor, CHAR_WIDTH, CHAR_HEIGHT, 16, DS_RIGHT | DS_PROPORTIONAL | DS_SHADOW );
			trap_R_SetColor( NULL );
		} else {
			cg.itemPickupCount = 0;
		}
	}

	return y;
}



/*
=====================
CG_DrawLowerLeft
=====================
*/
static void CG_DrawLowerLeft( void )
{
	float	y;

	y = cgs.screenYmax + 1 - STATUSBAR_HEIGHT;
	y = CG_DrawPickupItem( y );
}

/*
===============================================================================
LAGOMETER
===============================================================================
*/

#define	LAG_SAMPLES		128


typedef struct {
	int		frameSamples[LAG_SAMPLES];
	int		frameCount;
	int		snapshotFlags[LAG_SAMPLES];
	int		snapshotSamples[LAG_SAMPLES];
	int		snapshotCount;
} lagometer_t;

lagometer_t		lagometer;

/*
==============
CG_AddLagometerFrameInfo

Adds the current interpolate / extrapolate bar for this frame
==============
*/
void CG_AddLagometerFrameInfo( void )
{
	int		offset;

	offset = cg.time - cg.latestSnapshotTime;
	lagometer.frameSamples[ lagometer.frameCount & ( LAG_SAMPLES - 1) ] = offset;
	lagometer.frameCount++;
}


/*
==============
CG_AddLagometerSnapshotInfo

Each time a snapshot is received, log its ping time and
the number of snapshots that were dropped before it.

Pass NULL for a dropped packet.
==============
*/
void CG_AddLagometerSnapshotInfo( snapshot_t *snap )
{
	// dropped packet
	if ( !snap )
    {
		lagometer.snapshotSamples[ lagometer.snapshotCount & ( LAG_SAMPLES - 1) ] = -1;
		lagometer.snapshotCount++;
		return;
	}

	// add this snapshot's info
	lagometer.snapshotSamples[ lagometer.snapshotCount & ( LAG_SAMPLES - 1) ] = snap->ping;
	lagometer.snapshotFlags[ lagometer.snapshotCount & ( LAG_SAMPLES - 1) ] = snap->snapFlags;
	lagometer.snapshotCount++;
}


/*
==============
CG_DrawDisconnect

Should we draw something differnet for long lag vs no packets?
==============
*/
static void CG_DrawDisconnect( void )
{
	float		x, y;
	int			cmdNum;
	usercmd_t	cmd;
	const char	*s;

	// draw the phone jack if we are completely past our buffers
	cmdNum = trap_GetCurrentCmdNumber() - CMD_BACKUP + 1;
	trap_GetUserCmd( cmdNum, &cmd );
	if ( cmd.serverTime <= cg.snap->ps.commandTime
		|| cmd.serverTime > cg.time ) {	// special check for map_restart // bk 0102165 - FIXME
		return;
	}

	// also add text in center of screen
	s = "Connection Interrupted";
	CG_DrawString( 320, cgs.screenYmin + 100, s, colorWhite, BIGCHAR_WIDTH, BIGCHAR_HEIGHT, 0, DS_SHADOW | DS_PROPORTIONAL | DS_CENTER );

	// blink the icon
	if ( ( cg.time >> 9 ) & 1 ) {
		return;
	}

	x = cgs.screenXmax + 1 - 48;
	y = cgs.screenYmax + 1 - 48;

	CG_DrawPic( x, y, 48, 48, trap_R_RegisterShader( "gfx/2d/net.tga" ) );
}


#define	MAX_LAGOMETER_PING	900
#define	MAX_LAGOMETER_RANGE	300

/*
==============
CG_DrawLagometer
==============
*/
static void CG_DrawLagometer( void ) {
	int		a, x, y, i;
	float	v;
	float	ax, ay, aw, ah, mid, range;
	int		color;
	float	vscale;

	if ( !cg_lagometer.integer || cgs.localServer ) {
		CG_DrawDisconnect();
		return;
	}

	//
	// draw the graph
	x = cgs.screenXmax + 1 - 48;
	y = cgs.screenYmax + 1 - 48;

	trap_R_SetColor( NULL );
	CG_DrawPic( x, y, 48, 48, cgs.media.lagometerShader );

	ax = x;
	ay = y;
	aw = 48;
	ah = 48;
	CG_AdjustFrom640( &ax, &ay, &aw, &ah );

	color = -1;
	range = ah / 3;
	mid = ay + range;

	vscale = range / MAX_LAGOMETER_RANGE;

	// draw the frame interpoalte / extrapolate graph
	for ( a = 0 ; a < aw ; a++ ) {
		i = ( lagometer.frameCount - 1 - a ) & (LAG_SAMPLES - 1);
		v = lagometer.frameSamples[i];
		v *= vscale;
		if ( v > 0 ) {
			if ( color != 1 ) {
				color = 1;
				trap_R_SetColor( g_color_table[ColorIndex(COLOR_YELLOW)] );
			}
			if ( v > range ) 
				v = range;

			trap_R_DrawStretchPic ( ax + aw - a, mid - v, 1, v, 0, 0, 0, 0, cgs.media.whiteShader );
		} else if ( v < 0 ) {
			if ( color != 2 ) {
				color = 2;
				trap_R_SetColor( g_color_table[ColorIndex(COLOR_BLUE)] );
			}
			v = -v;
			if ( v > range )
				v = range;

			trap_R_DrawStretchPic( ax + aw - a, mid, 1, v, 0, 0, 0, 0, cgs.media.whiteShader );
		}
	}

	// draw the snapshot latency / drop graph
	range = ah / 2;
	vscale = range / MAX_LAGOMETER_PING;

	for ( a = 0 ; a < aw ; a++ ) {
		i = ( lagometer.snapshotCount - 1 - a ) & (LAG_SAMPLES - 1);
		v = lagometer.snapshotSamples[i];
		if ( v > 0 ) {
			if ( lagometer.snapshotFlags[i] & SNAPFLAG_RATE_DELAYED ) {
				if ( color != 5 ) {
					color = 5;	// YELLOW for rate delay
					trap_R_SetColor( g_color_table[ColorIndex(COLOR_YELLOW)] );
				}
			} else {
				if ( color != 3 ) {
					color = 3;
					trap_R_SetColor( g_color_table[ColorIndex(COLOR_GREEN)] );
				}
			}
			v = v * vscale;
			if ( v > range )
				v = range;

			trap_R_DrawStretchPic( ax + aw - a, ay + ah - v, 1, v, 0, 0, 0, 0, cgs.media.whiteShader );
		} else if ( v < 0 ) {
			if ( color != 4 ) {
				color = 4;		// RED for dropped snapshots
				trap_R_SetColor( g_color_table[ColorIndex(COLOR_RED)] );
			}
			trap_R_DrawStretchPic( ax + aw - a, ay + ah - range, 1, range, 0, 0, 0, 0, cgs.media.whiteShader );
		}
	}

	trap_R_SetColor( NULL );

	if ( cg_nopredict.integer || cgs.synchronousClients ) 
		CG_DrawString( cgs.screenXmax-1, y, "snc", colorWhite, 5, 10, 0, DS_PROPORTIONAL | DS_RIGHT );

	if ( !cg.demoPlayback ) 
		CG_DrawString( x+1, y, va( "%ims", cg.meanPing ), colorWhite, 5, 10, 0, DS_PROPORTIONAL );

	CG_DrawDisconnect();
}



/*
===============================================================================

CENTER PRINTING

===============================================================================
*/


/*
==============
CG_CenterPrint

Called for important messages that should stay in the center of the screen
for a few moments
==============
*/
void CG_CenterPrint( const char *str, int y, int charWidth )
{
	char	*s;

	Q_strncpyz( cg.centerPrint, str, sizeof(cg.centerPrint) );

	cg.centerPrintTime = cg.time;
	cg.centerPrintY = y;
	cg.centerPrintCharWidth = charWidth;

	// count the number of lines for centering
	cg.centerPrintLines = 1;
	s = cg.centerPrint;
	while( *s )
    {
		if (*s == '\n')
			cg.centerPrintLines++;

		s++;
	}
}


/*
===================
CG_DrawCenterString
===================
*/
static void CG_DrawCenterString( void )
{
	char	*start;
	int		l;
	int		y;
	float	*color;

	if ( !cg.centerPrintTime ) {
		return;
	}

	color = CG_FadeColor( cg.centerPrintTime, 1000 * cg_centertime.value );
	if ( !color )
		return;

	trap_R_SetColor( color );

	start = cg.centerPrint;

	y = cg.centerPrintY - cg.centerPrintLines * BIGCHAR_HEIGHT / 2;

	while ( 1 ) {
		char linebuffer[1024];

		for ( l = 0; l < 50; l++ ) {
			if ( !start[l] || start[l] == '\n' ) {
				break;
			}
			linebuffer[l] = start[l];
		}
		linebuffer[l] = 0;

		CG_DrawString( 320, y, linebuffer, color, cg.centerPrintCharWidth, cg.centerPrintCharWidth * 1.5, 0, DS_SHADOW | DS_CENTER | DS_PROPORTIONAL );

		y += cg.centerPrintCharWidth * 1.5;

		while ( *start && ( *start != '\n' ) ) {
			start++;
		}
		if ( !*start ) {
			break;
		}
		start++;
	}

	trap_R_SetColor( NULL );
}

/*
    DEFRAG TIMERS
*/

static void CG_DrawDFTimer( void )
{
    float x, y, w, h;
	int time;
	int col, col2;

    if ( cgs.gametype == GT_FREESTYLE || cgs.gametype == GT_ADVENTURE || (!hud_timer.integer) )
        return;

	time = (cg.predictedPlayerState.stats[STAT_DFTIMERHI] << 16) |
           (cg.predictedPlayerState.stats[STAT_DFTIMERLO] & 0xffff);

	if ( time == 0 )
        return;

    col  = hud_fontColor.integer;
    col2 = hud_frameColor.integer;

    if (col < 0)
        col = 0;

    if (col > 9)
        col = 9;

    if (col2 < 0)
        col2 = 0;

    if (col2 > 9)
        col2 = 9;

    x = hud_timerX.value;
    y = hud_timerY.value;

    if ( x > cgs.glconfig.vidWidth )
        x = cgs.glconfig.vidWidth;

    if ( y > cgs.glconfig.vidHeight )
        y = cgs.glconfig.vidHeight;

    w = hud_fontSize.integer * 12;
    h = hud_fontSize.integer * 2;

    CG_AdjustFrom640(&x, &y, &w, &h);
    // draw a frame
	trap_R_SetColor( g_color_table[col2+10] );
    trap_R_DrawStretchPic(x, y, w, h, 0, 0, 1, 1, cgs.media.timerFrame);
    // then clock lil offset with shadow
	trap_R_SetColor( g_color_table[0] );
    trap_R_DrawStretchPic(x + 8, y + 8, 24, 24, 0, 0, 1, 1, cgs.media.clockIcon);

    trap_R_SetColor( g_color_table[col] );
    trap_R_DrawStretchPic(x + 6, y + 6, 24, 24, 0, 0, 1, 1, cgs.media.clockIcon);

	// print the timer
	CG_DrawString( hud_timerX.value + ICON_SIZE, hud_timerY.value + (hud_fontSize.integer/2), FormatDFTimer( time, qfalse ), g_color_table[col], hud_fontSize.integer, hud_fontSize.integer, 10, DS_FORCE_COLOR | DS_SHADOW | DS_PROPORTIONAL );
}

#define CP_Y    420
static void CG_DrawDFCheckpoints( void )
{
	char *s;
	int i = 0, ws, time, prev = 0;
	float x, y, w, h;
	int col;

    if ( cgs.gametype == GT_ADVENTURE || cgs.gametype == GT_FASTCAP || cgs.gametype == GT_FREESTYLE )
        return;

    col = hud_fontColor.integer;
    if (col < 0)
        col = 0;

    if (col > 9)
        col = 9;

	if ( cg.currentCheckpoint > 0 ) {
		prev = 0;

		for ( i = 0; i < cg.currentCheckpoint; i++ )
        {
			if ( cg.bestCheckpoints[i] )
				s = FormatDFTimer( cg.bestCheckpoints[i] - cg.checkpoints[i], qtrue );
			else
				s = FormatDFTimer( cg.checkpoints[i] - prev, qfalse );

			prev = cg.checkpoints[i];

			if ( cg.currentCheckpoint - i > 8 )
				continue;

			w = hud_fontSize.integer * 12;
            ws = CG_DrawStrlen( s ) * hud_fontSize.integer;
            h = hud_fontSize.integer * 2;
            x = 640 - hud_fontSize.integer * 12;
            y = CP_Y - (i * ( hud_fontSize.integer * 2 ) );

            CG_AdjustFrom640( &x, &y, &w, &h );

			trap_R_SetColor( g_color_table[hud_frameColor.integer + 10] );
			trap_R_DrawStretchPic(x, y, w, h, 0, 0, 1, 1, cgs.media.checkpointFrame);

			CG_DrawString( 640 - (hud_fontSize.integer * 11) , CP_Y - (i * (hud_fontSize.integer * 2)) + (hud_fontSize.integer / 2) , s,  g_color_table[col], hud_fontSize.integer, hud_fontSize.integer, 10, DS_SHADOW | DS_PROPORTIONAL );
		}
	}

    time = (cg.predictedPlayerState.stats[STAT_DFTIMERHI] << 16) |
           (cg.predictedPlayerState.stats[STAT_DFTIMERLO] & 0xffff);

	if ( time == 0 )
		return;

	if ( cg.bestCheckpoints[i] ) {
		s = FormatDFTimer( cg.bestCheckpoints[i] - time, qtrue );
	} else {
		if ( time - prev  == 0 )
			return;

		s = FormatDFTimer( time - prev, qfalse );
	}

	w = CG_DrawStrlen( s ) * hud_fontSize.integer;
	CG_DrawString( 635 - w, CP_Y - (i * (hud_fontSize.integer * 2)) + (hud_fontSize.integer / 2), s,  g_color_table[col], hud_fontSize.integer, hud_fontSize.integer, 16, DS_CENTER | DS_SHADOW | DS_PROPORTIONAL );
}

float simFall( float z, float vel )
{
	float gravity = cg.predictedPlayerState.gravity;
	float frametime = (float)cgs.pmove_msec / 1000.0f;
	float oldz = 0, oldvel = 0;
	vec3_t	vec = {0, 0, 0};
	int frame = 0; // force stop in case we somehow get into an infinte loop

	if ( gravity < 1 )
		gravity = 800;

	if (z < 0 )
		return 0;

	while ( z > 0.0f && frame++ < 16384) {
		oldz = z;
		oldvel = vel;

		vel -= gravity * frametime;
		z += (vel + oldvel) * 0.5 * frametime;

		vec[2] = vel;
		trap_SnapVector( vec );
		vel = vec[2];
	}

	return oldz;
}

#define	JUMP_VELOCITY	270
static void CG_DrawChs_OBs( void )
{
	char    buf[MAX_STRING_CHARS];
    //char    obs[MAX_STRING_CHARS];
	vec3_t	start, end;
	vec3_t	dir = {0, 0, -1};
	vec3_t	mins = {-15, -15, -24};
	vec3_t	maxs = {15, 15, 16};
	vec_t	viewdistGo = 0, viewdistJump = 0, falldist;
	trace_t	trace;
	playerState_t *ps;

    if ( (!chs_ob.integer) || (cgs.gametype == GT_ADVENTURE) || (!cgs.obs) )
		return;

    //trap_Cvar_VariableStringBuffer("dfx_obs", obs, sizeof(obs));

    //if ( !atoi(obs) )
    //    return;

    if ( cgs.synchronousClients ) {
		ps = &cg.snap->ps;
	} else {
		ps = &cg.predictedPlayerState;
	}

	VectorCopy( ps->origin, start );
	VectorMA( start, 4096, dir, end );
	CG_Trace( &trace, start, mins, maxs, end, 0, CONTENTS_SOLID | CONTENTS_PLAYERCLIP);
	if ( start[2] - trace.endpos[2] >= 0.25 )
		falldist = simFall( start[2] - trace.endpos[2], ps->velocity[2] );

	AngleVectors( ps->viewangles, dir, NULL, NULL );
	VectorMA( start, 14, dir, start );
	start[2] += ps->viewheight;
	VectorMA( start, 4096, dir, end );
	CG_Trace( &trace, start, vec3_origin, vec3_origin, end, 0, CONTENTS_SOLID | CONTENTS_PLAYERCLIP);

	if ( trace.plane.normal[2] >= 0.7 ) {
        viewdistGo = simFall( -(trace.endpos[2] - ps->origin[2] + 24 ), 0);
		viewdistJump = simFall( -(trace.endpos[2] - ps->origin[2] + 24 ), JUMP_VELOCITY);
	}

	strcpy(buf, "");

    if (ps->groundEntityNum == ENTITYNUM_NONE) {

        if ( falldist < 0.25 && falldist > 0.0 ) {
            Q_strcat( buf, sizeof( buf ), "B" );
        } else {
            Q_strcat( buf, sizeof( buf ), " " );
        }

    } else if ( ps->groundEntityNum != ENTITYNUM_NONE ) {

		if ( viewdistGo < 0.25 && viewdistGo > 0.0 ) {
			Q_strcat( buf, sizeof( buf ), "G" );
		} else {
			Q_strcat( buf, sizeof( buf ), " " );
		}

		if ( viewdistJump < 0.25 && viewdistJump > 0.0 ) {
			Q_strcat( buf, sizeof( buf ), "J" );
		} else {
			Q_strcat( buf, sizeof( buf ), " " );
		}
	}

	CG_DrawString( chs_obX.integer, chs_obY.integer, buf, g_color_table[chs_fontColor.integer], chs_fontSize.integer, chs_fontSize.integer, 0, DS_CENTER | DS_SHADOW );
}


/*
================================================================================

CROSSHAIR

================================================================================
*/


/*
=================
CG_DrawCrosshair
=================
*/
static void CG_DrawCrosshair( void )
{
	float		w, h;
	qhandle_t	hShader;
	float		f;
	float		x = 0, y = 0;
	int			ca;

	if ( !cg_drawCrosshair.integer )
		return;

	if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR)
		return;

	if ( cg.renderingThirdPerson && cgs.gametype != GT_ADVENTURE )  // allow using crosshair in thirdperson in GT_ADVENTURE
		return;

    if ( cgs.gametype == GT_ADVENTURE ) {   // only crosshair for certain weaps in GT_ADVENTURE
        switch ( cg.snap->ps.weapon ) {
            case WP_GAUNTLET:
            case WP_MACHINEGUN:
            case WP_GRENADE_LAUNCHER:
            case WP_NONE:
            case WP_ROCKET_LAUNCHER:
            case WP_SHOTGUN:
                return;
        }

        if ( !cg.zoomed )       // only crosshair when zooming
            return;
    }

	// set color based on health
	if ( cg_crosshairHealth.integer || cgs.gametype == GT_ADVENTURE)
    {
		vec4_t	hcolor;

		CG_ColorForHealth( hcolor );
		trap_R_SetColor( hcolor );
	}
	else // set color chosen manually
    {
        int col;

        col = cg_crosshairColor.integer;
        if (col < 0)
            col = 0;

        if (col > 9)
            col = 9;


		trap_R_SetColor( g_color_table[col] );
	}

	w = h = cg_crosshairSize.value;

	// pulse the size of the crosshair when picking up items
	f = cg.time - cg.itemPickupBlendTime;
	if ( f > 0 && f < ITEM_BLOB_TIME ) {
		f /= ITEM_BLOB_TIME;
		w *= ( 1 + f );
		h *= ( 1 + f );
	}

	CG_AdjustFrom640( &x, &y, &w, &h );

	ca = cg_drawCrosshair.integer;
	if ( ca < 0 )
		ca = 0;

	if ( ca > NUM_CROSSHAIRS )
        ca = NUM_CROSSHAIRS;

	if (cgs.gametype != GT_ADVENTURE)
        hShader = cgs.media.crosshairShader[ ca % NUM_CROSSHAIRS ];
    else
        hShader = cgs.media.crosshairShader[ 1 ];

	trap_R_DrawStretchPic(  x + cg.refdef.x + 0.5 * (cg.refdef.width - w) - cgs.screenXBias,
                            y + cg.refdef.y + 0.5 * (cg.refdef.height - h) - cgs.screenYBias,
                            w, h, 0, 0, 1, 1, hShader );
}



/*
=================
CG_ScanForCrosshairEntity
=================
*/
static void CG_ScanForCrosshairEntity( void )
{
	trace_t		trace;
	vec3_t		start, end;
	int			content;

	VectorCopy( cg.refdef.vieworg, start );
	VectorMA( start, 131072, cg.refdef.viewaxis[0], end );

	CG_Trace( &trace, start, vec3_origin, vec3_origin, end,	cg.snap->ps.clientNum, CONTENTS_SOLID|CONTENTS_BODY );
	if ( trace.entityNum >= MAX_CLIENTS )
		return;

	// if the player is in fog, don't show it
	content = CG_PointContents( trace.endpos, 0 );
	if ( content & CONTENTS_FOG )
		return;

	// if the player is invisible, don't show it
	if ( cg_entities[ trace.entityNum ].currentState.powerups & ( 1 << PW_INVIS ) )
		return;

	// update the fade timer
	cg.crosshairClientNum = trace.entityNum;
	cg.crosshairClientTime = cg.time;
}


/*
=====================
CG_DrawCrosshairNames
=====================
*/
static void CG_DrawCrosshairNames( void )
{
	float		*color;
	const char	*name;

	if ( !cg_drawCrosshair.integer )
		return;

	if ( !cg_drawCrosshairNames.integer )
		return;

	if ( cg.renderingThirdPerson )
		return;

    if ( cgs.gametype == GT_ADVENTURE )
        return;

	// scan the known entities to see if the crosshair is sighted on one
	CG_ScanForCrosshairEntity();

	// draw the name of the player being looked at
	color = CG_FadeColor( cg.crosshairClientTime, 1000 );
	if ( !color ) {
		trap_R_SetColor( NULL );
		return;
	}

	name = cgs.clientinfo[ cg.crosshairClientNum ].name;
	color[3] *= 0.5f;
	CG_DrawString( 320, 174, name, color, BIGCHAR_WIDTH, BIGCHAR_HEIGHT, 0, DS_CENTER | DS_PROPORTIONAL );
	trap_R_SetColor( NULL );
}


//==============================================================================


/*
=================
CG_DrawVote
=================
*/
static void CG_DrawVote( void )
{
	char	*s;
	int		sec;

	if ( !cgs.voteTime ) {
		return;
	}

	// play a talk beep whenever it is modified
	if ( cgs.voteModified ) {
		cgs.voteModified = qfalse;
		trap_S_StartLocalSound( cgs.media.talkSound, CHAN_LOCAL_SOUND );
	}

	sec = ( VOTE_TIME - ( cg.time - cgs.voteTime ) ) / 1000;
	if ( sec < 0 ) {
		sec = 0;
	}

	s = va( "VOTE(%i):%s yes:%i no:%i", sec, cgs.voteString, cgs.voteYes, cgs.voteNo );
	CG_DrawString( cgs.screenXmin - 0, 58, s, colorWhite, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0, DS_PROPORTIONAL|DS_SHADOW );

}


/*
=================
CG_DrawIntermission
=================
*/
static void CG_DrawIntermission( void )
{
// SLK: fixme?
#if 0
	if ( cgs.gametype == GT_ADVENTURE ) {
		CG_DrawCenterString();
		return;
	}
#endif
	cg.scoreFadeTime = cg.time;
	cg.scoreBoardShowing = CG_DrawScoreboard();
}


/*
=================
CG_DrawFollow
=================
*/
static qbool CG_DrawFollow( void )
{
	const char	*name;

	if ( !(cg.snap->ps.pm_flags & PMF_FOLLOW) )
		return qfalse;

	CG_DrawString( 320, cgs.screenYmin + 24, "following", colorWhite, SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, 0, DS_CENTER | DS_SHADOW );

	name = cgs.clientinfo[ cg.snap->ps.clientNum ].name;
	CG_DrawString( 320, cgs.screenYmin + 40, name, colorWhite, BIGCHAR_WIDTH, BIGCHAR_HEIGHT, 0, DS_FORCE_COLOR | DS_SHADOW | DS_CENTER );

	return qtrue;
}


/*
=================
CG_DrawWarmup
=================
*/
static void CG_DrawWarmup( void )
{
	vec4_t	colorGreenTrans = {0.2, 1.0, 0, 0.5};
	if ( !cg.warmup || cg.warmupCount <= 0 )
		return;

	CG_DrawString( 320, 240 - (GIANTCHAR_HEIGHT / 2), va( "%i", cg.warmupCount ), colorGreenTrans, GIANTCHAR_WIDTH, GIANTCHAR_HEIGHT, 0, DS_CENTER | DS_SHADOW | DS_PROPORTIONAL | DS_FORCE_COLOR );
}


//==================================================================================
//
//  ***************** C G A Z     H U D *****************************
//
//==================================================================================

void CG_FillAngleYaw( float start, float end, float viewangle, float y, float height, vec4_t color )
{
    float scale, x[2], diff[2];
    int i;

    diff[0] = viewangle - start;
    diff[1] = viewangle - end;

    //http://stackoverflow.com/questions/24234609/standard-way-to-normalize-an-angle-to-%CF%80-radians-in-java
    //normalize angles for comparison
    for(i = 0; i < 2; i++)
        diff[i] = diff[i] - 360 * floor((diff[i] + 180) / 360);

    //dont draw if completely out of view
    if(fabs(diff[0]) > cg.refdef.fov_x / 2 && fabs(diff[1]) > cg.refdef.fov_x / 2)
        return;

    //clamp to edges of fov
    for(i = 0; i < 2; i++)
        if(fabs(diff[i]) > cg.refdef.fov_x / 2) diff[i] = (cg.refdef.fov_x / 2) * (diff[i] >= 0 ? 1 : -1);

    scale = (1/tan(DEG2RAD(cg.refdef.fov_x/2))) * (SCREEN_WIDTH/2);
    x[0] = SCREEN_WIDTH/2 + tan(DEG2RAD(diff[0])) * scale;
    x[1] = SCREEN_WIDTH/2 + tan(DEG2RAD(diff[1])) * scale;

    CG_FillRect(MIN(x[0], x[1]), y, fabs(x[0] - x[1]), height, color);
}


/*
=================
CG_DrawCgaz
=================
*/
extern float pm_accelerate;
extern float pm_friction;
extern float pm_aircontrol;

static void CG_DrawCgaz( void )
{

    playerState_t *ps = &cg.predictedPlayerState;
    usercmd_t      cmd;
    int            cmdnum;

    vec4_t green     = {0, 1, 0, 0.5};
    vec4_t darkgreen = {0, 0.25, 0.25, 0.5};
    vec4_t yellow    = {1, 1, 0, 0.5};
    vec4_t grey      = {0.25, 0.25, 0.25, 0.5};

    float y          = hud_cgazY.value;
    float alpha      = hud_cgazAlpha.value;
    float speed;
    float a;
    float velangle;
    float minangle;
    float optangle;
    float slowangle;
    float keysangle;

    if ( !hud_cgaz.integer || cgs.gametype == GT_ADVENTURE )
        return;

    green[3] = alpha;
    darkgreen[3] = alpha;
    yellow[3] = alpha;
    grey[3] = alpha;

    cmdnum = trap_GetCurrentCmdNumber();
    trap_GetUserCmd(cmdnum, &cmd);

    speed = sqrt(ps->velocity[0]*ps->velocity[0] + ps->velocity[1]*ps->velocity[1]);

    // red accel bars
	if ( hud_cgaz.integer & 1 ) {
		static int lastCommandTime;
		static float lastWidth, lastSpeed;
		float width;

		if ( ps->commandTime <= lastCommandTime ) {
			width = lastWidth;
		} else {
			width = (speed - lastSpeed) / (ps->commandTime - lastCommandTime) * 256.0f;

			if ( ps->groundEntityNum != ENTITYNUM_NONE ) {
				width *= 0.1f;
			}

			lastCommandTime = ps->commandTime;
			lastWidth = width;
			lastSpeed = speed;
		}

		if ( width > 0 ) {
			CG_FillRect( 320.0f + 24.0f, y - 16.0f, width, 16.0f, colorRed );
		} else if (width < 0 ) {
			CG_FillRect( 320.0f - 24.0f, y - 16.0f, width, 16.0f, colorRed );
		}

		CG_FillRect( 320.0f + 24.0f + 81.9199981689f, y - 12.0f, 1.0f, 8.0f, colorWhite );
		CG_FillRect( 320.0f - 24.0f - 81.9199981689f, y - 12.0f, -1.0f, 8.0f, colorWhite );
	}

    if ( ps->groundEntityNum == ENTITYNUM_NONE || cmd.upmove > 0 ) {
        a = ps->speed * .008;
    } else {
        speed -= speed * pm_friction * .008;
        a = ps->speed * pm_accelerate * .008;
    }

   	velangle = RAD2DEG(atan2(ps->velocity[1], ps->velocity[0]));

    if(speed <= ps->speed - a)
		return; // can get full accel from forward only

    optangle = RAD2DEG(acos((ps->speed - a) / speed));

    if(speed <= ps->speed)
		minangle = 0.0f;
    else
		minangle = RAD2DEG(acos(ps->speed / speed));

    slowangle = RAD2DEG(acos(a / speed));

    keysangle = 0;
    if (ps->movementDir == 1 )
        keysangle = 45;
    else if (ps->movementDir == 7 )
        keysangle = -45;
    else if (ps->movementDir == 2 )
        keysangle = 90;
    else if (ps->movementDir == 6 )
        keysangle = -90;
    else if (ps->movementDir == 3 )
        keysangle = 135;
    else if (ps->movementDir == 5 )
        keysangle = -135;
    else if (ps->movementDir == 4 )
        keysangle = 180;

    CG_FillAngleYaw(velangle + minangle - keysangle, velangle + optangle - keysangle,   cg.refdefViewAngles[YAW], y, 16, green);
    CG_FillAngleYaw(velangle + optangle - keysangle, velangle + slowangle - keysangle,  cg.refdefViewAngles[YAW], y, 16, darkgreen);
    CG_FillAngleYaw(velangle + slowangle - keysangle, velangle + 90 - keysangle,        cg.refdefViewAngles[YAW], y, 16, yellow);
    CG_FillAngleYaw(velangle + minangle - keysangle, velangle - minangle - keysangle,   cg.refdefViewAngles[YAW], y, 16, grey);
    CG_FillAngleYaw(velangle - minangle - keysangle, velangle - optangle - keysangle,   cg.refdefViewAngles[YAW], y, 16, green);
    CG_FillAngleYaw(velangle - optangle - keysangle, velangle - slowangle - keysangle,  cg.refdefViewAngles[YAW], y, 16, darkgreen);
    CG_FillAngleYaw(velangle - slowangle - keysangle, velangle - 90 - keysangle,        cg.refdefViewAngles[YAW], y, 16, yellow);
}


#define MAX_SNAP_ZONES 32

typedef struct {
	float start;	// starting angle of snap zone
	float end;		// ending angle
	float gain;		// how much speed will be gained
	float displayGain; // normalized for drawing the bar
} snapzone_t;

static snapzone_t snapZones[MAX_SNAP_ZONES];
static int numSnapZones;

static int QDECL SortFloats( const void *a, const void *b ) {
	if ( *(float *)a == *(float *)b )
		return 0;
	else
		return *(float *)a > *(float *)b ? 1 : -1;
}

static void CG_CalcSnapZones( float speed ) {
	float snapAngles[MAX_SNAP_ZONES];
	float step;
	float mingain = 100000, maxgain = 0;
	int i;

	speed /= 125;
	numSnapZones = 0;

	snapAngles[numSnapZones++] = 0;
	snapAngles[numSnapZones++] = DEG2RAD(90);

	// 0.5, 1.5, 2.5, etc...
	// the threshold for component rounding will be the snapzone boundaries
	for( step = floor(speed+0.5)-0.5; step > 0 && numSnapZones < MAX_SNAP_ZONES - 2; step-- ) {
		snapAngles[numSnapZones++] = acos( step/speed );

		// http://mathforum.org/library/drmath/view/65237.html
		// snapAngles[numSnapZones++] = asin( step/speed );
		// is there a better way?
		snapAngles[numSnapZones++] = atan2( step/speed, sqrt(1-(step/speed)*(step/speed)) );
	}
	qsort( snapAngles, numSnapZones, sizeof(snapAngles[0]), SortFloats );

	// numSnapZones was the number of angles
	// now make it the number of intervals
	numSnapZones--;

	for ( i = 0; i < numSnapZones; i++ ) {
		float angle, x, y, gain;

		// find speed gain after snapping for this zone
		// any angle in the interval will do
		angle = (snapAngles[i] + snapAngles[i+1]) / 2;

		// simulate trap_SnapVector
		x = floor( speed*cos( angle ) + 0.5 );
		y = floor( speed*sin( angle ) + 0.5 );

		gain = sqrt( x*x + y*y );
		if ( gain > maxgain ) {
			maxgain = gain;
		} else if ( gain < mingain ) {
			mingain = gain;
		}

		snapZones[i].start = RAD2DEG(snapAngles[i]);
		snapZones[i].end = RAD2DEG(snapAngles[i+1]);
		snapZones[i].gain = gain;
	}

	for ( i = 0; i < numSnapZones; i++ ) {
		snapZones[i].displayGain = (snapZones[i].gain - mingain) / (maxgain - mingain);

		// shrink the range so a bar is always shown if speed can be gained
		if ( mingain > 0 ) {
			snapZones[i].displayGain = snapZones[i].displayGain * 0.8 + 0.2;
		}
	}
}

static void CG_DrawSnap( void ) {
	playerState_t *ps = &cg.predictedPlayerState;
	static int speed;
	float angle;
	int i;
	vec4_t color;

	if ( !hud_snap.integer || cgs.gametype == GT_ADVENTURE )
		return;

	if ( speed != ps->speed  ) {
		speed = ps->speed;
		CG_CalcSnapZones( speed );
	}

	if ( pm_aircontrol > 0) {
		angle = 45;
	} else {
		qbool forward = (ps->stats[STAT_DFX_FLAG] & DFXF_KEYFORWARD) || (ps->stats[STAT_DFX_FLAG] & DFXF_KEYBACK);
		qbool right = (ps->stats[STAT_DFX_FLAG] & DFXF_KEYRIGHT) || (ps->stats[STAT_DFX_FLAG] & DFXF_KEYLEFT);

		if ( right != forward )
			angle = 0;
		 else
			angle = 45;
	}

	Q_sscanf( hud_snapRGBA.string, "%f %f %f %f", &color[0], &color[1], &color[2], &color[3] );

	for( i = 0; i < numSnapZones; i++ ) {
		float height = snapZones[i].displayGain * 16;
        float y = hud_snapY.value + 16;

		CG_FillAngleYaw(snapZones[i].start, snapZones[i].end, cg.refdefViewAngles[YAW] + angle, y + 16 - height, height, color);
		CG_FillAngleYaw(snapZones[i].start, snapZones[i].end, cg.refdefViewAngles[YAW] + angle + 90, y + 16 - height, height, color);
		CG_FillAngleYaw(snapZones[i].start, snapZones[i].end, cg.refdefViewAngles[YAW] + angle + 180, y + 16 - height, height, color);
		CG_FillAngleYaw(snapZones[i].start, snapZones[i].end, cg.refdefViewAngles[YAW] + angle + 270, y + 16 - height, height, color);
	}
}

/*
=================
CG_DrawChs_Keypress
=================
*/
static void CG_DrawChs_Keypress( void )
{
    playerState_t *ps = &cg.predictedPlayerState;

    int col;


    if(!chs_keypress.integer || cgs.gametype == GT_ADVENTURE)
        return;

    col = chs_fontColor.integer;

    if (col < 0)
        col = 0;

    if (col > 9)
        col = 9;

    trap_R_SetColor( g_color_table[col] );

    if(ps->stats[STAT_DFX_FLAG] & DFXF_KEYFORWARD)
        CG_DrawChar(316, 228, 8, 8, 0x87);
    else if(ps->stats[STAT_DFX_FLAG] & DFXF_KEYBACK)
        CG_DrawChar(316, 244, 8, 8, 0x86);

    if(ps->stats[STAT_DFX_FLAG] & DFXF_KEYRIGHT)
        CG_DrawChar(324, 236, 8, 8, 0x8d);
    else if(ps->stats[STAT_DFX_FLAG] & DFXF_KEYLEFT)
        CG_DrawChar(308, 236, 8, 8, 0x88);

    if(ps->stats[STAT_DFX_FLAG] & DFXF_KEYJUMP)
        CG_DrawChar(316, 220, 8, 8, 'J');
    else if(ps->stats[STAT_DFX_FLAG] & DFXF_KEYCROUCH)
        CG_DrawChar(316, 252, 8, 8, 'C');

}


/*
=================
CG_DrawChs_Angles
=================
*/
static void CG_DrawChs_Angles (void) {

    playerState_t *ps;
    const char *s;
    int x, y, col, col2, fs;

    if (!chs_angles.integer || cgs.gametype == GT_ADVENTURE)
        return;

    x = chs_anglesX.integer;
    y = chs_anglesY.integer;
    col = chs_fontColor.integer;
    col2 = chs_fontHighlightColor.integer;
    fs = chs_fontSize.integer;

    if (fs < 4)
        fs = 4;

    if (fs > 16)
        fs = 16;

    if ( x > cgs.glconfig.vidWidth )
        x = cgs.glconfig.vidWidth;

    if ( y > cgs.glconfig.vidHeight )
        y = cgs.glconfig.vidHeight;

    ps = &cg.predictedPlayerState;

    if ( chs_angles.integer>=2 && ((ps->viewangles[PITCH] >=72.2 && ps->viewangles[PITCH] < 72.6) ||  (ps->viewangles[PITCH] >=73.6 && ps->viewangles[PITCH] < 74.0)) )
        col = col2;

    if (col < 0)
        col = 0;

    if (col > 9)
        col = 9;

    if (chs_angles.integer == 1) {
        s = va( "%.1f", ps->viewangles[YAW] );
    } else if (chs_angles.integer == 2 ) {
        s = va( "%.1f", ps->viewangles[PITCH] );
    } else if (chs_angles.integer >= 3 ) {
        s = va( "%.1f %.1f", ps->viewangles[YAW], ps->viewangles[PITCH] );
    }

    CG_DrawString( x, y, s, g_color_table[col], fs, fs, 0, DS_CENTER | DS_SHADOW | DS_PROPORTIONAL );
}

/*
=================
CG_DrawChs_Ping
=================
*/
static void CG_DrawChs_Ping (void)
{
    int x, y, col, fs;

    if (!chs_ping.integer || cgs.localServer )
        return;

    x = chs_pingX.integer;
    y = chs_pingY.integer;
    col = chs_fontColor.integer;
    fs = chs_fontSize.integer;

    if (fs < 4)
        fs = 4;

    if (fs > 16)
        fs = 16;

    if ( x > cgs.glconfig.vidWidth )
        x = cgs.glconfig.vidWidth;

    if ( y > cgs.glconfig.vidHeight )
        y = cgs.glconfig.vidHeight;

    if (col < 0)
        col = 0;

    if (col > 9)
        col = 9;


    CG_DrawString( x+1, y, va( "%ims", cg.meanPing ), g_color_table[col], fs, fs, 0, DS_SHADOW | DS_RIGHT | DS_PROPORTIONAL );
}

/*
=================
CG_DrawChs_Speed
=================
*/
static void CG_DrawChs_Speed (void )
{
	const char *s;

    int x, y, col, fs;

    if (!chs_speed.integer || cgs.gametype == GT_ADVENTURE)
        return;

    x = chs_speedX.integer;
    y = chs_speedY.integer;
    col = chs_fontColor.integer;
    fs = chs_fontSize.integer;

    if (fs < 4)
        fs = 4;

    if (fs > 16)
        fs = 16;

    if ( x > cgs.glconfig.vidWidth )
        x = cgs.glconfig.vidWidth;

    if ( y > cgs.glconfig.vidHeight )
        y = cgs.glconfig.vidHeight;

    if (col < 0)
        col = 0;

    if (col > 9)
        col = 9;

	s = va( "%1.0f", cg.xyspeed );

	CG_DrawString( x, y, s, g_color_table[col], fs, fs, 0, DS_SHADOW | DS_CENTER | DS_PROPORTIONAL );
}

#ifdef DEVBUILD

#define	PSOFS(x) (size_t)&((playerState_t*)0)->x
#define	PSF(x) #x,PSOFS(x)

struct {
	char *name;
	int offset;
	enum{PS_INT, PS_VEC} type;
	int size;
} *field, psFields[] = {
	{PSF(commandTime), PS_INT, 1},
	{PSF(pm_type), PS_INT, 1},
	{PSF(bobCycle), PS_INT, 1},
	{PSF(pm_flags), PS_INT, 1},
	{PSF(pm_time), PS_INT, 1},
	{PSF(origin), PS_VEC, 3},
	{PSF(velocity), PS_VEC, 3},
	{PSF(weaponTime), PS_INT, 1},
	{PSF(gravity), PS_INT, 1},
	{PSF(speed), PS_INT, 1},
	{PSF(delta_angles), PS_INT, 3},
	{PSF(groundEntityNum), PS_INT, 1},
	{PSF(legsTimer), PS_INT, 1},
	{PSF(legsAnim), PS_INT, 1},
	{PSF(torsoTimer), PS_INT, 1},
	{PSF(torsoAnim), PS_INT, 1},
	{PSF(movementDir), PS_INT, 1},
	{PSF(grapplePoint), PS_VEC, 3},
	{PSF(eFlags), PS_INT, 1},
	{PSF(eventSequence), PS_INT, 1},
	{PSF(events), PS_INT, MAX_PS_EVENTS},
	{PSF(eventParms), PS_INT, MAX_PS_EVENTS},
	{PSF(externalEvent), PS_INT, 1},
	{PSF(externalEventParm), PS_INT, 1},
	{PSF(externalEventTime), PS_INT, 1},
	{PSF(clientNum), PS_INT, 1},
	{PSF(weapon), PS_INT, 1},
	{PSF(weaponstate), PS_INT, 1},
	{PSF(viewangles), PS_VEC, 3},
	{PSF(viewheight), PS_INT, 1},
	{PSF(damageEvent), PS_INT, 1},
	{PSF(damageYaw), PS_INT, 1},
	{PSF(damagePitch), PS_INT, 1},
	{PSF(damageCount), PS_INT, 1},
	//{PSF(stats), PS_INT, MAX_STATS},
	//{PSF(persistant), PS_INT, MAX_PERSISTANT}, // doing stats and persistant separately
	{PSF(powerups), PS_INT, MAX_POWERUPS},
	{PSF(ammo), PS_INT, MAX_WEAPONS},
	{PSF(generic1), PS_INT, 1},
	{PSF(loopSound), PS_INT, 1},
	{PSF(jumppad_ent), PS_INT, 1}
};

char *pm_type_names[] = {
	"PM_NORMAL", "PM_NOCLIP", "PM_SPECTATOR", "PM_DEAD",
	"PM_FREEZE", "PM_INTERMISSION", "PM_SPINTERMISSION"
};

char *pm_flags_names[] = {
	"PMF_DUCKED", "PMF_JUMP_HELD", "?", "PMF_BACKWARDS_JUMP",
	"PMF_BACKWARDS_RUN", "PMF_TIME_LAND", "PMF_TIME_KNOCKBACK", "?",
	"PMF_TIME_WATERJUMP", "PMF_RESPAWNED", "PMF_USE_ITEM_HELD", "PMF_GRAPPLE_PULL",
	"PMF_FOLLOW", "PMF_SCOREBOARD", "PMF_INVULEXPAND", "PMF_DUCK_HELD"
};

char *eFlags_names[] = {
	"EF_DEAD", "EF_TICKING", "EF_TELEPORT_BIT", "EF_AWARD_EXCELLENT",
	"EF_PLAYER_EVENT", "EF_BOUNCE", "EF_BOUNCE_HALF", "EF_AWARD_GAUNTLET",
	"EF_NODRAW", "EF_FIRING", "EF_KAMIKAZE", "EF_MOVER_STOP",
	"EF_AWARD_CAP", "EF_TALK", "EF_CONNECTION", "EF_VOTED",
	"EF_AWARD_IMPRESSIVE", "EF_AWARD_DEFEND", "EF_AWARD_ASSIST",
	"EF_AWARD_DENIED", "EF_TEAMVOTED"
};

char *events_names[] = {
    "EV_NONE",

	"EV_FOOTSTEP",
	"EV_FOOTSTEP_METAL",
	"EV_FOOTSTEP_FLESH",
	"EV_FOOTSTEP_STONE",
	"EV_FOOTSTEP_GRASS",
	"EV_FOOTSTEP_TALLGRASS",
	"EV_FOOTSTEP_WOOD",
	"EV_FOOTSTEP_SAND",
	"EV_FOOTSTEP_ENERGY",
	"EV_FOOTSTEP_LAVA",
    "EV_FOOTSTEP_ICE",

	"EV_FOOTSPLASH",
	"EV_FOOTWADE",
	"EV_SWIM",

	"EV_STEP_4",
	"EV_STEP_8",
	"EV_STEP_12",
	"EV_STEP_16",

	"EV_FALL_SHORT",
	"EV_FALL_MEDIUM",
	"EV_FALL_FAR",

	"EV_JUMP_PAD",		        // boing sound at origin", jump sound on player

	"EV_JUMP",
	"EV_WATER_TOUCH",	        // foot touches
	"EV_WATER_LEAVE",	        // foot leaves
	"EV_WATER_UNDER",	        // head touches
	"EV_WATER_CLEAR",	        // head leaves

	"EV_ITEM_PICKUP",			// normal item pickups are predictable
	"EV_GLOBAL_ITEM_PICKUP",	// powerup / team sounds are broadcast to everyone

	"EV_NOAMMO",
	"EV_CHANGE_WEAPON",
	"EV_FIRE_WEAPON",

	"EV_USE_ITEM0",
	"EV_USE_ITEM1",
	"EV_USE_ITEM2",
	"EV_USE_ITEM3",
	"EV_USE_ITEM4",
	"EV_USE_ITEM5",
	"EV_USE_ITEM6",
	"EV_USE_ITEM7",
	"EV_USE_ITEM8",
	"EV_USE_ITEM9",
	"EV_USE_ITEM10",
	"EV_USE_ITEM11",
	"EV_USE_ITEM12",
	"EV_USE_ITEM13",
	"EV_USE_ITEM14",
	"EV_USE_ITEM15",

	"EV_ITEM_RESPAWN",
	"EV_ITEM_POP",
	"EV_PLAYER_TELEPORT_IN",
	"EV_PLAYER_TELEPORT_OUT",

	"EV_GRENADE_BOUNCE",		// eventParm will be the soundindex

	"EV_GENERAL_SOUND",
	"EV_GLOBAL_SOUND",		    // no attenuation
	"EV_GLOBAL_TEAM_SOUND",

	"EV_BULLET_HIT_FLESH",
	"EV_BULLET_HIT_WALL",

	"EV_MISSILE_HIT",
	"EV_MISSILE_MISS",
	"EV_MISSILE_MISS_METAL",
	"EV_RAILTRAIL",
	"EV_SHOTGUN",
	"EV_BULLET",				// otherEntity is the shooter

	"EV_PAIN",
	"EV_DEATH1",
	"EV_DEATH2",
	"EV_DEATH3",
	"EV_OBITUARY",

	"EV_POWERUP_QUAD",
	"EV_POWERUP_BATTLESUIT",
	"EV_POWERUP_REGEN",

	"EV_GIB_PLAYER",			// gib a previously living player

	"EV_DEBUG_LINE",
	"EV_STOPLOOPINGSOUND",

	"EV_TAUNT",
	"EV_TAUNT_YES",
	"EV_TAUNT_NO",
	"EV_TAUNT_FOLLOWME",
	"EV_TAUNT_GETFLAG",
	"EV_TAUNT_GUARDBASE",
	"EV_TAUNT_PATROL",

	"EV_BREAKABLE",
    "EV_EARTHQUAKE",
    "EV_EXPLOSION",
    "EV_PARTICLES_GRAVITY",
	"EV_PARTICLES_LINEAR",
    "EV_PARTICLES_LINEAR_UP",
	"EV_PARTICLES_LINEAR_DOWN",

    "EV_LANDSOUND",
	"EV_LANDSOUND_METAL",
	"EV_LANDSOUND_FLESH",
	"EV_LANDSOUND_STONE",
	"EV_LANDSOUND_GRASS",
	"EV_LANDSOUND_TALLGRASS",
	"EV_LANDSOUND_WOOD",
	"EV_LANDSOUND_SAND",
	"EV_LANDSOUND_LAVA",
	"EV_LANDSOUND_ICE"
};

char *weaponstate_names[] = {
	"WEAPON_READY",  "WEAPON_RAISING", "WEAPON_DROPPING", "WEAPON_FIRING"
};

char *stats_names[] = {
	"STAT_HEALTH", "STAT_HOLDABLE_ITEM",
	"STAT_WEAPONS", "STAT_ARMOR", "STAT_DEAD_YAW", "STAT_CLIENTS_READY",
	"STAT_MAX_HEALTH", "STAT_JUMPTIME", "STAT_DFX_FLAG", "STAT_DFTIMERHI",
	"STAT_DFTIMERLO", "STAT_CROUCHTIME", "STAT_WJTIME", "STAT_WJCOUNT"
};

char *persistant_names[] = {
	"PERS_SCORE", "PERS_HITS", "PERS_RANK", "PERS_TEAM",
	"PERS_SPAWN_COUNT", "PERS_PLAYEREVENTS", "PERS_ATTACKER", "PERS_ATTACKEE_ARMOR",
	"PERS_KILLED", "PERS_IMPRESSIVE_COUNT", "PERS_EXCELLENT_COUNT", "PERS_DEFEND_COUNT",
	"PERS_ASSIST_COUNT", "PERS_GAUNTLET_FRAG_COUNT", /*"PERS_CAPTURES",*/ "PERS_SECRETS", "PERS_KEYS"
};

static void CG_DrawPS( void ) {
	playerState_t *ps = &cg.predictedPlayerState;
	static char buffer[MAX_STRING_CHARS];
	vec4_t color = {1, 1, 1, 0.75};
	float x = 0;
	float y = 0;
	int w = 5;
	int h = 6;
	int i, j;

	if ( !cg_psf.integer ) {
		return;
	}

	for ( field = psFields, i = 0; i < ARRAY_LEN(psFields); field++, i++ ) {
		vec_t *field_vec = (vec_t *)((byte *)ps + field->offset);
		int *field_int = (int *)((byte *)ps + field->offset);

		Q_strncpyz( buffer, va("%-17s:", field->name), sizeof(buffer) );

		switch( field->offset ) {
			case PSOFS(pm_type):
				if ( *field_int >= 0 && *field_int < ARRAY_LEN(pm_type_names) ) {
					Q_strcat( buffer, sizeof(buffer), pm_type_names[*field_int] );
				}
				break;

			case PSOFS(pm_flags):
				for ( j = 0; j < ARRAY_LEN(pm_flags_names); j++ ) {
					if ( *field_int & (1 << j) ) {
						Q_strcat( buffer, sizeof(buffer), va("%s ", pm_flags_names[j]) );
					}
				}
				break;

			case PSOFS(eFlags):
				for ( j = 0; j < ARRAY_LEN(eFlags_names); j++ ) {
					if ( *field_int & (1 << j) ) {
						Q_strcat( buffer, sizeof(buffer), va("%s ", eFlags_names[j]) );
					}
				}
				break;

			case PSOFS(events):
			case PSOFS(externalEvent):
				for ( j = 0; j < field->size; j++ ) {
					int event = field_int[j] & ~EV_EVENT_BITS;

					if ( event >= 0 && event < ARRAY_LEN(events_names) ) {
						Q_strcat( buffer, sizeof(buffer), va("%s ", events_names[event]) );
					}
				}
				break;

			case PSOFS(weaponstate):
				if ( *field_int >= 0 && *field_int < ARRAY_LEN(weaponstate_names) ) {
					Q_strcat( buffer, sizeof(buffer), weaponstate_names[*field_int] );
				}
				break;

			// just print as int(s)/float(s)
			default:
				switch( field->type ) {
					case PS_INT:
						for ( j = 0; j < field->size; j++ ) {
							Q_strcat( buffer, sizeof(buffer), va("%d ", field_int[j]) );
						}
						break;
					case PS_VEC:
						for ( j = 0; j < field->size; j++ ) {
							Q_strcat( buffer, sizeof(buffer), va("%.3f ", field_vec[j]) );
						}
						break;
				}
				break;
		}

		CG_DrawStringExt( x, y, buffer, color, qfalse, qfalse, w, h, 0 );
		y += h;
	}

	// now draw stats and persistant
	x += 320;
	y = 0;

	CG_DrawStringExt( x, y, "stats:", color, qfalse, qfalse, w, h, 0 );
	y += h;

	for ( i = 0; i < ARRAY_LEN(stats_names); i ++ ) {
		switch( i ) {
			case STAT_WEAPONS:
			case STAT_CLIENTS_READY:
				for ( j = 0; j < 16; j++ ) {
					buffer[j] = (ps->stats[i] & (1 << j )) ? '1' : '0';
				}
				buffer[j] = '\0';
				CG_DrawStringExt( x + 32, y, va("%-18s:%s", stats_names[i], buffer), color, qfalse, qfalse, w, h, 0 );
				break;
			default:
				CG_DrawStringExt( x + 32, y, va("%-18s:%d", stats_names[i], ps->stats[i]), color, qfalse, qfalse, w, h, 0 );
				break;
		}
		y += h;
	}

	y += h;;

	CG_DrawStringExt( x, y, "persistant:", color, qfalse, qfalse, w, h, 0 );
	y += h;

	for ( i = 0; i < ARRAY_LEN(persistant_names); i ++ ) {
		CG_DrawStringExt( x + 32, y, va("%-25s:%d", persistant_names[i], ps->persistant[i]), color, qfalse, qfalse, w, h, 0 );
		y += h;
	}
}

#endif // DEVBUILD


/************************
 * CG_GameStartEffects
/*************************/

static void CG_GameStartEffects ( void ) {
	vec4_t	color;

	 //make screen fade in from black at start of game
	if (cgs.gametype == GT_ADVENTURE && cg.time < (cg.fadeTime + BLACKOUT_TIME + FADEIN_TIME)) {
		color[0] = 0;
		color[1] = 0;
		color[2] = 0;
		if ( cg.time < cg.fadeTime + BLACKOUT_TIME )	//screen remains black for some time, fades in after that
			color[3] = 1;
		else
			color[3] = (FADEIN_TIME - ((cg.time - cg.fadeTime) - BLACKOUT_TIME)) / FADEIN_TIME;

		CG_FillRect(cgs.screenXmin, cgs.screenYmin, cgs.glconfig.vidWidth, cgs.glconfig.vidHeight, color);
	}

	if ( cg.scoreBoardShowing )
		return;

	//draw map message
	if ( cg.time > cg.fadeTime && cg.time < cg.fadeTime + TITLE_TIME + TITLE_FADEIN_TIME + TITLE_FADEOUT_TIME) {
		const char *s;

		s = CG_ConfigString( CS_MESSAGE );
		color[0] = 1;
		color[1] = 1;
		color[2] = 1;
		if ( cg.time < cg.fadeTime + TITLE_FADEIN_TIME ){
			color[3] = (cg.time - cg.fadeTime) / TITLE_FADEIN_TIME;
		}
		else if ( cg.time < cg.fadeTime + TITLE_TIME + TITLE_FADEIN_TIME )
			color[3] = 1;
		else
			color[3] = (TITLE_FADEOUT_TIME - ((cg.time - cg.fadeTime) - (TITLE_FADEIN_TIME + TITLE_TIME))) / TITLE_FADEOUT_TIME;

		CG_DrawString( 320, 70, s, color, BIGCHAR_WIDTH, BIGCHAR_HEIGHT, 0, DS_CENTER | DS_SHADOW | DS_PROPORTIONAL );
	}
}



/*===============================================================================================================================


===============================================================================================================================
*/

/*
=================
CG_Draw2D
=================
*/
static void CG_Draw2D( stereoFrame_t stereoFrame )
{
	if ( !cg_draw2D.integer )
		return;

	if ( cg.snap->ps.pm_type == PM_INTERMISSION ) {
		CG_DrawIntermission();
		return;
	}

	if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR )
    {
		CG_DrawCrosshair();
		CG_DrawCrosshairNames();
	}
	else
    {
		// don't draw any status if dead or the scoreboard is being explicitly shown
		if ( (!cg.showScores && cg.snap->ps.stats[STAT_HEALTH] > 0) && !cg.warmup && !cg_paused.integer )
		{
            CG_DrawStatusBar();
            CG_DrawCgaz();
			CG_DrawSnap();

			CG_DrawCrosshair();
			CG_DrawCrosshairNames();
			CG_DrawWeaponSelect();

			CG_DrawDFTimer();
			CG_DrawDFCheckpoints();
			CG_DrawChs_Keypress();
			CG_DrawChs_Angles();
			CG_DrawChs_Speed();
			CG_DrawChs_OBs();
			CG_DrawChs_Ping();
		}
	}

	CG_DrawVote();
	CG_DrawLagometer();

	if (!cg_paused.integer)
        CG_DrawUpperRight(stereoFrame);

	CG_DrawLowerRight();
	CG_DrawLowerLeft();


	if ( !CG_DrawFollow() )
		CG_DrawWarmup();

	// don't draw center string if scoreboard is up
	cg.scoreBoardShowing = CG_DrawScoreboard();
	if ( !cg.scoreBoardShowing )
		CG_DrawCenterString();

    if ( !cg.fadeTime )
		cg.fadeTime = cg.time;

    CG_GameStartEffects();

	// draw cursor when scoreboard is active
	if ( cgs.score_catched )
    {
		float x, y, w, h;
		trap_R_SetColor( NULL );
		x = cgs.cursorX - 12;
		y = cgs.cursorY - 12;
		w = 24;
		h = 24;
		CG_AdjustFrom640( &x, &y, &w, &h );
		trap_R_DrawStretchPic( x, y, w, h, 0, 0, 1, 1, cgs.media.cursor );
	}

#ifdef DEVBUILD
	CG_DrawPS();
#endif // DEVBUILD
}


static void CG_CalculatePing( void )
{
	int count, i, v;

	cg.meanPing = 0;

	for ( i = 0, count = 0; i < LAG_SAMPLES; i++ )
    {

		v = lagometer.snapshotSamples[i];
		if ( v >= 0 )
        {
			cg.meanPing += v;
			count++;
		}
	}

	if ( count )
    {
		cg.meanPing /= count;
	}
}


static void CG_WarmupEvents( void )
{
	int	count;

	if ( !cg.warmup )
		return;

	if ( cg.warmup < 0 )
    {
		cg.warmupCount = -1;
		return;
	}

	if ( cg.warmup < cg.time )
    {
		cg.warmup = 0;
		count = 0;
	}
	else
    {
		count = ( cg.warmup - cg.time + 999 ) / 1000;
	}

	if ( cg.warmupCount == -2 && cg.demoPlayback )
    {
		cg.warmupCount = 0;
	}

	if ( cg.warmupCount == count )
    {
		return;
	}

	cg.warmupCount = count;

	switch ( count ) {
		case 1:
		case 2:
		case 3:
			trap_S_StartLocalSound( cgs.media.countSound, CHAN_ANNOUNCER );
			break;

		default:
			break;

	}
}


// will be called on warmup end and when client changed
void CG_WarmupEvent( void )
{

	cg.attackerTime = 0;
	cg.attackerName[0] = '\0';

	cg.itemPickupTime = 0;
	cg.itemPickupBlendTime = 0;
	cg.itemPickupCount = 0;

	cg.killerTime = 0;
	cg.killerName[0] = '\0';

	cg.damageTime = 0;

	cg.rewardStack = 0;
	cg.rewardTime = 0;

	cg.weaponSelectTime = cg.time;

	cg.lowAmmoWarning = 0;

	cg.followTime = 0;
}


// called each time client team changed
static void CG_ApplyClientChange( void )
{
	CG_WarmupEvent();
	CG_ForceModelChange();
}


/*
=====================
CG_TrackClientTeamChange
=====================
*/
void CG_TrackClientTeamChange( void )
{
	static int spec_client = -1;
	static int spec_team = -1;
	static int curr_team = -1;

	int		ti; // team from clientinfo
	int		tp; // persistant team from snapshot

	if ( !cg.snap )
		return;

	tp = cg.snap->ps.persistant[ PERS_TEAM ];
	ti = cgs.clientinfo[ cg.snap->ps.clientNum ].team;

	if ( !(cg.snap->ps.pm_flags & PMF_FOLLOW) && tp != TEAM_SPECTATOR ) {
		ti = tp; // use team from persistant info
	}

	// team changed
	if ( curr_team != ti )
	{
		curr_team = ti;
		spec_client = cg.snap->ps.clientNum;
		spec_team = tp;

		if ( spec_team == TEAM_SPECTATOR )
			spec_team = curr_team;

		CG_ApplyClientChange();
		CG_ResetPlayerEntity( &cg.predictedPlayerEntity );
		return;
	}

	if ( curr_team == TEAM_SPECTATOR )
	{
		if ( spec_team != tp )
		{
			spec_team  = tp;
			spec_client = cg.snap->ps.clientNum;

			CG_ApplyClientChange();
			CG_ResetPlayerEntity( &cg.predictedPlayerEntity );
			return;
		}

		if ( cgs.gametype == GT_TEAMRUN )
		{
			spec_client = cg.snap->ps.clientNum;
			return;
		}
		// pass through to spec client checks
	}

	if ( spec_client != cg.snap->ps.clientNum )
	{
		spec_client = cg.snap->ps.clientNum;
		spec_team = tp;

		if ( spec_team == TEAM_SPECTATOR )
			spec_team = cgs.clientinfo[ cg.snap->ps.clientNum ].team;

		CG_ApplyClientChange();
		CG_ResetPlayerEntity( &cg.predictedPlayerEntity );
	}
}


/*
=====================
CG_DrawActive

Perform all drawing needed to completely fill the screen
=====================
*/
void CG_DrawActive( stereoFrame_t stereoView )
{
	// optionally draw the info screen instead
	if ( !cg.snap )
    {
		CG_DrawInformation();
		return;
	}

	if ( !cg.demoPlayback )
		CG_CalculatePing();

    // apply earthquake effect
	CG_Earthquake();

	// draw 3D view
	trap_R_RenderScene( &cg.refdef );

	// play warmup sounds and display text
	CG_WarmupEvents();

	// draw status bar and other floating elements
 	CG_Draw2D( stereoView );
}
