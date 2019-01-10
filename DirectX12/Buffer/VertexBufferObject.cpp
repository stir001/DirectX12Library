#include "stdafx.h"
#include "VertexBufferObject.h"
#include "Master/Dx12ResultCheckDefine.h"

VertexBufferObject::VertexBufferObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, unsigned int elementsize, unsigned int elementcount)
	:Dx12BufferObject(name)
{
	mElementCount = elementcount;
	mElementSize = elementsize;

	D3D12_RESOURCE_DESC rscDesc;
	rscDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	rscDesc.Alignment = 0;
	rscDesc.Width = elementcount * elementsize;
	rscDesc.Height = 1;
	rscDesc.DepthOrArraySize = 1;
	rscDesc.MipLevels = 1;
	rscDesc.Format = DXGI_FORMAT_UNKNOWN;
	rscDesc.SampleDesc.Count = 1;
	rscDesc.SampleDesc.Quality = 0;
	rscDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	rscDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_HEAP_PROPERTIES heapProp;
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 0;
	heapProp.VisibleNodeMask = 0;

#ifdef _DEBUG
	mRscDesc = rscDesc;
	mHeapProp = heapProp;
#endif

	mDeafultState = mCurrentState = D3D12_RESOURCE_STATE_GENERIC_READ;

	HRESULT result = dev->CreateCommittedResource(&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&rscDesc,
		mDeafultState,
		nullptr,
		IID_PPV_ARGS(&mBuffer));

	mBuffer->SetName(mName.data());
	
	D12RESULTCHECK(result, dev);

	mVbView.BufferLocation = mBuffer->GetGPUVirtualAddress();
	mVbView.SizeInBytes = (elementcount * elementsize);
	mVbView.StrideInBytes = elementsize;

}

VertexBufferObject::VertexBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc, unsigned int elementSize, unsigned int elementCount)
	:Dx12BufferObject(rsc, name, elementSize, elementCount)
{
	mDeafultState = mCurrentState = D3D12_RESOURCE_STATE_GENERIC_READ;
	mVbView.BufferLocation = mBuffer->GetGPUVirtualAddress();
	mVbView.SizeInBytes = (mElementCount * mElementSize);
	mVbView.StrideInBytes = mElementSize;
}


VertexBufferObject::~VertexBufferObject()
{
}

D3D12_VERTEX_BUFFER_VIEW VertexBufferObject::GetView() const
{
	return mVbView;
}
