#include "stdafx.h"
#include "FirstPathObject.h"
#include "CommandList/Dx12CommandList.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "Buffer/RendertargetObject.h"
#include "Camera/CameraHolder.h"
#include <d3d12.h>

FirstPathObject::FirstPathObject(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, std::shared_ptr<Dx12DescriptorHeapObject> depthHeap,
	int wWidth, int wHeight, std::shared_ptr<CameraHolder> holder): RenderingPathObject("Firstpath"),
	mDepthHeap(depthHeap), 
	mRendertarget(std::make_shared<RendertargetObject>(mPathName + "Rendertarget", dev, wWidth, wHeight)),
	mCameraHolder(holder), mWndRect{ 0, 0, static_cast<LONG>(wWidth), static_cast<LONG>(wHeight)}
{
	std::vector<std::shared_ptr<Dx12BufferObject>> buffers = { mRendertarget };
	mRtvHeap = std::make_shared<Dx12DescriptorHeapObject>(mPathName + "RendertargetHeap", dev, buffers , D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mCmdList = std::make_shared<Dx12CommandList>(mPathName, dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
}


FirstPathObject::~FirstPathObject()
{
}

void FirstPathObject::FirstUpdate()
{
	mCmdList->ClearDepthView(mDepthHeap->GetCPUHeapHandleStart(), 1.0f, &mWndRect);
	mCmdList->OMSetRenderTargets(1, mRtvHeap->GetCPUHeapHandleStart(), &mDepthHeap->GetCPUHeapHandleStart());
	float colors[4] = { 1.0f,1.0f,1.0f,1.0f };
	mCmdList->ClearRenderTargetView(mRtvHeap->GetCPUHeapHandleStart(), mRendertarget->GetClearValue().Color, &mWndRect);
	mCmdList->RSSetViewports(mCameraHolder->GetVi);
	mCmdList->RSSetScissorRects(mWndRect.data());
}

void FirstPathObject::PreExcuteUpdate()
{
	mCmdList->Close();
}

void FirstPathObject::ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue)
{
	cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList**)(mCmdList->GetCommandList().GetAddressOf()));
}

void FirstPathObject::ResetCommandList()
{
	mCmdList->Reset();
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> FirstPathObject::GetCommandList()
{
	return mCmdList->GetCommandList();
}

Microsoft::WRL::ComPtr<ID3D12Resource> FirstPathObject::GetRenderTarget()
{
	return mRendertarget->GetBuffer();
}
