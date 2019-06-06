#pragma once
#include "PipelineStateObject.h"
class NotCullPipelineState :
	public PipelineStateObject
{
public:
	NotCullPipelineState(const std::string& name,
		const std::shared_ptr<RootSignatureObject>& rootsignature,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~NotCullPipelineState();
};

