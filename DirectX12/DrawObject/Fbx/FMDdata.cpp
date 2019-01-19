#include "stdafx.h"
#include "FMDdata.h"
#include "Dx12MSLib.h"

namespace Fmd {

	FMDVertex FMDVertex::operator=(const Fbx::FbxVertex & v)
	{
		pos = v.pos;
		normal = v.normal;
		texCoord = v.texCoord;
		effectBoneNum = static_cast<int>(v.boneIndex.size());
		bones.resize(effectBoneNum);

		for (int i = 0; i < effectBoneNum; ++i)
		{
			bones[i].boneIndex = v.boneIndex[i];
			bones[i].boneWeight = v.boneWeight[i];
		}

		return *this;
	}

	FMDMaterial FMDMaterial::operator=(const Fbx::FbxMaterial & mat)
	{
		effectIndexNum = mat.effectIndexNum;
		diffuse = mat.diffuse.element;
		diffuseFactor = mat.diffuseFactor.element;
		ambient = mat.ambient.element;
		ambientFactor = mat.ambientFactor.element;
		specular = mat.specular.element;
		specularFactor = mat.specularFactor.element;
		shininess = mat.shininess.element;
		emissive = mat.emissive.element;
		emissiveFactor = mat.emissiveFactor.element;
		transparentColor = mat.transparentColor.element;
		transparencyFactor = mat.transparencyFactor.element;
		return *this;
	}

	std::string Fmd::FMDTexture::GetAllData() const
	{
		return std::string(diffuse + diffuseFactor
			+ ambient + ambientFactor
			+ specular + specularFactor + shininess
			+ emissive + emissiveFactor
			+ transparentColor + transparencyFactor);
	}

	FMDTexture FMDTexture::operator=(const Fbx::FbxMaterial & mat)
	{
		diffuse = mat.diffuse.textures.size() > 0 ? mat.diffuse.textures[0].texturePath : "";
		pathSizeTable[static_cast<int>(FMDTextureTable::diffuse)] = static_cast<int>(diffuse.size());
		diffuseFactor = mat.diffuse.textures.size() > 0 ? mat.diffuse.textures[0].texturePath : "";
		pathSizeTable[static_cast<int>(FMDTextureTable::diffuse)] = static_cast<int>(diffuse.size());
		ambient = mat.ambient.textures.size() > 0 ? mat.ambient.textures[0].texturePath : "";
		pathSizeTable[static_cast<int>(FMDTextureTable::ambient)] = static_cast<int>(ambient.size());
		ambientFactor = mat.ambientFactor.textures.size() > 0 ? mat.ambientFactor.textures[0].texturePath : "";
		pathSizeTable[static_cast<int>(FMDTextureTable::ambientFactor)] = static_cast<int>(ambientFactor.size());
		specular = mat.specular.textures.size() > 0 ? mat.specular.textures[0].texturePath : "";
		pathSizeTable[static_cast<int>(FMDTextureTable::specular)] = static_cast<int>(specular.size());
		specularFactor = mat.specularFactor.textures.size() > 0 ? mat.specularFactor.textures[0].texturePath : "";
		pathSizeTable[static_cast<int>(FMDTextureTable::specularFactor)] = static_cast<int>(specularFactor.size());
		shininess = mat.shininess.textures.size() > 0 ? mat.shininess.textures[0].texturePath : "";
		pathSizeTable[static_cast<int>(FMDTextureTable::shininess)] = static_cast<int>(shininess.size());
		emissive = mat.emissive.textures.size() > 0 ? mat.emissive.textures[0].texturePath : "";
		pathSizeTable[static_cast<int>(FMDTextureTable::emissive)] = static_cast<int>(emissive.size());
		emissiveFactor = mat.emissiveFactor.textures.size() > 0 ? mat.emissiveFactor.textures[0].texturePath : "";
		pathSizeTable[static_cast<int>(FMDTextureTable::emissiveFactor)] = static_cast<int>(emissiveFactor.size());
		transparentColor = mat.transparentColor.textures.size() > 0 ? mat.transparentColor.textures[0].texturePath : "";
		pathSizeTable[static_cast<int>(FMDTextureTable::transparentColor)] = static_cast<int>(transparentColor.size());
		transparencyFactor = mat.transparencyFactor.textures.size() > 0 ? mat.transparencyFactor.textures[0].texturePath : "";
		pathSizeTable[static_cast<int>(FMDTextureTable::transparencyFactor)] = static_cast<int>(transparencyFactor.size());
		return *this;
	}

	FMDSkeleton FMDSkeleton::operator=(const Fbx::FbxSkeleton & skl)
	{
		pos = skl.pos;
		rotation = skl.rotation;
		scale = skl.scale;
		name = skl.name;
		globalMatrix = skl.globalMatrix;
		localMatrix = skl.localMatrix;
		parentIndex = skl.parentIndex;
		dir = skl.dir;
		return FMDSkeleton();
	}

}