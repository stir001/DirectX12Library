#pragma once
#include "RootSignatureObject.h"

class Image2DRootSignature :
	public RootSignatureObject
{
public:
	Image2DRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~Image2DRootSignature();
};

