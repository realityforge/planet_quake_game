////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TRACKABLE_H__
#define __TRACKABLE_H__

#include "Omni-Bot_Events.h"

// class: Trackable
class Trackable
{
public:

	void AddReference(obuint32 _type);
	void DelReference(obuint32 _type);

	obuint32 GetRefCount(obuint32 _type);

	bool IsReferenced();

	Trackable();
	virtual ~Trackable();
protected:
	void _CheckIndex(obuint32 _type);
	
	typedef std::vector<obint32> TrackList;
	TrackList	m_TrackList;
};

#endif
