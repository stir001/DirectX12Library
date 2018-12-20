#pragma once
#include "RootSignatureObject.h"
class PrimitiveShadowRenderRootSignature :
	public RootSignatureObject
{
public:
	PrimitiveShadowRenderRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PrimitiveShadowRenderRootSignature();
};

