#pragma once
#include "RootSignatureObject.h"
class PMDBasicRootSignature :
	public RootSignatureObject
{
public:
	PMDBasicRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PMDBasicRootSignature();
};

