#pragma once
#include "Base\RenderingPassObject.h"
#include <d3d12.h>

class Dx12DescriptorHeapObject;
class RendertargetObject;

class UIPass :
	public RenderingPassObject
{
public:
	UIPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		std::shared_ptr<Dx12DescriptorHeapObject> rtvHeap,
		std::shared_ptr<RendertargetObject> rendertarget,
		int wWidth, int wHeight);
	~UIPass();

	/**
	*	�e�t���[���̈�ԍŏ��Ɏ��s�����
	*/
	void FirstUpdate();

	/**
	*	���̃p�X��ExecutePath���Ă΂�钼�O�Ɏ��s�����
	*/
	void PreExecuteUpdate();

	/**
	*	�R�}���h���X�g�̎��s������BGPU�҂��̐ӔC�͕���Ȃ�
	*/
	void ExecutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue);

	/**
	*	ExecutePath�̎��s��Ɏ��s�����BCommnadList�̃��Z�b�g������
	*/
	void ResetCommandList();

	/**
	*	�R�}���h���X�g���擾����
	*/
	std::shared_ptr<Dx12CommandList> GetCommandList();

	/**
	*	�ŏI�����_�����O���ʂ�Ԃ��֐��@��ԍŌ�̃p�X�̂ݕK�{����ȊO�͎������Ȃ��ł�����
	*/
	std::shared_ptr<Dx12BufferObject> GetRenderTarget();

private:
	D3D12_VIEWPORT mViewPort;
	D3D12_RECT mScissorRect;
	std::shared_ptr<Dx12DescriptorHeapObject> mRtvDescHeap;
	std::shared_ptr<RendertargetObject> mRendertarget;
};

