#pragma once
#include "RootSignatureObject.h"

class SkeletonRootSignature :
	public RootSignatureObject
{
public:
	SkeletonRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~SkeletonRootSignature();
};

