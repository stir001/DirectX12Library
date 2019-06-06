#include "stdafx.h"
#include "PrimitiveShadowPipelineState.h"
#include "Rootsignature/RootSignatureObject.h"

PrimitiveShadowPipelineState::PrimitiveShadowPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature,
	const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	auto gps = GetDefalutPipelineStateDesc();

	SetRootSignatureConfigure(gps, rootsignature);

	CreatePipelineState("PrimitiveShadowmap", gps, dev);
}


PrimitiveShadowPipelineState::~PrimitiveShadowPipelineState()
{
}
