#pragma once
/**
*	@addtogroup DrawObjectLoader
*	@file DrawObjectLoader
*	@brief �`��I�u�W�F�N�g�����[�h������N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/14
*/

#include "DirectX12/Shader/ShaderDatasStructure.h"

#include <string>
#include <memory>
#include <d3d12.h>

class File;
class ShaderCompiler;
class PipelineStateObject;
class RootSignatureObject;
struct ShaderDatas;


/**
*	@ingroup DrawObjectLoader
*	@class DrawObjectLoader
*	@brief �`��I�u�W�F�N�g�����[�h������N���X
*/
class DrawObjectLoader
{
public:
	DrawObjectLoader();
	virtual ~DrawObjectLoader();

	/**
	*	@brief	���[�_�[�Ń��[�h�������ׂẴI�u�W�F�N�g�ɓn���R�}���h���X�g��ݒ肷��
	*	@param[in]	cmdList		�I�u�W�F�N�g�ɓn���R�}���h���X�g
	*/
	void SetRenderingCommnadList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
protected:
	/**
	*	���[�h����Ƃ��Ɏg�p����t�@�C��
	*/
	std::shared_ptr<File> mFp;

	/**
	*	�g�p���Ă���t�@�C���ւ̑��΃p�X�ۑ��p
	*/
	std::string mRelativePath;

	/**
	*	�g�p����V�F�[�_�[���ۑ��p
	*/
	ShaderDatas mShader;

	/**
	*	���[�h�����I�u�W�F�N�g�ɓn��pipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mPipelinestate;

	/**
	*	���[�h�����I�u�W�F�N�g�ɓn��rootsignature
	*/
	std::shared_ptr<RootSignatureObject> mRootsignature;

	/**
	*	���[�h�����I�u�W�F�N�g�ɓn��commandlist
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;

	/**
	*	@brief	�n���ꂽ�t�@�C���p�X�̑��΃p�X��mReletivePath�ϐ��ɕۑ�����
	*/
	void GetRelativePath(const std::string& path);

	/**
	*	pipelinestate���쐬����
	*/
	virtual void CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev) = 0;

	/**
	*	rootsiganture���쐬����
	*/
	virtual void CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev) = 0;
private:

};