#include "stdafx.h"
#include "Dx12Fence.h"
#include <d3d12.h>

Dx12Fence::Dx12Fence(const Microsoft::WRL::ComPtr<ID3D12Device>& dev) : mValue(0)
{
	auto hr = dev->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetAddressOf()));
}


Dx12Fence::~Dx12Fence()
{
}

Microsoft::WRL::ComPtr<ID3D12Fence> Dx12Fence::GetFence() const
{
	return mFence;
}

unsigned int Dx12Fence::IncrementValue()
{
	return ++mValue;
}

unsigned int Dx12Fence::GetCompleteValue()
{
	return static_cast<unsigned int>(mFence->GetCompletedValue());
}

unsigned int Dx12Fence::GetFenceValue() const
{
	return mValue;
}

unsigned int Dx12Fence::Signal()
{
	mFence->Signal(mValue);
	return mValue;
}
