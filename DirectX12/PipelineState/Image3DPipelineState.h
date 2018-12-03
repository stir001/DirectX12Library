#pragma once
#include "PipelineStateObject.h"
#include <memory>

class RootSignatureObject;

class Image3DPipelineState :
	public PipelineStateObject
{
public:
	Image3DPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~Image3DPipelineState();
};

