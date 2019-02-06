#pragma once
#include "RootSignatureObject.h"
class FbxNonShadeRootSignature :
	public RootSignatureObject
{
public:
	FbxNonShadeRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~FbxNonShadeRootSignature();
};

