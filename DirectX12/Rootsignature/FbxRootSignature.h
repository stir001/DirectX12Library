#pragma once
#include "Rootsignature\RootSignatureObject.h"

class FbxRootSignature :
	public RootSignatureObject
{
public:
	FbxRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~FbxRootSignature();
};

