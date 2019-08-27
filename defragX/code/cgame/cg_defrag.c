//
// cg_defrag.c -- as much defrag-specific stuff as possible should go here

#include "cg_local.h"

int ar_starttime = 0;
int ar_stoptime = 0;
qbool readyToCopy = qfalse;

void CG_StartAutorecord( void ) {
	if ( !cg_autorecord.integer ) {
		return;
	}

	// if a demo is about to be saved, cut it short
	if ( ar_stoptime ) {
		ar_stoptime = cg.time;
	}

	if ( cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR || (cg.snap->ps.pm_flags & PMF_FOLLOW) ) {
		// stop recording if just joined spec and not about to save
		if ( !ar_stoptime ) {
#ifdef DEVBUILD			
			if ( cg_debugAutorecord.integer ) 
				Com_Printf( "^5stopping\n" );
#endif			
			trap_SendConsoleCommand( "silent stoprecord\n" );
		}
		return;
	}

	ar_starttime = cg.time;
}

void CG_RealStartAutorecord( void ) {
	int old_spdemo = CG_VariableIntegerValue( "ui_recordSPDemo" );
	int old_gsync = CG_VariableIntegerValue( "g_synchronousClients" );

	if ( !cg_autorecord.integer ) 
		return;

	trap_SendConsoleCommand( "set ui_recordSPDemo 1\n" );	   // disable RECORDING message
	trap_SendConsoleCommand( "set g_synchronousClients 1\n" ); // disable g_sync warning
	trap_SendConsoleCommand( "silent stoprecord\nsilent record temp/current\n" );

	if ( !old_spdemo ) 
		trap_SendConsoleCommand( "set ui_recordSPDemo 0\n" );

	if ( !old_gsync ) 
		trap_SendConsoleCommand( "set g_synchronousClients 0\n" );

#ifdef DEVBUILD
	if ( cg_debugAutorecord.integer ) 
		Com_Printf( "^5starting\n" );
#endif
	
}

void CG_StopAutorecord( void ) {
	if ( !cg_autorecord.integer ) 
		return;

	ar_stoptime = cg.time + 2000;
}

void CG_CheckAutorecord( void ) {
	if ( !cg_autorecord.integer ) 
		return;

	if ( cg.demoPlayback ) 
		return;

	if ( readyToCopy ) {
		int protocol = CG_VariableIntegerValue( "protocol" );

		readyToCopy = qfalse;

		if ( BG_CopyFile( va("demos/temp/current.dm_%d", protocol), va("demos/temp/prev.dm_%d", protocol) ) ) {
			Com_Printf("^3Demo ready to be saved.\n");
		} else {
			Com_Printf("^3Couldn't copy demo\n");
		}

		CG_StartAutorecord();
	} else if ( ar_stoptime && ar_stoptime <= cg.time ) {
		ar_stoptime = 0;
		trap_SendConsoleCommand( "silent stoprecord\n" );
		
#ifdef DEVBUILD		
		if ( cg_debugAutorecord.integer ) 
			Com_Printf("^5stopping\n");
#endif
		readyToCopy = qtrue; // copy next frame to give time for stoprecord command to get through
	} else if ( ar_starttime && ar_starttime <= cg.time ) {
		ar_starttime = 0;
		CG_RealStartAutorecord();
	}
}
