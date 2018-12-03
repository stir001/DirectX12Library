#include "stdafx.h"
#include "BulletlibLink.h"
#include "BulletGhostObject.h"
#include "bullet/Shape/BulletCollisionShape.h"
#include "bullet/System/PhysicsSystem.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


BulletGhostObject::BulletGhostObject()
	: mWorldID(-1)
{
	mGhost = std::make_shared<btGhostObject>();
	mGhost->setUserIndex(3);
}

BulletGhostObject::~BulletGhostObject()
{
	PhysicsSystem::Instance().RemoveGhost(mWorldID);
}

std::shared_ptr<btGhostObject> BulletGhostObject::GetGhostObject()
{
	return mGhost;
}

void BulletGhostObject::SetCollisionShape(std::shared_ptr<BulletCollisionShape> collisionShape)
{
	mShape = collisionShape;
	mGhost->setCollisionShape(mShape->GetShape().get());
}

int BulletGhostObject::GetWorldID()
{
	return mWorldID;
}
