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

void BulletGhostObject::SetTag1(int tag)
{
	mTag1 = tag;
	mGhost->setUserIndex(tag);
}

void BulletGhostObject::SetTag2(int tag)
{
	mTag2 = tag;
	mGhost->setUserIndex2(tag);
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

void BulletGhostObject::SetCollisionState(int flags)
{
	mGhost->setCollisionFlags(flags);
}

void BulletGhostObject::SetOrigin(float x, float y, float z)
{
	auto trans = mGhost->getWorldTransform();
	trans.setOrigin(btVector3(x, y, z));
	mGhost->setWorldTransform(trans);
}

void BulletGhostObject::SetVelocity(float x, float y, float z)
{
	mGhost->setInterpolationLinearVelocity(btVector3(x, y, z));
}

void BulletGhostObject::SetVelocity(const DirectX::XMFLOAT3 & vel)
{
	SetVelocity(vel.x, vel.y, vel.z);
}

void BulletGhostObject::RemoveWorld()
{
	PhysicsSystem::Instance().RemoveGhost(GetWorldID());
	auto pairs = mGhost->getOverlappingPairs();
}

