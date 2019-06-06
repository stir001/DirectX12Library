#include "stdafx.h"
#include "Image3DPipelineState.h"
#include "Rootsignature/RootSignatureObject.h"


Image3DPipelineState::Image3DPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature, const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	auto gps = GetDefalutPipelineStateDesc();
	gps.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;

	SetRootSignatureConfigure(gps, rootsignature);

	CreatePipelineState("Image3D", gps, dev);
}


Image3DPipelineState::~Image3DPipelineState()
{
}
