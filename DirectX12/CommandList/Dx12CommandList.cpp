#include "stdafx.h"
#include "Dx12CommandList.h"
#include "Buffer/Dx12BufferObject.h"
#include "Master/Dx12Ctrl.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "Util/CharToWChar.h"


Dx12CommandList::Dx12CommandList(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_COMMAND_LIST_TYPE type)
	:mType(type)
{
	wchar_t* buf = nullptr;
	ToWChar(&buf, name);
	std::wstring comName = buf;
	comName += L"Allocator";
	dev->CreateCommandAllocator(type, IID_PPV_ARGS(&mCmdallcator));
	mCmdallcator->SetName(comName.data());

	comName = buf;
	comName += L"CommandList";
	dev->CreateCommandList(0, mType ,mCmdallcator.Get(),nullptr,IID_PPV_ARGS(&mCmdList));
	mCmdList->SetName(comName.data());

	delete buf;

	mName = name;
}

Dx12CommandList::~Dx12CommandList()
{
	ULONG count = mCmdList.Reset();
	count = mCmdallcator.Reset();
}

const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& Dx12CommandList::GetCommandList() const
{
	return mCmdList;
}

D3D12_COMMAND_LIST_TYPE Dx12CommandList::GetType() const
{
	return mType;
}

HRESULT Dx12CommandList::Reset() const
{
	mCmdallcator->Reset();
	return mCmdList->Reset(mCmdallcator.Get(), nullptr);
}

HRESULT Dx12CommandList::Reset(Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState)
{
	mCmdallcator->Reset();
	return mCmdList->Reset(mCmdallcator.Get(), pipelineState.Get());
}

HRESULT Dx12CommandList::SetDescriptorHeap(const std::shared_ptr<Dx12DescriptorHeapObject>& descHeap) const
{
	HRESULT rtn = WSAEINVAL;
	mCmdList->SetDescriptorHeaps(1, descHeap->GetDescriptorHeap().GetAddressOf());

#ifdef _DEBUG
	rtn = GetDeviceRemoveReason();
#else
	rtn = S_OK;
#endif

	return rtn;
}

HRESULT Dx12CommandList::SetDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descHeap) const
{
	HRESULT rtn = WSAEINVAL;

	mCmdList->SetDescriptorHeaps(1, descHeap.GetAddressOf());

	return rtn;
}

HRESULT Dx12CommandList::SetGraphicsRootDescriptorTabel(std::shared_ptr<Dx12DescriptorHeapObject>& descHeap, int resourceIndex, int rootpramIndex) const
{
	HRESULT rtn = WSAEINVAL;

	descHeap->SetGprahicsDescriptorTable(mCmdList, resourceIndex, rootpramIndex);

	rtn = GetDeviceRemoveReason();
	return rtn;
}

HRESULT Dx12CommandList::SetGraphicsRootDescriptorTable(int rootparamaterIndex, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle) const
{
	mCmdList->SetGraphicsRootDescriptorTable(rootparamaterIndex, gpuHandle);
	return GetDeviceRemoveReason();
}

HRESULT Dx12CommandList::TransitionBarrier(std::shared_ptr<Dx12BufferObject>& resource, D3D12_RESOURCE_STATES transitionState) const
{
	HRESULT rtn = WSAEINVAL;
	if (resource->GetCurrentState() == transitionState)
	{
		return rtn;
	}

	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource->GetBuffer().Get();
	barrier.Transition.StateAfter = transitionState;
	barrier.Transition.StateBefore = resource->GetCurrentState();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	mCmdList->ResourceBarrier(1, &barrier);

	resource->SetCurrentState(transitionState);

#ifdef _DEBUG
	rtn = Dx12Ctrl::Instance().GetDeviceRemoveReason();
#else
	rtn = S_OK;
#endif
	return rtn;
}

void Dx12CommandList::ClearDepthView(D3D12_CPU_DESCRIPTOR_HANDLE depthCpuHandle, float clearValue, D3D12_RECT* rect, unsigned int rectNum)
{
	mCmdList->ClearDepthStencilView(depthCpuHandle, D3D12_CLEAR_FLAG_DEPTH, clearValue, 0, rectNum, rect);
}

void Dx12CommandList::ClearStencilView(D3D12_CPU_DESCRIPTOR_HANDLE stencilCpuHandle, unsigned int clearValue, D3D12_RECT * rect, unsigned int rectNum)
{
	mCmdList->ClearDepthStencilView(stencilCpuHandle, D3D12_CLEAR_FLAG_STENCIL, 1.0f , clearValue, rectNum, rect);
}

void Dx12CommandList::OMSetRenderTargets(unsigned int renderTargetCount, D3D12_CPU_DESCRIPTOR_HANDLE rendertargetCpuHandle, D3D12_CPU_DESCRIPTOR_HANDLE * depthCpuHandle)
{
	mCmdList->OMSetRenderTargets(renderTargetCount, &rendertargetCpuHandle, false, depthCpuHandle);
}

void Dx12CommandList::ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rendertargetCpuHandle, const float colorRGBA[4], D3D12_RECT * rect, unsigned int rectNum)
{
	mCmdList->ClearRenderTargetView(rendertargetCpuHandle, colorRGBA, rectNum, rect);
}

void Dx12CommandList::RSSetViewports(D3D12_VIEWPORT * viewPorts, unsigned int viewportCount)
{
	mCmdList->RSSetViewports(viewportCount, viewPorts);
}

void Dx12CommandList::RSSetScissorRects(D3D12_RECT* scissorRects, unsigned int scissorRectsCount)
{
	mCmdList->RSSetScissorRects(scissorRectsCount, scissorRects);
}

const std::string & Dx12CommandList::GetName() const
{
	return mName;
}

void Dx12CommandList::Close() const
{
	mCmdList->Close();
}

HRESULT Dx12CommandList::GetDeviceRemoveReason() const
{
	Microsoft::WRL::ComPtr<ID3D12Device> dev;
	mCmdList->GetDevice(IID_PPV_ARGS(&dev));
	return dev->GetDeviceRemovedReason();
}

