#pragma once
/**
*	@file Dx12UnorderedAccessViewDesc.h
*	@brief UnorderedAccessView���쐬���ێ�����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/11
*/

#include "Dx12BufferViewDesc.h"

/**
*	@ingroup Dx12BufferView
*	@class	Dx12UnorderedAccessViewDesc
*	@brief UnorderdAccessView���쐬���ێ�����
*/
class Dx12UnorderedAccessViewDesc :
	public Dx12BufferViewDesc
{
public:
	/**
	*	@param[in]	numElements	�v�f�̑���
	*	@param[in]	stride		��v�f�̃T�C�Y
	*/
	Dx12UnorderedAccessViewDesc(UINT numElements, UINT stride);
	~Dx12UnorderedAccessViewDesc();

	/**
	*	@brief	UnorderedAccessView��DescriptorHeap�̔C�ӂ̏ꏊ�Ɋ��蓖�Ă�
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	cpuHandle	DescriptorHeap�̊��蓖�Ă�CPUHandle
	*	@param[in]	gpuHandle	DescriptorHeap�̊��蓖�Ă�GPUHandle
	*	@param[in]	resource	UnorderdAccess�Ƃ��č���Ă���o�b�t�@
	*/
	void CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr);
private:
	/**
	*	�쐬����UnorderdAccessView
	*/
	const D3D12_UNORDERED_ACCESS_VIEW_DESC mUavDesc;
};

