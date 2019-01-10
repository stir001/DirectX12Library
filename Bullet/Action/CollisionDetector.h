#pragma once
/**
*	@file CollisionDetector.h
*	@brief 衝突を検知するためのクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/12/04
*/
#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <btBulletDynamicsCommon.h>
#include <memory>
#include <functional>
#include <map>
#include <DirectXMath.h>

enum class BulletShapeType;
enum class BulletCollisionState;
class BulletGhostObject;
class BulletCollisionShape;
class btGhostObject;
class IActionDefiner;
class CollisionActionCaller;
struct CalliedAction;

/**
*	@class CollisionDetector
*	@brief	特定のghostとの衝突を検知する
*/
class CollisionDetector : public btActionInterface
{
public:
	/**
	*	@param[in]	shape	衝突検知するghostの型
	*	@param[in]	tag		ghostに設定するユーザー定義のタグ
	*/
	CollisionDetector(std::shared_ptr<BulletCollisionShape> shape
		, int tag, std::shared_ptr<CalliedAction> calliedAction);
	virtual ~CollisionDetector();

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
	*	@brief	衝突するghostを設定する
	*	@param[in]	ghost	衝突検知に使うghost(世界への登録はほかでやる必要あり)
	*/
	void SetCollisionObject(std::shared_ptr<BulletGhostObject> ghost);

	/**
	*	@brief	衝突形状を設定する
	*	@param[in]	衝突する形状
	*/
	void SetShape(std::shared_ptr<BulletCollisionShape> shape);

	/**
	*	@brief	状態を設定する
	*	@param[in]	state	設定する状態(bit単位でのフラグで管理)
	*/
	void SetCollisionState(int state);

	/**
	*	@brief	状態を設定する
	*	@param[in]	state	設定する状態(bit単位でのフラグで管理)
	*/
	void SetCollisionState(BulletCollisionState state);

	/**
	*	@brief	中心座標を任意の点に設定する
	*	@param[in]　x	設定するx座標
	*	@param[in]	y	設定するy座標
	*	@param[in]	z	設定するz座標
	*/
	void SetOrigin(float x, float y, float z);

	/**
	*	@brief	ghostを取得する
	*	@return	保持しているghost
	*/
	std::shared_ptr<BulletGhostObject> GetPtr();

	/**
	*	@brief	タグ1を設定する
	*	@param[in]	設定するタグ1
	*/
	void SetTag1(int tag);

	/**
	*	@brief タグ1を取得する
	*	@return 保持しているタグ1
	*/
	int GetTag1() const;

	/**
	*	@brief	タグ2を設定する
	*	@param[in]	設定するタグ2
	*/
	void SetTag2(int tag);

	/**
	*	@brief	タグ2を取得する
	*	@return	保持しているタグ2
	*/
	int GetTag2() const;

	/**
	*	@brief	速度を設定する
	*	@param[in]	vel	設定する速度
	*/
	void SetVelocity(const DirectX::XMFLOAT3& vel);
private:
	/**
	*	@brief	ペアが当たっているかどうかを判定する
	*	@param[in]	pair	ペア
	*/
	bool IsCollide(btBroadphasePair* pair);

	int GetOtherProxyID(btBroadphasePair* pair, btBroadphaseProxy* mine);

	/**
	*	衝突検知用のオブジェクト
	*/
	std::shared_ptr<BulletGhostObject> mGhost;

	std::map<int, std::shared_ptr<CollisionActionCaller>> mCallers;

	/**
	*	アクションを呼ぶクラスの設定保存用
	*/
	std::shared_ptr<CalliedAction> mCalliedAction;
};

