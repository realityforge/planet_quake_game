////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __NAVIGATIONMANAGER_H__
#define __NAVIGATIONMANAGER_H__

// class: Navigation Manager
//		This class is responsible for creating and initializing the desired 
//		path planner for the bot to use. Singleton for easy access.
class NavigationManager
{
public:

	// function: CreatePathPlanner
	//		Creates a path planner from an enumerated type
	bool CreatePathPlanner(NavigatorID _navid);

	// function: DeletePathPlanner
	//		Deletes the current path planner
	void DeletePathPlanner();

	// function: GetCurrentPathPlanner
	//		Accessor for the current path planner
	PathPlannerBase *GetCurrentPathPlanner();

	static NavigationManager *GetInstance();
	static void DeleteInstance();	
protected:

	PathPlannerBase		*m_PathPlanner;

	static NavigationManager	*m_Instance;
	NavigationManager();
	~NavigationManager();
	NavigationManager &operator=(const NavigationManager&);

};

#endif
