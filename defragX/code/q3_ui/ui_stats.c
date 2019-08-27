//
// ui_stats.c
//

#include "ui_local.h"

#define ART_FRAME		"gfx/ui/frame"

#define ID_MAPNAME		100
#define ID_TOPTIME		101
#define ID_TOPSPEED		102

#define ID_CLOSE 		125

typedef struct {
	menuframework_s	menu;
	menubitmap_s	frame;
	
	menubitmap_s	levelshot;
	menutext_s		mapname;
		
	menutext_s		toptime;
	menutext_s		topspeed;
	
	menutext_s		close;
} stats_t;	

stats_t				s_stats;

static gametype_t 		gametype;
static ruleset_t		ruleset;
static defragmode_t		dfmode;
static fastcapmode_t	fcmode;

static int 	obs;
static int	reversemap;

/*
===============
Stats_MenuEvent
===============
*/
void Stats_MenuEvent( void* ptr, int event ) {

	if( event != QM_ACTIVATED ) 
		return;

	switch( ((menucommon_s*)ptr)->id ) {
			
	case ID_CLOSE:
		UI_PopMenu();
		break;
	}
}

/*
===============
Stats_MenuInit
===============
*/
void Stats_MenuInit( void ) {
	int 			x, y;
	char			info[BIG_INFO_STRING];
	char			*mapname [MAX_QPATH];
	static char		picname [MAX_QPATH];
	
	memset( &s_stats, 0, sizeof(s_stats) );
	
	trap_R_RegisterShaderNoMip( ART_FRAME );
	
	// parse our serverstring to check for current server variables states
	trap_GetConfigString( CS_SERVERINFO, info, sizeof( info ) );
	gametype 	= atoi( Info_ValueForKey( info, "dfx_gametype" ) );
	ruleset  	= atoi( Info_ValueForKey( info, "dfx_ruleset" ) );
	dfmode	 	= atoi( Info_ValueForKey( info, "dfx_mode" ) );
	fcmode	 	= atoi( Info_ValueForKey( info, "dfx_fastcapmode" ) );
	obs	 	 	= atoi( Info_ValueForKey( info, "dfx_obs" ) );
	reversemap  = atoi( Info_ValueForKey( info, "dfx_reversemap" ) );
	mapname[0]	= Info_ValueForKey( info, "mapname" );
	
	Com_sprintf( picname, sizeof( picname ), va("levelshots/%s.tga", mapname[0]) );
	trap_R_RegisterShaderNoMip( picname );
	
	// load the records file matching the above values
	//...
	
	s_stats.menu.wrapAround = qtrue;
	s_stats.menu.fullscreen = qfalse;
				
	s_stats.frame.generic.type			= MTYPE_BITMAP;
	s_stats.frame.generic.flags			= QMF_INACTIVE;
	s_stats.frame.generic.name			= ART_FRAME;
	s_stats.frame.width					= 520;
	s_stats.frame.height				= 286;
	s_stats.frame.generic.x				= (640-s_stats.frame.width)/2;
	s_stats.frame.generic.y				= (480-s_stats.frame.height)/2;
	
	y = s_stats.frame.generic.y + 12;
	x = 320;
	
	s_stats.mapname.generic.type		= MTYPE_PTEXT;
	s_stats.mapname.generic.flags		= QMF_CENTER_JUSTIFY;
	s_stats.mapname.generic.id			= ID_MAPNAME;
	s_stats.mapname.generic.callback	= Stats_MenuEvent;
	s_stats.mapname.generic.x			= x;
	s_stats.mapname.generic.y			= y;
	s_stats.mapname.string 				= mapname[0];
	s_stats.mapname.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_stats.mapname.color				= text_color_normal;
	
	s_stats.levelshot.generic.type		= MTYPE_BITMAP;
	s_stats.levelshot.generic.flags		= QMF_INACTIVE;
	s_stats.levelshot.generic.name 		= picname;
	s_stats.levelshot.width				= 160;
	s_stats.levelshot.height			= 120;
	s_stats.levelshot.generic.x			= s_stats.frame.generic.x + 32;
	s_stats.levelshot.generic.y			= s_stats.frame.generic.y + 48;
	s_stats.levelshot.errorpic			= "gfx/ui/unknownmap.tga";
	
	s_stats.close.generic.type			= MTYPE_PTEXT;
	s_stats.close.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_stats.close.generic.id			= ID_CLOSE;
	s_stats.close.generic.callback		= Stats_MenuEvent;
	s_stats.close.generic.x				= 310;
	s_stats.close.generic.y				= 348;
	s_stats.close.string				= "CLOSE";
	s_stats.close.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_stats.close.color					= text_color_normal;
	
	Menu_AddItem( &s_stats.menu, (void*) &s_stats.frame );
	Menu_AddItem( &s_stats.menu, (void*) &s_stats.mapname );
	Menu_AddItem( &s_stats.menu, (void*) &s_stats.levelshot );
	Menu_AddItem( &s_stats.menu, (void*) &s_stats.close );
}

/*
========================
UI_StatsMenu
=======================
*/
void UI_StatsMenu( void ) {
	Stats_MenuInit();
	UI_PushMenu( &s_stats.menu );
}
