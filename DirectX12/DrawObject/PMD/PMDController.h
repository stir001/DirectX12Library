#pragma once
/**
*	@file PMDController.h
*	@brief PMDModelを操作するクラスの定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/15
*/
#include "DirectX12/DrawObject/DrawController3D.h"
#include "PMDDataStructure.h"

class PMDModel;
class ConstantBufferObject;
class VMDMotion;
class VMDPlayer;
class DirectionalLight;
class Dx12DescriptorHeapObject;
class PipelineStateObject;
class RootSignatureObject;
class Dx12BufferObject;

/**
*	@ingroup Model
*	@class PMDController
*	@brief PMDModelを操作するクラス
*/
class PMDController
	: public DrawController3D
{
	friend class PMDLoader;
public:
	/**
	*	@param[in]	model	PMDのデータ情報
	*	@param[in]	dlight	このモデル用の光源データ
	*	@param[in]	name	このモデルの名前
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	cmdList	このモデルの描画命令を積むコマンドリスト
	*/
	PMDController(std::shared_ptr<PMDModel>& model, std::shared_ptr<DirectionalLight>& dlight, const std::string& name,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev, std::shared_ptr<Dx12CommandList>& cmdList);
	~PMDController();

	/**
	*	描画命令をCommandListに積む
	*/
	void Draw();

	/**
	*	shadowmap作成用
	*/
	void DrawShadowmap();

	/**
	*	shadowを使う描画
	*/
	void DrawShadow();

	/**
	*	@brief	VMDモーションを設定する
	*	@param[in]	motion	モデルに適応するモーション
	*/
	void SetMotion(std::shared_ptr<VMDMotion> motion);

	/**
	*	@brief	設定したモーションを再生する
	*	@param[in]	loopFlag	ループするかどうかの設定 true:ループする false:ループしない
	*/
	void PlayMotion(bool isLoop = false);

	/**
	*	設定したモーションの再生を止める
	*/
	void StopMotion();

	/**
	*	@brief	モデルに適応するライトを設定する
	*	@param[in]	dlight	設定するDirectionalLight
	*/
	void SetLight(std::shared_ptr<DirectionalLight> dlight);

	/**
	*	@brief	描画する際に使用するPipelineStateを設定する
	*	@param[in]	pipelineState	使用するPipelineState
	*/
	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelineState);

	/**
	*	@brief	描画する際に使用するRootSignatureを設定する
	*	@param[in]	rootsignature	設定するRootSignature
	*/
	void SetGraphicsRootSignature(std::shared_ptr<RootSignatureObject>& rootsiganture);

	/**
	*	@brief	テクスチャを使って描画する際に使用するPipelineStateを設定する
	*	@param[in]	pipelineState	使用するPipelineState
	*/
	void SetToonPipelineState(std::shared_ptr<PipelineStateObject>& pipelineState);

	/**
	*	@brief	テクスチャを使って描画する際に使用するRootSignatureを設定する
	*	@param[in]	rootsignature	設定するRootSignature
	*/
	void SetToonRootSignature(std::shared_ptr<RootSignatureObject>& rootsiganture);

	/**
	*	shadowmapを作成するためのrootsignature
	*/
	void SetShadowmapRootsignature(std::shared_ptr<RootSignatureObject>& rootsignature);

	/**
	*	shadowmapを作成するためのpipelinestate
	*/
	void SetShadowmapPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	/**
	*	shadowmapを使ってToonレンダリングするためのrootsignature
	*/
	void SetShadowRenderRootsignature(std::shared_ptr<RootSignatureObject>& rootsignature);
	
	/**
	*	shadowmapを使ってToonレンダリングするためのpipelinestate
	*/
	void SetShadowRenderPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	/**
	*	shadowmapを使って通常レンダリングするためのpipelinestate
	*/
	void SetShadowBasicRenderPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	/**
	*	shadowmapを使って通常レンダリングするためのrootsignature
	*/
	void SetShadowBasicRenderRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);

	/**
	*	shadowmapw作成用のコマンドリスト
	*/
	void SetShadowmapCommandList(std::shared_ptr<Dx12CommandList> cmdList);

	/**
	*	descriptorHeapを再構築する 未実装
	*/
	void UpdateDescriptorHeap();

	/**
	*	shadowrender用のDescHeapを作成する
	*/
	void CreateShadowRenderDescHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, const std::string & name);

	/**
	*	@brief	shadowmapテクスチャを設定する
	*/
	void SetShadowmap(std::shared_ptr<Dx12BufferObject> shadowmap);
private:
	/**
	*	PMDモデル情報
	*/
	std::shared_ptr<PMDModel> mModel;

	/**
	*	ボーンの回転情報を書き込むバッファ
	*/
	std::shared_ptr<ConstantBufferObject> mBoneMatrixBuffer;

	/**
	*	ボーンの回転情報
	*/
	std::vector<DirectX::XMFLOAT4X4> mBoneMatrix;

	/**
	*	モデルのDirectionalLight
	*/
	std::shared_ptr<DirectionalLight> mDirLight;

	/**
	*	VMDを再生するクラス
	*/
	std::shared_ptr<VMDPlayer> mVmdPlayer;

	/**
	*	toonのテクスチャを使用するpipeline
	*/
	std::shared_ptr<PipelineStateObject> mToonPipeline;

	/**
	*	toonテクスチャを使用するrootsignature
	*/
	std::shared_ptr<RootSignatureObject> mToonRootsignature;

	/**
	*	shadowmap作成用pipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mShadowmapPipeline;

	/**
	*	shadowmap作成用rootsignature
	*/
	std::shared_ptr<RootSignatureObject> mShadowmapRootsignature;

	/**
	*	toonのshadowをつけるpipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mShadowToonRenderPipeline;

	/**
	*	toonのshadowをつけるrootsignature
	*/
	std::shared_ptr<RootSignatureObject> mShadowToonRenderRootsignature;

	/**
	*	toonなしのshadowをつけるpipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mShadowRenderPipeline;

	/**
	*	toonなしのshadowをつけるrootsignature
	*/
	std::shared_ptr<RootSignatureObject> mShadowRenderRootsignature;

	/**
	*	shadowmap作成用commandlist
	*/
	std::shared_ptr<Dx12CommandList> mShadowmapCmdList;

	/**
	*	このコントローラーで使用するDescriptorHeap
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;

	/**
	*	shadowレンダリング用descriptorheap
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> mShadowRenderDescHeap;

	/**
	*	shadowmapのテクスチャ
	*/
	std::shared_ptr<Dx12BufferObject> mShadowmapTexture;

	/**
	*	DescriptorHeapでのコンスタントバッファまでのオフセット
	*/
	unsigned int mConstantBufferOffset;

	/**
	*	テクスチャの数
	*/
	unsigned int mTextureNum;

	/**
	*	bundleの更新状態をもつステート変数
	*/
	void (PMDController::*mBundleUpdate)();

	/**
	*	@brief	toonを使用する部分と使用しない部分を分けながらCommandListに命令を登録する
	*	@param[in]	cmdList		命令を登録するコマンドリスト
	*/
	void DrawWhileSetTable(const std::shared_ptr<Dx12CommandList>& cmdList
	, std::pair<std::shared_ptr<PipelineStateObject>, std::shared_ptr<RootSignatureObject>> toonPair
	, std::pair<std::shared_ptr<PipelineStateObject>, std::shared_ptr<RootSignatureObject>> basicPair);

	/**
	*	@brief	マテリアル情報をCommandListに設定する
	*	@param[in]	cmdList		マテリアルを設定するCommandList
	*	@param[in]	resourceIndex	何番目のマテリアルかのインデックス
	*	@param[in]	offsetCount		DescriptorHeapのオフセット番号
	*/
	void SetMaterial(const std::shared_ptr<Dx12CommandList>& cmdList, unsigned int resourceIndex, unsigned int offsetCount);

	/**
	*	@brief	現在の情報を使ってDescriptorHeapを作成する
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	name	DescriptorHeapの名前
	*/
	void CreateDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, const std::string& name);

	/**
	*	@brief	このモデルのConstantBufferをCmdListに設定する
	*	@param[in]	cmdList		ConstantBufferを設定するCommandList
	*/
	void SetConstantBuffers(const std::shared_ptr<Dx12CommandList>& cmdList);

	/**
	*	BundelCommadnListを更新する
	*/
	void UpdateBundle();

	/**
	*	BundleCommandListを更新しない
	*/
	void NonUpdateBundle();


};

