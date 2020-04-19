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

#ifndef CLIENT_H
#define CLIENT_H

#include "q_shared.h"
#include "qcommon.h"
#include "keys.h"
#include "client_sound.h"
#include "cg_public.h"
#include "bg_public.h"
#include "cg_local.h"
#include "client_render.h"

// file full of random crap that gets used to create cl_guid
#define QKEY_FILE "qkey"
#define QKEY_SIZE 2048

#define	RETRANSMIT_TIMEOUT	3000	// time between connection packet retransmits

// snapshots are a view of the server at a given time
typedef struct {
	qboolean		valid;			// cleared if delta parsing was invalid
	int				snapFlags;		// rate delayed and dropped commands

	int				serverTime;		// server time the message is valid for (in msec)

	int				messageNum;		// copied from netchan->incoming_sequence
	int				deltaNum;		// messageNum the delta is from
	int				ping;			// time from when cmdNum-1 was sent to time packet was reeceived
	byte			areamask[MAX_MAP_AREA_BYTES];		// portalarea visibility bits

	int				cmdNum;			// the next cmdNum the server is expecting
	playerState_t	ps;						// complete information about the current player at this time

	int				numEntities;			// all of the entities that need to be presented
	int				parseEntitiesNum;		// at the time of this snapshot

	int				serverCommandNum;		// execute all commands up to this before
											// making the snapshot current
} clSnapshot_t;



/*
=============================================================================

the clientActive_t structure is wiped completely at every
new gamestate_t, potentially several times during an established connection

=============================================================================
*/

typedef struct {
	int		p_cmdNumber;		// cl.cmdNumber when packet was sent
	int		p_serverTime;		// usercmd->serverTime when packet was sent
	int		p_realtime;			// cls.realtime when packet was sent
} outPacket_t;

// the parseEntities array must be large enough to hold PACKET_BACKUP frames of
// entities, so that when a delta compressed message arives from the server
// it can be un-deltad from the original 
#define	MAX_PARSE_ENTITIES	( PACKET_BACKUP * MAX_SNAPSHOT_ENTITIES )

EXTERNC int g_console_field_width;

typedef struct {
	int			timeoutcount;		// it requres several frames in a timeout condition
									// to disconnect, preventing debugging breaks from
									// causing immediate disconnects on continue
	clSnapshot_t	snap;			// latest received from server

	int			serverTime;			// may be paused during play
	int			oldServerTime;		// to prevent time from flowing bakcwards
	int			oldFrameServerTime;	// to check tournament restarts
	int			serverTimeDelta;	// cl.serverTime = cls.realtime + cl.serverTimeDelta
									// this value changes as net lag varies
	qboolean	extrapolatedSnapshot;	// set if any cgame frame has been forced to extrapolate
									// cleared when CL_AdjustTimeDelta looks at it
	qboolean	newSnapshots;		// set on parse of any valid packet

	gameState_t	gameState;			// configstrings
	char		mapname[MAX_QPATH];	// extracted from CS_SERVERINFO

	int			parseEntitiesNum;	// index (not anded off) into cl_parse_entities[]

	int			mouseDx[2], mouseDy[2];	// added to by mouse events
	int			mouseIndex;
	int			joystickAxis[MAX_JOYSTICK_AXIS];	// set by joystick events

	// cgame communicates a few values to the client system
	int			cgameUserCmdValue;	// current weapon to add to usercmd_t
	float		cgameSensitivity;

	// cmds[cmdNumber] is the predicted command, [cmdNumber-1] is the last
	// properly generated command
	usercmd_t	cmds[CMD_BACKUP];	// each mesage will send several old cmds
	int			cmdNumber;			// incremented each frame, because multiple
									// frames may need to be packed into a single packet

	outPacket_t	outPackets[PACKET_BACKUP];	// information about each packet we have sent out

	// the client maintains its own idea of view angles, which are
	// sent to the server each frame.  It is cleared to 0 upon entering each level.
	// the server sends a delta each frame which is added to the locally
	// tracked view angles to account for standing on rotating objects,
	// and teleport direction changes
	vec3_t		viewangles;

	int			serverId;			// included in each client message so the server
												// can tell if it is for a prior map_restart
	// big stuff at end of structure so most offsets are 15 bits or less
	clSnapshot_t	snapshots[PACKET_BACKUP];

	entityState_t	entityBaselines[MAX_GENTITIES];	// for delta compression when not in previous frame

	entityState_t	parseEntities[MAX_PARSE_ENTITIES];
} clientActive_t;

EXTERNC	clientActive_t		cl;

/*
=============================================================================

the clientConnection_t structure is wiped when disconnecting from a server,
either to go to a full screen console, play a demo, or connect to a different server

A connection can be to either a server through the network layer or a
demo through a file.

=============================================================================
*/

#define MAX_TIMEDEMO_DURATIONS	4096

typedef struct {

	connstate_t	state;				// connection status

	int			clientNum;
	int			lastPacketSentTime;			// for retransmits during connection
	int			lastPacketTime;				// for timeouts

	char		servername[MAX_OSPATH];		// name of server from original connect (used by reconnect)
	netadr_t	serverAddress;
	int			connectTime;				// for connection retransmits
	int			connectPacketCount;			// for display on connection dialog
	char		serverMessage[MAX_STRING_TOKENS];	// for display on connection dialog

	int			challenge;					// from the server to use for connecting
	int			checksumFeed;				// from the server for checksum calculations

	// these are our reliable messages that go to the server
	int			reliableSequence;
	int			reliableAcknowledge;		// the last one the server has executed
	char		reliableCommands[MAX_RELIABLE_COMMANDS][MAX_STRING_CHARS];

	// server message (unreliable) and command (reliable) sequence
	// numbers are NOT cleared at level changes, but continue to
	// increase as long as the connection is valid

	// message sequence is used by both the network layer and the
	// delta compression layer
	int			serverMessageSequence;

	// reliable messages received from server
	int			serverCommandSequence;
	int			lastExecutedServerCommand;		// last server command grabbed or executed with CL_GetServerCommand
	char		serverCommands[MAX_RELIABLE_COMMANDS][MAX_STRING_CHARS];

	// file transfer from server
	fileHandle_t download;
	char		downloadTempName[MAX_OSPATH];
	char		downloadName[MAX_OSPATH];
	int		sv_allowDownload;
	char		sv_dlURL[MAX_CVAR_VALUE_STRING];
	int			downloadNumber;
	int			downloadBlock;	// block we are waiting for
	int			downloadCount;	// how many bytes we got
	int			downloadSize;	// how many bytes we got
	char		downloadList[MAX_INFO_STRING]; // list of paks we need to download
	qboolean	downloadRestart;	// if true, we need to do another FS_Restart because we downloaded a pak

	// demo information
	char		demoName[MAX_QPATH];
	qboolean	spDemoRecording;
	qboolean	demorecording;
	qboolean	demoplaying;
	qboolean	demowaiting;	// don't record until a non-delta message is received
	qboolean	firstDemoFrameSkipped;
	fileHandle_t	demofile;

	int			timeDemoFrames;		// counter of rendered frames
	int			timeDemoStart;		// cls.realtime before first frame
	int			timeDemoBaseTime;	// each frame will be at this time + frameNum * 50
	int			timeDemoLastFrame;// time the last frame was rendered
	int			timeDemoMinDuration;	// minimum frame duration
	int			timeDemoMaxDuration;	// maximum frame duration
	unsigned char	timeDemoDurations[ MAX_TIMEDEMO_DURATIONS ];	// log of frame durations

	float		aviVideoFrameRemainder;
	float		aviSoundFrameRemainder;
	
#ifdef LEGACY_PROTOCOL
	qboolean compat;
#endif

	// big stuff at end of structure so most offsets are 15 bits or less
	netchan_t	netchan;
} clientConnection_t;

extern	clientConnection_t clc;

/*
==================================================================

the clientStatic_t structure is never wiped, and is used even when
no client connection is active at all
(except when CL_Shutdown is called)

==================================================================
*/


typedef struct {
	netadr_t	adr;
	char	  	hostName[MAX_NAME_LENGTH];
	char	  	mapName[MAX_NAME_LENGTH];
	char	  	game[MAX_NAME_LENGTH];
	int			netType;
	int			gameType;
	int		  	clients;
	int		  	maxClients;
	int			minPing;
	int			maxPing;
	int			ping;
	qboolean	visible;
	int			punkbuster;
	int			g_humanplayers;
	int			g_needpass;
} serverInfo_t;

typedef struct {
	// when the server clears the hunk, all of these must be restarted
	qboolean	rendererStarted;
	qboolean	soundStarted;
	qboolean	soundRegistered;
	qboolean	cgameStarted;

	int			framecount;
	int			frametime;			// msec since last frame

	int			realtime;			// ignores pause
	int			realFrametime;		// ignoring pause, so console always works

	int pingUpdateSource;		// source currently pinging or updating

	netadr_t	authorizeServer;

	// rendering info
	glconfig_t	glconfig;
	qhandle_t	charSetShader;
	qhandle_t	whiteShader;
	qhandle_t	consoleShader;
} clientStatic_t;

EXTERNC	clientStatic_t		cls;

EXTERNC	char		cl_oldGame[MAX_QPATH];
EXTERNC	qboolean	cl_oldGameSet;

EXTERNC	cvar_t	*cl_nodelta;
EXTERNC	cvar_t	*cl_debugMove;
EXTERNC	cvar_t	*cl_noprint;
EXTERNC	cvar_t	*cl_timegraph;
EXTERNC	cvar_t	*cl_maxpackets;
EXTERNC	cvar_t	*cl_packetdup;
EXTERNC	cvar_t	*cl_shownet;
EXTERNC	cvar_t	*cl_showSend;
EXTERNC	cvar_t	*cl_timeNudge;
EXTERNC	cvar_t	*cl_showTimeDelta;
EXTERNC	cvar_t	*cl_freezeDemo;
EXTERNC	cvar_t	*cl_yawspeed;
EXTERNC	cvar_t	*cl_pitchspeed;
EXTERNC	cvar_t	*cl_run;
EXTERNC	cvar_t	*cl_anglespeedkey;
EXTERNC	cvar_t	*cl_sensitivity;
EXTERNC	cvar_t	*cl_freelook;
EXTERNC	cvar_t	*cl_mouseAccel;
EXTERNC	cvar_t	*cl_mouseAccelOffset;
EXTERNC	cvar_t	*cl_mouseAccelStyle;
EXTERNC	cvar_t	*cl_showMouseRate;
EXTERNC	cvar_t	*m_pitch;
EXTERNC	cvar_t	*m_yaw;
EXTERNC	cvar_t	*m_forward;
EXTERNC	cvar_t	*m_side;
EXTERNC	cvar_t	*m_filter;
EXTERNC	cvar_t	*j_pitch;
EXTERNC	cvar_t	*j_yaw;
EXTERNC	cvar_t	*j_forward;
EXTERNC	cvar_t	*j_side;
EXTERNC	cvar_t	*j_up;
EXTERNC	cvar_t	*j_pitch_axis;
EXTERNC	cvar_t	*j_yaw_axis;
EXTERNC	cvar_t	*j_forward_axis;
EXTERNC	cvar_t	*j_side_axis;
EXTERNC	cvar_t	*j_up_axis;
EXTERNC	cvar_t	*cl_timedemo;
EXTERNC	cvar_t	*cl_aviFrameRate;
EXTERNC	cvar_t	*cl_aviMotionJpeg;
EXTERNC	cvar_t	*cl_activeAction;
EXTERNC	cvar_t	*cl_allowDownload;
EXTERNC  cvar_t  *cl_downloadMethod;
EXTERNC	cvar_t	*cl_conXOffset;
EXTERNC	cvar_t	*cl_inGameVideo;
EXTERNC	cvar_t	*cl_lanForcePackets;
EXTERNC	cvar_t	*cl_autoRecordDemo;
EXTERNC	cvar_t	*cl_consoleKeys;



//=================================================

//
// cl_main
//

CCALL void CL_Init (void);
CCALL void CL_AddReliableCommand(const char *cmd, qboolean isDisconnectCmd);
CCALL void CL_StartHunkUsers( qboolean rendererOnly );
CCALL void CL_Disconnect_f (void);
CCALL void CL_Snd_Restart_f (void);
CCALL void CL_ReadDemoMessage( void );
CCALL void CL_InitDownloads(void);
CCALL void CL_NextDownload(void);
CCALL void CL_InitRef( void );
CCALL qboolean CL_CheckPaused(void);

//
// cl_input
//
typedef struct {
	int			down[2];		// key nums holding it down
	unsigned	downtime;		// msec timestamp
	unsigned	msec;			// msec down this frame if both a down and up happened
	qboolean	active;			// current state
	qboolean	wasPressed;		// set when down, not cleared when up
} kbutton_t;

CCALL void CL_InitInput(void);
CCALL void CL_SendCmd (void);
CCALL void CL_ClearState (void);
CCALL void CL_WritePacket( void );
CCALL void IN_CenterView (void);
CCALL float CL_KeyState (kbutton_t *key);
CCALL int Key_StringToKeynum( char *str );
CCALL char *Key_KeynumToString (int keynum);

//
// cl_parse.c
//
EXTERNC int cl_connectedToPureServer;
EXTERNC int cl_connectedToCheatServer;

CCALL void CL_SystemInfoChanged( void );
CCALL void CL_ParseServerMessage( msg_t *msg );

//
// cl_scrn.c
//
CCALL void	SCR_Init (void);
CCALL void	SCR_UpdateScreen (void);
CCALL void	SCR_DebugGraph (float value);
CCALL int		SCR_GetBigStringWidth( const char *str );	// returns in virtual 640x480 coordinates
CCALL void	SCR_AdjustFrom640( float *x, float *y, float *w, float *h );
CCALL void	SCR_FillRect( float x, float y, float width, float height, const float *color );
CCALL void	SCR_DrawPic( float x, float y, float width, float height, qhandle_t hShader );
CCALL void	SCR_DrawNamedPic( float x, float y, float width, float height, const char *picname );
CCALL void	SCR_DrawBigString( int x, int y, const char *s, float alpha, qboolean noColorEscape );			// draws a string with embedded color control characters with fade
CCALL void	SCR_DrawBigStringColor( int x, int y, const char *s, vec4_t color, qboolean noColorEscape );	// ignores embedded color control characters
CCALL void	SCR_DrawSmallStringExt( int x, int y, const char *string, float *setColor, qboolean forceColor, qboolean noColorEscape );
CCALL void	SCR_DrawSmallChar( int x, int y, int ch );

//
// cl_cgame.c
//
CCALL void CL_InitCGame( void );
CCALL qboolean CL_GameCommand( void );
CCALL void CL_CGameRendering();
CCALL void CL_SetCGameTime( void );
CCALL void CL_FirstSnapshot( void );

//
// cl_ui.c
//
CCALL int Key_GetCatcher( void );
CCALL void Key_SetCatcher( int catcher );


//
// cl_net_chan.c
//
CCALL void CL_Netchan_Transmit( netchan_t *chan, msg_t* msg);	//int length, const byte *data );
CCALL qboolean CL_Netchan_Process( netchan_t *chan, msg_t *msg );

#endif