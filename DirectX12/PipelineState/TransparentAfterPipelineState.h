#pragma once
#include "PipelineStateObject.h"
class TransparentAfterPipelineState :
	public PipelineStateObject
{
public:
	TransparentAfterPipelineState(const std::string& name,
		const std::shared_ptr<RootSignatureObject>& rootsignature,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~TransparentAfterPipelineState();
};

