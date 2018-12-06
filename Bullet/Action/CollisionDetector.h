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

enum class BulletShapeType;
enum class BulletCollisionState;
class BulletGhostObject;
class BulletCollisionShape;
class btGhostObject;
class IActionDefiner;

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
	CollisionDetector(std::shared_ptr<BulletCollisionShape> shape, int tag);
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
	void Translate(float x, float y, float z);

	/**
	*	@brief	ghostを取得する
	*	@return	保持しているghost
	*/
	std::shared_ptr<BulletGhostObject> GetGhostObject();

	/**
	*	@brief	bulletのghostObject(btGhostObject)のポインタを取得する
	*	@return	btGhostObjectのポインタ
	*/
	btGhostObject* GetbtGhostPtr();

	/**
	*	@brief	衝突検知している間ずっと呼ばれる関数を設定する
	*	@param[in]	action	衝突検知時に実行されるアクション
	*/
	void SetAction(std::function<void(int)> action);

	/**
	*	@brief タグを取得する
	*	@return 保持しているタグ
	*/
	int GetTag() const;

	/**
	*	@brief	タグを設定する
	*	@param[in]	設定するタグ
	*/
	void SetTag(int tag);

	void TestRemove();
protected:
	/**
	*	衝突検知用のオブジェクト
	*/
	std::shared_ptr<BulletGhostObject> mGhost;

	/**
	*	衝突時に起こすアクションの関数
	*/
	std::function<void(int)> mAction;
};

