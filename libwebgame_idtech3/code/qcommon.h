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
// qcommon.h -- definitions common between client and server, but not game.or ref modules
#ifndef _QCOMMON_H_
#define _QCOMMON_H_

#include "cm_public.h"

//Ignore __attribute__ on non-gcc platforms
#ifndef __GNUC__
#ifndef __attribute__
#define __attribute__(x)
#endif
#endif

//#define	PRE_RELEASE_DEMO

//============================================================================

//
// msg.c
//
typedef struct {
	qboolean	allowoverflow;	// if false, do a Com_Error
	qboolean	overflowed;		// set to true if the buffer size failed (with allowoverflow set)
	qboolean	oob;			// set to true if the buffer size failed (with allowoverflow set)
	byte	*data;
	int		maxsize;
	int		cursize;
	int		readcount;
	int		bit;				// for bitwise reads and writes
} msg_t;

CCALL void MSG_Init (msg_t *buf, byte *data, int length);
CCALL void MSG_InitOOB( msg_t *buf, byte *data, int length );
CCALL void MSG_Clear (msg_t *buf);
CCALL void MSG_WriteData (msg_t *buf, const void *data, int length);
CCALL void MSG_Bitstream( msg_t *buf );

// TTimo
// copy a msg_t in case we need to store it as is for a bit
// (as I needed this to keep an msg_t from a static var for later use)
// sets data buffer as MSG_Init does prior to do the copy
CCALL void MSG_Copy(msg_t *buf, byte *data, int length, msg_t *src);

struct usercmd_s;
struct entityState_s;
struct playerState_s;

CCALL void MSG_WriteBits( msg_t *msg, int value, int bits );
CCALL void MSG_WriteChar (msg_t *sb, int c);
CCALL void MSG_WriteByte (msg_t *sb, int c);
CCALL void MSG_WriteShort (msg_t *sb, int c);
CCALL void MSG_WriteLong (msg_t *sb, int c);
CCALL void MSG_WriteFloat (msg_t *sb, float f);
CCALL void MSG_WriteString (msg_t *sb, const char *s);
CCALL void MSG_WriteBigString (msg_t *sb, const char *s);
CCALL void MSG_WriteAngle16 (msg_t *sb, float f);
CCALL int MSG_HashKey(const char *string, int maxlen);
CCALL void	MSG_BeginReading (msg_t *sb);
CCALL void	MSG_BeginReadingOOB(msg_t *sb);
CCALL int		MSG_ReadBits( msg_t *msg, int bits );
CCALL int		MSG_ReadChar (msg_t *sb);
CCALL int		MSG_ReadByte (msg_t *sb);
CCALL int		MSG_ReadShort (msg_t *sb);
CCALL int		MSG_ReadLong (msg_t *sb);
CCALL float	MSG_ReadFloat (msg_t *sb);
CCALL char	*MSG_ReadString (msg_t *sb);
CCALL char	*MSG_ReadBigString (msg_t *sb);
CCALL char	*MSG_ReadStringLine (msg_t *sb);
CCALL float	MSG_ReadAngle16 (msg_t *sb);
CCALL void	MSG_ReadData (msg_t *sb, void *buffer, int size);
CCALL int		MSG_LookaheadByte (msg_t *msg);
CCALL void MSG_WriteDeltaUsercmdKey( msg_t *msg, int key, usercmd_t *from, usercmd_t *to );
CCALL void MSG_ReadDeltaUsercmdKey( msg_t *msg, int key, usercmd_t *from, usercmd_t *to );
CCALL void MSG_WriteDeltaEntity( msg_t *msg, struct entityState_s *from, struct entityState_s *to, qboolean force );
CCALL void MSG_ReadDeltaEntity( msg_t *msg, entityState_t *from, entityState_t *to, int number );
CCALL void MSG_WriteDeltaPlayerstate( msg_t *msg, struct playerState_s *from, struct playerState_s *to );
CCALL void MSG_ReadDeltaPlayerstate( msg_t *msg, struct playerState_s *from, struct playerState_s *to );
CCALL void MSG_ReportChangeVectors_f( void );

//============================================================================

/*
==============================================================

NET

==============================================================
*/

#define NET_ENABLEV4            0x01
#define NET_ENABLEV6            0x02
// if this flag is set, always attempt ipv6 connections instead of ipv4 if a v6 address is found.
#define NET_PRIOV6              0x04
// disables ipv6 multicast support if set.
#define NET_DISABLEMCAST        0x08


#define	PACKET_BACKUP	32	// number of old messages that must be kept on client and
							// server for delta comrpession and ping estimation
#define	PACKET_MASK		(PACKET_BACKUP-1)

#define	MAX_PACKET_USERCMDS		32		// max number of usercmd_t in a packet

#define	MAX_SNAPSHOT_ENTITIES	256

#define	PORT_ANY			-1

#define	MAX_RELIABLE_COMMANDS	64			// max string commands buffered for restransmit

typedef enum {
	NA_BAD = 0,					// an address lookup failed
	NA_BOT,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IP6,
	NA_MULTICAST6,
	NA_UNSPEC
} netadrtype_t;

typedef enum {
	NS_CLIENT,
	NS_SERVER
} netsrc_t;

#define NET_ADDRSTRMAXLEN 48	// maximum length of an IPv6 address string including trailing '\0'
typedef struct {
	netadrtype_t	type;

	byte	ip[4];
	byte	ip6[16];

	unsigned short	port;
	unsigned long	scope_id;	// Needed for IPv6 link-local addresses
} netadr_t;

CCALL void		NET_Init( void );
CCALL void		NET_Shutdown( void );
CCALL void		NET_Restart_f( void );
CCALL void		NET_Config( qboolean enableNetworking );
CCALL void		NET_FlushPacketQueue(void);
CCALL void		NET_SendPacket (netsrc_t sock, int length, const void *data, netadr_t to);
CCALL void		NET_OutOfBandPrint( netsrc_t net_socket, netadr_t adr, const char *format, ...) __attribute__ ((format (printf, 3, 4)));
CCALL void		NET_OutOfBandData( netsrc_t sock, netadr_t adr, byte *format, int len );

CCALL qboolean	NET_CompareAdr (netadr_t a, netadr_t b);
CCALL qboolean	NET_CompareBaseAdrMask(netadr_t a, netadr_t b, int netmask);
CCALL qboolean	NET_CompareBaseAdr (netadr_t a, netadr_t b);
CCALL qboolean	NET_IsLocalAddress (netadr_t adr);
CCALL const char	*NET_AdrToString (netadr_t a);
CCALL const char	*NET_AdrToStringwPort (netadr_t a);
CCALL int		NET_StringToAdr ( const char *s, netadr_t *a, netadrtype_t family);
CCALL qboolean	NET_GetLoopPacket (netsrc_t sock, netadr_t *net_from, msg_t *net_message);
CCALL void		NET_Sleep(int msec);


#define	MAX_MSGLEN				16384		// max length of a message, which may
											// be fragmented into multiple packets

#define MAX_DOWNLOAD_WINDOW		48	// ACK window of 48 download chunks. Cannot set this higher, or clients
						// will overflow the reliable commands buffer
#define MAX_DOWNLOAD_BLKSIZE		1024	// 896 byte block chunks

#define NETCHAN_GENCHECKSUM(challenge, sequence) ((challenge) ^ ((sequence) * (challenge)))

/*
Netchan handles packet fragmentation and out of order / duplicate suppression
*/

typedef struct {
	netsrc_t	sock;

	int			dropped;			// between last packet and previous

	netadr_t	remoteAddress;
	int			qport;				// qport value to write when transmitting

	// sequencing variables
	int			incomingSequence;
	int			outgoingSequence;

	// incoming fragment assembly buffer
	int			fragmentSequence;
	int			fragmentLength;	
	byte		fragmentBuffer[MAX_MSGLEN];

	// outgoing fragment buffer
	// we need to space out the sending of large fragmented messages
	qboolean	unsentFragments;
	int			unsentFragmentStart;
	int			unsentLength;
	byte		unsentBuffer[MAX_MSGLEN];

	int			challenge;
	int		lastSentTime;
	int		lastSentSize;

#ifdef LEGACY_PROTOCOL
	qboolean	compat;
#endif
} netchan_t;

CCALL void Netchan_Init( int qport );
CCALL void Netchan_Setup(netsrc_t sock, netchan_t *chan, netadr_t adr, int qport, int challenge, qboolean compat);

CCALL void Netchan_Transmit( netchan_t *chan, int length, const byte *data );
CCALL void Netchan_TransmitNextFragment( netchan_t *chan );

CCALL qboolean Netchan_Process( netchan_t *chan, msg_t *msg );


/*
==============================================================

PROTOCOL

==============================================================
*/

#define	PROTOCOL_VERSION	71
#define PROTOCOL_LEGACY_VERSION	68
// 1.31 - 67

// maintain a list of compatible protocols for demo playing
// NOTE: that stuff only works with two digits protocols
EXTERNC int demo_protocols[];

// override on command line, config files etc.
#ifndef MASTER_SERVER_NAME
#define MASTER_SERVER_NAME	"master.quake3arena.com"
#endif

#ifndef STANDALONE
  #ifndef AUTHORIZE_SERVER_NAME
    #define	AUTHORIZE_SERVER_NAME	"authorize.quake3arena.com"
  #endif
  #ifndef PORT_AUTHORIZE
  #define	PORT_AUTHORIZE		27952
  #endif
#endif

#define	PORT_MASTER			27950
#define	PORT_UPDATE			27951
#define	PORT_SERVER			27960
#define	NUM_SERVER_PORTS	4		// broadcast scan this many ports after
									// PORT_SERVER so a single machine can
									// run multiple servers


// the svc_strings[] array in cl_parse.c should mirror this
//
// server to client
//
enum svc_ops_e {
	svc_bad,
	svc_nop,
	svc_gamestate,
	svc_configstring,			// [short] [string] only in gamestate messages
	svc_baseline,				// only in gamestate messages
	svc_serverCommand,			// [string] to be executed by client game module
	svc_download,				// [short] size [size bytes]
	svc_snapshot,
	svc_EOF,

// new commands, supported only by ioquake3 protocol but not legacy
	svc_voipSpeex,     // not wrapped in USE_VOIP, so this value is reserved.
	svc_voipOpus,      //

	// libwebgame
	svc_usermessage_1, // support for custom user messages like in Garrys Mod
	svc_usermessage_2, // support for custom user messages like in Garrys Mod
	svc_usermessage_3, // support for custom user messages like in Garrys Mod
	svc_usermessage_4, // support for custom user messages like in Garrys Mod
	svc_usermessage_5, // support for custom user messages like in Garrys Mod
	svc_hud_free,
};


//
// client to server
//
enum clc_ops_e {
	clc_bad,
	clc_nop, 		
	clc_move,				// [[usercmd_t]
	clc_moveNoDelta,		// [[usercmd_t]
	clc_clientCommand,		// [string] message
	clc_EOF,

// new commands, supported only by ioquake3 protocol but not legacy
	clc_voipSpeex,   // not wrapped in USE_VOIP, so this value is reserved.
	clc_voipOpus,    //
};

/*
==============================================================

CMD

Command text buffering and command execution

==============================================================
*/

/*

Any number of commands can be added in a frame, from several different sources.
Most commands come from either keybindings or console line input, but entire text
files can be execed.

*/

CCALL void Cbuf_Init (void);
// allocates an initial text buffer that will grow as needed

CCALL void Cbuf_AddText( const char *text );
// Adds command text at the end of the buffer, does NOT add a final \n

CCALL void Cbuf_ExecuteText( int exec_when, const char *text );
// this can be used in place of either Cbuf_AddText or Cbuf_InsertText

CCALL void Cbuf_Execute (void);
// Pulls off \n terminated lines of text from the command buffer and sends
// them through Cmd_ExecuteString.  Stops when the buffer is empty.
// Normally called once per frame, but may be explicitly invoked.
// Do not call inside a command function, or current args will be destroyed.

//===========================================================================

/*

Command execution takes a null terminated string, breaks it into tokens,
then searches for a command or variable that matches the first token.

*/

typedef void (*xcommand_t) (void);

CCALL void	Cmd_Init (void);

CCALL void	Cmd_AddCommand( const char *cmd_name, xcommand_t function );
// called by the init functions of other parts of the program to
// register commands and functions to call for them.
// The cmd_name is referenced later, so it should not be in temp memory
// if function is NULL, the command will be forwarded to the server
// as a clc_clientCommand instead of executed locally

CCALL void	Cmd_RemoveCommand( const char *cmd_name );

typedef void (*completionFunc_t)( char *args, int argNum );

// don't allow VMs to remove system commands
CCALL void	Cmd_RemoveCommandSafe( const char *cmd_name );

CCALL void	Cmd_CommandCompletion( void(*callback)(const char *s) );
// callback with each valid string
CCALL void Cmd_SetCommandCompletionFunc( const char *command, completionFunc_t complete );
CCALL void Cmd_CompleteArgument( const char *command, char *args, int argNum );

CCALL int		Cmd_Argc (void);
CCALL char	*Cmd_Argv (int arg);
CCALL void	Cmd_ArgvBuffer( int arg, char *buffer, int bufferLength );
CCALL char	*Cmd_Args (void);
CCALL char	*Cmd_ArgsFrom( int arg );
CCALL void	Cmd_ArgsBuffer( char *buffer, int bufferLength );
CCALL char	*Cmd_Cmd (void);
CCALL void	Cmd_Args_Sanitize( void );
// The functions that execute commands get their parameters with these
// functions. Cmd_Argv () will return an empty string, not a NULL
// if arg > argc, so string operations are allways safe.

CCALL void	Cmd_TokenizeString( const char *text );
CCALL void	Cmd_TokenizeStringIgnoreQuotes( const char *text_in );
// Takes a null terminated string.  Does not need to be /n terminated.
// breaks the string up into arg tokens.

CCALL void	Cmd_ExecuteString( const char *text );
// Parses a single line of text into arguments and tries to execute it
// as if it was typed at the console


/*
==============================================================

CVAR

==============================================================
*/

/*

cvar_t variables are used to hold scalar or string variables that can be changed
or displayed at the console or prog code as well as accessed directly
in C code.

The user can access cvars from the console in three ways:
r_draworder			prints the current value
r_draworder 0		sets the current value to 0
set r_draworder 0	as above, but creates the cvar if not present

Cvars are restricted from having the same names as commands to keep this
interface from being ambiguous.

The are also occasionally used to communicated information between different
modules of the program.

*/

CCALL cvar_t *Cvar_Get( const char *var_name, const char *value, int flags );
// creates the variable if it doesn't exist, or returns the existing one
// if it exists, the value will not be changed, but flags will be ORed in
// that allows variables to be unarchived without needing bitflags
// if value is "", the value will not override a previously set value.

CCALL void	Cvar_Register( vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags );
// basically a slightly modified Cvar_Get for the interpreted modules

CCALL void	Cvar_Update( vmCvar_t *vmCvar );
// updates an interpreted modules' version of a cvar

CCALL void 	Cvar_Set( const char *var_name, const char *value );
// will create the variable with no flags if it doesn't exist

CCALL cvar_t	*Cvar_Set2(const char *var_name, const char *value, qboolean force);
// same as Cvar_Set, but allows more control over setting of cvar

CCALL void	Cvar_SetSafe( const char *var_name, const char *value );
// sometimes we set variables from an untrusted source: fail if flags & CVAR_PROTECTED

CCALL void Cvar_SetLatched( const char *var_name, const char *value);
// don't set the cvar immediately

CCALL void	Cvar_SetValue( const char *var_name, float value );
CCALL void	Cvar_SetValueSafe( const char *var_name, float value );
// expands value to a string and calls Cvar_Set/Cvar_SetSafe

CCALL float	Cvar_VariableValue( const char *var_name );
CCALL int		Cvar_VariableIntegerValue( const char *var_name );
// returns 0 if not defined or non numeric

CCALL char	*Cvar_VariableString( const char *var_name );
CCALL void	Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize );
// returns an empty string if not defined

CCALL int	Cvar_Flags(const char *var_name);
// returns CVAR_NONEXISTENT if cvar doesn't exist or the flags of that particular CVAR.

CCALL void 	Cvar_Reset( const char *var_name );
CCALL void 	Cvar_ForceReset(const char *var_name);

CCALL void	Cvar_SetCheatState( void );
// reset all testing vars to a safe value

CCALL qboolean Cvar_Command( void );
// called by Cmd_ExecuteString when Cmd_Argv(0) doesn't match a known
// command.  Returns true if the command was a variable reference that
// was handled. (print or change)

CCALL void 	Cvar_WriteVariables( fileHandle_t f );
// writes lines containing "set variable value" for all variables
// with the archive flag set to true.

CCALL void	Cvar_Init( void );

CCALL char	*Cvar_InfoString( int bit );
CCALL char	*Cvar_InfoString_Big( int bit );
// returns an info string containing all the cvars that have the given bit set
// in their flags ( CVAR_USERINFO, CVAR_SERVERINFO, CVAR_SYSTEMINFO, etc )
CCALL void	Cvar_InfoStringBuffer( int bit, char *buff, int buffsize );
CCALL void Cvar_CheckRange( cvar_t *cv, float minVal, float maxVal, qboolean shouldBeIntegral );
CCALL void Cvar_SetDescription( cvar_t *var, const char *var_description );

CCALL void	Cvar_Restart(qboolean unsetVM);
CCALL void	Cvar_Restart_f( void );

CCALL void Cvar_CompleteCvarName( char *args, int argNum );

extern	int			cvar_modifiedFlags;
// whenever a cvar is modifed, its flags will be OR'd into this, so
// a single check can determine if any CVAR_USERINFO, CVAR_SERVERINFO,
// etc, variables have been modified since the last check.  The bit
// can then be cleared to allow another change detection.

/*
==============================================================

FILESYSTEM

No stdio calls should be used by any part of the game, because
we need to deal with all sorts of directory and seperator char
issues.
==============================================================
*/

// referenced flags
// these are in loop specific order so don't change the order
#define FS_GENERAL_REF	0x01
#define FS_UI_REF		0x02
#define FS_CGAME_REF	0x04
// number of id paks that will never be autodownloaded from baseq3/missionpack
#define NUM_ID_PAKS		9
#define NUM_TA_PAKS		4

#define	MAX_FILE_HANDLES	64

#ifdef DEDICATED
#	define Q3CONFIG_CFG "q3config_server.cfg"
#else
#	define Q3CONFIG_CFG "q3config.cfg"
#endif

CCALL long	FS_ReadFile(const char *qpath, void **buffer);
CCALL void 	FS_Printf( fileHandle_t f, const char *fmt, ... ) __attribute__ ((format (printf, 2, 3)));
// like fprintf


/*
==============================================================

MISC

==============================================================
*/

// centralized and cleaned, that's the max string you can send to a Com_Printf / Com_DPrintf (above gets truncated)
#define	MAXPRINTMSG	4096


typedef enum {
	// SE_NONE must be zero
	SE_NONE = 0,		// evTime is still valid
	SE_KEY,			// evValue is a key code, evValue2 is the down flag
	SE_CHAR,		// evValue is an ascii char
	SE_MOUSE,		// evValue and evValue2 are relative signed x / y moves
	SE_JOYSTICK_AXIS,	// evValue is an axis number and evValue2 is the current state (-127 to 127)
	SE_CONSOLE		// evPtr is a char*
} sysEventType_t;

typedef struct {
	int				evTime;
	sysEventType_t	evType;
	int				evValue, evValue2;
	int				evPtrLength;	// bytes of data pointed to by evPtr, for journaling
	void			*evPtr;			// this must be manually freed if not NULL
} sysEvent_t;

CCALL void		Com_QueueEvent( int time, sysEventType_t type, int value, int value2, int ptrLength, void *ptr );
CCALL int			Com_EventLoop( void );
CCALL sysEvent_t	Com_GetSystemEvent( void );
CCALL char		*CopyString( const char *in );
CCALL void		Info_Print( const char *s );

CCALL void		Com_BeginRedirect (char *buffer, int buffersize, void (*flush)(char *));
CCALL void		Com_EndRedirect( void );
CCALL void 		Com_Printf( const char *fmt, ... ) __attribute__ ((format (printf, 1, 2)));
CCALL void 		Com_DPrintf( const char *fmt, ... ) __attribute__ ((format (printf, 1, 2)));
CCALL void 		Com_Error( int code, const char *fmt, ... ) __attribute__ ((noreturn, format(printf, 2, 3)));
CCALL void 		Com_Quit_f( void ) __attribute__ ((noreturn));
CCALL void		Com_GameRestart(int checksumFeed, qboolean disconnect);

CCALL int			Com_Milliseconds( void );	// will be journaled properly
CCALL int			Com_Filter(char *filter, char *name, int casesensitive);
CCALL qboolean	Com_SafeMode( void );
CCALL void		Com_RunAndTimeServerPacket(netadr_t *evFrom, msg_t *buf);

CCALL qboolean	Com_IsVoipTarget(uint8_t *voipTargets, int voipTargetsSize, int clientNum);

CCALL void		Com_StartupVariable( const char *match );
// checks for and removes command line "+set var arg" constructs
// if match is NULL, all set commands will be executed, otherwise
// only a set with the exact name.  Only used during startup.

CCALL qboolean		Com_PlayerNameToFieldString( char *str, int length, const char *name );
CCALL int Com_strCompare( const void *a, const void *b );


EXTERNC	cvar_t	*com_developer;
EXTERNC	cvar_t	*com_dedicated;
EXTERNC	cvar_t	*com_speeds;
EXTERNC	cvar_t	*com_timescale;
EXTERNC	cvar_t	*com_sv_running;
EXTERNC	cvar_t	*com_cl_running;
EXTERNC	cvar_t	*com_version;
EXTERNC	cvar_t	*com_blood;
EXTERNC	cvar_t	*com_buildScript;		// for building release pak files
EXTERNC	cvar_t	*com_journal;
EXTERNC	cvar_t	*com_cameraMode;
EXTERNC	cvar_t	*com_ansiColor;
EXTERNC	cvar_t	*com_unfocused;
EXTERNC	cvar_t	*com_maxfpsUnfocused;
EXTERNC	cvar_t	*com_minimized;
EXTERNC	cvar_t	*com_maxfpsMinimized;
EXTERNC	cvar_t	*com_standalone;
EXTERNC	cvar_t	*com_basegame;
EXTERNC	cvar_t	*com_homepath;

// both client and server must agree to pause
EXTERNC	cvar_t	*cl_paused;
EXTERNC	cvar_t	*sv_paused;

EXTERNC	cvar_t	*cl_packetdelay;
EXTERNC	cvar_t	*sv_packetdelay;

EXTERNC	cvar_t	*com_gamename;
EXTERNC	cvar_t	*com_protocol;
#ifdef LEGACY_PROTOCOL
EXTERNC	cvar_t	*com_legacyprotocol;
#endif

// com_speeds times
EXTERNC	int		time_game;
EXTERNC	int		time_frontend;
EXTERNC	int		time_backend;		// renderer backend time

EXTERNC	int		com_frameTime;

EXTERNC	qboolean	com_errorEntered;
EXTERNC	qboolean	com_fullyInitialized;

EXTERNC	fileHandle_t	com_journalFile;
EXTERNC	fileHandle_t	com_journalDataFile;

typedef enum {
	TAG_FREE,
	TAG_GENERAL,
	TAG_BOTLIB,
	TAG_RENDERER,
	TAG_SMALL,
	TAG_STATIC
} memtag_t;

/*

--- low memory ----
server vm
server clipmap
---mark---
renderer initialization (shaders, etc)
UI vm
cgame vm
renderer map
renderer models

---free---

temp file loading
--- high memory ---

*/

#if !defined(NDEBUG) && !defined(BSPC)
	#define ZONE_DEBUG
#endif

#ifdef ZONE_DEBUG
#define Z_TagMalloc(size, tag)			Z_TagMallocDebug(size, tag, #size, __FILE__, __LINE__)
#define Z_Malloc(size)					Z_MallocDebug(size, #size, __FILE__, __LINE__)
#define S_Malloc(size)					S_MallocDebug(size, #size, __FILE__, __LINE__)
CCALL void *Z_TagMallocDebug( int size, int tag, char *label, char *file, int line );	// NOT 0 filled memory
CCALL void *Z_MallocDebug( int size, char *label, char *file, int line );			// returns 0 filled memory
CCALL void *S_MallocDebug( int size, char *label, char *file, int line );			// returns 0 filled memory
#else
CCALL void *Z_TagMalloc( int size, int tag );	// NOT 0 filled memory
CCALL void *Z_Malloc( int size );			// returns 0 filled memory
CCALL void *S_Malloc( int size );			// NOT 0 filled memory only for small allocations
#endif
CCALL void Z_Free( void *ptr );
CCALL void Z_FreeTags( int tag );
CCALL int Z_AvailableMemory( void );
CCALL void Z_LogHeap( void );

CCALL void Hunk_Clear( void );
CCALL void Hunk_ClearToMark( void );
CCALL void Hunk_SetMark( void );
CCALL qboolean Hunk_CheckMark( void );
CCALL void Hunk_ClearTempMemory( void );
CCALL void *Hunk_AllocateTempMemory( int size );
CCALL void Hunk_FreeTempMemory( void *buf );
CCALL int	Hunk_MemoryRemaining( void );
CCALL void Hunk_Log( void);

CCALL void Com_TouchMemory( void );

// commandLine should not include the executable name (argv[0])
CCALL void Com_Init( char *commandLine );
CCALL void Com_Frame( void );


/*
==============================================================

CLIENT / SERVER SYSTEMS

==============================================================
*/

//
// client interface
//
CCALL void CL_InitKeyCommands( void );
// the keyboard binding interface must be setup before execing
// config files, but the rest of client startup will happen later

CCALL void CL_Init( void );
CCALL void CL_Disconnect( qboolean showMainMenu );
CCALL void CL_Shutdown(char *finalmsg, qboolean disconnect, qboolean quit);
CCALL void CL_Frame( int msec );
CCALL qboolean CL_GameCommand( void );
CCALL void CL_KeyEvent (int key, qboolean down, unsigned time);

CCALL void CL_CharEvent( int key );
// char events are for field typing, not game control

CCALL void CL_MouseEvent( int dx, int dy, int time );
CCALL void CL_JoystickEvent( int axis, int value, int time );
CCALL void CL_PacketEvent( netadr_t from, msg_t *msg );
CCALL void CL_ConsolePrint( char *text );
CCALL void CL_MapLoading( void );
// do a screen update before starting to load a map
// when the server is going to load a new map, the entire hunk
// will be cleared, so the client must shutdown cgame, ui, and
// the renderer

CCALL void	CL_ForwardCommandToServer( const char *string );
// adds the current command line as a clc_clientCommand to the client message.
// things like godmode, noclip, etc, are commands directed to the server,
// so when they are typed in at the console, they will need to be forwarded.

CCALL void CL_FlushMemory( void ); // dump all memory on an error
CCALL void CL_ShutdownAll(qboolean shutdownRef); // shutdown client
CCALL void CL_InitRef(void); // initialize renderer interface
CCALL void CL_StartHunkUsers( qboolean rendererOnly ); // start all the client stuff using the hunk
CCALL void CL_Snd_Shutdown(void); // Restart sound subsystem
CCALL void Key_KeynameCompletion( void(*callback)(const char *s) ); // for keyname autocompletion
CCALL void Key_WriteBindings( fileHandle_t f ); // for writing the config files
CCALL void S_ClearSoundBuffer( void ); // call before filesystem access
CCALL void SCR_DebugGraph (float value);	// FIXME: move logging to common?

// AVI files have the start of pixel lines 4 byte-aligned
#define AVI_LINE_PADDING 4

//
// server interface
//
CCALL void SV_Init( void );
CCALL void SV_Shutdown( char *finalmsg );
CCALL void SV_Frame( int msec );
CCALL void SV_PacketEvent( netadr_t from, msg_t *msg );
CCALL int SV_FrameMsec(void);
CCALL qboolean SV_GameCommand( void );
CCALL int SV_SendQueuedPackets(void);


/*
==============================================================

NON-PORTABLE SYSTEM SERVICES

==============================================================
*/

#define MAX_JOYSTICK_AXIS 16

CCALL int Sys_Milliseconds (void); // Sys_Milliseconds should only be used for profiling purposes, any game related timing information should come from event timestamps
CCALL void	Sys_SendPacket( int length, const void *data, netadr_t to );
CCALL qboolean	Sys_StringToAdr( const char *s, netadr_t *a, netadrtype_t family ); //Does NOT parse port numbers, only base addresses.
CCALL qboolean	Sys_IsLANAddress (netadr_t adr);
CCALL void		Sys_ShowIP(void);

typedef enum
{
	DR_YES = 0,
	DR_NO = 1,
	DR_OK = 0,
	DR_CANCEL = 1
} dialogResult_t;

typedef enum
{
	DT_INFO,
	DT_WARNING,
	DT_ERROR,
	DT_YES_NO,
	DT_OK_CANCEL
} dialogType_t;

/* This is based on the Adaptive Huffman algorithm described in Sayood's Data
 * Compression book.  The ranks are not actually stored, but implicitly defined
 * by the location of a node within a doubly-linked list */

#define NYT HMAX					/* NYT = Not Yet Transmitted */
#define INTERNAL_NODE (HMAX+1)

typedef struct nodetype {
	struct	nodetype *left, *right, *parent; /* tree structure */ 
	struct	nodetype *next, *prev; /* doubly-linked list */
	struct	nodetype **head; /* highest ranked node in block */
	int		weight;
	int		symbol;
} node_t;

#define HMAX 256 /* Maximum symbol */

typedef struct {
	int			blocNode;
	int			blocPtrs;

	node_t*		tree;
	node_t*		lhead;
	node_t*		ltail;
	node_t*		loc[HMAX+1];
	node_t**	freelist;

	node_t		nodeList[768];
	node_t*		nodePtrs[768];
} huff_t;

typedef struct {
	huff_t		compressor;
	huff_t		decompressor;
} huffman_t;

CCALL void	Huff_Compress(msg_t *buf, int offset);
CCALL void	Huff_Decompress(msg_t *buf, int offset);
CCALL void	Huff_Init(huffman_t *huff);
CCALL void	Huff_addRef(huff_t* huff, byte ch);
CCALL int		Huff_Receive (node_t *node, int *ch, byte *fin);
CCALL void	Huff_transmit (huff_t *huff, int ch, byte *fout);
CCALL void	Huff_offsetReceive (node_t *node, int *ch, byte *fin, int *offset);
CCALL void	Huff_offsetTransmit (huff_t *huff, int ch, byte *fout, int *offset);
CCALL void	Huff_putBit( int bit, byte *fout, int *offset);
CCALL int		Huff_getBit( byte *fout, int *offset);

// don't use if you don't know what you're doing.
CCALL int		Huff_getBloc(void);
CCALL void	Huff_setBloc(int _bloc);


extern huffman_t clientHuffTables;

#define	SV_ENCODE_START		4
#define SV_DECODE_START		12
#define	CL_ENCODE_START		12
#define CL_DECODE_START		4

// flags for sv_allowDownload and cl_allowDownload
#define DLF_ENABLE 1
#define DLF_NO_REDIRECT 2
#define DLF_NO_UDP 4
#define DLF_NO_DISCONNECT 8

#endif // _QCOMMON_H_
