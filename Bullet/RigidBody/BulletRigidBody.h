#pragma once
/**
*	@file BulletRigidBody.h
*	@brief btRigidBodyのラッパークラス定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/26
*/
#include <btBulletDynamicsCommon.h>
#include <memory>
#include <DirectXMath.h>

class BulletCollisionObject;
class PhysicsSystem;
class BulletCollisionShape;

enum class BulletCollisionState 
{
	STATIC = 1,			//!移動しない固定オブジェクト　物理計算なし　当たり判定あり
	KINEMATIC = 2,		//!任意移動可能オブジェクト　物理計算なし	当たり判定あり
	NON_CONTACT = 4,	//!任意移動不可オブジェクト	重力のみ?あり　当たり判定なし
	CHARACTER = 16,		//!任意移動不可オブジェクト	物理計算あり	当たり判定あり
};

int operator|(const BulletCollisionState lval, const BulletCollisionState rval);

/**
*	@class BulletRigidBody
*	@brief btRigidBodyのラッパークラス
*/
class BulletRigidBody : public std::enable_shared_from_this<BulletRigidBody>
{
public:
	friend PhysicsSystem;

	/**
	*	何もしないコンストラクタ
	*/
	BulletRigidBody();

	/**
	*	@param[in]	collisionShape	rigidBodyに設定する形状
	*	@param[in]	pos		初期位置
	*/
	BulletRigidBody(std::shared_ptr<BulletCollisionShape> collisionShape
		, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	virtual ~BulletRigidBody();

	/**
	*	btRigidBodyを取得する
	*/
	virtual std::shared_ptr<btRigidBody> GetRigidBody();

	/**
	*	@brief	質量を設定する
	*	@param[in]	mass	設定する質量
	*/
	virtual void SetMass(float mass);

	/**
	*	@brief 位置を設定する	
				瞬間移動なので当たり判定時に不具合が起きる可能性あり
	*	@param[in]	x	x座標
	*	@param[in]	y	y座標
	*	@param[in]	z	z座標
	*/
	virtual void Translate(float x, float y, float z);

	/**
	*	@brief 位置を設定する
				瞬間移動なので当たり判定時に不具合が起きる可能性あり
	*	@param[in]	pos	設定する座標
	*/
	virtual void Translate(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	ワールド変換行列を取得する
	*	@return	ワールド変換行列
	*/
	DirectX::XMFLOAT4X4 GetWorldTransform() const;

	/**
	*	@brief	ワールド変換行列を設定する
	*	@param[in]	matrix	ワールド変換行列
	*/
	void SetWorldTransform(const DirectX::XMFLOAT4X4& matrix);

	/**
	*	@brief	タグを取得する
	*	@return	設定されたタグ
	*/
	int GetTag() const;


	/**
	*	@brief	任意のタグを設定する
				コリジョンの判定時にオブジェクトを判別するためのタグ
	*	@param[in]	tag		設定するタグ
	*/
	void SetTag(int tag);

	/**
	*	コリジョンの状態を変更する
		enum BulletCollisionState	の値を基準にする
	*/
	void SetCollisionState(int state);

	/**
	*	コリジョンの状態を変更する
			enum BulletCollisionState	の値を基準にする
	*/
	void SetCollisionState(BulletCollisionState state);

	/**
	*	@brief	摩擦力を設定する
	*	@param[in]	friction	摩擦力
	*/
	void SetFriction(float friction);

	/**
	*	@brief	回転摩擦力を設定する
	*	@param[in]	friction	摩擦力
	*/
	void SetSpinFriction(float friction);

	/**
	*	@brief	反発係数を設定する
	*	@param[in]	restitution	反発係数
	*/
	void SetRestitution(float restitution);

	/**
	*	@brief	Yaw Pitch Rollを設定する
	*	@param[in]	ypr		(Pitch,Yaw,Roll)
	*/
	void SetYawPitchRoll(const DirectX::XMFLOAT3& ypr);

	/**
	*	@brief	Yaw Pitch Rollを設定する
	*	@param[in]	x	Pitch
	*	@param[in]	y	Yaw
	*	@param[in]	z	Roll
	*/
	void SetYawPitchRoll(float x, float y,float z);

	/**
	*	@brief	中心点を設定する
	*	@param[in]	pos		設定する座標
	*/
	void SetOrigin(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	中心点を設定する
	*	@param[in]	x	設定するx座標
	*	@param[in]	y	設定するy座標
	*	@param[in]	z	設定するz座標
	*/
	void SetOrigin(float x, float y, float z);
protected:
	/**
	*	btRigidBodyを作成する
	*/
	virtual void CreateRigidBody();

	/**
	*	rigidBody
	*/
	std::shared_ptr<btRigidBody> mRigidBody;

	/**
	*	collisionの形状
	*/
	std::shared_ptr<BulletCollisionShape> mCollisionShape;

	/**
	*	モーションステート	剛体との同期に必要
	*/
	std::shared_ptr<btMotionState> mMotionState;

	/**
	*	タグ
	*/
	int mTag;

	/**
	*	質量
	*/
	btScalar mMass;
};

