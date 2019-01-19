#include "stdafx.h"
#include "FbxStructure.h"
#include "Texture/TextureLoader.h"
#include "FMDdata.h"


Fbx::FbxMaterial::FbxMaterial()
	: effectIndexNum(0)
{
	diffuse.element = { 0.8f,0.8f,0.8f, 1.0f };
	diffuseFactor.element = 1.0f;
	ambient.element = { 0.1f, 0.1f, 0.1f, 1.0f };
	ambientFactor.element = 1.0f;
	specular.element = { 0.1f, 0.1f, 0.1f, 1.0f };
	specularFactor.element = 1.0f;
	shininess.element = 3.0f;
	emissive.element = { 0.0f, 0.0f, 0.0f, 1.0f };
	emissiveFactor.element = 1.0f;
	transparentColor.element = { 1.0f, 1.0f, 1.0f, 1.0f };
	transparencyFactor.element = 0.0f;
}

Fbx::FbxTexturesSet Fbx::FbxMaterial::GetTexture(Fbx::FbxMaterial::eELEMENT_TYPE type)
{
	Fbx::FbxTexturesSet rtn;
	switch (type)
	{
	case Fbx::FbxMaterial::eELEMENT_TYPE_DIFFUSE:
		rtn = diffuse.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_DIFFUSE_FACTOR:
		rtn = diffuseFactor.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_AMBIENT:
		rtn = ambient.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_AMBIENT_FACTOR:
		rtn = ambientFactor.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_SPECULAR:
		rtn = specular.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_SPECULAR_FACTOR:
		rtn = specularFactor.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_SHININESS:
		rtn = shininess.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_EMISSIVE:
		rtn = emissive.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_EMISSIVE_FACTOR:
		rtn = emissiveFactor.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_TRANSPARENT_COLOR:
		rtn = transparentColor.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_TRANSPARENCY_FACTOR:
		rtn = transparencyFactor.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE::eELEMENT_TYPE_NUM:
		break;
	default:
		assert(false);
		break;
	}
	return rtn;
}

std::shared_ptr<TextureObject> Fbx::FbxMaterial::CreateTextureUseElement(eELEMENT_TYPE type)
{
	std::shared_ptr<TextureObject> rtn;
	switch (type)
	{
	case Fbx::FbxMaterial::eELEMENT_TYPE_DIFFUSE:
		rtn = TextureLoader::Instance().CreateSingleColorTexture(diffuse.element);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_DIFFUSE_FACTOR:
		rtn = TextureLoader::Instance().CreateSingleColorTexture(diffuseFactor.element);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_AMBIENT:
		rtn = TextureLoader::Instance().CreateSingleColorTexture(ambient.element);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_AMBIENT_FACTOR:
		rtn = TextureLoader::Instance().CreateSingleColorTexture(ambientFactor.element);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_SPECULAR:
		rtn = TextureLoader::Instance().CreateSingleColorTexture(specular.element);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_SPECULAR_FACTOR:
		rtn = TextureLoader::Instance().CreateSingleColorTexture(specularFactor.element);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_SHININESS:
		rtn = TextureLoader::Instance().CreateSingleColorTexture(shininess.element);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_EMISSIVE:
		rtn = TextureLoader::Instance().CreateSingleColorTexture(emissive.element);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_EMISSIVE_FACTOR:
		rtn = TextureLoader::Instance().CreateSingleColorTexture(emissiveFactor.element);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_TRANSPARENT_COLOR:
		rtn = TextureLoader::Instance().CreateSingleColorTexture(transparentColor.element);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_TRANSPARENCY_FACTOR:
		rtn = TextureLoader::Instance().CreateSingleColorTexture(transparencyFactor.element);
		break;
	//case Fbx::FbxMaterial::eELEMENT_TYPE_NORMALMAP:
	//	rtn = TextureLoader::Instance().CreateSingleColorTexture(normalmap.element);
	//	break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_NUM:
		break;
	default:
		assert(false);
		break;
	}
	return rtn;
}

void Fbx::FbxMaterial::SetTexture(Fbx::FbxMaterial::eELEMENT_TYPE type, Fbx::FbxTexturesSet& texSet)
{
	switch (type)
	{
	case Fbx::FbxMaterial::eELEMENT_TYPE_DIFFUSE:
		diffuse.textures = std::move(texSet);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_DIFFUSE_FACTOR:
		diffuseFactor.textures = std::move(texSet);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_AMBIENT:
		ambient.textures = std::move(texSet);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_AMBIENT_FACTOR:
		ambientFactor.textures = std::move(texSet);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_SPECULAR:
		specular.textures = std::move(texSet);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_SPECULAR_FACTOR:
		specularFactor.textures = std::move(texSet);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_SHININESS:
		shininess.textures = std::move(texSet);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_EMISSIVE:
		emissive.textures = std::move(texSet);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_EMISSIVE_FACTOR:
		emissiveFactor.textures = std::move(texSet);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_TRANSPARENT_COLOR:
		transparentColor.textures = std::move(texSet);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_TRANSPARENCY_FACTOR:
		transparencyFactor.textures = std::move(texSet);
		break;
	//case Fbx::FbxMaterial::eELEMENT_TYPE_NORMALMAP:
	//	normalmap.textures = std::move(texSet);
	case Fbx::FbxMaterial::eELEMENT_TYPE_NUM:
		break;
	default:
		assert(false);
		break;
	}
}

Fbx::FbxVertex Fbx::FbxVertex::operator=(const Fmd::FMDVertex& v)
{
	pos = v.pos;
	normal = v.normal;
	texCoord = v.texCoord;
	boneIndex.resize(v.effectBoneNum);
	boneWeight.resize(v.effectBoneNum);
	for (int i = 0; i < v.effectBoneNum; ++i)
	{
		boneIndex[i] = v.bones[i].boneIndex;
		boneWeight[i] = v.bones[i].boneWeight;
	}
	return *this;
}

Fbx::FbxSkeleton Fbx::FbxSkeleton::operator=(const Fmd::FMDSkeleton & skl)
{
	this->name = skl.name;
	this->parentIndex = skl.parentIndex;
	this->pos = skl.pos;
	this->rotation = skl.rotation;
	this->scale = skl.scale;
	return *this;
}
