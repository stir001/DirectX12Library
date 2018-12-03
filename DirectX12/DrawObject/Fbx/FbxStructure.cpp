#include "stdafx.h"
#include "FbxStructure.h"
#include "Texture/TextureLoader.h"


Fbx::FbxTexturesSet Fbx::FbxMaterial::GetTexture(Fbx::FbxMaterial::eELEMENT_TYPE type)
{
	Fbx::FbxTexturesSet rtn;
	switch (type)
	{
	case Fbx::FbxMaterial::eELEMENT_TYPE_DIFFUSE:
		return diffuse.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_DIFFUSE_FACTOR:
		return diffuseFactor.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_AMBIENT:
		return ambient.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_AMBIENT_FACTOR:
		return ambientFactor.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_SPECULAR:
		return specular.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_SPECULAR_FACTOR:
		return specularFactor.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_SHININESS:
		return shininess.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_EMISSIVE:
		return emissive.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_EMISSIVE_FACTOR:
		return emissiveFactor.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_TRANSPARENT_COLOR:
		return transparentColor.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_TRANSPARENCY_FACTOR:
		return transparencyFactor.textures;
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE::eELEMENT_TYPE_NUM:
		return rtn;
		break;
	default:
		return rtn;
		break;
	}
	assert(false);
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
	case Fbx::FbxMaterial::eELEMENT_TYPE_NORMALMAP:
		rtn = TextureLoader::Instance().CreateSingleColorTexture(normalmap.element);
		break;
	case Fbx::FbxMaterial::eELEMENT_TYPE_NUM:
		break;
	default:
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
	case Fbx::FbxMaterial::eELEMENT_TYPE_NORMALMAP:
		normalmap.textures = std::move(texSet);
	case Fbx::FbxMaterial::eELEMENT_TYPE_NUM:
		break;
	default:
		break;
	}
}