#pragma once
#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <memory>
#include <vector>

class BulletRigidBody;
class BulletGhostObject;

class PositionSyncAction : public btActionInterface
{
public:
	PositionSyncAction();
	~PositionSyncAction();

	/**
	*	@brief	bullet側から呼ばれる関数　この中で衝突を検知する
	*	@param[in]	collidionWorld	剛体やghostなどを保持している世界
	*	@param[in]	deltaTime		ステップ時間
	*/
	void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);

	/**
	*	@brief	bullet側から呼ばれる関数 必要ないので未実装
	*	@param[in]	debugDrawer		デバッグ描画用のクラス
	*/
	void debugDraw(btIDebugDraw* debugDrawer);

	/**
	*	@brief	座標を同期するghostを追加する
	*	@param[in]	ghost	同期するghost
	*/
	void AddSyncGhosts(std::shared_ptr<BulletGhostObject> ghost);

	/**
	*	@brief	同期するghostを削除する
	*	@param[in]	ghostIndex	削除するghostのインデックス
	*/
	void RemoveSyncGhost(unsigned int ghostIndex);

private:
	std::shared_ptr<BulletRigidBody> mRigidBody;
	std::vector<std::shared_ptr<BulletGhostObject>> mSyncGhosts;
};

