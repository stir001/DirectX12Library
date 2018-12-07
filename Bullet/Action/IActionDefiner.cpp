#include "stdafx.h"
#include "IActionDefiner.h"
#include "CollisionDetector.h"
#include "Bullet/System/PhysicsSystem.h"
#include "Bullet/Action/CollisionActionCaller.h"

IActionDefiner::IActionDefiner(std::shared_ptr<BulletCollisionShape> shape, int tag)
{
	std::shared_ptr<CalliedAction> action = std::make_shared<CalliedAction>();
	action->stayAction = [&](int tag) { StayAction(tag); };
	action->onAction = [&](int tag) { OnAction(tag); };
	action->exitAction = [&](int tag) { ExitAction(tag); };
	mDetector = std::make_shared<CollisionDetector>(shape, tag, action);
	PhysicsSystem::Instance().AddAction(mDetector);
}


IActionDefiner::~IActionDefiner()
{
	PhysicsSystem::Instance().RemoveAction(mDetector);
	PhysicsSystem::Instance().RemoveGhost(mDetector->GetPtr());
}

int IActionDefiner::GetTag() const
{
	return mDetector->GetTag();
}

void IActionDefiner::Translate(float x, float y, float z)
{
	mDetector->Translate(x, y, z);
}

void IActionDefiner::Translate(const DirectX::XMFLOAT3 & pos)
{
	mDetector->Translate(pos.x, pos.y, pos.z);
}
