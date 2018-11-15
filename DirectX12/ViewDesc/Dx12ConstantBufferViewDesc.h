#pragma once
/**
*	@file Dx12ConstantBufferViewDesc.h
*	@brief ConstantBufferVIewDesc���쐬����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/11
*/
#include "Dx12BufferViewDesc.h"

/**
*	@ingroup Dx12BufferView
*	@class Dx12ConstantBufferViewDesc
*	@brief D3D12_CONSTANT_BUFFER_VIEW_DESC��K�v���쐬���ێ�����
*/
class Dx12ConstantBufferViewDesc :
	public Dx12BufferViewDesc
{
public:
	/**
	*	@param[in]	gpuVirtualAddress	Buffer��gpuVirtualAddress
	*	@param[in]	sizeInBytes		Buffer��byte�P�ʂ̃T�C�Y
	*	@param[in]	elementCount	�v�f�̑���
	*/
	Dx12ConstantBufferViewDesc(D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress, UINT sizeInBytes, unsigned int elementCount);
	~Dx12ConstantBufferViewDesc();

	/**
	*	@brief	ConstantBufferViewDesc��DescriptorHeap�̔C�ӂ̕����ɐݒ肷��
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	cpuHandle	�ݒ肷��DescriptorHeap��CPUHandle
	*	@param[in]	gpuHandle	�ݒ肷��DescriptorHeap��GPUhandle
	*	@param[in]	resource	�R���X�^���g�o�b�t�@�Ƃ��č����ID3D12Resource;
	*/
	void CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource);

	/**
	*	@brief	����ConstantBufferView�����������Ă��邩��Ԃ�
	*	@return		����View�������Ă��邩
	*/
	unsigned int GetViewDescCount() const;
private:
	/**
	*	�ێ����Ă���ConstantBufferView
	*/
	const D3D12_CONSTANT_BUFFER_VIEW_DESC mCbvView;

	/**
	*	View������邩�̐�
	*/
	unsigned int mElementCount;

	/**
	*	ConstantBuffer���쐬����Ƃ��̃A���C�����g��
	*/
	const static unsigned int BUFFER_ALIGNMENT = 256;
};

