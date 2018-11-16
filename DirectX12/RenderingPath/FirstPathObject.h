#pragma once
#include "RenderingPath/Base/RenderingPathObject.h"
#include <d3d12.h>
#include <vector>

class Dx12DescriptorHeapObject;
class RendertargetObject;
class CameraHolder;

class FirstPathObject :
	public RenderingPathObject
{
public:
	FirstPathObject(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, std::shared_ptr<Dx12DescriptorHeapObject> depthHeap,
		 int wWidth, int wHeight, std::shared_ptr<CameraHolder> holder);
	~FirstPathObject();

	void FirstUpdate();

	void PreExcuteUpdate();

	void ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue);

	void ResetCommandList();

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList();

	Microsoft::WRL::ComPtr<ID3D12Resource> GetRenderTarget();
private:
	D3D12_RECT mWndRect;
	std::shared_ptr<CameraHolder> mCameraHolder;
	std::shared_ptr<Dx12DescriptorHeapObject> mDepthHeap;
	std::shared_ptr<Dx12DescriptorHeapObject> mRtvHeap;
	std::shared_ptr<RendertargetObject> mRendertarget;
};

