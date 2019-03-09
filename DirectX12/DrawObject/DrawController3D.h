#pragma once
/**
*	@file DrawController3D.h
*	@brief 3Dとして操作するためのコントローラーの定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/15
*/
#include "DrawObjectController.h"
#include <DirectXMath.h>

class ConstantBufferObject;
class IndexBufferObject;
class VertexBufferObject;
class Dx12DescriptorHeapObject;

/**
*	@ingroup DrawObjectController
*	@class DrawController3D
*	@brief 3Dとして操作するための関数を定義
*/
class DrawController3D :
	public DrawObjectController
{
public:
	/**
	*	@param[in]	modelName	操作するものの名前
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	cmdList	描画のコマンドを積むコマンドリスト
	*/
	DrawController3D(const std::string& modelName, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		std::shared_ptr<Dx12CommandList>& cmdList);
	virtual ~DrawController3D();

	/**
	*	DescriptorHeapを再構築する
	*/
	virtual void UpdateDescriptorHeap() = 0;

	/**
	*	@brief カメラバッファを設定する
	*	@param cameraBuffer	カメラ情報保持用バッファ
	*/
	virtual void SetCameraBuffer(std::shared_ptr<ConstantBufferObject> cameraBuffer);

	/**
	*	@brief	基準点を指定の場所に平行移動させる
	*	@param[in]	pos		平行移動させたい場所
	*/
	virtual void SetPosition(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	x,y,z等倍で拡大率を設定する
	*	@param[in]	設定したい拡大率
	*/
	virtual void SetScale(float scale);

	/**
	*	@brief x,y,z各成分で拡大率を設定する
	*	@param[in]	scaleX	xの拡大率
	*	@param[in]	scaleY	yの拡大率
	*	@param[in]	scaleZ	zの拡大率
	*/
	virtual void SetScale(float scaleX, float scaleY, float scaleZ);

	/**
	*	@brief	x軸中心で追加で回転させる
	*	@param[in]	rad		回転量(radian,弧度法)
	*/
	virtual void AddRotaX(float rad);

	/**
	*	@brief	y軸中心で追加で回転させる
	*	@param[in]	rad		回転量(radian,弧度法)
	*/
	virtual void AddRotaY(float rad);

	/**
	*	@brief	z軸中心で追加で回転させる
	*	@param[in]	rad		回転量(radian,弧度法)
	*/
	virtual void AddRotaZ(float rad);

	/**
	*	@broef	回転を設定する(ラジアン単位)
	*	@param[in]	x	x軸の回転量
	*	@param[in]	y	y軸の回転量
	*	@param[in]	z	z軸の回転量
	*/
	virtual void SetRota(float x, float y, float z);

	/**
	*	@broef	回転を設定する(ラジアン単位)
	*	@param[in]	rota	各軸の回転量
	*/
	virtual void SetRota(const DirectX::XMFLOAT3& rota);

	/**
	*	@brief	任意の軸で回転する四元数を設定する
	*	@param[in]	quaternion	軸と回転量を表す四元数
	*/
	virtual void SetRotaQuaternion(const DirectX::XMFLOAT4& quaternion);

	/**
	*	@brief	ボーンの色を設定する
	*	@param[in]	color	設定する色
	*/
	virtual void SetSkeletonColor(DirectX::XMFLOAT4& color);

	/**
	*	@brief	ボーンの色を設定する
	*	@param[in]	color	設定する色
	*/
	virtual void SetSkeletonColor(float r, float g, float b, float a = 1.0f);

protected:
	/**
	*	mModelMatrix(アフィン行列)を更新する
	*/
	void UpdateMatrix();

	/**
	*/
	void UpdateSkeltonColor();

	/**
	*	アフィン行列を書き込むバッファ
	*/
	std::shared_ptr<ConstantBufferObject> mModelMatrixBuffer;

	/**
	*	アフィン行列を保持
	*/
	DirectX::XMFLOAT4X4 mModelMatrix;

	/**
	*	拡大率
	*/
	DirectX::XMFLOAT3 mScale;

	/**
	*	基準点の座標
	*/
	DirectX::XMFLOAT3 mPos;

	/**
	*	回転行列
	*/
	DirectX::XMFLOAT4X4 mRotationMatrix;

	/**
	*	回転を表す四元数
	*/
	DirectX::XMFLOAT4 mQuaternion;

	/**
	*	ボーンの色情報
	*/
	DirectX::XMFLOAT4 mSkeletonColor;

	/**
	*	骨を表示するときに使用するrootsignature
	*/
	std::shared_ptr<RootSignatureObject> mSkeletonRootsignature;

	/**
	*	骨を表示するときに使用するpipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mSkeletonPipelineState;

	/**
	*	骨用のインデックスバッファ
	*/
	std::shared_ptr<IndexBufferObject> mSkeletonIndexBuffer;

	/**
	*	骨用の頂点バッファ
	*/
	std::shared_ptr<VertexBufferObject> mSkeletonVertexBuffer;

	/**
	*	骨用の色情報バッファ
	*/
	std::shared_ptr<ConstantBufferObject> mSkeletonColorConstantBuffer;

	/**
	*	骨用のDescriptorHeap
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> mSkeletonHeap;

	/**
	*	カメラ用コンスタントバッファ
	*/
	std::shared_ptr<ConstantBufferObject> mCameraBuffer;
};

