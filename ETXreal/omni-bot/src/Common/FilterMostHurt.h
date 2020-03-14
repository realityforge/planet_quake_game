////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __FILTERMOSTHURT_H__
#define __FILTERMOSTHURT_H__

#include "FilterSensory.h"

// class: FilterMostHurt
//		This filter finds the entity with the least health matching the 
//		requested type, category, and class
class FilterMostHurt : public FilterSensory
{
public:
	
	virtual void Check(int _index, const MemoryRecord &_record);

	virtual void Reset();

	inline float GetMostHurtHealth() { return m_MostHurtHealthPc; }

	FilterMostHurt(Client *_client, AiState::SensoryMemory::Type _type);
	virtual ~FilterMostHurt();
protected:
	
	float		m_MostHurtHealthPc;

	FilterMostHurt();
};

#endif
