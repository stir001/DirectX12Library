#include "stdafx.h"
#include "BulletlibLink.h"
#include "BulletGhostObject.h"
#include "bullet/Shape/BulletCollisionShape.h"
#include "bullet/System/PhysicsSystem.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


BulletGhostObject::BulletGhostObject(int tag)
	: mTag(tag), mGhost(std::make_shared<btGhostObject>())
{
	mGhost->setUserIndex(tag);
}

BulletGhostObject::~BulletGhostObject()
{
	PhysicsSystem::Instance().RemoveGhost(mTag);
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

int BulletGhostObject::GetTag() const
{
	return mTag;
}
