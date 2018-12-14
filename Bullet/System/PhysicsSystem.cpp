#include "stdafx.h"
#include "BulletlibLink.h"
#include "PhysicsSystem.h"
#include "BulletDebugDrawDx.h"
#include "Master/Dx12Ctrl.h"
#include "bullet/CollisionObject/BulletRigidBody.h"
#include "bullet/Shape/BulletCollisionShape.h"
#include "bullet/Shape/SphereCollisionShape.h"
#include "bullet/Shape/BoxCollisionShape.h"
#include "bullet/Shape/StaticPlaneShape.h"
#include "bullet/Shape/CapsuleCollisionShape.h"
#include "bullet/Shape/ConeCollisionShape.h"
#include "bullet/Shape/CylinderCollisionShape.h"
#include "bullet/Action/CollisionDetector.h"
#include "bullet/CollisionObject/BulletGhostObject.h"

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

void PhysicsSystem::Release()
{
	for (auto collision : mCollisions)
	{
		auto rigid = btRigidBody::upcast(collision.second.get());
		if (rigid)
		{
			mWorld->removeRigidBody(rigid);
			continue;
		}

		auto ghost = btGhostObject::upcast(collision.second.get());
		if (ghost)
		{
			mWorld->removeCollisionObject(ghost);
		}
	}

	mCollisions.clear();
}


PhysicsSystem::~PhysicsSystem()
{
	mWorld.reset();
	mSolver.reset();
	mDispatcher.reset();
	mCollisionConfiguration.reset();
	mBroadphase.reset();
	mGhostCallBack.reset();
	mDebugDrawer.reset();
}

void PhysicsSystem::DebugDraw()
{
	mWorld->debugDrawWorld();
	mDebugDrawer->DebugDraw();
}

void PhysicsSystem::AddRigidBody(std::shared_ptr<BulletRigidBody> rigid)
{
	if (rigid->GetWorldID() == -1) return;
	mWorld->addRigidBody(rigid->GetRigidPtr().get());
	mCollisions[rigid->GetWorldID()] = rigid->GetPtr();
}

void PhysicsSystem::Simulation()
{
	long currentTime = clock();
	mWorld->stepSimulation(static_cast<float>(currentTime - mTime));
	mTime = currentTime;
}

void PhysicsSystem::Simulation(float deltaTime)
{
	mWorld->stepSimulation(deltaTime);
}

void PhysicsSystem::RemoveRigidBody(std::shared_ptr<BulletRigidBody> rigid)
{
	if (rigid->GetWorldID() == -1)
	{
		return;
	}
	auto fitr = mCollisions.find(rigid->GetWorldID());
	if (fitr == mCollisions.end())
	{
		return;
	}
	mWorld->removeRigidBody(rigid->GetRigidPtr().get());
	mCollisions.erase(fitr);
	rigid->mWorldID = -1;
}

void PhysicsSystem::RemoveRigidBody(int worldID)
{
	auto itr = mCollisions.find(worldID);
	if (itr == mCollisions.end())
	{
		return;
	}
	auto rigid = btRigidBody::upcast((*itr).second.get());
	if (rigid)
	{
		mWorld->removeRigidBody(rigid);
	}

	
	mCollisions.erase(itr);
}

std::shared_ptr<BulletRigidBody> PhysicsSystem::CreateRigidBody(const BulletShapeType type, const DirectX::XMFLOAT3& data, const DirectX::XMFLOAT3& pos)
{	
	auto rigidBody = std::make_shared<BulletRigidBody>(CreateCollisionShape(type, data), GetAvailableMinID(), pos);

	AddRigidBody(rigidBody);

	return rigidBody;
}

std::shared_ptr<BulletRigidBody> PhysicsSystem::CreateRigidBody(const std::shared_ptr<BulletCollisionShape> shape, const DirectX::XMFLOAT3& pos)
{
	auto rigid = std::make_shared<BulletRigidBody>(shape, GetAvailableMinID(), pos);
	return rigid;
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
		shape = std::make_shared<StaticPlaneShape>(0.0f, data);
		break;
	case BulletShapeType::CONE:
		shape = std::make_shared<ConeCollisionShape>(data.x, data.y);
		break;
	default:
		break;
	}

	return shape;
}

std::shared_ptr<BulletGhostObject> PhysicsSystem::CreateGhostObject(const BulletShapeType type, const DirectX::XMFLOAT3& data, const DirectX::XMFLOAT3& pos)
{
	auto ghost = std::make_shared<BulletGhostObject>(CreateCollisionShape(type, data), GetAvailableMinID());
	return ghost;
}

std::shared_ptr<BulletGhostObject> PhysicsSystem::CreateGhostObject(std::shared_ptr<BulletCollisionShape> shape
	, const DirectX::XMFLOAT3& pos)
{
	auto ghost = std::make_shared<BulletGhostObject>(shape, GetAvailableMinID());
	return ghost;
}

void PhysicsSystem::AddAction(std::shared_ptr<CollisionDetector> action)
{
	mWorld->addAction(action.get());
}

void PhysicsSystem::RemoveAction(std::shared_ptr<CollisionDetector> action)
{
	mWorld->removeAction(action.get());
}

void PhysicsSystem::AddGhost(std::shared_ptr<BulletGhostObject> ghost)
{
	if (ghost->GetWorldID() == -1) return;
	auto col = ghost->GetPtr();
	mWorld->addCollisionObject(col.get()
		, btBroadphaseProxy::CollisionFilterGroups::SensorTrigger
		, btBroadphaseProxy::CollisionFilterGroups::DefaultFilter | btBroadphaseProxy::CollisionFilterGroups::StaticFilter);
	mCollisions[ghost->GetWorldID()] = ghost->GetPtr();
}

void PhysicsSystem::RemoveGhost(int worldID)
{
	if (worldID == -1)return;
	auto fitr = mCollisions.find(worldID);
	if (fitr == mCollisions.end()) return;
	mWorld->removeCollisionObject((*fitr).second.get());
	mCollisions.erase(fitr);
}

void PhysicsSystem::RemoveGhost(std::shared_ptr<BulletGhostObject> ghost)
{
	if (ghost->GetWorldID() == -1) return;
	auto fitr = mCollisions.find(ghost->GetWorldID());
	if (fitr == mCollisions.end()) return;
	mWorld->removeCollisionObject(ghost->GetPtr().get());
	mCollisions.erase(fitr);
	ghost->mWorldID = -1;
}

int PhysicsSystem::GetAvailableMinID()
{
	int id = 0;
	auto endItr = mCollisions.end();
	for (auto itr = mCollisions.begin(); itr != endItr; ++itr, ++id)
	{
		if (itr->first == id)
		{
			continue;
		}
		auto fitr = std::find_if(itr, endItr, [id](std::pair<int, std::shared_ptr<btCollisionObject>> col) {return col.first == id; });
		if (fitr == endItr)
		{
			break;
		}
	}

	return id;
}
