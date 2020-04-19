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
// cl_scrn.c -- master for refresh, status bar, console, chat, notify, etc

#include "client.h"

qboolean	scr_initialized;		// ready to draw

cvar_t		*cl_timegraph;
cvar_t		*cl_debuggraph;
cvar_t		*cl_graphheight;
cvar_t		*cl_graphscale;
cvar_t		*cl_graphshift;

void SCR_DrawNamedPic( float x, float y, float width, float height, const char *picname ) {}

/*
================
SCR_AdjustFrom640

Adjusted for resolution and screen aspect ratio
================
*/
void SCR_AdjustFrom640( float *x, float *y, float *w, float *h ) {
	float	xscale;
	float	yscale;

#if 0
		// adjust for wide screens
		if ( cls.glconfig.vidWidth * 480 > cls.glconfig.vidHeight * 640 ) {
			*x += 0.5 * ( cls.glconfig.vidWidth - ( cls.glconfig.vidHeight * 640 / 480 ) );
		}
#endif

	// scale for screen sizes
	xscale = cls.glconfig.vidWidth / 640.0;
	yscale = cls.glconfig.vidHeight / 480.0;
	if ( x ) {
		*x *= xscale;
	}
	if ( y ) {
		*y *= yscale;
	}
	if ( w ) {
		*w *= xscale;
	}
	if ( h ) {
		*h *= yscale;
	}
}

void SCR_FillRect( float x, float y, float width, float height, const float *color ) {}
void SCR_DrawPic( float x, float y, float width, float height, qhandle_t hShader ) {}
void SCR_DrawSmallChar( int x, int y, int ch ) {}
void SCR_DrawStringExt( int x, int y, float size, const char *string, float *setColor, qboolean forceColor, qboolean noColorEscape ) {}
void SCR_DrawBigString( int x, int y, const char *s, float alpha, qboolean noColorEscape ) {}
void SCR_DrawBigStringColor( int x, int y, const char *s, vec4_t color, qboolean noColorEscape ) {}
void SCR_DrawSmallStringExt( int x, int y, const char *string, float *setColor, qboolean forceColor, qboolean noColorEscape ) {}
int	SCR_GetBigStringWidth( const char *str ) { return 0; }

/*
===============================================================================

DEBUG GRAPH

===============================================================================
*/

static	int			current;
static	float		values[1024];

void SCR_DebugGraph (float value)
{
	values[current] = value;
	current = (current + 1) % ARRAY_LEN(values);
}

/*
==============
SCR_DrawDebugGraph
==============
*/
void SCR_DrawDebugGraph (void)
{
	//int		a, x, y, w, i, h;
	//float	v;
	//
	////
	//// draw the graph
	////
	//w = cls.glconfig.vidWidth;
	//x = 0;
	//y = cls.glconfig.vidHeight;
	//re.SetColor( g_color_table[0] );
	//re.DrawStretchPic(x, y - cl_graphheight->integer, 
	//	w, cl_graphheight->integer, 0, 0, 0, 0, cls.whiteShader );
	//re.SetColor( NULL );
	//
	//for (a=0 ; a<w ; a++)
	//{
	//	i = (ARRAY_LEN(values)+current-1-(a % ARRAY_LEN(values))) % ARRAY_LEN(values);
	//	v = values[i];
	//	v = v * cl_graphscale->integer + cl_graphshift->integer;
	//	
	//	if (v < 0)
	//		v += cl_graphheight->integer * (1+(int)(-v / cl_graphheight->integer));
	//	h = (int)v % cl_graphheight->integer;
	//	re.DrawStretchPic( x+w-1-a, y - h, 1, h, 0, 0, 0, 0, cls.whiteShader );
	//}
}

void SCR_Init() {
	cl_timegraph = Cvar_Get ("timegraph", "0", CVAR_CHEAT);
	cl_debuggraph = Cvar_Get ("debuggraph", "0", CVAR_CHEAT);
	cl_graphheight = Cvar_Get ("graphheight", "32", CVAR_CHEAT);
	cl_graphscale = Cvar_Get ("graphscale", "1", CVAR_CHEAT);
	cl_graphshift = Cvar_Get ("graphshift", "0", CVAR_CHEAT);
	scr_initialized = qtrue;
}

void SCR_DrawScreenField() {
	switch( clc.state ) {
		default:
			Com_Error( ERR_FATAL, "SCR_DrawScreenField: bad clc.state" );
			break;
		case CA_DISCONNECTED:
			S_StopAllSounds();
			break;
		case CA_CONNECTING:
		case CA_CHALLENGING:
		case CA_CONNECTED:
			break;
		case CA_LOADING:
		case CA_PRIMED:
			CL_CGameRendering();
			break;
		case CA_ACTIVE:
			CL_CGameRendering();
			break;
	}

	// debug graph can be drawn on top of anything
	if ( cl_debuggraph->integer || cl_timegraph->integer || cl_debugMove->integer ) {
		SCR_DrawDebugGraph ();
	}
}

void SCR_UpdateScreen( void ) {
	static int	recursive;

	if ( !scr_initialized ) {
		return;				// not initialized yet
	}
	
	if ( ++recursive > 2 ) {
		Com_Error( ERR_FATAL, "SCR_UpdateScreen: recursively called" );
	}
	recursive = 1;

	#ifdef EMSCRIPTEN
	EM_ASM_({
	RE_BeginFrame_callback($0, $1, $2);
	}, cg_fov.value, cg.refdef.fov_x, cg.refdef.fov_y);
	#endif

	SCR_DrawScreenField();

	#ifdef EMSCRIPTEN
	EM_ASM_({
	RE_EndFrame_callback($0);
	}, 123);
	#endif

	recursive = 0;
}

