// Copyright (C) 1999-2000 Id Software, Inc.
//

/*
The work contained within this file is software written by various
copyright holders.  The initial contributor, Id Software holds all
copyright over their software.  However, software used and written by
and for UI Enhanced has copyrights held by the initial author of the
software.

The changes written by and for UI Enhanced are contained alongside the
original work from Id Software for convenience and ease of interoperability.

For the code contained herein that was written by Id Software, see
the license agreement on their original archive for restrictions and
limitations.

The UI Enhanced copyright owner permit free reuse of his code contained
herein, as long as the following terms are met:

----------------------------------------------------------------------
1) Credit is given in a place where users of the mod may read
   it. (Title screen, credit screen or README will do).  The
   recommended format is: "First, Last, alias, email"

2) There are no attempts to misrepresent the public as to who made the
   alterations.  The UI Enhanced copyright owner does not give permission
   for others to release software under the UI Enhanced name.
----------------------------------------------------------------------

Ian Jefferies   - HypoThermia ( uie@planetquake.com )
http://www.planetquake.com/uie
*/


#include "ui_local.h"

void UI_SPArena_Start( const char *arenaInfo ) {
	char	*map;
	int		level;
	int		n;
	char	*txt;
#if 0
	int 	timelimit, fraglimit;
#endif	

	n = (int)trap_Cvar_VariableValue( "sv_maxclients" );
	if ( n < 8 ) {
		trap_Cvar_SetValue( "sv_maxclients", 8 );
	}

	level = atoi( Info_ValueForKey( arenaInfo, "num" ) );
	txt = Info_ValueForKey( arenaInfo, "special" );
	if( txt[0] ) {
		if( Q_stricmp( txt, "training" ) == 0 ) {
			level = -4;
		}
		else if( Q_stricmp( txt, "final" ) == 0 ) {
			level = UI_GetNumSPTiers() * ARENAS_PER_TIER;
		}
	}
	trap_Cvar_SetValue( "ui_spSelection", level );

#if 0
	// fraglimit/timelimit bug fix
	txt = Info_ValueForKey( arenaInfo, "timelimit" );
	timelimit = atoi(txt);

	txt = Info_ValueForKey( arenaInfo, "fraglimit" );
	fraglimit = atoi(txt);

	trap_Cmd_ExecuteText( EXEC_APPEND, va( "timelimit %i\nfraglimit %i\nwriteconfig q3config\n", timelimit, fraglimit ) );
#endif

	map = Info_ValueForKey( arenaInfo, "map" );
	trap_Cmd_ExecuteText( EXEC_APPEND, va( "spmap %s\n", map ) );
}
