#pragma once
/**
*	@file PMDController.h
*	@brief PMDModelを操作するクラスの定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/11/15
*/
#include "DrawObject/DrawController3D.h"
#include "PMDDataStructure.h"

class PMDModel;
class ConstantBufferObject;
class VMDMotion;
class VMDPlayer;
class DirectionalLight;
class Dx12DescriptorHeapObject;
class PipelineStateObject;
class RootSignatureObject;

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
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev,Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	~PMDController();

	/**
	*	描画命令をCommandListに積む
	*/
	void Draw();

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
	*	@brief	マテリアルを使って描画する際に使用するPipelineStateを設定する
	*	@param[in]	pipelineState	使用するPipelineState
	*/
	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelineState);

	/**
	*	@brief	マテリアルを使って描画する際に使用するRootSignatureを設定する
	*	@param[in]	rootsignature	設定するRootSignature
	*/
	void SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsiganture);

	/**
	*	@brief	テクスチャを使って描画する際に使用するPipelineStateを設定する
	*	@param[in]	pipelineState	使用するPipelineState
	*/
	void SetSubPipelineState(std::shared_ptr<PipelineStateObject>& pipelineState);

	/**
	*	@brief	テクスチャを使って描画する際に使用するRootSignatureを設定する
	*	@param[in]	rootsignature	設定するRootSignature
	*/
	void SetSubRootSignature(std::shared_ptr<RootSignatureObject>& rootsiganture);

	void UpdateDescriptorHeap();
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
	*	テクスチャを使用するときに使うPipelineState
	*/
	std::shared_ptr<PipelineStateObject> mSubPipeline;

	/**
	*	テクスチャを使用するときに使うRootSignature
	*/
	std::shared_ptr<RootSignatureObject> mSubRootsignature;

	/**
	*	このコントローラーで使用するDescriptorHeap
	*/
	std::unique_ptr<Dx12DescriptorHeapObject> mDescHeap;

	/**
	*	bundleの更新状態をもつステート変数
	*/
	void (PMDController::*mBundleUpdate)();

	/**
	*	@brief	マテリアルやテクスチャを使用する部分を分けながらCommandListに命令を登録する
	*	@param[in]	cmdList		命令を登録するコマンドリスト
	*/
	void DrawWhileSetTable(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);

	/**
	*	@brief	マテリアルに設定されているテクスチャをCommandListに設定する
	*	@param[in]	cmdList		テクスチャを設定するCommandList
	*	@param[in]	matarial	テクスチャ情報を保持しているマテリアル
	*/
	void SetTexture(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, PMDMaterial& material);

	/**
	*	@brief	マテリアル情報をCommandListに設定する
	*	@param[in]	cmdList		マテリアルを設定するCommandList
	*	@param[in]	resourceIndex	何番目のマテリアルかのインデックス
	*	@param[in]	offsetCount		DescriptorHeapのオフセット番号
	*/
	void SetMaterial(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, unsigned int resourceIndex, unsigned int offsetCount);

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
	void SetConstantBuffers(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);

	/**
	*	BundelCommadnListを更新する
	*/
	void UpdateBundle();

	/**
	*	BundleCommandListを更新しない
	*/
	void NonUpdateBundle();
};

