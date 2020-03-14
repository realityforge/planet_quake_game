////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __GM_DEBUGWINDOW_H__
#define __GM_DEBUGWINDOW_H__

#ifdef ENABLE_DEBUG_WINDOW

class gmMachine;
void gmBindDebugWindowLibrary(gmMachine *_machine);
void gmBindDebugWindowLibraryDocs(gmMachine *_m, gmBind2::TableConstructor &_tc);

#endif

#endif
