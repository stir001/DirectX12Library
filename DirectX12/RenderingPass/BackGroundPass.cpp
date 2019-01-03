#include "stdafx.h"
#include "BackGroundPass.h"
#include "Buffer/RendertargetObject.h"
#include "CommandList/Dx12CommandList.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"

BackGroundPass::BackGroundPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, int wWidth, int wHeight)
	: RenderingPassObject("BackGroundPass")
	, mViewPort{ 0.0f, 0.0f, static_cast<FLOAT>(wWidth), static_cast<FLOAT>(wHeight), 0.0f, 1.0f}
	, mScissorRect{ 0 ,0, static_cast<LONG>(wWidth), static_cast<LONG>(wHeight)}
	, mRendertarget(std::make_shared<RendertargetObject>("BackGroundPathRendertarget", dev, wWidth, wHeight))
{
	std::vector<std::shared_ptr<Dx12BufferObject>> buffers = { mRendertarget };
	mRtvDescHeap = std::make_shared<Dx12DescriptorHeapObject>("BackGroundRtvHeap", dev, buffers, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mCmdList = std::make_shared<Dx12CommandList>("BackGround", dev);
}

BackGroundPass::~BackGroundPass()
{
}

void BackGroundPass::FirstUpdate()
{
	mCmdList->OMSetRenderTargets(1, mRtvDescHeap->GetCPUHeapHandleStart());
	mCmdList->ClearRenderTargetView(mRtvDescHeap->GetCPUHeapHandleStart(), mRendertarget->GetClearValue().Color, &mScissorRect);
	mCmdList->RSSetViewports(&mViewPort);
	mCmdList->RSSetScissorRects(&mScissorRect);
}

void BackGroundPass::PreExcuteUpdate()
{
	mCmdList->Close();
}

void BackGroundPass::ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue)
{
	cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList**)mCmdList->GetCommandList().GetAddressOf());
}

void BackGroundPass::ResetCommandList()
{
	mCmdList->Reset();
}

std::shared_ptr<Dx12CommandList> BackGroundPass::GetCommandList()
{
	return mCmdList;
}

std::shared_ptr<RendertargetObject> BackGroundPass::GetRendertargetObject()
{
	return mRendertarget;
}

std::shared_ptr<Dx12DescriptorHeapObject> BackGroundPass::GetRtvDescHeapObject()
{
	return mRtvDescHeap;
}
