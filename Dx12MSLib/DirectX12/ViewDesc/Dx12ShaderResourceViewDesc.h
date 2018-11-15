#pragma once
/**
*	@file Dx12ShaderResourceViewDesc.h
*	@brief ShadreResourceView���쐬���ێ�����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/11
*/

#include "Dx12BufferViewDesc.h"

/**
*	@ingroup Dx12BufferView
*	@class	Dx12ShaderResourceViewDesc
*	@brief ShadreResourceView���쐬���ێ�����
*/
class Dx12ShaderResourceViewDesc :
	public Dx12BufferViewDesc
{
public:
	/**
	*	@param[in]	format	�v�f��ɑΉ�����DXGI_FORMAT
	*/
	Dx12ShaderResourceViewDesc(DXGI_FORMAT format);
	~Dx12ShaderResourceViewDesc();

	/**
	*	@brief	ShadreResourceView��DescriptorHeap�̔C�ӂ̏ꏊ�Ɋ��蓖�Ă�
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	cpuHandle	DescriptorHeap�̊��蓖�Ă�CPUHandle
	*	@param[in]	gpuHandle	DescriptorHeap�̊��蓖�Ă�GPUHandle
	*	@param[in]	resource	ShaderResource�Ƃ��č���Ă���o�b�t�@
	*/
	void CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr);
private:
	/**
	*	�쐬����ShaderResourceView
	*/
	D3D12_SHADER_RESOURCE_VIEW_DESC mSrvDesc;
};

