#pragma once
#include <vector>
#include <list>
#include <DirectXMath.h>
#include <string>
//#include <fbxsdk.h>
#include <memory>

class TextureObject;

namespace Fbx
{
	struct FbxIndexes {
		std::vector<unsigned int> indexes;
		int	indexCount;
	};

	struct UVSet {
		DirectX::XMFLOAT2 texCoord;
		std::string	uvSetName;
	};

	struct FbxVertexElement
	{
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 normal;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT4X4 vertexMatrix;
	};

	struct FbxVertex {
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 normal;
		DirectX::XMFLOAT2 texCoord;
		std::vector<int> boneIndex;
		std::vector<float> boneWeight;
		std::vector<std::string> boneName;
	};

	struct FbxVertexesInfo {
		std::vector<FbxVertex> vertexes;
	};

	struct FbxTexture
	{
		enum eBLEND_MODE{
			eBLEND_MODE_TRANSLUCENT,	
			eBLEND_MODE_ADDITIVE,		
			eBLEND_MODE_MODULATE,		
			eBLEND_MODE_MODULATE2,		
			eBLEND_MODE_OVER,			
			eBLEND_MODE_NORMAL,			
			eBLEND_MODE_DISSOLVE,		
			eBLEND_MODE_DARKEN,			
			eBLEND_MODE_COLOR_BURN,		
			eBLEND_MODE_LINEAR_BURN,	
			eBLEND_MODE_DRARLER_COLOR,	
			eBLEND_MODE_LIGHTEN,		
			eBLEND_MODE_SCREEN,			
			eBLEND_MODE_COLOR_DODGE,	
			eBLEND_MODE_LINEAR_DODGE,	
			eBLEND_MODE_LINEAR_COLOR,	
			eBLEND_MODE_SOFT_LIGHT,
			eBLEND_MODE_HARD_LIGHT,
			eBLEND_MODE_VIVID_LIGHT,
			eBLEND_MODE_PIN_LIGHT,
			eBLEND_MODE_HARD_MIX,
			eBLEND_MODE_DIFFERENCE,
			eBLEND_MODE_EXCLUSION,
			eBLEND_MODE_SUBTRACT,
			eBLEND_MODE_DIVIDE,
			eBLEND_MODE_HUE,
			eBLEND_MODE_SATURATION,
			eBLEND_MODE_COLOR,
			eBLEND_MODE_LUMINOSITY,
			eBLEND_MODE_OVERLAY,
			eBLEND_MODE_BLEND_MODE_COUNT,
			eBLEND_MODE_NON_TEXTURE,
		};
		std::string	textureName;
		std::string	texturePath;
		std::string	uvSetName;
		eBLEND_MODE blendMode;
		
		unsigned int textureCount;
	};

	typedef class std::vector<FbxTexture> FbxTexturesSet;

	template<typename T> struct MaterialElements
	{
		FbxTexturesSet textures;
		T element;
	};

	struct FbxMaterial
	{
		FbxMaterial();
		unsigned int effectIndexNum;

		MaterialElements<DirectX::XMFLOAT4> diffuse;
		MaterialElements<float> diffuseFactor;

		MaterialElements<DirectX::XMFLOAT4> ambient;
		MaterialElements<float> ambientFactor;

		MaterialElements<DirectX::XMFLOAT4> specular;
		MaterialElements<float> specularFactor;
		MaterialElements<float> shininess;	//specularのpower

		MaterialElements<DirectX::XMFLOAT4> emissive;
		MaterialElements<float> emissiveFactor;

		MaterialElements<DirectX::XMFLOAT4> transparentColor;
		MaterialElements<float> transparencyFactor;

		//MaterialElements<DirectX::XMFLOAT4> normalmap;

		enum eELEMENT_TYPE
		{
			eELEMENT_TYPE_DIFFUSE,
			eELEMENT_TYPE_DIFFUSE_FACTOR,
			eELEMENT_TYPE_AMBIENT,
			eELEMENT_TYPE_AMBIENT_FACTOR,
			eELEMENT_TYPE_SPECULAR,
			eELEMENT_TYPE_SPECULAR_FACTOR,
			eELEMENT_TYPE_SHININESS,
			eELEMENT_TYPE_EMISSIVE,
			eELEMENT_TYPE_EMISSIVE_FACTOR,
			eELEMENT_TYPE_TRANSPARENT_COLOR,
			eELEMENT_TYPE_TRANSPARENCY_FACTOR,
			//eELEMENT_TYPE_NORMALMAP,
			eELEMENT_TYPE_NUM,
		};

		FbxTexturesSet GetTexture(eELEMENT_TYPE type);

		std::shared_ptr<TextureObject> CreateTextureUseElement(eELEMENT_TYPE type);

		void SetTexture(eELEMENT_TYPE type, FbxTexturesSet& texSet);
	};

	struct FbxBoneInfo {
		DirectX::XMMATRIX initMatrix;
		int	index;
		std::string	boneName;
		std::vector<float>	affectedVertexWeight;
		std::vector<int> affectedVertexIndex;
	};

	struct FbxBone {
		std::string	boneName;
		int	index;
		DirectX::XMFLOAT4X4 initMatrix;
	};

	struct FbxSkeleton {
		std::string name;
		DirectX::XMFLOAT4 pos;
		DirectX::XMFLOAT4 rotation;
		DirectX::XMFLOAT4 scale;
	};

	struct FbxModelData
	{
		std::string modelPath;
		Fbx::FbxIndexes	indexes;
		Fbx::FbxVertexesInfo vertexesInfo;
		std::vector<Fbx::FbxMaterial> materials;
		std::vector<Fbx::FbxBone> bones;
		std::vector<Fbx::FbxSkeleton> skeletons;
		std::vector<unsigned int> skeletonIndices;
	};

	struct BoneFrameData
	{
		int frame;
		DirectX::XMMATRIX matrix;
	};

	struct BoneMatrixData
	{
		std::string boneName;
		unsigned int currentDataIndex;
		std::vector<BoneFrameData> frameData;
	};

	typedef std::vector<BoneMatrixData> AnimationDatas_t;

	//頂点整合用のデータ
	struct VertexResource {
		std::vector<DirectX::XMFLOAT3> pos;
		std::vector<DirectX::XMFLOAT3> normal;
		std::vector<DirectX::XMFLOAT2> texCoord;
		std::vector<int> indexes;
		std::vector<int> uvIndexList;
		std::vector<int> normalIndexList;
	};

	struct TmpWeight
	{
		float weight;
		int boneNo;
	};

	struct TmpUV
	{
		DirectX::XMFLOAT2 uv;
		int vertexNo;			//仮想的な頂点番号
		std::string uvSetName;
		bool operator ==(const TmpUV& rval)
		{
			return (
				this->uv.x == rval.uv.x &&
				this->uv.y == rval.uv.y);
		}
		TmpUV() : uv{ 0, 0 }, vertexNo(0){};
		TmpUV(const DirectX::XMFLOAT2& uv, const std::string uvSet)
			: uv(uv), uvSetName(uvSet), vertexNo(0){};
	};

	struct TmpNormal
	{
		DirectX::XMFLOAT3 normal;
		int vertexNo;			//仮想的な頂点番号
		bool operator ==(const TmpNormal& rval)
		{
			return (
				this->normal.x == rval.normal.x &&
				this->normal.y == rval.normal.y &&
				this->normal.z == rval.normal.z);
		}
		TmpNormal() : normal{ 0,0,0 }, vertexNo(0){};
		TmpNormal(const DirectX::XMFLOAT3& normal)
			: normal(normal), vertexNo(0) {};
	};

	struct TmpVertex
	{
		DirectX::XMFLOAT3 pos;
		std::vector<TmpNormal> normal;
		std::vector<TmpUV> uv;
		std::vector<TmpWeight> weights;
		std::vector<std::string> boneName;
		int refcount;
	};

	struct TmpBone
	{
		std::string skeletonName;
	};

	struct FbxTextureMaterial
	{
		unsigned int drawIndexNum;
		std::vector<std::shared_ptr<TextureObject>> textures;
	};

	struct MaterialIndexSet
	{
		unsigned int materialId;
		unsigned int polygonCount;
		std::list<unsigned int> polygonIds;
		MaterialIndexSet() :materialId(0), polygonCount(0) {};
		MaterialIndexSet(unsigned int matId, unsigned int polygonNum) : materialId(matId), polygonCount(polygonNum) {};
	};
}