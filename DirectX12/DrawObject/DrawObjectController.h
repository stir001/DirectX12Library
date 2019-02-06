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
class DrawObjectController : public std::enable_shared_from_this<DrawObjectController>
{
public:
	/**
	*	@param[in]	bundleName	BundleCommandListの名前
	*	@param[in]	dev		ID3D12Deviceの名前
	*	@param[in]	cmdList	描画命令を積むコマンドリスト
	*/
	DrawObjectController(const std::string& bundleName, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		std::shared_ptr<Dx12CommandList>& cmdList);
	virtual ~DrawObjectController();

	/**
	*	@brief	使用するrootsignatureを設定する
	*	@param[in]	rootsignature	使用するrootsignature
	*/
	virtual void SetGraphicsRootSignature(const std::shared_ptr<RootSignatureObject>& rootsignature);

	/**
	*	@brief	使用するpipelinestateを設定する
	*	@param[in]	pipelinestate	使用するpipelinestate
	*/
	virtual void SetPipelineState(const std::shared_ptr<PipelineStateObject>& pipelinestate);

	/**
	*	@brief	使用するコマンドリストを設定する
	*	@param[in]	cmdList	使用するコマンドリスト
	*/
	virtual void SetCommandList(std::shared_ptr<Dx12CommandList>& cmdList);

	/**
	*	描画する
	*/
	virtual void Draw() = 0;
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
	*	使用するコマンドリスト パスごとに共通
	*/
	std::shared_ptr<Dx12CommandList> mCmdList;

	/**
	*	使用するバンドルコマンドリスト コントローラローラー単位で所持
	*/
	std::shared_ptr<Dx12CommandList> mBundleCmdList;

	/**
	*	使用するデバイス
	*/
	Microsoft::WRL::ComPtr<ID3D12Device> mDevice;
private:
};

