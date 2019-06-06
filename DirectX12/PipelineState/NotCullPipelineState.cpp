#include "stdafx.h"
#include "NotCullPipelineState.h"


NotCullPipelineState::NotCullPipelineState(const std::string& name,
	const std::shared_ptr<RootSignatureObject>& rootsignature,
	const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	auto gps = GetDefalutPipelineStateDesc();
	gps.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;

	SetRootSignatureConfigure(gps, rootsignature);

	CreatePipelineState(name, gps, dev);
}


NotCullPipelineState::~NotCullPipelineState()
{
}
