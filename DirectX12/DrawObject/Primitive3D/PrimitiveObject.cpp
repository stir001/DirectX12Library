#include "stdafx.h"
#include "PrimitiveObject.h"
#include <d3d12.h>

PrimitiveObject::PrimitiveObject(const std::string& name): mName(name), mColor(INIT_COLOR)
{
}

PrimitiveObject::~PrimitiveObject()
{
}

const std::string& PrimitiveObject::GetName() const
{
	return mName;
}

const std::vector<PrimitiveVertex>& PrimitiveObject::GetVertices() const
{
	return mVertices;
}

const std::vector<unsigned int>& PrimitiveObject::GetIndices() const
{
	return mIndices;
}

const DirectX::XMFLOAT4 & PrimitiveObject::GetColor() const
{
	return mColor;
}

void PrimitiveObject::SetColor(const DirectX::XMFLOAT4 & color)
{
	mColor = color;
}
