/*
=============================================================================
START SERVER MENU *****
=============================================================================
*/

#include "ui_local.h"

#define GAMESERVER_FRAME		"gfx/ui/frame"
#define GAMESERVER_BACK0		"gfx/ui/back_0"
#define GAMESERVER_BACK1		"gfx/ui/back_1"
#define GAMESERVER_NEXT0		"gfx/ui/next_0"
#define GAMESERVER_NEXT1		"gfx/ui/next_1"
#define GAMESERVER_SELECT		"gfx/ui/maps_select"
#define GAMESERVER_SELECTED		"gfx/ui/maps_selected"
#define GAMESERVER_FIGHT0		"gfx/ui/play_0"
#define GAMESERVER_FIGHT1		"gfx/ui/play_1"
#define GAMESERVER_UNKNOWNMAP	"gfx/ui/unknownmap"
#define GAMESERVER_ARROWS		"gfx/ui/arrows_horz_0"
#define GAMESERVER_ARROWSL		"gfx/ui/arrows_horz_left"
#define GAMESERVER_ARROWSR		"gfx/ui/arrows_horz_right"

#define MAX_MAPROWS		2
#define MAX_MAPCOLS		2
#define MAX_MAPSPERPAGE	4
#define MAX_NAMELENGTH	32

#define ID_GAMETYPE				10
#define ID_PICTURES				11
#define ID_PREVPAGE				15
#define ID_NEXTPAGE				16
#define ID_STARTSERVERBACK		17
#define ID_STARTSERVERNEXT		18

typedef struct
{
	menuframework_s	menu;
	menulist_s		gametype;
	menubitmap_s	frame;
	menubitmap_s	mappics[MAX_MAPSPERPAGE];
	menubitmap_s	mapbuttons[MAX_MAPSPERPAGE];
	menubitmap_s	arrows;
	menubitmap_s	prevpage;
	menubitmap_s	nextpage;
	menubitmap_s	back;
	menubitmap_s	next;
	menutext_s		mapname;
	menubitmap_s	item_null;
	qbool		    multiplayer;
	int				currentmap;
	int				nummaps;
	int				page;
	int				maxpages;
	int 			maplist[MAX_ARENAS];
} startserver_t;

startserver_t s_startserver;

static const char *gametype_items[] =
{
	"Fastcap",
	"Adventure",
	"Defrag",
	"Freestyle",
	NULL
};

static void UI_ServerOptionsMenu( qbool multiplayer );

/*
=================
GametypeBits
=================
*/
static int GametypeBits( char *string )
{
	int		bits;
	char	*p;
	char	*token;

	bits = 0;
	p = string;
	while( 1 ) {
		token = COM_ParseExt( &p, qfalse );
		if( token[0] == 0 )
		{
			break;
		}

		if( Q_stricmp( token, "ctf" ) == 0 || Q_stricmp( token, "fastcap" ) == 0 )
		{
			bits |= 1 << GT_FASTCAP;
			continue;
		}

		if( Q_stricmp( token, "adventure" ) == 0 || Q_stricmp( token, "level" ) == 0 )
		{
			bits |= 1 << GT_ADVENTURE;
			continue;
		}

		if( Q_stricmp( token, "defrag" ) == 0 || Q_stricmp( token, "run" ) == 0 || Q_stricmp( token, "training" ) == 0 || Q_stricmp( token, "accuracy" ) == 0 )
		{
			bits |= 1 << GT_DEFRAG;
			continue;
		}

		if( Q_stricmp( token, "freestyle" ) == 0 || Q_stricmp( token, "ffa" ) == 0 || Q_stricmp( token, "tourney" ) == 0 || Q_stricmp( token, "team" ) == 0)
		{
			bits |= 1 << GT_FREESTYLE-1; //SLK: since we skip GT_TEAMRUN
			continue;
		}
	}

	return bits;
}


/*
=================
StartServer_Update
=================
*/
static void StartServer_Update( void )
{
	int				i;
	int				top;
	static	char	picname[MAX_MAPSPERPAGE][MAX_QPATH];
	const char		*info;
	char			mapname[MAX_NAMELENGTH];

	top = s_startserver.page*MAX_MAPSPERPAGE;

	for (i=0; i<MAX_MAPSPERPAGE; i++)
	{
		if (top+i >= s_startserver.nummaps)
			break;

		info = UI_GetArenaInfoByNumber( s_startserver.maplist[ top + i ]);
		Q_strncpyz( mapname, Info_ValueForKey( info, "map"), MAX_NAMELENGTH );
		Q_strupr( mapname );

		Com_sprintf( picname[i], sizeof(picname[i]), "levelshots/%s", mapname );

		s_startserver.mappics[i].generic.flags &= ~QMF_HIGHLIGHT;
		s_startserver.mappics[i].generic.name   = picname[i];
		s_startserver.mappics[i].shader         = 0;

		// reset
		s_startserver.mapbuttons[i].generic.flags |= QMF_PULSEIFFOCUS;
		s_startserver.mapbuttons[i].generic.flags &= ~QMF_INACTIVE;
	}

	for (; i<MAX_MAPSPERPAGE; i++)
	{
		s_startserver.mappics[i].generic.flags &= ~QMF_HIGHLIGHT;
		s_startserver.mappics[i].generic.name   = NULL;
		s_startserver.mappics[i].shader         = 0;

		// disable
		s_startserver.mapbuttons[i].generic.flags &= ~QMF_PULSEIFFOCUS;
		s_startserver.mapbuttons[i].generic.flags |= QMF_INACTIVE;
	}


	// no servers to start
	if( !s_startserver.nummaps )
	{
		s_startserver.next.generic.flags |= QMF_INACTIVE;

		// set the map name
		strcpy( s_startserver.mapname.string, "NO MAPS FOUND" );
	}
	else
	{
		// set the highlight
		s_startserver.next.generic.flags &= ~QMF_INACTIVE;
		i = s_startserver.currentmap - top;
		if ( i >=0 && i < MAX_MAPSPERPAGE )
		{
			s_startserver.mappics[i].generic.flags    |= QMF_HIGHLIGHT;
			s_startserver.mapbuttons[i].generic.flags &= ~QMF_PULSEIFFOCUS;
		}

		// set the map name
		info = UI_GetArenaInfoByNumber( s_startserver.maplist[ s_startserver.currentmap ]);
		Q_strncpyz( s_startserver.mapname.string, Info_ValueForKey( info, "map" ), MAX_NAMELENGTH);
	}

	Q_strupr( s_startserver.mapname.string );
}


/*
=================
StartServer_MapEvent
=================
*/
static void StartServer_MapEvent( void* ptr, int event )
{
	if( event != QM_ACTIVATED)
		return;

	s_startserver.currentmap = (s_startserver.page*MAX_MAPSPERPAGE) + (((menucommon_s*)ptr)->id - ID_PICTURES);
	StartServer_Update();
}


/*
=================
StartServer_GametypeEvent
=================
*/
static void StartServer_GametypeEvent( void* ptr, int event )
 {
	int			i;
	int			count;
	int			gamebits;
	int			matchbits;
	const char	*info;

	if( event != QM_ACTIVATED)
		return;

	count = UI_GetNumArenas();
	s_startserver.nummaps = 0;
	matchbits = 1 << s_startserver.gametype.curvalue;

	for( i = 0; i < count; i++ ) {
		info = UI_GetArenaInfoByNumber( i );

		// SLK: token for gametype
		gamebits = GametypeBits( Info_ValueForKey( info, "type") );
		if( !( gamebits & matchbits ) ) { // if no result, try other token
			gamebits = GametypeBits( Info_ValueForKey( info, "style") );
			if( !( gamebits & matchbits ) )
				continue;
		}

		s_startserver.maplist[ s_startserver.nummaps ] = i;
		s_startserver.nummaps++;
	}
	s_startserver.maxpages = (s_startserver.nummaps + MAX_MAPSPERPAGE-1)/MAX_MAPSPERPAGE;
	s_startserver.page = 0;
	s_startserver.currentmap = 0;

	StartServer_Update();
}


/*
=================
StartServer_MenuEvent
=================
*/
static void StartServer_MenuEvent( void* ptr, int event )
{
	if( event != QM_ACTIVATED )
		return;

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_PREVPAGE:
		if( s_startserver.page > 0 )
		{
			s_startserver.page--;
			StartServer_Update();
		}
		break;

	case ID_NEXTPAGE:
		if( s_startserver.page < s_startserver.maxpages - 1 )
		{
			s_startserver.page++;
			StartServer_Update();
		}
		break;

	case ID_STARTSERVERNEXT:
		UI_ServerOptionsMenu( qfalse );
		break;

	case ID_STARTSERVERBACK:
		UI_PopMenu();
		break;
	}
}


/*
===============
StartServer_LevelshotDraw
===============
*/
static void StartServer_LevelshotDraw( void *self )
{
	menubitmap_s	*b;
	int				x;
	int				y;
	int				w;
	int				h;
	int				n;
	const char		*info;

	b = (menubitmap_s *)self;

	if( !b->generic.name )
		return;

	if( b->generic.name && !b->shader ) {
		b->shader = trap_R_RegisterShaderNoMip( b->generic.name );
		if( !b->shader && b->errorpic )
			b->shader = trap_R_RegisterShaderNoMip( b->errorpic );
	}

	if( b->focuspic && !b->focusshader )
		b->focusshader = trap_R_RegisterShaderNoMip( b->focuspic );

	x = b->generic.x;
	y = b->generic.y;
	w = b->width;
	h =	b->height;
	if( b->shader )
		UI_DrawHandlePic( x, y, w, h, b->shader );

	x = b->generic.x;
	y = b->generic.y + b->height;
	UI_FillRect( x, y, b->width, 28, colorBlack );

	x += b->width / 2;
	y += 4;
	n = s_startserver.page * MAX_MAPSPERPAGE + b->generic.id - ID_PICTURES;

	info = UI_GetArenaInfoByNumber( s_startserver.maplist[ n ]);
	UI_DrawString( x, y, Info_ValueForKey( info, "map" ), UI_CENTER|UI_SMALLFONT, color_white );

	x = b->generic.x;
	y = b->generic.y;
	w = b->width;
	h =	b->height + 28;
	if( b->generic.flags & QMF_HIGHLIGHT )
		UI_DrawHandlePic( x, y, w, h, b->focusshader );

}


/*
=================
StartServer_MenuInit
=================
*/
static void StartServer_MenuInit( void )
{
	int	i, x, y;
	static char mapnamebuffer[64];

	// zero set all our globals
	memset( &s_startserver, 0 ,sizeof(startserver_t) );

	StartServer_Cache();

	s_startserver.menu.wrapAround = qtrue;
	s_startserver.menu.fullscreen = qtrue;
	
	s_startserver.frame.generic.type	= MTYPE_BITMAP;
	s_startserver.frame.generic.flags	= QMF_INACTIVE;
	s_startserver.frame.generic.name	= GAMESERVER_FRAME;
	s_startserver.frame.width			= 640;
	s_startserver.frame.height			= 480;
	s_startserver.frame.generic.x		= 0;
	s_startserver.frame.generic.y		= 0;
	
	s_startserver.gametype.generic.type		= MTYPE_SPINCONTROL;
	s_startserver.gametype.generic.name		= "Game Type:";
	s_startserver.gametype.generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_startserver.gametype.generic.callback	= StartServer_GametypeEvent;
	s_startserver.gametype.generic.id		= ID_GAMETYPE;
	s_startserver.gametype.generic.x		= 320 - 24;
	s_startserver.gametype.generic.y		= 368;
	s_startserver.gametype.itemnames		= gametype_items;

	for (i=0; i<MAX_MAPSPERPAGE; i++)
	{
		x =	(i % MAX_MAPCOLS) * (128+8) + 188;
		y = (i / MAX_MAPROWS) * (128+8) + 96;

		s_startserver.mappics[i].generic.type   = MTYPE_BITMAP;
		s_startserver.mappics[i].generic.flags  = QMF_LEFT_JUSTIFY|QMF_INACTIVE;
		s_startserver.mappics[i].generic.x	    = x;
		s_startserver.mappics[i].generic.y	    = y;
		s_startserver.mappics[i].generic.id		= ID_PICTURES+i;
		s_startserver.mappics[i].width  		= 128;
		s_startserver.mappics[i].height  	    = 96;
		s_startserver.mappics[i].focuspic       = GAMESERVER_SELECTED;
		s_startserver.mappics[i].errorpic       = GAMESERVER_UNKNOWNMAP;
		s_startserver.mappics[i].generic.ownerdraw = StartServer_LevelshotDraw;

		s_startserver.mapbuttons[i].generic.type     = MTYPE_BITMAP;
		s_startserver.mapbuttons[i].generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_NODEFAULTINIT;
		s_startserver.mapbuttons[i].generic.id       = ID_PICTURES+i;
		s_startserver.mapbuttons[i].generic.callback = StartServer_MapEvent;
		s_startserver.mapbuttons[i].generic.x	     = x - 30;
		s_startserver.mapbuttons[i].generic.y	     = y - 32;
		s_startserver.mapbuttons[i].width  		     = 256;
		s_startserver.mapbuttons[i].height  	     = 248;
		s_startserver.mapbuttons[i].generic.left     = x;
		s_startserver.mapbuttons[i].generic.top  	 = y;
		s_startserver.mapbuttons[i].generic.right    = x + 128;
		s_startserver.mapbuttons[i].generic.bottom   = y + 128;
		s_startserver.mapbuttons[i].focuspic         = GAMESERVER_SELECT;
	}

	s_startserver.arrows.generic.type  = MTYPE_BITMAP;
	s_startserver.arrows.generic.name  = GAMESERVER_ARROWS;
	s_startserver.arrows.generic.flags = QMF_INACTIVE;
	s_startserver.arrows.generic.x	   = 260;
	s_startserver.arrows.generic.y	   = 400;
	s_startserver.arrows.width  	   = 128;
	s_startserver.arrows.height  	   = 32;

	s_startserver.prevpage.generic.type	    = MTYPE_BITMAP;
	s_startserver.prevpage.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_startserver.prevpage.generic.callback = StartServer_MenuEvent;
	s_startserver.prevpage.generic.id	    = ID_PREVPAGE;
	s_startserver.prevpage.generic.x		= 260;
	s_startserver.prevpage.generic.y		= 400;
	s_startserver.prevpage.width  		    = 64;
	s_startserver.prevpage.height  		    = 32;
	s_startserver.prevpage.focuspic         = GAMESERVER_ARROWSL;

	s_startserver.nextpage.generic.type	    = MTYPE_BITMAP;
	s_startserver.nextpage.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_startserver.nextpage.generic.callback = StartServer_MenuEvent;
	s_startserver.nextpage.generic.id	    = ID_NEXTPAGE;
	s_startserver.nextpage.generic.x		= 325;
	s_startserver.nextpage.generic.y		= 400;
	s_startserver.nextpage.width  		    = 64;
	s_startserver.nextpage.height  		    = 32;
	s_startserver.nextpage.focuspic         = GAMESERVER_ARROWSR;

	s_startserver.mapname.generic.type  = MTYPE_PTEXT;
	s_startserver.mapname.generic.flags = QMF_CENTER_JUSTIFY|QMF_INACTIVE;
	s_startserver.mapname.generic.x	    = 320;
	s_startserver.mapname.generic.y	    = 56;
	s_startserver.mapname.string        = mapnamebuffer;
	s_startserver.mapname.style         = UI_CENTER|UI_SMALLFONT;
	s_startserver.mapname.color         = color_white;

	s_startserver.back.generic.type	    = MTYPE_BITMAP;
	s_startserver.back.generic.name     = GAMESERVER_BACK0;
	s_startserver.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_startserver.back.generic.callback = StartServer_MenuEvent;
	s_startserver.back.generic.id	    = ID_STARTSERVERBACK;
	s_startserver.back.generic.x		= 0;
	s_startserver.back.generic.y		= 480-64;
	s_startserver.back.width  		    = 128;
	s_startserver.back.height  		    = 64;
	s_startserver.back.focuspic         = GAMESERVER_BACK1;

	s_startserver.next.generic.type	    = MTYPE_BITMAP;
	s_startserver.next.generic.name     = GAMESERVER_NEXT0;
	s_startserver.next.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_startserver.next.generic.callback = StartServer_MenuEvent;
	s_startserver.next.generic.id	    = ID_STARTSERVERNEXT;
	s_startserver.next.generic.x		= 640;
	s_startserver.next.generic.y		= 480-64;
	s_startserver.next.width  		    = 128;
	s_startserver.next.height  		    = 64;
	s_startserver.next.focuspic         = GAMESERVER_NEXT1;

	s_startserver.item_null.generic.type	= MTYPE_BITMAP;
	s_startserver.item_null.generic.flags	= QMF_LEFT_JUSTIFY|QMF_MOUSEONLY|QMF_SILENT;
	s_startserver.item_null.generic.x		= 0;
	s_startserver.item_null.generic.y		= 0;
	s_startserver.item_null.width			= 640;
	s_startserver.item_null.height			= 480;

	Menu_AddItem( &s_startserver.menu, &s_startserver.frame );
	
	Menu_AddItem( &s_startserver.menu, &s_startserver.gametype );
	for (i=0; i<MAX_MAPSPERPAGE; i++)
	{
		Menu_AddItem( &s_startserver.menu, &s_startserver.mappics[i] );
		Menu_AddItem( &s_startserver.menu, &s_startserver.mapbuttons[i] );
	}

	Menu_AddItem( &s_startserver.menu, &s_startserver.arrows );
	Menu_AddItem( &s_startserver.menu, &s_startserver.prevpage );
	Menu_AddItem( &s_startserver.menu, &s_startserver.nextpage );
	Menu_AddItem( &s_startserver.menu, &s_startserver.back );
	Menu_AddItem( &s_startserver.menu, &s_startserver.next );
	Menu_AddItem( &s_startserver.menu, &s_startserver.item_null );

	StartServer_GametypeEvent( NULL, QM_ACTIVATED );
}


/*
=================
StartServer_Cache
=================
*/
void StartServer_Cache( void )
{
	int			i;
	const char	*info;
	qbool		precache;
	char		picname[ MAX_QPATH ], mapname[ MAX_NAMELENGTH ] ;

	trap_R_RegisterShaderNoMip( GAMESERVER_FRAME );
	trap_R_RegisterShaderNoMip( GAMESERVER_BACK0 );
	trap_R_RegisterShaderNoMip( GAMESERVER_BACK1 );
	trap_R_RegisterShaderNoMip( GAMESERVER_NEXT0 );
	trap_R_RegisterShaderNoMip( GAMESERVER_NEXT1 );
	trap_R_RegisterShaderNoMip( GAMESERVER_SELECT );
	trap_R_RegisterShaderNoMip( GAMESERVER_SELECTED );
	trap_R_RegisterShaderNoMip( GAMESERVER_UNKNOWNMAP );
	trap_R_RegisterShaderNoMip( GAMESERVER_ARROWS );
	trap_R_RegisterShaderNoMip( GAMESERVER_ARROWSL );
	trap_R_RegisterShaderNoMip( GAMESERVER_ARROWSR );

	precache = trap_Cvar_VariableValue("com_buildscript");

	if( precache ) {
		for( i = 0; i < UI_GetNumArenas(); i++ ) {
			info = UI_GetArenaInfoByNumber( i );
			Q_strncpyz( mapname, Info_ValueForKey( info, "map"), MAX_NAMELENGTH );
			Q_strupr( mapname );

			Com_sprintf( picname, sizeof(picname), "levelshots/%s", mapname );
			trap_R_RegisterShaderNoMip(picname);
		}
	}

}


/*
=================
UI_StartServerMenu
=================
*/
void UI_StartServerMenu( qbool multiplayer )
{
	StartServer_MenuInit();
	s_startserver.multiplayer = multiplayer;
	UI_PushMenu( &s_startserver.menu );
}



/*
===========================================================================================================================================
SERVER OPTIONS MENU *****
SLK: this comes after picking a gametype and map
===========================================================================================================================================
*/

#define ID_RULESET      21
#define ID_DEFRAGMODE   22
#define ID_FASTCAPMODE  23
#define ID_GO			24
#define ID_BACK			25

typedef struct
{
	menuframework_s		menu;
	
	menubitmap_s		frame;
	menubitmap_s		mappic;
	menubitmap_s		picframe;

	menulist_s			ruleset;
	menulist_s			defragmode;
	menulist_s			fastcapmode;
	menuradiobutton_s	overbounces;
	menuradiobutton_s	cheats;
	menuradiobutton_s	reversemap;

	menubitmap_s		go;
	menubitmap_s		next;
	menubitmap_s		back;

	qbool				multiplayer;

	int					gametype;
	char				mapnamebuffer[32];
} serveroptions_t;

static const char *ruleset_items[] =
{
	"VQ3",
	"CPM",
	"xVQ3",
	"xCPM",
	NULL
};

static const char *fastcap_items[] =
{
	"0 Default",
	"1 no Teleporters",
	"2 no Jump Pads",
	"3 no Doors",
	"4 no Teles, no Pads",
	"5 Disable all",
	"6 Rocket Launcher, Disable All",
	"7 Plasma Gun, Disable All",
	"8 Weapon Combo, Disable All",
	NULL
};

static const char *defrag_items[] =
{
	"0 Default",
	"1 Strafe",
	"2 Rocket Launcher",
	"3 Plasma Gun",
	"4 Weapon Combo",
	"5 Ice Mode",
	NULL
};

serveroptions_t s_serveroptions;

/*
=================
ServerOptions_Start
=================
*/
void ServerOptions_Start( void )
{
	const char *info;

	// we play alone local, no interference possible
	trap_Cvar_SetValue ( "dfx_interference", 0 );
	trap_Cvar_SetValue ( "dfx_obs", s_serveroptions.overbounces.curvalue );
	trap_Cvar_SetValue ( "dfx_reversemap", s_serveroptions.reversemap.curvalue );
	trap_Cvar_SetValue ( "sv_cheats", s_serveroptions.cheats.curvalue );
	trap_Cvar_SetValue ( "dedicated", 0 );
	trap_Cvar_SetValue ( "timelimit", 0 );
	trap_Cvar_SetValue ( "sv_punkbuster", 0 );

	info = s_serveroptions.mapnamebuffer;
	Com_Printf( s_serveroptions.mapnamebuffer);

	// we play alone local, no teamruns would work
	if (s_serveroptions.gametype == GT_TEAMRUN)
		s_serveroptions.gametype = GT_DEFRAG;

	if ( s_serveroptions.cheats.curvalue )
		trap_Cmd_ExecuteText( EXEC_APPEND, va( "wait ; wait ; devmap %s\n", info));
	else
		trap_Cmd_ExecuteText( EXEC_APPEND, va( "wait ; wait ; map %s\n", info));

	trap_Cvar_SetValue ( "dfx_gametype", s_serveroptions.gametype );
	trap_Cvar_SetValue ( "dfx_ruleset", s_serveroptions.ruleset.curvalue );

	if (s_serveroptions.gametype == GT_DEFRAG)
		trap_Cvar_SetValue ( "dfx_mode", s_serveroptions.defragmode.curvalue );
	else
		trap_Cvar_SetValue ( "dfx_mode", 0 );

	if (s_serveroptions.gametype == GT_FASTCAP)
		trap_Cvar_SetValue ( "dfx_fastcapmode", s_serveroptions.fastcapmode.curvalue );
	else
		trap_Cvar_SetValue ( "dfx_fastcapmode", 0 );

	// no need to unlag when playin a local game
	trap_Cmd_ExecuteText( EXEC_APPEND, "g_unlagged 0\n");

	// set player's team
	trap_Cmd_ExecuteText( EXEC_APPEND, "team free\n" );
}


/*
=================
ServerOptions_Event
=================
*/
static void ServerOptions_Event( void* ptr, int event ) {

	if( event != QM_ACTIVATED )
		return;

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_GO:
		ServerOptions_Start();
		break;

	case ID_STARTSERVERNEXT:
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	}

}

/*
===============
ServerOptions_LevelshotDraw
===============
*/
static void ServerOptions_LevelshotDraw( void *self )
{
	menubitmap_s    *b;
	int				x, y;

	b = (menubitmap_s *)self;

	Bitmap_Draw( b );

	x = b->generic.x;
	y = b->generic.y + b->height;
	UI_FillRect( x, y, b->width, 40, colorBlack );

	x += b->width / 2;
	y += 4;
	UI_DrawString( x, y, s_serveroptions.mapnamebuffer, UI_CENTER|UI_SMALLFONT, color_white );

	y += SMALLCHAR_HEIGHT;
	UI_DrawString( x, y, gametype_items[s_serveroptions.gametype], UI_CENTER|UI_SMALLFONT, color_white );
}


/*
=================
ServerOptions_SetMenuItems
=================
*/
void ServerOptions_SetMenuItems( void )
{
	static char picname[MAX_QPATH];
	char		mapname[MAX_NAMELENGTH];
	const char	*info;

	s_serveroptions.ruleset.curvalue      = trap_Cvar_VariableValue( "dfx_ruleset" );
	s_serveroptions.defragmode.curvalue   = trap_Cvar_VariableValue( "dfx_mode" );
	s_serveroptions.fastcapmode.curvalue  = trap_Cvar_VariableValue( "dfx_fastcapmode" );
	s_serveroptions.overbounces.curvalue  = trap_Cvar_VariableValue( "dfx_obs" );
	s_serveroptions.reversemap.curvalue   = trap_Cvar_VariableValue( "dfx_reversemap" );

	// set the map pic
	info = UI_GetArenaInfoByNumber( s_startserver.maplist[ s_startserver.currentmap ]);
	Q_strncpyz( mapname, Info_ValueForKey( info, "map" ), sizeof( mapname ) );
	Q_strupr( mapname );
	Com_sprintf( picname, sizeof( picname ), "levelshots/%s", mapname );
	s_serveroptions.mappic.generic.name = picname;

	// set the map name
	strcpy( s_serveroptions.mapnamebuffer, s_startserver.mapname.string );
	Q_strupr( s_serveroptions.mapnamebuffer );
}



/*
=================
ServerOptions_MenuInit
=================
*/
#define OPTIONS_X	456
#define OPTIONS_Y   272

static void ServerOptions_MenuInit( qbool multiplayer )
{
	int			n, y;
	gametype_t	gametype;

	memset( &s_serveroptions, 0 ,sizeof(serveroptions_t) );
	s_serveroptions.multiplayer		= qfalse;

	// SLK: since we 'skip GT_TEAMRUN in the menulist we need to check for it and 'add' +1, so it becomes correct gametype
	if (s_startserver.gametype.curvalue == GT_TEAMRUN)
		s_serveroptions.gametype = GT_FREESTYLE;
	else
		s_serveroptions.gametype = s_startserver.gametype.curvalue;

	ServerOptions_Cache();

	s_serveroptions.menu.wrapAround = qtrue;
	s_serveroptions.menu.fullscreen = qtrue;
	
	s_serveroptions.frame.generic.type	= MTYPE_BITMAP;
	s_serveroptions.frame.generic.flags	= QMF_INACTIVE;
	s_serveroptions.frame.generic.name	= GAMESERVER_FRAME;
	s_serveroptions.frame.width			= 640;
	s_serveroptions.frame.height		= 480;
	s_serveroptions.frame.generic.x		= 0;
	s_serveroptions.frame.generic.y		= 0;

	s_serveroptions.mappic.generic.type			= MTYPE_BITMAP;
	s_serveroptions.mappic.generic.flags		= QMF_LEFT_JUSTIFY|QMF_INACTIVE;
	s_serveroptions.mappic.generic.x			= 352;
	s_serveroptions.mappic.generic.y			= 80;
	s_serveroptions.mappic.width				= 160;
	s_serveroptions.mappic.height				= 120;
	s_serveroptions.mappic.errorpic				= GAMESERVER_UNKNOWNMAP;
	s_serveroptions.mappic.generic.ownerdraw	= ServerOptions_LevelshotDraw;

	s_serveroptions.picframe.generic.type		= MTYPE_BITMAP;
	s_serveroptions.picframe.generic.flags		= QMF_LEFT_JUSTIFY|QMF_INACTIVE|QMF_HIGHLIGHT;
	s_serveroptions.picframe.generic.x			= 352 - 38;
	s_serveroptions.picframe.generic.y			= 80 - 40;
	s_serveroptions.picframe.width  			= 320;
	s_serveroptions.picframe.height  			= 320;
	s_serveroptions.picframe.focuspic			= GAMESERVER_SELECT;

	y = OPTIONS_Y;
    s_serveroptions.ruleset.generic.type		= MTYPE_SPINCONTROL;
	s_serveroptions.ruleset.generic.name		= "Ruleset:";
	s_serveroptions.ruleset.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_serveroptions.ruleset.generic.callback	= StartServer_GametypeEvent;
	s_serveroptions.ruleset.generic.id			= ID_RULESET;
	s_serveroptions.ruleset.generic.x			= OPTIONS_X;
	s_serveroptions.ruleset.generic.y			= y;
	s_serveroptions.ruleset.itemnames			= ruleset_items;

	if( s_serveroptions.gametype == GT_FASTCAP ) {
		y += BIGCHAR_HEIGHT+2;
		s_serveroptions.fastcapmode.generic.type		= MTYPE_SPINCONTROL;
		s_serveroptions.fastcapmode.generic.name		= "Fastcap Mode:";
		s_serveroptions.fastcapmode.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
		s_serveroptions.fastcapmode.generic.id			= ID_FASTCAPMODE;
		s_serveroptions.fastcapmode.generic.x			= OPTIONS_X;
		s_serveroptions.fastcapmode.generic.y			= y;
		s_serveroptions.fastcapmode.itemnames			= fastcap_items;
	}

	if( s_serveroptions.gametype == GT_DEFRAG ) {
		y += BIGCHAR_HEIGHT+2;
		s_serveroptions.defragmode.generic.type			= MTYPE_SPINCONTROL;
		s_serveroptions.defragmode.generic.name			= "Defrag Mode:";
		s_serveroptions.defragmode.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
		s_serveroptions.defragmode.generic.id			= ID_DEFRAGMODE;
		s_serveroptions.defragmode.generic.x			= OPTIONS_X;
		s_serveroptions.defragmode.generic.y			= y;
		s_serveroptions.defragmode.itemnames			= defrag_items;

		y += BIGCHAR_HEIGHT+2;
		s_serveroptions.reversemap.generic.type			= MTYPE_RADIOBUTTON;
		s_serveroptions.reversemap.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
		s_serveroptions.reversemap.generic.x			= OPTIONS_X;
		s_serveroptions.reversemap.generic.y			= y;
		s_serveroptions.reversemap.generic.name			= "Reverse Map:";
	}

	y += BIGCHAR_HEIGHT+2;
	s_serveroptions.overbounces.generic.type			= MTYPE_RADIOBUTTON;
	s_serveroptions.overbounces.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_serveroptions.overbounces.generic.x				= OPTIONS_X;
	s_serveroptions.overbounces.generic.y				= y;
	s_serveroptions.overbounces.generic.name			= "Enable Overbounces:";

	if ( s_serveroptions.gametype != GT_ADVENTURE ) {
		y += BIGCHAR_HEIGHT+2;
		s_serveroptions.cheats.generic.type				= MTYPE_RADIOBUTTON;
		s_serveroptions.cheats.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
		s_serveroptions.cheats.generic.x				= OPTIONS_X;
		s_serveroptions.cheats.generic.y				= y;
		s_serveroptions.cheats.generic.name				= "Enable Cheats:";
	}

	y += ( 2 * SMALLCHAR_HEIGHT );
	s_serveroptions.back.generic.type		= MTYPE_BITMAP;
	s_serveroptions.back.generic.name		= GAMESERVER_BACK0;
	s_serveroptions.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_serveroptions.back.generic.callback	= ServerOptions_Event;
	s_serveroptions.back.generic.id			= ID_BACK;
	s_serveroptions.back.generic.x			= 0;
	s_serveroptions.back.generic.y			= 480-64;
	s_serveroptions.back.width				= 128;
	s_serveroptions.back.height				= 64;
	s_serveroptions.back.focuspic			= GAMESERVER_BACK1;

	s_serveroptions.next.generic.type		= MTYPE_BITMAP;
	s_serveroptions.next.generic.name		= GAMESERVER_NEXT0;
	s_serveroptions.next.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_INACTIVE|QMF_GRAYED|QMF_HIDDEN;
	s_serveroptions.next.generic.callback	= ServerOptions_Event;
	s_serveroptions.next.generic.id			= ID_STARTSERVERNEXT;
	s_serveroptions.next.generic.x			= 640;
	s_serveroptions.next.generic.y			= 480-64-72;
	s_serveroptions.next.width				= 128;
	s_serveroptions.next.height				= 64;
	s_serveroptions.next.focuspic			= GAMESERVER_NEXT1;

	s_serveroptions.go.generic.type			= MTYPE_BITMAP;
	s_serveroptions.go.generic.name			= GAMESERVER_FIGHT0;
	s_serveroptions.go.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_serveroptions.go.generic.callback		= ServerOptions_Event;
	s_serveroptions.go.generic.id			= ID_GO;
	s_serveroptions.go.generic.x			= 640;
	s_serveroptions.go.generic.y			= 480-64;
	s_serveroptions.go.width				= 128;
	s_serveroptions.go.height				= 64;
	s_serveroptions.go.focuspic				= GAMESERVER_FIGHT1;

	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.frame );
	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.mappic );
	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.picframe );
	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.ruleset );
	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.overbounces );

	if ( s_serveroptions.gametype != GT_ADVENTURE )
		Menu_AddItem ( &s_serveroptions.menu, &s_serveroptions.cheats );

	if ( s_serveroptions.gametype == GT_FASTCAP) {
		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.fastcapmode );
	} else if ( s_serveroptions.gametype == GT_DEFRAG) {
		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.defragmode );
		Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.reversemap );
	}

	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.back );
	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.next );
	Menu_AddItem( &s_serveroptions.menu, &s_serveroptions.go );

	ServerOptions_SetMenuItems();
}

/*
=================
ServerOptions_Cache
=================
*/
void ServerOptions_Cache( void )
{
	trap_R_RegisterShaderNoMip( GAMESERVER_BACK0 );
	trap_R_RegisterShaderNoMip( GAMESERVER_BACK1 );
	trap_R_RegisterShaderNoMip( GAMESERVER_FIGHT0 );
	trap_R_RegisterShaderNoMip( GAMESERVER_FIGHT1 );
	trap_R_RegisterShaderNoMip( GAMESERVER_SELECT );
	trap_R_RegisterShaderNoMip( GAMESERVER_UNKNOWNMAP );
}


/*
=================
UI_ServerOptionsMenu
=================
*/
static void UI_ServerOptionsMenu( qbool multiplayer )
{
	ServerOptions_MenuInit( multiplayer );
	UI_PushMenu( &s_serveroptions.menu );
}
