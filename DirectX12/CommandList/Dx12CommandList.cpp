#include "stdafx.h"
#include "Dx12CommandList.h"
#include "Buffer/Dx12BufferObject.h"
#include "Master/Dx12Ctrl.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "Util/CharToWChar.h"
#include "PipelineState/PipelineStateObject.h"
#include "RootSignature/RootSignatureObject.h"
#include "Buffer/IndexBufferObject.h"
#include "Buffer/VertexBufferObject.h"
#include "DrawObject/DrawCallIssuer.h"


Dx12CommandList::Dx12CommandList(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_COMMAND_LIST_TYPE type)
	:mType(type)
{
	std::wstring buf;
	ToWChar(buf, name);
	std::wstring comName = buf;
	comName += L"Allocator";
	dev->CreateCommandAllocator(type, IID_PPV_ARGS(&mCmdallcator));
	mCmdallcator->SetName(comName.data());

	comName = buf;
	comName += L"CommandList";
	dev->CreateCommandList(0, mType ,mCmdallcator.Get(),nullptr,IID_PPV_ARGS(&mCmdList));
	mCmdList->SetName(comName.data());

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

HRESULT Dx12CommandList::Reset()
{
	ClearControllers();
	mCmdallcator->Reset();
	return mCmdList->Reset(mCmdallcator.Get(), nullptr);
}

HRESULT Dx12CommandList::Reset(Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState)
{
	mCmdallcator->Reset();
	return mCmdList->Reset(mCmdallcator.Get(), pipelineState.Get());
}

HRESULT Dx12CommandList::SetDescriptorHeap(const std::shared_ptr<const Dx12DescriptorHeapObject>& descHeap) const
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

HRESULT Dx12CommandList::SetGraphicsRootDescriptorTable(std::shared_ptr<Dx12DescriptorHeapObject>& descHeap, int resourceIndex, int rootpramIndex)
{
	HRESULT rtn = WSAEINVAL;

	SetGraphicsRootDescriptorTable(rootpramIndex, descHeap->GetResourceGPUHandle(resourceIndex));

	rtn = GetDeviceRemoveReason();
	return rtn;
}

HRESULT Dx12CommandList::SetGraphicsRootDescriptorTable(int rootparamaterIndex, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle)
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

void Dx12CommandList::SetDrawController(std::shared_ptr<DrawObjectController> controller)
{
	mControllers.push_back(controller);
}

void Dx12CommandList::ClearControllers()
{
	mControllers.clear();
	mDrawIssuers.clear();
}

void Dx12CommandList::ExecuteBundle(std::shared_ptr<Dx12CommandList>& bundle)
{
	mCmdList->ExecuteBundle(bundle->GetCommandList().Get());
}

void Dx12CommandList::SetPipelineState(const std::shared_ptr<PipelineStateObject>& pipelineState)
{
	mCmdList->SetPipelineState(pipelineState->GetPipelineState().Get());
}

void Dx12CommandList::SetGraphicsRootSignature(const std::shared_ptr<RootSignatureObject>& rootSignature)
{
	mCmdList->SetGraphicsRootSignature(rootSignature->GetRootSignature().Get());
}

void Dx12CommandList::IASetIndexBuffer(const std::shared_ptr<IndexBufferObject>& indexBuffer)
{
	mCmdList->IASetIndexBuffer(&indexBuffer->GetView());
}

void Dx12CommandList::IASetVertexBuffers(const std::shared_ptr<VertexBufferObject> vertexBuffers[], unsigned int vertexBufferNum)
{
	std::vector<D3D12_VERTEX_BUFFER_VIEW> vbViews(vertexBufferNum);
	for (unsigned int i = 0; i < vertexBufferNum; ++i)
	{
		vbViews[i] = vertexBuffers[i]->GetView();
	}
	mCmdList->IASetVertexBuffers(0, vertexBufferNum, vbViews.data());
}

void Dx12CommandList::IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY primitiveTopology)
{
	mCmdList->IASetPrimitiveTopology(primitiveTopology);
}

void Dx12CommandList::DrawIndexedInstanced(unsigned int indexNum, unsigned int instanceNum, unsigned int startIndexLocation, unsigned int baseVertexLocation, unsigned int startInstanceLocation)
{
	mCmdList->DrawIndexedInstanced(indexNum, instanceNum, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void Dx12CommandList::DrawInstanced(unsigned int vertexNum, unsigned int instanceNum, unsigned int startVertexLocation, unsigned int startInstanceLocation)
{
	mCmdList->DrawInstanced(vertexNum, instanceNum, startVertexLocation, startInstanceLocation);
}

void Dx12CommandList::CopyResource(std::shared_ptr<Dx12BufferObject> dst, std::shared_ptr<Dx12BufferObject> src)
{
	mCmdList->CopyResource(dst->GetBuffer().Get(), src->GetBuffer().Get());
}

void Dx12CommandList::SetDrawCallIssuer(const std::shared_ptr<DrawCallIssuer>& drawIssuer)
{
	mDrawIssuers.push_back(drawIssuer);
}

void Dx12CommandList::StackDrawCall()
{
	for (auto& issuer : mDrawIssuers)
	{
		issuer->IssueDrawCall(shared_from_this());
	}
}

HRESULT Dx12CommandList::GetDeviceRemoveReason() const
{
	Microsoft::WRL::ComPtr<ID3D12Device> dev;
	mCmdList->GetDevice(IID_PPV_ARGS(&dev));
	return dev->GetDeviceRemovedReason();
}

