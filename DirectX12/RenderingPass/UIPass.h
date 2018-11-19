#pragma once
#include "RenderingPass\Base\RenderingPassObject.h"
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
	*	���̃p�X��ExcutePath���Ă΂�钼�O�Ɏ��s�����
	*/
	void PreExcuteUpdate();

	/**
	*	�R�}���h���X�g�̎��s������BGPU�҂��̐ӔC�͕���Ȃ�
	*/
	void ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue);

	/**
	*	ExcutePath�̎��s��Ɏ��s�����BCommnadList�̃��Z�b�g������
	*/
	void ResetCommandList();

	/**
	*	�R�}���h���X�g���擾����
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList();

	/**
	*	�ŏI�����_�����O���ʂ�Ԃ��֐��@��ԍŌ�̃p�X�̂ݕK�{����ȊO�͎������Ȃ��ł�����
	*/
	Microsoft::WRL::ComPtr<ID3D12Resource> GetRenderTarget();

private:
	D3D12_VIEWPORT mViewPort;
	D3D12_RECT mSciddorRect;
	std::shared_ptr<Dx12DescriptorHeapObject> mRtvDescHeap;
	std::shared_ptr<RendertargetObject> mRendertarget;
};

