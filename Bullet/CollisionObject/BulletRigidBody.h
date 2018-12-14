#pragma once
/**
*	@file BulletRigidBody.h
*	@brief btRigidBodyのラッパークラス定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/12/14
*/
#include "Base/ICollisionObject.h"
#include <memory>
#include <DirectXMath.h>

class btRigidBody;
class btMotionState;
class BulletCollisionObject;
class PhysicsSystem;
class BulletCollisionShape;
class IActionDefiner;

/**
*	@class BulletRigidBody
*	@brief btRigidBodyのラッパークラス
*/
class BulletRigidBody : public ICollisionObject
{
public:
	/**
	*	@param[in]	collisionShape	rigidBodyに設定する形状
	*	@param[in]	pos		初期位置
	*/
	BulletRigidBody(std::shared_ptr<BulletCollisionShape> collisionShape, int worldID
		, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	virtual ~BulletRigidBody();

	/**
	*	@brief	質量を設定する
	*	@param[in]	mass	設定する質量
	*/
	virtual void SetMass(float mass);

	/**
	*	@brief 現在の位置から指定分だけ中心点を移動させる(addの計算)	
				瞬間移動なので当たり判定時に不具合が起きる可能性あり
	*	@param[in]	x	x移動成分
	*	@param[in]	y	y移動成分
	*	@param[in]	z	z移動成分
	*/
	virtual void Translate(float x, float y, float z);

	/**
	*	@brief 現在の位置から指定分だけ中心点を移動させる(addの計算)	
				瞬間移動なので当たり判定時に不具合が起きる可能性あり
	*	@param[in]	pos	移動成分情報
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
	*	@brief	任意のタグ1を設定する
	*	@param[in]	tag		設定するタグ1
	*/
	void SetTag1(int tag);

	/**
	*	@brief	任意のタグ2を設定する
	*	@param[in]	tag		設定するタグ2
	*/
	void SetTag2(int tag);

	/**
	*	@brief	コリジョンの状態を変更する
	*		enum BulletCollisionState	の値を基準にする
	*	@param[in]	state	設定するステート
	*/
	void SetCollisionState(int state);

	/**
	*	@brief	コリジョンの状態を変更する
	*		enum BulletCollisionState	の値を基準にする
	*	@param[in]	state	設定するステート
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

	/**
	*	@brief	btCollisionObjectのポインタを取得する
	*	@return btCollisionObjectのポインタ
	*/
	std::shared_ptr<btCollisionObject> GetPtr() const;

	/**
	*	@brief	btRigidBodyのポインタを取得する
	*	@return btRigidBodyのポインタ
	*/
	std::shared_ptr<btRigidBody> GetRigidPtr() const;

	/**
	*	@brief	当たり判定をとらないアクションを定義する
	*	@param[in]	ignoreAction	無視するアクション
	*/
	void SetIgnoreAction(std::shared_ptr<IActionDefiner> ignoreAction);

	/**
	*	@brief	物理演算時に回転する力の強さを定義する
	*	@param[in]	factor	回転する要素の大きさ(0で回転しない)
	*/
	void SetAngularFactor(float factor);

	/**
	*	@brief	現在の衝突形状の中心点座標を得る
	*	@return	現在の衝突形状の中心座標
	*/
	DirectX::XMFLOAT3 GetOrigin() const;

	/**
	*	@brief	剛体に力を加える
	*	@param[in]	x	x軸方向の力の成分
	*	@param[in]	y	y軸方向の力の成分
	*	@param[in]	z	z軸方向の力の成分
	*/
	void AddForce(float x, float y, float z);

	/**
	*	@broef	剛体に力を加える
	*	@param[in]	force	加える力の成分
	*/
	void AddForce(const DirectX::XMFLOAT3& force);

	/**
	*	@brief	剛体に瞬間的に力を加える
	*	@param[in]	x	x軸方向の力の成分
	*	@param[in]	y	y軸方向の力の成分
	*	@param[in]	z	z軸方向の力の成分
	*/
	void AddImpulse(float x, float y, float z);

	/**
	*	@brief	剛体に瞬間的に力を加える
	*	@param[in]	impulse	加える力の成分
	*/
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	/**
	*	@brief	速度を設定する
	*	@param[in]	x	x軸方向の速度成分
	*	@param[in]	y	y軸方向の速度成分
	*	@param[in]	z	z軸方向の速度成分
	*/
	void SetVelocity(float x, float y, float z);

	/**
	*	@brief	速度を設定する
	*	@param[in]	vel	設定する速度成分
	*/
	void SetVelocity(const DirectX::XMFLOAT3& vel);

	/**
	*	@brief	速度を取得する
	*	@return	現在の速度
	*/
	DirectX::XMFLOAT3 GetVelocity() const;

	/**
	*	@brief	重力を設定する
	*	@param[in]	gravity	設定する重力成分
	*/
	void SetGravity(const DirectX::XMFLOAT3& gravity);

	/**
	*	@brief	重力を設定する
	*	@param[in]	x	x軸方向の重力成分
	*	@param[in]	y	y軸方向の重力成分
	*	@param[in]	z	z軸方向の重力成分
	*/
	void SetGravity(float x, float y, float z);
private:
	/**
	*	世界から自分を削除する
	*/
	void RemoveWorld() override;

	/**
	*	btRigidBodyを作成する
	*/
	virtual void CreateRigidBody();

	/**
	*	物理演算を有効な状態にする
	*/
	void Activation();

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
	*	質量
	*/
	float mMass;
};

