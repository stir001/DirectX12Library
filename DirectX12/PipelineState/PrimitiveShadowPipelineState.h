#pragma once
#include "PipelineStateObject.h"
#include <memory>
class RootSignatureObject;
class PrimitiveShadowPipelineState :
	public PipelineStateObject
{
public:
	PrimitiveShadowPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PrimitiveShadowPipelineState();
};

