////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-09-20 08:26:27 +0200 (Mo, 20 Sep 2010) $
// $LastChangedRevision: 159 $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ET_CLIENT_H__
#define __ET_CLIENT_H__

#include "Client.h"
#include "ET_Config.h"

// class: ET_Client
//		Extended client class for Enemy-Territory.
class ET_Client : public Client
{
public:	
	friend void gmBindETBotLibrary(gmMachine *_machine);

	void Init(int _gameid);

	NavFlags GetTeamFlag();
	NavFlags GetTeamFlag(int _team);

	void SendVoiceMacro(int _macroId);

	void ProcessGotoNode(const Path &_path);

	float GetGameVar(GameVar _var) const;
	float GetAvoidRadius(int _class) const;

	bool DoesBotHaveFlag(MapGoalPtr _mapgoal);
	bool IsFlagGrabbable(MapGoalPtr _mapgoal);

	bool CanBotSnipe();
	bool GetSniperWeapon(int &nonscoped, int &scoped);

	bool GetSkills(gmMachine *machine, gmTableObject *tbl);

	float GetBreakableTargetDist() const { return m_BreakableTargetDistance; }

	float NavCallback(const NavFlags &_flag, Waypoint *from, Waypoint *to) ;

	void SetupBehaviorTree();

#ifdef ENABLE_REMOTE_DEBUGGING
	virtual void UpdateSyncImage( SyncImage & syncImage );
#endif

	ET_Client();
	virtual ~ET_Client();
protected:
	void ProcessEvent(const MessageHelper &_message, CallbackParameters &_cb);
	int HandleVoiceMacroEvent(const MessageHelper &_message);

	float		m_BreakableTargetDistance;
};

#endif
