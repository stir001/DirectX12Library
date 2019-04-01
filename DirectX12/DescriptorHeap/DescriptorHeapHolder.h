#pragma once
#include <memory>
class Dx12DescriptorHeapObject;

class DescriptorHeapHolder
{
public:
	DescriptorHeapHolder();
	~DescriptorHeapHolder();
	std::shared_ptr<Dx12DescriptorHeapObject> GetCBVSRVUAVHeap() const;
	std::shared_ptr<Dx12DescriptorHeapObject> GetRTVHeap() const;
	std::shared_ptr<Dx12DescriptorHeapObject> GetSMPHeap() const;

private:
	std::shared_ptr<Dx12DescriptorHeapObject> mCBVSRVUAVHeap;
	std::shared_ptr<Dx12DescriptorHeapObject> mRTVHeap;
	std::shared_ptr<Dx12DescriptorHeapObject> mSMPHeap;
};

