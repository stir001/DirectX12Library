#pragma once
/**
*	@addtogroup Dx12BufferView
*	@file �t�@�C����
*	@brief �t�@�C���̏ڍ�
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/11
*/
#include <d3d12.h>
#include <wrl.h>

/**
*	@ingroup Dx12BufferView
*	@class Dx12BufferViewDesc
*	@brief D3D12��ViewDesc�����N���X�̊��N���X
*/
class Dx12BufferViewDesc
{
public:
	Dx12BufferViewDesc();
	virtual ~Dx12BufferViewDesc();

	/**
	*	@brief	ViewDesc���������邩��Ԃ� �I�[�o�[���C�h���Ȃ��ꍇ�K��1��Ԃ�
	*	@return		ViewDesc�̑���
	*/
	virtual unsigned int GetViewDescCount() const;

	/**
	*	@brief	ViewDesc��DescreptorHeap�̔C�ӂ̏ꏊ�ɃZ�b�g����
	*	@param[in]	dev	ID3D12Device�̎Q��
	*	@param[in]	cpuHandle	DescriptorHeap��CPUHandle
	*	@param[in]	gpuHandle	DescriptorHeap��GPUHandle
	*	@param[in]	resource	�K�v�ȏꍇ�̂�View�ɕK�v��ID3D12Resource �f�t�H���g��nullptr
	*/
	virtual void CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
		D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr) = 0;
};

