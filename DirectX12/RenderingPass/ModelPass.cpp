#include "stdafx.h"
#include "ModelPass.h"
#include "CommandList/Dx12CommandList.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "Buffer/RendertargetObject.h"
#include "Camera/CameraHolder.h"
#include <d3d12.h>

ModelPass::ModelPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	std::shared_ptr<Dx12DescriptorHeapObject> depthHeap,
	std::shared_ptr<Dx12DescriptorHeapObject> rtvHeap,
	int wWidth, int wHeight, std::shared_ptr<CameraHolder> holder): RenderingPassObject("ModelPass"),
	mDepthHeap(depthHeap),  mRtvHeap(rtvHeap),
	mCameraHolder(holder), mWndRect{ 0, 0, static_cast<LONG>(wWidth), static_cast<LONG>(wHeight)}
{
	mCmdList = std::make_shared<Dx12CommandList>(mPassName, dev, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);
}


ModelPass::~ModelPass()
{
}

void ModelPass::FirstUpdate()
{
	mCmdList->ClearDepthView(mDepthHeap->GetCPUHeapHandleStart(), 1.0f, &mWndRect);
	mCmdList->OMSetRenderTargets(1, mRtvHeap->GetCPUHeapHandleStart(), &mDepthHeap->GetCPUHeapHandleStart());
	mCmdList->RSSetViewports(mCameraHolder->GetViewPorts().data(), static_cast<unsigned int>(mCameraHolder->GetViewPorts().size()));
	mCmdList->RSSetScissorRects(mCameraHolder->GetScissorRects().data(), static_cast<unsigned int>(mCameraHolder->GetViewPorts().size()));
}

void ModelPass::PreExcuteUpdate()
{
	mCmdList->Close();
}

void ModelPass::ExecutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue)
{
	cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList**)(mCmdList->GetCommandList().GetAddressOf()));
}

void ModelPass::ResetCommandList()
{
	mCmdList->Reset();
}

std::shared_ptr<Dx12CommandList> ModelPass::GetCommandList()
{
	return mCmdList;
}
