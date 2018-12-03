#pragma once
#include "RootSignatureObject.h"
class WireFrameRootSignature :
	public RootSignatureObject
{
public:
	WireFrameRootSignature(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~WireFrameRootSignature();
};

