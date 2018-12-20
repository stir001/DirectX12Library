#pragma once
#include "PipelineStateObject.h"
#include <memory>
class RootSignatureObject;

class PrimitiveShadowRenderPipelineState :
	public PipelineStateObject
{
public:
	PrimitiveShadowRenderPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PrimitiveShadowRenderPipelineState();
};

