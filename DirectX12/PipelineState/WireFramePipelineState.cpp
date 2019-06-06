#include "stdafx.h"
#include "WireFramePipelineState.h"
#include "Rootsignature/RootSignatureObject.h"

WireFramePipelineState::WireFramePipelineState(std::shared_ptr<RootSignatureObject>& rootsignature, const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	auto gps = GetDefalutPipelineStateDesc();
	gps.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
	gps.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

	SetRootSignatureConfigure(gps, rootsignature);

	CreatePipelineState("WireFrame", gps, dev);
}


WireFramePipelineState::~WireFramePipelineState()
{
}
