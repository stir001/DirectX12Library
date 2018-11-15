#pragma once
/**
*	@file Dx12DepthStencilViewDesc.h
*	@brief DepthstencilView���쐬����
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/11
*/

#include "Dx12BufferViewDesc.h"

/**
*	@ingroup Dx12BufferView
*	@class Dx12DepthStencilViewDesc
*	@brief DephtStencilView���쐬���ێ�����
*/
class Dx12DepthStencilViewDesc :
	public Dx12BufferViewDesc
{
public:
	/**
	*	@param[in]	format	�v�f��ɑΉ�����DXGI_FORMAT
	*/
	Dx12DepthStencilViewDesc(DXGI_FORMAT format);
	~Dx12DepthStencilViewDesc();

	/**
	*	@brief	DepthStencilView��DescriptorHeap�̔C�ӂ̏ꏊ�Ɋ��蓖�Ă�
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	cpuHandle	DescriptorHeap�̊��蓖�Ă�CPUHandle
	*	@param[in]	gpuHandle	DescriptorHeap�̊��蓖�Ă�GPUHandle
	*	@param[in]	resource	DepthStencilBuffer�Ƃ��č���Ă���o�b�t�@
	*/
	void CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle,
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource);
private:
	D3D12_DEPTH_STENCIL_VIEW_DESC mDsvDesc;
};

