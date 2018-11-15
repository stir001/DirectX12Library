#pragma once
#include "PipelineState\PipelineStateObject.h"

class RootSignatureObject;

class FbxPipelineState :
	public PipelineStateObject
{
public:
	FbxPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~FbxPipelineState();
};

