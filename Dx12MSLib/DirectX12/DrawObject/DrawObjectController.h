#pragma once
/**
*	@addtogroup DrawObjectController
*	@file DrawObjectController.h
*	@brief 描画オブジェクトを操作する基底クラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/21
*/
#include <memory>
#include <wrl.h>
#include <d3d12.h>
#include <string>

class RootSignatureObject;
class PipelineStateObject;
class Dx12CommandList;

/**
*	@ingroup DrawObjectController
*	@class DrawObjcetController
*	@brief 描画オブジェクトを操作する基底クラス
*/
class DrawObjectController
{
public:
	/**
	*	@param[in]	bundleName	BundleCommandListの名前
	*	@param[in]	dev		ID3D12Deviceの名前
	*	@param[in]	cmdList	描画命令を積むコマンドリスト
	*/
	DrawObjectController(const std::string& bundleName, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	virtual ~DrawObjectController();

	/**
	*	@brief	使用するrootsignatureを設定する
	*	@param[in]	rootsignature	使用するrootsignature
	*/
	virtual void SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);

	/**
	*	@brief	使用するpipelinestateを設定する
	*	@param[in]	pipelinestate	使用するpipelinestate
	*/
	virtual void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	/**
	*	@brief	使用するコマンドリストを設定する
	*	@param[in]	cmdList	使用するコマンドリスト
	*/
	virtual void SetCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
protected:

	/**
	*	通常描画時に使用するrootsignature
	*/
	std::shared_ptr<RootSignatureObject> mRootsignature;

	/**
	*	通常描画時に使用するpipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mPipelinestate;

	/**
	*	使用するコマンドリスト
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;

	/**
	*	使用するバンドルコマンドリスト
	*/
	std::shared_ptr<Dx12CommandList> mBundleCmdList;
private:
};

