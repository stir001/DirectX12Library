#include "stdafx.h"
#include "FbxModelDataCoverter.h"
#include "FbxModel.h"
#include "FbxModelController.h"
#include "Buffer/IndexBufferObject.h"
#include "Buffer/VertexBufferObject.h"
#include "Buffer/ConstantBufferObject.h"
#include "Master/Dx12Ctrl.h"
#include "Util/CharToWChar.h"
#include "Util/XMFloatOperators.h"
#include "Util/Util.h"
#include "Texture/TextureLoader.h"
#include "FMDdata.h"

using namespace Fbx;

FbxModelDataConverter::FbxModelDataConverter()
{
}

FbxModelDataConverter::~FbxModelDataConverter()
{
}

std::shared_ptr<FbxModel> FbxModelDataConverter::ConvertToFbxModel(std::shared_ptr<FbxModelData> data)
{
	std::shared_ptr<FbxModel> model = std::make_shared<FbxModel>();
	mModel = model;
	mModel.lock()->mModelPath = data->modelPath;
	mConvertData = data;
	mRelativePath = GetRelativePath(data->modelPath);

	size_t size = data->modelPath.rfind('/');

	for (unsigned int i = static_cast<unsigned int>(size) + 1U; i < static_cast<unsigned int>(data->modelPath.size()); ++i)
	{
		mModel.lock()->mModelName.push_back(data->modelPath[i]);
	}

	auto& dev = Dx12Ctrl::Instance().GetDev();

	ConvertIndex(dev);
	ConvertVertex(dev);
	ConvertTexture(dev);
	ConvertSkeletons(dev);
	return model;
}

std::shared_ptr<Fbx::FbxModel> FbxModelDataConverter::ConvertToFbxModel(Fmd::FMDFileData & data)
{
	auto fbxmodeldata = std::make_shared<Fbx::FbxModelData>();
	fbxmodeldata->modelPath = data.filepath;
	fbxmodeldata->indexes.indexCount = data.indexNum;
	fbxmodeldata->indexes.indexes.resize(data.indexNum);
	std::copy(data.indices.begin(), data.indices.end(), fbxmodeldata->indexes.indexes.begin());
	fbxmodeldata->vertexesInfo.vertexes.resize(data.vertexNum);
	std::copy(data.vertices.begin(), data.vertices.end(), fbxmodeldata->vertexesInfo.vertexes.begin());
	fbxmodeldata->materials.resize(data.materialNum);
	//auto relativePath = GetRelativePath(mFilePath);
	for (int i = 0; i < data.materialNum; ++i)
	{
		fbxmodeldata->materials[i].effectIndexNum = data.materials[i].effectIndexNum;
		fbxmodeldata->materials[i].diffuse.element = data.materials[i].diffuse;
		if (data.textures[i].diffuse.size() != 0)
		{
			fbxmodeldata->materials[i].diffuse.textures.resize(1);
			fbxmodeldata->materials[i].diffuse.textures[0].textureName = (data.textures[i].diffuse);
			fbxmodeldata->materials[i].diffuse.textures[0].texturePath = (data.textures[i].diffuse);
		}

		fbxmodeldata->materials[i].diffuseFactor.element = data.materials[i].diffuseFactor;
		if (data.textures[i].diffuseFactor.size() != 0)
		{
			fbxmodeldata->materials[i].diffuseFactor.textures.resize(1);
			fbxmodeldata->materials[i].diffuseFactor.textures[0].textureName = (data.textures[i].diffuseFactor);
			fbxmodeldata->materials[i].diffuseFactor.textures[0].texturePath = (data.textures[i].diffuseFactor);
		}

		fbxmodeldata->materials[i].ambient.element = data.materials[i].ambient;
		if (data.textures[i].ambient.size() != 0)
		{
			fbxmodeldata->materials[i].ambient.textures.resize(1);
			fbxmodeldata->materials[i].ambient.textures[0].textureName = (data.textures[i].ambient);
			fbxmodeldata->materials[i].ambient.textures[0].texturePath = (data.textures[i].ambient);
		}

		fbxmodeldata->materials[i].ambientFactor.element = data.materials[i].ambientFactor;
		if (data.textures[i].ambientFactor.size() != 0)
		{
			fbxmodeldata->materials[i].ambientFactor.textures.resize(1);
			fbxmodeldata->materials[i].ambientFactor.textures[0].textureName = (data.textures[i].ambientFactor);
			fbxmodeldata->materials[i].ambientFactor.textures[0].texturePath = (data.textures[i].ambientFactor);
		}

		fbxmodeldata->materials[i].specular.element = data.materials[i].specular;
		if (data.textures[i].specular.size() != 0)
		{
			fbxmodeldata->materials[i].specular.textures.resize(1);
			fbxmodeldata->materials[i].specular.textures[0].textureName = (data.textures[i].specular);
			fbxmodeldata->materials[i].specular.textures[0].texturePath = (data.textures[i].specular);
		}

		fbxmodeldata->materials[i].specularFactor.element = data.materials[i].specularFactor;
		if (data.textures[i].specularFactor.size() != 0)
		{
			fbxmodeldata->materials[i].specularFactor.textures.resize(1);
			fbxmodeldata->materials[i].specularFactor.textures[0].textureName = (data.textures[i].specularFactor);
			fbxmodeldata->materials[i].specularFactor.textures[0].texturePath = (data.textures[i].specularFactor);
		}

		fbxmodeldata->materials[i].shininess.element = data.materials[i].shininess;
		if (data.textures[i].shininess.size() != 0)
		{
			fbxmodeldata->materials[i].shininess.textures.resize(1);
			fbxmodeldata->materials[i].shininess.textures[0].textureName = (data.textures[i].shininess);
			fbxmodeldata->materials[i].shininess.textures[0].texturePath = (data.textures[i].shininess);
		}

		fbxmodeldata->materials[i].emissive.element = data.materials[i].emissive;
		if (data.textures[i].emissive.size() != 0)
		{
			fbxmodeldata->materials[i].emissive.textures.resize(1);
			fbxmodeldata->materials[i].emissive.textures[0].textureName = (data.textures[i].emissive);
			fbxmodeldata->materials[i].emissive.textures[0].texturePath = (data.textures[i].emissive);
		}

		fbxmodeldata->materials[i].emissiveFactor.element = data.materials[i].emissiveFactor;
		if (data.textures[i].emissiveFactor.size() != 0)
		{
			fbxmodeldata->materials[i].emissiveFactor.textures.resize(1);
			fbxmodeldata->materials[i].emissiveFactor.textures[0].textureName = (data.textures[i].emissiveFactor);
			fbxmodeldata->materials[i].emissiveFactor.textures[0].texturePath = (data.textures[i].emissiveFactor);
		}

		fbxmodeldata->materials[i].transparentColor.element = data.materials[i].transparentColor;
		if (data.textures[i].transparentColor.size() != 0)
		{
			fbxmodeldata->materials[i].transparentColor.textures.resize(1);
			fbxmodeldata->materials[i].transparentColor.textures[0].textureName = (data.textures[i].transparentColor);
			fbxmodeldata->materials[i].transparentColor.textures[0].texturePath = (data.textures[i].transparentColor);
		}

		fbxmodeldata->materials[i].transparencyFactor.element = data.materials[i].transparencyFactor;
		if (data.textures[i].transparencyFactor.size() != 0)
		{
			fbxmodeldata->materials[i].transparencyFactor.textures.resize(1);
			fbxmodeldata->materials[i].transparencyFactor.textures[0].textureName = (data.textures[i].transparencyFactor);
			fbxmodeldata->materials[i].transparencyFactor.textures[0].texturePath = (data.textures[i].transparencyFactor);
		}
	}
	fbxmodeldata->skeletons.resize(data.skeletonNum);
	fbxmodeldata->skeletonIndices.resize(data.skeletonNum * 2);
	for (int i = 0; i < data.skeletonNum; ++i)
	{
		fbxmodeldata->skeletons[i] = data.skeletons[i];
		fbxmodeldata->skeletonIndices[i * 2] = data.skeletons[i].parentIndex;
		fbxmodeldata->skeletonIndices[i * 2 + 1] = i;
	}
	unsigned int max = UINT_MAX;
	if (fbxmodeldata->skeletonIndices[0] == max)
	{
		fbxmodeldata->skeletonIndices.erase(fbxmodeldata->skeletonIndices.begin(), fbxmodeldata->skeletonIndices.begin() + 1);
	}
	fbxmodeldata->skeletonIndices.shrink_to_fit();
	return ConvertToFbxModel(fbxmodeldata);
}

void FbxModelDataConverter::ConvertIndex(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mModel.lock()->mIndexes = mConvertData.lock()->indexes.indexes;
	std::string name = mModel.lock()->GetModelName();
	name += "IndexBuffer";

	mModel.lock()->mIndexBuffer = std::make_shared<IndexBufferObject>(name, dev, static_cast<unsigned int>(sizeof(mModel.lock()->mIndexes[0])), static_cast<unsigned int>(mModel.lock()->mIndexes.size()));
	mModel.lock()->mIndexBuffer->WriteBuffer(&mModel.lock()->mIndexes[0], static_cast<unsigned int>(mModel.lock()->mIndexes.size() * sizeof(mModel.lock()->mIndexes[0])));
}

void FbxModelDataConverter::ConvertVertex(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mModel.lock()->mVertexes = mConvertData.lock()->vertexesInfo.vertexes;
}

void FbxModelDataConverter::ConvertTexture(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mModel.lock()->mMaterials.resize(mConvertData.lock()->materials.size());
	
	std::wstring wpath;
	wpath.resize(mRelativePath.size());
	for (unsigned int i = 0; i < static_cast<unsigned int>(mRelativePath.size()); ++i)
	{
		wpath[i] = mRelativePath[i];
	}

	auto& mats = mConvertData.lock()->materials;
	for (int i = 0; i < static_cast<int>(mConvertData.lock()->materials.size()); ++i)
	{
		for (int j = 0; j < Fbx::FbxMaterial::eELEMENT_TYPE::eELEMENT_TYPE_NUM; ++j)
		{
			auto& textures = mats[i].GetTexture(static_cast<Fbx::FbxMaterial::eELEMENT_TYPE>(j));
			if (textures.size() != 0)
			{
				for (auto tex : textures)
				{
					mModel.lock()->mMaterials[i].textures.push_back(TextureLoader::Instance().LoadTexture(mRelativePath + tex.texturePath));
				}
			}
			else
			{
				mModel.lock()->mMaterials[i].textures.push_back(mats[i].CreateTextureUseElement(static_cast<Fbx::FbxMaterial::eELEMENT_TYPE>(j)));
			}
			mModel.lock()->mMaterials[i].drawIndexNum = mConvertData.lock()->materials[i].effectIndexNum;
		}
	}
}

void FbxModelDataConverter::ConvertSkeletons(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mModel.lock()->mSkeleton = mConvertData.lock()->skeletons;
	mModel.lock()->mSkeletonIndices = mConvertData.lock()->skeletonIndices;
	unsigned int skeletonNum = static_cast<unsigned int>(mModel.lock()->mSkeleton.size());
	mModel.lock()->mSkeletonPos.resize(skeletonNum);
	for (unsigned int i = 0;i < skeletonNum; ++i)
	{
		mModel.lock()->mSkeletonPos[i] = mModel.lock()->mSkeleton[i].pos;
	}
}
