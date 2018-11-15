#pragma once
/**
*	@file Dx12RenderTargetViewDesc.h
*	@brief RenderTargetView���쐬���ێ�����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/11
*/

#include "Dx12BufferViewDesc.h"

/**
*	@ingroup Dx12BufferView
*	@class	Dx12RenderTargetViewDesc
*	@brief RenderTargetView���쐬���ێ�����
*/
class Dx12RenderTargetViewDesc :
	public Dx12BufferViewDesc
{
public:
	/**
	*	@param[in]	format	�v�f��ɑΉ�����DXGI_FORMAT
	*/
	Dx12RenderTargetViewDesc(DXGI_FORMAT format);
	~Dx12RenderTargetViewDesc();

	/**
	*	@brief	RenderTargetView��DescriptorHeap�̔C�ӂ̏ꏊ�Ɋ��蓖�Ă�
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	cpuHandle	DescriptorHeap�̊��蓖�Ă�CPUHandle
	*	@param[in]	gpuHandle	DescriptorHeap�̊��蓖�Ă�GPUHandle
	*	@param[in]	resource	RenderTarget�Ƃ��č���Ă���o�b�t�@
	*/
	void CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource);
private:
	/**
	*	�쐬����RenderTargetViewDesc
	*/
	D3D12_RENDER_TARGET_VIEW_DESC mRtvDesc;
};

