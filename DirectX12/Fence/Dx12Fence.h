#pragma once
#include <wrl.h>

struct ID3D12Fence;
struct ID3D12Device;

class Dx12Fence
{
public:
	Dx12Fence(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~Dx12Fence();

	Microsoft::WRL::ComPtr<ID3D12Fence> GetFence() const;
	unsigned int IncrementValue();
	unsigned int GetCompleteValue();
	unsigned int GetFenceValue() const;
	unsigned int Signal();
private:
	unsigned int mValue;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
};

