#pragma once
#include "PipelineStateObject.h"

class TransParentPipelineState :
	public PipelineStateObject
{
public:
	TransParentPipelineState(const std::string& name,
		const std::shared_ptr<RootSignatureObject>& rootsignature,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev, bool isUseDepth = true);
	~TransParentPipelineState();
};

