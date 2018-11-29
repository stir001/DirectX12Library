#pragma once
#include "Base\RenderingPassObject.h"
#include <d3d12.h>

class RendertargetObject;
class Dx12DescriptorHeapObject;

class BackGroundPass :
	public RenderingPassObject
{
public:
	BackGroundPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, int wWidth, int wHeight);
	~BackGroundPass();

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
	*	Rendertarget�N���X���擾����
	*/	
	std::shared_ptr<RendertargetObject> GetRendertargetObject();

	/**
	*	DescriptorHeap�N���X���擾����
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> GetRtvDescHeapObject();

private:
	std::shared_ptr<RendertargetObject> mRendertarget;
	std::shared_ptr<Dx12DescriptorHeapObject> mRtvDescHeap;
	D3D12_VIEWPORT mViewPort;
	D3D12_RECT mScissorRect;
};
