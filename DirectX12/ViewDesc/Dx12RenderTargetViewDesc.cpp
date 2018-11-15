#include "stdafx.h"
#include "Dx12RenderTargetViewDesc.h"

Dx12RenderTargetViewDesc::Dx12RenderTargetViewDesc(DXGI_FORMAT format)
{
	mRtvDesc.Format = format;
	mRtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	mRtvDesc.Texture2D.MipSlice = 0;
	mRtvDesc.Texture2D.PlaneSlice = 0;
}

Dx12RenderTargetViewDesc::~Dx12RenderTargetViewDesc()
{
}

void Dx12RenderTargetViewDesc::CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
	D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource)
{
	UINT incrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	dev->CreateRenderTargetView(resource.Get(), &mRtvDesc, cpuHandle);
	cpuHandle.ptr += incrementSize;
	gpuHandle.ptr += incrementSize;
}
