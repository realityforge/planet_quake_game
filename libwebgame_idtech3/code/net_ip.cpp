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

#include <q_shared.h>
#include <qcommon.h>

#ifdef _WIN32
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	if WINVER < 0x501
#		ifdef __MINGW32__
			// wspiapi.h isn't available on MinGW, so if it's
			// present it's because the end user has added it
			// and we should look for it in our tree
#			include "wspiapi.h"
#		else
#			include <wspiapi.h>
#		endif
#	else
#		include <ws2spi.h>
#	endif

typedef int socklen_t;
#	ifdef ADDRESS_FAMILY
#		define sa_family_t	ADDRESS_FAMILY
#	else
typedef unsigned short sa_family_t;
#	endif

#	define EAGAIN					WSAEWOULDBLOCK
#	define EADDRNOTAVAIL	WSAEADDRNOTAVAIL
#	define EAFNOSUPPORT		WSAEAFNOSUPPORT
#	define ECONNRESET			WSAECONNRESET
typedef u_long	ioctlarg_t;
#	define socketError		WSAGetLastError( )

static WSADATA	winsockdata;
static qboolean	winsockInitialized = qfalse;

#else

#	if MAC_OS_X_VERSION_MIN_REQUIRED == 1020
		// needed for socklen_t on OSX 10.2
#		define _BSD_SOCKLEN_T_
#	endif

#	include <sys/socket.h>
#	include <errno.h>
#	include <netdb.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#	include <net/if.h>
#	include <sys/ioctl.h>
#	include <sys/types.h>
#	include <sys/time.h>
#	include <unistd.h>
#	if !defined(__sun) && !defined(__sgi)
#		include <ifaddrs.h>
#	endif

#	ifdef __sun
#		include <sys/filio.h>
#	endif

typedef int SOCKET;
#	define INVALID_SOCKET		-1
#	define SOCKET_ERROR			-1
#	define closesocket			close
#	define ioctlsocket			ioctl
typedef int	ioctlarg_t;
#	define socketError			errno

#endif

static int networkingEnabled = 0;
static cvar_t	*net_enabled;
static cvar_t	*net_ip;
static cvar_t	*net_port;
static cvar_t	*net_dropsim;

static struct sockaddr	socksRelayAddr;

static SOCKET	ip_socket = INVALID_SOCKET;

// Keep track of currently joined multicast group.
static struct ipv6_mreq curgroup;
// And the currently bound address.
static struct sockaddr_in6 boundto;

#ifndef IF_NAMESIZE
  #define IF_NAMESIZE 16
#endif


#define	MAX_IPS		32

typedef struct
{
	char ifname[IF_NAMESIZE];
	
	netadrtype_t type;
	sa_family_t family;
	struct sockaddr_storage addr;
	struct sockaddr_storage netmask;
} nip_localaddr_t;

static nip_localaddr_t localIP[MAX_IPS];
static int numIP;


//=============================================================================


/*
====================
NET_ErrorString
====================
*/
char *NET_ErrorString( void ) {
#ifdef _WIN32
	//FIXME: replace with FormatMessage?
	switch( socketError ) {
		case WSAEINTR: return "WSAEINTR";
		case WSAEBADF: return "WSAEBADF";
		case WSAEACCES: return "WSAEACCES";
		case WSAEDISCON: return "WSAEDISCON";
		case WSAEFAULT: return "WSAEFAULT";
		case WSAEINVAL: return "WSAEINVAL";
		case WSAEMFILE: return "WSAEMFILE";
		case WSAEWOULDBLOCK: return "WSAEWOULDBLOCK";
		case WSAEINPROGRESS: return "WSAEINPROGRESS";
		case WSAEALREADY: return "WSAEALREADY";
		case WSAENOTSOCK: return "WSAENOTSOCK";
		case WSAEDESTADDRREQ: return "WSAEDESTADDRREQ";
		case WSAEMSGSIZE: return "WSAEMSGSIZE";
		case WSAEPROTOTYPE: return "WSAEPROTOTYPE";
		case WSAENOPROTOOPT: return "WSAENOPROTOOPT";
		case WSAEPROTONOSUPPORT: return "WSAEPROTONOSUPPORT";
		case WSAESOCKTNOSUPPORT: return "WSAESOCKTNOSUPPORT";
		case WSAEOPNOTSUPP: return "WSAEOPNOTSUPP";
		case WSAEPFNOSUPPORT: return "WSAEPFNOSUPPORT";
		case WSAEAFNOSUPPORT: return "WSAEAFNOSUPPORT";
		case WSAEADDRINUSE: return "WSAEADDRINUSE";
		case WSAEADDRNOTAVAIL: return "WSAEADDRNOTAVAIL";
		case WSAENETDOWN: return "WSAENETDOWN";
		case WSAENETUNREACH: return "WSAENETUNREACH";
		case WSAENETRESET: return "WSAENETRESET";
		case WSAECONNABORTED: return "WSWSAECONNABORTEDAEINTR";
		case WSAECONNRESET: return "WSAECONNRESET";
		case WSAENOBUFS: return "WSAENOBUFS";
		case WSAEISCONN: return "WSAEISCONN";
		case WSAENOTCONN: return "WSAENOTCONN";
		case WSAESHUTDOWN: return "WSAESHUTDOWN";
		case WSAETOOMANYREFS: return "WSAETOOMANYREFS";
		case WSAETIMEDOUT: return "WSAETIMEDOUT";
		case WSAECONNREFUSED: return "WSAECONNREFUSED";
		case WSAELOOP: return "WSAELOOP";
		case WSAENAMETOOLONG: return "WSAENAMETOOLONG";
		case WSAEHOSTDOWN: return "WSAEHOSTDOWN";
		case WSASYSNOTREADY: return "WSASYSNOTREADY";
		case WSAVERNOTSUPPORTED: return "WSAVERNOTSUPPORTED";
		case WSANOTINITIALISED: return "WSANOTINITIALISED";
		case WSAHOST_NOT_FOUND: return "WSAHOST_NOT_FOUND";
		case WSATRY_AGAIN: return "WSATRY_AGAIN";
		case WSANO_RECOVERY: return "WSANO_RECOVERY";
		case WSANO_DATA: return "WSANO_DATA";
		default: return "NO ERROR";
	}
#else
	return strerror(socketError);
#endif
}

static void NetadrToSockadr( netadr_t *a, struct sockaddr *s ) {
	if( a->type == NA_BROADCAST ) {
		((struct sockaddr_in *)s)->sin_family = AF_INET;
		((struct sockaddr_in *)s)->sin_port = a->port;
		((struct sockaddr_in *)s)->sin_addr.s_addr = INADDR_BROADCAST;
	}
	else if( a->type == NA_IP ) {
		((struct sockaddr_in *)s)->sin_family = AF_INET;
		((struct sockaddr_in *)s)->sin_addr.s_addr = *(int *)&a->ip;
		((struct sockaddr_in *)s)->sin_port = a->port;
	}
	else if( a->type == NA_IP6 ) {
		((struct sockaddr_in6 *)s)->sin6_family = AF_INET6;
		((struct sockaddr_in6 *)s)->sin6_addr = * ((struct in6_addr *) &a->ip6);
		((struct sockaddr_in6 *)s)->sin6_port = a->port;
		((struct sockaddr_in6 *)s)->sin6_scope_id = a->scope_id;
	}
	else if(a->type == NA_MULTICAST6)
	{
		((struct sockaddr_in6 *)s)->sin6_family = AF_INET6;
		((struct sockaddr_in6 *)s)->sin6_addr = curgroup.ipv6mr_multiaddr;
		((struct sockaddr_in6 *)s)->sin6_port = a->port;
	}
}


static void SockadrToNetadr( struct sockaddr *s, netadr_t *a ) {
	if (s->sa_family == AF_INET) {
		a->type = NA_IP;
		*(int *)&a->ip = ((struct sockaddr_in *)s)->sin_addr.s_addr;
		a->port = ((struct sockaddr_in *)s)->sin_port;
	}
}


static struct addrinfo *SearchAddrInfo(struct addrinfo *hints, sa_family_t family)
{
	while(hints)
	{
		if(hints->ai_family == family)
			return hints;

		hints = hints->ai_next;
	}
	
	return NULL;
}

/*
=============
Sys_StringToSockaddr
=============
*/
static qboolean Sys_StringToSockaddr(const char *s, struct sockaddr *sadr, int sadr_len, sa_family_t family)
{
	struct addrinfo hints;
	struct addrinfo *res = NULL;
	struct addrinfo *search = NULL;
	struct addrinfo *hintsp;
	int retval;
	
	memset(sadr, '\0', sizeof(*sadr));
	memset(&hints, '\0', sizeof(hints));

	hintsp = &hints;
	hintsp->ai_family = family;
	hintsp->ai_socktype = SOCK_DGRAM;
	
	retval = getaddrinfo(s, NULL, hintsp, &res);

	if(!retval)
	{
		if(family == AF_UNSPEC)
		{

			search = SearchAddrInfo(res, AF_INET);
		}
		else
			search = SearchAddrInfo(res, family);

		if(search)
		{
			if(search->ai_addrlen > sadr_len)
				search->ai_addrlen = sadr_len;
				
			memcpy(sadr, search->ai_addr, search->ai_addrlen);
			freeaddrinfo(res);
			
			return qtrue;
		}
		else
			Com_Printf("Sys_StringToSockaddr: Error resolving %s: No address of required type found.\n", s);
	}
	else
		Com_Printf("Sys_StringToSockaddr: Error resolving %s: %s\n", s, gai_strerror(retval));
	
	if(res)
		freeaddrinfo(res);
	
	return qfalse;
}

static void Sys_SockaddrToString(char *dest, int destlen, struct sockaddr *input)
{
	socklen_t inputlen;

	if (input->sa_family == AF_INET6)
		inputlen = sizeof(struct sockaddr_in6);
	else
		inputlen = sizeof(struct sockaddr_in);

	if(getnameinfo(input, inputlen, dest, destlen, NULL, 0, NI_NUMERICHOST) && destlen > 0)
		*dest = '\0';
}

qboolean Sys_StringToAdr( const char *s, netadr_t *a, netadrtype_t family ) {
	struct sockaddr_storage sadr;
	sa_family_t fam;
	
	switch(family)
	{
		case NA_IP:
			fam = AF_INET;
		break;
		case NA_IP6:
			fam = AF_INET6;
		break;
		default:
			fam = AF_UNSPEC;
		break;
	}
	if( !Sys_StringToSockaddr(s, (struct sockaddr *) &sadr, sizeof(sadr), fam ) ) {
		return qfalse;
	}
	
	SockadrToNetadr( (struct sockaddr *) &sadr, a );
	return qtrue;
}

/*
===================
NET_CompareBaseAdrMask

Compare without port, and up to the bit number given in netmask.
===================
*/
qboolean NET_CompareBaseAdrMask(netadr_t a, netadr_t b, int netmask)
{
	byte cmpmask, *addra, *addrb;
	int curbyte;
	
	if (a.type != b.type)
		return qfalse;

	if (a.type == NA_LOOPBACK)
		return qtrue;

	if(a.type == NA_IP)
	{
		addra = (byte *) &a.ip;
		addrb = (byte *) &b.ip;
		
		if(netmask < 0 || netmask > 32)
			netmask = 32;
	}
	else if(a.type == NA_IP6)
	{
		addra = (byte *) &a.ip6;
		addrb = (byte *) &b.ip6;
		
		if(netmask < 0 || netmask > 128)
			netmask = 128;
	}
	else
	{
		Com_Printf ("NET_CompareBaseAdr: bad address type\n");
		return qfalse;
	}

	curbyte = netmask >> 3;

	if(curbyte && memcmp(addra, addrb, curbyte))
			return qfalse;

	netmask &= 0x07;
	if(netmask)
	{
		cmpmask = (1 << netmask) - 1;
		cmpmask <<= 8 - netmask;

		if((addra[curbyte] & cmpmask) == (addrb[curbyte] & cmpmask))
			return qtrue;
	}
	else
		return qtrue;
	
	return qfalse;
}


/*
===================
NET_CompareBaseAdr

Compares without the port
===================
*/
qboolean NET_CompareBaseAdr (netadr_t a, netadr_t b)
{
	return NET_CompareBaseAdrMask(a, b, -1);
}

const char	*NET_AdrToString (netadr_t a)
{
	static	char	s[NET_ADDRSTRMAXLEN];

	if (a.type == NA_LOOPBACK)
		Com_sprintf (s, sizeof(s), "loopback");
	else if (a.type == NA_BOT)
		Com_sprintf (s, sizeof(s), "bot");
	else if (a.type == NA_IP || a.type == NA_IP6)
	{
		struct sockaddr_storage sadr;
	
		memset(&sadr, 0, sizeof(sadr));
		NetadrToSockadr(&a, (struct sockaddr *) &sadr);
		Sys_SockaddrToString(s, sizeof(s), (struct sockaddr *) &sadr);
	}

	return s;
}

const char	*NET_AdrToStringwPort (netadr_t a)
{
	static	char	s[NET_ADDRSTRMAXLEN];

	if (a.type == NA_LOOPBACK)
		Com_sprintf (s, sizeof(s), "loopback");
	else if (a.type == NA_BOT)
		Com_sprintf (s, sizeof(s), "bot");
	else if(a.type == NA_IP)
		Com_sprintf(s, sizeof(s), "%s:%hu", NET_AdrToString(a), ntohs(a.port));
	else if(a.type == NA_IP6)
		Com_sprintf(s, sizeof(s), "[%s]:%hu", NET_AdrToString(a), ntohs(a.port));

	return s;
}


qboolean	NET_CompareAdr (netadr_t a, netadr_t b)
{
	if(!NET_CompareBaseAdr(a, b))
		return qfalse;
	
	if (a.type == NA_IP || a.type == NA_IP6)
	{
		if (a.port == b.port)
			return qtrue;
	}
	else
		return qtrue;
		
	return qfalse;
}


qboolean	NET_IsLocalAddress( netadr_t adr ) {
	return adr.type == NA_LOOPBACK;
}

//=============================================================================

/*
==================
NET_GetPacket

Receive one packet
==================
*/
qboolean NET_GetPacket(netadr_t *net_from, msg_t *net_message, fd_set *fdr)
{
	int 	ret;
	struct sockaddr_storage from;
	socklen_t	fromlen;
	int		err;
	
	if(ip_socket != INVALID_SOCKET && FD_ISSET(ip_socket, fdr))
	{
		fromlen = sizeof(from);
		ret = recvfrom( ip_socket, (char *)net_message->data, net_message->maxsize, 0, (struct sockaddr *) &from, &fromlen );
		

		

		if (ret == SOCKET_ERROR)
		{
			err = socketError;

			if( err != EAGAIN && err != ECONNRESET )
				Com_Printf( "NET_GetPacket: %s\n", NET_ErrorString() );
		}
		else
		{

			memset( ((struct sockaddr_in *)&from)->sin_zero, 0, 8 );
			SockadrToNetadr( (struct sockaddr *) &from, net_from );
			net_message->readcount = 0;
			
			if( ret >= net_message->maxsize ) {
				Com_Printf( "Oversize packet from %s\n", NET_AdrToString (*net_from) );
				return qfalse;
			}
			

			
			Com_Printf("NET_GetPacket: from=%s fromlen=%d\n", NET_AdrToString (*net_from), fromlen);

			net_message->cursize = ret;
			return qtrue;
		}
	}
	
	return qfalse;
}

void Sys_SendPacket( int length, const void *data, netadr_t to ) {
#if EMSCRIPTEN
	int ip_a = to.ip[0]; // 0
	int ip_b = to.ip[1]; // 1
	int ip_c = to.ip[2]; // 2
	int ip_d = to.ip[3]; // 3
	int port = to.port; // 4
	int msg = (int)data; // 5
	int msg_length = length; // 6
	EM_ASM_({
		NET_send_packet($0, $1, $2, $3, $4, $5, $6);
	}, ip_a, ip_b, ip_c, ip_d, port, msg, msg_length);
#else
	int ret = SOCKET_ERROR;
	struct sockaddr_storage	addr;
	memset(&addr, 0, sizeof(addr));
	NetadrToSockadr( &to, (struct sockaddr *) &addr );
	ret = sendto( ip_socket, (const char *)data, length, 0, (struct sockaddr *) &addr, sizeof(struct sockaddr_in) );
	//Com_Printf("Sys_SendPacket: to=%d.%d.%d.%d:%d length=%d data=%d\n", to.ip[0], to.ip[1], to.ip[2], to.ip[3], to.port, length, data);
	if( ret == SOCKET_ERROR ) {
		int err = socketError;
		if( err == EAGAIN )
			return; // wouldblock is silent
		Com_Printf( "Sys_SendPacket: %s\n", NET_ErrorString() );
	}
#endif
}


//=============================================================================

/*
==================
Sys_IsLANAddress

LAN clients will have their rate var ignored
==================
*/
qboolean Sys_IsLANAddress( netadr_t adr ) {
	return qfalse;
}

void Sys_ShowIP(void) {
	int i;
	char addrbuf[NET_ADDRSTRMAXLEN];

	for(i = 0; i < numIP; i++)
	{
		Sys_SockaddrToString(addrbuf, sizeof(addrbuf), (struct sockaddr *) &localIP[i].addr);

		if(localIP[i].type == NA_IP)
			Com_Printf( "IP: %s\n", addrbuf);
	}
}

SOCKET NET_IPSocket( char *net_interface, int port, int *err ) {
	SOCKET				newsocket;
	struct sockaddr_in	address;
	ioctlarg_t			_true = 1;
	int					i = 1;

	*err = 0;

	if( net_interface ) {
		Com_Printf( "Opening IP socket: %s:%i\n", net_interface, port );
	}
	else {
		Com_Printf( "Opening IP socket: 0.0.0.0:%i\n", port );
	}

	if( ( newsocket = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP ) ) == INVALID_SOCKET ) {
		*err = socketError;
		Com_Printf( "WARNING: NET_IPSocket: socket: %s\n", NET_ErrorString() );
		return newsocket;
	}
	// make it non-blocking
	if( ioctlsocket( newsocket, FIONBIO, &_true ) == SOCKET_ERROR ) {
		Com_Printf( "WARNING: NET_IPSocket: ioctl FIONBIO: %s\n", NET_ErrorString() );
		*err = socketError;
		closesocket(newsocket);
		return INVALID_SOCKET;
	}

	// make it broadcast capable
	if( setsockopt( newsocket, SOL_SOCKET, SO_BROADCAST, (char *) &i, sizeof(i) ) == SOCKET_ERROR ) {
		Com_Printf( "WARNING: NET_IPSocket: setsockopt SO_BROADCAST: %s\n", NET_ErrorString() );
	}

	if( !net_interface || !net_interface[0]) {
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		if(!Sys_StringToSockaddr( net_interface, (struct sockaddr *)&address, sizeof(address), AF_INET))
		{
			closesocket(newsocket);
			return INVALID_SOCKET;
		}
	}

	if( port == PORT_ANY ) {
		address.sin_port = 0;
	}
	else {
		address.sin_port = htons( (short)port );
	}

	if( bind( newsocket, (const sockaddr *)&address, sizeof(address) ) == SOCKET_ERROR ) {
		Com_Printf( "WARNING: NET_IPSocket: bind: %s\n", NET_ErrorString() );
		*err = socketError;
		closesocket( newsocket );
		return INVALID_SOCKET;
	}

	return newsocket;
}

void NET_OpenIP( void ) {
	int		i;
	int		err;
	int		port;
	port = net_port->integer;
	// automatically scan for a valid port, so multiple
	// dedicated servers can be started without requiring
	// a different net_port for each one

	for( i = 0 ; i < 10 ; i++ ) {
		ip_socket = NET_IPSocket( net_ip->string, port + i, &err );
		if (ip_socket != INVALID_SOCKET) {
			Cvar_SetValue( "net_port", port + i );
			break;
		}
		else
		{
			if(err == EAFNOSUPPORT)
				break;
		}
	}
		
	if(ip_socket == INVALID_SOCKET)
		Com_Printf( "WARNING: Couldn't bind to a v4 ip address.\n");
}

static qboolean NET_GetCvars( void ) {
	int modified;

	net_enabled = Cvar_Get( "net_enabled", "1", CVAR_LATCH | CVAR_ARCHIVE );
	modified = net_enabled->modified;
	net_enabled->modified = qfalse;

	net_ip = Cvar_Get( "net_ip", "0.0.0.0", CVAR_LATCH );
	modified += net_ip->modified;
	net_ip->modified = qfalse;
	
	net_port = Cvar_Get( "net_port", va( "%i", PORT_SERVER ), CVAR_LATCH );
	modified += net_port->modified;
	net_port->modified = qfalse;
	
	net_dropsim = Cvar_Get("net_dropsim", "", CVAR_TEMP);

	return modified ? qtrue : qfalse;
}

void NET_Config( qboolean enableNetworking ) {
	qboolean	modified;
	qboolean	stop;
	qboolean	start;

	// get any latched changes to cvars
	modified = NET_GetCvars();

	if( !net_enabled->integer ) {
		enableNetworking = 0;
	}

	// if enable state is the same and no cvars were modified, we have nothing to do
	if( enableNetworking == networkingEnabled && !modified ) {
		return;
	}

	if( enableNetworking == networkingEnabled ) {
		if( enableNetworking ) {
			stop = qtrue;
			start = qtrue;
		}
		else {
			stop = qfalse;
			start = qfalse;
		}
	}
	else {
		if( enableNetworking ) {
			stop = qfalse;
			start = qtrue;
		}
		else {
			stop = qtrue;
			start = qfalse;
		}
		networkingEnabled = enableNetworking;
	}

	if( stop ) {
		if ( ip_socket != INVALID_SOCKET ) {
			closesocket( ip_socket );
			ip_socket = INVALID_SOCKET;
		}
	}
	if( start ) {
		if (net_enabled->integer) {
			NET_OpenIP();
		}
	}
}


void NET_Init( void ) {}
void NET_Shutdown( void ) {}

/*
====================
NET_Event

Called from NET_Sleep which uses select() to determine which sockets have seen action.
====================
*/

CCALL void NET_inject_packet(int ip_a, int ip_b, int ip_c, int ip_d, int port, char *msg, int msg_length) {
	byte bufData[MAX_MSGLEN + 1];
	netadr_t from = {(netadrtype_t)0};
	from.type = NA_IP;
	msg_t netmsg;
	from.ip[0] = ip_a;
	from.ip[1] = ip_b;
	from.ip[2] = ip_c;
	from.ip[3] = ip_d;
	from.port = port;
	

	//printf("NET_inject_packet: %d.%d.%d.%d:%d msg=%d msg_length=%d\n", ip_a, ip_b, ip_c, ip_d, port, msg, msg_length);

	MSG_Init(&netmsg, (byte *)msg, msg_length);
	netmsg.readcount = 0;
	netmsg.cursize = msg_length;
	CL_PacketEvent(from, &netmsg);
}

CCALL void NET_inject_packet_for_client(int ip_a, int ip_b, int ip_c, int ip_d, int port, char *msg, int msg_length) {
	byte bufData[MAX_MSGLEN + 1];
	netadr_t from = {(netadrtype_t)0};
	from.type = NA_IP;
	msg_t netmsg;
	from.ip[0] = ip_a;
	from.ip[1] = ip_b;
	from.ip[2] = ip_c;
	from.ip[3] = ip_d;
	from.port = port;
	

	//printf("NET_inject_packet_for_client: %d.%d.%d.%d:%d msg=%d msg_length=%d\n", ip_a, ip_b, ip_c, ip_d, port, msg, msg_length);

	MSG_Init(&netmsg, (byte *)msg, msg_length);
	netmsg.readcount = 0;
	netmsg.cursize = msg_length;
	SV_PacketEvent(from, &netmsg);
}

void NET_Event(fd_set *fdr)
{
	byte bufData[MAX_MSGLEN + 1];
	netadr_t from = { (netadrtype_t)0};
	msg_t netmsg;
	
	while(1)
	{
		MSG_Init(&netmsg, bufData, sizeof(bufData));

		if(NET_GetPacket(&from, &netmsg, fdr))
		{
			if(net_dropsim->value > 0.0f && net_dropsim->value <= 100.0f)
			{
				// com_dropsim->value percent of incoming packets get dropped.
				if(rand() < (int) (((double) RAND_MAX) / 100.0 * (double) net_dropsim->value))
					continue;          // drop this packet
			}

			if(com_sv_running->integer)
				Com_RunAndTimeServerPacket(&from, &netmsg);
			else
				CL_PacketEvent(from, &netmsg);
		}
		else
			break;
	}
}

/*
====================
NET_Sleep

Sleeps msec or until something happens on the network
====================
*/
void NET_Sleep(int msec)
{
	struct timeval timeout;
	fd_set fdr;
	int retval;
	SOCKET highestfd = INVALID_SOCKET;

	if(msec < 0)
		msec = 0;

	FD_ZERO(&fdr);

	if(ip_socket != INVALID_SOCKET)
	{
		FD_SET(ip_socket, &fdr);

		highestfd = ip_socket;
	}
	

#ifdef _WIN32
	if(highestfd == INVALID_SOCKET)
	{
		// windows ain't happy when select is called without valid FDs
		SleepEx(msec, 0);
		return;
	}
#endif

	timeout.tv_sec = msec/1000;
	timeout.tv_usec = (msec%1000)*1000;

	retval = select(highestfd + 1, &fdr, NULL, NULL, &timeout);

	if(retval == SOCKET_ERROR)
		Com_Printf("Warning: select() syscall failed: %s\n", NET_ErrorString());
	else if(retval > 0)
		NET_Event(&fdr);
}

/*
====================
NET_Restart_f
====================
*/
void NET_Restart_f(void)
{
	NET_Config(qtrue);
}
