////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#include "PrecompET.h"
#include "ET_FilterClosest.h"

#include "ScriptManager.h"

ET_FilterClosest::ET_FilterClosest(Client *_client, AiState::SensoryMemory::Type _type) :
	FilterClosest				(_client, _type)
{
}

bool ET_FilterClosest::CheckEx(const MemoryRecord &_record)
{
	// Special consideration for some entity types.
	switch(_record.m_TargetInfo.m_EntityClass) 
	{
	case ET_CLASSEX_VEHICLE_HVY:
		{
			break;
		}
	case ET_CLASSEX_MG42MOUNT:
		{
			GameEntity mounted = InterfaceFuncs::GetMountedPlayerOnMG42(m_Client, _record.GetEntity());
			if(!mounted.IsValid() || m_Client->IsAllied(mounted))
				return false;
			break;
		}
	case ET_CLASSEX_BREAKABLE:
		{
			float fBreakableDist = static_cast<ET_Client*>(m_Client)->GetBreakableTargetDist();
			float fDistance = (m_Client->GetPosition() - _record.GetLastSensedPosition()).SquaredLength();
			if(fDistance > (fBreakableDist * fBreakableDist))
				return false;
			break;
		}
	}

	// TODO: Handle disguised covertops
	// cs: done in script for the moment
	//if (_record.m_TargetInfo.m_EntityFlags.CheckFlag(ET_ENT_FLAG_DISGUISED) && Mathf::UnitRandom() > 0.33f)
	//	return false;

	return true;
}

