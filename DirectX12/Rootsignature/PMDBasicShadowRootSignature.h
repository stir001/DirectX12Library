#pragma once
#include "RootSignatureObject.h"
class PMDBasicShadowRootSignature :
	public RootSignatureObject
{
public:
	PMDBasicShadowRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PMDBasicShadowRootSignature();
};

