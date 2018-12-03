#pragma once
#include "PipelineStateObject.h"
#include <memory>

class RootSignatureObject;

class FbxPipelineState :
	public PipelineStateObject
{
public:
	FbxPipelineState(std::shared_ptr<RootSignatureObject>& rootsignature,
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~FbxPipelineState();
};

