#pragma once
/**
*	@file Transform3DCalculator.h
*	@brief 3D描画オブジェクトを操作するときに使う計算をするクラスの定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/21
*/
#include "TransformMatrixCalculator.h"

/**
*	@ingroup Calculator
*	@class Transform3DCalculator
*	@brief 3D描画オブジェクトを操作するときに使う計算をする
*/
class Transform3DCalculator : public TransformMatrixCalculator
{
public:
	Transform3DCalculator();
	~Transform3DCalculator();

	/**
	*	@brief	平行移動行列を計算し現在の行列にかける
	*	@param[in]	pos		平行移動成分
	*/
	void AddPositon(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	拡大縮小行列を計算し現在の行列にかける
	*	@param[in]	scale	拡大率
	*/
	void AddScale(const DirectX::XMFLOAT3& scale);

	/**
	*	@brief	x軸中心の回転行列を計算し現在の行列にかける
	*	@param[in]	deg		回転量(degree,度数法)
	*/
	void AddRotaX(float deg);

	/**
	*	@brief	y軸中心の回転行列を計算し現在の行列にかける
	*	@param[in]	deg		回転量(degree,度数法)
	*/
	void AddRotaY(float deg);

	/**
	*	@brief	z軸中心の回転行列を計算し現在の行列にかける
	*	@param[in]	deg		回転量(degree,度数法)
	*/
	void AddRotaZ(float deg);

	/**
	*	@brief	現在の行列に回転を表す四元数から作られる行列を計算し現在の行列にかける
	*	@param[in]	quaternion	回転を表す四元数
	*/
	void AddRotaQuaternion(const DirectX::XMFLOAT4& quaternion);

	/**
	*	@brief	現在の行列を取得する
	*	@return	現在の行列
	*/
	DirectX::XMFLOAT4X4 GetAMatrix();

	/**
	*	現在の行列を単位行列にする
	*/
	void Init();
private:
	/**
	*	@brief	XMFLOAT4X4にXMMATRIXを右からかける
	*	@param[in]	matF44	乗算するXMFLOAT4X4
	*	@param[in]	matXM	乗算するXMMATRIX
	*	@return	計算結果(XMFLOAT4X4)
	*/
	DirectX::XMFLOAT4X4 Multiplication(DirectX::XMFLOAT4X4& matF44,const DirectX::XMMATRIX& matXM);

	/**
	*	計算しているアフィン変換行列
	*/
	DirectX::XMFLOAT4X4 mAMatrix;
};

