////////////////////////////////////////////////////////////////////////////////
// 
// $LastChangedBy: jswigart $
// $LastChangedDate: 2010-08-28 07:12:05 +0200 (Sa, 28 Aug 2010) $
// $LastChangedRevision: 32 $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __OBSTACLEMANAGER_H__
#define __OBSTACLEMANAGER_H__

class ObstacleManager
{
public:

	void ModifyForObstacles( Client * ai, Vector3f & nextPt );

	void AddObstacle( GameEntity entity );
	void RemoveObstacle( GameEntity entity );

	void Update();

	ObstacleManager();
	~ObstacleManager();
private:
	struct obstacle_t {
		GameEntity		entity;
		int				entClass;
		Box3f			bounds;
		int				obstacleGroup;

		void UpdateObstacle();

		obstacle_t();
	};

	struct obstacleGroup_t {
		int				groupNum;
		// todo: convex bounds

		obstacleGroup_t();
	};

	enum { MaxObstacles = 128 };
	obstacle_t			obstacles[MaxObstacles];
	obstacleGroup_t		obstacleGroups[MaxObstacles];
	int					numObstacles;
	int					numObstacleGroups;

	void RemoveObstacle( int index );
};

extern ObstacleManager obstacleManager;

#endif