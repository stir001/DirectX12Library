#include "stdafx.h"
#include "PositionSyncAction.h"
#include "Bullet/CollisionObject/BulletGhostObject.h"
#include "Bullet/CollisionObject/BulletRigidBody.h"

PositionSyncAction::PositionSyncAction()
{
}


PositionSyncAction::~PositionSyncAction()
{
}

void PositionSyncAction::updateAction(btCollisionWorld * collisionWorld, btScalar deltaTimeStep)
{
	auto rigidPos = mRigidBody->GetOrigin();
	for (auto& ghost : mSyncGhosts)
	{
		ghost->SetOrigin(rigidPos);
	}
}

void PositionSyncAction::debugDraw(btIDebugDraw * debugDrawer)
{
}

void PositionSyncAction::AddSyncGhosts(std::shared_ptr<BulletGhostObject> ghost)
{
	mSyncGhosts.push_back(ghost);
}

void PositionSyncAction::RemoveSyncGhost(unsigned int ghostIndex)
{
	if (mSyncGhosts.size() <= 0) return;
	mSyncGhosts.erase(mSyncGhosts.begin() + ghostIndex);
}
