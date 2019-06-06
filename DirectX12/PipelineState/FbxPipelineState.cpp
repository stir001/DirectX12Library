#include "stdafx.h"
#include "FbxPipelineState.h"
#include "Rootsignature/RootSignatureObject.h"

FbxPipelineState::FbxPipelineState(const std::shared_ptr<RootSignatureObject>& rootsignature,const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	auto gps = GetDefalutPipelineStateDesc();

	SetRootSignatureConfigure(gps, rootsignature);

	CreatePipelineState("Fbx", gps, dev);
}

FbxPipelineState::~FbxPipelineState()
{
}
