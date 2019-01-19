#pragma once
#include "Base/RenderingPassObject.h"
#include <d3d12.h>
#include <vector>

class Dx12DescriptorHeapObject;
class RendertargetObject;
class CameraHolder;

class ModelPass :
	public RenderingPassObject
{
public:
	ModelPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, std::shared_ptr<Dx12DescriptorHeapObject> depthHeap,
		 std::shared_ptr<Dx12DescriptorHeapObject> rtvHeap,
		 int wWidth, int wHeight, std::shared_ptr<CameraHolder> holder);
	~ModelPass();

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

private:
	D3D12_RECT mWndRect;
	std::shared_ptr<CameraHolder> mCameraHolder;
	std::shared_ptr<Dx12DescriptorHeapObject> mDepthHeap;
	std::shared_ptr<Dx12DescriptorHeapObject> mRtvHeap;
};

