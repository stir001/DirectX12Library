#pragma once
/**
*	@addtogroup Calculator
*	@file TransformMatrixCalculator.h
*	@brief 平行移動、拡大・縮小、回転の計算をするクラスの基底クラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/20
*/
#include <DirectXMath.h>

/**
*	@ingroup Calculator
*	@class TransformMatrixCalculator名
*	@brief 平行移動、拡大・縮小、回転の計算をする基底クラス
*/
class TransformMatrixCalculator
{
public:
	TransformMatrixCalculator();
	virtual ~TransformMatrixCalculator();

	/**
	*	@brief	任意の量平行移動させる
	*	@param[in]	offset	へいう移動成分
	*/
	virtual void AddPositon(const DirectX::XMFLOAT3& offset) = 0;

	/**
	*	@brief	任意の拡大率を追加する
	*	@param[in]	scale	追加拡大率
	*/
	virtual void AddScale(const DirectX::XMFLOAT3& scale) = 0;

	/**
	*	@brief	x軸中心で追加で回転させる
	*	@param[in]	rad		回転量(radian,弧度法)
	*/
	virtual void AddRotaX(float rad) = 0;

	/**
	*	@brief	y軸中心で追加で回転させる
	*	@param[in]	rad		回転量(radian,弧度法)
	*/
	virtual void AddRotaY(float rad) = 0;

	/**
	*	@brief	z軸中心で追加で回転させる
	*	@param[in]	rad		回転量(radian,弧度法)
	*/
	virtual void AddRotaZ(float rad) = 0;

	/**
	*	@brief	四元数で指定された量追加で回転させる
	*	@param[in]	quaternion	追加で回転させる回転を表している四元数
	*/
	virtual void AddRotaQuaternion(const DirectX::XMFLOAT4& quaternion) = 0;

	/**
	*	今まで計算した行列を得る
	*/
	virtual DirectX::XMFLOAT4X4 GetAMatrix() = 0;
};

