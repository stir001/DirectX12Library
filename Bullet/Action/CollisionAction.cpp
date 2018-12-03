#include "stdafx.h"
#include "BulletlibLink.h"
#include "CollisionAction.h"
#include "bullet/Shape/BulletCollisionShape.h"
#include "bullet/Ghost/BulletGhostObject.h"
#include <algorithm>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

CollisionAction::CollisionAction()
	: mGhost(std::make_shared<BulletGhostObject>())
{
	auto trans = mGhost->GetGhostObject()->getWorldTransform();
	trans.setIdentity();
	trans.setOrigin(btVector3(0, 0, 0));
	mGhost->GetGhostObject()->setWorldTransform(trans);
	int num = mGhost->GetGhostObject()->getCollisionFlags();
	//mCollision->GetGhostObject()->setActivationState(DISABLE_DEACTIVATION);
	mGhost->GetGhostObject()->setCollisionFlags(num | 
		btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE | 
		btCollisionObject::CollisionFlags::CF_STATIC_OBJECT);
}


CollisionAction::~CollisionAction()
{
}

void CollisionAction::updateAction(btCollisionWorld * collisionWorld, btScalar deltaTimeStep)
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
		for (int i = 0; i < arraySize; ++i)
		{
			int contactNum = manifoldArray[i]->getNumContacts();
			for (int point = 0; point < contactNum; ++point)
			{
				auto p = manifoldArray[i]->getContactPoint(point);
				if (p.getDistance() < 0)
				{
					Action(tag);
					break;
				}
			}
		}
	}
}

void CollisionAction::debugDraw(btIDebugDraw* debugDrawer)
{
}

void CollisionAction::SetCollisionObject(std::shared_ptr<BulletGhostObject> shape)
{
	mGhost = shape;
}

void CollisionAction::SetShape(std::shared_ptr<BulletCollisionShape> shape)
{
	mGhost->SetCollisionShape(shape);
}

void CollisionAction::SetCollisionState(int state)
{
	mGhost->GetGhostObject()->setCollisionFlags(state);
}

void CollisionAction::Translate(float x, float y, float z)
{
	auto trans = mGhost->GetGhostObject()->getWorldTransform();
	trans.setOrigin({ x,y,z });
	mGhost->GetGhostObject()->setWorldTransform(trans);
}

std::shared_ptr<BulletGhostObject> CollisionAction::GetGhostObject()
{
	return mGhost;
}

btGhostObject* CollisionAction::GetbtGhostPtr()
{
	return mGhost->GetGhostObject().get();
}
