////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ET_FILTERCLOSEST_H__
#define __ET_FILTERCLOSEST_H__

#include "FilterClosest.h"
#include "BotSensoryMemory.h"

// class: ET_FilterClosest
//		This filter finds the closest entity matching the 
//		requested type, category, and class. Also provides
//		additional functionality required in ET to take into
//		account whether the entity is disguised or feigned dead
class ET_FilterClosest : public FilterClosest
{
public:
	
	virtual bool CheckEx(const MemoryRecord &_record);

	ET_FilterClosest(Client *_client, AiState::SensoryMemory::Type _type);
	virtual ~ET_FilterClosest() {}
protected:
	float	m_BestDistance;

	ET_FilterClosest();
};

#endif
