#pragma once
#include "RootSignatureObject.h"
class PrimitiveShadowRootSignature :
	public RootSignatureObject
{
public:
	PrimitiveShadowRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PrimitiveShadowRootSignature();
};

