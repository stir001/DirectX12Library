#include "stdafx.h"
#include "LightObject.h"

LightObject::LightObject()
{
}


LightObject::~LightObject()
{
}

std::shared_ptr<ConstantBufferObject>& LightObject::GetLightBuffer()
{
	return mCbuffer;
}
