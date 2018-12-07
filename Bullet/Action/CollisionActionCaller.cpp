#include "stdafx.h"
#include "CollisionActionCaller.h"


CalliedAction::CalliedAction() : onAction([](int) {}), stayAction([](int) {}), exitAction([](int) {})
{
}

CalliedAction::CalliedAction(std::function<void(int)> onCollisionAction, std::function<void(int)> stayCollisionAction, std::function<void(int)> exitCollisionAction)
	: onAction(onCollisionAction), stayAction(stayCollisionAction), exitAction(exitCollisionAction)
{
}


CollisionActionCaller::CollisionActionCaller(CalliedAction& actions, int tag)
	: mActions(actions), mActionFunc(&CollisionActionCaller::NonAction)
	, mStateFunc(&CollisionActionCaller::NonStateCollide)
	, mIsPreCollide(false), mOtherColliderTag(tag)
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
	(this->*mActionFunc)(mOtherColliderTag);
}

bool CollisionActionCaller::IsPreCollide() const
{
	return mIsPreCollide;
}

bool CollisionActionCaller::IsCollide() const
{
	return mIsCollide;
}

void CollisionActionCaller::NonAction(int tag)
{
}

void CollisionActionCaller::OnAction(int tag)
{
	mActions.onAction(tag);
	mStateFunc = &CollisionActionCaller::StayStateCollide;
	mActionFunc = &CollisionActionCaller::ExitAction;
	mIsPreCollide = false;
	mIsCollide = true;
}

void CollisionActionCaller::StayAction(int tag)
{
	mActions.stayAction(tag);
	mActionFunc = &CollisionActionCaller::ExitAction;
	mIsPreCollide = true;
	mIsCollide = true;
}

void CollisionActionCaller::ExitAction(int tag)
{
	mActions.exitAction(tag);
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
