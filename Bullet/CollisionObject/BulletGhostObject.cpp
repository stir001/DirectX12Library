#include "stdafx.h"
#include "BulletlibLink.h"
#include "BulletGhostObject.h"
#include "bullet/Shape/BulletCollisionShape.h"
#include "bullet/System/PhysicsSystem.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


BulletGhostObject::BulletGhostObject(int worldID)
	: ICollisionObject(worldID), mGhost(std::make_shared<btGhostObject>())
{
}

BulletGhostObject::BulletGhostObject(std::shared_ptr<BulletCollisionShape> collisionShape, int worldID)
	: ICollisionObject(worldID), mGhost(std::make_shared<btGhostObject>())
{
	SetCollisionShape(collisionShape);
}

BulletGhostObject::~BulletGhostObject()
{
	RemoveWorld();
}

void BulletGhostObject::SetCollisionShape(std::shared_ptr<BulletCollisionShape> collisionShape)
{
	mShape = collisionShape;
	mGhost->setCollisionShape(mShape->GetShape().get());
}

void BulletGhostObject::SetTag(int tag)
{
	mTag = tag;
	mGhost->setUserIndex(tag);
}

std::shared_ptr<btCollisionObject> BulletGhostObject::GetPtr() const
{
	return mGhost;
}

int BulletGhostObject::GetNumOvwelappingObjects()
{
	return mGhost->getNumOverlappingObjects();
}

btCollisionObject* BulletGhostObject::GetOverlappingObject(int i)
{
	return mGhost->getOverlappingObject(i);
}

void BulletGhostObject::RemoveWorld()
{
	PhysicsSystem::Instance().RemoveGhost(GetWorldID());
	auto pairs = mGhost->getOverlappingPairs();
}

