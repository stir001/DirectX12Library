#include "stdafx.h"
#include "BulletlibLink.h"
#include "CollisionDetector.h"
#include "bullet/Shape/BulletCollisionShape.h"
#include "bullet/CollisionObject/BulletGhostObject.h"
#include "Bullet/System/PhysicsSystem.h"
#include <algorithm>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

CollisionDetector::CollisionDetector(std::shared_ptr<BulletCollisionShape> shape, int tag)
{
	mGhost = PhysicsSystem::Instance().CreateGhostObject(shape);
	mGhost->SetTag(tag);
	mGhost->SetCollisionShape(shape);
	auto trans = mGhost->GetGhostObject()->getWorldTransform();
	trans.setIdentity();
	trans.setOrigin(btVector3(0, 0, 0));
	mGhost->GetGhostObject()->setWorldTransform(trans);
	int num = mGhost->GetGhostObject()->getCollisionFlags();
	mGhost->GetGhostObject()->setCollisionFlags(num | 
		btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE | 
		btCollisionObject::CollisionFlags::CF_STATIC_OBJECT);
	PhysicsSystem::Instance().AddGhost(mGhost);
}


CollisionDetector::~CollisionDetector()
{
}

void CollisionDetector::updateAction(btCollisionWorld * collisionWorld, btScalar deltaTimeStep)
{
	int num = mGhost->GetGhostObject()->getNumOverlappingObjects();
	for (int i = 0; i < num; ++i)
	{
		auto pairCollision = mGhost->GetGhostObject()->getOverlappingObject(i);
		auto findPair = collisionWorld->getPairCache()->findPair(mGhost->GetGhostObject()->getBroadphaseHandle()
			, pairCollision->getBroadphaseHandle());
		if (!findPair) continue;
		int tag = pairCollision->getUserIndex();
		btManifoldArray manifoldArray;
		findPair->m_algorithm->getAllContactManifolds(manifoldArray);
		int arraySize = manifoldArray.size();
		for (int j = 0; j < arraySize; ++j)
		{
			int contactNum = manifoldArray[j]->getNumContacts();
			for (int point = 0; point < contactNum; ++point)
			{
				auto p = manifoldArray[j]->getContactPoint(point);
				if (p.getDistance() < 0)
				{
					mAction(tag);
					break;
				}
			}
		}
	}
}

void CollisionDetector::debugDraw(btIDebugDraw* debugDrawer)
{
}

void CollisionDetector::SetCollisionObject(std::shared_ptr<BulletGhostObject> shape)
{
	mGhost = shape;
}

void CollisionDetector::SetShape(std::shared_ptr<BulletCollisionShape> shape)
{
	mGhost->SetCollisionShape(shape);
}

void CollisionDetector::SetCollisionState(int state)
{
	mGhost->GetGhostObject()->setCollisionFlags(state);
}

void CollisionDetector::SetCollisionState(BulletCollisionState state)
{
	mGhost->GetGhostObject()->setCollisionFlags(static_cast<int>(state));
}

void CollisionDetector::Translate(float x, float y, float z)
{
	auto trans = mGhost->GetGhostObject()->getWorldTransform();
	trans.setOrigin({ x,y,z });
	mGhost->GetGhostObject()->setWorldTransform(trans);
}

std::shared_ptr<BulletGhostObject> CollisionDetector::GetGhostObject()
{
	return mGhost;
}

btGhostObject* CollisionDetector::GetbtGhostPtr()
{
	return mGhost->GetGhostObject().get();
}

void CollisionDetector::SetAction(std::function<void(int)> action)
{
	mAction = action;
}

int CollisionDetector::GetTag() const
{
	return mGhost->GetTag();
}

void CollisionDetector::SetTag(int tag)
{
	mGhost->SetTag(tag);
}

