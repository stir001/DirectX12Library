#pragma once
/**
*	@file ICollisionObject.h
*	@brief 衝突オブジェクトの定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/12/5
*/
#include <memory>

class PhysicsSystem;
class btCollisionObject;

/**
*	@class ICollisionObject
*	@brief 衝突オブジェクトの定義
*/
class ICollisionObject
{
	friend PhysicsSystem;
public:

	ICollisionObject(int worldID);
	virtual ~ICollisionObject();

	/**
	*	@brief	btCollisionObjectのポインタを取得する
	*	@return btCollisionObjectのポインタ
	*/
	virtual std::shared_ptr<btCollisionObject> GetPtr() const = 0;

	/**
	*	@brief	タグを設定する
	*	@param[in]	tag		ユーザー定義のタグ
	*/
	virtual void SetTag1(int tag) = 0;

	/**
	*	@brief	ユーザー定義のタグ1を取得する
	*	@return	タグ1
	*/
	int GetTag1() const;

	virtual void SetTag2(int tag) = 0;

	/**
	*	@brief	ユーザー定義のタグ2を取得す
	*	@return	タグ2
	*/
	int GetTag2() const;

	/**
	*	@brief	ワールド個別識別子を取得する
	*	@return	識別子
	*/
	int GetWorldID() const;

protected:
	/**
	*	自身を世界から削除する
	*/
	virtual void RemoveWorld() = 0;

	/**
	*	ユーザー定義のタグ1 デフォルトは-1
	*/
	int mTag1;

	/**
	*	ユーザー定義のタグ2	デフォルトは-1
	*/
	int mTag2;
private:
	/**
	*	個別識別子
	*/
	int mWorldID;
};

