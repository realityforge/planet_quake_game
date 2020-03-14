////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#include "PrecompCommon.h"
#include "Regulator.h"
#include "IGame.h"



bool Regulator::IsReady()
{
	int iCurrentTime = IGame::GetTime();
	if(iCurrentTime >= m_NextUpdateTime)
	{
		m_NextUpdateTime = iCurrentTime + m_UpdateInterval;
		return true;
	}
	return false;
}

