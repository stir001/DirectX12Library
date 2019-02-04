#include "stdafx.h"
#include "FMDLoader.h"
#include "FMDdata.h"
#include <Windows.h>
#include <fstream>

using namespace Fmd;

FMDLoader::FMDLoader()
{
}


FMDLoader::~FMDLoader()
{
}

FMDFileData FMDLoader::LoadFMD(const std::string& fmdPath)
{
	std::ifstream fileStream(fmdPath, std::ios_base::in  | std::ios_base::binary);
	if (!fileStream)
	{
		MessageBoxA(nullptr, "ƒtƒ@ƒCƒ‹‚ªŒ©‚Â‚©‚è‚Ü‚¹‚ñ", "FMDLoader", MB_OK);
		return FMDFileData();
	}
	char header[4];
	fileStream.read(header, sizeof(char) * 4);
	FMDFileData fmdData;
	fmdData.filepath = fmdPath;
	LoadVertices(fileStream, fmdData);
	LoadIndices(fileStream, fmdData);
	LoadMaterials(fileStream, fmdData);
	LoadSkeletons(fileStream, fmdData);
	return fmdData;
}

void FMDLoader::LoadVertices(std::ifstream& stream, FMDFileData& modelData)
{
	stream.read(reinterpret_cast<char*>(&modelData.vertexNum), sizeof(modelData.vertexNum));
	modelData.vertices.resize(modelData.vertexNum);

	int dataSize = 0; 
	{
		FMDVertex v;
		dataSize = sizeof(v.pos) + sizeof(v.normal) + sizeof(v.texCoord) + sizeof(v.effectBoneNum);
	}
	for (auto& vert : modelData.vertices)
	{
		stream.read(reinterpret_cast<char*>(&vert), dataSize);
		vert.bones.resize(vert.effectBoneNum);
		stream.read(reinterpret_cast<char*>(vert.bones.data()), sizeof(FMDVertexEffectBone) * vert.effectBoneNum);
	}
}

void FMDLoader::LoadIndices(std::ifstream& stream, FMDFileData& modelData)
{
	stream.read(reinterpret_cast<char*>(&modelData.indexNum), sizeof(modelData.indexNum));
	modelData.indices.resize(modelData.indexNum);
	stream.read(reinterpret_cast<char*>(modelData.indices.data()), sizeof(int) * modelData.indexNum);
}

void FMDLoader::LoadMaterials(std::ifstream& stream, FMDFileData& modelData)
{
	stream.read(reinterpret_cast<char*>(&modelData.materialNum), sizeof(modelData.materialNum));
	modelData.materials.resize(modelData.materialNum);
	modelData.textures.resize(modelData.materialNum);
	
	stream.read(reinterpret_cast<char*>(modelData.materials.data()), sizeof(FMDMaterial) * modelData.materialNum);

	for (auto& tex : modelData.textures)
	{
		stream.read(reinterpret_cast<char*>(tex.pathSizeTable), sizeof(tex.pathSizeTable[0]) * static_cast<int>(FMDTexture::FMDTextureTable::tableMax));
		if (tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::diffuse)] > 0)
		{
			tex.diffuse.resize(tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::diffuse)]);
			stream.read(reinterpret_cast<char*>(&tex.diffuse[0]), sizeof(tex.diffuse[0]) * tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::diffuse)]);
		}
		if (tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::diffuseFactor)] > 0)
		{
			tex.diffuseFactor.resize(tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::diffuseFactor)]);
			stream.read(reinterpret_cast<char*>(&tex.diffuseFactor[0]), sizeof(tex.diffuseFactor[0]) * tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::diffuseFactor)]);
		}
		if (tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::ambient)] > 0)
		{
			tex.ambient.resize(tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::ambient)]);
			stream.read(reinterpret_cast<char*>(&tex.ambient[0]), sizeof(tex.ambient[0]) * tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::ambient)]);
		}
		if (tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::diffuse)] > 0)
		{
			tex.diffuse.resize(tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::diffuse)]);
			stream.read(reinterpret_cast<char*>(&tex.diffuse[0]), sizeof(tex.diffuse[0]) * tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::diffuse)]);
		}
		if (tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::specular)] > 0)
		{
			tex.specular.resize(tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::specular)]);
			stream.read(reinterpret_cast<char*>(&tex.specular[0]), sizeof(tex.specular[0]) * tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::specular)]);
		}
		if (tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::specularFactor)] > 0)
		{
			tex.specularFactor.resize(tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::specularFactor)]);
			stream.read(reinterpret_cast<char*>(&tex.specularFactor[0]), sizeof(tex.specularFactor[0]) * tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::specularFactor)]);
		}
		if (tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::shininess)] > 0)
		{
			tex.shininess.resize(tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::shininess)]);
			stream.read(reinterpret_cast<char*>(&tex.shininess[0]), sizeof(tex.shininess[0]) * tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::shininess)]);
		}
		if (tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::emissive)] > 0)
		{
			tex.emissive.resize(tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::emissive)]);
			stream.read(reinterpret_cast<char*>(&tex.emissive[0]), sizeof(tex.emissive[0]) * tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::emissive)]);
		}
		if (tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::emissiveFactor)] > 0)
		{
			tex.emissiveFactor.resize(tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::emissiveFactor)]);
			stream.read(reinterpret_cast<char*>(&tex.emissiveFactor[0]), sizeof(tex.emissiveFactor[0]) * tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::emissiveFactor)]);
		}
		if (tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::transparentColor)] > 0)
		{
			tex.transparentColor.resize(tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::transparentColor)]);
			stream.read(reinterpret_cast<char*>(&tex.transparentColor[0]), sizeof(tex.transparentColor[0]) * tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::transparentColor)]);
		}
		if (tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::transparencyFactor)] > 0)
		{
			tex.transparencyFactor.resize(tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::transparencyFactor)]);
			stream.read(reinterpret_cast<char*>(&tex.transparencyFactor[0]), sizeof(tex.transparencyFactor[0]) * tex.pathSizeTable[static_cast<int>(FMDTexture::FMDTextureTable::transparencyFactor)]);
		}
	}
}

void FMDLoader::LoadSkeletons(std::ifstream& stream, FMDFileData& modelData)
{
	stream.read(reinterpret_cast<char*>(&modelData.skeletonNum), sizeof(modelData.skeletonNum));
	modelData.skeletons.resize(modelData.skeletonNum);
	int dataSize = 0;
	{
		FMDSkeleton skl;
		dataSize = SKELETON_CONSTANT_DATA_SIZE;
	}
	for (auto& s : modelData.skeletons)
	{
		stream.read(reinterpret_cast<char*>(&s), dataSize);
		if (s.nameSize > 0)
		{
			s.name.resize(s.nameSize);
			stream.read(reinterpret_cast<char*>(&s.name[0]), sizeof(s.name[0]) * s.nameSize);
		}
	}
}

