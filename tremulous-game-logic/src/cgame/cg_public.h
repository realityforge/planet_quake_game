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


#define CMD_BACKUP      64
#define CMD_MASK      (CMD_BACKUP - 1)
// allow a lot of command backups for very fast systems
// multiple commands may be combined into a single packet, so this
// needs to be larger than PACKET_BACKUP


#define MAX_ENTITIES_IN_SNAPSHOT  256

// snapshots are a view of the server at a given time

// Snapshots are generated at regular time intervals by the server,
// but they may not be sent if a client's rate level is exceeded, or
// they may be dropped by the network.
typedef struct
{
  int           snapFlags;                            // SNAPFLAG_RATE_DELAYED, etc
  int           ping;

  int           serverTime;                           // server time the message is valid for (in msec)

  byte          areamask[ MAX_MAP_AREA_BYTES ];       // portalarea visibility bits

  playerState_t ps;                                   // complete information about the current player at this time

  int           numEntities;                          // all of the entities that need to be presented
  entityState_t entities[ MAX_ENTITIES_IN_SNAPSHOT ]; // at the time of this snapshot

  int           numServerCommands;                    // text based server commands to execute when this
  int           serverCommandSequence;                // snapshot becomes current
} snapshot_t;

enum
{
  CGAME_EVENT_NONE,
  CGAME_EVENT_TEAMMENU,
  CGAME_EVENT_SCOREBOARD,
  CGAME_EVENT_EDITHUD
};

/*
==================================================================

functions imported from the main executable

==================================================================
*/

#define CGAME_IMPORT_API_VERSION  4

typedef enum
{
  CG_PRINT,
  CG_ERROR,
  CG_MILLISECONDS,
  CG_CVAR_REGISTER,
  CG_CVAR_UPDATE,
  CG_CVAR_SET,
  CG_CVAR_VARIABLESTRINGBUFFER,
  CG_ARGC,
  CG_ARGV,
  CG_ARGS,
  CG_FS_FOPENFILE,
  CG_FS_READ,
  CG_FS_WRITE,
  CG_FS_FCLOSEFILE,
  CG_SENDCONSOLECOMMAND,
  CG_ADDCOMMAND,
  CG_SENDCLIENTCOMMAND,
  CG_UPDATESCREEN,
  CG_CM_LOADMAP,
  CG_CM_NUMINLINEMODELS,
  CG_CM_INLINEMODEL,
  CG_CM_LOADMODEL,
  CG_CM_TEMPBOXMODEL,
  CG_CM_POINTCONTENTS,
  CG_CM_TRANSFORMEDPOINTCONTENTS,
  CG_CM_BOXTRACE,
  CG_CM_TRANSFORMEDBOXTRACE,
  CG_CM_MARKFRAGMENTS,
  CG_S_STARTSOUND,
  CG_S_STARTLOCALSOUND,
  CG_S_CLEARLOOPINGSOUNDS,
  CG_S_ADDLOOPINGSOUND,
  CG_S_UPDATEENTITYPOSITION,
  CG_S_RESPATIALIZE,
  CG_S_REGISTERSOUND,
  CG_S_STARTBACKGROUNDTRACK,
  CG_R_LOADWORLDMAP,
  CG_R_REGISTERMODEL,
  CG_R_REGISTERSKIN,
  CG_R_REGISTERSHADER,
  CG_R_CLEARSCENE,
  CG_R_ADDREFENTITYTOSCENE,
  CG_R_ADDPOLYTOSCENE,
  CG_R_ADDLIGHTTOSCENE,
  CG_R_RENDERSCENE,
  CG_R_SETCOLOR,
#ifndef MODULE_INTERFACE_11
  CG_R_SETCLIPREGION,
#endif
  CG_R_DRAWSTRETCHPIC,
  CG_R_MODELBOUNDS,
  CG_R_LERPTAG,
  CG_GETGLCONFIG,
  CG_GETGAMESTATE,
  CG_GETCURRENTSNAPSHOTNUMBER,
  CG_GETSNAPSHOT,
  CG_GETSERVERCOMMAND,
  CG_GETCURRENTCMDNUMBER,
  CG_GETUSERCMD,
  CG_SETUSERCMDVALUE,
  CG_R_REGISTERSHADERNOMIP,
  CG_MEMORY_REMAINING,
  CG_R_REGISTERFONT,
  CG_KEY_ISDOWN,
  CG_KEY_GETCATCHER,
  CG_KEY_SETCATCHER,
  CG_KEY_GETKEY,
#ifdef MODULE_INTERFACE_11
  CG_PARSE_ADD_GLOBAL_DEFINE,
  CG_PARSE_LOAD_SOURCE,
  CG_PARSE_FREE_SOURCE,
  CG_PARSE_READ_TOKEN,
  CG_PARSE_SOURCE_FILE_AND_LINE,
#endif
  CG_S_STOPBACKGROUNDTRACK,
  CG_REAL_TIME,
  CG_SNAPVECTOR,
  CG_REMOVECOMMAND,
  CG_R_LIGHTFORPOINT,
  CG_CIN_PLAYCINEMATIC,
  CG_CIN_STOPCINEMATIC,
  CG_CIN_RUNCINEMATIC,
  CG_CIN_DRAWCINEMATIC,
  CG_CIN_SETEXTENTS,
  CG_R_REMAP_SHADER,
  CG_S_ADDREALLOOPINGSOUND,
  CG_S_STOPLOOPINGSOUND,

  CG_CM_TEMPCAPSULEMODEL,
  CG_CM_CAPSULETRACE,
  CG_CM_TRANSFORMEDCAPSULETRACE,
  CG_R_ADDADDITIVELIGHTTOSCENE,
  CG_GET_ENTITY_TOKEN,
  CG_R_ADDPOLYSTOSCENE,
  CG_R_INPVS,
  CG_FS_SEEK,
  CG_FS_GETFILELIST,
  CG_LITERAL_ARGS,
  CG_CM_BISPHERETRACE,
  CG_CM_TRANSFORMEDBISPHERETRACE,
  CG_GETDEMOSTATE,
  CG_GETDEMOPOS,
  CG_GETDEMONAME,

  CG_KEY_KEYNUMTOSTRINGBUF,
  CG_KEY_GETBINDINGBUF,
  CG_KEY_SETBINDING,

#ifndef MODULE_INTERFACE_11
  CG_PARSE_ADD_GLOBAL_DEFINE,
  CG_PARSE_LOAD_SOURCE,
  CG_PARSE_FREE_SOURCE,
  CG_PARSE_READ_TOKEN,
  CG_PARSE_SOURCE_FILE_AND_LINE,

  CG_KEY_SETOVERSTRIKEMODE,
  CG_KEY_GETOVERSTRIKEMODE,

  CG_S_SOUNDDURATION,
  CG_FIELD_COMPLETELIST,
#endif

  CG_MEMSET = 200,
  CG_MEMCPY,
  CG_STRNCPY,
  CG_SIN,
  CG_COS,
  CG_ATAN2,
  CG_SQRT,
  CG_FLOOR,
  CG_CEIL,

  CG_TESTPRINTINT,
  CG_TESTPRINTFLOAT,
  CG_ACOS
} cgameImport_t;


/*
==================================================================

functions exported to the main executable

==================================================================
*/

typedef enum
{
  CG_INIT,
  // void CG_Init( int serverMessageNum, int serverCommandSequence, int clientNum )
  // called when the level loads or when the renderer is restarted
  // all media should be registered at this time
  // cgame will display loading status by calling SCR_Update, which
  // will call CG_DrawInformation during the loading process
  // reliableCommandSequence will be 0 on fresh loads, but higher for
  // demos, tourney restarts, or vid_restarts

  CG_SHUTDOWN,
  // void (*CG_Shutdown)( void );
  // oportunity to flush and close any open files

  CG_CONSOLE_COMMAND,
  // qboolean (*CG_ConsoleCommand)( void );
  // a console command has been issued locally that is not recognized by the
  // main game system.
  // use Cmd_Argc() / Cmd_Argv() to read the command, return qfalse if the
  // command is not known to the game

  CG_DRAW_ACTIVE_FRAME,
  // void (*CG_DrawActiveFrame)( int serverTime, stereoFrame_t stereoView, qboolean demoPlayback );
  // Generates and draws a game scene and status information at the given time.
  // If demoPlayback is set, local movement prediction will not be enabled

  CG_CROSSHAIR_PLAYER,
  // int (*CG_CrosshairPlayer)( void );

  CG_LAST_ATTACKER,
  // int (*CG_LastAttacker)( void );

  CG_KEY_EVENT,
  // void  (*CG_KeyEvent)( int key, qboolean down );

  CG_MOUSE_EVENT,
  // void  (*CG_MouseEvent)( int dx, int dy );
  CG_EVENT_HANDLING,
  // void (*CG_EventHandling)(int type);

  CG_CONSOLE_TEXT,
  // void (*CG_ConsoleText)( void );
  // pass text that has been printed to the console to cgame
  // use Cmd_Argc() / Cmd_Argv() to read it

  CG_VOIP_STRING,
  // char *(*CG_VoIPString)( void );
  // returns a string of comma-delimited clientnums based on cl_voipSendTarget

  CG_CONSOLE_COMPLETARGUMENT
  // qboolean (*CG_Console_CompleteArgument)( int argNum )
  // Requests CGAME to try to complete the command line
  // argument. argNum indicates which argument we're completing. CGAME
  // uses trap_Argv and trap_Argc to read the command line
  // contents. Returns true if a completion function is found in
  // CGAME, otherwise client tries another completion method.  
} cgameExport_t;

//----------------------------------------------
