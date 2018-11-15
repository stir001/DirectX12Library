#pragma once
#include "Rootsignature\RootSignatureObject.h"
class Image3DRootSignature :
	public RootSignatureObject
{
public:
	Image3DRootSignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~Image3DRootSignature();
};

