// Copyright (C) 1999-2000 Id Software, Inc.
//
/*
=======================================================================

GAME OPTIONS MENU

=======================================================================
*/

#include "ui_local.h"


#define ART_FRAMEL				"menu/art/frame2_l"
#define ART_FRAMER				"menu/art/frame1_r"
#define ART_BACK0				"menu/art/back_0"
#define ART_BACK1				"menu/art/back_1"
#define ART_APPLY0				"menu/art/accept_0"
#define ART_APPLY1				"menu/art/accept_1"

#define SERVER_PREFERENCES_X_POS		250 //360

#define ID_RCONIP				124
#define ID_RCONPASSWORD			125
#define ID_SENDRCONCOMMANDS		126
#define ID_SCORING				127
#define ID_SPACE				128
#define ID_SPLASHDAMAGE			129
#define ID_ALLOWGRAPPLE			130
#define ID_ALLOWQUAD			131
#define ID_ALLOWHASTE			132
#define ID_ALLOWINVIS			133
#define ID_ALWAYSGIB			134
#define ID_SPAWNTIME			135
#define ID_BOUNCES				136
#define ID_GRAPPLESPEED			137
#define ID_GRAPPLEPULL			138

#define ID_SPAWNNOFIRE			139
#define ID_INVISGAME			140
#define ID_INVISFIRE			141
#define ID_INVISFLICKER			142
#define ID_ANTICAMP				143
#define ID_ANTICAMPTIME			144
#define ID_ANTICAMPRADIUS		145

#define	ID_HEADHUNT				146
#define	ID_HEADFROMCORPSE		147

#define ID_DELAYEDFIRE			148
#define ID_FIREDELAY			149
#define	ID_WORLDDAMAGE			150
#define	ID_FIRERATE				151

#define ID_VALKYRMODE			152

#define	ID_SUICIDE				153

#define ID_BACK					154

const char *grapplelist[] = {
	"disabled",
	"CorkScrew",
	"Quake III",
	"Quake II CTF",
	0
};

typedef struct {

	menuframework_s		menu;

	// if both are false, don't allow configuration
	qboolean			localServer;
	qboolean			remoteServer;
	// if just setting up a server then always allow config
	qboolean			allowConfig;

	menutext_s			banner;
	menubitmap_s		framel;
	menubitmap_s		framer;

	menuradiobutton_s	scoring;
	menuradiobutton_s	space;
	menuradiobutton_s	splashdamage;
	menuradiobutton_s	alwaysgib;
	menuradiobutton_s	allowquad;
	menuradiobutton_s	allowhaste;
	menuradiobutton_s	allowinvis;
	menuradiobutton_s	worlddamage;
	menuradiobutton_s	valkyrmode;

	menulist_s			grapple;
	menufield_s			spawntime;
	menufield_s			bounces;
	menuradiobutton_s	suicide;

	menufield_s			grapplespeed;
	menufield_s			grapplepull;
	menufield_s			firerate;

	menuradiobutton_s	spawnnofire;
	menuradiobutton_s	invisgame;
	menuradiobutton_s	invisfire;
	menuradiobutton_s	anticamp;
	menufield_s			invisflicker;
	menufield_s			anticamptime;
	menufield_s			anticampradius;

	menuradiobutton_s	headhunt;
	menuradiobutton_s	headfromcorpse;

	menuradiobutton_s	delayedfire;
	menufield_s			firedelay;

	menufield_s			rconAddress;
	menufield_s			rconPassword;
	menutext_s			sendRconCommands;

	menubitmap_s		back;
} server_preferences_t;

static server_preferences_t s_server_preferences;

/*
=================
server_preferences_SetMenuItems
=================
*/

static void server_preferences_SetMenuItems( void ) {
	float temp;
	char string[64];

	trap_Cvar_VariableStringBuffer( "rconAddress", string, sizeof(string) );
	Com_sprintf( s_server_preferences.rconAddress.field.buffer, sizeof(string), string );
	trap_Cvar_VariableStringBuffer( "rconPassword", string, sizeof(string) );
	Com_sprintf( s_server_preferences.rconPassword.field.buffer, sizeof(string), string );

	// if remote server, use defaults
	if( s_server_preferences.remoteServer ) {
		s_server_preferences.scoring.curvalue = 0;
		s_server_preferences.space.curvalue = 1;
		s_server_preferences.worlddamage.curvalue = 0;
		s_server_preferences.splashdamage.curvalue = 1;
		s_server_preferences.alwaysgib.curvalue = 0;
		s_server_preferences.allowquad.curvalue = 1;
		s_server_preferences.allowhaste.curvalue = 1;
		s_server_preferences.allowinvis.curvalue = 1;
		s_server_preferences.valkyrmode.curvalue = 0;

		s_server_preferences.grapple.curvalue = 1;
		s_server_preferences.spawnnofire.curvalue = 1;
		s_server_preferences.invisgame.curvalue = 0;
		s_server_preferences.invisfire.curvalue = 0;
		s_server_preferences.anticamp.curvalue = 0;

		s_server_preferences.suicide.curvalue = 0;

		s_server_preferences.headhunt.curvalue = 0;
		s_server_preferences.headfromcorpse.curvalue = 1;

		s_server_preferences.delayedfire.curvalue = 0;

		Com_sprintf( s_server_preferences.invisflicker.field.buffer, 2, "1" );
		Com_sprintf( s_server_preferences.anticamptime.field.buffer, 3 , "10");
		Com_sprintf( s_server_preferences.anticampradius.field.buffer, 4, "192");
		Com_sprintf( s_server_preferences.spawntime.field.buffer, 2, "2");
		Com_sprintf( s_server_preferences.bounces.field.buffer, 2, "0");
		Com_sprintf( s_server_preferences.grapplespeed.field.buffer, 5, "1600");
		Com_sprintf( s_server_preferences.grapplepull.field.buffer, 4, "800");
		Com_sprintf( s_server_preferences.firerate.field.buffer, 4, "100");
		Com_sprintf( s_server_preferences.firedelay.field.buffer, 4, "800");

		return;
	}

	// if firerate == 0, i assume the cvars aren't initialized yet, so do it now
	// this happens only if this machine never ran a server before, including any
	// single player, eg: corkscrew is installed, and player makes a server
	// and pushes the server preferences button in the server configuration menu
	if ( trap_Cvar_VariableValue( "g_firerate" ) == 0 ) {
		trap_Cvar_SetValue( "g_scoring", 0 );
		trap_Cvar_SetValue( "g_spaceProtection", 1 );
		trap_Cvar_SetValue( "g_worldDamage", 0 );
		trap_Cvar_SetValue( "g_railgunExplosions", 1 );
		trap_Cvar_SetValue( "g_railgunBounces", 0 );
		trap_Cvar_SetValue( "g_railgunSuicide", 0 );
		trap_Cvar_SetValue( "g_alwaysGib", 0 );
		trap_Cvar_SetValue( "g_allowQuad", 1 );
		trap_Cvar_SetValue( "g_allowHaste", 1 );
		trap_Cvar_SetValue( "g_allowInvis", 1 );
		trap_Cvar_SetValue( "g_allowGrapple", 1 );
		trap_Cvar_SetValue( "g_valkyrMode", 0 );

		trap_Cvar_SetValue( "g_spawnProtectionNoFire", 1 );
		trap_Cvar_SetValue( "g_spawnProtection", 2 );
		trap_Cvar_SetValue( "g_invisFire", 0 );
		trap_Cvar_SetValue( "g_invisFlicker", 1 );
		trap_Cvar_SetValue( "g_invis", 0 );
		trap_Cvar_SetValue( "g_anticampTime", 10 );
		trap_Cvar_SetValue( "g_anticampRadius", 192 );
		trap_Cvar_SetValue( "g_anticamp", 0 );
		trap_Cvar_SetValue( "g_grappleSpeed", 1600 );
		trap_Cvar_SetValue( "g_grapplePull", 800 );
		trap_Cvar_SetValue( "g_firerate", 1 );

		trap_Cvar_SetValue( "g_headHunt", 0 );
		trap_Cvar_SetValue( "g_headFromCorpse", 1 );

		trap_Cvar_SetValue( "g_railgunDelayedFire", 0 );
		trap_Cvar_SetValue( "g_railgunFireDelay", 800 );
	}

	temp = trap_Cvar_VariableValue( "g_firerate" ) * 100.0;

	s_server_preferences.scoring.curvalue		= trap_Cvar_VariableValue( "g_scoring" ) != 0;
	s_server_preferences.space.curvalue			= trap_Cvar_VariableValue( "g_spaceProtection" ) != 0;
	s_server_preferences.worlddamage.curvalue	= trap_Cvar_VariableValue( "g_worldDamage" ) != 0;
	s_server_preferences.splashdamage.curvalue	= trap_Cvar_VariableValue( "g_railgunExplosions" ) != 0;
	s_server_preferences.alwaysgib.curvalue		= trap_Cvar_VariableValue( "g_alwaysgib" ) != 0;
	s_server_preferences.allowquad.curvalue		= trap_Cvar_VariableValue( "g_allowQuad" ) != 0;
	s_server_preferences.allowhaste.curvalue	= trap_Cvar_VariableValue( "g_allowHaste" ) != 0;
	s_server_preferences.allowinvis.curvalue	= trap_Cvar_VariableValue( "g_allowInvis" ) != 0;
	s_server_preferences.grapple.curvalue		= (int)Com_Clamp( 0, 3, trap_Cvar_VariableValue( "g_allowGrapple" ) );
	s_server_preferences.valkyrmode.curvalue	= trap_Cvar_VariableValue( "g_valkyrMode" ) != 0;

	s_server_preferences.spawnnofire.curvalue	= trap_Cvar_VariableValue( "g_spawnProtectionNoFire" ) != 0;
	s_server_preferences.invisgame.curvalue		= trap_Cvar_VariableValue( "g_invis" ) != 0;
	s_server_preferences.invisfire.curvalue		= trap_Cvar_VariableValue( "g_invisFire" ) != 0;
	s_server_preferences.anticamp.curvalue		= trap_Cvar_VariableValue( "g_anticamp" ) != 0;

	s_server_preferences.headhunt.curvalue			= trap_Cvar_VariableValue( "g_headHunt" ) != 0;
	s_server_preferences.headfromcorpse.curvalue	= trap_Cvar_VariableValue( "g_headFromCorpse" ) != 0;

	s_server_preferences.delayedfire.curvalue	= trap_Cvar_VariableValue( "g_railgunDelayedFire" ) != 0;

	s_server_preferences.suicide.curvalue		= trap_Cvar_VariableValue( "g_railgunSuicide" ) != 0;

	Com_sprintf( s_server_preferences.grapplespeed.field.buffer, 5, "%i", (int)Com_Clamp( 0, 9999, trap_Cvar_VariableValue( "g_grappleSpeed" ) ) );
	Com_sprintf( s_server_preferences.grapplepull.field.buffer, 5, "%i", (int)Com_Clamp( 0, 9999, trap_Cvar_VariableValue( "g_grapplePull" ) ) );
	Com_sprintf( s_server_preferences.spawntime.field.buffer, 3, "%i", (int)Com_Clamp( 0, 99, trap_Cvar_VariableValue( "g_spawnProtection" ) ) );
	Com_sprintf( s_server_preferences.bounces.field.buffer, 3, "%i", (int)Com_Clamp( 0, 99, trap_Cvar_VariableValue( "g_railgunBounces" ) ) );
	Com_sprintf( s_server_preferences.firerate.field.buffer, 5, "%i", (int)Com_Clamp( 1, 2500, temp ) );

	Com_sprintf( s_server_preferences.invisflicker.field.buffer, 4, "%i", (int)Com_Clamp( 0, 100, trap_Cvar_VariableValue( "g_invisFlicker" ) ) );
	Com_sprintf( s_server_preferences.anticamptime.field.buffer, 3, "%i", (int)Com_Clamp( 0, 99, trap_Cvar_VariableValue( "g_anticampTime" ) ) );
	Com_sprintf( s_server_preferences.anticampradius.field.buffer, 4, "%i", (int)Com_Clamp( 0, 999, trap_Cvar_VariableValue( "g_anticampRadius" ) ) );

	Com_sprintf( s_server_preferences.firedelay.field.buffer, 5, "%i", (int)Com_Clamp( 0, 9999, trap_Cvar_VariableValue( "g_railgunFireDelay" ) ) );

	if ( s_server_preferences.headhunt.curvalue && s_server_preferences.scoring.curvalue ) {
		s_server_preferences.headhunt.curvalue = 0;	// don't allow both to be set, which would be plain dumb
		s_server_preferences.scoring.curvalue = 0;
	}
}


/*
=================
UI_SendRconCommands
=================
*/

void UI_SendRconCommands( void ) {
	float temp;
	char rconString[1024];

	temp = atof( s_server_preferences.firerate.field.buffer ) / 100.0;
	temp = temp > 25 ? 25 : temp;
	temp = temp <= 0 ? 1 : temp;

	Com_sprintf( rconString, sizeof(rconString),
		"wait;wait;rcon g_scoring %i\n"
		"wait;wait;rcon g_spaceProtection %i\n"
		"wait;wait;rcon g_worldDamage %i\n"
		"wait;wait;rcon g_railgunExplosions %i\n"
		"wait;wait;rcon g_railgunBounces %i\n"
		"wait;wait;rcon g_railgunSuicide %i\n"
		"wait;wait;rcon g_alwaysGib %i\n"
		"wait;wait;rcon g_allowQuad %i\n"
		"wait;wait;rcon g_allowHaste %i\n"
		"wait;wait;rcon g_allowInvis %i\n"
		"wait;wait;rcon g_allowGrapple %i\n"
		"wait;wait;rcon g_valkyrMode %i\n"
		"wait;wait;rcon g_spawnProtectionNoFire %i\n"
		"wait;wait;rcon g_spawnProtection %f\n"
		"wait;wait;rcon g_invisFire %i\n"
		"wait;wait;rcon g_invisFlicker %f\n"
		"wait;wait;rcon g_invis %i\n"
		"wait;wait;rcon g_anticampTime %f\n"
		"wait;wait;rcon g_anticampRadius %f\n"
		"wait;wait;rcon g_anticamp %i\n"
		"wait;wait;rcon g_grappleSpeed %i\n"
		"wait;wait;rcon g_grapplePull %i\n"
		"wait;wait;rcon g_firerate %f\n"
		"wait;wait;rcon g_headHunt %i\n"
		"wait;wait;rcon g_headFromCorpse %i\n"
		"wait;wait;rcon g_railgunDelayedFire %i\n"
		"wait;wait;rcon g_railgunFireDelay %i\n",
		s_server_preferences.scoring.curvalue,
		s_server_preferences.space.curvalue,
		s_server_preferences.worlddamage.curvalue,
		s_server_preferences.splashdamage.curvalue,
		(int)Com_Clamp( 0, 99, atoi( s_server_preferences.bounces.field.buffer ) ),
		s_server_preferences.suicide.curvalue,
		s_server_preferences.alwaysgib.curvalue,
		s_server_preferences.allowquad.curvalue,
		s_server_preferences.allowhaste.curvalue,
		s_server_preferences.allowinvis.curvalue,
		s_server_preferences.grapple.curvalue,
		s_server_preferences.valkyrmode.curvalue,
		s_server_preferences.spawnnofire.curvalue,
		Com_Clamp( 0, 99, atof( s_server_preferences.spawntime.field.buffer ) ),
		s_server_preferences.invisfire.curvalue,
		Com_Clamp( 0, 100, atof( s_server_preferences.invisflicker.field.buffer ) ),
		s_server_preferences.invisgame.curvalue,
		Com_Clamp( 0, 99, atof( s_server_preferences.anticamptime.field.buffer ) ),
		Com_Clamp( 0, 999, atof( s_server_preferences.anticampradius.field.buffer ) ),
		s_server_preferences.anticamp.curvalue,
		(int)Com_Clamp( 0, 9999, atoi( s_server_preferences.grapplespeed.field.buffer ) ),
		(int)Com_Clamp( 0, 9999, atoi( s_server_preferences.grapplepull.field.buffer ) ),
		temp,
		s_server_preferences.headhunt.curvalue,
		s_server_preferences.headfromcorpse.curvalue,
		s_server_preferences.delayedfire.curvalue,
		(int)Com_Clamp( 0, 9999, atoi( s_server_preferences.firedelay.field.buffer ) )
		);

	trap_Cmd_ExecuteText( EXEC_APPEND, rconString );
}

/*
=================
server_preferences_Save
=================
*/

void server_preferences_Save( void ) {
	float temp;
	trap_Cvar_Set( "rconAddress", s_server_preferences.rconAddress.field.buffer );
	trap_Cvar_Set( "rconPassword", s_server_preferences.rconPassword.field.buffer );

	// the on/off values are saved on changing them, expect ofcourse for the rcon
	if ( !s_server_preferences.remoteServer )  {
		temp = atoi( s_server_preferences.firerate.field.buffer ) / 100.0;
		temp = temp > 25 ? 25 : temp;
		temp = temp <= 0 ? 1 : temp;
		trap_Cvar_SetValue ("g_firerate", temp );
		trap_Cvar_SetValue ("g_grappleSpeed", Com_Clamp( 0, 9999, atoi( s_server_preferences.grapplespeed.field.buffer ) ) );
		trap_Cvar_SetValue ("g_grapplePull", Com_Clamp( 0, 9999, atoi( s_server_preferences.grapplepull.field.buffer ) ) );
		trap_Cvar_SetValue ("g_railgunBounces", Com_Clamp( 0, 99, atoi( s_server_preferences.bounces.field.buffer ) ) );
		trap_Cvar_SetValue ("g_spawnProtection", Com_Clamp( 0, 99, atof( s_server_preferences.spawntime.field.buffer ) ) );
		trap_Cvar_SetValue ("g_invisFlicker", Com_Clamp( 0, 100, atof( s_server_preferences.invisflicker.field.buffer ) ) );
		trap_Cvar_SetValue ("g_anticampTime", Com_Clamp( 0, 99, atof( s_server_preferences.anticamptime.field.buffer ) ) );
		trap_Cvar_SetValue ("g_anticampRadius", Com_Clamp( 0, 999, atof( s_server_preferences.anticampradius.field.buffer ) ) );
		trap_Cvar_SetValue ("g_allowGrapple", s_server_preferences.grapple.curvalue );
		trap_Cvar_SetValue ("g_railgunFireDelay", Com_Clamp( 0, 9999, atof( s_server_preferences.firedelay.field.buffer ) ) );
	}
}

/*
=================
server_preferences_Event
=================
*/

static void server_preferences_Event( void* ptr, int notification ) {
	float temp;

	if( notification != QM_ACTIVATED ) {
		return;
	}

	if ( ((menucommon_s*)ptr)->id == ID_SENDRCONCOMMANDS ) {
		trap_Cvar_Set( "rconAddress", s_server_preferences.rconAddress.field.buffer );
		trap_Cvar_Set( "rconPassword", s_server_preferences.rconPassword.field.buffer );
		UI_SendRconCommands();
		return;
	}

	// never set local cvars with remote values you dumbass!

	switch( ((menucommon_s*)ptr)->id ) {

	case ID_SCORING:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_scoring", s_server_preferences.scoring.curvalue );
		if ( s_server_preferences.scoring.curvalue ) {
			s_server_preferences.headhunt.curvalue = 0;
			trap_Cvar_SetValue( "g_headHunt", 0 );
			s_server_preferences.headfromcorpse.generic.flags |= QMF_GRAYED;
		}
		break;

	case ID_SPACE:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_spaceProtection", s_server_preferences.space.curvalue );
		break;

	case ID_SPLASHDAMAGE:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_railgunExplosions", s_server_preferences.splashdamage.curvalue );
		break;

	case ID_WORLDDAMAGE:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_worldDamage", s_server_preferences.worlddamage.curvalue );
		break;

	case ID_ALWAYSGIB:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_alwaysGib", s_server_preferences.alwaysgib.curvalue );
		break;

	case ID_ALLOWQUAD:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_allowQuad", s_server_preferences.allowquad.curvalue );
		break;

	case ID_ALLOWHASTE:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_allowHaste", s_server_preferences.allowhaste.curvalue );
		break;

	case ID_ALLOWINVIS:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_allowInvis", s_server_preferences.allowinvis.curvalue );
		break;

	case ID_VALKYRMODE:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_valkyrMode", s_server_preferences.valkyrmode.curvalue );
		break;

	case ID_ALLOWGRAPPLE:
		if ( !s_server_preferences.grapple.curvalue ) {
			s_server_preferences.grapplespeed.generic.flags |= QMF_GRAYED;
			s_server_preferences.grapplepull.generic.flags |= QMF_GRAYED;
		} else {
			s_server_preferences.grapplespeed.generic.flags &= ~QMF_GRAYED;
			s_server_preferences.grapplepull.generic.flags &= ~QMF_GRAYED;
		}
		break;

	case ID_INVISGAME:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_invis", s_server_preferences.invisgame.curvalue );
		if ( s_server_preferences.invisgame.curvalue ) {
			s_server_preferences.invisfire.generic.flags &= ~QMF_GRAYED;
			s_server_preferences.invisflicker.generic.flags &= ~QMF_GRAYED;
		} else {
			s_server_preferences.invisfire.generic.flags |= QMF_GRAYED;
			s_server_preferences.invisflicker.generic.flags |= QMF_GRAYED;
		}
		break;

	case ID_INVISFIRE:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_invisFire", s_server_preferences.invisfire.curvalue );
		break;

	case ID_ANTICAMP:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_anticamp", s_server_preferences.anticamp.curvalue );
		if ( s_server_preferences.anticamp.curvalue ) {
			s_server_preferences.anticamptime.generic.flags &= ~QMF_GRAYED;
			s_server_preferences.anticampradius.generic.flags &= ~QMF_GRAYED;
		} else {
			s_server_preferences.anticamptime.generic.flags |= QMF_GRAYED;
			s_server_preferences.anticampradius.generic.flags |= QMF_GRAYED;
		}
		break;

	case ID_SPAWNNOFIRE:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_spawnProtectionNoFire", s_server_preferences.spawnnofire.curvalue );
		break;

	case ID_HEADHUNT:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_headHunt", s_server_preferences.headhunt.curvalue );
		if ( s_server_preferences.headhunt.curvalue ) {
			s_server_preferences.scoring.curvalue = 0;
			trap_Cvar_SetValue( "g_scoring", 0 );
			s_server_preferences.headfromcorpse.generic.flags &= ~QMF_GRAYED;
		} else {
			s_server_preferences.headfromcorpse.generic.flags |= QMF_GRAYED;
		}
		break;

	case ID_HEADFROMCORPSE:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_headFromCorpse", s_server_preferences.headfromcorpse.curvalue );
		break;

	case ID_DELAYEDFIRE:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_railgunDelayedFire", s_server_preferences.delayedfire.curvalue );
		if ( s_server_preferences.delayedfire.curvalue ) {
			s_server_preferences.firedelay.generic.flags &= ~QMF_GRAYED;
		} else {
			s_server_preferences.firedelay.generic.flags |= QMF_GRAYED;
		}
		break;

	case ID_SUICIDE:
		if ( !s_server_preferences.remoteServer ) trap_Cvar_SetValue( "g_railgunSuicide", s_server_preferences.suicide.curvalue );
		break;


	case ID_BACK:
		server_preferences_Save();
		UI_PopMenu();
		break;
	}
}

/*
=================
server_preferences_StatusBar
=================
*/
static void server_preferences_StatusBar( void* ptr ) {
	const char *string1, *string2;

	string1 = "DEBUG!";
	string2 = NULL;

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_SCORING:
		string1 = "Enable scoring by accuracy";
		break;
	case ID_SPACE:
		string1 = "Enable space protection";
		break;
	case ID_SPLASHDAMAGE:
		string1 = "Enable splash damage effects";
		break;
	case ID_ALLOWGRAPPLE:
		string1 = "Choose grapple type";
		break;
	case ID_ALLOWQUAD:
		string1 = "Allow Quad Damage in this game";
		break;
	case ID_ALLOWHASTE:
		string1 = "Allow Haste in this game";
		break;
	case ID_ALLOWINVIS:
		string1 = "Allow Invisibility in this game";
		break;
	case ID_ALWAYSGIB:
		string1 = "Always gib players when they die";
		break;
	case ID_VALKYRMODE:
		string1 = "Enable some trippy graphics";
		break;
	case ID_SPAWNTIME:
		string1 = "Duration of the spawn protection";
		break;
	case ID_BOUNCES:
		string1 = "Controls how many times the shots will bounce";
		break;
	case ID_SUICIDE:
		string1 = "Controls whether you can kill yourself with a";
		string2 = "bounced shot";
		break;
	case ID_GRAPPLESPEED:
		string1 = "Speed at which the grapple will travel";
		break;
	case ID_GRAPPLEPULL:
		string1 = "Speed at which players will be pulled";
		string2 = "towards the grapple point";
		break;
	case ID_SPAWNNOFIRE:
		string1 = "When on, players can not fire";
		string2 = "during the spawn protection";
		break;
	case ID_INVISGAME:
		string1 = "When on, players will be partially invisible";
		break;
	case ID_INVISFIRE:
		string1 = "Controls whether players will";
		string2 = "become visible when firing";
		break;
	case ID_INVISFLICKER:
		string1 = "Range: 0 - 100. Controls how much";
		string2 = "the invisibility will 'flicker'";
		break;
	case ID_ANTICAMP:
		string1 = "Enable anti-camp measures";
		break;
	case ID_ANTICAMPTIME:
		string1 = "Controls how long players may camp";
		break;
	case ID_ANTICAMPRADIUS:
		string1 = "Controls how far a player must move";
		string2 = "to avoid the anti-camp measures";
		break;
	case ID_HEADHUNT:
		string1 = "When on, players must collect heads to score";
		break;
	case ID_HEADFROMCORPSE:
		string1 = "When on, players can shoot corpses to get heads";
		break;
	case ID_DELAYEDFIRE:
		string1 = "When on, the railgun will charge before it fires";
		break;
	case ID_FIREDELAY:
		string1 = "Controls how long the railgun will charge";
		break;
	case ID_WORLDDAMAGE:
		string1 = "Controls whether players can be";
		string2 = "damaged by world damage (lava, slime)";
		break;
	case ID_RCONIP:
		string1 = "Enter the IP of the server you wish to control";
		break;
	case ID_RCONPASSWORD:
		string1 = "Enter the RCON password of";
		string2 = "the server you wish to control";
		break;
	case ID_SENDRCONCOMMANDS:
		string1 = "Push this to update the remote server's";
		string2 = "configuration to the current settings";
		break;
	case ID_FIRERATE:
		string1 = "Controls how fast the railgun will fire,";
		string2 = "100% is the default rate, 200% is double, etc.";
		break;
	}

	UI_DrawString( 320, 445, string1, UI_CENTER|UI_SMALLFONT, colorWhite );
	if( string2 ) {
		UI_DrawString( 320, 465, string2, UI_CENTER|UI_SMALLFONT, colorWhite );
	}
}

/*
=================
s_server_preferences_MenuKey
=================
*/
static sfxHandle_t s_server_preferences_MenuKey( int key ) {
	// catch esc and mouse2 so we don't quit the menu without confirming the changes
	switch( key ) {
	case K_MOUSE2:
		server_preferences_Save();	// confirm changes
		break;
	case K_ESCAPE:
		server_preferences_Save();	// confirm changes
		break;
	}

	return Menu_DefaultKey( &s_server_preferences.menu, key );
}

/*
=================
server_preferences_MenuInit
=================
*/
static void server_preferences_MenuInit( qboolean allowConfig ) {
	int				y;

	memset( &s_server_preferences, 0 ,sizeof(server_preferences_t) );

	server_preferences_Cache();

	s_server_preferences.menu.wrapAround 	= qtrue;
	s_server_preferences.menu.fullscreen 	= qtrue;
	s_server_preferences.menu.key			= s_server_preferences_MenuKey;	// keycatcher

	s_server_preferences.allowConfig = allowConfig;
	s_server_preferences.localServer = trap_Cvar_VariableValue( "sv_running" );

/*	if ( !(s_server_preferences.localServer || allowConfig) ) {
		char buf[128];
		trap_Cvar_VariableStringBuffer("rconPassword", buf, sizeof(buf) );
		s_server_preferences.remoteServer = (Q_stricmp(buf, "") && 1);
	} else*/ s_server_preferences.remoteServer = 0;

	s_server_preferences.banner.generic.type		= MTYPE_BTEXT;
	s_server_preferences.banner.generic.x			= 320;
	s_server_preferences.banner.generic.y			= 16;
	if ( s_server_preferences.localServer || s_server_preferences.allowConfig ) {
		s_server_preferences.banner.string = "LOCAL SERVER OPTIONS";
	} else if ( s_server_preferences.remoteServer ) {
		s_server_preferences.banner.string = "REMOTE SERVER OPTIONS";
	} else {
		s_server_preferences.banner.string = "SERVER OPTIONS";
	}
	s_server_preferences.banner.color				= color_white;
	s_server_preferences.banner.style				= UI_CENTER;

	s_server_preferences.framel.generic.type		= MTYPE_BITMAP;
	s_server_preferences.framel.generic.name		= ART_FRAMEL;
	s_server_preferences.framel.generic.flags		= QMF_INACTIVE;
	s_server_preferences.framel.generic.x			= 0;
	s_server_preferences.framel.generic.y			= 78;
	s_server_preferences.framel.width				= 256;
	s_server_preferences.framel.height			= 329;

	s_server_preferences.framer.generic.type		= MTYPE_BITMAP;
	s_server_preferences.framer.generic.name		= ART_FRAMER;
	s_server_preferences.framer.generic.flags		= QMF_INACTIVE;
	s_server_preferences.framer.generic.x			= 376;
	s_server_preferences.framer.generic.y			= 76;
	s_server_preferences.framer.width				= 256;
	s_server_preferences.framer.height  	  	 	= 334;

	y = 64;
/*	s_server_preferences.rconAddress.generic.type			= MTYPE_FIELD;
	s_server_preferences.rconAddress.generic.name			= "Rcon Address:";
	s_server_preferences.rconAddress.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.rconAddress.generic.x				= 320; //SERVER_PREFERENCES_X_POS + 250;
	s_server_preferences.rconAddress.generic.y				= y;
	s_server_preferences.rconAddress.field.widthInChars		= 21;
	s_server_preferences.rconAddress.field.maxchars			= 64;
	s_server_preferences.rconAddress.generic.statusbar		= server_preferences_StatusBar;
	s_server_preferences.rconAddress.generic.id				= ID_RCONIP;


	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.rconPassword.generic.type			= MTYPE_FIELD;
	s_server_preferences.rconPassword.generic.name			= "Rcon Password:";
	s_server_preferences.rconPassword.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.rconPassword.generic.x				= 320; //SERVER_PREFERENCES_X_POS + 250;
	s_server_preferences.rconPassword.generic.y				= y;
	s_server_preferences.rconPassword.field.widthInChars	= 21;
	s_server_preferences.rconPassword.field.maxchars		= 64;
	s_server_preferences.rconPassword.generic.statusbar 	= server_preferences_StatusBar;
	s_server_preferences.rconPassword.generic.id			= ID_RCONPASSWORD;

	y += 2*BIGCHAR_HEIGHT+2;
	s_server_preferences.sendRconCommands.generic.type		= MTYPE_PTEXT;
	s_server_preferences.sendRconCommands.generic.flags		= QMF_PULSEIFFOCUS|QMF_CENTER_JUSTIFY|QMF_SMALLFONT;
	s_server_preferences.sendRconCommands.generic.x			= 320;
	s_server_preferences.sendRconCommands.generic.y			= y;
	s_server_preferences.sendRconCommands.generic.callback	= server_preferences_Event;
	s_server_preferences.sendRconCommands.generic.id		= ID_SENDRCONCOMMANDS;
	s_server_preferences.sendRconCommands.color				= color_red;
	s_server_preferences.sendRconCommands.string			= "Send rcon commands";
	s_server_preferences.sendRconCommands.style				= UI_CENTER|UI_SMALLFONT;
	s_server_preferences.sendRconCommands.generic.statusbar = server_preferences_StatusBar;
*/
	y += 2*BIGCHAR_HEIGHT+2;
	s_server_preferences.scoring.generic.type    			= MTYPE_RADIOBUTTON;
	s_server_preferences.scoring.generic.name				= "Scoring by accuracy:";
	s_server_preferences.scoring.generic.flags				= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.scoring.generic.callback			= server_preferences_Event;
	s_server_preferences.scoring.generic.id					= ID_SCORING;
	s_server_preferences.scoring.generic.x					= SERVER_PREFERENCES_X_POS;
	s_server_preferences.scoring.generic.y					= y;
	s_server_preferences.scoring.generic.statusbar			= server_preferences_StatusBar;


	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.space.generic.type					= MTYPE_RADIOBUTTON;
	s_server_preferences.space.generic.name					= "Space protection:";
	s_server_preferences.space.generic.flags				= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.space.generic.callback				= server_preferences_Event;
	s_server_preferences.space.generic.id					= ID_SPACE;
	s_server_preferences.space.generic.x					= SERVER_PREFERENCES_X_POS;
	s_server_preferences.space.generic.y					= y;
	s_server_preferences.space.generic.statusbar			= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.splashdamage.generic.type			= MTYPE_RADIOBUTTON;
	s_server_preferences.splashdamage.generic.name			= "Splash damage effects:";
	s_server_preferences.splashdamage.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.splashdamage.generic.callback		= server_preferences_Event;
	s_server_preferences.splashdamage.generic.id			= ID_SPLASHDAMAGE;
	s_server_preferences.splashdamage.generic.x				= SERVER_PREFERENCES_X_POS;
	s_server_preferences.splashdamage.generic.y				= y;
	s_server_preferences.splashdamage.generic.statusbar 	= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.worlddamage.generic.type			= MTYPE_RADIOBUTTON;
	s_server_preferences.worlddamage.generic.name			= "World damage:";
	s_server_preferences.worlddamage.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.worlddamage.generic.callback		= server_preferences_Event;
	s_server_preferences.worlddamage.generic.id				= ID_WORLDDAMAGE;
	s_server_preferences.worlddamage.generic.x				= SERVER_PREFERENCES_X_POS;
	s_server_preferences.worlddamage.generic.y				= y;
	s_server_preferences.worlddamage.generic.statusbar		= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.alwaysgib.generic.type				= MTYPE_RADIOBUTTON;
	s_server_preferences.alwaysgib.generic.name				= "Always gib:";
	s_server_preferences.alwaysgib.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.alwaysgib.generic.callback			= server_preferences_Event;
	s_server_preferences.alwaysgib.generic.id				= ID_ALWAYSGIB;
	s_server_preferences.alwaysgib.generic.x				= SERVER_PREFERENCES_X_POS;
	s_server_preferences.alwaysgib.generic.y				= y;
	s_server_preferences.alwaysgib.generic.statusbar		= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.allowquad.generic.type				= MTYPE_RADIOBUTTON;
	s_server_preferences.allowquad.generic.name				= "Allow Quad Damage:";
	s_server_preferences.allowquad.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.allowquad.generic.callback			= server_preferences_Event;
	s_server_preferences.allowquad.generic.id				= ID_ALLOWQUAD;
	s_server_preferences.allowquad.generic.x				= SERVER_PREFERENCES_X_POS;
	s_server_preferences.allowquad.generic.y				= y;
	s_server_preferences.allowquad.generic.statusbar		= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.allowhaste.generic.type			= MTYPE_RADIOBUTTON;
	s_server_preferences.allowhaste.generic.name			= "Allow Haste:";
	s_server_preferences.allowhaste.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.allowhaste.generic.callback		= server_preferences_Event;
	s_server_preferences.allowhaste.generic.id				= ID_ALLOWHASTE;
	s_server_preferences.allowhaste.generic.x				= SERVER_PREFERENCES_X_POS;
	s_server_preferences.allowhaste.generic.y				= y;
	s_server_preferences.allowhaste.generic.statusbar		= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.allowinvis.generic.type			= MTYPE_RADIOBUTTON;
	s_server_preferences.allowinvis.generic.name			= "Allow Invisibility:";
	s_server_preferences.allowinvis.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.allowinvis.generic.callback		= server_preferences_Event;
	s_server_preferences.allowinvis.generic.id				= ID_ALLOWINVIS;
	s_server_preferences.allowinvis.generic.x				= SERVER_PREFERENCES_X_POS;
	s_server_preferences.allowinvis.generic.y				= y;
	s_server_preferences.allowinvis.generic.statusbar		= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.valkyrmode.generic.type			= MTYPE_RADIOBUTTON;
	s_server_preferences.valkyrmode.generic.name			= "^2V^1a^3l^4k^5y^6r ^1Mode:";
	s_server_preferences.valkyrmode.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.valkyrmode.generic.callback		= server_preferences_Event;
	s_server_preferences.valkyrmode.generic.id				= ID_VALKYRMODE;
	s_server_preferences.valkyrmode.generic.x				= SERVER_PREFERENCES_X_POS;
	s_server_preferences.valkyrmode.generic.y				= y;
	s_server_preferences.valkyrmode.generic.statusbar		= server_preferences_StatusBar;

	y += 2*BIGCHAR_HEIGHT;
	s_server_preferences.spawntime.generic.type				= MTYPE_FIELD;
	s_server_preferences.spawntime.generic.name				= "Spawn protection:";
	s_server_preferences.spawntime.generic.flags			= QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.spawntime.generic.x				= SERVER_PREFERENCES_X_POS;
	s_server_preferences.spawntime.generic.y				= y;
	s_server_preferences.spawntime.generic.id				= ID_SPAWNTIME;
	s_server_preferences.spawntime.field.widthInChars		= 2;
	s_server_preferences.spawntime.field.maxchars			= 2;
	s_server_preferences.spawntime.generic.statusbar		= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.spawnnofire.generic.type			= MTYPE_RADIOBUTTON;
	s_server_preferences.spawnnofire.generic.name			= "No fire when protected:";
	s_server_preferences.spawnnofire.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.spawnnofire.generic.callback		= server_preferences_Event;
	s_server_preferences.spawnnofire.generic.id				= ID_SPAWNNOFIRE;
	s_server_preferences.spawnnofire.generic.x				= SERVER_PREFERENCES_X_POS;
	s_server_preferences.spawnnofire.generic.y				= y;
	s_server_preferences.spawnnofire.generic.statusbar		= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.firerate.generic.type				= MTYPE_FIELD;
	s_server_preferences.firerate.generic.name				= "Firerate X 100%:";
	s_server_preferences.firerate.generic.flags				= QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.firerate.generic.x					= SERVER_PREFERENCES_X_POS;
	s_server_preferences.firerate.generic.y					= y;
	s_server_preferences.firerate.generic.id				= ID_FIRERATE;
	s_server_preferences.firerate.field.widthInChars		= 4;
	s_server_preferences.firerate.field.maxchars			= 4;
	s_server_preferences.firerate.generic.statusbar			= server_preferences_StatusBar;

	y += 2*BIGCHAR_HEIGHT;
	s_server_preferences.bounces.generic.type				= MTYPE_FIELD;
	s_server_preferences.bounces.generic.name				= "Slug bounces:";
	s_server_preferences.bounces.generic.flags				= QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.bounces.generic.x					= SERVER_PREFERENCES_X_POS;
	s_server_preferences.bounces.generic.y					= y;
	s_server_preferences.bounces.generic.id					= ID_BOUNCES;
	s_server_preferences.bounces.field.widthInChars			= 2;
	s_server_preferences.bounces.field.maxchars				= 2;
	s_server_preferences.bounces.generic.statusbar			= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.suicide.generic.type				= MTYPE_RADIOBUTTON;
	s_server_preferences.suicide.generic.name				= "Allow suicide:";
	s_server_preferences.suicide.generic.flags				= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.suicide.generic.callback			= server_preferences_Event;
	s_server_preferences.suicide.generic.id					= ID_SUICIDE;
	s_server_preferences.suicide.generic.x					= SERVER_PREFERENCES_X_POS;
	s_server_preferences.suicide.generic.y					= y;
	s_server_preferences.suicide.generic.statusbar			= server_preferences_StatusBar;

	y = s_server_preferences.scoring.generic.y;
	s_server_preferences.grapple.generic.type				= MTYPE_SPINCONTROL;
	s_server_preferences.grapple.generic.name				= "Offhand grapple:";
	s_server_preferences.grapple.generic.flags				= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.grapple.generic.x					= SERVER_PREFERENCES_X_POS + 270;
	s_server_preferences.grapple.generic.y					= y;
	s_server_preferences.grapple.generic.id					= ID_ALLOWGRAPPLE;
	s_server_preferences.grapple.generic.callback			= server_preferences_Event;
	s_server_preferences.grapple.itemnames					= grapplelist;
	s_server_preferences.grapple.generic.statusbar			= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.grapplespeed.generic.type			= MTYPE_FIELD;
	s_server_preferences.grapplespeed.generic.name			= "Grapple speed:";
	s_server_preferences.grapplespeed.generic.flags			= QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.grapplespeed.generic.x				= SERVER_PREFERENCES_X_POS + 270;
	s_server_preferences.grapplespeed.generic.y				= y;
	s_server_preferences.grapplespeed.generic.id			= ID_GRAPPLESPEED;
	s_server_preferences.grapplespeed.field.widthInChars	= 4;
	s_server_preferences.grapplespeed.field.maxchars		= 4;
	s_server_preferences.grapplespeed.generic.statusbar 	= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.grapplepull.generic.type			= MTYPE_FIELD;
	s_server_preferences.grapplepull.generic.name			= "Grapple pull:";
	s_server_preferences.grapplepull.generic.flags			= QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.grapplepull.generic.x				= SERVER_PREFERENCES_X_POS + 270;
	s_server_preferences.grapplepull.generic.id				= ID_GRAPPLEPULL;
	s_server_preferences.grapplepull.generic.y				= y;
	s_server_preferences.grapplepull.field.widthInChars		= 4;
	s_server_preferences.grapplepull.field.maxchars			= 4;
	s_server_preferences.grapplepull.generic.statusbar		= server_preferences_StatusBar;

	y += 2*BIGCHAR_HEIGHT;
	s_server_preferences.invisgame.generic.type				= MTYPE_RADIOBUTTON;
	s_server_preferences.invisgame.generic.name				= "Invisibility game:";
	s_server_preferences.invisgame.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.invisgame.generic.callback			= server_preferences_Event;
	s_server_preferences.invisgame.generic.id				= ID_INVISGAME;
	s_server_preferences.invisgame.generic.x				= SERVER_PREFERENCES_X_POS + 270;
	s_server_preferences.invisgame.generic.y				= y;
	s_server_preferences.invisgame.generic.statusbar		= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.invisfire.generic.type				= MTYPE_RADIOBUTTON;
	s_server_preferences.invisfire.generic.name				= "Invisible when firing:";
	s_server_preferences.invisfire.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.invisfire.generic.callback			= server_preferences_Event;
	s_server_preferences.invisfire.generic.id				= ID_INVISFIRE;
	s_server_preferences.invisfire.generic.x				= SERVER_PREFERENCES_X_POS + 270;
	s_server_preferences.invisfire.generic.y				= y;
	s_server_preferences.invisfire.generic.statusbar		= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.invisflicker.generic.type			= MTYPE_FIELD;
	s_server_preferences.invisflicker.generic.name			= "Invisibility flicker:";
	s_server_preferences.invisflicker.generic.flags			= QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.invisflicker.generic.x				= SERVER_PREFERENCES_X_POS + 270;
	s_server_preferences.invisflicker.generic.y				= y;
	s_server_preferences.invisflicker.generic.id			= ID_INVISFLICKER;
	s_server_preferences.invisflicker.field.widthInChars	= 2;
	s_server_preferences.invisflicker.field.maxchars		= 2;
	s_server_preferences.invisflicker.generic.statusbar 	= server_preferences_StatusBar;

	y += 2*BIGCHAR_HEIGHT;
	s_server_preferences.anticamp.generic.type				= MTYPE_RADIOBUTTON;
	s_server_preferences.anticamp.generic.name				= "Anti camper:";
	s_server_preferences.anticamp.generic.flags				= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.anticamp.generic.callback			= server_preferences_Event;
	s_server_preferences.anticamp.generic.id				= ID_ANTICAMP;
	s_server_preferences.anticamp.generic.x					= SERVER_PREFERENCES_X_POS + 270;
	s_server_preferences.anticamp.generic.y					= y;
	s_server_preferences.anticamp.generic.statusbar			= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.anticamptime.generic.type			= MTYPE_FIELD;
	s_server_preferences.anticamptime.generic.name			= "Anti camper time:";
	s_server_preferences.anticamptime.generic.flags			= QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.anticamptime.generic.x				= SERVER_PREFERENCES_X_POS + 270;
	s_server_preferences.anticamptime.generic.y				= y;
	s_server_preferences.anticamptime.field.widthInChars	= 2;
	s_server_preferences.anticamptime.field.maxchars		= 2;
	s_server_preferences.anticamptime.generic.id			= ID_ANTICAMPTIME;
	s_server_preferences.anticamptime.generic.statusbar 	= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.anticampradius.generic.type		= MTYPE_FIELD;
	s_server_preferences.anticampradius.generic.name		= "Anti camper radius:";
	s_server_preferences.anticampradius.generic.flags		= QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.anticampradius.generic.x			= SERVER_PREFERENCES_X_POS + 270;
	s_server_preferences.anticampradius.generic.y			= y;
	s_server_preferences.anticampradius.field.widthInChars	= 3;
	s_server_preferences.anticampradius.field.maxchars		= 3;
	s_server_preferences.anticampradius.generic.id			= ID_ANTICAMPRADIUS;
	s_server_preferences.anticampradius.generic.statusbar	= server_preferences_StatusBar;

	y += 2*BIGCHAR_HEIGHT;
	s_server_preferences.headhunt.generic.type				= MTYPE_RADIOBUTTON;
	s_server_preferences.headhunt.generic.name				= "Head hunter game:";
	s_server_preferences.headhunt.generic.flags				= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.headhunt.generic.callback			= server_preferences_Event;
	s_server_preferences.headhunt.generic.id				= ID_HEADHUNT;
	s_server_preferences.headhunt.generic.x					= SERVER_PREFERENCES_X_POS + 270;
	s_server_preferences.headhunt.generic.y					= y;
	s_server_preferences.headhunt.generic.statusbar			= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.headfromcorpse.generic.type		= MTYPE_RADIOBUTTON;
	s_server_preferences.headfromcorpse.generic.name		= "Heads from corpses:";
	s_server_preferences.headfromcorpse.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.headfromcorpse.generic.callback	= server_preferences_Event;
	s_server_preferences.headfromcorpse.generic.id			= ID_HEADFROMCORPSE;
	s_server_preferences.headfromcorpse.generic.x			= SERVER_PREFERENCES_X_POS + 270;
	s_server_preferences.headfromcorpse.generic.y			= y;
	s_server_preferences.headfromcorpse.generic.statusbar	= server_preferences_StatusBar;

	y += 2*BIGCHAR_HEIGHT;
	s_server_preferences.delayedfire.generic.type			= MTYPE_RADIOBUTTON;
	s_server_preferences.delayedfire.generic.name			= "Delayed railgun fire:";
	s_server_preferences.delayedfire.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.delayedfire.generic.callback		= server_preferences_Event;
	s_server_preferences.delayedfire.generic.id				= ID_DELAYEDFIRE;
	s_server_preferences.delayedfire.generic.x				= SERVER_PREFERENCES_X_POS + 270;
	s_server_preferences.delayedfire.generic.y				= y;
	s_server_preferences.delayedfire.generic.statusbar		= server_preferences_StatusBar;

	y += BIGCHAR_HEIGHT+2;
	s_server_preferences.firedelay.generic.type				= MTYPE_FIELD;
	s_server_preferences.firedelay.generic.name				= "Railgun fire delay:";
	s_server_preferences.firedelay.generic.flags			= QMF_NUMBERSONLY|QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_server_preferences.firedelay.generic.x				= SERVER_PREFERENCES_X_POS + 270;
	s_server_preferences.firedelay.generic.y				= y;
	s_server_preferences.firedelay.field.widthInChars		= 3;
	s_server_preferences.firedelay.field.maxchars			= 3;
	s_server_preferences.firedelay.generic.id				= ID_FIREDELAY;
	s_server_preferences.firedelay.generic.statusbar		= server_preferences_StatusBar;

	if ( !s_server_preferences.allowConfig &&
			!s_server_preferences.localServer &&
			!s_server_preferences.remoteServer ) {
		s_server_preferences.scoring.generic.flags |= QMF_GRAYED;
		s_server_preferences.space.generic.flags |= QMF_GRAYED;
		s_server_preferences.splashdamage.generic.flags |= QMF_GRAYED;
		s_server_preferences.alwaysgib.generic.flags |= QMF_GRAYED;
		s_server_preferences.allowquad.generic.flags |= QMF_GRAYED;
		s_server_preferences.allowhaste.generic.flags |= QMF_GRAYED;
		s_server_preferences.allowinvis.generic.flags |= QMF_GRAYED;
		s_server_preferences.valkyrmode.generic.flags |= QMF_GRAYED;
		s_server_preferences.grapple.generic.flags |= QMF_GRAYED;
		s_server_preferences.spawnnofire.generic.flags |= QMF_GRAYED;
		s_server_preferences.grapplespeed.generic.flags |= QMF_GRAYED;
		s_server_preferences.grapplepull.generic.flags |= QMF_GRAYED;
		s_server_preferences.spawntime.generic.flags |= QMF_GRAYED;
		s_server_preferences.firerate.generic.flags |= QMF_GRAYED;
		s_server_preferences.bounces.generic.flags |= QMF_GRAYED;
		s_server_preferences.invisgame.generic.flags |= QMF_GRAYED;
		s_server_preferences.invisfire.generic.flags |= QMF_GRAYED;
		s_server_preferences.invisflicker.generic.flags |= QMF_GRAYED;
		s_server_preferences.anticamp.generic.flags |= QMF_GRAYED;
		s_server_preferences.anticamptime.generic.flags |= QMF_GRAYED;
		s_server_preferences.anticampradius.generic.flags |= QMF_GRAYED;
		s_server_preferences.headfromcorpse.generic.flags |= QMF_GRAYED;
		s_server_preferences.headhunt.generic.flags	|= QMF_GRAYED;
		s_server_preferences.delayedfire.generic.flags	|= QMF_GRAYED;
		s_server_preferences.firedelay.generic.flags	|= QMF_GRAYED;
		s_server_preferences.worlddamage.generic.flags	|= QMF_GRAYED;
		s_server_preferences.suicide.generic.flags	|= QMF_GRAYED;
	}

	s_server_preferences.back.generic.type			= MTYPE_BITMAP;
	s_server_preferences.back.generic.name			= ART_BACK0;
	s_server_preferences.back.generic.flags			= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_server_preferences.back.generic.callback		= server_preferences_Event;
	s_server_preferences.back.generic.id			= ID_BACK;
	s_server_preferences.back.generic.x			= 0;
	s_server_preferences.back.generic.y			= 480-64;
	s_server_preferences.back.width				= 128;
	s_server_preferences.back.height				= 64;
	s_server_preferences.back.focuspic			= ART_BACK1;

	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.banner );
//	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.framel );
//	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.framer );

//	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.rconAddress );
//	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.rconPassword );
//	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.sendRconCommands );

	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.scoring );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.space );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.splashdamage );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.worlddamage );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.alwaysgib );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.allowquad );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.allowhaste );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.allowinvis );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.valkyrmode );

	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.spawntime );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.spawnnofire );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.firerate );

	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.bounces );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.suicide );

	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.grapple );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.grapplespeed );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.grapplepull );

	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.invisgame );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.invisfire );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.invisflicker );

	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.anticamp );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.anticamptime );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.anticampradius );

	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.headhunt );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.headfromcorpse );

	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.delayedfire );
	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.firedelay );

	Menu_AddItem( &s_server_preferences.menu, &s_server_preferences.back );

	server_preferences_SetMenuItems();

	if ( !s_server_preferences.remoteServer ) {
		s_server_preferences.sendRconCommands.generic.flags |= QMF_GRAYED;
	}
	if ( !s_server_preferences.grapple.curvalue ) {
		s_server_preferences.grapplespeed.generic.flags |= QMF_GRAYED;
		s_server_preferences.grapplepull.generic.flags |= QMF_GRAYED;
	}
	if ( !s_server_preferences.invisgame.curvalue ) {
		s_server_preferences.invisfire.generic.flags |= QMF_GRAYED;
		s_server_preferences.invisflicker.generic.flags |= QMF_GRAYED;
	}
	if ( !s_server_preferences.anticamp.curvalue ) {
		s_server_preferences.anticamptime.generic.flags |= QMF_GRAYED;
		s_server_preferences.anticampradius.generic.flags |= QMF_GRAYED;
	}
	if ( !s_server_preferences.headhunt.curvalue ) {
		s_server_preferences.headfromcorpse.generic.flags |= QMF_GRAYED;
	}
	if ( !s_server_preferences.delayedfire.curvalue ) {
		s_server_preferences.firedelay.generic.flags |= QMF_GRAYED;
	}
}


/*
===============
server_preferences_Cache
===============
*/
void server_preferences_Cache( void ) {
	int		n;

	trap_R_RegisterShaderNoMip( ART_FRAMEL );
	trap_R_RegisterShaderNoMip( ART_FRAMER );
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
}


/*
===============
UI_server_preferencesMenu
===============
*/
void UI_server_preferencesMenu( qboolean allowConfig ) {
	server_preferences_MenuInit( allowConfig );
	UI_PushMenu( &s_server_preferences.menu );
}
