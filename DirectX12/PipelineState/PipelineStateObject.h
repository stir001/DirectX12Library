#pragma once
/**
*	@addtogroup Dx12PipelineState
*	@file PipelineStateObject.h
*	@brief �t�@�C���̏ڍ�
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/18
*/
#include "Shader/ShaderDatasStructure.h"

#include <string>
#include <d3d12.h>
#include <wrl.h>

/**
*	@ingroup Dx12PipelineState
*	@class PipelineStateObject
*	@brief ID3D12PipelineState���쐬���ێ�����
*/
class PipelineStateObject
{
public:
	/**
	*	@param[in]	name	GraphicsPipeline�̖��O
	*	@param[in]	gpsDesc	GraphicsPipeline��Desc
	*	@param[in]	dev		ID3D12Device�̎Q��
	*/
	PipelineStateObject(const std::string& name, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	/**
	*	@param[in]	name	ComputePipeline�̖��O
	*	@param[in]	cpsDesc	ComputePipeline��Desc
	*	@param[in]	dev		ID3D12Device�̎Q��
	*/
	PipelineStateObject(const std::string& name, D3D12_COMPUTE_PIPELINE_STATE_DESC& cpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	virtual ~PipelineStateObject();

	/**
	*	@brief	�쐬����PipelineState���擾����
	*	@return	�쐣��PipelineState
	*/
	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState() const;

	/**
	*	@brief PipelineState��ݒ肷��
	*	@param[in] cmdList	�ݒ肷��R�}���h���X�g
	*/
	void SetPipelineState(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList);

protected:
	PipelineStateObject();

	/**
	*	@brief	GraphicsPipelineState���쐬����
	*	@param[in]	name	PipelineState�̖��O
	*	@param[in]	gpsDesc	GraphicsPipeline��Desc
	*	@param[in]	dev		ID3D12Device�̎Q��
	*/
	void CreatePipelineState(const std::string& name, D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	/**
	*	@brief	ComputePipelineState���쐬����
	*	@param[in]	name	PipelineState�̖��O
	*	@param[in]	cpsDesc	ComputePipeline��Desc
	*	@param[in]	dev		ID3D12Device�̎Q��
	*/
	void CreatePipelineState(const std::string& name, D3D12_COMPUTE_PIPELINE_STATE_DESC& cpsDesc, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	/**
	*	@brief	GraphicsPipeline��Desc�ɃV�F�[�_�[���Z�b�g����
	*	@param[in,out]	gpsDesc		�V�F�[�_�[��o�^������GraphicsPipeline��Desc
	*	@param[in]	shaders		�o�^�������V�F�[�_�[
	*/
	void SetShaders(D3D12_GRAPHICS_PIPELINE_STATE_DESC& gpsDesc, const ShaderDatas& shaders);

	/**
	*	@brief	PipelineState�ɖ��O���Z�b�g����
	*	@param[in]	name	PipelineState�̖��O
	*/
	void SetName(const std::string& name);

	/**
	*	�쐬����PipelineState
	*/
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPipelineState;

	/**
	*	PipelineState�̖��O
	*/
	std::string mName;
};

