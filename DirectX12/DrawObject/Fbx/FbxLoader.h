#pragma once
#include "DirectX12/DrawObject/DrawObjectLoader.h"
#include "FbxStructure.h"
#include "FbxsdkHaveStruct.h"

#include <functional>
#include <string>
#include <vector>
#include <DirectXMath.h>
#include <map>
#include <memory>
#include <d3d12.h>

class FbxModelDataConverter;
class FbxModelController;
class FbxMotionData;
class FbxMotionConverter;
class LightObject;
class FADLoader;

namespace Fmd
{
	class FMDLoader;
}

namespace Fbx {
	struct FbxModelData;
	struct FbxIndexes;
	struct FbxVertex;
	struct VertexResource;
	struct FbxVertexesInfo;
	struct FbxMaterial;
	struct FbxTexture;
	struct FbxBoneInfo;
	struct AnimationData;
	struct AnimCurveData;
	struct AnimKeyData;
	class FbxModel;
	struct SkeletonAnimationData;
}

namespace fbxsdk
{
	class FbxAMatrix;
	class FbxNodeAttribute;
	class FbxMesh;
	class FbxNode;
	class FbxManager;
	class FbxScene;
	class FbxImporter;
	class FbxPose;
	class FbxTime;
}

struct NodeTree
{
	std::string nodeName;
	DirectX::XMFLOAT4X4 globalPosition;
	DirectX::XMFLOAT4X4 offsetMatrix;
	unsigned int attributeType;
	DirectX::XMFLOAT3 translation;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 tailPos;
	std::vector<NodeTree> children;
	NodeTree() : 
		globalPosition{ 1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f},
		offsetMatrix{ 1.0f, 0.0f, 0.0f, 0.0f,
					  0.0f, 1.0f, 0.0f, 0.0f,
					  0.0f, 0.0f, 1.0f, 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f},
		attributeType(0),
		translation{ 0.0f, 0.0f, 0.0f }, rotation{ 0.0f, 0.0f, 0.0f }, scale{ 1.0f, 1.0f, 1.0f }, tailPos{ 0.0f, 0.0f, 0.0f } {}
};


class FbxLoader : public DrawObjectLoader
{
public:
	virtual ~FbxLoader();

	static void Destroy() {
		if (mInstance != nullptr) {
			delete mInstance;
			mInstance = nullptr;
		}
	};
	static FbxLoader& Instance() {
		if (mInstance == nullptr)
		{
			mInstance = new FbxLoader();
		}
		return *mInstance;
	};
	std::shared_ptr<FbxModelController>	LoadMesh(const std::string& modelPath);
	std::shared_ptr<FbxModelController>	LoadFMD(const std::string& modelPath);

	void ReleaseModel(const std::string& modelPath);

	void ReleaseNonRefModel();

	void ReleaseAllModel();

	std::shared_ptr<FbxMotionData> LoadAnimation(const std::string& animationPath);

	void ReleaseAnimation(const std::string& animationPath);

	void ReleaseNonRefAnimation();

	void ReleaseAllAnimation();

	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	void SetGraphicsRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);

	std::shared_ptr<Fbx::FbxModelData> GetMeshData(const std::string& modelPath);
private:
	FbxLoader();
	FbxLoader(const FbxLoader& ref) {};
	FbxLoader(FbxLoader&& ref) {};
	FbxLoader& operator = (const FbxLoader&) {};
	FbxLoader& operator = (FbxLoader&&) {};

	fbxsdk::FbxManager* mManager;
	fbxsdk::FbxScene* mScene;
	fbxsdk::FbxImporter* mImporter;
	fbxsdk::FbxPose* mPose;

	std::map<std::string, std::shared_ptr<Fbx::FbxModel>> mModelDatas;
	std::map<std::string, std::shared_ptr<FbxMotionData>> mAnimationDatas;
	NodeTree mNodeTree;
	
	std::shared_ptr<FbxModelDataConverter> mModelConverter;
	std::shared_ptr<FbxMotionConverter> mMotionConverter;
	std::shared_ptr<Fmd::FMDLoader> mFmdLoader;
	std::shared_ptr<FADLoader> mFadLoader;

	static FbxLoader* mInstance;
	std::vector<fbxsdk::FbxMesh*> mMeshDatas;
	std::vector<fbxsdk::FbxNode*> mNodeDatas;
	std::vector<Fbx::AnimationData> mAnimCurves;
	std::vector<Fbx::SkeletonAnimationData> mSkeletonMatrix;
	DirectX::XMFLOAT4X4 mGeometryOffset;

	std::vector<Fbx::TmpVertex> mTmpVertices;
	int mNormalMappingMode;
	int mUVMappingMode;
	std::vector<int> mTmpIndexes;
	std::map<std::string,Fbx::TmpBone> mBones;
	std::vector<Fbx::FbxSkeleton> mSkeletons;
	std::vector<unsigned int> mSkeletonIndices;
	std::vector<Fbx::MaterialIndexSet> mMaterialSets;
	//ポリゴンの順番に対応したそのポリゴンに対応するマテリアルのIDの情報
	std::vector<int> mMaterialIDPerPolygon;

	std::shared_ptr<LightObject> mLight;

	bool LoaderInitializie(std::string fbxPath);
	void CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	void CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	std::shared_ptr<Fbx::FbxModelData> ConnectMeshes(std::vector<std::shared_ptr<Fbx::FbxModelData>>& datas);

	//頂点整合用関数
	int AlignVertexSameNormalUV(int vertexIndex, std::vector<Fbx::FbxVertex>& vertex);

	int AlignVertexDiffNormalUV(int vertexIndex, std::vector<Fbx::FbxVertex>& vertex);

	void FixVertexInfo(std::shared_ptr<Fbx::FbxModelData> model, fbxsdk::FbxMesh* mesh);

	void StackSearchNode(fbxsdk::FbxNode* parent, unsigned int searchtype, NodeTree& parenTree, std::function<void(fbxsdk::FbxNode*)> hitFunction);

	void StackNode(fbxsdk::FbxNode* pNode, unsigned int type, std::vector<fbxsdk::FbxNode*>& nodeArray);

	std::shared_ptr<Fbx::FbxModelData> MainLoad(fbxsdk::FbxMesh* mesh, std::string path);

	void LoadVertexPosition(fbxsdk::FbxMesh* mesh);

	void LoadVertexNormal(fbxsdk::FbxMesh* mesh);

	void LoadVertexUV(fbxsdk::FbxMesh* mesh);

	void LoadCluster(fbxsdk::FbxMesh* mesh);

	void ClearTmpInfo();

	void LoadAnimationMain(fbxsdk::FbxScene* scene, unsigned int meshId);

	void LoadCurve(Fbx::AnimCurveData& curveData);

	void DestroyScence(fbxsdk::FbxScene* scene);

	void DestroyNode(fbxsdk::FbxNode* node);

	void LoadMatarial(std::shared_ptr<Fbx::FbxModelData> model, fbxsdk::FbxMesh* mesh);

	void LoadSkeletons();

	std::vector<fbxsdk::FbxTime> ExtractingKeyFrames(fbxsdk::FbxScene* scene, unsigned int meshId);

	std::shared_ptr<FbxModelController> CreateController(std::shared_ptr<Fbx::FbxModel>& model);
};

