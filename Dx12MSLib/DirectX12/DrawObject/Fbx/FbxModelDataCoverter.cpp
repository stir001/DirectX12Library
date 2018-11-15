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
	ConvertBone(dev);
	ConvertSkeletons(dev);
	return model;
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

void FbxModelDataConverter::ConvertBone(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mModel.lock()->mBones.resize(mConvertData.lock()->bones.size());
	
	for (unsigned int i = 0; i < static_cast<unsigned int>(mModel.lock()->mBones.size()); ++i)
	{
		mModel.lock()->mBones[i] = mConvertData.lock()->bones[i];
	}

	if (static_cast<unsigned int>(mModel.lock()->mBones.size()) == 0)
	{
		Fbx::FbxBone bone;
		DirectX::XMStoreFloat4x4(&bone.initMatrix, DirectX::XMMatrixIdentity());
		mModel.lock()->mBones.push_back(bone);
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
