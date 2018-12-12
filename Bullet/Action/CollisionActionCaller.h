#pragma once
#include <functional>

struct CalliedAction
{
	CalliedAction();
	CalliedAction(std::function<void(int,int)> onCollisionAction
		, std::function<void(int,int)> stayCollisionAction
		, std::function<void(int,int)> exitCollisionAction);
	std::function<void(int,int)> onAction;
	std::function<void(int,int)> stayAction;
	std::function<void(int,int)> exitAction;
};

class CollisionActionCaller
{
public:
	CollisionActionCaller(CalliedAction& actions, int tag1, int tag2);
	~CollisionActionCaller();

	void Collide();

	void Action();

	bool IsPreCollide() const;

	bool IsCollide() const;
private:
	void NonAction(int tag1, int tag2);

	void OnAction(int tag1, int tag2);

	void StayAction(int tag1, int tag2);

	void ExitAction(int tag1, int tag2);

	void NonStateCollide();

	void OnStateCollide();

	void StayStateCollide();

	void ExitStateCollide();

	void (CollisionActionCaller::*mActionFunc)(int tag1, int tag2);

	void (CollisionActionCaller::*mStateFunc)();

	CalliedAction mActions;

	bool mIsPreCollide;
	bool mIsCollide;

	const int mOtherColliderTag1;
	const int mOtherColliderTag2;
};

