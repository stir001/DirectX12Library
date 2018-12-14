#include "stdafx.h"
#include "IActionDefiner.h"
#include "CollisionDetector.h"
#include "Bullet/System/PhysicsSystem.h"
#include "Bullet/Action/CollisionActionCaller.h"

IActionDefiner::IActionDefiner(std::shared_ptr<BulletCollisionShape> shape, int tag)
{
	std::shared_ptr<CalliedAction> action = std::make_shared<CalliedAction>();
	action->stayAction = [&](int tag1, int tag2) { StayAction(tag1, tag2); };
	action->onAction = [&](int tag1, int tag2) { OnAction(tag1, tag2); };
	action->exitAction = [&](int tag1, int tag2) { ExitAction(tag, tag2); };
	mDetector = std::make_shared<CollisionDetector>(shape, tag, action);
	PhysicsSystem::Instance().AddAction(mDetector);
}


IActionDefiner::~IActionDefiner()
{
	PhysicsSystem::Instance().RemoveAction(mDetector);
	PhysicsSystem::Instance().RemoveGhost(mDetector->GetPtr());
}

void IActionDefiner::SetTag1(int tag)
{
	mDetector->SetTag1(tag);
}

int IActionDefiner::GetTag1() const
{
	return mDetector->GetTag1();
}

void IActionDefiner::SetTag2(int tag)
{
	mDetector->SetTag2(tag);
}

int IActionDefiner::GetTag2() const
{
	return mDetector->GetTag2();
}

void IActionDefiner::Translate(float x, float y, float z)
{
	mDetector->Translate(x, y, z);
}

void IActionDefiner::Translate(const DirectX::XMFLOAT3 & pos)
{
	mDetector->Translate(pos.x, pos.y, pos.z);
}

std::shared_ptr<BulletGhostObject> IActionDefiner::GetGhost() const
{
	return mDetector->GetPtr();
}
