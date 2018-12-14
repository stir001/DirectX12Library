#pragma once
/**
*	@file IActionDefiner.h
*	@brief 衝突時の動作を定義するための基底クラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/12/04
*/
#include <memory>
#include <DirectXMath.h>

class CollisionDetector;
class BulletCollisionShape;
class BulletGhostObject;

/**
*	@class IActionDefiner
*	@brief	衝突時のアクションを定義する基底するクラス
*/
class IActionDefiner
{
public:
	/**
	*	@param[in]	shape	衝突形状
	*	@param[in]	tag1	自身のタグ1(ユーザーが自由に定義)
	*/
	IActionDefiner(std::shared_ptr<BulletCollisionShape> shape, int tag1);
	~IActionDefiner();

	/**
	*	@brief	衝突した最初に呼び出される関数
	*	@brief	tag		衝突したコリジョンのタグ
	*/
	virtual void OnAction(int tag1, int tag2) = 0;

	/**
	*	@brief	衝突している間OnActionの後ずっと呼び出される関数
	*	@param[in]	tag		衝突したコリジョンのタグ
	*/
	virtual void StayAction(int tag1, int tag2) = 0;

	/**
	*	@brief	衝突しなくなった時に飛び出される関数
	*	@param[in]	tag		衝突したコリジョンのタグ
	*/
	virtual void ExitAction(int tag1, int tag2) = 0;

	/**
	*	@brief	タグ1を設定する
	*	@param[in]	tag	設定するタグ1
	*/
	void SetTag1(int tag);

	/**
	*	@brief	自身のタグ1を取得する
	*	@return	タグ1
	*/
	int GetTag1() const;

	/**
	*	@brief	タグ2を設定する
	*	@param[in]	tag	設定するタグ2
	*/
	void SetTag2(int tag);

	/**
	*	@brief	自身のタグ2を取得する
	*	@return	タグ2
	*/
	int GetTag2() const;

	/**
	*	@brief	当たり判定を瞬間移動させる
	*	@param[in]	x	移動させるx座標
	*	@param[in]	y	移動させるy座標
	*	@param[in]	z	移動させるz座標
	*/
	void Translate(float x, float y, float z);

	/**
	*	@brief	当たり判定を瞬間移動させる
	*	@param[in]	pos	移動させる座標
	*/
	void Translate(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	当たり判定用ghostを取得する
	*	@return	判定用ghost
	*/
	std::shared_ptr<BulletGhostObject> GetGhost() const;
private:
	/**
	*	衝突検知用オブジェクト
	*/
	std::shared_ptr<CollisionDetector> mDetector;
};

