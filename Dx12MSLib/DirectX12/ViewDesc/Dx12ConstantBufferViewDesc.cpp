#include "stdafx.h"
#include "Dx12ConstantBufferViewDesc.h"

Dx12ConstantBufferViewDesc::Dx12ConstantBufferViewDesc(D3D12_GPU_VIRTUAL_ADDRESS gpuVirtualAddress, UINT sizeInBytes, unsigned int elementCount)
	:mElementCount(elementCount), mCbvView{ gpuVirtualAddress, (sizeInBytes + 0xff) & ~0xff}
{
}

Dx12ConstantBufferViewDesc::~Dx12ConstantBufferViewDesc()
{
}

void Dx12ConstantBufferViewDesc::CreateView(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_CPU_DESCRIPTOR_HANDLE& cpuhandle, D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, Microsoft::WRL::ComPtr<ID3D12Resource> resource)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = mCbvView;
	UINT incrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	for (unsigned int i = 0; i < mElementCount; ++i)
	{
		desc.BufferLocation = mCbvView.BufferLocation + i * BUFFER_ALIGNMENT;
		dev->CreateConstantBufferView(&desc, cpuhandle);
		cpuhandle.ptr += incrementSize;
		gpuHandle.ptr += incrementSize;
	}
}

unsigned int Dx12ConstantBufferViewDesc::GetViewDescCount() const
{
	return mElementCount;
}
