#pragma once
#include "PipelineStateObject.h"

class RootSignatureObject;

class Image2DPipelineState :
	public PipelineStateObject
{
public:
	Image2DPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~Image2DPipelineState();
};

