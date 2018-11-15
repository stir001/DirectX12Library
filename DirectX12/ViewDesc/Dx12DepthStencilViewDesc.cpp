#include "stdafx.h"
#include "Dx12DepthStencilViewDesc.h"

Dx12DepthStencilViewDesc::Dx12DepthStencilViewDesc(DXGI_FORMAT format)
{
	mDsvDesc.Format = format;
	mDsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	mDsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	mDsvDesc.Texture2D.MipSlice = 0;
}

Dx12DepthStencilViewDesc::~Dx12DepthStencilViewDesc()
{
}

void Dx12DepthStencilViewDesc::CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
	D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource)
{

	HRESULT result = dev->GetDeviceRemovedReason();
	UINT incrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	dev->CreateDepthStencilView(resource.Get(), &mDsvDesc, cpuHandle);
	cpuHandle.ptr += incrementSize;
	gpuHandle.ptr += incrementSize;

	result = dev->GetDeviceRemovedReason();
}
