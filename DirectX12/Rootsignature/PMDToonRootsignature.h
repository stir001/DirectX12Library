#pragma once
#include "RootSignatureObject.h"
class PMDToonRootsignature :
	public RootSignatureObject
{
public:
	PMDToonRootsignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~PMDToonRootsignature();
};

