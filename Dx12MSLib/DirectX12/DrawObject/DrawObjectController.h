#pragma once
/**
*	@addtogroup DrawObjectController
*	@file DrawObjectController.h
*	@brief �`��I�u�W�F�N�g�𑀍삷����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/21
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
*	@brief �`��I�u�W�F�N�g�𑀍삷����N���X
*/
class DrawObjectController
{
public:
	/**
	*	@param[in]	bundleName	BundleCommandList�̖��O
	*	@param[in]	dev		ID3D12Device�̖��O
	*	@param[in]	cmdList	�`�施�߂�ςރR�}���h���X�g
	*/
	DrawObjectController(const std::string& bundleName, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	virtual ~DrawObjectController();

	/**
	*	@brief	�g�p����rootsignature��ݒ肷��
	*	@param[in]	rootsignature	�g�p����rootsignature
	*/
	virtual void SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);

	/**
	*	@brief	�g�p����pipelinestate��ݒ肷��
	*	@param[in]	pipelinestate	�g�p����pipelinestate
	*/
	virtual void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	/**
	*	@brief	�g�p����R�}���h���X�g��ݒ肷��
	*	@param[in]	cmdList	�g�p����R�}���h���X�g
	*/
	virtual void SetCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
protected:

	/**
	*	�ʏ�`�掞�Ɏg�p����rootsignature
	*/
	std::shared_ptr<RootSignatureObject> mRootsignature;

	/**
	*	�ʏ�`�掞�Ɏg�p����pipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mPipelinestate;

	/**
	*	�g�p����R�}���h���X�g
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;

	/**
	*	�g�p����o���h���R�}���h���X�g
	*/
	std::shared_ptr<Dx12CommandList> mBundleCmdList;
private:
};

