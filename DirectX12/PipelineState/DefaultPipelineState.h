#pragma once
#include "PipelineStateObject.h"
#include <memory>

class RootSignatureObject;

class DefaultPipelineState :
	public PipelineStateObject
{
public:
	DefaultPipelineState(const std::string& name,
		const std::shared_ptr<RootSignatureObject>& rootsignature,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~DefaultPipelineState();
};

