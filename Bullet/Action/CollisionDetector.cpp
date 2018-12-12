#include "stdafx.h"
#include "BulletlibLink.h"
#include "CollisionDetector.h"
#include "bullet/Shape/BulletCollisionShape.h"
#include "bullet/CollisionObject/BulletGhostObject.h"
#include "Bullet/System/PhysicsSystem.h"
#include "Bullet/Action/CollisionActionCaller.h"

#include <algorithm>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

CollisionDetector::CollisionDetector(std::shared_ptr<BulletCollisionShape> shape
	, int tag, std::shared_ptr<CalliedAction> calliedAction)
	: mCalliedAction(calliedAction)
{
	mGhost = PhysicsSystem::Instance().CreateGhostObject(shape);
	mGhost->SetTag1(tag);
	mGhost->SetCollisionShape(shape);
	auto trans = mGhost->GetPtr()->getWorldTransform();
	trans.setIdentity();
	trans.setOrigin(btVector3(0, 0, 0));
	mGhost->GetPtr()->setWorldTransform(trans);
	int num = mGhost->GetPtr()->getCollisionFlags();
	mGhost->GetPtr()->setCollisionFlags(num |
		btCollisionObject::CollisionFlags::CF_NO_CONTACT_RESPONSE | 
		btCollisionObject::CollisionFlags::CF_STATIC_OBJECT);
	PhysicsSystem::Instance().AddGhost(mGhost);
}


CollisionDetector::~CollisionDetector()
{
}

void CollisionDetector::updateAction(btCollisionWorld * collisionWorld, btScalar deltaTimeStep)
{
	int num = mGhost->GetNumOvwelappingObjects();
	auto mineProxy = mGhost->GetPtr()->getBroadphaseHandle();
	for (int i = 0; i < num; ++i)
	{
		auto pairCollision = mGhost->GetOverlappingObject(i);
		auto findPair = collisionWorld->getPairCache()->findPair(mGhost->GetPtr()->getBroadphaseHandle()
			, pairCollision->getBroadphaseHandle());
		if (!findPair) continue;
		int otherid = GetOtherProxyID(findPair, mineProxy);
		auto caller = mCallers.find(otherid);
		if (caller == mCallers.end())
		{
			int tag1 = pairCollision->getUserIndex();
			int tag2 = pairCollision->getUserIndex2();
			mCallers[otherid] = std::make_shared<CollisionActionCaller>(*mCalliedAction, tag1, tag2);
			caller = mCallers.find(otherid);
		}
		if (IsCollide(findPair))
		{
			(*caller).second->Collide();
		}
	}

	const unsigned int maxLoop = static_cast<unsigned int>(mCallers.size());
	unsigned int roopCounter = 0;
	for (auto itr = mCallers.begin(); itr != mCallers.end(); ++itr)
	{
		(*itr).second->Action();
		roopCounter = 0;
		while (roopCounter < maxLoop)
		{
			++roopCounter;
			if (!(*itr).second->IsCollide() && !(*itr).second->IsPreCollide())
			{
				
				itr = mCallers.erase(itr);
				if (itr != mCallers.end())
				{
					continue;
				}

			}
			break;
		}
		if (itr == mCallers.end())
		{
			break;
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
	mGhost->GetPtr()->setCollisionFlags(state);
}

void CollisionDetector::SetCollisionState(BulletCollisionState state)
{
	mGhost->GetPtr()->setCollisionFlags(static_cast<int>(state));
}

void CollisionDetector::Translate(float x, float y, float z)
{
	auto trans = mGhost->GetPtr()->getWorldTransform();
	trans.setOrigin({ x,y,z });
	mGhost->GetPtr()->setWorldTransform(trans);
}

std::shared_ptr<BulletGhostObject> CollisionDetector::GetPtr()
{
	return mGhost;
}

int CollisionDetector::GetTag1() const
{
	return mGhost->GetTag1();
}

void CollisionDetector::SetTag1(int tag)
{
	mGhost->SetTag1(tag);
}

void CollisionDetector::SetTag2(int tag)
{
	mGhost->SetTag2(tag);
}

int CollisionDetector::GetTag2() const
{
	return mGhost->GetTag2();
}

bool CollisionDetector::IsCollide(btBroadphasePair* pair)
{
	bool rtn = false;
	btManifoldArray manifoldArray;
	pair->m_algorithm->getAllContactManifolds(manifoldArray);
	int arraySize = manifoldArray.size();
	for (int j = 0; j < arraySize; ++j)
	{
		int contactNum = manifoldArray[j]->getNumContacts();
		for (int point = 0; point < contactNum; ++point)
		{
			auto p = manifoldArray[j]->getContactPoint(point);
			if (p.getDistance() < 0)
			{
				rtn = true;
				break;
			}
		}
	}
	return rtn;
}

int CollisionDetector::GetOtherProxyID(btBroadphasePair * pair, btBroadphaseProxy* mine)
{
	int id = -1;
	if (pair->m_pProxy0 != mine)
	{
		id = pair->m_pProxy0->m_uniqueId;
	}
	else if(pair->m_pProxy1 != mine)
	{
		id = pair->m_pProxy1->m_uniqueId;
	}
	return id;
}



