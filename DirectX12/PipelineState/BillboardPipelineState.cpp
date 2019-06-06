#include "stdafx.h"
#include "BillboardPipelineState.h"
#include "Rootsignature/RootSignatureObject.h"

BillboardPipelineState::BillboardPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature, const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	auto gps = GetDefalutPipelineStateDesc();
	gps.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	SetRootSignatureConfigure(gps, rootsignature);

	CreatePipelineState("BillBoard", gps, dev);
}


BillboardPipelineState::~BillboardPipelineState()
{
}
