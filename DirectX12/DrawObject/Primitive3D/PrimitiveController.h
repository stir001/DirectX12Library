#pragma once
/**
*	@addtogroup primitive
*	@file	PrimitiveController.h
*	@brief	primitiveクラスで定義した頂点を描画するクラス定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/27
*/
#include "DirectX12/DrawObject/DrawController3D.h"
#include <vector>

class PrimitiveObject;
class VertexBufferObject;
class ConstantBufferObject;
class Dx12DescriptorHeapObject;
class IndexBufferObject;
class LightObject;
class Dx12Camera;
class TextureObject;
class Dx12BufferObject;
class Dx12CommandList;
struct PrimitiveVertex;

/**
*	@ingroup primitive
*	@class PrimitiveController
*	@brief primitiveクラスで定義した頂点を描画するクラス
*/
class PrimitiveController :
	public DrawController3D
{
public:
	/**
	*	@param[in]	primitive	頂点を作成したプリミティブクラス
	*	@param[in]	dev			使用するID3D12Device
	*	@param[in]	cmdList		使用するコマンドリスト
	*/
	PrimitiveController(std::shared_ptr<PrimitiveObject> primitive
		, Microsoft::WRL::ComPtr<ID3D12Device>& dev
		, std::shared_ptr<Dx12CommandList>& cmdList);

	PrimitiveController(std::vector<PrimitiveVertex>& vertices, std::vector<unsigned int> indices
		, Microsoft::WRL::ComPtr<ID3D12Device>& dev
		, std::shared_ptr<Dx12CommandList>& cmdList);

	virtual ~PrimitiveController();

	/**
	*	@brief	テクスチャを設定する
	*	@param[in]	texture		設定するテクスチャ
	*/
	void SetTexture(std::shared_ptr<TextureObject>& texture);

	/**
	*	@brief	ライトを設定する
	*	@param[in]	light	設定するライト
	*/
	void SetLightBuffer(std::shared_ptr<LightObject>& light);

	/**
	*	@brief	インスタンシングでオブジェクトを描画する
	*	@param[in]	instancePositions	各オブジェクトの位置
	*/
	void Instancing(std::vector<DirectX::XMFLOAT3>& instancePositions);

	/**
	*	@brief	インスタンシング描画時に使うアフィン行列を設定する
	*	@param[in]	matrix	アフィン行列
	*	@param[in]	startIndex	行列を適応するオブジェクトの始めの番号
	*	@param[in]	endIndex	行列を適応するオブジェクトの最後の番号
	*/
	void SetInstancingMatrix(std::vector<DirectX::XMFLOAT4X4>& matrix, unsigned int startIndex, unsigned int endIndex);

	/**
	*	@brief	アフィン行列を取得する
	*	@return		アフィン行列
	*/
	DirectX::XMFLOAT4X4 GetMatrix() const;

	/**
	*	描画する
	*/
	void Draw();

	/**
	*	@brief	座標を設定する
	*	@param[in]	pos		設定する座標
	*/
	void SetPosition(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	拡大率を等倍で設定する
	*	@param[in]	scale	拡大率
	*/
	void SetScale(float scale);

	/**
	*	@brief	拡大率を各種設定する
	*	@param[in]	scaleX	xの拡大率
	*	@param[in]	scaleY	yの拡大率
	*	@param[in]	scaleZ	zの拡大率
	*/
	void SetScale(float scaleX, float scaleY, float scaleZ);

	/**
	*	@brief	x軸中心で追加で回転させる
	*	@param[in]	rad		回転量(radian,弧度法)
	*/
	void AddRotaX(float rad);

	/**
	*	@brief	y軸中心で追加で回転させる
	*	@param[in]	rad		回転量(radian,弧度法)
	*/
	void AddRotaY(float rad);

	/**
	*	@brief	z軸中心で追加で回転させる
	*	@param[in]	rad		回転量(radian,弧度法)
	*/
	void AddRotaZ(float rad);

	/**
	*	@brief	任意の軸で回転する四元数を設定する
	*	@param[in]	quaternion	軸と回転量を表す四元数
	*/
	void SetRotaQuaternion(const DirectX::XMFLOAT4& quaternion);

	/**
	*	DescriptorHeapを更新する
	*	基本的には呼ばなくていい
	*/
	void UpdateDescriptorHeap();

	/**
	*	@brief プリミティブの色を設定する
	*	@param color 0~1の範囲のrbgの色情報
	*/
	void SetColor(const DirectX::XMFLOAT4& color, int index = 0);

	void SetShadowmapRootSignature(std::shared_ptr<RootSignatureObject> rootsignature);

	void SetShadowmapPipelineState(std::shared_ptr<PipelineStateObject> pipelinestate);

	void SetShadowRenderRootSignature(std::shared_ptr<RootSignatureObject> rootsignature);

	void SetShadowRenderPipelineState(std::shared_ptr<PipelineStateObject> pipelinestate);

	void SetShadowmapTexture(std::shared_ptr<Dx12BufferObject> shadowmapTex);

	void CreateShadowmapDescHeap();

	void DrawShadowmap();

	void DrawShadow();

	void SetShadowmapCommandList(std::shared_ptr<Dx12CommandList>& cmdList);

	void SetGraphicsRootSignature(const std::shared_ptr<RootSignatureObject>& rootSignature);

	void SetPipelineState(const std::shared_ptr<PipelineStateObject>& pipelineState);

protected:
	void UpdateInstanceVertexBuffer();
	void NonUpdate();
	void Initialize(Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_TEXTURE,
		eROOT_PARAMATER_INDEX_CAMERA,
		eROOT_PARAMATER_INDEX_LIGHT,
		eROOT_PARAMATER_INDEX_SHADOWMAP,
		eROOT_PARAMATER_INDEX_MAX
	};

	struct InstanceDatas
	{
		DirectX::XMFLOAT4X4 aMatrix;
		DirectX::XMFLOAT4 offset;
		DirectX::XMFLOAT4 color;
	};
	std::shared_ptr<PrimitiveObject> mPrimitive;
	std::shared_ptr<VertexBufferObject> mVertexBuffer;
	std::shared_ptr<IndexBufferObject> mIndexBuffer;
	std::vector<InstanceDatas> mInstanceDatas;
	std::shared_ptr<VertexBufferObject> mInstanceVertexBuffer;
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;
	std::shared_ptr<ConstantBufferObject> mLightBuffer;
	std::shared_ptr<TextureObject> mTexObj;
	void (PrimitiveController::*mInstanceUpdate)();
	void (PrimitiveController::*mDescHeapUpdate)();
	std::shared_ptr<PipelineStateObject> mShadowmapPipeline;
	std::shared_ptr<PipelineStateObject> mShadowRenderPipeline;
	std::shared_ptr<RootSignatureObject> mShadowmapRootSignature;
	std::shared_ptr<RootSignatureObject> mShadowRenderRootSignature;
	std::shared_ptr<Dx12BufferObject> mShadowmapTexture;
	std::shared_ptr<Dx12CommandList> mShadowmapCmdList;
};

