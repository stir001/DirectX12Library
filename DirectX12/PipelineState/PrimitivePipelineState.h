#pragma once
#include "PipelineStateObject.h"
#include "DirectX12/Shader/ShaderDatasStructure.h"

#include <memory>

class RootSignatureObject;

class PrimitivePipelineState :
	public PipelineStateObject
{
public:
	PrimitivePipelineState(std::shared_ptr<RootSignatureObject>& rootsignature, 
		const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PrimitivePipelineState();
};

