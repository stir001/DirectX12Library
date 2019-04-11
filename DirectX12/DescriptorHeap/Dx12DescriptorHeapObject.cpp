#include "stdafx.h"
#include "Dx12DescriptorHeapObject.h"
#include "Buffer/Dx12BufferObject.h"
#include "ViewDesc/Dx12BufferViewDesc.h"
#include "Util/CharToWChar.h"
#include "CommandList/Dx12CommandList.h"

#include <d3d12.h>
#include <cassert>

D3D12_GPU_DESCRIPTOR_HANDLE operator +(const D3D12_GPU_DESCRIPTOR_HANDLE& handle, const unsigned int ptrVal)
{
	D3D12_GPU_DESCRIPTOR_HANDLE rtn = handle;
	rtn.ptr += ptrVal;
	return rtn;
}

Dx12DescriptorHeapObject::Dx12DescriptorHeapObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, std::vector<std::shared_ptr<Dx12BufferObject>>& buffers,D3D12_DESCRIPTOR_HEAP_TYPE heapType)
{
	unsigned int viewcount = 0;
	std::vector<std::shared_ptr<Dx12BufferViewDesc>> viewDescs;
	viewDescs.reserve(buffers.size());
	for (auto& buffer : buffers)
	{
		viewDescs.push_back(buffer->GetViewDesc());
		viewcount += buffer->GetViewDesc()->GetViewDescCount();
	}

	//https://msdn.microsoft.com/ja-jp/library/windows/desktop/dn770359(v=vs.85).aspx
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	heapDesc.Type = heapType;
	heapDesc.NumDescriptors = viewcount;
	if ((heapType == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) || (heapType == D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER))
	{
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	}
	else
	{
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	}
	heapDesc.NodeMask = 0;
	mHeapIncrementSize = dev->GetDescriptorHandleIncrementSize(heapType);
	
	HRESULT result = S_OK;
	
	result = dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescHeap));

	{
		std::wstring buf;
		ToWChar(buf, name);
		mDescHeap->SetName(buf.data());
	}

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = mDescHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = mDescHeap->GetGPUDescriptorHandleForHeapStart();

	ResourceBind rscbind;
	mResourceBinds.reserve(buffers.size());
	for (auto& buffer : buffers)
	{
		rscbind.cpuHandle = cpuHandle;
		rscbind.gpuHandle = gpuHandle;
		rscbind.resource = buffer;
		buffer->GetViewDesc()->CreateView(dev, cpuHandle, gpuHandle, buffer->GetBuffer());

		mResourceBinds.push_back(rscbind);
	}
}

Dx12DescriptorHeapObject::Dx12DescriptorHeapObject(const std::string & name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, HeapType heapType, unsigned int heapNum)
{

}

Dx12DescriptorHeapObject::~Dx12DescriptorHeapObject()
{
	mDescHeap.Reset();
	mResourceBinds.clear();
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12DescriptorHeapObject::GetCPUHeapHandleStart() const
{
	return mDescHeap->GetCPUDescriptorHandleForHeapStart();
}

D3D12_GPU_DESCRIPTOR_HANDLE Dx12DescriptorHeapObject::GetGPUHeapHandleStart() const
{
	return mDescHeap->GetGPUDescriptorHandleForHeapStart();
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Dx12DescriptorHeapObject::GetDescriptorHeap()const
{
	return mDescHeap;
}

void Dx12DescriptorHeapObject::SetDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const
{
	cmdList->SetDescriptorHeaps(1, mDescHeap.GetAddressOf());
}

void Dx12DescriptorHeapObject::SetDescriptorHeap(const std::shared_ptr<Dx12CommandList>& cmdList) const
{
	cmdList->SetDescriptorHeap(shared_from_this());
}

void Dx12DescriptorHeapObject::SetGraphicsDescriptorTable(std::shared_ptr<Dx12CommandList>& cmdList, unsigned int resourceHandleIndex, unsigned int rootParamaterIndex, unsigned int handleOffsetCount) const
{
	auto hr = cmdList->SetGraphicsRootDescriptorTable(rootParamaterIndex, GetResourceGPUHandle(resourceHandleIndex, handleOffsetCount));
	assert(SUCCEEDED(hr));
}

D3D12_GPU_DESCRIPTOR_HANDLE Dx12DescriptorHeapObject::GetResourceGPUHandle(unsigned int resourceHandleIndex, unsigned int handleOffsetCount) const
{
	return mResourceBinds[resourceHandleIndex].gpuHandle + (handleOffsetCount * mHeapIncrementSize);
}
