#pragma once
#include "DirectX12/DrawObject/Model.h"
#include "FbxStructure.h"

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <DirectXMath.h>

class FbxModelDataConverter;

namespace Fbx {

	class FbxModel : public Model
	{
		friend FbxModelDataConverter;
	public:
		enum eROOT_PARAMATER_INDEX
		{
			eROOT_PARAMATER_INDEX_DIFFUSE,
			eROOT_PARAMATER_INDEX_DIFFUSE_FACTOR,
			eROOT_PARAMATER_INDEX_AMBIENT,
			eROOT_PARAMATER_INDEX_AMBIENT_FACTOR,
			eROOT_PARAMATER_INDEX_SPECULAR,
			eROOT_PARAMATER_INDEX_SPECULAR_FACTOR,
			eROOT_PARAMATER_INDEX_SHINESS,
			eROOT_PARAMATER_INDEX_EMISSIVE,
			eROOT_PARAMATER_INDEX_EMISSIVE_FACTOR,
			eROOT_PARAMATER_INDEX_TRANSPARENT_COLOR,
			eROOT_PARAMATER_INDEX_TRANSPARENCY_FACTOR,
			eROOT_PARAMATER_INDEX_CAMERA,
			eROOT_PARAMATER_INDEX_LIGHT,
			eROOT_PARAMATER_INDEX_MATRIX,
			eROOT_PARAMATER_INDEX_MAX,
		};

		FbxModel();
		~FbxModel();

		std::vector<std::shared_ptr<TextureObject>>& GetTextureObjects();

		std::vector<unsigned int> mIndexes;
		std::vector<FbxVertex> mVertexes;
		std::vector<Fbx::FbxBone> mBones;
		std::vector<Fbx::FbxSkeleton> mSkeleton;
		std::vector<DirectX::XMFLOAT4> mSkeletonPos;
		std::vector<unsigned int> mSkeletonIndices;
		std::vector<Fbx::FbxTextureMaterial> mMaterials;
	private:
	};
}

