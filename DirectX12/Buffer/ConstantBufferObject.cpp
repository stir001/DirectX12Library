#include "stdafx.h"
#include "ConstantBufferObject.h"


ConstantBufferObject::ConstantBufferObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, unsigned int elementsize,unsigned int elementcount) :Dx12BufferObject(name), mSetBufferUpdate([](){})
{
	mSetBufferUpdate = []() {};

	D3D12_HEAP_PROPERTIES heapProp;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC rscDesc;
	rscDesc.Alignment = 0;
	rscDesc.DepthOrArraySize = 1;
	rscDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	rscDesc.MipLevels = 1;
	rscDesc.Width = elementcount * ((elementsize + 0xff) & ~0xff);
	rscDesc.Height = 1;
	rscDesc.SampleDesc.Count = 1;
	rscDesc.SampleDesc.Quality = 0;
	rscDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	rscDesc.Format = DXGI_FORMAT_UNKNOWN;
	rscDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

#ifdef _DEBUG
	mHeapProp = heapProp;
	mRscDesc = rscDesc;
#endif // _DEBUG

	mDeafultState = mCurrentState = D3D12_RESOURCE_STATE_GENERIC_READ;

	HRESULT result = dev->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&rscDesc,
		mDeafultState,
		nullptr,
		IID_PPV_ARGS(&mBuffer)
	);

	mBuffer->SetName(mName.data());

	mElementCount = elementcount;
	mElementSize = elementsize;

	CreateConstantBufferViewDesc();
}

ConstantBufferObject::ConstantBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc
	, D3D12_RESOURCE_STATES state, unsigned int elementsize, unsigned int elementcount)
	:Dx12BufferObject(rsc, name ,elementsize, elementcount)
{
	mSetBufferUpdate = []() {};
	mCurrentState = mDeafultState = state;
	CreateConstantBufferViewDesc();
}

ConstantBufferObject::~ConstantBufferObject()
{
}

void ConstantBufferObject::UpdateInstanceMatrix()
{
	mSetBufferUpdate();
}

void ConstantBufferObject::SetUpdate(std::function<void(void)>& updataFunc)
{
	mSetBufferUpdate = updataFunc;
}
