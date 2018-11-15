#include "stdafx.h"
#include "RenderingPathObject.h"
#include <d3d12.h>

RenderingPathObject::RenderingPathObject(const std::string& pathName): mPathName(pathName), mIsActive(true)
{
}

RenderingPathObject::~RenderingPathObject()
{
}

void RenderingPathObject::SetActive(bool isActive)
{
	mIsActive = isActive;
}

bool RenderingPathObject::IsActive() const
{
	return mIsActive;
}

const std::string & RenderingPathObject::GetPathName() const
{
	return mPathName;
}

Microsoft::WRL::ComPtr<ID3D12Resource> RenderingPathObject::GetRenderTarget()
{
	return nullptr;
}
