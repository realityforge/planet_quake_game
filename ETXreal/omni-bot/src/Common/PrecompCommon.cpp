////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#include "PrecompCommon.h"

// Global variables.
IEngineInterface *g_EngineFuncs = 0;

#ifdef BOOST_NO_EXCEPTIONS
void throw_exception(std::exception const & e)
{
	OBASSERT(0, e.what());
}
#endif
