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
	*	
	*/
	BulletGhostObject(int worldID);

	/**
	*	@param[in]	shape	衝突形状
	*	@param[in]	worldID	固有のID
	*/
	BulletGhostObject(std::shared_ptr<BulletCollisionShape> collisionShape, int worldID);
	virtual ~BulletGhostObject();

	/**
	*	@brief	衝突形状の設定
	*	@param[in]	collisionShape	衝突形状
	*/
	void SetCollisionShape(std::shared_ptr<BulletCollisionShape> collisionShape);

	/**
	*	@brief	タグ1を設定する
	*	@param[in]	tag	設定するタグ1
	*/
	void SetTag1(int tag);

	/**
	*	@brief	タグ2を設定する
	*	@param[in]	tag	設定するタグ2
	*/
	void SetTag2(int tag);

	/**
	*	@brief	btCollisionObjectのポインタを取得する
	*	@return btCollisionObjectのポインタ
	*/
	std::shared_ptr<btCollisionObject> GetPtr() const;

	/**
	*	@brief	重複しているオブジェクトの数を取得する(AABB単位かグループ単位?)
	*	@return 重複オブジェクトの数
	*/
	int GetNumOvwelappingObjects();

	/**
	*	@brief	重複しているオブジェクトを取得する
	*	@param[in]	オブジェクトのインデックス
	*	@return		重複オブジェクト
	*/
	btCollisionObject* GetOverlappingObject(int i);

	/**
	*	@brief	コリジョンの状態を設定する
				BulletCollisionStateをもとに設定する
	*/
	void SetCollisionState(int flags);

	/**
	*	@brief	位置を指定の場所へ移動させる
	*	@param[in]	pos		移動させる場所
	*/
	void SetOrigin(float x, float y, float z);
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

