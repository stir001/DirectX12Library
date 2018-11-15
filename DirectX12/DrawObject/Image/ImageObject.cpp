#include "stdafx.h"
#include "ImageObject.h"
#include "Master/Dx12Ctrl.h"
#include "Buffer/VertexBufferObject.h"
#include "Texture/TextureObject.h"
#include "Util/XMFloatOperators.h"
#include "Util/Rect.h"

ImageObject::ImageObject(int inwidth, int inheight, std::shared_ptr<TextureObject> intexObj)
	:mWidth(inwidth), mHeight(inheight)
	, mTexObj(intexObj)
{
}

std::shared_ptr<ImageObject> ImageObject::Create(int width, int height, std::shared_ptr<TextureObject> texObj)
{
	class CreateImageObjcetHelper : public ImageObject
	{
	public:
		CreateImageObjcetHelper(int width, int height, std::shared_ptr<TextureObject> texObj) :ImageObject(width, height, texObj) {}
		~CreateImageObjcetHelper() { ImageObject::~ImageObject(); };
	};
	return std::make_shared<CreateImageObjcetHelper>(width,height,texObj);
}

ImageObject::~ImageObject()
{
}

DirectX::XMFLOAT2 ImageObject::GetImageSize()
{
	return DirectX::XMFLOAT2(static_cast<float>(mWidth), static_cast<float>(mHeight));
}

std::shared_ptr<ShaderResourceObject> ImageObject::GetShaderResource() const
{
	return mTexObj->GetShaderResource();
}

const std::string& ImageObject::GetTextureName() const
{
	return mTexObj->GetTextureName();
}

const std::string & ImageObject::GetFilePath() const
{
	return mTexObj->GetFilePath();
}

float ImageObject::GetGamma() const
{
	return mTexObj->GetGamma();
}
