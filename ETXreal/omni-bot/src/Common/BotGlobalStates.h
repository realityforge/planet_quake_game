////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __BotGlobalStates_H__
#define __BotGlobalStates_H__

#include "StateMachine.h"
#include "ScriptManager.h"

class gmScriptGoal;

namespace AiState
{
	class RegionTriggers : public StateChild
	{
	public:

		RegionTriggers();
	private:
	};

	class GlobalRoot : public StateFirstAvailable
	{
	public:
		GlobalRoot();
	private:
	};
};

#endif
