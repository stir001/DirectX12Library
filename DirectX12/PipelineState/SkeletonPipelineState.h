#pragma once
#include "PipelineStateObject.h"
#include <memory>

class RootSignatureObject;

class SkeletonPipelineState :
	public PipelineStateObject
{
public:
	SkeletonPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~SkeletonPipelineState();
};

