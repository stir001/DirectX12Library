#pragma once
#include "RootSignatureObject.h"
class PMDToonShadowRootSignature :
	public RootSignatureObject
{
public:
	PMDToonShadowRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PMDToonShadowRootSignature();
};

