#include "stdafx.h"
#include "ShaderResourceObject.h"
#include "Util/Dx12Getter.h"

ShaderResourceObject::ShaderResourceObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, unsigned int width, unsigned int height, DXGI_FORMAT format)
	:Dx12BufferObject(name)
{
	mElementCount = width * height;
	mElementSize = dx12_getter::GetDxgiFormatByteSize(format);

	D3D12_HEAP_PROPERTIES heapProp;
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.CreationNodeMask = 0;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC rscDesc;
	rscDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	rscDesc.Alignment = 0;
	rscDesc.Width = width;
	rscDesc.Height = height;
	rscDesc.DepthOrArraySize = 1;
	rscDesc.MipLevels = 1;
	rscDesc.Format = format;
	rscDesc.SampleDesc.Count = 1;
	rscDesc.SampleDesc.Quality = 0;
	rscDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	rscDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

#ifdef _DEBUG
	mHeapProp = heapProp;
	mRscDesc = rscDesc;
#endif

	mDeafultState = mCurrentState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	mClearValue.Format = format;
	mClearValue.Color[0] = 0.0f;
	mClearValue.Color[1] = 0.0f;
	mClearValue.Color[2] = 0.0f;
	mClearValue.Color[3] = 1.0f;

	dev->CreateCommittedResource(&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&rscDesc,
		mDeafultState,
		nullptr,
		IID_PPV_ARGS(&mBuffer));

	mBuffer->SetName(mName.data());
	
}

ShaderResourceObject::ShaderResourceObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc)
	:Dx12BufferObject(rsc,name,dx12_getter::GetDxgiFormatByteSize(rsc->GetDesc().Format),static_cast<unsigned int>( rsc->GetDesc().Height * rsc->GetDesc().Width))
{
	mDeafultState = mCurrentState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	CreateShaderResourceViewDesc();
}

ShaderResourceObject::~ShaderResourceObject()
{
}
