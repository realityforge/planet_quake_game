////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __MAPGOALDATABASE_H__
#define __MAPGOALDATABASE_H__

typedef std::map<obuint32, MapGoalPtr> MapGoalMap;

// class: MapGoalDatabase
class MapGoalDatabase
{
public:

	void RegisterMapGoal(const String &_type, const MapGoalPtr &_wpn);
	gmGCRoot<gmUserObject> CreateMapGoalType(const String &_typename);

	String GetWeaponName(int _weaponId);

	MapGoalPtr GetNewMapGoal(const String &_type);

	void LoadMapGoalDefinitions(bool _clearall);
	void Unload();

	MapGoalDatabase();
	~MapGoalDatabase();
private:
	MapGoalMap	m_MapGoalMap;
};

extern MapGoalDatabase g_MapGoalDatabase;

#endif
