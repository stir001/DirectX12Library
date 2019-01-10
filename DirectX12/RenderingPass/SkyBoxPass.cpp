#include "stdafx.h"
#include "SkyBoxPass.h"
#include "CommandList/Dx12CommandList.h"
#include "Buffer/RendertargetObject.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "DrawObject/SkyBox.h"
#include "Buffer/RendertargetObject.h"
#include "Camera/CameraHolder.h"
#include "Master/Dx12Ctrl.h"

#include <d3d12.h>


SkyBoxPass::SkyBoxPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	std::shared_ptr<Dx12DescriptorHeapObject> rtvHeap,
	std::shared_ptr<RendertargetObject> rendertarget,
	int width, int height, SkyBoxTextures& textures)
	: RenderingPassObject("SkyBoxPass")
	, mRendertarget(rendertarget)
	, mRtvDescHeap(rtvHeap)
	, mHolder(Dx12Ctrl::Instance().GetCameraHolder())
	, mWndWidth(width), mWndHeight(height)
{
	mCmdList = std::make_shared<Dx12CommandList>("SkyBox", dev);
	mSkyBox = std::make_shared<SkyBox>(mCmdList, textures, mHolder);
}


SkyBoxPass::~SkyBoxPass()
{
}

void SkyBoxPass::FirstUpdate()
{
	mCmdList->OMSetRenderTargets(1, mRtvDescHeap->GetCPUHeapHandleStart());
	D3D12_RECT rect = { 0, 0, mWndWidth, mWndHeight };
	mCmdList->ClearRenderTargetView(mRtvDescHeap->GetCPUHeapHandleStart(), mRendertarget->GetClearValue().Color, &rect);
	mCmdList->RSSetViewports(mHolder->GetViewPorts().data(), static_cast<unsigned int>(mHolder->GetViewPorts().size()));
	mCmdList->RSSetScissorRects(mHolder->GetScissorRects().data(), static_cast<unsigned int>(mHolder->GetScissorRects().size()));
}

void SkyBoxPass::PreExcuteUpdate()
{
	mSkyBox->UpdateCameraBuffer();
	mSkyBox->Draw();
	mCmdList->Close();
}

void SkyBoxPass::ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue)
{
	cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList**)(mCmdList->GetCommandList().GetAddressOf()));
}

void SkyBoxPass::ResetCommandList()
{
	mCmdList->Reset();
}

std::shared_ptr<Dx12CommandList> SkyBoxPass::GetCommandList()
{
	return mCmdList;
}

std::shared_ptr<Dx12BufferObject> SkyBoxPass::GetRenderTarget()
{
	return mRendertarget;
}
