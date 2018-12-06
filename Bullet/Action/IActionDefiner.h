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

/**
*	@class IActionDefiner
*	@brief	衝突時のアクションを定義する規定するクラス
*/
class IActionDefiner
{
public:
	/**
	*	@param[in]	shape	衝突形状
	*	@param[in]	tag		自身のタグ(ユーザーが自由に定義)
	*/
	IActionDefiner(std::shared_ptr<BulletCollisionShape> shape, int tag);
	~IActionDefiner();

	/**
	*	@brief	衝突している間ずっと呼び出される関数
	*	@param[in]	tag		各コリジョンに設定するタグ
	*/
	virtual void Action(int tag) = 0;

	/**
	*	@brief	自身のタグを取得する
	*	@return	自身のタグ
	*/
	int GetTag() const;

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
private:
	/**
	*	衝突検知用オブジェクト
	*/
	std::shared_ptr<CollisionDetector> mDetector;
};

