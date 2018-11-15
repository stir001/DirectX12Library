#pragma once
#include "PipelineState\PipelineStateObject.h"

class RootSignatureObject;

class BillboardPipelineState :
	public PipelineStateObject
{
public:
	BillboardPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature, const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~BillboardPipelineState();
};

