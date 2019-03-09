#pragma once
#include "DrawObject\DrawCallIssuer.h"
#include <string>
#include <vector>
#include <wrl.h>

class Dx12DescriptorHeapObject;
struct ID3D12Device;

class ImageDrawIssuer :
	public DrawCallIssuer
{
public:
	/**
	*	コンストラクタ引数
	*/
	struct ConstructorArg
	{
		std::string name;
		std::shared_ptr<Dx12DescriptorHeapObject> descHeap;
		unsigned int texViewIndex;
		std::shared_ptr<VertexBufferObject> vertexBuffer;
		std::shared_ptr<VertexBufferObject> uvBuffer;
		std::shared_ptr<VertexBufferObject> matrixBuffer;
		std::shared_ptr<PipelineStateObject> pipelineState;
		std::shared_ptr<RootSignatureObject> rootSignature;
		Microsoft::WRL::ComPtr<ID3D12Device> dev;
	};

	ImageDrawIssuer(const ImageDrawIssuer::ConstructorArg& arg);
	~ImageDrawIssuer();

	/**
	*	@brief	描画命令を発行する
	*	@param[in]	cmdList	命令を積むコマンドリスト
	*/
	void IssueDrawCall(std::shared_ptr<Dx12CommandList>& cmdList);

	/**
	*	@brief	バンドルを更新する
	*/
	void UpdateBundle();

	/**
	*	@brief	pipelineStateを設定する
	*	@param[in]	pipelineState	使用するPipelineState
	*/
	void SetPipelineState(const std::shared_ptr<PipelineStateObject>& pipelineState);

	/**
	*	@brief	pipelineStateを設定する
	*	@param[in]	pipelineState	使用するPipelineState
	*/
	void SetRootSignature(const std::shared_ptr<RootSignatureObject>& rootSignature);

	/**
	*	@brief	基本の頂点情報を持つ頂点バッファを設定する
	*/
	void SetVertexBuffer(const std::shared_ptr<VertexBufferObject>& vertex);

	/**
	*	@brief	UV情報を持つ頂点バッファを設定する
	*/
	void SetUVBuffer(const std::shared_ptr<VertexBufferObject>& uv);

	/**
	*	@brief	状態変化用行列を持つ頂点バッファを設定する
	*/
	void SetMatrixBuffer(const std::shared_ptr<VertexBufferObject>& mat);
private:
	/**
	*	このrootsigantureで使うルートパラメーター
	*/
	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_TEXTURE,
	};

	enum eVertexBufferID
	{
		VERTEX,
		UV,
		MATRIX,
		VERTEXIDR_MAX,
	};

	/**
	*	通常描画時に使用するrootsignature
	*/
	std::shared_ptr<RootSignatureObject> mRootsignature;

	/**
	*	通常描画時に使用するpipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mPipelinestate;

	/**
	*	ヒープ
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;

	/**
	*	テクスチャのDescriptorHrapのView番号
	*/
	unsigned int mTexViewIndex;

	/**
	*	バーテックスバッファー
	*/
	std::vector<std::shared_ptr<VertexBufferObject>> mVertexBuffers;

};

