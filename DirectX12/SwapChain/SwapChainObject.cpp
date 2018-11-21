#include "stdafx.h"
#include "SwapChainObject.h"
#include "Master/Dx12Ctrl.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "Buffer/Dx12BufferObject.h"
#include "Buffer/RendertargetObject.h"
#include "d3dx12.h"

#include <d3d12.h>
#include <dxgi1_4.h>

const unsigned int SWAPCHAINBUFFERCOUNT = 2;

SwapChainObject::SwapChainObject(HWND& hwnd, Microsoft::WRL::ComPtr<ID3D12Device>& device, Microsoft::WRL::ComPtr<IDXGIFactory4>& factory, Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue)
	:mSwapchain(nullptr), mRtvDescriptorHeap(nullptr)
{
	DX12CTRL_INSTANCE;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Height = static_cast<UINT>(d12.GetWindowSize().y);
	swapChainDesc.Width = static_cast<UINT>(d12.GetWindowSize().x);
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = SWAPCHAINBUFFERCOUNT;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//DXGI_SWAP_CHAIN_FLAG_NONPREROTATED;

	factory->CreateSwapChainForHwnd(cmdQueue.Get(), hwnd, &(swapChainDesc), nullptr, nullptr, (IDXGISwapChain1**)(mSwapchain.GetAddressOf()));
	DXGI_SWAP_CHAIN_DESC swdesc = {};
	mSwapchain->GetDesc(&swdesc);

	
	mRenderTargets.resize(swdesc.BufferCount);
	D3D12_RENDER_TARGET_VIEW_DESC rtdesc;
	rtdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtdesc.Texture2D.MipSlice = 0;
	rtdesc.Texture2D.PlaneSlice = 0;

	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	
	Microsoft::WRL::ComPtr<ID3D12Resource> tmpResource;
	
	for (unsigned int i = 0; i < mRenderTargets.size(); i++)
	{
		mSwapchain->GetBuffer(i, IID_PPV_ARGS(&tmpResource));
		
		std::string name = "SwapChainRenderTarget";
		name += i + '0';
		mRenderTargets[i].reset(new RendertargetObject(name, tmpResource));
		/*device->CreateRenderTargetView(mRenderTargets[i].Get(), &rtdesc, cpuhandle);
		cpuhandle.ptr += (mHeapIncrementsize);*/
	}

	mRtvDescriptorHeap = std::make_shared<Dx12DescriptorHeapObject>("SwapChainDescriptorHeap",device, mRenderTargets, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuhandle = mRtvDescriptorHeap->GetCPUHeapHandleStart();

	mHeapIncrementsize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	mBbindex = mSwapchain->GetCurrentBackBufferIndex();
}

SwapChainObject::~SwapChainObject()
{
}

Microsoft::WRL::ComPtr<IDXGISwapChain3> SwapChainObject::GetSwapChain() const
{
	return mSwapchain;
}

D3D12_CPU_DESCRIPTOR_HANDLE SwapChainObject::GetCurrentRTVHeap() const
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtn;
	rtn.ptr = mRtvDescriptorHeap->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart().ptr + mBbindex * mHeapIncrementsize;
	return rtn;
}

Microsoft::WRL::ComPtr<ID3D12Resource> SwapChainObject::GetCurrentRenderTarget() const
{
	return mRenderTargets[mBbindex]->GetBuffer();
}

HRESULT SwapChainObject::SwapChainPresent(UINT SyncInterval, UINT flags)
{
	HRESULT rtn = mSwapchain->Present(SyncInterval, flags);
	mBbindex = mSwapchain->GetCurrentBackBufferIndex();
	return rtn;
}

void SwapChainObject::Release()
{
	mRenderTargets.clear();
}

std::shared_ptr<Dx12DescriptorHeapObject> SwapChainObject::GetDescriptorHeap() const
{
	return mRtvDescriptorHeap;
}