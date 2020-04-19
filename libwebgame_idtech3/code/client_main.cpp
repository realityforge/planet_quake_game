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
// cl_main.c  -- client main loop

#include "client.h"
#include <limits.h>
#include <client_render.h>

cvar_t	*cl_nodelta;
cvar_t	*cl_debugMove;
cvar_t	*cl_noprint;
cvar_t	*rcon_client_password;
cvar_t	*rconAddress;
cvar_t	*cl_timeout;
cvar_t	*cl_maxpackets;
cvar_t	*cl_packetdup;
cvar_t	*cl_timeNudge;
cvar_t	*cl_showTimeDelta;
cvar_t	*cl_freezeDemo;
cvar_t	*cl_shownet;
cvar_t	*cl_showSend;
cvar_t	*cl_timedemo;
cvar_t	*cl_timedemoLog;
cvar_t	*cl_autoRecordDemo;
cvar_t	*cl_freelook;
cvar_t	*cl_sensitivity;
cvar_t	*cl_mouseAccel;
cvar_t	*cl_mouseAccelOffset;
cvar_t	*cl_mouseAccelStyle;
cvar_t	*cl_showMouseRate;
cvar_t	*m_pitch;
cvar_t	*m_yaw;
cvar_t	*m_forward;
cvar_t	*m_side;
cvar_t	*m_filter;
cvar_t	*j_pitch;
cvar_t	*j_yaw;
cvar_t	*j_forward;
cvar_t	*j_side;
cvar_t	*j_up;
cvar_t	*j_pitch_axis;
cvar_t	*j_yaw_axis;
cvar_t	*j_forward_axis;
cvar_t	*j_side_axis;
cvar_t	*j_up_axis;
cvar_t	*cl_activeAction;
cvar_t	*cl_motdString;
cvar_t	*cl_allowDownload;
cvar_t	*cl_conXOffset;
cvar_t	*cl_inGameVideo;
cvar_t	*cl_serverStatusResendTime;
cvar_t	*cl_lanForcePackets;
cvar_t	*cl_guidServerUniq;
cvar_t	*cl_consoleKeys;
cvar_t	*cl_rate;

clientActive_t		cl;
clientConnection_t	clc;
clientStatic_t		cls;

char				cl_reconnectArgs[MAX_OSPATH];
char				cl_oldGame[MAX_QPATH];
qboolean			cl_oldGameSet;

static int noGameRestart = qfalse;

void CL_CheckForResend( void );


/*
=======================================================================

CLIENT RELIABLE COMMAND COMMUNICATION

=======================================================================
*/

/*
======================
CL_AddReliableCommand

The given command will be transmitted to the server, and is gauranteed to
not have future usercmd_t executed before it is executed
======================
*/
void CL_AddReliableCommand(const char *cmd, qboolean isDisconnectCmd)
{
	int unacknowledged = clc.reliableSequence - clc.reliableAcknowledge;
	
	// if we would be losing an old command that hasn't been acknowledged,
	// we must drop the connection
	// also leave one slot open for the disconnect command in this case.
	
	if ((isDisconnectCmd && unacknowledged > MAX_RELIABLE_COMMANDS) ||
	    (!isDisconnectCmd && unacknowledged >= MAX_RELIABLE_COMMANDS))
	{
		if(com_errorEntered)
			return;
		else
			Com_Error(ERR_DROP, "Client command overflow");
	}

	Q_strncpyz(clc.reliableCommands[++clc.reliableSequence & (MAX_RELIABLE_COMMANDS - 1)],
		   cmd, sizeof(*clc.reliableCommands));
}

/*
=======================================================================

CLIENT SIDE DEMO PLAYBACK

=======================================================================
*/

/*
=================
CL_DemoFrameDurationSDev
=================
*/
static float CL_DemoFrameDurationSDev( void )
{
	int i;
	int numFrames;
	float mean = 0.0f;
	float variance = 0.0f;

	if( ( clc.timeDemoFrames - 1 ) > MAX_TIMEDEMO_DURATIONS )
		numFrames = MAX_TIMEDEMO_DURATIONS;
	else
		numFrames = clc.timeDemoFrames - 1;

	for( i = 0; i < numFrames; i++ )
		mean += clc.timeDemoDurations[ i ];
	mean /= numFrames;

	for( i = 0; i < numFrames; i++ )
	{
		float x = clc.timeDemoDurations[ i ];

		variance += ( ( x - mean ) * ( x - mean ) );
	}
	variance /= numFrames;

	return sqrt( variance );
}

void CL_DemoCompleted( void )
{}

/*
=================
CL_ReadDemoMessage
=================
*/
void CL_ReadDemoMessage( void ) {
#if 0
	int			r;
	msg_t		buf;
	byte		bufData[ MAX_MSGLEN ];
	int			s;

	if ( !clc.demofile ) {
		CL_DemoCompleted ();
		return;
	}

	// get the sequence number
	r = FS_Read( &s, 4, clc.demofile);
	if ( r != 4 ) {
		CL_DemoCompleted ();
		return;
	}
	clc.serverMessageSequence = LittleLong( s );

	// init the message
	MSG_Init( &buf, bufData, sizeof( bufData ) );

	// get the length
	r = FS_Read (&buf.cursize, 4, clc.demofile);
	if ( r != 4 ) {
		CL_DemoCompleted ();
		return;
	}
	buf.cursize = LittleLong( buf.cursize );
	if ( buf.cursize == -1 ) {
		CL_DemoCompleted ();
		return;
	}
	if ( buf.cursize > buf.maxsize ) {
		Com_Error (ERR_DROP, "CL_ReadDemoMessage: demoMsglen > MAX_MSGLEN");
	}
	r = FS_Read( buf.data, buf.cursize, clc.demofile );
	if ( r != buf.cursize ) {
		Com_Printf( "Demo file was truncated.\n");
		CL_DemoCompleted ();
		return;
	}

	clc.lastPacketTime = cls.realtime;
	buf.readcount = 0;
	CL_ParseServerMessage( &buf );
#endif
}

/*
====================
CL_WalkDemoExt
====================
*/
static int CL_WalkDemoExt(char *arg, char *name, int *demofile)
{

	
	return -1;
}

void CL_ShutdownAll(qboolean shutdownRef)
{
	S_DisableSounds();
	cls.cgameStarted = qfalse;
	cls.rendererStarted = qfalse;
	cls.soundRegistered = qfalse;
}

/*
=================
CL_ClearMemory

Called by Com_GameRestart
=================
*/
void CL_ClearMemory(qboolean shutdownRef)
{
	// shutdown all the client stuff
	CL_ShutdownAll(shutdownRef);

	// if not running a server clear the whole hunk
	if ( !com_sv_running->integer ) {
		// clear the whole hunk
		Hunk_Clear();
		// clear collision map data
		CM_ClearMap();
	}
	else {
		// clear all the client data on the hunk
		Hunk_ClearToMark();
	}
}

/*
=================
CL_FlushMemory

Called by CL_MapLoading, CL_Connect_f, CL_PlayDemo_f, and CL_ParseGamestate the only
ways a client gets into a game
Also called by Com_Error
=================
*/
void CL_FlushMemory(void)
{
	CL_ClearMemory(qfalse);
	CL_StartHunkUsers(qfalse);
}

/*
=====================
CL_MapLoading

A local server is starting to load a map, so update the
screen to let the user know about it, then dump all client
memory on the hunk from cgame, ui, and renderer
=====================
*/
void CL_MapLoading( void ) {
	if ( com_dedicated->integer ) {
		clc.state = CA_DISCONNECTED;
		Key_SetCatcher( KEYCATCH_CONSOLE );
		return;
	}

	if ( !com_cl_running->integer ) {
		return;
	}
	
	Key_SetCatcher( 0 );

	// if we are already connected to the local host, stay connected
	if ( clc.state >= CA_CONNECTED && !Q_stricmp( clc.servername, "localhost" ) ) {
		clc.state = CA_CONNECTED;		// so the connect screen is drawn
		Com_Memset( clc.serverMessage, 0, sizeof( clc.serverMessage ) );
		Com_Memset( &cl.gameState, 0, sizeof( cl.gameState ) );
		clc.lastPacketSentTime = -9999;
		SCR_UpdateScreen();
	} else {
		// clear nextmap so the cinematic shutdown doesn't execute it
		Cvar_Set( "nextmap", "" );
		//Com_Printf("CL_Disconnect( qtrue );\n");
		CL_Disconnect( qtrue );
		//Com_Printf("after CL_Disconnect( qtrue );\n");
		Q_strncpyz( clc.servername, "localhost", sizeof(clc.servername) );
		clc.state = CA_CHALLENGING;		// so the connect screen is drawn
		Key_SetCatcher( 0 );
		SCR_UpdateScreen();
		clc.connectTime = -RETRANSMIT_TIMEOUT;
		NET_StringToAdr( clc.servername, &clc.serverAddress, NA_UNSPEC);
		// we don't need a challenge on the localhost

		//Com_Printf("before CL_CheckForResend\n");
		CL_CheckForResend();
		//Com_Printf("after CL_CheckForResend\n");
	}
}

/*
=====================
CL_ClearState

Called before parsing a gamestate
=====================
*/
void CL_ClearState (void) {

//	S_StopAllSounds();

	Com_Memset( &cl, 0, sizeof( cl ) );
}

static void CL_OldGame(void)
{

}

/*
=====================
CL_Disconnect

Called when a connection, demo, or cinematic is being terminated.
Goes from a connected state to either a menu state or a console state
Sends a disconnect message to the server
This is also called on Com_Error and Com_Quit, so it shouldn't cause any errors
=====================
*/
void CL_Disconnect( qboolean showMainMenu ) {
	if ( !com_cl_running || !com_cl_running->integer ) {
		return;
	}



	*clc.downloadTempName = *clc.downloadName = 0;
	Cvar_Set( "cl_downloadName", "" );

	//if ( clc.demofile ) {
	//	FS_FCloseFile( clc.demofile );
	//	clc.demofile = 0;
	//}

	S_ClearSoundBuffer();

	// send a disconnect message to the server
	// send it a few times in case one is dropped
	if ( clc.state >= CA_CONNECTED ) {
		CL_AddReliableCommand("disconnect", qtrue);
		CL_WritePacket();
		CL_WritePacket();
		CL_WritePacket();
	}
	
	
	CL_ClearState ();

	// wipe the client connection
	Com_Memset( &clc, 0, sizeof( clc ) );

	clc.state = CA_DISCONNECTED;

	// allow cheats locally
	Cvar_Set( "sv_cheats", "1" );

	// not connected to a pure server anymore
	cl_connectedToPureServer = qfalse;

	if(!noGameRestart)
		CL_OldGame();
	else
		noGameRestart = qfalse;
}


/*
===================
CL_ForwardCommandToServer

adds the current command line as a clientCommand
things like godmode, noclip, etc, are commands directed to the server,
so when they are typed in at the console, they will need to be forwarded.
===================
*/
void CL_ForwardCommandToServer( const char *string ) {
	char	*cmd;

	cmd = Cmd_Argv(0);

	// ignore key up commands
	if ( cmd[0] == '-' ) {
		return;
	}

	if ( clc.demoplaying || clc.state < CA_CONNECTED || cmd[0] == '+' ) {
		Com_Printf ("Unknown command \"%s" S_COLOR_WHITE "\"\n", cmd);
		return;
	}

	if ( Cmd_Argc() > 1 ) {
		CL_AddReliableCommand(string, qfalse);
	} else {
		CL_AddReliableCommand(cmd, qfalse);
	}
}

/*
======================================================================

CONSOLE COMMANDS

======================================================================
*/

/*
==================
CL_ForwardToServer_f
==================
*/
void CL_ForwardToServer_f( void ) {
	if ( clc.state != CA_ACTIVE || clc.demoplaying ) {
		Com_Printf ("Not connected to a server.\n");
		return;
	}
	
	// don't forward the first argument
	if ( Cmd_Argc() > 1 ) {
		CL_AddReliableCommand(Cmd_Args(), qfalse);
	}
}

/*
==================
CL_Disconnect_f
==================
*/
void CL_Disconnect_f( void ) {
	Cvar_Set("ui_singlePlayerActive", "0");
	if ( clc.state != CA_DISCONNECTED) {
		//Com_Error (ERR_DISCONNECT, "Disconnected from server");
	}
}


/*
================
CL_Reconnect_f

================
*/
void CL_Reconnect_f( void ) {
	if ( !strlen( cl_reconnectArgs ) )
		return;
	Cvar_Set("ui_singlePlayerActive", "0");
	Cbuf_AddText( va("connect %s\n", cl_reconnectArgs ) );
}

/*
================
CL_Connect_f

================
*/
void CL_Connect_f( void ) {
	char	*server;
	const char	*serverString;
	int argc = Cmd_Argc();
	netadrtype_t family = NA_UNSPEC;

	if ( argc != 2 && argc != 3 ) {
		Com_Printf( "usage: connect [-4|-6] server\n");
		return;	
	}
	
	if(argc == 2)
		server = Cmd_Argv(1);
	else
	{
		if(!strcmp(Cmd_Argv(1), "-4"))
			family = NA_IP;
		else if(!strcmp(Cmd_Argv(1), "-6"))
			family = NA_IP6;
		else
			Com_Printf( "warning: only -4 or -6 as address type understood.\n");
		
		server = Cmd_Argv(2);
	}

	// save arguments for reconnect
	Q_strncpyz( cl_reconnectArgs, Cmd_Args(), sizeof( cl_reconnectArgs ) );

	Cvar_Set("ui_singlePlayerActive", "0");

	// clear any previous "server full" type messages
	clc.serverMessage[0] = 0;

	if ( com_sv_running->integer && !strcmp( server, "localhost" ) ) {
		// if running a local server, kill it
		SV_Shutdown( "Server quit" );
	}

	// make sure a local server is killed
	Cvar_Set( "sv_killserver", "1" );
	SV_Frame( 0 );

	noGameRestart = qtrue;
	CL_Disconnect( qtrue );

	Q_strncpyz( clc.servername, server, sizeof(clc.servername) );

	if (!NET_StringToAdr(clc.servername, &clc.serverAddress, family) ) {
		Com_Printf ("Bad server address\n");
		clc.state = CA_DISCONNECTED;
		return;
	}
	if (clc.serverAddress.port == 0) {
		clc.serverAddress.port = BigShort( PORT_SERVER );
	}

	serverString = NET_AdrToStringwPort(clc.serverAddress);

	Com_Printf( "%s resolved to %s\n", clc.servername, serverString);

	// if we aren't playing on a lan, we need to authenticate
	// with the cd key
	if(NET_IsLocalAddress(clc.serverAddress))
		clc.state = CA_CHALLENGING;
	else
	{
		clc.state = CA_CONNECTING;
		
		// Set a client challenge number that ideally is mirrored back by the server.
		clc.challenge = ((rand() << 16) ^ rand()) ^ Com_Milliseconds();
	}

	Key_SetCatcher( 0 );
	clc.connectTime = -99999;	// CL_CheckForResend() will fire immediately
	clc.connectPacketCount = 0;

	// server connection string
	Cvar_Set( "cl_currentServerAddress", server );
}

#define MAX_RCON_MESSAGE 1024

void CL_SendPureChecksums( void ) {
	char cMsg[MAX_INFO_VALUE];

	// if we are pure we need to send back a command with our referenced pk3 checksums
	Com_sprintf(cMsg, sizeof(cMsg), "cp %d %s", cl.serverId, "asd");

	CL_AddReliableCommand(cMsg, qfalse);
}

void CL_ResetPureClientAtServer( void ) {
	CL_AddReliableCommand("vdr", qfalse);
}


/*
=================
CL_Snd_Restart

Restart the sound subsystem
=================
*/
void CL_Snd_Shutdown(void)
{
	S_Shutdown();
	cls.soundStarted = qfalse;
}

/*
=================
CL_Snd_Restart_f

Restart the sound subsystem
The cgame and game must also be forced to restart because
handles will be invalid
=================
*/
void CL_Snd_Restart_f(void)
{
	CL_Snd_Shutdown();
}


/*
==================
CL_PK3List_f
==================
*/
void CL_OpenedPK3List_f( void ) {
}

/*
==================
CL_PureList_f
==================
*/
void CL_ReferencedPK3List_f( void ) {
}

/*
==================
CL_Configstrings_f
==================
*/
void CL_Configstrings_f( void ) {
	int		i;
	int		ofs;

	if ( clc.state != CA_ACTIVE ) {
		Com_Printf( "Not connected to a server.\n");
		return;
	}

	for ( i = 0 ; i < MAX_CONFIGSTRINGS ; i++ ) {
		ofs = cl.gameState.stringOffsets[ i ];
		if ( !ofs ) {
			continue;
		}
		Com_Printf( "%4i: %s\n", i, cl.gameState.stringData + ofs );
	}
}

/*
==============
CL_Clientinfo_f
==============
*/
void CL_Clientinfo_f( void ) {
	Com_Printf( "--------- Client Information ---------\n" );
	Com_Printf( "state: %i\n", clc.state );
	Com_Printf( "Server: %s\n", clc.servername );
	Com_Printf ("User info settings:\n");
	Info_Print( Cvar_InfoString( CVAR_USERINFO ) );
	Com_Printf( "--------------------------------------\n" );
}


//====================================================================

/*
=================
CL_DownloadsComplete

Called when all downloading has been completed
=================
*/
void CL_DownloadsComplete( void ) {
	// if we downloaded files we need to restart the file system
	if (clc.downloadRestart) {
		clc.downloadRestart = qfalse;


		// inform the server so we get new gamestate info
		CL_AddReliableCommand("donedl", qfalse);

		// by sending the donedl command we request a new gamestate
		// so we don't want to load stuff yet
		return;
	}

	// let the client game init and load data
	clc.state = CA_LOADING;

	// Pump the loop, this may change gamestate!
	Com_EventLoop();

	// if the gamestate was changed by calling Com_EventLoop
	// then we loaded everything already and we don't want to do it again.
	if ( clc.state != CA_LOADING ) {
		return;
	}

	// starting to load a map so we get out of full screen ui mode
	Cvar_Set("r_uiFullScreen", "0");

	// flush client memory and start loading stuff
	// this will also (re)load the UI
	// if this is a local client then only the client part of the hunk
	// will be cleared, note that this is done after the hunk mark has been set
	CL_FlushMemory();

	// initialize the CGame
	cls.cgameStarted = qtrue;
	CL_InitCGame();

	// set pure checksums
	CL_SendPureChecksums();

	CL_WritePacket();
	CL_WritePacket();
	CL_WritePacket();
}

/*
=================
CL_BeginDownload

Requests a file to download from the server.  Stores it in the current
game directory.
=================
*/
void CL_BeginDownload( const char *localName, const char *remoteName ) {

	Com_DPrintf("***** CL_BeginDownload *****\n"
				"Localname: %s\n"
				"Remotename: %s\n"
				"****************************\n", localName, remoteName);

	Q_strncpyz ( clc.downloadName, localName, sizeof(clc.downloadName) );
	Com_sprintf( clc.downloadTempName, sizeof(clc.downloadTempName), "%s.tmp", localName );

	// Set so UI gets access to it
	Cvar_Set( "cl_downloadName", remoteName );
	Cvar_Set( "cl_downloadSize", "0" );
	Cvar_Set( "cl_downloadCount", "0" );
	Cvar_SetValue( "cl_downloadTime", cls.realtime );

	clc.downloadBlock = 0; // Starting new file
	clc.downloadCount = 0;

	CL_AddReliableCommand(va("download %s", remoteName), qfalse);
}

/*
=================
CL_NextDownload

A download completed or failed
=================
*/
void CL_NextDownload(void)
{

}

/*
=================
CL_InitDownloads

After receiving a valid game state, we valid the cgame and local zip files here
and determine if we need to download them
=================
*/
void CL_InitDownloads(void) {
  char missingfiles[1024];

#if 0
  if ( !(cl_allowDownload->integer & DLF_ENABLE) )
  {
    // autodownload is disabled on the client
    // but it's possible that some referenced files on the server are missing
    if (FS_ComparePaks( missingfiles, sizeof( missingfiles ), qfalse ) )
    {      
      // NOTE TTimo I would rather have that printed as a modal message box
      //   but at this point while joining the game we don't know wether we will successfully join or not
      Com_Printf( "\nWARNING: You are missing some files referenced by the server:\n%s"
                  "You might not be able to join the game\n"
                  "Go to the setting menu to turn on autodownload, or get the file elsewhere\n\n", missingfiles );
    }
  }
  else if ( FS_ComparePaks( clc.downloadList, sizeof( clc.downloadList ) , qtrue ) ) {

    Com_Printf("Need paks: %s\n", clc.downloadList );

		if ( *clc.downloadList ) {
			// if autodownloading is not enabled on the server
			clc.state = CA_CONNECTED;

			*clc.downloadTempName = *clc.downloadName = 0;
			Cvar_Set( "cl_downloadName", "" );

			CL_NextDownload();
			return;
		}

	}
#endif
	CL_DownloadsComplete();
}

/*
=================
CL_CheckForResend

Resend a connect message if the last one has timed out
=================
*/
void CL_CheckForResend( void ) {
	
	int		port;
	char	info[MAX_INFO_STRING];
	char	data[MAX_INFO_STRING + 10];

	// don't send anything if playing back a demo
	if ( clc.demoplaying ) {
		return;
	}

	// resend if we haven't gotten a reply yet
	if ( clc.state != CA_CONNECTING && clc.state != CA_CHALLENGING ) {
		return;
	}

	if ( cls.realtime - clc.connectTime < RETRANSMIT_TIMEOUT ) {
		return;
	}

	clc.connectTime = cls.realtime;	// for retransmit requests
	clc.connectPacketCount++;

	Com_Printf("clc.state=%d\n", clc.state);
	char* ret = NULL;
	switch ( clc.state ) {
	case CA_CONNECTING:
		// The challenge request shall be followed by a client challenge so no malicious server can hijack this connection.
		// Add the gamename so the server knows we're running the correct game or can reject the client
		// with a meaningful message
		Com_sprintf(data, sizeof(data), "getchallenge %d %s", clc.challenge, com_gamename->string);
		
		//Com_Printf("Connecting 1\n");
		NET_OutOfBandPrint(NS_CLIENT, clc.serverAddress, "%s", data);
		//Com_Printf("Connecting 2\n");
		break;
		
	case CA_CHALLENGING:
		//Com_Printf("Chall a\n");
		// sending back the challenge
		port = Cvar_VariableValue ("net_qport");
		
		//Com_Printf("Chall b\n");
		ret = Cvar_InfoString( CVAR_USERINFO );
		Com_Printf("Got: %s\n", ret);
		Q_strncpyz( info, ret, sizeof( info ) );
		
		//Com_Printf("Chall 1\n");

#ifdef LEGACY_PROTOCOL
		if(com_legacyprotocol->integer == com_protocol->integer)
			clc.compat = qtrue;

		if(clc.compat)
			Info_SetValueForKey(info, "protocol", va("%i", com_legacyprotocol->integer));
		else
#endif
			Info_SetValueForKey(info, "protocol", va("%i", com_protocol->integer));
		Info_SetValueForKey( info, "qport", va("%i", port ) );
		Info_SetValueForKey( info, "challenge", va("%i", clc.challenge ) );
		
		//Com_Printf("Chall 2\n");
		Com_sprintf( data, sizeof(data), "connect \"%s\"", info );
		NET_OutOfBandData( NS_CLIENT, clc.serverAddress, (byte *) data, strlen ( data ) );
		//Com_Printf("Chall 3\n");
		// the most current userinfo has been sent, so watch for any
		// newer changes to userinfo variables
		cvar_modifiedFlags &= ~CVAR_USERINFO;
		break;

	default:
		Com_Error( ERR_FATAL, "CL_CheckForResend: bad clc.state" );
	}
}

void CL_InitServerInfo( serverInfo_t *server, netadr_t *address ) {
	server->adr = *address;
	server->clients = 0;
	server->hostName[0] = '\0';
	server->mapName[0] = '\0';
	server->maxClients = 0;
	server->maxPing = 0;
	server->minPing = 0;
	server->ping = -1;
	server->game[0] = '\0';
	server->gameType = 0;
	server->netType = 0;
	server->punkbuster = 0;
	server->g_humanplayers = 0;
	server->g_needpass = 0;
}

#define MAX_SERVERSPERPACKET	256


/*
=================
CL_ConnectionlessPacket

Responses to broadcasts, etc
=================
*/
void CL_ConnectionlessPacket( netadr_t from, msg_t *msg ) {
	char	*s;
	char	*c;
	int challenge = 0;

	MSG_BeginReadingOOB( msg );
	MSG_ReadLong( msg );	// skip the -1

	s = MSG_ReadStringLine( msg );

	Cmd_TokenizeString( s );

	c = Cmd_Argv(0);

	Com_DPrintf ("CL packet %s: %s\n", NET_AdrToStringwPort(from), c);

	// challenge from the server we are connecting to
	if (!Q_stricmp(c, "challengeResponse"))
	{
		char *strver;
		int ver;
	
		if (clc.state != CA_CONNECTING)
		{
			Com_DPrintf("Unwanted challenge response received. Ignored.\n");
			return;
		}
		
		c = Cmd_Argv(2);
		if(*c)
			challenge = atoi(c);

		strver = Cmd_Argv(3);
		if(*strver)
		{
			ver = atoi(strver);
			
			if(ver != com_protocol->integer)
			{
#ifdef LEGACY_PROTOCOL
				if(com_legacyprotocol->integer > 0)
				{
					// Server is ioq3 but has a different protocol than we do.
					// Fall back to idq3 protocol.
					clc.compat = qtrue;

					Com_Printf(S_COLOR_YELLOW "Warning: Server reports protocol version %d, "
						   "we have %d. Trying legacy protocol %d.\n",
						   ver, com_protocol->integer, com_legacyprotocol->integer);
				}
				else
#endif
				{
					Com_Printf(S_COLOR_YELLOW "Warning: Server reports protocol version %d, we have %d. "
						   "Trying anyways.\n", ver, com_protocol->integer);
				}
			}
		}
#ifdef LEGACY_PROTOCOL
		else
			clc.compat = qtrue;
		
		if(clc.compat)
		{
			if(!NET_CompareAdr(from, clc.serverAddress))
			{
				// This challenge response is not coming from the expected address.
				// Check whether we have a matching client challenge to prevent
				// connection hi-jacking.
			
				if(!*c || challenge != clc.challenge)
				{
					Com_DPrintf("Challenge response received from unexpected source. Ignored.\n");
					return;
				}
			}
		}
		else
#endif
		{
			if(!*c || challenge != clc.challenge)
			{
				Com_Printf("Bad challenge for challengeResponse. Ignored.\n");
				return;
			}
		}

		// start sending challenge response instead of challenge request packets
		clc.challenge = atoi(Cmd_Argv(1));
		clc.state = CA_CHALLENGING;
		clc.connectPacketCount = 0;
		clc.connectTime = -99999;

		// take this address as the new server address.  This allows
		// a server proxy to hand off connections to multiple servers
		clc.serverAddress = from;
		Com_DPrintf ("challengeResponse: %d\n", clc.challenge);
		return;
	}

	// server connection
	if ( !Q_stricmp(c, "connectResponse") ) {
		if ( clc.state >= CA_CONNECTED ) {
			Com_Printf ("Dup connect received. Ignored.\n");
			return;
		}
		if ( clc.state != CA_CHALLENGING ) {
			Com_Printf ("connectResponse packet while not connecting. Ignored.\n");
			return;
		}
		if ( !NET_CompareAdr( from, clc.serverAddress ) ) {
			Com_Printf( "connectResponse from wrong address. Ignored.\n" );
			return;
		}

#ifdef LEGACY_PROTOCOL
		if(!clc.compat)
#endif
		{
			c = Cmd_Argv(1);

			if(*c)
				challenge = atoi(c);
			else
			{
				Com_Printf("Bad connectResponse received. Ignored.\n");
				return;
			}
			
			if(challenge != clc.challenge)
			{
				Com_Printf("ConnectResponse with bad challenge received. Ignored.\n");
				return;
			}
		}

#ifdef LEGACY_PROTOCOL
		Netchan_Setup(NS_CLIENT, &clc.netchan, from, Cvar_VariableValue("net_qport"),
			      clc.challenge, clc.compat);
#else
		Netchan_Setup(NS_CLIENT, &clc.netchan, from, Cvar_VariableValue("net_qport"),
			      clc.challenge, qfalse);
#endif

		clc.state = CA_CONNECTED;
		clc.lastPacketSentTime = -9999;		// send first packet immediately
		return;
	}

	//// server responding to an info broadcast
	//if ( !Q_stricmp(c, "infoResponse") ) {
	//	CL_ServerInfoPacket( from, msg );
	//	return;
	//}
	//
	//// server responding to a get playerlist
	//if ( !Q_stricmp(c, "statusResponse") ) {
	//	CL_ServerStatusResponse( from, msg );
	//	return;
	//}

	// echo request from server
	if ( !Q_stricmp(c, "echo") ) {
		NET_OutOfBandPrint( NS_CLIENT, from, "%s", Cmd_Argv(1) );
		return;
	}

	// cd check
	if ( !Q_stricmp(c, "keyAuthorize") ) {
		// we don't use these now, so dump them on the floor
		return;
	}

	// echo request from server
	if(!Q_stricmp(c, "print")){
		s = MSG_ReadString( msg );
		
		Q_strncpyz( clc.serverMessage, s, sizeof( clc.serverMessage ) );
		Com_Printf( "%s", s );

		return;
	}

	//// list of servers sent back by a master server (classic)
	//if ( !Q_strncmp(c, "getserversResponse", 18) ) {
	//	CL_ServersResponsePacket( &from, msg, qfalse );
	//	return;
	//}
	//
	//// list of servers sent back by a master server (extended)
	//if ( !Q_strncmp(c, "getserversExtResponse", 21) ) {
	//	CL_ServersResponsePacket( &from, msg, qtrue );
	//	return;
	//}

	Com_DPrintf ("Unknown connectionless packet command.\n");
}


/*
=================
CL_PacketEvent

A packet has arrived from the main event loop
=================
*/
void CL_PacketEvent( netadr_t from, msg_t *msg ) {
	int		headerBytes;

	clc.lastPacketTime = cls.realtime;

	//Com_Printf("CL_PacketEvent 1\n");

	if ( msg->cursize >= 4 && *(int *)msg->data == -1 ) {
		CL_ConnectionlessPacket( from, msg );
		return;
	}

	//Com_Printf("CL_PacketEvent 2\n");

	if ( clc.state < CA_CONNECTED ) {
		return;		// can't be a valid sequenced packet
	}

	//Com_Printf("CL_PacketEvent 3\n");

	if ( msg->cursize < 4 ) {
		Com_Printf ("%s: Runt packet\n", NET_AdrToStringwPort( from ));
		return;
	}

	//Com_Printf("CL_PacketEvent 4\n");

	//
	// packet from server
	//
	if ( !NET_CompareAdr( from, clc.netchan.remoteAddress ) ) {
		Com_DPrintf ("%s:sequenced packet without connection\n"
			, NET_AdrToStringwPort( from ) );
		// FIXME: send a client disconnect?
		return;
	}

	//Com_Printf("CL_PacketEvent 5\n");

	if (!CL_Netchan_Process( &clc.netchan, msg) ) {
		return;		// out of order, duplicated, etc
	}

	//Com_Printf("CL_PacketEvent 6\n");

	// the header is different lengths for reliable and unreliable messages
	headerBytes = msg->readcount;

	// track the last message received so it can be returned in 
	// client messages, allowing the server to detect a dropped
	// gamestate
	clc.serverMessageSequence = LittleLong( *(int *)msg->data );

	clc.lastPacketTime = cls.realtime;
	CL_ParseServerMessage( msg );
	
	//Com_Printf("gnaaaahhhhhh\n");
	//Com_Printf("gnaaaahhhhhh\n");
	//Com_Printf("gnaaaahhhhhh\n");
	//Com_Printf("gnaaaahhhhhh\n");
	//Com_Printf("CL_PacketEvent 7\n");

	//
	// we don't know if it is ok to save a demo message until
	// after we have parsed the frame
	//
	//if ( clc.demorecording && !clc.demowaiting ) {
	//	Com_Printf("Write fucking demo?\n");
	//	CL_WriteDemoMessage( msg, headerBytes );
	//}
	
	//Com_Printf("CL_PacketEvent 7\n");
	//Com_Printf("CL_PacketEvent 7\n");
	//Com_Printf("CL_PacketEvent 7\n");
}

/*
==================
CL_CheckTimeout

==================
*/
void CL_CheckTimeout( void ) {
	//
	// check timeout
	//
	if ( ( !CL_CheckPaused() || !sv_paused->integer ) 
		&& clc.state >= CA_CONNECTED
	    && cls.realtime - clc.lastPacketTime > cl_timeout->value*1000) {
		if (++cl.timeoutcount > 5) {	// timeoutcount saves debugger
			Com_Printf ("\nServer connection timed out.\n");
			CL_Disconnect( qtrue );
			return;
		}
	} else {
		cl.timeoutcount = 0;
	}
}

/*
==================
CL_CheckPaused
Check whether client has been paused.
==================
*/
qboolean CL_CheckPaused(void)
{
	// if cl_paused->modified is set, the cvar has only been changed in
	// this frame. Keep paused in this frame to ensure the server doesn't
	// lag behind.
	if(cl_paused->integer || cl_paused->modified)
		return qtrue;
	
	return qfalse;
}

//============================================================================

/*
==================
CL_CheckUserinfo

==================
*/
void CL_CheckUserinfo( void ) {
	// don't add reliable commands when not yet connected
	if(clc.state < CA_CONNECTED)
		return;

	// don't overflow the reliable command buffer when paused
	if(CL_CheckPaused())
		return;

	// send a reliable userinfo update if needed
	if(cvar_modifiedFlags & CVAR_USERINFO)
	{
		cvar_modifiedFlags &= ~CVAR_USERINFO;
		CL_AddReliableCommand(va("userinfo \"%s\"", Cvar_InfoString( CVAR_USERINFO ) ), qfalse);
	}
}

/*
==================
CL_Frame

==================
*/
EXTERNC int clientframe;
int clientframe = 0;
void CL_Frame ( int msec ) {

	if ( !com_cl_running->integer ) {
		return;
	}

	clientframe = 1;

	if ( clc.state == CA_DISCONNECTED && !( Key_GetCatcher( ) & KEYCATCH_UI ) && !com_sv_running->integer) {
		// if disconnected, bring up the menu
		S_StopAllSounds();
	}

	// save the msec before checking pause
	cls.realFrametime = msec;

	// decide the simulation time
	cls.frametime = msec;

	cls.realtime += cls.frametime;


	if ( cl_timegraph->integer ) {
		SCR_DebugGraph ( cls.realFrametime * 0.25 );
	}

	// see if we need to update any userinfo
	CL_CheckUserinfo();

	// if we haven't gotten a packet in a long time,
	// drop the connection
	CL_CheckTimeout();
	
	//return;
	// from here shit fucks up
	// send intentions now
	CL_SendCmd();

	// resend a connection request if necessary
	CL_CheckForResend();

	// decide on the serverTime to render
	CL_SetCGameTime();

	// update the screen
	SCR_UpdateScreen();

	// update audio
	S_Update();
	
	cls.framecount++;
	clientframe = 0;
}


//============================================================================

/*
================
CL_RefPrintf

DLL glue
================
*/
static __attribute__ ((format (printf, 2, 3))) void CL_RefPrintf( int print_level, const char *fmt, ...) {
	va_list		argptr;
	char		msg[MAXPRINTMSG];
	
	va_start (argptr,fmt);
	Q_vsnprintf (msg, sizeof(msg), fmt, argptr);
	va_end (argptr);

	if ( print_level == PRINT_ALL ) {
		Com_Printf ("%s", msg);
	} else if ( print_level == PRINT_WARNING ) {
		Com_Printf (S_COLOR_YELLOW "%s", msg);		// yellow
	} else if ( print_level == PRINT_DEVELOPER ) {
		Com_DPrintf (S_COLOR_RED "%s", msg);		// red
	}
}


// so i can call it easily via ffi
CCALL void beginRegistration() {
}

CCALL void CL_InitRenderer() {

}

/*
============================
CL_StartHunkUsers

After the server has cleared the hunk, these will need to be restarted
This is the only place that any of these functions are called from
============================
*/
void CL_StartHunkUsers( qboolean rendererOnly ) {
	if (!com_cl_running) {
		return;
	}

	if ( !com_cl_running->integer ) {
		return;
	}


	if ( rendererOnly ) {
		return;
	}

	if ( !cls.soundStarted ) {
		cls.soundStarted = qtrue;
		S_Init();
	}

	if ( !cls.soundRegistered ) {
		cls.soundRegistered = qtrue;
		S_BeginRegistration();
	}

	if( com_dedicated->integer ) {
		return;
	}
}

int CL_ScaledMilliseconds(void) {
	return Sys_Milliseconds()*com_timescale->value;
}

void CL_InitRef( void ) {

	// unpause so the cgame definately gets a snapshot and renders a frame
	Cvar_Set( "cl_paused", "0" );
}

void CL_SetModel_f( void ) {
	char	*arg;
	char	name[256];

	arg = Cmd_Argv( 1 );
	if (arg[0]) {
		Cvar_Set( "model", arg );
		Cvar_Set( "headmodel", arg );
	} else {
		Cvar_VariableStringBuffer( "model", name, sizeof(name) );
		Com_Printf("model is set to %s\n", name);
	}
}

void CL_Init( void ) {
	Com_Printf( "----- Client Initialization -----\n" );

	if(!com_fullyInitialized)
	{
		CL_ClearState();
		clc.state = CA_DISCONNECTED;	// no longer CA_UNINITIALIZED
		cl_oldGameSet = qfalse;
	}

	cls.realtime = 0;

	CL_InitInput ();

	//
	// register our variables
	//
	cl_noprint = Cvar_Get( "cl_noprint", "0", 0 );
	cl_timeout = Cvar_Get ("cl_timeout", "200", 0);
	cl_timeNudge = Cvar_Get ("cl_timeNudge", "0", CVAR_TEMP );
	cl_shownet = Cvar_Get ("cl_shownet", "0", CVAR_TEMP );
	cl_showSend = Cvar_Get ("cl_showSend", "0", CVAR_TEMP );
	cl_showTimeDelta = Cvar_Get ("cl_showTimeDelta", "0", CVAR_TEMP );
	cl_freezeDemo = Cvar_Get ("cl_freezeDemo", "0", CVAR_TEMP );
	rcon_client_password = Cvar_Get ("rconPassword", "", CVAR_TEMP );
	cl_activeAction = Cvar_Get( "activeAction", "", CVAR_TEMP );
	cl_timedemo = Cvar_Get ("timedemo", "0", 0);
	cl_timedemoLog = Cvar_Get ("cl_timedemoLog", "", CVAR_ARCHIVE);
	cl_autoRecordDemo = Cvar_Get ("cl_autoRecordDemo", "0", CVAR_ARCHIVE);
	rconAddress = Cvar_Get ("rconAddress", "", 0);
	cl_yawspeed = Cvar_Get ("cl_yawspeed", "140", CVAR_ARCHIVE);
	cl_pitchspeed = Cvar_Get ("cl_pitchspeed", "140", CVAR_ARCHIVE);
	cl_anglespeedkey = Cvar_Get ("cl_anglespeedkey", "1.5", 0);
	cl_maxpackets = Cvar_Get ("cl_maxpackets", "30", CVAR_ARCHIVE );
	cl_packetdup = Cvar_Get ("cl_packetdup", "1", CVAR_ARCHIVE );
	cl_run = Cvar_Get ("cl_run", "1", CVAR_ARCHIVE);
	cl_sensitivity = Cvar_Get ("sensitivity", "5", CVAR_ARCHIVE);
	cl_mouseAccel = Cvar_Get ("cl_mouseAccel", "0", CVAR_ARCHIVE);
	cl_freelook = Cvar_Get( "cl_freelook", "1", CVAR_ARCHIVE );

	// 0: legacy mouse acceleration
	// 1: new implementation
	cl_mouseAccelStyle = Cvar_Get( "cl_mouseAccelStyle", "0", CVAR_ARCHIVE );
	// offset for the power function (for style 1, ignored otherwise)
	// this should be set to the max rate value
	cl_mouseAccelOffset = Cvar_Get( "cl_mouseAccelOffset", "5", CVAR_ARCHIVE );
	Cvar_CheckRange(cl_mouseAccelOffset, 0.001f, 50000.0f, qfalse);
	cl_showMouseRate = Cvar_Get ("cl_showmouserate", "0", 0);
	cl_allowDownload = Cvar_Get ("cl_allowDownload", "0", CVAR_ARCHIVE);


	cl_conXOffset = Cvar_Get ("cl_conXOffset", "0", 0);
	cl_inGameVideo = Cvar_Get ("r_inGameVideo", "1", CVAR_ARCHIVE);
	cl_serverStatusResendTime = Cvar_Get ("cl_serverStatusResendTime", "750", 0);

	// init autoswitch so the ui will have it correctly even
	// if the cgame hasn't been started
	Cvar_Get ("cg_autoswitch", "1", CVAR_ARCHIVE);

	m_pitch = Cvar_Get ("m_pitch", "0.022", CVAR_ARCHIVE);
	m_yaw = Cvar_Get ("m_yaw", "0.022", CVAR_ARCHIVE);
	m_forward = Cvar_Get ("m_forward", "0.25", CVAR_ARCHIVE);
	m_side = Cvar_Get ("m_side", "0.25", CVAR_ARCHIVE);
#ifdef __APPLE__
	// Input is jittery on OS X w/o this
	m_filter = Cvar_Get ("m_filter", "1", CVAR_ARCHIVE);
#else
	m_filter = Cvar_Get ("m_filter", "0", CVAR_ARCHIVE);
#endif

	j_pitch =        Cvar_Get ("j_pitch",        "0.022", CVAR_ARCHIVE);
	j_yaw =          Cvar_Get ("j_yaw",          "-0.022", CVAR_ARCHIVE);
	j_forward =      Cvar_Get ("j_forward",      "-0.25", CVAR_ARCHIVE);
	j_side =         Cvar_Get ("j_side",         "0.25", CVAR_ARCHIVE);
	j_up =           Cvar_Get ("j_up",           "0", CVAR_ARCHIVE);

	j_pitch_axis =   Cvar_Get ("j_pitch_axis",   "3", CVAR_ARCHIVE);
	j_yaw_axis =     Cvar_Get ("j_yaw_axis",     "2", CVAR_ARCHIVE);
	j_forward_axis = Cvar_Get ("j_forward_axis", "1", CVAR_ARCHIVE);
	j_side_axis =    Cvar_Get ("j_side_axis",    "0", CVAR_ARCHIVE);
	j_up_axis =      Cvar_Get ("j_up_axis",      "4", CVAR_ARCHIVE);

	Cvar_CheckRange(j_pitch_axis, 0, MAX_JOYSTICK_AXIS-1, qtrue);
	Cvar_CheckRange(j_yaw_axis, 0, MAX_JOYSTICK_AXIS-1, qtrue);
	Cvar_CheckRange(j_forward_axis, 0, MAX_JOYSTICK_AXIS-1, qtrue);
	Cvar_CheckRange(j_side_axis, 0, MAX_JOYSTICK_AXIS-1, qtrue);
	Cvar_CheckRange(j_up_axis, 0, MAX_JOYSTICK_AXIS-1, qtrue);

	cl_motdString = Cvar_Get( "cl_motdString", "", CVAR_ROM );

	Cvar_Get( "cl_maxPing", "800", CVAR_ARCHIVE );

	cl_lanForcePackets = Cvar_Get ("cl_lanForcePackets", "1", CVAR_ARCHIVE);

	cl_guidServerUniq = Cvar_Get ("cl_guidServerUniq", "1", CVAR_ARCHIVE);

	// ~ and `, as keys and characters
	cl_consoleKeys = Cvar_Get( "cl_consoleKeys", "~ ` 0x7e 0x60", CVAR_ARCHIVE);

	// userinfo
	Cvar_Get ("name", "UnnamedPlayer", CVAR_USERINFO | CVAR_ARCHIVE );
	cl_rate = Cvar_Get ("rate", "25000", CVAR_USERINFO | CVAR_ARCHIVE );
	Cvar_Get ("snaps", "20", CVAR_USERINFO | CVAR_ARCHIVE );
	Cvar_Get ("model", "sarge", CVAR_USERINFO | CVAR_ARCHIVE );
	Cvar_Get ("headmodel", "sarge", CVAR_USERINFO | CVAR_ARCHIVE );
	Cvar_Get ("team_model", "james", CVAR_USERINFO | CVAR_ARCHIVE );
	Cvar_Get ("team_headmodel", "*james", CVAR_USERINFO | CVAR_ARCHIVE );
	Cvar_Get ("g_redTeam", "Stroggs", CVAR_SERVERINFO | CVAR_ARCHIVE);
	Cvar_Get ("g_blueTeam", "Pagans", CVAR_SERVERINFO | CVAR_ARCHIVE);
	Cvar_Get ("color1",  "4", CVAR_USERINFO | CVAR_ARCHIVE );
	Cvar_Get ("color2", "5", CVAR_USERINFO | CVAR_ARCHIVE );
	Cvar_Get ("handicap", "100", CVAR_USERINFO | CVAR_ARCHIVE );
	Cvar_Get ("teamtask", "0", CVAR_USERINFO );
	Cvar_Get ("sex", "male", CVAR_USERINFO | CVAR_ARCHIVE );
	Cvar_Get ("cl_anonymous", "0", CVAR_USERINFO | CVAR_ARCHIVE );

	Cvar_Get ("password", "", CVAR_USERINFO);
	Cvar_Get ("cg_predictItems", "1", CVAR_USERINFO | CVAR_ARCHIVE );
	
	// cgame might not be initialized before menu is used
	Cvar_Get ("cg_viewsize", "100", CVAR_ARCHIVE );
	// Make sure cg_stereoSeparation is zero as that variable is deprecated and should not be used anymore.
	Cvar_Get ("cg_stereoSeparation", "0", CVAR_ROM);

	Cmd_AddCommand ("cmd", CL_ForwardToServer_f);
	Cmd_AddCommand ("configstrings", CL_Configstrings_f);
	Cmd_AddCommand ("clientinfo", CL_Clientinfo_f);
	Cmd_AddCommand ("snd_restart", CL_Snd_Restart_f);
	Cmd_AddCommand ("disconnect", CL_Disconnect_f);
	Cmd_AddCommand ("connect", CL_Connect_f);
	Cmd_AddCommand ("reconnect", CL_Reconnect_f);
	Cmd_AddCommand ("fs_openedList", CL_OpenedPK3List_f );
	Cmd_AddCommand ("fs_referencedList", CL_ReferencedPK3List_f );
	Cmd_AddCommand ("model", CL_SetModel_f );
}

void CL_Shutdown(char *finalmsg, qboolean disconnect, qboolean quit)
{
	static qboolean recursive = qfalse;
	
	// check whether the client is running at all.
	if(!(com_cl_running && com_cl_running->integer))
		return;
	
	Com_Printf( "----- Client Shutdown (%s) -----\n", finalmsg );

	if ( recursive ) {
		Com_Printf( "WARNING: Recursive shutdown\n" );
		return;
	}
	recursive = qtrue;

	noGameRestart = quit;

	if(disconnect)
		CL_Disconnect(qtrue);
	
	CL_ClearMemory(qtrue);
	CL_Snd_Shutdown();

	Cvar_Set( "cl_running", "0" );

	recursive = qfalse;

	Com_Memset( &cls, 0, sizeof( cls ) );
	Key_SetCatcher( 0 );

	Com_Printf( "-----------------------\n" );

}



#include <signal.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <float.h>
#include <client.h>

#include <q_shared.h>
#include <qcommon.h>

//typedef enum { false, true } bool;

int sys_timeBase;

unsigned int (*callback_getticks)() = NULL;

CCALL void set_callback_getticks(unsigned int (*cb)()) {
	callback_getticks = cb;
}

CCALL void callback_add(unsigned int (*cb)(int a, int b)) {
	cb(10, 30);
}

int Sys_Milliseconds (void)
{
	int             sys_curtime = 0;	
	static qboolean initialized = qfalse;
	int ticks = 0;
#ifdef EMSCRIPTEN
	ticks = EM_ASM_INT({  
		return _SDL_GetTicks();
	}, 123);
#else
	//ticks = callback_getticks();
	//ticks = SDL_GetTicks();
#endif
	if (!initialized) {
		sys_timeBase = ticks;
		initialized = qtrue;
	}
	sys_curtime = ticks - sys_timeBase;
	//if (callback_getticks)
	//	sys_curtime = callback_getticks() * 10;
	//printf("milliseconds=%d\n", sys_curtime);
	return sys_curtime;
	//return imguidata.global_ticks;
}

static char binaryPath[ MAX_OSPATH ] = { 0 };
static char installPath[ MAX_OSPATH ] = { 0 };

/*
=================
Sys_SetBinaryPath
=================
*/
void Sys_SetBinaryPath(const char *path)
{
	Q_strncpyz(binaryPath, path, sizeof(binaryPath));
}

/*
=================
Sys_BinaryPath
=================
*/
char *Sys_BinaryPath(void)
{
	return binaryPath;
}

/*
=================
Sys_SetDefaultInstallPath
=================
*/
void Sys_SetDefaultInstallPath(const char *path)
{
	Q_strncpyz(installPath, path, sizeof(installPath));
}

void Sys_Print( const char *msg )
{
	printf( msg );
}

CCALL void *c_malloc(size_t bytes) { return malloc(bytes); }
CCALL int c_getchar() { return getchar(); }
CCALL int Sys_MilliSeconds(void) { return clock() * 1000 / CLOCKS_PER_SEC; }



/*
		htmlfile->original = fopen(filename, mode);
		fseek(htmlfile->original, 0, SEEK_END);
		htmlfile->size = ftell(htmlfile->original);
		htmlfile->data = malloc(htmlfile->size);
		fseek(htmlfile->original, 0L, SEEK_SET);
		fread(htmlfile->data, htmlfile->size, 1, htmlfile->original);
		fseek(htmlfile->original, 0L, SEEK_SET);
*/

CCALL void *c_pointer(void *ptr) { return ptr; }
CCALL void *c_memcpy(void *dst, void *src, size_t size) { return memcpy(dst, src, size); }

CCALL void c_set_uint(unsigned char *addr, unsigned char new_val) { *addr=new_val; }

CCALL unsigned char c_get_uint(unsigned char *addr) { return *addr; }
qboolean Com_AddStartupCommands( void );

void GL_SetupFunctionPointers();

cvar_t *com_hunkmegs;

CCALL int q3_main() {

	/*
	_GL_SetupFunctionPointers()
	_Sys_Milliseconds()
	_Com_Init(alloc_string("set com_hunkmegs 32"))
	_CL_Init()
	_CL_InitRef()
	_SCR_Init()
	_Cvar_Set(alloc_string("cl_running"), alloc_string("1"));

	CL_InitRenderer();

	//_GLimp_InitExtraExtensions()
	_CL_StartHunkUsers(false)

	Cmd_ExecuteString(alloc_string("set sv_pure 0"))
	Cmd_ExecuteString(alloc_string("map mp_grass"))
	*/


	
	Sys_Milliseconds();
	//Com_Init("set com_hunkmegs 32");
	Com_Init("set dedicated 0");

	
		com_hunkmegs = Cvar_Get( "com_hunkMegs", "128", CVAR_LATCH | CVAR_ARCHIVE );

#ifndef DEDICATED
	CL_Init();
	CL_InitRef();
	SCR_Init ();
	//Cbuf_Execute ();
	Cvar_Set( "cl_running", "1" );
	Com_Printf( "----- Client Initialization Complete -----\n" );
#endif

	// set com_frameTime so that if a map is started on the
	// command line it will still be able to count on com_frameTime
	// being random enough for a serverid
	com_frameTime = Com_Milliseconds();

	// add + commands from command line
	if ( !Com_AddStartupCommands() ) {
		// if the user didn't give any commands, run default action
		if ( !com_dedicated->integer ) {
			// here was the intro playing
		}
	}

	CL_StartHunkUsers( qfalse );

	com_fullyInitialized = qtrue;
	Com_Printf ("--- Common Initialization Complete ---\n");


	Cmd_ExecuteString("set sv_pure 0");
	Cmd_ExecuteString("set g_speed 200"); // feels way more realistic, but todo: get movement from ET or jedi academy and turn it cod-ish
	return 0;
}

extern glconfig_t glConfig;

CCALL void quake_set_widthheight(int width, int height)   {
	// +1, because otherwise the q3 render has a flickering 1px bottom line after minimizing/maximizing the window (Win10)
	// ok, fuck the +1, seems like emcc/webgl fails in glViewport because of it... out of screen probably not allowed
	glConfig.vidWidth = width + 0;
	glConfig.vidHeight = height + 0;
	glConfig.windowAspect = (float)glConfig.vidWidth / (float)glConfig.vidHeight;

	cgs.glconfig.vidWidth = width;
	cgs.glconfig.vidHeight = height;
	cgs.glconfig.windowAspect = glConfig.windowAspect;
	
	cgs.screenXScale = cgs.glconfig.vidWidth  / 640.0f;
	cgs.screenYScale = cgs.glconfig.vidHeight / 480.0f;

	cls.glconfig.vidWidth = width;
	cls.glconfig.vidHeight = height;
	cls.glconfig.windowAspect = glConfig.windowAspect;

	cg.refdef.width = width;
	cg.refdef.height = height;
}

CCALL int q3_main_dedicated() {

	/*
	_GL_SetupFunctionPointers()
	_Sys_Milliseconds()
	_Com_Init(alloc_string("set com_hunkmegs 32"))
	_CL_Init()
	_CL_InitRef()
	_SCR_Init()
	_Cvar_Set(alloc_string("cl_running"), alloc_string("1"));

	CL_InitRenderer();

	//_GLimp_InitExtraExtensions()
	_CL_StartHunkUsers(false)

	Cmd_ExecuteString(alloc_string("set sv_pure 0"))
	Cmd_ExecuteString(alloc_string("map mp_grass"))
	*/


	
	Sys_Milliseconds();
	//Com_Init("set com_hunkmegs 32");
	Com_Init("set dedicated 2");

	
		com_hunkmegs = Cvar_Get( "com_hunkMegs", "128", CVAR_LATCH | CVAR_ARCHIVE );

#ifndef DEDICATED
	CL_Init();
	CL_InitRef();
	SCR_Init ();
	//Cbuf_Execute ();
	Cvar_Set( "cl_running", "1" );
	Com_Printf( "----- Client Initialization Complete -----\n" );
#endif

	// set com_frameTime so that if a map is started on the
	// command line it will still be able to count on com_frameTime
	// being random enough for a serverid
	com_frameTime = Com_Milliseconds();

	// add + commands from command line
	if ( !Com_AddStartupCommands() ) {
		// if the user didn't give any commands, run default action
		if ( !com_dedicated->integer ) {
			// here was the intro playing
		}
	}

	CL_StartHunkUsers( qfalse );

	com_fullyInitialized = qtrue;
	Com_Printf ("--- Common Initialization Complete ---\n");


	Cmd_ExecuteString("set sv_pure 0");
	Cmd_ExecuteString("set g_speed 200"); // feels way more realistic, but todo: get movement from ET or jedi academy and turn it cod-ish
	return 0;
}

#include <stdio.h>
#include "include_ccall.h"

#if EMSCRIPTEN
int main(int argc, char **argv) {
	EM_ASM({
		//alert(Pointer_stringify($0));
		callback_main();
	}, 123);
	return 345;
}
#endif