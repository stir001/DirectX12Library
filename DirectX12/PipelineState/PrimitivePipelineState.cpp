#include "stdafx.h"
#include "PrimitivePipelineState.h"
#include "RootSignature/RootSignatureObject.h"

PrimitivePipelineState::PrimitivePipelineState(std::shared_ptr<RootSignatureObject>& rootsignature, const Microsoft::WRL::ComPtr<ID3D12Device>& dev):PipelineStateObject()
{
	auto gps = GetDefalutPipelineStateDesc();

	SetRootSignatureConfigure(gps, rootsignature);

	CreatePipelineState("Primitive", gps, dev);
}

PrimitivePipelineState::~PrimitivePipelineState()
{
}
