#include "stdafx.h"
#include "ShadowmapPass.h"
#include "CommandList/Dx12CommandList.h"
#include "Buffer/DepthBufferObject.h"
#include "Light/DirectionalLight.h"
#include <d3d12.h>

ShadowmapPass::ShadowmapPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev) : RenderingPassObject("ShadowmapPass")
{
	mCmdList = std::make_shared<Dx12CommandList>("ShadowmapPass", dev);
}

ShadowmapPass::~ShadowmapPass()
{
}

void ShadowmapPass::FirstUpdate()
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = { 0 };
	mCmdList->OMSetRenderTargets(0, cpuHandle);
}

void ShadowmapPass::PreExcuteUpdate()
{
}

void ShadowmapPass::ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue)
{
	ID3D12CommandList* cmdList = mCmdList->GetCommandList().Get();
	cmdQueue->ExecuteCommandLists(1, &cmdList);
}

void ShadowmapPass::ResetCommandList()
{
	mCmdList->Reset();
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> ShadowmapPass::GetCommandList()
{
	return mCmdList->GetCommandList();
}
