#pragma once
#include "DrawObject/DrawObjectLoader.h"
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
	DirectX::XMFLOAT4X4 globalOffsetPosition;
	unsigned int attributeType;
	DirectX::XMFLOAT3 translation;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	std::vector<NodeTree> children;
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

	void ReleaseModel(const std::string& modelPath);

	void ReleaseNonRefModel();

	void ReleaseAllModel();

	std::shared_ptr<FbxMotionData> LoadAnimation(const std::string& animationPath);

	void ReleaseAnimation(const std::string& animationPath);

	void ReleaseNonRefAnimation();

	void ReleaseAllAnimation();

	void SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate);

	void SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature);

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

	static FbxLoader* mInstance;
	std::vector<fbxsdk::FbxMesh*> mMeshDatas;
	std::vector<fbxsdk::FbxNode*> mNodeDatas;
	std::vector<Fbx::AnimationData> mAnimCurves;
	std::vector<Fbx::SkeletonAnimationData> mSkeletonMatrix;
	DirectX::XMFLOAT4X4 mGeometryOffset;

	std::vector<Fbx::TmpVertex> mTmpVertices;
	std::vector<int> mTmpIndexes;
	std::map<std::string,Fbx::TmpBone> mBones;
	std::vector<Fbx::FbxSkeleton> mSkeletons;
	std::vector<unsigned int> mSkeletonIndices;
	std::vector<Fbx::MaterialIndexSet> mMaterialSets;
	//�|���S���̏��ԂɑΉ��������̃|���S���ɑΉ�����}�e���A����ID�̏��
	std::vector<int> mMaterialIDPerPolygon;

	std::shared_ptr<LightObject> mLight;

	bool LoaderInitializie(std::string fbxPath);
	void CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	void CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev);

	std::shared_ptr<Fbx::FbxModelData> ConnectMeshes(std::vector<std::shared_ptr<Fbx::FbxModelData>>& datas);

	//���_�����p�֐�
	int AlignVertex(int vertexIndex, std::vector<Fbx::FbxVertex>& vertex);

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

	std::vector<fbxsdk::FbxTime> ExtractingKeyFrames(fbxsdk::FbxScene* scene, unsigned int meshId, std::vector<fbxsdk::FbxNode*>& linkNode);

	//const NodeTree* GetNodeTree(std::string nodename);

	//const NodeTree* GetNodeTreeRecursive(std::string nodeName, const NodeTree& nodeTree);
};

