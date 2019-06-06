#include "stdafx.h"
#include "PrimitiveShadowRenderPipelineState.h"
#include "Rootsignature/RootSignatureObject.h"

PrimitiveShadowRenderPipelineState::PrimitiveShadowRenderPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature,
	const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	auto gps = GetDefalutPipelineStateDesc();

	SetRootSignatureConfigure(gps, rootsignature);

	CreatePipelineState("PrimitiveShadowRender", gps, dev);
}


PrimitiveShadowRenderPipelineState::~PrimitiveShadowRenderPipelineState()
{
}
