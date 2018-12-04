#include "stdafx.h"
#include "BulletlibLink.h"
#include "PhysicsSystem.h"
#include "BulletDebugDrawDx.h"
#include "Master/Dx12Ctrl.h"
#include "bullet/RigidBody/BulletRigidBody.h"
#include "bullet/Shape/BulletCollisionShape.h"
#include "bullet/Shape/SphereCollisionShape.h"
#include "bullet/Shape/BoxCollisionShape.h"
#include "bullet/Shape/StaticPlaneShape.h"
#include "bullet/Shape/CapsuleCollisionShape.h"
#include "bullet/Shape/ConeCollisionShape.h"
#include "bullet/Shape/CylinderCollisionShape.h"
#include "bullet/Action/CollisionDetector.h"
#include "bullet/Ghost/BulletGhostObject.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <btBulletDynamicsCommon.h>
#include <ctime>
#include <algorithm>

PhysicsSystem* PhysicsSystem::mInstance = nullptr;

PhysicsSystem::PhysicsSystem()
{
	//広域位相アルゴリズムの実装をインスタンス化
	mBroadphase = std::make_shared<btDbvtBroadphase>();
	mGhostCallBack = std::make_shared<btGhostPairCallback>();
	mBroadphase->getOverlappingPairCache()->setInternalGhostPairCallback(mGhostCallBack.get());

	//狭域位相アルゴリズムの実装をインスタンス化
	mCollisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
	mDispatcher = std::make_shared<btCollisionDispatcher>(mCollisionConfiguration.get());

	//ソルバー
	mSolver = std::make_shared<btSequentialImpulseConstraintSolver>();

	//設定を適応した世界を作る
	mWorld = std::make_shared<btDiscreteDynamicsWorld>(mDispatcher.get(), mBroadphase.get(), mSolver.get(), mCollisionConfiguration.get());

	//重力を設定する
	mWorld->setGravity(btVector3(0.f, -9.8f, 0.f));	

	mDebugDrawer = std::make_shared<BulletDebugDrawDx>(Dx12Ctrl::Instance().GetDev());
	mDebugDrawer->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawWireframe);
	mWorld->setDebugDrawer(mDebugDrawer.get());

	mTime = clock();
}


PhysicsSystem::~PhysicsSystem()
{
	for (auto rigid : mRigidBodies)
	{
		mWorld->removeRigidBody(rigid.second->GetRigidBody().get());
	}
	for (auto ghost : mGhosts)
	{
		mWorld->removeCollisionObject(ghost.second->GetGhostObject().get());
	}
	mRigidBodies.clear();
}

void PhysicsSystem::DebugDraw()
{
	mWorld->debugDrawWorld();
	mDebugDrawer->DebugDraw();
}

void PhysicsSystem::ClearDebugDraw()
{
	mDebugDrawer->ClearLines();
}

void PhysicsSystem::AddRigidBody(std::shared_ptr<BulletRigidBody> rigid)
{
	if (rigid->GetTag() != -1) return;
	int key = 0;

	for (auto itr = mRigidBodies.begin(); itr != mRigidBodies.end(); ++itr)
	{
		if ((*itr).first != key)
		{
			auto fitr = std::find_if(itr, mRigidBodies.end(), [key](std::pair<const int, std::shared_ptr<BulletRigidBody>> data) { return data.first == key; });
			if (fitr == mRigidBodies.end())
			{
				break;
			}
		}
		++key;
	}

	mRigidBodies[key] = rigid;
	rigid->mTag = key;
	mWorld->addRigidBody(rigid->GetRigidBody().get());
}

void PhysicsSystem::Simulation()
{
	long currentTime = clock();
	mWorld->stepSimulation(currentTime - mTime);
	mTime = currentTime;
}

void PhysicsSystem::Simulation(float deltaTime)
{
	mWorld->stepSimulation(deltaTime);
}

void PhysicsSystem::RemoveRigidBody(std::shared_ptr<BulletRigidBody> rigid)
{
	if (rigid->GetTag() == -1)
	{
		return;
	}
	auto fitr = mRigidBodies.find(rigid->GetTag());
	if (fitr == mRigidBodies.end())
	{
		return;
	}
	mWorld->removeRigidBody(rigid->GetRigidBody().get());
	mRigidBodies.erase(fitr);
}

void PhysicsSystem::RemoveRigidBody(int tag)
{
	auto itr = mRigidBodies.find(tag);
	if (itr == mRigidBodies.end())
	{
		return;
	}
	mWorld->removeRigidBody((*itr).second->GetRigidBody().get());
	mRigidBodies.erase(itr);
}

std::shared_ptr<BulletRigidBody> PhysicsSystem::CreateRigitBody(const BulletShapeType type, const DirectX::XMFLOAT3& data, const DirectX::XMFLOAT3& pos)
{
	std::shared_ptr<BulletCollisionShape> shape;
	
	auto rigidBody = std::make_shared<BulletRigidBody>(CreateCollisionShape(type, data), pos);

	AddRigidBody(rigidBody);

	return rigidBody;
}

std::shared_ptr<BulletCollisionShape> PhysicsSystem::CreateCollisionShape(const BulletShapeType type, const DirectX::XMFLOAT3& data)
{
	std::shared_ptr<BulletCollisionShape> shape;
	switch (type)
	{
	case BulletShapeType::BOX:
		shape = std::make_shared<BoxCollisionShape>(data);
		break;
	case BulletShapeType::SPHERE:
		shape = std::make_shared<SphereCollisionShape>(data.x);
		break;
	case BulletShapeType::CYLINDER:
		shape = std::make_shared<CylinderCollisionShape>(data.x, data.y);
		break;
	case BulletShapeType::CAPSULE:
		shape = std::make_shared<CapsuleCollisionShape>(data.x, data.y);
		break;
	case BulletShapeType::PLANE:
		shape = std::make_shared<StaticPlaneShape>(0, data);
		break;
	case BulletShapeType::CONE:
		shape = std::make_shared<ConeCollisionShape>(data.x, data.y);
		break;
	default:
		break;
	}

	return shape;
}

void PhysicsSystem::AddAction(std::shared_ptr<CollisionDetector> action)
{
	mWorld->addAction(action.get());
	AddGhost(action->GetGhostObject());
	//mWorld->addCollisionObject(action->GetGhostObject()->GetGhostObject().get());
}

void PhysicsSystem::AddGhost(std::shared_ptr<BulletGhostObject> ghost)
{
	if (ghost->GetTag() != -1) return;
	int index = 0;
	for (auto gID = mGhosts.begin(); gID != mGhosts.end(); ++gID)
	{
		if ((*gID).first < index)
		{
			if(std::find_if(gID, mGhosts.end(), [index](std::pair<int, std::shared_ptr<BulletGhostObject>> value) 
			{
				return value.first == index;
			})
			 == mGhosts.end())
			break;
		}
		index = (*gID).first;
		++index;
	}
	mGhosts[index] = ghost;
	ghost->mTag = index;
	auto col = ghost->GetGhostObject();
	mWorld->addCollisionObject(col.get());
}

void PhysicsSystem::RemoveGhost(int index)
{
	if (index == -1)return;
	auto fitr = std::find_if(mGhosts.begin(), mGhosts.end(), [index](std::pair<int, std::shared_ptr<BulletGhostObject>> value)
	{
		return index == value.first;
	});
	if (fitr == mGhosts.end()) return;
	mWorld->removeCollisionObject((*fitr).second->GetGhostObject().get());
	mGhosts.erase(fitr);
}
