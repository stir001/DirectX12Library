#include "stdafx.h"
#include "Dx12BufferObject.h"
#include "ViewDesc/Dx12ConstantBufferViewDesc.h"
#include "ViewDesc/Dx12ShaderResourceViewDesc.h"
#include "ViewDesc/Dx12DepthStencilViewDesc.h"
#include "ViewDesc/Dx12RenderTargetViewDesc.h"
#include "ViewDesc/Dx12UnorderedAccessViewDesc.h"
#include "Util/CharToWChar.h"
#include "Master/Dx12Ctrl.h"
#include "Util//Dx12Getter.h"


Dx12BufferObject::Dx12BufferObject(const std::string& name) : mBuffer(nullptr),mElementBuffer(nullptr)
{
	std::wstring buf;
	ToWChar(buf, name);
	mName = buf;
}

Dx12BufferObject::Dx12BufferObject(Microsoft::WRL::ComPtr<ID3D12Resource>& rsc, const std::string& name,
	unsigned int elemenetSize, unsigned int elementCount)
	: mBuffer(rsc), mElementSize(elemenetSize), mElementCount(elementCount), mElementBuffer(nullptr)
{
#ifdef _DEBUG
	mRscDesc = rsc->GetDesc();
	D3D12_HEAP_FLAGS flag;
	rsc->GetHeapProperties(&mHeapProp, &flag);
#endif // _DEBUG

	std::wstring buf;
	ToWChar(buf, name);
	mName = buf;
	mBuffer->SetName(mName.data());
}

Dx12BufferObject::~Dx12BufferObject()
{
	if (mElementBuffer != nullptr)
	{
		mBuffer->Unmap(0, nullptr);
	}
	mElementBuffer = nullptr;
}

void Dx12BufferObject::WriteBuffer(const void* pData, unsigned int amountDatasSize)
{
	D3D12_RANGE range{ 0, amountDatasSize };
	mBuffer->Map(0, &range, &mElementBuffer);
	memcpy(mElementBuffer, pData, amountDatasSize);
	mBuffer->Unmap(0,&range);
	mElementBuffer = nullptr;
}

void Dx12BufferObject::Map()
{

}

void Dx12BufferObject::WriteBuffer256Alignment(const void* pData, unsigned int datasize, unsigned int datacount)
{
	D3D12_RANGE range{ 0, ((datasize + 0xff) & ~0xff) * datacount };
	mBuffer->Map(0, &range, &mElementBuffer);
	for (unsigned int i = 0; i < datacount; i++)
	{
		memcpy((void*)((char*)mElementBuffer + i * ((datasize + 0xff) & ~0xff)), (void*)((char*)pData + i * datasize), datasize);
	}
	mBuffer->Unmap(0, &range);
	mElementBuffer = nullptr;
}

Microsoft::WRL::ComPtr<ID3D12Resource>& Dx12BufferObject::GetBuffer()
{
	return mBuffer;
}

D3D12_RESOURCE_STATES Dx12BufferObject::GetCurrentState() const
{
	return mCurrentState;
}

void Dx12BufferObject::SetCurrentState(D3D12_RESOURCE_STATES state)
{
	mCurrentState = state;
}

D3D12_RESOURCE_STATES Dx12BufferObject::GetDefaultState() const
{
	return mDeafultState;
}

void Dx12BufferObject::CreateConstantBufferViewDesc()
{
	mViewDescs = std::make_shared<Dx12ConstantBufferViewDesc>(mBuffer->GetGPUVirtualAddress(),(mElementSize + 0xff) & ~0xff, mElementCount);
}

void Dx12BufferObject::CreateUnorderdAccessViewDesc()
{
	mViewDescs = std::make_shared<Dx12UnorderedAccessViewDesc>(mElementCount, mElementSize);
}

void Dx12BufferObject::CreateShaderResourceViewDesc()
{
	mViewDescs = std::make_shared<Dx12ShaderResourceViewDesc>(mBuffer->GetDesc().Format);
}

void Dx12BufferObject::CreateRenderTargetViewDesc()
{
	mViewDescs = std::make_shared<Dx12RenderTargetViewDesc>(mBuffer->GetDesc().Format);
}

void Dx12BufferObject::CreateDepthStecilViewDesc()
{
	DXGI_FORMAT format = mBuffer->GetDesc().Format;
	if (format != DXGI_FORMAT_D32_FLOAT &&
		format != DXGI_FORMAT_D16_UNORM &&
		format != DXGI_FORMAT_D24_UNORM_S8_UINT)
	{
		auto byteSize = dx12_getter::GetDxgiFormatByteSize(format);
		if (byteSize == 4)
		{
			format = DXGI_FORMAT_D32_FLOAT;
		}
		else if (byteSize == 2)
		{
			format = format;
		}
	}
	mViewDescs = std::make_shared<Dx12DepthStencilViewDesc>(format);
}

const std::shared_ptr<Dx12BufferViewDesc>& Dx12BufferObject::GetViewDesc() const
{
	return mViewDescs;
}

