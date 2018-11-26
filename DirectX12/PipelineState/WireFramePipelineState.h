#pragma once
#include "PipelineStateObject.h"
#include <memory>

class RootSignatureObject;

class WireFramePipelineState :
	public PipelineStateObject
{
public:
	WireFramePipelineState(std::shared_ptr<RootSignatureObject>& rootsignature
		, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~WireFramePipelineState();
};

