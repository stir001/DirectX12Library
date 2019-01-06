#pragma once
#include "PipelineStateObject.h"

class RootSignatureObject;

class SkyBoxPipelineState :
	public PipelineStateObject
{
public:
	SkyBoxPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature
		, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~SkyBoxPipelineState();
};

