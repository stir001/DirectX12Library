#include "stdafx.h"
#include "FbxModel.h"
#include "Texture/TextureObject.h"
#include "Master/Dx12Ctrl.h"
#include "Buffer/VertexBufferObject.h"
#include "Buffer/ConstantBufferObject.h"

using namespace Fbx;

FbxModel::FbxModel()
{
}

FbxModel::~FbxModel()
{
}

std::vector<std::shared_ptr<TextureObject>>& Fbx::FbxModel::GetTextureObjects()
{
	return mTextureObjects;
}
