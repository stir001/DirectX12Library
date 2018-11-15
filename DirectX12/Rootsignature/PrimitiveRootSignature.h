#pragma once
#include "RootSignatureObject.h"

class PrimitiveRootSignature :
	public RootSignatureObject
{
public:
	PrimitiveRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PrimitiveRootSignature();
};

