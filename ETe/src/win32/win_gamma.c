/*
===========================================================================

Wolfenstein: Enemy Territory GPL Source Code
Copyright (C) 1999-2010 id Software LLC, a ZeniMax Media company. 

This file is part of the Wolfenstein: Enemy Territory GPL Source Code (Wolf ET Source Code).  

Wolf ET Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Wolf ET Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Wolf ET Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Wolf: ET Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Wolf ET Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
/*
** WIN_GAMMA.C
*/
#include "../renderer/tr_local.h"
#include "../qcommon/qcommon.h"
#include "glw_win.h"
#include "win_local.h"

static unsigned short s_oldHardwareGamma[3][256];

/*
** GLW_InitGamma
**
** Determines if the underlying hardware supports the Win32 gamma correction API.
*/
void GLimp_InitGamma( glconfig_t *config )
{
	HDC		hDC;

	config->deviceSupportsGamma = qfalse;

	if ( glw_state.displayName[0] )
	{
		hDC = CreateDC( TEXT( "DISPLAY" ), glw_state.displayName, NULL, NULL );
		config->deviceSupportsGamma = ( GetDeviceGammaRamp( hDC, s_oldHardwareGamma ) == FALSE ) ? qfalse : qtrue;
		if ( config->deviceSupportsGamma )
		{
			// do test setup
			if ( SetDeviceGammaRamp( hDC, s_oldHardwareGamma ) == FALSE )
			{
				config->deviceSupportsGamma = qfalse;
			}
		}
		DeleteDC( hDC );
	}
	else
	{
		hDC = GetDC( GetDesktopWindow() );
		config->deviceSupportsGamma = ( GetDeviceGammaRamp( hDC, s_oldHardwareGamma ) == FALSE ) ? qfalse : qtrue;
		if ( config->deviceSupportsGamma )
		{
			if ( SetDeviceGammaRamp( hDC, s_oldHardwareGamma ) == FALSE )
			{
				config->deviceSupportsGamma = qfalse;
			}
		}
		ReleaseDC( GetDesktopWindow(), hDC );
	}

	if ( config->deviceSupportsGamma )
	{
		//
		// do a sanity check on the gamma values
		//
		if ( ( HIBYTE( s_oldHardwareGamma[0][255] ) <= HIBYTE( s_oldHardwareGamma[0][0] ) ) ||
			 ( HIBYTE( s_oldHardwareGamma[1][255] ) <= HIBYTE( s_oldHardwareGamma[1][0] ) ) ||
			 ( HIBYTE( s_oldHardwareGamma[2][255] ) <= HIBYTE( s_oldHardwareGamma[2][0] ) ) )
		{
			config->deviceSupportsGamma = qfalse;
			Com_Printf( S_COLOR_YELLOW "WARNING: device has broken gamma support\n" );
		}

		//
		// make sure that we didn't have a prior crash in the game, and if so we need to
		// restore the gamma values to at least a linear value
		//
		if ( ( HIBYTE( s_oldHardwareGamma[0][181] ) == 255 ) )
		{
			int g;

			Com_Printf( S_COLOR_YELLOW "WARNING: suspicious gamma tables, using linear ramp for restoration\n" );

			for ( g = 0; g < 256; g++ )
			{
				s_oldHardwareGamma[0][g] = g << 8;
				s_oldHardwareGamma[1][g] = g << 8;
				s_oldHardwareGamma[2][g] = g << 8;
			}
		}
	} // if ( config->deviceSupportsGamma )

	glw_state.deviceSupportsGamma = config->deviceSupportsGamma;
}


/*
void mapGammaMax( void ) {
	int		i, j;
	unsigned short table[3][256];

	// try to figure out what win2k will let us get away with setting
	for ( i = 0 ; i < 256 ; i++ ) {
		if ( i >= 128 ) {
			table[0][i] = table[1][i] = table[2][i] = 0xffff;
		} else {
			table[0][i] = table[1][i] = table[2][i] = i<<9;
		}
	}

	for ( i = 0 ; i < 128 ; i++ ) {
		for ( j = i*2 ; j < 255 ; j++ ) {
			table[0][i] = table[1][i] = table[2][i] = j<<8;
			if ( !SetDeviceGammaRamp( glw_state.hDC, table ) ) {
				break;
			}
		}
		table[0][i] = table[1][i] = table[2][i] = i<<9;
		Com_Printf( "index %i max: %i\n", i, j-1 );
	}
}
*/


/*
** GLimp_SetGamma
**
** This routine should only be called if glConfig.deviceSupportsGamma is TRUE
*/
void GLimp_SetGamma( unsigned char red[256], unsigned char green[256], unsigned char blue[256] ) {
	unsigned short table[3][256];
	int		i, j;
	BOOL	ret;
	HDC		hDC;

	if ( /*!glw_state.hDC* ||*/ !gw_active )
		return;

//mapGammaMax();

	for ( i = 0; i < 256; i++ ) {
		table[0][i] = ( ( ( unsigned short ) red[i] ) << 8 ) | red[i];
		table[1][i] = ( ( ( unsigned short ) green[i] ) << 8 ) | green[i];
		table[2][i] = ( ( ( unsigned short ) blue[i] ) << 8 ) | blue[i];
	}

	// Win2K and newer put this odd restriction on gamma ramps...
	Com_DPrintf( "performing gamma clamp.\n" );
	for ( j = 0 ; j < 3 ; j++ ) {
		for ( i = 0 ; i < 128 ; i++ ) {
			if ( table[j][i] > ( (128+i) << 8 ) ) {
				table[j][i] = (128+i) << 8;
			}
		}
		if ( table[j][127] > 254<<8 ) {
			table[j][127] = 254<<8;
		}
	}

	// enforce constantly increasing
	for ( j = 0 ; j < 3 ; j++ ) {
		for ( i = 1 ; i < 256 ; i++ ) {
			if ( table[j][i] < table[j][i-1] ) {
				table[j][i] = table[j][i-1];
			}
		}
	}

	if ( glw_state.displayName[0] ) {
		hDC = CreateDC( TEXT( "DISPLAY" ), glw_state.displayName, NULL, NULL );
		ret = SetDeviceGammaRamp( hDC, table );
		DeleteDC( hDC );
	} else {
		hDC = GetDC( GetDesktopWindow() );
		ret = SetDeviceGammaRamp( hDC, table );
		ReleaseDC( GetDesktopWindow(), hDC );
	}

	if ( !ret ) {
		Com_Printf( S_COLOR_YELLOW "SetDeviceGammaRamp failed.\n" );
	} else {
		glw_state.gammaSet = qtrue;
	}
}


/*
** GLW_RestoreGamma
*/
void GLW_RestoreGamma( void )
{
	HDC hDC;
	BOOL ret;

	if ( !glw_state.deviceSupportsGamma ) {
		return;
	}	

	if ( glw_state.displayName[0] ) {
		hDC = CreateDC( TEXT( "DISPLAY" ), glw_state.displayName, NULL, NULL );
		ret = SetDeviceGammaRamp( hDC, s_oldHardwareGamma );
		DeleteDC( hDC);
	} else {
		hDC = GetDC( GetDesktopWindow() );
		ret = SetDeviceGammaRamp( hDC, s_oldHardwareGamma );
		ReleaseDC( GetDesktopWindow(), hDC );
	}

	if ( ret ) {
		glw_state.gammaSet = qfalse;
	}
}
