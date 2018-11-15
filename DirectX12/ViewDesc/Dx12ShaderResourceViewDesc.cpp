#include "stdafx.h"
#include "Dx12ShaderResourceViewDesc.h"

Dx12ShaderResourceViewDesc::Dx12ShaderResourceViewDesc(DXGI_FORMAT format)
{
	mSrvDesc.Format = format;
	mSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	mSrvDesc.Texture2D.MipLevels = 1;
	mSrvDesc.Texture2D.MostDetailedMip = 0;
	mSrvDesc.Texture2D.PlaneSlice = 0;
	mSrvDesc.Texture2D.ResourceMinLODClamp = 0;
	mSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
}

Dx12ShaderResourceViewDesc::~Dx12ShaderResourceViewDesc()
{
}

void Dx12ShaderResourceViewDesc::CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
	D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource)
{
	UINT incrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	dev->CreateShaderResourceView(resource.Get(), &mSrvDesc, cpuHandle);
	cpuHandle.ptr += incrementSize;
	gpuHandle.ptr += incrementSize;
}
