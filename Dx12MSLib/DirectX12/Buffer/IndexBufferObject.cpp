#include "stdafx.h"
#include "IndexBufferObject.h"
#include "Util/Dx12Getter.h"

#include "d3dx12.h"

IndexBufferObject::IndexBufferObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, unsigned int elementsize, unsigned int elementcount, DXGI_FORMAT format)
	:Dx12BufferObject(name)
{
	mElementCount = elementcount;
	mElementSize = elementsize;

	D3D12_RESOURCE_DESC rscDesc;
	rscDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	rscDesc.Alignment = 0;
	rscDesc.Width = (elementsize * elementcount);
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
#endif // _DEBUG

	mDeafultState = mCurrentState = D3D12_RESOURCE_STATE_GENERIC_READ;

	HRESULT hr = dev->CreateCommittedResource(&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&rscDesc,
		mDeafultState,
		nullptr,
		IID_PPV_ARGS(&mBuffer));

	mBuffer->SetName(mName.data());

	mIndexBufferView.BufferLocation = mBuffer->GetGPUVirtualAddress();
	mIndexBufferView.Format = format;
	mIndexBufferView.SizeInBytes = (elementsize * elementcount);

}

IndexBufferObject::IndexBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc)
	:Dx12BufferObject(rsc, name, dx12_getter::GetDxgiFormatByteSize(rsc->GetDesc().Format), static_cast<unsigned int>( rsc->GetDesc().Height * rsc->GetDesc().Width))
{
	mIndexBufferView.BufferLocation = mBuffer->GetGPUVirtualAddress();
	mIndexBufferView.Format = mBuffer->GetDesc().Format;
	mIndexBufferView.SizeInBytes = (mElementSize * mElementCount);
}

IndexBufferObject::~IndexBufferObject()
{
}

void IndexBufferObject::SetBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const
{
	cmdList->IASetIndexBuffer(&mIndexBufferView);
	//cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}