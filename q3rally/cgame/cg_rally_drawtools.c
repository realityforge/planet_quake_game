/*
**  	Copyright (C) 2004 by the Q3Rally Development team
**  	All rights reserved.
**
**  	cg_rally_drawtools.c
**
**  	Contains some draw code for Q3Rally hud text.
**
**		Author: STONELANCE
*/

#include "cg_local.h"


/*
===============
CG_DrawDigitalChar

Coordinates and size in 640*480 virtual screen size
===============
*/
void CG_DrawDigitalChar( int x, int y, int width, int height, int ch ) {
	int row, col;
	float frow, fcol;
	float size;
	float	ax, ay, aw, ah;

	ch &= 255;

	if ( ch == ' ' ) {
		return;
	}

	ax = x;
	ay = y;
	aw = width;
	ah = height;
	CG_AdjustFrom640( &ax, &ay, &aw, &ah );

	row = ch>>4;
	col = ch&15;

	frow = 0.5 + row*0.0625;
	fcol = col*0.0625;
	size = 0.0625;

	trap_R_DrawStretchPic( ax, ay, aw, ah,
					   fcol, frow, 
					   fcol + size, frow + size, 
					   cgs.media.charsetShader );
}


/*
==================
CG_DrawDigitalStringExt

Draws a multi-colored string with a drop shadow, optionally forcing
to a fixed color.

Coordinates are at 640 by 480 virtual resolution
==================
*/
void CG_DrawDigitalStringExt( int x, int y, const char *string, const float *setColor, 
		qboolean forceColor, qboolean shadow, int charWidth, int charHeight, int maxChars ) {
	vec4_t		color;
	const char	*s;
	int			xx;
	int			cnt;

	if (maxChars <= 0)
		maxChars = 32767; // do them all!

// STONELANCE - move font closer together
	x -= 1;
// END

	// draw the drop shadow
	if (shadow) {
		color[0] = color[1] = color[2] = 0;
		color[3] = setColor[3];
		trap_R_SetColor( color );
		s = string;
		xx = x;
		cnt = 0;
		while ( *s && cnt < maxChars) {
			if ( Q_IsColorString( s ) ) {
				s += 2;
				continue;
			}
			CG_DrawDigitalChar( xx + 2, y + 2, charWidth, charHeight, *s );
			cnt++;
			xx += charWidth - 2;
			s++;
		}
	}

	// draw the colored text
	s = string;
	xx = x;
	cnt = 0;
	trap_R_SetColor( setColor );
	while ( *s && cnt < maxChars) {
		if ( Q_IsColorString( s ) ) {
			if ( !forceColor ) {
				memcpy( color, g_color_table[ColorIndex(*(s+1))], sizeof( color ) );
				color[3] = setColor[3];
				trap_R_SetColor( color );
			}
			s += 2;
			continue;
		}

		CG_DrawDigitalChar( xx, y, charWidth, charHeight, *s );
		xx += charWidth - 2;
		cnt++;
		s++;
	}
	trap_R_SetColor( NULL );
}


void CG_DrawGiantDigitalString( int x, int y, const char *s, float alpha ) {
	float	color[4];

	color[0] = color[1] = color[2] = 1.0;
	color[3] = alpha;
	CG_DrawDigitalStringExt( x, y, s, color, qfalse, qfalse, GIANTCHAR_WIDTH, GIANTCHAR_HEIGHT, 0 );
}

void CG_DrawGiantDigitalStringColor( int x, int y, const char *s, vec4_t color ) {
	CG_DrawDigitalStringExt( x, y, s, color, qtrue, qfalse, GIANTCHAR_WIDTH, GIANTCHAR_HEIGHT, 0 );
}

void CG_DrawSmallDigitalString( int x, int y, const char *s, float alpha ) {
	float	color[4];

	color[0] = color[1] = color[2] = 1.0;
	color[3] = alpha;
	CG_DrawDigitalStringExt( x, y, s, color, qfalse, qfalse, SMALLCHAR_WIDTH+2, SMALLCHAR_HEIGHT, 0 );
}

void CG_DrawSmallDigitalStringColor( int x, int y, const char *s, vec4_t color ) {
	CG_DrawDigitalStringExt( x, y, s, color, qtrue, qfalse, SMALLCHAR_WIDTH+2, SMALLCHAR_HEIGHT, 0 );
}

void CG_DrawTinyDigitalString( int x, int y, const char *s, float alpha ) {
	float	color[4];

	color[0] = color[1] = color[2] = 1.0;
	color[3] = alpha;
	CG_DrawDigitalStringExt( x, y, s, color, qfalse, qfalse, TINYCHAR_WIDTH+2, TINYCHAR_HEIGHT, 0 );
}

void CG_DrawTinyDigitalStringColor( int x, int y, const char *s, vec4_t color ) {
	CG_DrawDigitalStringExt( x, y, s, color, qtrue, qfalse, TINYCHAR_WIDTH+2, TINYCHAR_HEIGHT, 0 );
}

void CG_DrawTinyString( int x, int y, const char *s, float alpha ) {
	float	color[4];

	color[0] = color[1] = color[2] = 1.0;
	color[3] = alpha;
	CG_DrawStringExt( x, y, s, color, qfalse, qfalse, TINYCHAR_WIDTH+2, TINYCHAR_HEIGHT, 0 );
}

void CG_DrawTinyStringColor( int x, int y, const char *s, vec4_t color ) {
	CG_DrawStringExt( x, y, s, color, qtrue, qfalse, TINYCHAR_WIDTH+2, TINYCHAR_HEIGHT, 0 );
}
