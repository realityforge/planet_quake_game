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

/**********************************************************************
  UI_ATOMS.C
 
  User interface building blocks and support functions.
**********************************************************************/
#include "ui_local.h"

qboolean    m_entersound;    // after a frame, so caching won't disrupt the sound

void QDECL Com_Error( int level, const char *error, ... )
{
  va_list    argptr;
  char    text[1024];

  va_start( argptr, error );
  Q_vsnprintf( text, sizeof( text ), error, argptr );
  va_end( argptr );

  trap_Error( text );
}

void QDECL Com_Printf( const char *msg, ... )
{
  va_list    argptr;
  char    text[1024];

  va_start( argptr, msg );
  Q_vsnprintf( text, sizeof( text ), msg, argptr );
  va_end( argptr );

  trap_Print( text );
}


/*
=================
UI_ClampCvar
=================
*/
float UI_ClampCvar( float min, float max, float value )
{
  if( value < min ) return min;

  if( value > max ) return max;

  return value;
}

/*
=================
UI_StartDemoLoop
=================
*/
void UI_StartDemoLoop( void )
{
  trap_Cmd_ExecuteText( EXEC_APPEND, "d1\n" );
}

char *UI_Argv( int arg )
{
  static char  buffer[MAX_STRING_CHARS];

  trap_Argv( arg, buffer, sizeof( buffer ) );

  return buffer;
}

char *UI_ConcatArgs( int arg, char *buf, int len )
{
  char *p;
  int c;

  if( len <= 0 )
    return buf;

  p = buf;
  c = trap_Argc();

  for( ; arg < c; arg++ )
  {
    char *argp = UI_Argv( arg );

    while( *argp && p < &buf[ len - 1 ] )
      *p++ = *argp++;

    if( p < &buf[ len - 2 ] )
      *p++ = ' ';
    else
      break;
  }

  *p = '\0';

  return buf;
}

char *UI_Cvar_VariableString( const char *var_name )
{
  static char  buffer[MAX_STRING_CHARS];

  trap_Cvar_VariableStringBuffer( var_name, buffer, sizeof( buffer ) );

  return buffer;
}

static void  UI_Cache_f( void )
{
  Display_CacheAll();
}

static void UI_Menu_f( void )
{
    if( Menu_Count( ) > 0 )
    {
      trap_Key_SetCatcher( KEYCATCH_UI );
      Menus_ActivateByName( UI_Argv( 1 ) );
    }
}

static void UI_CloseMenus_f( void )
{
    if( Menu_Count( ) > 0 )
    {
      trap_Key_SetCatcher( trap_Key_GetCatcher( ) & ~KEYCATCH_UI );
      trap_Key_ClearStates( );
      trap_Cvar_Set( "cl_paused", "0" );
      Menus_CloseAll( );
    }
}

static void UI_MessageMode_f( void )
{
  char *arg = UI_Argv( 0 );

  trap_Cvar_Set( "ui_sayBuffer", "" );

  switch( arg[ 11 ] )
  {
    default:
    case '\0':
      // Global
      uiInfo.chatTeam             = qfalse;
      uiInfo.chatAdmins           = qfalse;
      uiInfo.chatClan             = qfalse;
      break;

    case '2':
      // Team
      uiInfo.chatTeam             = qtrue;
      uiInfo.chatAdmins           = qfalse;
      uiInfo.chatClan             = qfalse;
      break;

    case '5':
      // Admins
      uiInfo.chatTeam             = qfalse;
      uiInfo.chatAdmins           = qtrue;
      uiInfo.chatClan             = qfalse;
      break;

    case '6':
      // Clan
      uiInfo.chatTeam             = qfalse;
      uiInfo.chatAdmins           = qfalse;
      uiInfo.chatClan             = qtrue;
      break;
  }

  trap_Key_SetCatcher( KEYCATCH_UI );
  Menus_CloseByName( "say" );
  Menus_CloseByName( "say_team" );
  Menus_CloseByName( "say_admins" );
  Menus_CloseByName( "say_clan" );

  if( uiInfo.chatTeam )
    Menus_ActivateByName( "say_team" );
  else if( uiInfo.chatAdmins )
    Menus_ActivateByName( "say_admins" );
  else if( uiInfo.chatClan )
    Menus_ActivateByName( "say_clan" );
  else
    Menus_ActivateByName( "say" );
}

static void UI_Me_f( void )
{
  char buf[ MAX_SAY_TEXT - 4 ];

  UI_ConcatArgs( 1, buf, sizeof( buf ) );

  trap_Cmd_ExecuteText( EXEC_APPEND, va( "say \"/me %s\"\n", buf ) );
}

struct uicmd
{
  char *cmd;
  void ( *function )( void );
} commands[ ] = {
  { "closemenus", UI_CloseMenus_f },
  { "me", UI_Me_f },
  { "menu", UI_Menu_f },
  { "messagemode", UI_MessageMode_f },
  { "messagemode2", UI_MessageMode_f },
  { "messagemode5", UI_MessageMode_f },
  { "messagemode6", UI_MessageMode_f },
  { "ui_cache", UI_Cache_f },
  { "ui_load", UI_Load },
  { "ui_report", UI_Report }
};

/*
=================
UI_ConsoleCommand
=================
*/
qboolean UI_ConsoleCommand( int realTime )
{
  struct uicmd *cmd = bsearch( UI_Argv( 0 ), commands,
    ARRAY_LEN( commands ), sizeof( commands[ 0 ] ),
    cmdcmp );

  uiInfo.uiDC.frameTime = realTime - uiInfo.uiDC.realTime;
  uiInfo.uiDC.realTime = realTime;

  if( cmd )
  {
    cmd->function( );
    return qtrue;
  }

  return qfalse;
}

void UI_DrawNamedPic( float x, float y, float width, float height, const char *picname )
{
  qhandle_t  hShader;

  hShader = trap_R_RegisterShaderNoMip( picname );
  UI_AdjustFrom640( &x, &y, &width, &height );
  trap_R_DrawStretchPic( x, y, width, height, 0, 0, 1, 1, hShader );
}

void UI_DrawHandlePic( float x, float y, float w, float h, qhandle_t hShader )
{
  float  s0;
  float  s1;
  float  t0;
  float  t1;

  if( w < 0 )
  {  // flip about vertical
    w  = -w;
    s0 = 1;
    s1 = 0;
  }
  else
  {
    s0 = 0;
    s1 = 1;
  }

  if( h < 0 )
  {  // flip about horizontal
    h  = -h;
    t0 = 1;
    t1 = 0;
  }
  else
  {
    t0 = 0;
    t1 = 1;
  }

  UI_AdjustFrom640( &x, &y, &w, &h );
  trap_R_DrawStretchPic( x, y, w, h, s0, t0, s1, t1, hShader );
}

/*
================
UI_FillRect
 
Coordinates are 640*480 virtual values
=================
*/
void UI_FillRect( float x, float y, float width, float height, const float *color )
{
  trap_R_SetColor( color );

  UI_AdjustFrom640( &x, &y, &width, &height );
  trap_R_DrawStretchPic( x, y, width, height, 0, 0, 0, 0, uiInfo.uiDC.whiteShader );

  trap_R_SetColor( NULL );
}

void UI_SetColor( const float *rgba )
{
  trap_R_SetColor( rgba );
}
