#include "stdafx.h"
#include "Dx12UnorderedAccessViewDesc.h"

Dx12UnorderedAccessViewDesc::Dx12UnorderedAccessViewDesc(UINT numElements,UINT stride)
	:mUavDesc{ DXGI_FORMAT_UNKNOWN, D3D12_UAV_DIMENSION_BUFFER, 0, numElements, stride, 0, D3D12_BUFFER_UAV_FLAG_NONE }
{
	//D3D12_UNORDERED_ACCESS_VIEW_DESC
	//https://msdn.microsoft.com/en-us/library/windows/desktop/dn770451(v=vs.85).aspx
	
	//D3D12_BUFFER_UAV
	//https://msdn.microsoft.com/en-us/library/windows/desktop/dn770345(v=vs.85).aspx
}

Dx12UnorderedAccessViewDesc::~Dx12UnorderedAccessViewDesc()
{
}

void Dx12UnorderedAccessViewDesc::CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, 
	D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource)
{
	UINT incrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	dev->CreateUnorderedAccessView(resource.Get(), nullptr, &mUavDesc, cpuHandle);
	cpuHandle.ptr += incrementSize;
	gpuHandle.ptr += incrementSize;
}
