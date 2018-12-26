#pragma once
#include <DirectXMath.h>
#include <string>
#include <vector>

struct FMDVertexEffectBone;
struct FMDVertex;
struct FMDMaterial;
struct FMDTexture;
struct FMDSkeleton;
using FMDVertices = std::vector<FMDVertex>;
using FMDVertexEffectBones = std::vector<FMDVertexEffectBone>;
using FMDIndices = std::vector<int>;
using FMDMaterials = std::vector<FMDMaterial>;
using FMDTextures = std::vector<FMDTexture>;
using FMDSkeletons = std::vector<FMDSkeleton>;
using FMDSkeletonNameTable = std::vector<std::string>;

namespace Fbx {
	struct FbxModelData;
	struct FbxVertex;
	struct FbxMaterial;
	struct FbxSkeleton;
};

struct FMDVertexEffectBone
{
	int boneIndex;
	float boneWeight;
};

struct FMDVertex
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 normal;
	DirectX::XMFLOAT2 texCoord;
	int effectBoneNum;
	FMDVertexEffectBones bones;
	FMDVertex operator=(const Fbx::FbxVertex& v);
};

struct FMDMaterial
{
	int effectIndexNum;//描画するインデクスの数
	DirectX::XMFLOAT4 diffuse;
	float diffuseFactor;
	DirectX::XMFLOAT4 ambient;
	float ambientFactor;
	DirectX::XMFLOAT4 specular;
	float specularFactor;
	float shininess;
	DirectX::XMFLOAT4 emissive;
	float emissiveFactor;
	DirectX::XMFLOAT4 transparentColor;
	float transparencyFactor;
	FMDMaterial operator=(const Fbx::FbxMaterial& mat);
};

struct FMDTexture
{
	enum class FMDTextureTable
	{
		diffuse,
		diffuseFactor,
		ambient,
		ambientFactor,
		specular,
		specularFactor,
		shininess,
		emissive,
		emissiveFactor,
		transparentColor,
		transparencyFactor,
		tableMax
	};
	int pathSizeTable[static_cast<int>(FMDTextureTable::tableMax)];//! 角文字列の長さ(バイト単位)のテーブル
	std::string diffuse;
	std::string diffuseFactor;
	std::string ambient;
	std::string ambientFactor;
	std::string specular;
	std::string specularFactor;
	std::string shininess;
	std::string emissive;
	std::string emissiveFactor;
	std::string transparentColor;
	std::string transparencyFactor;
	std::string GetAllData() const;
	FMDTexture operator=(const Fbx::FbxMaterial& mat);
};

struct FMDSkeleton
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 rotation;
	DirectX::XMFLOAT4 scale;
	int	parentIndex;
	int nameSize;//文字数
	std::string name;
	FMDSkeleton operator=(const Fbx::FbxSkeleton& skl);
};

struct FMDFileData
{
	int vertexNum;
	FMDVertices vertices;
	int indexNum;
	FMDIndices indices;
	int materialNum;
	FMDMaterials materials;
	FMDTextures textures;//マテリアルと同じ個数
	int skeletonNum;
	FMDSkeletons skeletons;

};