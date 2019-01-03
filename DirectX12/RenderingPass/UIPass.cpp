#include "stdafx.h"
#include "UIPass.h"
#include "CommandList/Dx12CommandList.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "Buffer/RendertargetObject.h"

UIPass::UIPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	std::shared_ptr<Dx12DescriptorHeapObject> rtvHeap,
	std::shared_ptr<RendertargetObject> rendertarget,
	int wWidth, int wHeight):RenderingPassObject("UIPass")
	, mRtvDescHeap(rtvHeap), mRendertarget(rendertarget)
	, mSciddorRect{ 0, 0, static_cast<LONG>(wWidth), static_cast<LONG>(wHeight) }
	, mViewPort{ 0.0f, 0.0f, static_cast<FLOAT>(wWidth), static_cast<FLOAT>(wHeight), 0.0f, 1.0f}
{
	mCmdList = std::make_shared<Dx12CommandList>("UI", dev);
}

UIPass::~UIPass()
{
}

void UIPass::FirstUpdate()
{
	mCmdList->OMSetRenderTargets(1, mRtvDescHeap->GetCPUHeapHandleStart());
	mCmdList->RSSetViewports(&mViewPort);
	mCmdList->RSSetScissorRects(&mSciddorRect);
}

void UIPass::PreExcuteUpdate()
{
	mCmdList->Close();
}

void UIPass::ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue)
{
	cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList**)mCmdList->GetCommandList().GetAddressOf());
}

void UIPass::ResetCommandList()
{
	mCmdList->Reset();
}

std::shared_ptr<Dx12CommandList> UIPass::GetCommandList()
{
	return mCmdList;
}

std::shared_ptr<Dx12BufferObject> UIPass::GetRenderTarget()
{
	return mRendertarget;
}
