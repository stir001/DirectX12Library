#include "stdafx.h"
#include "ShadowmapPass.h"
#include "CommandList/Dx12CommandList.h"
#include "Buffer/DepthBufferObject.h"
#include "Light/DirectionalLight.h"
#include "Master/Dx12Ctrl.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "CommandQueue/Dx12CommandQueue.h"
#include <d3d12.h>

ShadowmapPass::ShadowmapPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev) : RenderingPassObject("ShadowmapPass")
{
	auto size = Dx12Ctrl::Instance().GetWindowSize();
	mBufferSize = size.x > size.y ? size.x : size.y;
	mCmdList = std::make_shared<Dx12CommandList>("ShadowmapPass", dev);
	mShadowmapDepth = std::make_shared<DepthBufferObject>("ShadowmapDepthBuffer", dev, static_cast<int>(mBufferSize), static_cast<int>(mBufferSize), DXGI_FORMAT_R32_TYPELESS);
	std::vector<std::shared_ptr<Dx12BufferObject>> buffers(1);
	buffers[0] = mShadowmapDepth;
	mShadowDepthDescHeap = std::make_shared<Dx12DescriptorHeapObject>("ShadowmapDescHeap"
		, dev, buffers, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

ShadowmapPass::~ShadowmapPass()
{
}

void ShadowmapPass::FirstUpdate()
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = { 0 };
	mCmdList->TransitionBarrier(mShadowmapDepth, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	mCmdList->OMSetRenderTargets(0, cpuHandle, &mShadowDepthDescHeap->GetCPUHeapHandleStart());
	D3D12_RECT rect = { 0,0, static_cast<LONG>(mBufferSize), static_cast<LONG>(mBufferSize) };
	mCmdList->ClearDepthView(mShadowDepthDescHeap->GetCPUHeapHandleStart(), 1.0f, &rect, 1);
	D3D12_VIEWPORT viewport = { 0.f, 0.f, mBufferSize, mBufferSize, 0.0f, 1.0f };
	mCmdList->RSSetViewports(&viewport);
	mCmdList->RSSetScissorRects(&rect);
}

void ShadowmapPass::PreExecuteUpdate()
{
	mCmdList->TransitionBarrier(mShadowmapDepth, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	mCmdList->Close();
}

void ShadowmapPass::ExecutePath(std::shared_ptr<Dx12CommandQueue>& cmdQueue)
{
	cmdQueue->ExecuteCommandList(mCmdList);
}

void ShadowmapPass::ResetCommandList()
{
	mCmdList->Reset();
}

std::shared_ptr<Dx12CommandList> ShadowmapPass::GetCommandList()
{
	return mCmdList;
}

std::shared_ptr<Dx12BufferObject> ShadowmapPass::GetShadowmap()
{
	return mShadowmapDepth;
}
