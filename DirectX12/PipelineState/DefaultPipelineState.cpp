#include "stdafx.h"
#include "DefaultPipelineState.h"


DefaultPipelineState::DefaultPipelineState(const std::string& name,
	const std::shared_ptr<RootSignatureObject>& rootsignature,
	const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	auto gps = GetDefalutPipelineStateDesc();

	SetRootSignatureConfigure(gps, rootsignature);

	CreatePipelineState(name, gps, dev);
}

DefaultPipelineState::~DefaultPipelineState()
{
}
