#include "stdafx.h"
#include "RendertargetObject.h"
#include "Util/Dx12Getter.h"

RendertargetObject::RendertargetObject(const std::string& name,const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	unsigned int width, unsigned int height, DXGI_FORMAT format)
	:Dx12BufferObject(name)
{
	mElementCount = width * height;

	D3D12_RESOURCE_DESC desc;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = width;
	desc.Height = height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProp.CreationNodeMask = 1;
	heapProp.VisibleNodeMask = 1;

	mClearValue;
	mClearValue.Format = format;
	mClearValue.Color[0] = 0.5f;
	mClearValue.Color[1] = 0.5f;
	mClearValue.Color[2] = 0.5f;
	mClearValue.Color[3] = 1.0f;

	mDeafultState = mCurrentState = D3D12_RESOURCE_STATE_RENDER_TARGET;

	dev->CreateCommittedResource(&heapProp
		, D3D12_HEAP_FLAG_NONE
		, &desc
		, mDeafultState
		, &mClearValue
		, IID_PPV_ARGS(&mBuffer));

	mElementSize = dx12_getter::GetDxgiFormatByteSize(format);

	CreateRenderTargetViewDesc();
}

RendertargetObject::RendertargetObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc)
	:Dx12BufferObject(rsc, name, dx12_getter::GetDxgiFormatByteSize(rsc->GetDesc().Format), static_cast<unsigned int>( rsc->GetDesc().Width * rsc->GetDesc().Height))
{
	mDeafultState = mCurrentState = D3D12_RESOURCE_STATE_RENDER_TARGET;
	CreateRenderTargetViewDesc();
}

RendertargetObject::~RendertargetObject()
{
}

void RendertargetObject::SetClearValue(D3D12_CLEAR_VALUE clrValue)
{
	mClearValue = clrValue;
}

D3D12_CLEAR_VALUE RendertargetObject::GetClearValue() const
{
	return mClearValue;
}