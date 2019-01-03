#include "stdafx.h"
#include "Model.h"
#include "Buffer/IndexBufferObject.h"
#include "Buffer/VertexBufferObject.h"
#include "Texture/TextureLoader.h"
#include "Texture/TextureObject.h"
#include "CommandList/Dx12CommandList.h"


Model::Model():mTexturecount(0),mIndexBuffer(nullptr),mVertexBuffer(nullptr)
{
}

Model::~Model()
{
	mIndexBuffer.reset();
	mVertexBuffer.reset();
	mMaterialBuffer.reset();
	for (auto& texObj : mTextureObjects)
	{
		TextureLoader::Instance().Release(texObj->GetFilePath());
	}
	mTextureObjects.clear();
}

void Model::SetIndexBuffer(const std::shared_ptr<Dx12CommandList>& cmdList) const
{
	cmdList->IASetIndexBuffer(mIndexBuffer);
}

void Model::SetVertexBuffer(const std::shared_ptr<Dx12CommandList>& cmdList) const
{
	cmdList->IASetVertexBuffers({ &mVertexBuffer }, 1);
}

std::shared_ptr<IndexBufferObject> Model::GetIndexBuffer() const
{
	return mIndexBuffer;
}

std::shared_ptr<VertexBufferObject> Model::GetVertexBuffer() const
{
	return mVertexBuffer;
}

std::vector<std::shared_ptr<TextureObject>> Model::GetTextureObjects() const
{
	return mTextureObjects;
}

std::shared_ptr<ConstantBufferObject> Model::GetMaterialBuffer() const
{
	return mMaterialBuffer;
}

const std::string& Model::GetModelName() const
{
	return mModelName;
}

const std::string & Model::GetModelPath() const
{
	return mModelPath;
}
