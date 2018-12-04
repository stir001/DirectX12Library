#include "stdafx.h"
#include "IActionDefiner.h"
#include "CollisionDetector.h"

IActionDefiner::IActionDefiner(std::shared_ptr<BulletCollisionShape> shape, int tag)
{
	mDetector = std::make_shared<CollisionDetector>(shape, tag);
	mDetector->SetAction([&](int tag)->void { Action(tag); }
	);
}


IActionDefiner::~IActionDefiner()
{
}

int IActionDefiner::GetTag() const
{
	return mDetector->GetTag();
}
