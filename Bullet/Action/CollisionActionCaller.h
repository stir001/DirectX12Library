#pragma once
#include <functional>

struct CalliedAction
{
	CalliedAction();
	CalliedAction(std::function<void(int)> onCollisionAction, std::function<void(int)> stayCollisionAction, std::function<void(int)> exitCollisionAction);
	std::function<void(int)> onAction;
	std::function<void(int)> stayAction;
	std::function<void(int)> exitAction;
};

class CollisionActionCaller
{
public:
	CollisionActionCaller(CalliedAction& actions, int tag);
	~CollisionActionCaller();

	void Collide();

	void Action();

	bool IsPreCollide() const;

	bool IsCollide() const;
private:
	void NonAction(int tag);

	void OnAction(int tag);

	void StayAction(int tag);

	void ExitAction(int tag);

	void NonStateCollide();

	void OnStateCollide();

	void StayStateCollide();

	void ExitStateCollide();

	void (CollisionActionCaller::*mActionFunc)(int tag);

	void (CollisionActionCaller::*mStateFunc)();

	CalliedAction mActions;

	bool mIsPreCollide;
	bool mIsCollide;

	const int mOtherColliderTag;
};

