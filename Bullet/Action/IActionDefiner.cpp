#include "stdafx.h"
#include "IActionDefiner.h"
#include "CollisionDetector.h"
#include "Bullet/System/PhysicsSystem.h"

IActionDefiner::IActionDefiner(std::shared_ptr<BulletCollisionShape> shape, int tag)
{
	mDetector = std::make_shared<CollisionDetector>(shape, tag);
	mDetector->SetAction([&](int tag)->void { Action(tag); }
	);
	PhysicsSystem::Instance().AddAction(mDetector);
}


IActionDefiner::~IActionDefiner()
{
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
