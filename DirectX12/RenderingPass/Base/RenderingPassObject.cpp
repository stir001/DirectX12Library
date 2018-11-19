#include "stdafx.h"
#include "RenderingPassObject.h"
#include <d3d12.h>

RenderingPassObject::RenderingPassObject(const std::string& passName): mPassName(passName), mIsActive(true)
{
}

RenderingPassObject::~RenderingPassObject()
{
}

void RenderingPassObject::SetActive(bool isActive)
{
	mIsActive = isActive;
}

bool RenderingPassObject::IsActive() const
{
	return mIsActive;
}

const std::string & RenderingPassObject::GetPassName() const
{
	return mPassName;
}

Microsoft::WRL::ComPtr<ID3D12Resource> RenderingPassObject::GetRenderTarget()
{
	return nullptr;
}
