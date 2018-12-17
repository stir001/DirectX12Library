#pragma once
#include "Rootsignature\RootSignatureObject.h"
class PMDShadowRootSignature :
	public RootSignatureObject
{
public:
	PMDShadowRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PMDShadowRootSignature();
};

