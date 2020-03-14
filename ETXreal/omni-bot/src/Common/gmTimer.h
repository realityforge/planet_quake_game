////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __GM_TIMER_H__
#define __GM_TIMER_H__

#include "gmBind.h"

class gmTimer : public gmBind<Timer, gmTimer>
{
public:
	GMBIND_DECLARE_FUNCTIONS( );

	//////////////////////////////////////////////////////////////////////////
	// Functions
	static int gmfResetTimer(gmThread *a_thread);
	static int gmfGetElapsedTime(gmThread *a_thread);

};

#endif
