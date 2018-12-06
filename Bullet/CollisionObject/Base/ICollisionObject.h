#pragma once
/**
*	@file ICollisionObject.h
*	@brief 衝突オブジェクトの定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/12/5
*/

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
	/**
	*	@param[in] worldID	個別のID
	*/
	ICollisionObject(int worldID);
	virtual ~ICollisionObject();

	/**
	*	@brief	タグを設定する
	*	@param[in]	tag		ユーザー定義のタグ
	*/
	virtual void SetTag(int tag) = 0;


	/**
	*	@brief	ユーザー定義のタグを取得する
	*	@return	タグ
	*/
	int GetTag() const;

	/**
	*	@brief	ワールド個別識別子を取得する
	*	@return	識別子
	*/
	int GetWorldID() const;

	/**
	*	@brief	btCollisionObjectのポインタを取得する
	*	@return btCollisionObjectのポインタ
	*/
	virtual btCollisionObject* GetPtr() const = 0;
protected:
	/**
	*	自身を世界から削除する
	*/
	virtual void RemoveWorld() = 0;

	/**
	*	ユーザー定義のタグ デフォルトは-1
	*/
	int mTag;
private:

	/**
	*	ワールドに存在するオブジェクトを決めるための個別ID
	*/
	int mWorldID;
};

