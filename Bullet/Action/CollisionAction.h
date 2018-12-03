#pragma once
#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <btBulletDynamicsCommon.h>
#include <memory>
#include <list>

enum class BulletShapeType;
class BulletGhostObject;
class BulletCollisionShape;
class btGhostObject;

class CollisionAction : public btActionInterface
{
public:
	CollisionAction();
	virtual ~CollisionAction();

	virtual void Action(int tag) = 0;

	void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);

	virtual void debugDraw(btIDebugDraw* debugDrawer);

	void SetCollisionObject(std::shared_ptr<BulletGhostObject> ghost);

	void SetShape(std::shared_ptr<BulletCollisionShape> shape);

	void SetCollisionState(int state);

	void Translate(float x, float y, float z);

	std::shared_ptr<BulletGhostObject> GetGhostObject();

	btGhostObject* GetbtGhostPtr();
protected:
	std::shared_ptr<BulletGhostObject> mGhost;

	std::list<int> mTargetTags;
};

