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
	mBroadphase.reset( new btDbvtBroadphase());
	mGhostCallBack.reset( new btGhostPairCallback());
	mBroadphase->getOverlappingPairCache()->setInternalGhostPairCallback(mGhostCallBack.get());

	//狭域位相アルゴリズムの実装をインスタンス化
	mCollisionConfiguration.reset(new btDefaultCollisionConfiguration());
	mDispatcher.reset(new btCollisionDispatcher(mCollisionConfiguration.get()));

	//ソルバー
	mSolver.reset(new btSequentialImpulseConstraintSolver());

	//設定を適応した世界を作る
	mWorld.reset(new btDiscreteDynamicsWorld(mDispatcher.get(), mBroadphase.get(), mSolver.get(), mCollisionConfiguration.get()));

	//重力を設定する
	mWorld->setGravity(btVector3(0.f, -9.8f, 0.f));	

	mDebugDrawer.reset(new BulletDebugDrawDx(Dx12Ctrl::Instance().GetDev()));
	mDebugDrawer->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawWireframe);
	mWorld->setDebugDrawer(mDebugDrawer.get());

	mTime = clock();
}

void PhysicsSystem::Release()
{
	for (auto rigid : mRigidBodies)
	{
		RemoveRigidBody(rigid.second);
	}
	mRigidBodies.clear();
	for (auto ghost : mGhosts)
	{
		RemoveGhost(ghost.second->GetWorldID());
	}
	mGhosts.clear();
}


PhysicsSystem::~PhysicsSystem()
{
	mWorld.reset();
	mSolver.reset();
	mDispatcher.reset();
	mCollisionConfiguration.reset();
	mBroadphase.reset();
	mGhostCallBack.reset();
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
	if (rigid->GetWorldID() == -1) return;
	mRigidBodies[rigid->GetWorldID()] = rigid;
	mWorld->addRigidBody(rigid->GetRigidBody().get());
}

int PhysicsSystem::GetValidityWorldID()
{
	int worldID = 0;

	for (auto itr = mRigidBodies.begin(); itr != mRigidBodies.end(); ++itr)
	{
		if ((*itr).first != worldID)
		{
			auto fitr = std::find_if(itr, mRigidBodies.end(), [worldID](std::pair<const int, std::shared_ptr<BulletRigidBody>> data) { return data.first == worldID; });
			if (fitr == mRigidBodies.end())
			{
				break;
			}
		}
		++worldID;
	}
	return worldID;
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
	(*fitr).second->mWorldID = -1;
	mRigidBodies.erase(fitr);
}

void PhysicsSystem::RemoveRigidBody(int worldID)
{
	auto itr = mRigidBodies.find(worldID);
	if (itr == mRigidBodies.end())
	{
		return;
	}
	mWorld->removeRigidBody((*itr).second->GetRigidBody().get());
	(*itr).second->mWorldID = -1;
	mRigidBodies.erase(itr);
}

std::shared_ptr<BulletRigidBody> PhysicsSystem::CreateRigitBody(const BulletShapeType type, const DirectX::XMFLOAT3& data, const DirectX::XMFLOAT3& pos)
{	
	auto rigidBody = std::make_shared<BulletRigidBody>(CreateCollisionShape(type, data), GetValidityWorldID(), pos);

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

std::shared_ptr<BulletGhostObject> PhysicsSystem::CreateGhostObject(const BulletShapeType type, const DirectX::XMFLOAT3& data, const DirectX::XMFLOAT3& pos)
{
	auto ghost = std::make_shared<BulletGhostObject>(CreateCollisionShape(type, data), GetValidityWorldID());

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
	mGhosts[ghost->GetWorldID()] = ghost;
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
	(*fitr).second->mWorldID = -1;
	mGhosts.erase(fitr);
}
