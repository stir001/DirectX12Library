#pragma once
#include <memory>

class btGhostObject;
class BulletCollisionShape;
class PhysicsSystem;

class BulletGhostObject
{
	friend PhysicsSystem;
public:
	BulletGhostObject();
	virtual ~BulletGhostObject();

	std::shared_ptr<btGhostObject> GetGhostObject();
	void SetCollisionShape(std::shared_ptr<BulletCollisionShape> collisionShape);
	int GetWorldID();
private:
	std::shared_ptr<btGhostObject> mGhost;
	std::shared_ptr<BulletCollisionShape> mShape;

	int mWorldID;
};

