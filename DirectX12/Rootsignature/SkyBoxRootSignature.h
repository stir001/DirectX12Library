#pragma once
#include "RootSignatureObject.h"

class SkyBoxRootSignature : public RootSignatureObject
{
public:
	SkyBoxRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~SkyBoxRootSignature();
};

