#include "stdafx.h"
#include "Image2DPipelineState.h"
#include "Rootsignature/RootSignatureObject.h"

Image2DPipelineState::Image2DPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature, const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	auto gps = GetDefalutPipelineStateDesc();
	gps.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;

	SetRootSignatureConfigure(gps, rootsignature);

	CreatePipelineState("Image2D", gps, dev);
}


Image2DPipelineState::~Image2DPipelineState()
{
}
