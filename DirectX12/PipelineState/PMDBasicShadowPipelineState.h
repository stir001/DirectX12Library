#pragma once
#include "PipelineStateObject.h"
#include <memory>

class RootSignatureObject;

class PMDBasicShadowPipelineState :
	public PipelineStateObject
{
public:
	PMDBasicShadowPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PMDBasicShadowPipelineState();
};

