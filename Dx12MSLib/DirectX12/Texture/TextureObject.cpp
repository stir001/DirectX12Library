#include "stdafx.h"
#include "TextureObject.h"
#include "DirectXTex.h"
#include "Master/Dx12Ctrl.h"
#include "Rootsignature/RootSignatureObject.h"
#include "Buffer/ShaderResourceObject.h"

#include <d3d12.h>


TextureObject::TextureObject():mImageData(std::make_shared<DirectX::ScratchImage>())
{
}

TextureObject::~TextureObject()
{
}

int TextureObject::GetWidth() const
{
	return static_cast<int>(mShaderResource->GetBuffer()->GetDesc().Width);
}

int TextureObject::GetHeight() const
{
	return static_cast<int>(mShaderResource->GetBuffer()->GetDesc().Height);
}

std::shared_ptr<ShaderResourceObject> TextureObject::GetShaderResource() const
{
	return mShaderResource;
}

const std::string& TextureObject::GetTextureName() const
{
	return mTextureName;
}

const std::string & TextureObject::GetFilePath() const
{
	return mFilepath;
}

float TextureObject::GetGamma() const
{
	return mGamma;
}
