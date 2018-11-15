#pragma once
#include "Rootsignature\RootSignatureObject.h"

class BillboardRootSignature :
	public RootSignatureObject
{
public:
	BillboardRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~BillboardRootSignature();
};

