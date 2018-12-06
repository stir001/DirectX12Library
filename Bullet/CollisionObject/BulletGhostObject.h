#pragma once
/**
*	@file BulletGhostObject.h
*	@brief 衝突検知用のオブジェクト
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/12/5
*/

#include "Base/ICollisionObject.h"
#include <memory>

class btGhostObject;
class BulletCollisionShape;
class PhysicsSystem;

/**
*	@class BulletGhostObject
*	@brief 衝突検知オブジェクト
*/
class BulletGhostObject :public ICollisionObject
{
public:
	/**
	*	@param[in]	worldID	固有のID
	*/
	BulletGhostObject(int worldID);

	/**
	*	@param[in]	shape	衝突形状
	*	@param[in]	worldID	固有のID
	*/
	BulletGhostObject(std::shared_ptr<BulletCollisionShape> collisionShape, int worldID);
	virtual ~BulletGhostObject();

	/**
	*	@brief	バレットのGhostObjectを取得する
	*	@return	btGhostObject
	*/
	std::shared_ptr<btGhostObject> GetGhostObject();

	/**
	*	@brief	衝突形状の設定
	*	@param[in]	collisionShape	衝突形状
	*/
	void SetCollisionShape(std::shared_ptr<BulletCollisionShape> collisionShape);

	/**
	*	@brief	タグを設定する
	*	@param[in]	tag	設定するタグ
	*/
	void SetTag(int tag);

	/**
	*	@brief	btCollisionObjectのポインタを取得する
	*	@return btCollisionObjectのポインタ
	*/
	btCollisionObject* GetPtr() const;

	void TestRemove();
private:
	/**
	*	世界から登録を外す
	*/
	void RemoveWorld() override;

	/**
	*	バレットのghost
	*/
	std::shared_ptr<btGhostObject> mGhost;

	/**
	*	衝突形状
	*/
	std::shared_ptr<BulletCollisionShape> mShape;
};

