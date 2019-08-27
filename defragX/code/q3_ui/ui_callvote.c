//
// ui_callvote.c
//

#include "ui_local.h"

void UI_CallvoteGametypeMenu( void );
void UI_CallvoteRulesetMenu( void );
void UI_CallvoteDfmodeMenu( void );
void UI_CallvoteFcmodeMenu( void );
void UI_CallvoteMapMenu( void );
void UI_CallvoteKickMenu( void );

#define ART_FRAME		"gfx/ui/frame"
#define CV_GAMETYPE		101
#define CV_RULESET		102
#define CV_DFMODE		103
#define CV_FCMODE		104
#define CV_OBS			105
#define CV_INTERFERENCE 106
#define CV_REVERSEMAP	107
#define CV_MAP			108
//#define CV_MAPRESTART	109
#define CV_CLIENTKICK	110
#define CV_CANCEL		125

typedef struct
{
	menuframework_s	menu;
	menubitmap_s	frame;
	
	menutext_s		gametype;
	menutext_s		ruleset;
	menutext_s		dfmode;
	menutext_s		fcmode;
	menutext_s		obs;
	menutext_s		interference;
	menutext_s		reversemap;
	menutext_s		map;
	//menutext_s		maprestart;
	menutext_s		clientkick;
	
	menutext_s		cancel;
} cv_t;

cv_t				s_callvote;

static gametype_t 		gametype;
static ruleset_t		ruleset;
static defragmode_t		dfmode;
static fastcapmode_t	fcmode;

static int 	obs, interference, reversemap;
int 	y;
char	info[BIG_INFO_STRING];

/*
===============
Callvote_MenuEvent
===============
*/
void Callvote_MenuEvent( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) 
		return;

	switch( ((menucommon_s*)ptr)->id ) {
	case CV_GAMETYPE:
		UI_CallvoteGametypeMenu();
		break;
		
	case CV_RULESET:
		UI_CallvoteRulesetMenu();
		break;
		
	case CV_DFMODE:
		UI_CallvoteDfmodeMenu();
		break;
		
	case CV_FCMODE:
		UI_CallvoteFcmodeMenu();
		break;
		
	case CV_OBS:
		if (!obs) {
			trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_obs 1\n" );
		} else {
			trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_obs 0\n" );
		}
		UI_ForceMenuOff();
		break;
		
	case CV_INTERFERENCE:
		if (!interference) {
			trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_interference 1\n" );
		} else {
			trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_interference 0\n" );
		}
		UI_ForceMenuOff();
		break;
		
	case CV_REVERSEMAP:
		if (!reversemap) {
			trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_reversemap 1\n" );
		} else {
			trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_reversemap 0\n" );
		}
		UI_ForceMenuOff();
		break;
		
	case CV_MAP:
		UI_CallvoteMapMenu();
		break;
		
	/*case CV_MAPRESTART:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv map_restart\n" );
		UI_ForceMenuOff();
		break;*/
	
	case CV_CLIENTKICK:
		UI_CallvoteKickMenu();
		//UI_ForceMenuOff();
		break;	
		
	case CV_CANCEL:
		UI_PopMenu();
		break;
	}
}

/*
===============
Callvote_MenuInit
===============
*/
void Callvote_MenuInit( void ) {
	memset( &s_callvote, 0, sizeof(s_callvote) );

	trap_R_RegisterShaderNoMip( ART_FRAME );
	
	// parse our serverstring to check for current server variables states
	trap_GetConfigString( CS_SERVERINFO, info, sizeof( info ) );
	gametype = atoi( Info_ValueForKey( info, "dfx_gametype" ) );
	ruleset  = atoi( Info_ValueForKey( info, "dfx_ruleset" ) );
	dfmode	 = atoi( Info_ValueForKey( info, "dfx_mode" ) );
	fcmode	 = atoi( Info_ValueForKey( info, "dfx_fastcapmode" ) );

	s_callvote.menu.wrapAround = qtrue;
	s_callvote.menu.fullscreen = qfalse;

	s_callvote.frame.generic.type			= MTYPE_BITMAP;
	s_callvote.frame.generic.flags			= QMF_INACTIVE;
	s_callvote.frame.generic.name			= ART_FRAME;
	s_callvote.frame.width					= 300;
	s_callvote.frame.height					= 286;
	s_callvote.frame.generic.x				= (640-s_callvote.frame.width)/2;
	s_callvote.frame.generic.y				= (480-s_callvote.frame.height)/2;

	y = 124;
	s_callvote.gametype.generic.type			= MTYPE_PTEXT;
	s_callvote.gametype.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_callvote.gametype.generic.id				= CV_GAMETYPE;
	s_callvote.gametype.generic.callback		= Callvote_MenuEvent;
	s_callvote.gametype.generic.x				= 320;
	s_callvote.gametype.generic.y				= y;
	s_callvote.gametype.string					= "GAMETYPE";
	s_callvote.gametype.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_callvote.gametype.color					= text_color_normal;

	y += 23;
	s_callvote.ruleset.generic.type				= MTYPE_PTEXT;
	s_callvote.ruleset.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_callvote.ruleset.generic.id				= CV_RULESET;
	s_callvote.ruleset.generic.callback			= Callvote_MenuEvent;
	s_callvote.ruleset.generic.x				= 320;
	s_callvote.ruleset.generic.y				= y;
	s_callvote.ruleset.string					= "RULESET";
	s_callvote.ruleset.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_callvote.ruleset.color					= text_color_normal;
	
	y += 23;
	s_callvote.dfmode.generic.type				= MTYPE_PTEXT;
	s_callvote.dfmode.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_callvote.dfmode.generic.id				= CV_DFMODE;
	s_callvote.dfmode.generic.callback			= Callvote_MenuEvent;
	s_callvote.dfmode.generic.x					= 320;
	s_callvote.dfmode.generic.y					= y;
	s_callvote.dfmode.string					= "DEFRAG MODE";
	s_callvote.dfmode.style						= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_callvote.dfmode.color						= text_color_normal;
	if( gametype != GT_DEFRAG )
        s_callvote.dfmode.generic.flags |= QMF_GRAYED;
	
	y += 23;
	s_callvote.fcmode.generic.type				= MTYPE_PTEXT;
	s_callvote.fcmode.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_callvote.fcmode.generic.id				= CV_FCMODE;
	s_callvote.fcmode.generic.callback			= Callvote_MenuEvent;
	s_callvote.fcmode.generic.x					= 320;
	s_callvote.fcmode.generic.y					= y;
	s_callvote.fcmode.string					= "FASTCAP MODE";
	s_callvote.fcmode.style						= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_callvote.fcmode.color						= text_color_normal;
	if( gametype != GT_FASTCAP )
        s_callvote.fcmode.generic.flags |= QMF_GRAYED;

	y += 23;
	s_callvote.obs.generic.type				= MTYPE_PTEXT;
	s_callvote.obs.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_callvote.obs.generic.id				= CV_OBS;
	s_callvote.obs.generic.callback			= Callvote_MenuEvent;
	s_callvote.obs.generic.x				= 320;
	s_callvote.obs.generic.y				= y;
		obs = atoi( Info_ValueForKey( info, "dfx_obs" ) );
		if (!obs) {
			s_callvote.obs.string					= "ENABLE OVERBOUNCES";
		} else {
			s_callvote.obs.string					= "DISABLE OVERBOUNCES";
		}
	s_callvote.obs.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_callvote.obs.color					= text_color_normal;
	
	y += 23;
	s_callvote.interference.generic.type			= MTYPE_PTEXT;
	s_callvote.interference.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_callvote.interference.generic.id				= CV_INTERFERENCE;
	s_callvote.interference.generic.callback		= Callvote_MenuEvent;
	s_callvote.interference.generic.x				= 320;
	s_callvote.interference.generic.y				= y;
		interference = atoi( Info_ValueForKey( info, "dfx_interference" ) );
		if (!interference) {
			s_callvote.interference.string			= "ENABLE INTERFERENCE";
		} else {
			s_callvote.interference.string			= "DISABLE INTERFERENCE";
		}
	s_callvote.interference.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_callvote.interference.color					= text_color_normal;
	// interference only voteable in GT_FREESTYLE
	if( gametype != GT_FREESTYLE )
        s_callvote.interference.generic.flags |= QMF_GRAYED;
		
	y += 23;
	s_callvote.reversemap.generic.type				= MTYPE_PTEXT;
	s_callvote.reversemap.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_callvote.reversemap.generic.id				= CV_REVERSEMAP;
	s_callvote.reversemap.generic.callback			= Callvote_MenuEvent;
	s_callvote.reversemap.generic.x					= 320;
	s_callvote.reversemap.generic.y					= y;
		reversemap = atoi( Info_ValueForKey( info, "dfx_reversemap" ) );
		if (!reversemap) {
			s_callvote.reversemap.string			= "PLAY MAP BACKWARDS";
		} else {
			s_callvote.reversemap.string			= "PLAY MAP NORMAL";
		}
	s_callvote.reversemap.style						= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_callvote.reversemap.color						= text_color_normal;
	// reversemap only voteable in GT_DEFRAG
	if( gametype != GT_DEFRAG )
        s_callvote.reversemap.generic.flags |= QMF_GRAYED;
	
	y += 23;
	s_callvote.map.generic.type					= MTYPE_PTEXT;
	s_callvote.map.generic.flags				= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_callvote.map.generic.id					= CV_MAP;
	s_callvote.map.generic.callback				= Callvote_MenuEvent;
	s_callvote.map.generic.x					= 320;
	s_callvote.map.generic.y					= y;
	s_callvote.map.string						= "CHANGE MAP";
	s_callvote.map.style						= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_callvote.map.color						= text_color_normal;
	
	/*
	y += 23;
	s_callvote.maprestart.generic.type			= MTYPE_PTEXT;
	s_callvote.maprestart.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_callvote.maprestart.generic.id			= CV_MAPRESTART;
	s_callvote.maprestart.generic.callback		= Callvote_MenuEvent;
	s_callvote.maprestart.generic.x				= 320;
	s_callvote.maprestart.generic.y				= y;
	s_callvote.maprestart.string				= "MAP RESTART";
	s_callvote.maprestart.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_callvote.maprestart.color					= text_color_normal;
	*/
	
	y += 23;
	s_callvote.clientkick.generic.type		= MTYPE_PTEXT;
	s_callvote.clientkick.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_callvote.clientkick.generic.id		= CV_CLIENTKICK;
	s_callvote.clientkick.generic.callback	= Callvote_MenuEvent;
	s_callvote.clientkick.generic.x			= 320;
	s_callvote.clientkick.generic.y			= y;
	s_callvote.clientkick.string			= "KICK Player";
	s_callvote.clientkick.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_callvote.clientkick.color				= text_color_normal;
	
	y += 46;
	s_callvote.cancel.generic.type			= MTYPE_PTEXT;
	s_callvote.cancel.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_callvote.cancel.generic.id			= CV_CANCEL;
	s_callvote.cancel.generic.callback		= Callvote_MenuEvent;
	s_callvote.cancel.generic.x				= 320;
	s_callvote.cancel.generic.y				= y;
	s_callvote.cancel.string				= "CANCEL";
	s_callvote.cancel.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_callvote.cancel.color					= text_color_normal;

	Menu_AddItem( &s_callvote.menu, (void*) &s_callvote.frame );
	
	Menu_AddItem( &s_callvote.menu, (void*) &s_callvote.gametype );
	Menu_AddItem( &s_callvote.menu, (void*) &s_callvote.ruleset );
	Menu_AddItem( &s_callvote.menu, (void*) &s_callvote.dfmode );
	Menu_AddItem( &s_callvote.menu, (void*) &s_callvote.fcmode );
	Menu_AddItem( &s_callvote.menu, (void*) &s_callvote.obs );
	Menu_AddItem( &s_callvote.menu, (void*) &s_callvote.interference );
	Menu_AddItem( &s_callvote.menu, (void*) &s_callvote.reversemap );
	Menu_AddItem( &s_callvote.menu, (void*) &s_callvote.map );
	//Menu_AddItem( &s_callvote.menu, (void*) &s_callvote.maprestart );
	Menu_AddItem( &s_callvote.menu, (void*) &s_callvote.clientkick );
	
	Menu_AddItem( &s_callvote.menu, (void*) &s_callvote.cancel );
}

/*
===============
UI_CallvoteMenu
===============
*/
void UI_CallvoteMenu( void ) {
	Callvote_MenuInit();
	UI_PushMenu ( &s_callvote.menu );
}


/*
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
CALLVOTE GAMETYPE UI
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
#define CV_GT_ADVENTURE	201
#define CV_GT_FASTCAP	202
#define CV_GT_DEFRAG	203
#define CV_GT_TEAMRUN	204
#define	CV_GT_FREESTYLE	205
#define CV_GT_CANCEL	225

typedef struct
{
	menuframework_s	menu;
	menubitmap_s	frame;
	
	menutext_s		adventure;
	menutext_s		fastcap;
	menutext_s		defrag;
	menutext_s		teamrun;
	menutext_s		freestyle;
	
	menutext_s		cancel;
} cv_gametype_t;

cv_gametype_t s_gametype;

/*
===============
Gametype_MenuEvent
===============
*/
void Gametype_MenuEvent( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) 
		return;

	switch( ((menucommon_s*)ptr)->id ) {
	case CV_GT_FASTCAP:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_gametype 0\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_GT_ADVENTURE:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_gametype 1\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_GT_DEFRAG:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_gametype 2\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_GT_TEAMRUN:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_gametype 3\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_GT_FREESTYLE:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_gametype 4\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_GT_CANCEL:
		UI_PopMenu();
		break;
	}
}


/*
=====================
Cv_Gametype_MenuInit
=====================
*/
void Cv_Gametype_MenuInit( void ) {
	memset( &s_gametype, 0, sizeof(s_gametype) );
	
	s_gametype.menu.wrapAround = qtrue;
	s_gametype.menu.fullscreen = qfalse;

	s_gametype.frame.generic.type			= MTYPE_BITMAP;
	s_gametype.frame.generic.flags			= QMF_INACTIVE;
	s_gametype.frame.generic.name			= ART_FRAME;
	s_gametype.frame.width					= 300;
	s_gametype.frame.height					= 264;
	s_gametype.frame.generic.x				= (640-s_gametype.frame.width)/2;
	s_gametype.frame.generic.y				= (480-s_gametype.frame.height)/2;

	y = 132;
	s_gametype.fastcap.generic.type			= MTYPE_PTEXT;
	s_gametype.fastcap.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_gametype.fastcap.generic.id			= CV_GT_FASTCAP;
	s_gametype.fastcap.generic.callback		= Gametype_MenuEvent;
	s_gametype.fastcap.generic.x			= 320;
	s_gametype.fastcap.generic.y			= y;
	s_gametype.fastcap.string				= "FASTCAP";
	s_gametype.fastcap.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_gametype.fastcap.color				= text_color_normal;
	if( gametype == GT_FASTCAP )
        s_gametype.fastcap.generic.flags |= QMF_GRAYED;
	
	y += 23;
	s_gametype.adventure.generic.type		= MTYPE_PTEXT;
	s_gametype.adventure.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_gametype.adventure.generic.id			= CV_GT_ADVENTURE;
	s_gametype.adventure.generic.callback	= Gametype_MenuEvent;
	s_gametype.adventure.generic.x			= 320;
	s_gametype.adventure.generic.y			= y;
	s_gametype.adventure.string				= "ADVENTURE";
	s_gametype.adventure.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_gametype.adventure.color				= text_color_normal;
	if( gametype == GT_ADVENTURE )
        s_gametype.adventure.generic.flags |= QMF_GRAYED;
	
	y += 23;
	s_gametype.defrag.generic.type			= MTYPE_PTEXT;
	s_gametype.defrag.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_gametype.defrag.generic.id			= CV_GT_DEFRAG;
	s_gametype.defrag.generic.callback		= Gametype_MenuEvent;
	s_gametype.defrag.generic.x				= 320;
	s_gametype.defrag.generic.y				= y;
	s_gametype.defrag.string				= "DEFRAG";
	s_gametype.defrag.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_gametype.defrag.color					= text_color_normal;
	if( gametype == GT_DEFRAG )
        s_gametype.defrag.generic.flags |= QMF_GRAYED;	

	y += 23;
	s_gametype.teamrun.generic.type			= MTYPE_PTEXT;
	s_gametype.teamrun.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_gametype.teamrun.generic.id			= CV_GT_TEAMRUN;
	s_gametype.teamrun.generic.callback		= Gametype_MenuEvent;
	s_gametype.teamrun.generic.x			= 320;
	s_gametype.teamrun.generic.y			= y;
	s_gametype.teamrun.string				= "TEAMRUN";
	s_gametype.teamrun.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_gametype.teamrun.color				= text_color_normal;
	if( gametype == GT_TEAMRUN )
        s_gametype.teamrun.generic.flags |= QMF_GRAYED;
		
	y += 23;
	s_gametype.freestyle.generic.type		= MTYPE_PTEXT;
	s_gametype.freestyle.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_gametype.freestyle.generic.id			= CV_GT_FREESTYLE;
	s_gametype.freestyle.generic.callback	= Gametype_MenuEvent;
	s_gametype.freestyle.generic.x			= 320;
	s_gametype.freestyle.generic.y			= y;
	s_gametype.freestyle.string				= "FREESTYLE";
	s_gametype.freestyle.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_gametype.freestyle.color				= text_color_normal;
	if( gametype == GT_FREESTYLE )
        s_gametype.freestyle.generic.flags |= QMF_GRAYED;

	y += 46;
	s_gametype.cancel.generic.type			= MTYPE_PTEXT;
	s_gametype.cancel.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_gametype.cancel.generic.id			= CV_GT_CANCEL;
	s_gametype.cancel.generic.callback		= Gametype_MenuEvent;
	s_gametype.cancel.generic.x				= 320;
	s_gametype.cancel.generic.y				= y;
	s_gametype.cancel.string				= "CANCEL";
	s_gametype.cancel.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_gametype.cancel.color					= text_color_normal;

	Menu_AddItem( &s_gametype.menu, (void*) &s_gametype.frame );
	
	Menu_AddItem( &s_gametype.menu, (void*) &s_gametype.adventure );
	Menu_AddItem( &s_gametype.menu, (void*) &s_gametype.fastcap );
	Menu_AddItem( &s_gametype.menu, (void*) &s_gametype.defrag );
	Menu_AddItem( &s_gametype.menu, (void*) &s_gametype.teamrun );
	Menu_AddItem( &s_gametype.menu, (void*) &s_gametype.freestyle );
	
	Menu_AddItem( &s_gametype.menu, (void*) &s_gametype.cancel );
}

/*
========================
UI_CallvoteGametypeMenu
=======================
*/
void UI_CallvoteGametypeMenu( void ) {
	Cv_Gametype_MenuInit();
	UI_PushMenu( &s_gametype.menu );
}


/*
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
CALLVOTE RULESET UI
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
#define CV_RS_VANILLA  	301
#define CV_RS_CPM		302
#define CV_RS_XVANILLA	303
#define CV_RS_XCPM		304
#define CV_RS_CANCEL	325

typedef struct
{
	menuframework_s	menu;
	menubitmap_s	frame;
	
	menutext_s		vanilla;
	menutext_s		cpm;
	menutext_s		xvanilla;
	menutext_s		xcpm;
	
	menutext_s		cancel;
} cv_ruleset_t;

cv_ruleset_t s_ruleset;

/*
===============
Ruleset_MenuEvent
===============
*/
void Ruleset_MenuEvent( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) 
		return;

	switch( ((menucommon_s*)ptr)->id ) {
	case CV_RS_VANILLA:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_ruleset 0\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_RS_CPM:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_ruleset 1\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_RS_XVANILLA:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_ruleset 2\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_RS_XCPM:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_ruleset 3\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_RS_CANCEL:
		UI_PopMenu();
		break;
	}
}


/*
=====================
Cv_Ruleset_MenuInit
=====================
*/
void Cv_Ruleset_MenuInit( void ) {
	memset( &s_ruleset, 0, sizeof(s_ruleset) );
	
	s_ruleset.menu.wrapAround = qtrue;
	s_ruleset.menu.fullscreen = qfalse;

	s_ruleset.frame.generic.type			= MTYPE_BITMAP;
	s_ruleset.frame.generic.flags			= QMF_INACTIVE;
	s_ruleset.frame.generic.name			= ART_FRAME;
	s_ruleset.frame.width					= 300;
	s_ruleset.frame.height					= 264;
	s_ruleset.frame.generic.x				= (640-s_ruleset.frame.width)/2;
	s_ruleset.frame.generic.y				= (480-s_ruleset.frame.height)/2;

	y = 132;
	s_ruleset.vanilla.generic.type			= MTYPE_PTEXT;
	s_ruleset.vanilla.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ruleset.vanilla.generic.id			= CV_RS_VANILLA;
	s_ruleset.vanilla.generic.callback		= Ruleset_MenuEvent;
	s_ruleset.vanilla.generic.x				= 320;
	s_ruleset.vanilla.generic.y				= y;
	s_ruleset.vanilla.string				= "VQ3";
	s_ruleset.vanilla.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_ruleset.vanilla.color					= text_color_normal;
	if( ruleset == RS_VANILLA )
        s_ruleset.vanilla.generic.flags |= QMF_GRAYED;
	
	y += 23;
	s_ruleset.cpm.generic.type			= MTYPE_PTEXT;
	s_ruleset.cpm.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ruleset.cpm.generic.id			= CV_RS_CPM;
	s_ruleset.cpm.generic.callback		= Ruleset_MenuEvent;
	s_ruleset.cpm.generic.x				= 320;
	s_ruleset.cpm.generic.y				= y;
	s_ruleset.cpm.string				= "CPM";
	s_ruleset.cpm.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_ruleset.cpm.color					= text_color_normal;
	if( ruleset == RS_CPM )
        s_ruleset.cpm.generic.flags |= QMF_GRAYED;
		
	y += 23;
	s_ruleset.xvanilla.generic.type			= MTYPE_PTEXT;
	s_ruleset.xvanilla.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ruleset.xvanilla.generic.id			= CV_RS_XVANILLA;
	s_ruleset.xvanilla.generic.callback		= Ruleset_MenuEvent;
	s_ruleset.xvanilla.generic.x			= 320;
	s_ruleset.xvanilla.generic.y			= y;
	s_ruleset.xvanilla.string				= "XVQ3";
	s_ruleset.xvanilla.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_ruleset.xvanilla.color				= text_color_normal;
	if( ruleset == RS_XVANILLA )
        s_ruleset.xvanilla.generic.flags |= QMF_GRAYED;
		
	y += 23;
	s_ruleset.xcpm.generic.type			= MTYPE_PTEXT;
	s_ruleset.xcpm.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ruleset.xcpm.generic.id			= CV_RS_XCPM;
	s_ruleset.xcpm.generic.callback		= Ruleset_MenuEvent;
	s_ruleset.xcpm.generic.x			= 320;
	s_ruleset.xcpm.generic.y			= y;
	s_ruleset.xcpm.string				= "XCPM";
	s_ruleset.xcpm.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_ruleset.xcpm.color				= text_color_normal;
	if( ruleset == RS_XCPM )
        s_ruleset.xcpm.generic.flags |= QMF_GRAYED;

	y += 46;
	s_ruleset.cancel.generic.type			= MTYPE_PTEXT;
	s_ruleset.cancel.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_ruleset.cancel.generic.id				= CV_RS_CANCEL;
	s_ruleset.cancel.generic.callback		= Ruleset_MenuEvent;
	s_ruleset.cancel.generic.x				= 320;
	s_ruleset.cancel.generic.y				= y;
	s_ruleset.cancel.string					= "CANCEL";
	s_ruleset.cancel.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_ruleset.cancel.color					= text_color_normal;

	Menu_AddItem( &s_ruleset.menu, (void*) &s_ruleset.frame );
	
	Menu_AddItem( &s_ruleset.menu, (void*) &s_ruleset.vanilla );
	Menu_AddItem( &s_ruleset.menu, (void*) &s_ruleset.cpm );
	Menu_AddItem( &s_ruleset.menu, (void*) &s_ruleset.xvanilla );
	Menu_AddItem( &s_ruleset.menu, (void*) &s_ruleset.xcpm );
	
	Menu_AddItem( &s_ruleset.menu, (void*) &s_ruleset.cancel );
}

/*
========================
UI_CallvoteRulesetMenu
=======================
*/
void UI_CallvoteRulesetMenu( void ) {
	Cv_Ruleset_MenuInit();
	UI_PushMenu( &s_ruleset.menu );
}



/*
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
CALLVOTE DEFRAG MODE UI
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
#define CV_DFMODE_DEFAULT 401
#define CV_DFMODE_STRAFE  402
#define CV_DFMODE_ROCKET  403
#define CV_DFMODE_PLASMA  404
#define CV_DFMODE_COMBO   405
#define CV_DFMODE_ICE	  406
#define CV_DFMODE_CANCEL  425

typedef struct
{
	menuframework_s	menu;
	menubitmap_s	frame;
	
	menutext_s		mdefault;
	menutext_s		mstrafe;
	menutext_s		mrocket;
	menutext_s		mplasma;
	menutext_s		mcombo;
	menutext_s		mice;
	
	menutext_s		cancel;
} cv_dfmode_t;

cv_dfmode_t s_dfmode;

/*
===============
Dfmode_MenuEvent
===============
*/
void Dfmode_MenuEvent( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) 
		return;

	switch( ((menucommon_s*)ptr)->id ) {
	case CV_DFMODE_DEFAULT:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_mode 0\n" );
		UI_ForceMenuOff();
		break;
	
	case CV_DFMODE_STRAFE:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_mode 1\n" );
		UI_ForceMenuOff();
		break;			
	
	case CV_DFMODE_ROCKET:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_mode 2\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_DFMODE_PLASMA:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_mode 3\n" );
		UI_ForceMenuOff();
		break;
	
	case CV_DFMODE_COMBO:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_mode 4\n" );
		UI_ForceMenuOff();
		break;
	
	case CV_DFMODE_ICE:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_mode 5\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_DFMODE_CANCEL:
		UI_PopMenu();
		break;
	}
}


/*
===============
Cv_Dfmode_MenuInit
===============
*/
void Cv_Dfmode_MenuInit( void ) {
	memset( &s_dfmode, 0, sizeof(s_dfmode) );
	
	s_dfmode.menu.wrapAround = qtrue;
	s_dfmode.menu.fullscreen = qfalse;

	s_dfmode.frame.generic.type				= MTYPE_BITMAP;
	s_dfmode.frame.generic.flags			= QMF_INACTIVE;
	s_dfmode.frame.generic.name				= ART_FRAME;
	s_dfmode.frame.width					= 300;
	s_dfmode.frame.height					= 264;
	s_dfmode.frame.generic.x				= (640-s_dfmode.frame.width)/2;
	s_dfmode.frame.generic.y				= (480-s_dfmode.frame.height)/2;

	y = 132;
	s_dfmode.mdefault.generic.type			= MTYPE_PTEXT;
	s_dfmode.mdefault.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_dfmode.mdefault.generic.id			= CV_DFMODE_DEFAULT;
	s_dfmode.mdefault.generic.callback		= Dfmode_MenuEvent;
	s_dfmode.mdefault.generic.x				= 320;
	s_dfmode.mdefault.generic.y				= y;
	s_dfmode.mdefault.string					= "DEFAULT";
	s_dfmode.mdefault.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_dfmode.mdefault.color					= text_color_normal;
	if( dfmode == DF_DEFAULT )
        s_dfmode.mdefault.generic.flags |= QMF_GRAYED;
	
	y += 23;
	s_dfmode.mstrafe.generic.type			= MTYPE_PTEXT;
	s_dfmode.mstrafe.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_dfmode.mstrafe.generic.id				= CV_DFMODE_STRAFE;
	s_dfmode.mstrafe.generic.callback		= Dfmode_MenuEvent;
	s_dfmode.mstrafe.generic.x				= 320;
	s_dfmode.mstrafe.generic.y				= y;
	s_dfmode.mstrafe.string					= "STRAFE ONLY";
	s_dfmode.mstrafe.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_dfmode.mstrafe.color					= text_color_normal;
	if( dfmode == DF_STRAFE )
        s_dfmode.mstrafe.generic.flags |= QMF_GRAYED;
		
	y += 23;
	s_dfmode.mrocket.generic.type			= MTYPE_PTEXT;
	s_dfmode.mrocket.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_dfmode.mrocket.generic.id				= CV_DFMODE_ROCKET;
	s_dfmode.mrocket.generic.callback		= Dfmode_MenuEvent;
	s_dfmode.mrocket.generic.x				= 320;
	s_dfmode.mrocket.generic.y				= y;
	s_dfmode.mrocket.string					= "ROCKET LAUNCHER";
	s_dfmode.mrocket.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_dfmode.mrocket.color					= text_color_normal;
	if( dfmode == DF_ROCKET )
        s_dfmode.mrocket.generic.flags |= QMF_GRAYED;

	y += 23;
	s_dfmode.mplasma.generic.type			= MTYPE_PTEXT;
	s_dfmode.mplasma.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_dfmode.mplasma.generic.id				= CV_DFMODE_PLASMA;
	s_dfmode.mplasma.generic.callback		= Dfmode_MenuEvent;
	s_dfmode.mplasma.generic.x				= 320;
	s_dfmode.mplasma.generic.y				= y;
	s_dfmode.mplasma.string					= "PLASMA GUN";
	s_dfmode.mplasma.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_dfmode.mplasma.color					= text_color_normal;
	if( dfmode == DF_PLASMA )
        s_dfmode.mplasma.generic.flags |= QMF_GRAYED;

	y += 23;
	s_dfmode.mcombo.generic.type			= MTYPE_PTEXT;
	s_dfmode.mcombo.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_dfmode.mcombo.generic.id				= CV_DFMODE_COMBO;
	s_dfmode.mcombo.generic.callback		= Dfmode_MenuEvent;
	s_dfmode.mcombo.generic.x				= 320;
	s_dfmode.mcombo.generic.y				= y;
	s_dfmode.mcombo.string					= "WEAPON COMBO";
	s_dfmode.mcombo.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_dfmode.mcombo.color					= text_color_normal;
	if( dfmode == DF_COMBO )
        s_dfmode.mcombo.generic.flags |= QMF_GRAYED;
	
	y += 23;
	s_dfmode.mice.generic.type			= MTYPE_PTEXT;
	s_dfmode.mice.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_dfmode.mice.generic.id			= CV_DFMODE_ICE;
	s_dfmode.mice.generic.callback		= Dfmode_MenuEvent;
	s_dfmode.mice.generic.x				= 320;
	s_dfmode.mice.generic.y				= y;
	s_dfmode.mice.string				= "ICE MODE";
	s_dfmode.mice.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_dfmode.mice.color					= text_color_normal;
	if( dfmode == DF_ICEMODE )
        s_dfmode.mice.generic.flags |= QMF_GRAYED;

	y += 46;
	s_dfmode.cancel.generic.type			= MTYPE_PTEXT;
	s_dfmode.cancel.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_dfmode.cancel.generic.id				= CV_DFMODE_CANCEL;
	s_dfmode.cancel.generic.callback		= Dfmode_MenuEvent;
	s_dfmode.cancel.generic.x				= 320;
	s_dfmode.cancel.generic.y				= y;
	s_dfmode.cancel.string					= "CANCEL";
	s_dfmode.cancel.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_dfmode.cancel.color					= text_color_normal;

	Menu_AddItem( &s_dfmode.menu, (void*) &s_dfmode.frame );
	
	Menu_AddItem( &s_dfmode.menu, (void*) &s_dfmode.mdefault );
	Menu_AddItem( &s_dfmode.menu, (void*) &s_dfmode.mstrafe );
	Menu_AddItem( &s_dfmode.menu, (void*) &s_dfmode.mrocket );
	Menu_AddItem( &s_dfmode.menu, (void*) &s_dfmode.mplasma );
	Menu_AddItem( &s_dfmode.menu, (void*) &s_dfmode.mcombo );
	Menu_AddItem( &s_dfmode.menu, (void*) &s_dfmode.mice );
	
	Menu_AddItem( &s_dfmode.menu, (void*) &s_dfmode.cancel );
}

/*
========================
UI_CallvoteDfmodeMenu
=======================
*/
void UI_CallvoteDfmodeMenu( void ) {
	Cv_Dfmode_MenuInit();
	UI_PushMenu( &s_dfmode.menu );
}



/*
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
CALLVOTE FASTCAP MODE UI
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
#define CV_FCMODE_DEFAULT 		501
#define CV_FCMODE_NOTELES 		502
#define CV_FCMODE_NOPADS  		503
#define CV_FCMODE_NODOORS  		504
#define CV_FCMODE_BOTH   		505
#define CV_FCMODE_DISABLEALL	506
#define CV_FCMODE_ROCKET		507
#define CV_FCMODE_PLASMA		508
#define CV_FCMODE_COMBO			509
#define CV_FCMODE_CANCEL  		525

typedef struct
{
	menuframework_s	menu;
	menubitmap_s	frame;
	
	menutext_s		fcdefault;
	menutext_s		fcnoteles;
	menutext_s		fcnopads;
	menutext_s		fcnodoors;
	menutext_s		fcboth;
	menutext_s		fcdisableall;
	menutext_s		fcrocket;
	menutext_s		fcplasma;
	menutext_s		fccombo;
	
	menutext_s		cancel;
} cv_fcmode_t;

cv_fcmode_t s_fcmode;

/*
===============
Fcmode_MenuEvent
===============
*/
void Fcmode_MenuEvent( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) 
		return;

	switch( ((menucommon_s*)ptr)->id ) {
	case CV_FCMODE_DEFAULT:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_fastcapmode 0\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_FCMODE_NOTELES:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_fastcapmode 1\n" );
		UI_ForceMenuOff();
		break;
	
	case CV_FCMODE_NOPADS:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_fastcapmode 2\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_FCMODE_NODOORS:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_fastcapmode 3\n" );
		UI_ForceMenuOff();
		break;
	
	case CV_FCMODE_BOTH:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_fastcapmode 4\n" );
		UI_ForceMenuOff();
		break;
	
	case CV_FCMODE_DISABLEALL:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_fastcapmode 5\n" );
		UI_ForceMenuOff();
		break;
	
	case CV_FCMODE_ROCKET:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_fastcapmode 6\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_FCMODE_PLASMA:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_fastcapmode 7\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_FCMODE_COMBO:
		trap_Cmd_ExecuteText( EXEC_APPEND, "cv dfx_fastcapmode 8\n" );
		UI_ForceMenuOff();
		break;
		
	case CV_FCMODE_CANCEL:
		UI_PopMenu();
		break;
	}
}


/*
===============
Cv_Fcmode_MenuInit
===============
*/
void Cv_Fcmode_MenuInit( void ) {
	memset( &s_fcmode, 0, sizeof(s_fcmode) );
	
	s_fcmode.menu.wrapAround = qtrue;
	s_fcmode.menu.fullscreen = qfalse;

	s_fcmode.frame.generic.type				= MTYPE_BITMAP;
	s_fcmode.frame.generic.flags			= QMF_INACTIVE;
	s_fcmode.frame.generic.name				= ART_FRAME;
	s_fcmode.frame.width					= 300;
	s_fcmode.frame.height					= 286;
	s_fcmode.frame.generic.x				= (640-s_fcmode.frame.width)/2;
	s_fcmode.frame.generic.y				= (480-s_fcmode.frame.height)/2;

	y = 124;
	s_fcmode.fcdefault.generic.type			= MTYPE_PTEXT;
	s_fcmode.fcdefault.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_fcmode.fcdefault.generic.id			= CV_FCMODE_DEFAULT;
	s_fcmode.fcdefault.generic.callback		= Fcmode_MenuEvent;
	s_fcmode.fcdefault.generic.x			= 320;
	s_fcmode.fcdefault.generic.y			= y;
	s_fcmode.fcdefault.string				= "DEFAULT";
	s_fcmode.fcdefault.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_fcmode.fcdefault.color				= text_color_normal;
	if( fcmode == FC_DEFAULT )
        s_fcmode.fcdefault.generic.flags |= QMF_GRAYED;
	
	y += 23;
	s_fcmode.fcnoteles.generic.type			= MTYPE_PTEXT;
	s_fcmode.fcnoteles.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_fcmode.fcnoteles.generic.id			= CV_FCMODE_NOTELES;
	s_fcmode.fcnoteles.generic.callback		= Fcmode_MenuEvent;
	s_fcmode.fcnoteles.generic.x			= 320;
	s_fcmode.fcnoteles.generic.y			= y;
	s_fcmode.fcnoteles.string				= "NO TELEPORTERS";
	s_fcmode.fcnoteles.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_fcmode.fcnoteles.color				= text_color_normal;
	if( fcmode == FC_NOTELES )
        s_fcmode.fcnoteles.generic.flags |= QMF_GRAYED;

	y += 23;
	s_fcmode.fcnopads.generic.type		= MTYPE_PTEXT;
	s_fcmode.fcnopads.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_fcmode.fcnopads.generic.id		= CV_FCMODE_NOPADS;
	s_fcmode.fcnopads.generic.callback	= Fcmode_MenuEvent;
	s_fcmode.fcnopads.generic.x			= 320;
	s_fcmode.fcnopads.generic.y			= y;
	s_fcmode.fcnopads.string			= "NO JUMP PADS";
	s_fcmode.fcnopads.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_fcmode.fcnopads.color				= text_color_normal;
	if( fcmode == FC_NOPADS )
        s_fcmode.fcnopads.generic.flags |= QMF_GRAYED;

	y += 23;
	s_fcmode.fcnodoors.generic.type		= MTYPE_PTEXT;
	s_fcmode.fcnodoors.generic.flags	= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_fcmode.fcnodoors.generic.id		= CV_FCMODE_NODOORS;
	s_fcmode.fcnodoors.generic.callback	= Fcmode_MenuEvent;
	s_fcmode.fcnodoors.generic.x		= 320;
	s_fcmode.fcnodoors.generic.y		= y;
	s_fcmode.fcnodoors.string			= "NO DOORS";
	s_fcmode.fcnodoors.style			= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_fcmode.fcnodoors.color			= text_color_normal;
	if( fcmode == FC_NODOORS )
        s_fcmode.fcnodoors.generic.flags |= QMF_GRAYED;

	y += 23;
	s_fcmode.fcboth.generic.type		= MTYPE_PTEXT;
	s_fcmode.fcboth.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_fcmode.fcboth.generic.id			= CV_FCMODE_BOTH;
	s_fcmode.fcboth.generic.callback	= Fcmode_MenuEvent;
	s_fcmode.fcboth.generic.x			= 320;
	s_fcmode.fcboth.generic.y			= y;
	s_fcmode.fcboth.string				= "NO TELES AND PADS";
	s_fcmode.fcboth.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_fcmode.fcboth.color				= text_color_normal;
	if( fcmode == FC_NOTELES_NOPADS )
        s_fcmode.fcboth.generic.flags |= QMF_GRAYED;

	y += 23;
	s_fcmode.fcdisableall.generic.type		= MTYPE_PTEXT;
	s_fcmode.fcdisableall.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_fcmode.fcdisableall.generic.id		= CV_FCMODE_DISABLEALL;
	s_fcmode.fcdisableall.generic.callback	= Fcmode_MenuEvent;
	s_fcmode.fcdisableall.generic.x			= 320;
	s_fcmode.fcdisableall.generic.y			= y;
	s_fcmode.fcdisableall.string			= "DISABLE EVERYTHING";
	s_fcmode.fcdisableall.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_fcmode.fcdisableall.color				= text_color_normal;
	if( fcmode == FC_DISABLEALL )
        s_fcmode.fcdisableall.generic.flags |= QMF_GRAYED;

	y += 23;
	s_fcmode.fcrocket.generic.type		= MTYPE_PTEXT;
	s_fcmode.fcrocket.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_fcmode.fcrocket.generic.id		= CV_FCMODE_ROCKET;
	s_fcmode.fcrocket.generic.callback	= Fcmode_MenuEvent;
	s_fcmode.fcrocket.generic.x			= 320;
	s_fcmode.fcrocket.generic.y			= y;
	s_fcmode.fcrocket.string			= "ROCKET LAUNCHER";
	s_fcmode.fcrocket.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_fcmode.fcrocket.color				= text_color_normal;
	if( fcmode == FC_ROCKET )
        s_fcmode.fcrocket.generic.flags |= QMF_GRAYED;
		
	y += 23;
	s_fcmode.fcplasma.generic.type		= MTYPE_PTEXT;
	s_fcmode.fcplasma.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_fcmode.fcplasma.generic.id		= CV_FCMODE_PLASMA;
	s_fcmode.fcplasma.generic.callback	= Fcmode_MenuEvent;
	s_fcmode.fcplasma.generic.x			= 320;
	s_fcmode.fcplasma.generic.y			= y;
	s_fcmode.fcplasma.string			= "PLASMA GUN";
	s_fcmode.fcplasma.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_fcmode.fcplasma.color				= text_color_normal;
	if( fcmode == FC_PLASMA )
        s_fcmode.fcplasma.generic.flags |= QMF_GRAYED;
		
	y += 23;
	s_fcmode.fccombo.generic.type		= MTYPE_PTEXT;
	s_fcmode.fccombo.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_fcmode.fccombo.generic.id			= CV_FCMODE_COMBO;
	s_fcmode.fccombo.generic.callback	= Fcmode_MenuEvent;
	s_fcmode.fccombo.generic.x			= 320;
	s_fcmode.fccombo.generic.y			= y;
	s_fcmode.fccombo.string				= "WEAPON COMBO";
	s_fcmode.fccombo.style				= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_fcmode.fccombo.color				= text_color_normal;
	if( fcmode == FC_COMBO )
        s_fcmode.fccombo.generic.flags |= QMF_GRAYED;
		
	y += 46;
	s_fcmode.cancel.generic.type			= MTYPE_PTEXT;
	s_fcmode.cancel.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_fcmode.cancel.generic.id				= CV_FCMODE_CANCEL;
	s_fcmode.cancel.generic.callback		= Fcmode_MenuEvent;
	s_fcmode.cancel.generic.x				= 320;
	s_fcmode.cancel.generic.y				= y;
	s_fcmode.cancel.string					= "CANCEL";
	s_fcmode.cancel.style					= UI_CENTER|UI_SMALLFONT|UI_DROPSHADOW;
	s_fcmode.cancel.color					= text_color_normal;

	Menu_AddItem( &s_fcmode.menu, (void*) &s_fcmode.frame );
	
	Menu_AddItem( &s_fcmode.menu, (void*) &s_fcmode.fcdefault );
	Menu_AddItem( &s_fcmode.menu, (void*) &s_fcmode.fcnoteles );
	Menu_AddItem( &s_fcmode.menu, (void*) &s_fcmode.fcnopads );
	Menu_AddItem( &s_fcmode.menu, (void*) &s_fcmode.fcnodoors );
	Menu_AddItem( &s_fcmode.menu, (void*) &s_fcmode.fcboth );
	Menu_AddItem( &s_fcmode.menu, (void*) &s_fcmode.fcdisableall );
	Menu_AddItem( &s_fcmode.menu, (void*) &s_fcmode.fcrocket );
	Menu_AddItem( &s_fcmode.menu, (void*) &s_fcmode.fcplasma );
	Menu_AddItem( &s_fcmode.menu, (void*) &s_fcmode.fccombo );
	
	Menu_AddItem( &s_fcmode.menu, (void*) &s_fcmode.cancel );
}

/*
========================
UI_CallvoteFcmodeMenu
=======================
*/
void UI_CallvoteFcmodeMenu( void ) {
	Cv_Fcmode_MenuInit();
	UI_PushMenu( &s_fcmode.menu );
}



/*
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
CALLVOTE MAP/DEVMAP UI
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

#define CV_MAP_MAP	 		601
#define CV_MAP_DEVMAP 		602
#define CV_MAP_LIST			603
#define CV_MAP_RANDOM		604
#define CV_MAP_CANCEL	  	625

#define CV_MAX_MAPS			32768
#define NAMEBUFSIZE         65536

typedef struct
{
	menuframework_s	menu;
	menubitmap_s	frame;
	menubitmap_s	levelshot;
	
	menutext_s		map;
	menutext_s		devmap;
	menutext_s		pickrandom;
	menulist_s		maplist;
	
	int				numMaps;
	char 			mapname[CV_MAX_MAPS];
	
	menutext_s		cancel;
} cv_map_t;

cv_map_t s_map;

typedef struct {
	char *file_name;
	int	 file_nlen;
	char file_type;
} map_entry_t;

static map_entry_t 	mentry[CV_MAX_MAPS];
static map_entry_t *dptr[CV_MAX_MAPS];
static char			show_names[CV_MAX_MAPS][64*3];
static int 			num_files;
static char			buffer[NAMEBUFSIZE];

/*
===============
LevelshotDraw
===============
*/
void LevelshotDraw( void *self )
{
	static char		picname[MAX_QPATH];
	map_entry_t 	*selected;
	int				curlen;
	
	menubitmap_s    *b;
	
	b = (menubitmap_s *)self;
	
	selected = dptr[ s_map.maplist.curvalue ];
	curlen   = selected->file_nlen;
	
	// strip extension
	if ( !Q_stricmp( selected->file_name + curlen - 4, ".bsp" ) )
		selected->file_name[ curlen-4 ] = '\0';
	
	// set the map pic
	Com_sprintf( picname, sizeof( picname ), va("levelshots/%s.tga", selected->file_name) );
	trap_R_RegisterShaderNoMip( picname );
	s_map.levelshot.generic.name = picname;

	Bitmap_Draw( b );
}

/*
===============
LevelshotUpdate
===============
*/
void LevelshotUpdate( void )
{
	static char		picname[MAX_QPATH];
	map_entry_t 	*selected;
	int				curlen;
	
	selected = dptr[ s_map.maplist.curvalue ];
	curlen   = selected->file_nlen;
	
	// strip extension
	if ( !Q_stricmp( selected->file_name + curlen - 4, ".bsp" ) )
		selected->file_name[ curlen-4 ] = '\0';
	
	// set the map pic
	Com_sprintf( picname, sizeof( picname ), va("levelshots/%s.tga", selected->file_name) );
	trap_R_RegisterShaderNoMip( picname );
	s_map.levelshot.generic.name = picname;
	s_map.levelshot.shader = 0;
}
	

/*
===============
UI_MapsReadDir
===============
*/
static void UI_MapsReadDir( void )
{
	char	*s;
	int		i, len, n, c = 0;

	n = trap_FS_GetFileList( "maps", "bsp", buffer, sizeof( buffer ) );
	if ( n > CV_MAX_MAPS )
		n = CV_MAX_MAPS;
		
	s = buffer;
	for ( i = 0; i < n; i++ ) {
		len = strlen( s );
		mentry[c].file_type = 1;
		mentry[c].file_name = s;
		mentry[c].file_nlen = len;
		c++;
		s += len + 1;
	}
	num_files = c;
	s_map.numMaps = num_files;
}

/*
===============
Map_MenuEvent
===============
*/
void Map_MenuEvent( void* ptr, int event ) {
	map_entry_t 	*selected;
	char			*r;
	int				curlen;
	
	LevelshotUpdate();
	
	if( event != QM_ACTIVATED ) 
		return;
		
	selected = dptr[ s_map.maplist.curvalue ];
	curlen	 = selected->file_nlen;
	
	// strip extension
	if ( !Q_stricmp( selected->file_name + curlen - 4, ".bsp" ) )
		selected->file_name[ curlen-4 ] = '\0';
		
	switch( ((menucommon_s*)ptr)->id ) {
	case CV_MAP_MAP:
		UI_ForceMenuOff();
		r = va( "cv map %s\n", selected->file_name );
		trap_Cmd_ExecuteText( EXEC_APPEND, r);
		break;
		
	case CV_MAP_DEVMAP:
		UI_ForceMenuOff();
		r = va( "cv devmap %s\n", selected->file_name );
		trap_Cmd_ExecuteText( EXEC_APPEND, r);
		break;
		
	case CV_MAP_RANDOM:
		s_map.maplist.curvalue = rand()&num_files - 1; 
		if ( s_map.maplist.curvalue <= s_map.maplist.height)
			s_map.maplist.top = s_map.maplist.curvalue;
		else if ( s_map.maplist.curvalue >= num_files-s_map.maplist.height )
			s_map.maplist.top = num_files - s_map.maplist.height + 1;
		else
			s_map.maplist.top = s_map.maplist.curvalue - s_map.maplist.height + 1;
			
		break;
		
	case CV_MAP_CANCEL:
		UI_PopMenu();
		break;
	}
	
	
}

static int CV_cstricmp( const char *src, const char *dst ) {
    int ret = 0;
    int c1 = 0, c2 = 0;
	unsigned char	ch1, ch2;
    do {
        if ( *src == '^' && src[1] ) {
            c1 = src[1];
            src += 2;
            continue;
        }

        if ( *dst == '^' && dst[1] ) {
            c2 = dst[1];
            dst += 2;
            continue;
        }

		ch1 = *src;
		ch2 = *dst;

		if ( ch1 >= 'a' && ch1 <= 'z' )
			ch1 = ch1 - 'a' + 'A';
		if ( ch2 >= 'a' && ch2 <= 'z' )
			ch2 = ch2 - 'a' + 'A';

        ret = ch1 - ch2;
        if ( !ret )
            ret = c1 - c2;

        dst++;
        if ( ret || !*dst )
            break;

        src++;

    } while ( 1 );

    if ( ret < 0 )
        ret = -1;
    else if ( ret > 0 )
        ret = 1;

    return( ret );
}

static int compare_entries( map_entry_t * a, map_entry_t *b, int mode )
{
	int t;
	t = b->file_type - a->file_type;
	if ( !t )
		if ( mode == 1 )
			return CV_cstricmp( a->file_name, b->file_name );
		else
			return -CV_cstricmp( a->file_name, b->file_name );

	else
		return t;
}


static void UI_mapsort( map_entry_t **a, int n, int mode )
{
    map_entry_t * tmp;
    int i = 0, j = n;
    map_entry_t *m = a[ n>>1 ];
    do
    {
        while ( compare_entries( a[i], m, mode ) < 0 ) i++;
        while ( compare_entries( a[j], m, mode ) > 0 ) j--;

		if ( i <= j )
        {
            tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
            i++;
            j--;
        }
    }
    while ( i <= j );
		if ( j > 0 ) 
			UI_mapsort( a, j, mode );
		
		if ( n > i ) 
			UI_mapsort( a+i, n-i, mode );
}

static void UI_MapsFillList( void ) {
	int		i, diff, len;
	char	matchname[256];
	map_entry_t *d;

	s_map.maplist.numitems = 0;

	for ( i = 0; i < s_map.numMaps; i++ ) {
		len = mentry[i].file_nlen;
		
		// strip extension
		if ( !Q_stricmp( matchname + len - 4, ".bsp" ) )
			matchname[ len-4 ] = '\0';

		dptr[ s_map.maplist.numitems++ ] = &mentry[ i ];
		if ( s_map.maplist.numitems >= CV_MAX_MAPS )
			break;
	}

	if ( s_map.maplist.numitems > 1 ) {
		UI_mapsort( dptr, s_map.maplist.numitems - 1, 1 );
		s_map.maplist.curvalue = 0;
		s_map.maplist.top = 0;
	}
	
	for ( i = 0; i < s_map.maplist.numitems; i++ ) {
		d = dptr[ i ];
		s_map.maplist.itemnames[i] = show_names[i];

		Q_strncpyz( show_names[i], d->file_name, sizeof( show_names[i] ) );
		
		len = d->file_nlen;
		// strip extension
		if ( !Q_stricmp( show_names[i] +  len - 4,".bsp" ) ) {
			memset( show_names[i] +  len - 4, ' ', 4 );
			len -= 4;
		}

		diff = UI_cstrdiff ( show_names[i] );

		show_names[i][s_map.maplist.width-1+diff] = '\0';


		if ( len - diff > s_map.maplist.width - 1 ) {
			strcpy( &show_names[i][s_map.maplist.width-1+diff], "^7>" );
		}
	}

}

/*
===============
Cv_Map_MenuInit
===============
*/
void Cv_Map_MenuInit( void ) {
	int x, y;
	memset( &s_map, 0, sizeof(s_map) );
	
	s_map.menu.wrapAround = qtrue;
	s_map.menu.fullscreen = qfalse;

	s_map.frame.generic.type			= MTYPE_BITMAP;
	s_map.frame.generic.flags			= QMF_INACTIVE;
	s_map.frame.generic.name			= ART_FRAME;
	s_map.frame.width					= 520;
	s_map.frame.height					= 286;
	s_map.frame.generic.x				= (640-s_map.frame.width)/2;
	s_map.frame.generic.y				= (480-s_map.frame.height)/2;
	
	s_map.maplist.generic.type			= MTYPE_SCROLLLIST;
	s_map.maplist.generic.flags			= QMF_PULSEIFFOCUS;
	s_map.maplist.generic.callback		= Map_MenuEvent;
	s_map.maplist.generic.id			= CV_MAP_LIST;
	s_map.maplist.generic.x				= (640-s_map.frame.width)/2 + 24;
	s_map.maplist.generic.y				= (480-s_map.frame.height)/2 + 16;
	s_map.maplist.width					= 32;
	s_map.maplist.height				= 16;
	s_map.maplist.columns				= 1;
	s_map.maplist.scroll				= s_map.maplist.height - 1;
	s_map.maplist.itemnames				= (const char **)s_map.mapname;
	
	s_map.levelshot.generic.ownerdraw		= LevelshotDraw;
	s_map.levelshot.generic.type			= MTYPE_BITMAP;
	s_map.levelshot.generic.flags			= QMF_LEFT_JUSTIFY|QMF_INACTIVE;
	s_map.levelshot.generic.x				= 380;
	s_map.levelshot.generic.y				= 242;
	s_map.levelshot.width					= 160;
	s_map.levelshot.height					= 120;
	s_map.levelshot.errorpic				= "gfx/ui/unknownmap.tga";
	
	UI_MapsReadDir();
	
	if ( !s_map.numMaps ) {
		strcpy( buffer, "No Maps Found." );
		s_map.maplist.numitems = 1;

		s_map.map.generic.flags |= (QMF_INACTIVE|QMF_HIDDEN);
		s_map.devmap.generic.flags |= (QMF_INACTIVE|QMF_HIDDEN);
	} else {
		UI_MapsFillList();
	}

	y = 132;
	x = 540;
	s_map.map.generic.type			= MTYPE_PTEXT;
	s_map.map.generic.flags			= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_map.map.generic.id			= CV_MAP_MAP;
	s_map.map.generic.callback		= Map_MenuEvent;
	s_map.map.generic.x				= x;
	s_map.map.generic.y				= y;
	s_map.map.string				= "VOTE MAP";
	s_map.map.style					= UI_RIGHT|UI_SMALLFONT|UI_DROPSHADOW;
	s_map.map.color					= text_color_normal;
	
	y += 24;
	s_map.devmap.generic.type			= MTYPE_PTEXT;
	s_map.devmap.generic.flags			= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_map.devmap.generic.id				= CV_MAP_DEVMAP;
	s_map.devmap.generic.callback		= Map_MenuEvent;
	s_map.devmap.generic.x				= x;
	s_map.devmap.generic.y				= y;
	s_map.devmap.string					= "VOTE DEVMAP";
	s_map.devmap.style					= UI_RIGHT|UI_SMALLFONT|UI_DROPSHADOW;
	s_map.devmap.color					= text_color_normal;
	
	y += 24;
	s_map.pickrandom.generic.type		= MTYPE_PTEXT;
	s_map.pickrandom.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_map.pickrandom.generic.id			= CV_MAP_RANDOM;
	s_map.pickrandom.generic.callback	= Map_MenuEvent;
	s_map.pickrandom.generic.x			= x;
	s_map.pickrandom.generic.y			= y;
	s_map.pickrandom.string				= "PICK RANDOM";
	s_map.pickrandom.style				= UI_RIGHT|UI_SMALLFONT|UI_DROPSHADOW;
	s_map.pickrandom.color				= text_color_normal;
	
	y += 24;
	s_map.cancel.generic.type			= MTYPE_PTEXT;
	s_map.cancel.generic.flags			= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_map.cancel.generic.id				= CV_MAP_CANCEL;
	s_map.cancel.generic.callback		= Map_MenuEvent;
	s_map.cancel.generic.x				= x;
	s_map.cancel.generic.y				= y;
	s_map.cancel.string					= "CANCEL";
	s_map.cancel.style					= UI_RIGHT|UI_SMALLFONT|UI_DROPSHADOW;
	s_map.cancel.color					= text_color_normal;
	
	Menu_AddItem( &s_map.menu, (void*) &s_map.frame );
	Menu_AddItem( &s_map.menu, (void*) &s_map.maplist );
	Menu_AddItem( &s_map.menu, (void*) &s_map.map );
	Menu_AddItem( &s_map.menu, (void*) &s_map.levelshot );
	Menu_AddItem( &s_map.menu, (void*) &s_map.devmap );
	Menu_AddItem( &s_map.menu, (void*) &s_map.pickrandom );
	Menu_AddItem( &s_map.menu, (void*) &s_map.cancel );
}

/*
========================
UI_CallvoteMapMenu
=======================
*/
void UI_CallvoteMapMenu( void ) {
	Cv_Map_MenuInit();
	UI_PushMenu( &s_map.menu );
}



/*
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
CALLVOTE KICK PLAYER UI
--------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

#define CV_KICK_KICKPLAYER	701
#define CV_KICK_LIST		702
#define CV_KICK_CANCEL	  	725

typedef struct
{
	menuframework_s	menu;
	menubitmap_s	frame;
	
	menutext_s		kick;
	menulist_s		list;
	
	menutext_s		cancel;
	int				clientCount;
	char            *clientList[MAX_CLIENTS+1];
	char 		    clientNames[MAX_CLIENTS][MAX_NAME_LENGTH];
	
} cv_kick_t;

cv_kick_t s_kick;

/*
===============
Kick_MenuEvent
===============
*/
void Kick_MenuEvent( void* ptr, int event ) {
	
	if( event != QM_ACTIVATED ) 
		return;

	switch( ((menucommon_s*)ptr)->id ) {
	
	case CV_KICK_KICKPLAYER:
		trap_Cmd_ExecuteText( EXEC_APPEND, va("cv kick %s\n", s_kick.clientList[ s_kick.list.curvalue ] ));
		UI_ForceMenuOff();
		break;
		
	case CV_KICK_CANCEL:
		UI_PopMenu();
		break;
	}
}



/*
===============
Cv_Kick_MenuInit
===============
*/
void Cv_Kick_MenuInit( void ) {
	
	char			info[MAX_INFO_STRING];
	int 			x, y, n;
	
	memset( &s_kick, 0, sizeof(s_kick) );
	
	s_kick.menu.wrapAround = qtrue;
	s_kick.menu.fullscreen = qfalse;

	n = 0;
	s_kick.clientCount = 0;
	while ( n < MAX_CLIENTS ) {
		trap_GetConfigString( CS_PLAYERS + n, info, MAX_INFO_STRING );
				
		if (info[0]) {
			Q_strncpyz( s_kick.clientNames[ n ], Info_ValueForKey( info, "n" ), MAX_NAME_LENGTH );
			//Q_CleanStr( s_kick.clientNames[ n ] );
			s_kick.clientList[n] = s_kick.clientNames[n];
			s_kick.clientCount++;
		}
		n++;
	}
	
	s_kick.list.numitems = s_kick.clientCount;
	s_kick.clientList[ n ] = NULL;
				
	s_kick.frame.generic.type		= MTYPE_BITMAP;
	s_kick.frame.generic.flags		= QMF_INACTIVE;
	s_kick.frame.generic.name		= ART_FRAME;
	s_kick.frame.width				= 520;
	s_kick.frame.height				= 286;
	s_kick.frame.generic.x			= (640-s_kick.frame.width)/2;
	s_kick.frame.generic.y			= (480-s_kick.frame.height)/2;
	
	s_kick.list.generic.type		= MTYPE_SCROLLLIST;
	s_kick.list.generic.flags		= QMF_PULSEIFFOCUS;
	s_kick.list.generic.callback	= Kick_MenuEvent;
	s_kick.list.generic.id			= CV_KICK_LIST;
	s_kick.list.generic.x			= (640-s_kick.frame.width)/2 + 24;
	s_kick.list.generic.y			= (480-s_kick.frame.height)/2 + 16;
	s_kick.list.width				= 32;
	s_kick.list.height				= 16;
	s_kick.list.columns				= 1;
	s_kick.list.scroll				= s_kick.list.height - 1;
	s_kick.list.itemnames			= (const char **) s_kick.clientList;
	
	y = 132;
	x = 540;
	s_kick.kick.generic.type			= MTYPE_PTEXT;
	s_kick.kick.generic.flags			= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_kick.kick.generic.id				= CV_KICK_KICKPLAYER;
	s_kick.kick.generic.callback		= Kick_MenuEvent;
	s_kick.kick.generic.x				= x;
	s_kick.kick.generic.y				= y;
	s_kick.kick.string					= "KICK PLAYER";
	s_kick.kick.style					= UI_RIGHT|UI_SMALLFONT|UI_DROPSHADOW;
	s_kick.kick.color					= text_color_normal;
	
	y += 48;
	s_kick.cancel.generic.type			= MTYPE_PTEXT;
	s_kick.cancel.generic.flags			= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_kick.cancel.generic.id			= CV_KICK_CANCEL;
	s_kick.cancel.generic.callback		= Kick_MenuEvent;
	s_kick.cancel.generic.x				= x;
	s_kick.cancel.generic.y				= y;
	s_kick.cancel.string				= "CANCEL";
	s_kick.cancel.style					= UI_RIGHT|UI_SMALLFONT|UI_DROPSHADOW;
	s_kick.cancel.color					= text_color_normal;
	
	Menu_AddItem( &s_kick.menu, (void*) &s_kick.frame );
	Menu_AddItem( &s_kick.menu, (void*) &s_kick.list );
	Menu_AddItem( &s_kick.menu, (void*) &s_kick.kick );
	Menu_AddItem( &s_kick.menu, (void*) &s_kick.cancel );
}

/*
========================
UI_CallvoteKickMenu
=======================
*/
void UI_CallvoteKickMenu( void ) {
	Cv_Kick_MenuInit();
	UI_PushMenu( &s_kick.menu );
}
