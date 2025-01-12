////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////


#ifndef __ET_GAME_H__
#define __ET_GAME_H__

class Waypoint;
class gmMachine;
class gmTableObject;

#include "IGame.h"

// class: ET_Game
//		Game Type for Enemy-Territory.
class ET_Game : public IGame
{
public:
	bool Init();

	void RegisterNavigationFlags(PathPlannerBase *_planner);
	void RegisterPathCheck(PathPlannerWaypoint::pfbWpPathCheck &_pfnPathCheck);

	virtual Client *CreateGameClient();

	int GetVersionNum() const;
	const char *GetDLLName() const;
	const char *GetGameName() const;
	const char *GetModSubFolder() const;
	const char *GetNavSubfolder() const;
	const char *GetScriptSubfolder() const;
	const char *GetGameDatabaseAbbrev() const { return "et"; }
	eNavigatorID GetDefaultNavigator() const;
	bool ReadyForDebugWindow() const;

	GoalManager *GetGoalManager();

	void AddBot(Msg_Addbot &_addbot, bool _createnow = true);

	void ClientJoined(const Event_SystemClientConnected *_msg);

	const char *FindClassName(obint32 _classId);

	void GetTeamEnumeration(const IntEnum *&_ptr, int &num);
	void GetWeaponEnumeration(const IntEnum *&_ptr, int &num);

	virtual bool AddWeaponId(const char * weaponName, int weaponId);

	ET_Game() {};
	virtual ~ET_Game() {};
protected:

	void GetGameVars(GameVars &_gamevars);

	// Script support.
	void InitScriptBinds(gmMachine *_machine);
	void InitScriptCategories(gmMachine *_machine, gmTableObject *_table);
	void InitScriptClasses(gmMachine *_machine, gmTableObject *_table);
	void InitScriptSkills(gmMachine *_machine, gmTableObject *_table);
	void InitScriptEvents(gmMachine *_machine, gmTableObject *_table);
	void InitScriptEntityFlags(gmMachine *_machine, gmTableObject *_table);
	void InitScriptPowerups(gmMachine *_machine, gmTableObject *_table);
	void InitVoiceMacros(gmMachine *_machine, gmTableObject *_table);

	// Commands
	void InitCommands();

	static const float ET_GetEntityClassTraceOffset(const int _class, const BitFlag64 &_entflags);
	static const float ET_GetEntityClassAimOffset(const int _class, const BitFlag64 &_entflags);
	static const float ET_GetEntityVisDistance(const int _class, const BitFlag64 &_entflags, float _default);
	static const float ET_GetEntityClassAvoidRadius(const int _class);

	StringBuffer m_ExtraWeaponNames;
};

#endif
