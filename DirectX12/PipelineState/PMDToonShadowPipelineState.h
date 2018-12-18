#pragma once
#include "PipelineStateObject.h"
#include <memory>

class RootSignatureObject;

class PMDToonShadowPipelineState :
	public PipelineStateObject
{
public:
	PMDToonShadowPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PMDToonShadowPipelineState();
};

