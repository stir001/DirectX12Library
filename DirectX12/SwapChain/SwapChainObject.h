#pragma once
#include <vector>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <wrl.h>
#include <memory>

class Dx12DescriptorHeapObject;
class Dx12BufferObject;

class SwapChainObject
{
public:
	SwapChainObject(HWND& hwnd, Microsoft::WRL::ComPtr<ID3D12Device>& device, Microsoft::WRL::ComPtr<IDXGIFactory4>& factory, Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue);
	~SwapChainObject();
	Microsoft::WRL::ComPtr<IDXGISwapChain3> GetSwapChain() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTVHeap() const;
	std::shared_ptr<Dx12BufferObject> GetCurrentRenderTarget() const;
	std::shared_ptr<Dx12DescriptorHeapObject> GetDescriptorHeap() const;
	HRESULT SwapChainPresent(UINT SyncInterval, UINT flags);
	void Release();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain3> mSwapchain;
	std::shared_ptr<Dx12DescriptorHeapObject> mRtvDescriptorHeap;
	std::vector<std::shared_ptr<Dx12BufferObject>> mRenderTargets;
	unsigned int mHeapIncrementsize;
	unsigned int mBbindex;//back buffer index
};

