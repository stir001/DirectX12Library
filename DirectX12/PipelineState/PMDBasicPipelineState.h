#pragma once
#include "PipelineStateObject.h"
#include <memory>

class RootSignatureObject;

class PMDBasicPipelineState :
	public PipelineStateObject
{
public:
	PMDBasicPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PMDBasicPipelineState();
};

