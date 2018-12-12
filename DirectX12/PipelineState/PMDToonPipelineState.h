#pragma once
#include "PipelineStateObject.h"
#include <memory>

class RootSignatureObject;

class PMDToonPipelineState :
	public PipelineStateObject
{
public:
	PMDToonPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PMDToonPipelineState();
};

