#include "stdafx.h"
#include "CollisionActionCaller.h"


CalliedAction::CalliedAction() : onAction([](int,int) {}), stayAction([](int, int) {}), exitAction([](int, int) {})
{
}

CalliedAction::CalliedAction(std::function<void(int, int)> onCollisionAction
	, std::function<void(int, int)> stayCollisionAction
	, std::function<void(int, int)> exitCollisionAction)
	: onAction(onCollisionAction), stayAction(stayCollisionAction), exitAction(exitCollisionAction)
{
}


CollisionActionCaller::CollisionActionCaller(CalliedAction& actions, int tag, int tag2)
	: mActions(actions), mActionFunc(&CollisionActionCaller::NonAction)
	, mStateFunc(&CollisionActionCaller::NonStateCollide)
	, mIsPreCollide(false), mOtherColliderTag1(tag), mOtherColliderTag2(tag2)
{
}

CollisionActionCaller::~CollisionActionCaller()
{
}

void CollisionActionCaller::Collide()
{
	mIsCollide = true;
	(this->*mStateFunc)();
}

void CollisionActionCaller::Action()
{
	(this->*mActionFunc)(mOtherColliderTag1, mOtherColliderTag2);
}

bool CollisionActionCaller::IsPreCollide() const
{
	return mIsPreCollide;
}

bool CollisionActionCaller::IsCollide() const
{
	return mIsCollide;
}

void CollisionActionCaller::NonAction(int tag1, int tag2)
{
}

void CollisionActionCaller::OnAction(int tag1, int tag2)
{
	mActions.onAction(tag1, tag2);
	mStateFunc = &CollisionActionCaller::StayStateCollide;
	mActionFunc = &CollisionActionCaller::ExitAction;
	mIsPreCollide = false;
	mIsCollide = true;
}

void CollisionActionCaller::StayAction(int tag1, int tag2)
{
	mActions.stayAction(tag1, tag2);
	mActionFunc = &CollisionActionCaller::ExitAction;
	mIsPreCollide = true;
	mIsCollide = true;
}

void CollisionActionCaller::ExitAction(int tag1, int tag2)
{
	mActions.exitAction(tag1, tag2);
	mActionFunc = &CollisionActionCaller::NonAction;
	mStateFunc = &CollisionActionCaller::ExitStateCollide;
	mIsPreCollide = false;
	mIsCollide = false;
}

void CollisionActionCaller::NonStateCollide()
{
	mStateFunc = &CollisionActionCaller::OnStateCollide;
	mActionFunc = &CollisionActionCaller::OnAction;
	mIsPreCollide = false;
}

void CollisionActionCaller::OnStateCollide()
{
	mStateFunc = &CollisionActionCaller::StayStateCollide;
	mActionFunc = &CollisionActionCaller::StayAction;
	mIsPreCollide = true;
}

void CollisionActionCaller::StayStateCollide()
{
	mActionFunc = &CollisionActionCaller::StayAction;
	mIsPreCollide = true;
}

void CollisionActionCaller::ExitStateCollide()
{
	mStateFunc = &CollisionActionCaller::OnStateCollide;
	mActionFunc = &CollisionActionCaller::OnAction;
	mIsPreCollide = false;
}
