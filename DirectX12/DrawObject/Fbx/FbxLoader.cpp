#include "stdafx.h"
#include "FbxLoader.h"
#include "FbxModel.h"
#include "FbxModelController.h"
#include "FbxModelDataCoverter.h"
#include "FbxMotionConverter.h"
#include "FbxMotionData.h"
#include "Shader/ShaderCompiler.h"
#include "Master/Dx12Ctrl.h"
#include "Util/XMFloatOperators.h"
#include "Light/DirectionalLight.h"
#include "RenderingPass/Manager/RenderingPassManager.h"
#include "FbxsdkHaveStruct.h"
#include "PipelineState/FbxPipelineState.h"
#include "Rootsignature/FbxRootSignature.h"
#include "FMDLoader.h"
#include "FMDdata.h"

#include <fbxsdk.h>
#include <memory>
#include <cassert>
#include <algorithm>
#include <Windows.h>
#include <atlstr.h>

FbxLoader* FbxLoader::mInstance = nullptr;

void StoreFbxMatrixToXMMatrix(const fbxsdk::FbxAMatrix& fmat, DirectX::XMMATRIX& xmmat);

FbxLoader::FbxLoader() :mModelConverter(std::make_shared<FbxModelDataConverter>()), mMotionConverter(std::make_shared<FbxMotionConverter>()), mLight(std::make_shared<DirectionalLight>(1.0f, -1.0f, 1.0f))
{
	DX12CTRL_INSTANCE
		CreateRootsignature(d12.GetDev());
	CreatePipelineState(d12.GetDev());
	mCmdList = RenderingPassManager::Instance().GetRenderingPassCommandList(static_cast<unsigned int>(DefaultPass::Model));
}

void FbxLoader::CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mPipelinestate = std::make_shared<FbxPipelineState>(mRootsignature, dev);
}

void FbxLoader::CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	mRootsignature = std::make_shared<FbxRootSignature>(dev);
}

void FbxLoader::SetPipelineState(std::shared_ptr<PipelineStateObject>& Pipelinestate)
{
	mPipelinestate = Pipelinestate;
}

void FbxLoader::SetGraphicsRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature)
{
	mRootsignature = rootsignature;
}

FbxLoader::~FbxLoader()
{
	mModelConverter = nullptr;
	mMotionConverter = nullptr;
	mModelDatas.clear();
	mModelDatas.clear();
}

bool FbxLoader::LoaderInitializie(std::string fbxPath)
{
	//SDKマネージャの作成

	mManager = fbxsdk::FbxManager::Create();
	//IOセッティングの設定

	fbxsdk::FbxIOSettings* ioSettings = fbxsdk::FbxIOSettings::Create(mManager, IOSROOT);
	mManager->SetIOSettings(ioSettings);

	fbxsdk::FbxString lPath = fbxsdk::FbxGetApplicationDirectory();
	mManager->LoadPluginsDirectory(lPath.Buffer());

	//シーンの作成
	mScene = fbxsdk::FbxScene::Create(mManager, "Scene");

	//インポーターの作成
	mImporter = fbxsdk::FbxImporter::Create(mManager, "importer");
	int fileFormat = -1;
	if (!mManager->GetIOPluginRegistry()->DetectReaderFileFormat(fbxPath.data(), fileFormat))
	{
		fileFormat = mManager->GetIOPluginRegistry()->FindReaderIDByDescription("Fbx binary (*.fbx)");
	}

	//インポーター初期化
	if (!mImporter->Initialize(fbxPath.c_str(), fileFormat)) {
		ATL::CString str = fbxPath.c_str();
		MessageBox(nullptr, L"Load Error ファイルパスが不正です : " + str, L"FBXModelLoader", MB_OK);
		return false;
	}

	//ロードしたファイルをシーンにインポート
	mImporter->Import(mScene);

	fbxsdk::FbxGeometryConverter geometryConverter(mManager);
	//bool isSpilted = geometryConverter.SplitMeshesPerMaterial(mScene, true);
	geometryConverter.Triangulate(mScene, true);

	fbxsdk::FbxAxisSystem sceneAxisSystem = mScene->GetGlobalSettings().GetAxisSystem();

	fbxsdk::FbxAxisSystem directXAxisSys(fbxsdk::FbxAxisSystem::eDirectX);

	fbxsdk::FbxAxisSystem system = directXAxisSys;
	if (sceneAxisSystem != system)
	{
		system.ConvertScene(mScene);
	}

	return true;
}

std::shared_ptr<FbxModelController> FbxLoader::LoadMesh(const std::string& modelPath)
{
	std::shared_ptr<FbxModelController> rtn(nullptr);
	auto itr = mModelDatas.find(modelPath);
	if (itr != mModelDatas.end())
	{
		rtn = CreateController(itr->second);
		return rtn;
	}

	auto modeldata = GetMeshData(modelPath);

	std::shared_ptr<Fbx::FbxModel> model(mModelConverter->ConvertToFbxModel(modeldata));
	mModelDatas[modelPath] = model;
	rtn = CreateController(model);

	return rtn;
}

std::shared_ptr<FbxModelController> FbxLoader::LoadFMD(const std::string & modelPath)
{
	auto itr = mModelDatas.find(modelPath);
	if (itr != mModelDatas.end())
	{
		auto rtn = CreateController(itr->second);
		return rtn;
	}
	auto data = mFmdLoader->LoadFMD(modelPath);
	auto model = mModelConverter->ConvertToFbxModel(data);
	auto rtn = CreateController(model);
	mModelDatas[modelPath] = model;
	return rtn;
}

std::shared_ptr<FbxMotionData> FbxLoader::LoadAnimation(const std::string& animationPath)
{

	if (!LoaderInitializie(animationPath))
	{
		return nullptr;
	}

	fbxsdk::FbxNode* rootNode = mScene->GetRootNode();
	if (rootNode) {
		fbxsdk::FbxAMatrix dummy;
		dummy.SetIdentity();
		NodeTree rNode = {};
		rNode.nodeName = rootNode->GetName();
		fbxsdk::FbxTime t = 0;
		mNodeTree = rNode;
		StackSearchNode(rootNode, fbxsdk::FbxNodeAttribute::EType::eMesh, mNodeTree, [&](fbxsdk::FbxNode* node) {
			mNodeDatas.push_back(node);
			mMeshDatas.push_back(node->GetMesh());
		});
	}
	else {
		MessageBox(nullptr, L"Load Error <rootNode is null>", L"FBXModelLoader", MB_OK);
		return nullptr;
	}

	fbxsdk::FbxArray<fbxsdk::FbxString*> mAnimStacknameArray;

	mScene->FillAnimStackNameArray(mAnimStacknameArray);

	LoadAnimationMain(mScene, 0);

	fbxsdk::FbxTakeInfo* pTakeInfo = mImporter->GetTakeInfo(0);
	fbxsdk::FbxLongLong start = pTakeInfo->mLocalTimeSpan.GetStart().Get();
	fbxsdk::FbxLongLong stop = pTakeInfo->mLocalTimeSpan.GetStop().Get();

	fbxsdk::FbxLongLong max = (stop - start) / fbxsdk::FbxTime::GetOneFrameValue(fbxsdk::FbxTime::eFrames60);

	std::shared_ptr<FbxMotionData> motion(mMotionConverter->ConvertMotionData(mSkeletonMatrix, max, animationPath));

	mAnimationDatas[animationPath] = motion;

	DestroyScence(mScene);
	mManager->Destroy();
	ClearTmpInfo();

	return motion;
}

void FbxLoader::ReleaseModel(const std::string& modelPath)
{
	if (mModelDatas.find(modelPath) != mModelDatas.end())
	{
		mModelDatas.erase(modelPath);
	}
}

void FbxLoader::ReleaseNonRefModel()
{
	std::vector<std::string> keyValue;
	for (auto& model : mModelDatas)
	{
		if (model.second.use_count() == 1)
		{
			keyValue.push_back(model.first);
		}
	}

	for (auto& key : keyValue)
	{
		mModelDatas.erase(key);
	}
}

void FbxLoader::ReleaseAllModel()
{
	mModelDatas.clear();
}


void FbxLoader::ReleaseAnimation(const std::string& animationPath)
{
	if (mAnimationDatas.find(animationPath) != mAnimationDatas.end())
	{
		mAnimationDatas.erase(animationPath);
	}
}

void FbxLoader::ReleaseNonRefAnimation()
{
	std::vector<std::string> keyValue;
	for (auto& anim : mAnimationDatas)
	{
		if (anim.second.use_count() == 1)
		{
			keyValue.push_back(anim.first);
		}
	}

	for (auto& key : keyValue)
	{
		mAnimationDatas.erase(key);
	}
}

void FbxLoader::ReleaseAllAnimation()
{
	mAnimationDatas.clear();
}

void StoreFbxMatrixToXMMatrix(const fbxsdk::FbxAMatrix& fmat, DirectX::XMMATRIX& xmmat)
{
	xmmat.r[0].m128_f32[0] = static_cast<float>(fmat.Get(0, 0)); xmmat.r[0].m128_f32[1] = static_cast<float>(fmat.Get(0, 1)); xmmat.r[0].m128_f32[2] = static_cast<float>(fmat.Get(0, 2)); xmmat.r[0].m128_f32[3] = static_cast<float>(fmat.Get(0, 3));
	xmmat.r[1].m128_f32[0] = static_cast<float>(fmat.Get(1, 0)); xmmat.r[1].m128_f32[1] = static_cast<float>(fmat.Get(1, 1)); xmmat.r[1].m128_f32[2] = static_cast<float>(fmat.Get(1, 2)); xmmat.r[1].m128_f32[3] = static_cast<float>(fmat.Get(1, 3));
	xmmat.r[2].m128_f32[0] = static_cast<float>(fmat.Get(2, 0)); xmmat.r[2].m128_f32[1] = static_cast<float>(fmat.Get(2, 1)); xmmat.r[2].m128_f32[2] = static_cast<float>(fmat.Get(2, 2)); xmmat.r[2].m128_f32[3] = static_cast<float>(fmat.Get(2, 3));
	xmmat.r[3].m128_f32[0] = static_cast<float>(fmat.Get(3, 0)); xmmat.r[3].m128_f32[1] = static_cast<float>(fmat.Get(3, 1)); xmmat.r[3].m128_f32[2] = static_cast<float>(fmat.Get(3, 2)); xmmat.r[3].m128_f32[3] = static_cast<float>(fmat.Get(3, 3));
}

void FbxLoader::StackNode(fbxsdk::FbxNode* pNode, unsigned int type, std::vector<fbxsdk::FbxNode*>& nodeArray)
{
	if (pNode)
	{
		if (pNode->GetNodeAttribute())
		{
			if (pNode->GetNodeAttribute()->GetAttributeType() == type)
			{
				nodeArray.push_back(pNode);
			}
			const int count = pNode->GetChildCount();
			for (int i = 0; i < count; ++i)
			{
				StackNode(pNode->GetChild(i), type, nodeArray);
			}
		}
	}
}

void FbxLoader::LoadVertexPosition(fbxsdk::FbxMesh* mesh)
{
	//START vertex position load
	int ctlPointcount = mesh->GetControlPointsCount();
	mTmpVertices.clear();
	mTmpVertices.shrink_to_fit();
	mTmpVertices.resize(ctlPointcount);

	fbxsdk::FbxVector4* vertexBuffer = mesh->GetControlPoints();
	for (int i = 0; i < ctlPointcount; i++)
	{
		DirectX::XMFLOAT4 pos = {};
		pos.x = static_cast<float>(vertexBuffer[i][0]);
		pos.y = static_cast<float>(vertexBuffer[i][1]);
		pos.z = static_cast<float>(vertexBuffer[i][2]);
		pos.w = 1.0f;
		pos *= mGeometryOffset;
		mTmpVertices[i].pos = { pos.x , pos.y, pos.z };
	}
	//END vertex position load 
}

void FbxLoader::LoadVertexNormal(fbxsdk::FbxMesh* mesh)
{
	int* polygonvertices = mesh->GetPolygonVertices();
	int polygonverticesCount = mesh->GetPolygonVertexCount();

	std::vector<int> normalArrayIndices;
	std::vector<int> vertexIndices;
	normalArrayIndices.resize(polygonverticesCount);
	vertexIndices.resize(polygonverticesCount);
	fbxsdk::FbxGeometryElementNormal* normalElement = nullptr;
	fbxsdk::FbxGeometryElement::EReferenceMode referenceMode;
	fbxsdk::FbxGeometryElement::EMappingMode mappingMode;
	fbxsdk::FbxVector4 t_normal;
	Fbx::TmpNormal nl;

	//START vertex normal load
	int normallayercount = mesh->GetElementNormalCount();
	if (!(normallayercount <= 1))
	{
		MessageBox(nullptr, L"Load Error 法線の複数レイヤーには対応していません", L"FBXModelLoader", MB_OK);
	}
	for (int layer = 0; layer < normallayercount; layer++)
	{
		//note: normalLayerが1より多い場合には未対応
		normalElement = mesh->GetElementNormal(layer);
		referenceMode = normalElement->GetReferenceMode();
		fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector4>* normalArray = &normalElement->GetDirectArray();
		mNormalMappingMode = mappingMode = normalElement->GetMappingMode();


		fbxsdk::FbxLayerElementArrayTemplate<int>* normalArrayIndicesArray = &normalElement->GetIndexArray();//normalArrayに対応するIndexを格納している配列

		int eIndexToDirectCount = 0;

		switch (mappingMode)
		{
		case fbxsdk::FbxLayerElement::eNone://マッピングが未定　対応できないふざけんな
			break;
		case fbxsdk::FbxLayerElement::eByControlPoint://コントロールポイントと1対1対応
			switch (referenceMode)
			{
			case fbxsdk::FbxLayerElement::eDirect://eByControllPointの場合は頂点の順番に定義すればいいはず
				for (int i = 0; i < static_cast<int>(mTmpVertices.size()); ++i)
				{
					mTmpVertices[i].normal.resize(1);

					t_normal = normalArray->GetAt(i);
					mTmpVertices[i].normal[0].normal.x = static_cast<float>(t_normal[0]);
					mTmpVertices[i].normal[0].normal.y = static_cast<float>(t_normal[1]);
					mTmpVertices[i].normal[0].normal.z = static_cast<float>(t_normal[2]);
					mTmpVertices[i].normal[0].vertexNo = i;
				}
				break;
			case fbxsdk::FbxLayerElement::eIndex:
			case fbxsdk::FbxLayerElement::eIndexToDirect://おそらくこれは来ない？
				assert(false);
				eIndexToDirectCount = normalArrayIndicesArray->GetCount();
				for (int i = 0; i < eIndexToDirectCount; ++i)
				{
					t_normal = normalArray->GetAt(normalArrayIndicesArray->GetAt(i));
					nl.normal.x = static_cast<float>(t_normal[0]);
					nl.normal.y = static_cast<float>(t_normal[1]);
					nl.normal.z = static_cast<float>(t_normal[2]);
					nl.vertexNo = i;
					mTmpVertices[i].normal.push_back(nl);
				}
				break;
			default:
				break;
			}
			break;
		case fbxsdk::FbxLayerElement::eByPolygonVertex://ポリゴンごとの頂点情報と1対t1対応　インデックス配列分用意されている？
			switch (referenceMode)
			{
			case fbxsdk::FbxLayerElement::eDirect://eByPolygonVertexの場合はPolygonVertexの順番で頂点を定義すればいいはず
				for (int i = 0; i < polygonverticesCount; ++i)
				{

					t_normal = normalArray->GetAt(i);
					nl.normal.x = static_cast<float>(t_normal[0]);
					nl.normal.y = static_cast<float>(t_normal[1]);
					nl.normal.z = static_cast<float>(t_normal[2]);
					nl.vertexNo = polygonvertices[i];
					mTmpVertices[polygonvertices[i]].normal.push_back(nl);
				}
				break;
			case fbxsdk::FbxLayerElement::eIndex:
			case fbxsdk::FbxLayerElement::eIndexToDirect:
				for (int i = 0; i < polygonverticesCount; ++i)
				{
					normalArrayIndices[i] = normalArrayIndicesArray->GetAt(i);
					vertexIndices[i] = polygonvertices[i];

					t_normal = normalArray->GetAt(normalArrayIndicesArray->GetAt(i));
					nl.normal.x = static_cast<float>(t_normal[0]);
					nl.normal.y = static_cast<float>(t_normal[1]);
					nl.normal.z = static_cast<float>(t_normal[2]);
					nl.vertexNo = polygonvertices[i];
					mTmpVertices[polygonvertices[i]].normal.push_back(nl);
				}
				break;
			default:
				break;
			}
			break;
		case fbxsdk::FbxLayerElement::eByPolygon://ポリゴンごとに共通
			assert(false);
			break;
		case fbxsdk::FbxLayerElement::eByEdge://よくわからん
			assert(false);
			break;
		case fbxsdk::FbxLayerElement::eAllSame://全部一緒
			assert(false);
			break;
		default:
			break;
		}
	}
	//END vertex normal load
	if (normalElement != nullptr)
	{
		normalElement = nullptr;
	}
}

//normalのrefaerncemode、mappingmodeと異なる場合には未対応
void FbxLoader::LoadVertexUV(fbxsdk::FbxMesh* mesh)
{
	int* polygonvertices = mesh->GetPolygonVertices();
	int polygonverticesCount = mesh->GetPolygonVertexCount();

	std::vector<int> uvlArrayIndices;
	std::vector<int> vertexIndices;
	uvlArrayIndices.resize(polygonverticesCount);
	vertexIndices.resize(polygonverticesCount);

	//START vertex uv load

	fbxsdk::FbxStringList t_UVSetNameList;
	mesh->GetUVSetNames(t_UVSetNameList);
	int uvSetCount = t_UVSetNameList.GetCount();
	if (uvSetCount >= 2)
	{
		MessageBox(nullptr, L"Load Error uvSetが複数あるものには対応してません", L"FBXModelLoader", MB_OK);
	}

	for (int uvSetIndex = 0; uvSetIndex < 1; ++uvSetIndex)
	{
		const char* t_uvSetName = t_UVSetNameList.GetStringAt(uvSetIndex);

		const fbxsdk::FbxGeometryElementUV* t_uvElement = mesh->GetElementUV(t_uvSetName);

		if (t_uvElement == nullptr) continue;


		fbxsdk::FbxLayerElementArrayTemplate<fbxsdk::FbxVector2>* uvArray = &t_uvElement->GetDirectArray();
		fbxsdk::FbxLayerElementArrayTemplate<int>* uvArrayIndicesArray = &t_uvElement->GetIndexArray();
		int uvArrayIndicesCount = uvArrayIndicesArray->GetCount();

		fbxsdk::FbxGeometryElement::EReferenceMode uvReferenceMode = t_uvElement->GetReferenceMode();
		fbxsdk::FbxGeometryElement::EMappingMode uvMappingMode = t_uvElement->GetMappingMode();

		mUVMappingMode = uvMappingMode;

		DirectX::XMFLOAT2 t_uv;

		switch (uvMappingMode)
		{
		case fbxsdk::FbxLayerElement::eNone:
			break;
		case fbxsdk::FbxLayerElement::eByControlPoint:
			switch (uvReferenceMode)
			{
			case fbxsdk::FbxLayerElement::eDirect:
				for (int i = 0; i < static_cast<int>(mTmpVertices.size()); ++i)
				{
					t_uv.x = static_cast<float>(uvArray->GetAt(i)[0]);
					t_uv.y = 1.0f - static_cast<float>(uvArray->GetAt(i)[0]);
					mTmpVertices[i].uv.emplace_back(Fbx::TmpUV(t_uv, t_uvSetName, i));
				}
				break;
			case fbxsdk::FbxLayerElement::eIndex:
			case fbxsdk::FbxLayerElement::eIndexToDirect://多分これはあり得ない
				assert(false);
				for (int i = 0; i < uvArrayIndicesCount; ++i)
				{
					t_uv.x = static_cast<float>(uvArray->GetAt(uvArrayIndicesArray->GetAt(i))[0]);
					t_uv.y = 1.0f - static_cast<float>(uvArray->GetAt(uvArrayIndicesArray->GetAt(i))[1]);
					mTmpVertices[i].uv.emplace_back(Fbx::TmpUV(t_uv, t_uvSetName, i));
				}
				break;
			default:
				break;
			}
			break;
		case fbxsdk::FbxLayerElement::eByPolygonVertex:
			switch (uvReferenceMode)
			{
			case fbxsdk::FbxLayerElement::eDirect:
				for (int i = 0; i < polygonverticesCount; ++i)
				{
					t_uv.x = static_cast<float>(uvArray->GetAt(i)[0]);
					t_uv.y = 1.0f - static_cast<float>(uvArray->GetAt(i)[1]);
					mTmpVertices[polygonvertices[i]].uv.emplace_back(Fbx::TmpUV(t_uv, t_uvSetName, polygonvertices[i]));
				}
				break;
			case fbxsdk::FbxLayerElement::eIndex:
			case fbxsdk::FbxLayerElement::eIndexToDirect:
				for (int i = 0; i < uvArrayIndicesCount; ++i)
				{
					uvlArrayIndices[i] = uvArrayIndicesArray->GetAt(i);
					vertexIndices[i] = polygonvertices[i];

					t_uv.x = static_cast<float>(uvArray->GetAt(uvArrayIndicesArray->GetAt(i))[0]);
					t_uv.y = 1.0f - static_cast<float>(uvArray->GetAt(uvArrayIndicesArray->GetAt(i))[1]);
					mTmpVertices[polygonvertices[i]].uv.emplace_back(Fbx::TmpUV(t_uv, t_uvSetName, polygonvertices[i]));
				}
				break;
			default:
				break;
			}
			break;
		case fbxsdk::FbxLayerElement::eByPolygon:
			assert(false);
			break;
		case fbxsdk::FbxLayerElement::eByEdge:
			assert(false);
			break;
		case fbxsdk::FbxLayerElement::eAllSame:
			assert(false);
			break;
		default:
			break;
		}
	}

	//END vertex uv load
}

void StoreSkeleton(Fbx::FbxSkeleton& skl, const NodeTree& tree) {
	skl.name = tree.nodeName;
	skl.pos = ConvertXMFloat3ToXMFloat4(tree.translation);
	skl.rotation = ConvertXMFloat3ToXMFloat4(tree.rotation);
	skl.scale = ConvertXMFloat3ToXMFloat4(tree.scale);
	skl.initMatrix = tree.globalPosition;
};

void CreateskeletonData(const NodeTree& skeletonTree,
	std::vector<unsigned int>& skeletonIndices,
	std::vector<Fbx::FbxSkeleton>& skeletons,
	unsigned int& skeletonIndex, unsigned int parentIndex)
{
	Fbx::FbxSkeleton skl;
	skl.parentIndex = parentIndex;

	//子ボーンの処理とインデックスの作成
	for (unsigned int i = 0; i < static_cast<unsigned int>(skeletonTree.children.size()); ++i)
	{
		StoreSkeleton(skl, skeletonTree.children[i]);
		unsigned int myidx = skeletonIndex++;
		skeletons[myidx] = skl;
		//インデックスの作成
		if (skeletonTree.nodeName != "" && skeletonTree.nodeName != "root")
		{
			skeletonIndices.push_back(skl.parentIndex);
			skeletonIndices.push_back(myidx);
		}

		CreateskeletonData(skeletonTree.children[i], skeletonIndices, skeletons, skeletonIndex, myidx);
	}
}

void FbxLoader::LoadCluster(fbxsdk::FbxMesh* mesh)
{
	//START vertex bone weight load
	int t_skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	int t_clusterCount = 0;

	fbxsdk::FbxCluster* t_cluster;
	int offset = 0;
	Fbx::TmpBone t_bone;
	fbxsdk::FbxAMatrix t_fbxMatrix;

	for (int i = 0; i < t_skinCount; ++i)
	{
		t_clusterCount = static_cast<fbxsdk::FbxSkin*> (mesh->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
		fbxsdk::FbxSkin::EType skintype = static_cast<fbxsdk::FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin))->GetSkinningType();
		for (int j = 0; j < t_clusterCount; ++j, ++offset)
		{
			t_cluster = static_cast<fbxsdk::FbxSkin*> (mesh->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);

			fbxsdk::FbxCluster::ELinkMode mode = t_cluster->GetLinkMode();

			int ctrlPointIndicesCount = t_cluster->GetControlPointIndicesCount();
			int* ctrlPointIndicesArray = t_cluster->GetControlPointIndices();
			double* ctrlPointWeightArray = t_cluster->GetControlPointWeights();

			Fbx::TmpWeight t_weight;

			t_bone.skeletonName = t_cluster->GetLink()->GetName();

			auto itr = mBones.find(t_bone.skeletonName);
			if (itr != mBones.end())
			{
				t_bone = itr->second;
			}

			std::vector<int> t_vertexIndicesArray;
			t_vertexIndicesArray.reserve(ctrlPointIndicesCount);
			for (int k = 0; k < ctrlPointIndicesCount; ++k)
			{
				t_weight.weight = static_cast<float>(ctrlPointWeightArray[k]);
				t_weight.boneNo = offset;
				mTmpVertices[ctrlPointIndicesArray[k]].weights.push_back(t_weight);
				mTmpVertices[ctrlPointIndicesArray[k]].boneName.push_back(t_bone.skeletonName);
				t_vertexIndicesArray.push_back(ctrlPointIndicesArray[k]);
			}

			mBones[t_bone.skeletonName] = (t_bone);
		}
	}
	//EMD vertex bone weight load
}

//@param out mv: モデル頂点データ 
//@param tv: mvに入れる頂点データ
//@paran tmpvertexId: tvのnormalとuvのどのデータを使うかを示すインデックス
void StoreTmpVertexToModelVertex(Fbx::FbxVertex& mv, const Fbx::TmpVertex& tv, int tmpvertexId)
{
	mv.pos = { tv.pos.x,tv.pos.y,tv.pos.z,1 };
	mv.normal = { tv.normal[tmpvertexId].normal.x,tv.normal[tmpvertexId].normal.y,tv.normal[tmpvertexId].normal.z,1 };
	mv.texCoord = tv.uv[tmpvertexId].uv;
	mv.boneIndex.resize(tv.weights.size());
	mv.boneWeight.resize(tv.weights.size());
	mv.boneName = tv.boneName;
	for (int i = 0; i < static_cast<int>(tv.weights.size()); ++i)
	{
		mv.boneIndex[i] = tv.weights[i].boneNo;
		mv.boneWeight[i] = tv.weights[i].weight;
	}
}

//@param out mv: モデル頂点データ 
//@param tv: mvに入れる頂点データ
//@paran tmpvertexNormalId: tvのnormalのどのデータを使うかを示すインデックス
//@paran tmpvertexUVId: tvのuvのどのデータを使うかを示すインデックス
void StoreTmpVertexToModelVertex(Fbx::FbxVertex& mv, const Fbx::TmpVertex& tv, int tmpvertexNormalId, int tmpvertexUVId)
{
	mv.pos = { tv.pos.x,tv.pos.y,tv.pos.z,1 };
	mv.normal = { tv.normal[tmpvertexNormalId].normal.x, tv.normal[tmpvertexNormalId].normal.y, tv.normal[tmpvertexNormalId].normal.z, 1 };
	mv.texCoord = tv.uv[tmpvertexUVId].uv;
	mv.boneIndex.resize(tv.weights.size());
	mv.boneWeight.resize(tv.weights.size());
	mv.boneName = tv.boneName;
	for (int i = 0; i < static_cast<int>(tv.weights.size()); ++i)
	{
		mv.boneIndex[i] = tv.weights[i].boneNo;
		mv.boneWeight[i] = tv.weights[i].weight;
	}
}

//頂点整合用関数
//@return 整合用頂点番号
int FbxLoader::AlignVertexSameNormalUV(int vertexIndex, std::vector<Fbx::FbxVertex>& vertices)
{
	if (vertexIndex == 61)
	{
		bool irregular = true;
	}
	Fbx::TmpVertex& t_vertex = mTmpVertices[vertexIndex];
	int refcount = t_vertex.refcount++;
	if (refcount == 0)
	{
		StoreTmpVertexToModelVertex(vertices[vertexIndex], t_vertex, refcount);
		return vertexIndex;
	}

	for (int i = 0; i < refcount; ++i)
	{
		if (t_vertex.normal[i] == t_vertex.normal[refcount]
			&& t_vertex.uv[i] == t_vertex.uv[refcount])
		{
			int vertexNo = t_vertex.normal[i].vertexNo;
			t_vertex.normal[refcount].vertexNo = vertexNo;
			t_vertex.uv[refcount].vertexNo = vertexNo;
			StoreTmpVertexToModelVertex(vertices[vertexNo], t_vertex, refcount);
			return vertexNo;
		}
	}

	int newVertexNo = static_cast<int>(mTmpVertices.size());
	Fbx::TmpVertex v;
	v.pos = t_vertex.pos;
	v.normal.push_back(t_vertex.normal[refcount]);
	v.uv.push_back(t_vertex.uv[refcount]);
	v.refcount = 1;
	v.weights = t_vertex.weights;
	v.normal[0].vertexNo = newVertexNo;
	v.normal[0].vertexNo = newVertexNo;
	v.boneName = t_vertex.boneName;

	t_vertex.normal[refcount].vertexNo = newVertexNo;
	t_vertex.uv[refcount].vertexNo = newVertexNo;
	mTmpVertices.push_back(v);

	StoreTmpVertexToModelVertex(vertices[newVertexNo], v, 0);
	return newVertexNo;
}

int FbxLoader::AlignVertexDiffNormalUV(int vertexIndex, std::vector<Fbx::FbxVertex>& vertices)
{
	Fbx::TmpVertex& t_vertex = mTmpVertices[vertexIndex];
	int refcount = t_vertex.refcount++;
	if (refcount == 0)
	{
		StoreTmpVertexToModelVertex(vertices[vertexIndex], t_vertex, refcount);
		return vertexIndex;
	}

	if (mNormalMappingMode == fbxsdk::FbxLayerElement::eByControlPoint)
	{
		for (int i = 0; i < refcount; ++i)
		{
			if (t_vertex.uv[i] == t_vertex.uv[refcount])
			{
				int vertexNo = t_vertex.uv[i].vertexNo;
				t_vertex.normal[0].vertexNo = vertexNo;
				t_vertex.uv[refcount].vertexNo = vertexNo;
				StoreTmpVertexToModelVertex(vertices[vertexNo], t_vertex, 0, refcount);
				return vertexNo;
			}
		}

		int newVertexNo = static_cast<int>(mTmpVertices.size());
		Fbx::TmpVertex v;
		v.pos = t_vertex.pos;
		v.normal.push_back(t_vertex.normal[0]);
		v.uv.push_back(t_vertex.uv[refcount]);
		v.refcount = 1;
		v.weights = t_vertex.weights;
		v.normal.back().vertexNo = newVertexNo;
		v.uv.back().vertexNo = newVertexNo;
		v.boneName = t_vertex.boneName;

		t_vertex.normal[0].vertexNo = newVertexNo;
		t_vertex.uv[refcount].vertexNo = newVertexNo;
		mTmpVertices.push_back(v);

		StoreTmpVertexToModelVertex(vertices[newVertexNo], v, 0);

		return newVertexNo;
	}
	else if (mUVMappingMode == fbxsdk::FbxLayerElement::eByControlPoint)
	{
		for (int i = 0; i < refcount; ++i)
		{
			if (t_vertex.normal[i] == t_vertex.normal[refcount])
			{
				int vertexNo = t_vertex.normal[i].vertexNo;
				t_vertex.normal[refcount].vertexNo = vertexNo;
				t_vertex.uv[0].vertexNo = vertexNo;
				StoreTmpVertexToModelVertex(vertices[vertexNo], t_vertex, refcount, 0);
				return vertexNo;
			}
		}

		int newVertexNo = static_cast<int>(mTmpVertices.size());
		Fbx::TmpVertex v;
		v.pos = t_vertex.pos;
		v.normal.push_back(t_vertex.normal[refcount]);
		v.uv.push_back(t_vertex.uv[0]);
		v.refcount = 1;
		v.weights = t_vertex.weights;
		v.normal.back().vertexNo = newVertexNo;
		v.uv.back().vertexNo = newVertexNo;
		v.boneName = t_vertex.boneName;

		t_vertex.normal[refcount].vertexNo = newVertexNo;
		t_vertex.uv[0].vertexNo = newVertexNo;
		mTmpVertices.push_back(v);

		StoreTmpVertexToModelVertex(vertices[newVertexNo], v, 0);
		return newVertexNo;
	}

	assert(false);
	return -1;
}

void FbxLoader::FixVertexInfo(std::shared_ptr<Fbx::FbxModelData> model, fbxsdk::FbxMesh* mesh)
{
	//START store vertex data

	int* indexBuffer = mesh->GetPolygonVertices();

	int polygoncount = mesh->GetPolygonCount();
	int polygonsize = 0;
	int error = 0;

	int polygonvertexcount = mesh->GetPolygonVertexCount();
	model->vertexesInfo.vertexes.resize(polygoncount * 6);

	std::vector<int> t_idx(polygonvertexcount);
	for (int i = 0; i < polygonvertexcount; ++i)
	{
		t_idx[i] = indexBuffer[i];
	}

	int indexplus1 = 0;
	int indexplus2 = 0;

	int vNum = 0;

	auto& mats = model->materials;
	std::vector<std::vector<unsigned int>> indicesPerMaterial(mMaterialSets.size());

	std::function<int(int, std::vector<Fbx::FbxVertex>&)> VertexAlignFunc;
	if (mNormalMappingMode == mUVMappingMode)
	{
		VertexAlignFunc = [&](int vertexIndex, std::vector<Fbx::FbxVertex>& vertices)
		{
			return AlignVertexSameNormalUV(vertexIndex, vertices);
		};
	}
	else
	{
		VertexAlignFunc = [&](int vertexIndex, std::vector<Fbx::FbxVertex>& vertices)
		{
			return AlignVertexDiffNormalUV(vertexIndex, vertices);
		};
	}

	for (auto& idxMat : indicesPerMaterial)
	{
		idxMat.reserve(polygoncount * 6);
	}
	auto matIDPerPolyItr = mMaterialIDPerPolygon.begin();
	for (unsigned int pi = 0, pvi = 0, materialId = 0, polygonCount = 0; static_cast<int>(pi) < polygoncount; ++pi) {
		if (mMaterialSets[materialId].polygonCount <= polygonCount)
		{
			++materialId;
			polygonCount = 0;
		}
		polygonsize = mesh->GetPolygonSize(pi);
		switch (polygonsize)
		{
		case 3:
			for (int i = 0; i < polygonsize; ++i)
			{
				indicesPerMaterial[*matIDPerPolyItr].push_back(VertexAlignFunc(indexBuffer[pvi++], model->vertexesInfo.vertexes));
			}

			mats[materialId].effectIndexNum += 3;
			break;
		case 4:
			indicesPerMaterial[*matIDPerPolyItr].push_back(indexplus1 = VertexAlignFunc(indexBuffer[pvi], model->vertexesInfo.vertexes));

			indicesPerMaterial[*matIDPerPolyItr].push_back(AlignVertexSameNormalUV(indexBuffer[pvi + 1], model->vertexesInfo.vertexes));

			indicesPerMaterial[*matIDPerPolyItr].push_back(indexplus2 = VertexAlignFunc(indexBuffer[pvi + 2], model->vertexesInfo.vertexes));


			indicesPerMaterial[*matIDPerPolyItr].push_back(indexplus1);

			indicesPerMaterial[*matIDPerPolyItr].push_back(indexplus2);

			indicesPerMaterial[*matIDPerPolyItr].push_back(VertexAlignFunc(indexBuffer[pvi + 3], model->vertexesInfo.vertexes));

			mats[materialId].effectIndexNum += 6;

			pvi += 4;
			break;
		default:
			++error;
			break;
		}
		++polygonCount;
		++matIDPerPolyItr;
	}

	auto& modelIndices = model->indexes.indexes;
	size_t itrOffset = 0;
	for (auto& idxPerMatItr : indicesPerMaterial)
	{
		itrOffset = idxPerMatItr.size();
		modelIndices.resize(modelIndices.size() + itrOffset);
		std::copy(idxPerMatItr.begin(), idxPerMatItr.end(), modelIndices.end() - itrOffset);
	}

	if (error > 0)
	{
		MessageBox(NULL, L"エラポリあるんやけど", L"POLYGON ERROR", MB_OK);

	}

	std::vector<unsigned int>(model->indexes.indexes).swap(model->indexes.indexes);
	model->indexes.indexCount = static_cast<int>(model->indexes.indexes.size());

	auto beginitr = model->vertexesInfo.vertexes.begin();
	auto enditr = model->vertexesInfo.vertexes.end();
	int vindex = static_cast<int>(mTmpVertices.size());

	model->vertexesInfo.vertexes.erase(beginitr + vindex, enditr);
	model->vertexesInfo.vertexes.shrink_to_fit();
}

void FbxLoader::StackSearchNode(fbxsdk::FbxNode* parent, unsigned int searchtype, NodeTree& parentTree, std::function<void(fbxsdk::FbxNode*)> hitFunction)
{
	int childNum = parent->GetChildCount();

	for (int i = 0, skipCount = 0; i < childNum; ++i)
	{
		fbxsdk::FbxNode* childNode = parent->GetChild(i);
		fbxsdk::FbxNodeAttribute* nodeAttribute = childNode->GetNodeAttribute();
		if (nodeAttribute == nullptr)
		{
			++skipCount;
			StackSearchNode(childNode, searchtype, parentTree, hitFunction);
			continue;
		}

		NodeTree childNodeTree;
		childNodeTree.nodeName = childNode->GetName();

		childNodeTree.attributeType = nodeAttribute->GetAttributeType();
		if (childNodeTree.attributeType == searchtype)
		{
			//http://help.autodesk.com/view/FBX/2019/ENU/?guid=FBX_Developer_Help_nodes_and_scene_graph_fbx_nodes_transformation_data_html
			auto t_translation = childNode->LclTranslation.Get();
			/*auto globalMatrix = childNode->EvaluateLocalTransform().Inverse();
			t_translation = globalMatrix.MultT(fbxsdk::FbxVector4(t_translation.mData[0], t_translation.mData[1], t_translation.mData[2]));*/
			childNodeTree.translation = { static_cast<float>(t_translation.mData[0]), static_cast<float>(t_translation.mData[1]) , static_cast<float>(t_translation.mData[2]) };
			//childNodeTree.translation += parentTree.translation;
			auto t_rotation = childNode->LclRotation.Get();
			childNodeTree.rotation = { static_cast<float>(t_rotation.mData[0]), static_cast<float>(t_rotation.mData[1]), static_cast<float>(t_rotation.mData[2]) };
			//childNodeTree.rotation += parentTree.rotation;
			auto t_scale = childNode->LclScaling.Get();
			childNodeTree.scale = { static_cast<float>(t_scale.mData[0]), static_cast<float>(t_scale.mData[1]), static_cast<float>(t_scale.mData[2]) };

			fbxsdk::FbxAMatrix mat(t_translation, t_rotation, t_scale);
			DirectX::XMMATRIX xmat;
			StoreFbxMatrixToXMMatrix(mat, xmat);
			DirectX::XMFLOAT4X4 mxf44;
			DirectX::XMStoreFloat4x4(&mxf44, xmat);
			//DirectX::XMFLOAT4 parentpos = { parentTree.translation.x, parentTree.translation.y, parentTree.translation.z, 1.0f };
			DirectX::XMFLOAT4 origin = { 0,0,0,1 };
			childNodeTree.globalPosition = mxf44 * parentTree.globalPosition;
			DirectX::XMFLOAT4 childpos = origin * childNodeTree.globalPosition;
			childNodeTree.translation = { childpos.x, childpos.y, childpos.z };

			parentTree.children.push_back(childNodeTree);
			hitFunction(childNode);
			StackSearchNode(childNode, searchtype, parentTree.children[i - skipCount], hitFunction);
		}
		else
		{
			++skipCount;
			StackSearchNode(childNode, searchtype, parentTree, hitFunction);
		}
	}
}

std::shared_ptr<Fbx::FbxModelData> FbxLoader::MainLoad(fbxsdk::FbxMesh* mesh, std::string path)
{
	std::shared_ptr<Fbx::FbxModelData> model = std::make_shared<Fbx::FbxModelData>();

	LoadVertexPosition(mesh);

	LoadVertexNormal(mesh);

	LoadVertexUV(mesh);

	LoadCluster(mesh);

	LoadMatarial(model, mesh);

	FixVertexInfo(model, mesh);

	return model;
}

Fbx::FbxVertex CalculatoFbxVertexByFbxAMatrix(Fbx::FbxVertex v, fbxsdk::FbxAMatrix fmat)
{
	DirectX::XMMATRIX dmat;
	DirectX::XMMATRIX noMoveMat;
	Fbx::FbxVertex t_v;
	t_v = v;

	StoreFbxMatrixToXMMatrix(fmat, dmat);

	noMoveMat.r[3].m128_f32[0] = 0;
	noMoveMat.r[3].m128_f32[1] = 0;
	noMoveMat.r[3].m128_f32[2] = 0;

	DirectX::XMVECTOR t_pos;
	t_pos = DirectX::XMLoadFloat4(&v.pos);
	t_pos = DirectX::XMVector4Transform(t_pos, dmat);
	DirectX::XMStoreFloat4(&t_v.pos, t_pos);

	DirectX::XMVECTOR t_normal;
	t_normal = DirectX::XMLoadFloat4(&v.normal);
	t_normal = DirectX::XMVector4Transform(t_normal, dmat);
	DirectX::XMStoreFloat4(&t_v.normal, t_normal);

	return t_v;

}

/**
*	@param[in,out] resultDataこのデータの後ろに追加のデータを結合する
*	@param[in] connectData このデータをresultDataの後ろに結合する
*/
template <typename T>
void ConnectSTLVectorData(std::vector<T>& resultData, const std::vector<T>& connectData)
{
	unsigned int endNum = static_cast<unsigned int>(resultData.size());
	unsigned int addNum = static_cast<unsigned int>(connectData.size());
	resultData.resize(endNum + addNum);
	std::copy(connectData.begin(), connectData.end(), resultData.begin() + endNum);
}

/**
*	@param[in,out] resultIndices このデータの後ろに追加のデータを結合する
*	@param[in] connectIndices このデータをresultIndicesの後ろに結合する
*	@param[in] offset インデックスのオフセット量
*/
template <typename T>
void ConnectSTLVectorIndices(std::vector<T>& resultIndices, const std::vector<T>& connectIndices, const unsigned int offsetNum)
{
	unsigned int endNum = static_cast<unsigned int>(resultIndices.size());
	unsigned int addNum = static_cast<unsigned int>(connectIndices.size());
	resultIndices.reserve(endNum + addNum);
	for (auto& idx : connectIndices)
	{
		resultIndices.push_back(idx + offsetNum);
	}
}

std::shared_ptr<Fbx::FbxModelData> FbxLoader::ConnectMeshes(std::vector<std::shared_ptr<Fbx::FbxModelData>>& datas)
{
	std::shared_ptr<Fbx::FbxModelData> rtn = std::make_shared<Fbx::FbxModelData>();
	rtn->modelPath = datas[0]->modelPath;
	int endCount = 0;
	int addCount = 0;
	for (int i = 0; i < static_cast<int>(datas.size()); ++i)
	{
		endCount = static_cast<int>(rtn->indexes.indexes.size());
		addCount = static_cast<int>(datas[i]->indexes.indexes.size());
		rtn->indexes.indexCount = endCount + addCount;
		ConnectSTLVectorIndices(rtn->indexes.indexes, datas[i]->indexes.indexes, static_cast<unsigned int>(rtn->vertexesInfo.vertexes.size()));

		ConnectSTLVectorData(rtn->vertexesInfo.vertexes, datas[i]->vertexesInfo.vertexes);

		ConnectSTLVectorData(rtn->materials, datas[i]->materials);
	}

	rtn->skeletons = std::move(mSkeletons);
	rtn->skeletonIndices = std::move(mSkeletonIndices);

	return rtn;
}

void FbxLoader::ClearTmpInfo()
{
	mMeshDatas.clear();
	mMeshDatas.shrink_to_fit();
	mNodeDatas.clear();
	mNodeDatas.shrink_to_fit();
	mTmpVertices.clear();
	mTmpVertices.shrink_to_fit();
	mTmpIndexes.clear();
	mTmpIndexes.shrink_to_fit();
	mBones.clear();
	mSkeletons.clear();
	mSkeletons.shrink_to_fit();
	mSkeletonIndices.clear();
	mSkeletonIndices.shrink_to_fit();
	mAnimCurves.clear();
	mAnimCurves.shrink_to_fit();
	mSkeletonMatrix.clear();
	mSkeletonMatrix.shrink_to_fit();
	DirectX::XMFLOAT4X4 identity;
	identity = ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixIdentity());
	mNodeTree.globalPosition = identity;
	mNodeTree.nodeName.clear();
	mNodeTree.nodeName.shrink_to_fit();
	mNodeTree.children.clear();
	mNodeTree.children.shrink_to_fit();
	mMaterialSets.clear();
	mMaterialSets.shrink_to_fit();
}

void FbxLoader::LoadAnimationMain(fbxsdk::FbxScene* scene, unsigned int meshId)
{
	std::vector<fbxsdk::FbxTime> times = ExtractingKeyFrames(scene, meshId);

	NodeTree nodeTree;
	std::vector<FbxNode*> skeletonNode;
	StackSearchNode(scene->GetRootNode(), fbxsdk::FbxNodeAttribute::eSkeleton, nodeTree
		, [&skeletonNode](fbxsdk::FbxNode* node) {
		skeletonNode.push_back(node);
	});

	fbxsdk::FbxLongLong oneFrameValue = fbxsdk::FbxTime::GetOneFrameValue(fbxsdk::FbxTime::eFrames60);
	mSkeletonMatrix.resize(skeletonNode.size());
	for (int i = 0; i < static_cast<int>(skeletonNode.size()); ++i)
	{
		mSkeletonMatrix[i].animMatrix.resize(times.size());
		mSkeletonMatrix[i].skeletonName = skeletonNode[i]->GetName();
		for (int j = 0; j < static_cast<int>(times.size()); ++j)
		{
			DirectX::XMMATRIX t_mat;
			StoreFbxMatrixToXMMatrix(skeletonNode[i]->EvaluateLocalTransform(times[j]), t_mat);
			mSkeletonMatrix[i].animMatrix[j].matrix = t_mat;
			mSkeletonMatrix[i].animMatrix[j].frame = static_cast<int>(times[j].Get() / oneFrameValue);
		}
	}
}

void StackAnimationTime(const std::vector<Fbx::AnimKeyData>& data, std::vector<fbxsdk::FbxTime>& stack)
{
	for (int i = 0; i < static_cast<int>(data.size()); ++i)
	{
		auto itr = std::find_if(stack.begin(), stack.end(), [&data, i](const fbxsdk::FbxTime& t) {return t.Get() == (data[i].time); });
		if (itr == stack.end())
		{
			stack.push_back(data[i].time);
		}
	}
}

std::vector<fbxsdk::FbxTime> FbxLoader::ExtractingKeyFrames(fbxsdk::FbxScene* scene, unsigned int meshId)
{
	int animStack = scene->GetSrcObjectCount<fbxsdk::FbxAnimStack>();

	std::vector<fbxsdk::FbxCluster*> clusters;
	std::vector<fbxsdk::FbxNode*> linkNode;

	for (int stack = 0; stack < animStack; ++stack)
	{
		fbxsdk::FbxAnimStack* pAnimStack = scene->GetMember<fbxsdk::FbxAnimStack>(stack);

		int numAnimLayers = pAnimStack->GetMemberCount<fbxsdk::FbxAnimLayer>();
		for (int layer = 0; layer < numAnimLayers; ++layer)
		{
			fbxsdk::FbxAnimLayer* pAnimLayer = pAnimStack->GetMember<fbxsdk::FbxAnimLayer>(layer);

			unsigned int defomerCount = mMeshDatas[meshId]->GetDeformerCount(FbxDeformer::eSkin);
			for (unsigned int deformer = 0; deformer < defomerCount; ++deformer)
			{
				mAnimCurves.clear();
				fbxsdk::FbxSkin* pDeformer = (fbxsdk::FbxSkin*)mMeshDatas[meshId]->GetDeformer(deformer, FbxDeformer::eSkin);
				fbxsdk::FbxSkin::EType skintype = pDeformer->GetSkinningType();
				unsigned int clusterCount = pDeformer->GetClusterCount();
				mAnimCurves.reserve(clusterCount);
				linkNode.reserve(clusterCount);
				Fbx::AnimationData animData;

				for (unsigned int cluster = 0; cluster < clusterCount; ++cluster)
				{
					fbxsdk::FbxCluster* pCluster = pDeformer->GetCluster(cluster);
					clusters.push_back(pCluster);
					linkNode.push_back(pCluster->GetLink());

					animData.skeletonName = linkNode[cluster]->GetName();

					//transform
					animData.translationX.curve = linkNode[cluster]->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					LoadCurve(animData.translationX);


					animData.translationY.curve = linkNode[cluster]->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					LoadCurve(animData.translationY);


					animData.translationZ.curve = linkNode[cluster]->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					LoadCurve(animData.translationZ);


					//rotation
					animData.rotationX.curve = linkNode[cluster]->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					LoadCurve(animData.rotationX);


					animData.rotationY.curve = linkNode[cluster]->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					LoadCurve(animData.rotationY);


					animData.rotationZ.curve = linkNode[cluster]->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					LoadCurve(animData.rotationZ);


					//scale
					animData.scaleX.curve = linkNode[cluster]->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
					LoadCurve(animData.scaleX);

					animData.scaleY.curve = linkNode[cluster]->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
					LoadCurve(animData.scaleY);

					animData.scaleZ.curve = linkNode[cluster]->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
					LoadCurve(animData.scaleZ);

					mAnimCurves.push_back(animData);
				}
			}
		}
	}

	std::vector<fbxsdk::FbxTime> times;

	int iCounter = 0;

	unsigned int size = static_cast<unsigned int>(mAnimCurves.size());

	for (unsigned int i = 0; i < size; ++i)
	{
		StackAnimationTime(mAnimCurves[i].translationX.keys, times);
		StackAnimationTime(mAnimCurves[i].translationY.keys, times);
		StackAnimationTime(mAnimCurves[i].translationZ.keys, times);

		StackAnimationTime(mAnimCurves[i].rotationX.keys, times);
		StackAnimationTime(mAnimCurves[i].rotationY.keys, times);
		StackAnimationTime(mAnimCurves[i].rotationZ.keys, times);

		StackAnimationTime(mAnimCurves[i].scaleX.keys, times);
		StackAnimationTime(mAnimCurves[i].scaleY.keys, times);
		StackAnimationTime(mAnimCurves[i].scaleZ.keys, times);
	}

	std::sort(times.begin(), times.end(), [](fbxsdk::FbxTime lval, fbxsdk::FbxTime rval) {return lval.Get() < rval.Get(); });

	return times;
}

std::shared_ptr<FbxModelController> FbxLoader::CreateController(std::shared_ptr<Fbx::FbxModel>& model)
{
	auto rtn = std::make_shared<FbxModelController>(model, Dx12Ctrl::Instance().GetDev(), mCmdList, mPipelinestate, mRootsignature);
	rtn->SetLight(mLight);
	return rtn;
}

std::shared_ptr<Fbx::FbxModelData> FbxLoader::GetMeshData(const std::string & modelPath)
{
	if (!LoaderInitializie(modelPath))
	{
		return nullptr;
	}

	//ルートノードのゲット
	fbxsdk::FbxNode* rootNode = mScene->GetRootNode();

	if (rootNode) {
		fbxsdk::FbxAMatrix dummy;
		dummy.SetIdentity();
		NodeTree rNode;
		rNode.nodeName = rootNode->GetName();
		fbxsdk::FbxTime t = 0;
		mNodeTree = rNode;
		StackSearchNode(rootNode, fbxsdk::FbxNodeAttribute::EType::eMesh, mNodeTree, [&](fbxsdk::FbxNode* node) {
			mNodeDatas.push_back(node);
			mMeshDatas.push_back(node->GetMesh());
		});
	}
	else {
		MessageBox(nullptr, L"Load Error <rootNode is null>", L"FBXModelLoader", MB_OK);
		return nullptr;
	}
	if (mMeshDatas.size() == 0)
	{
		MessageBox(nullptr, L"Error : メッシュが見つかりません", L"FBXModelLoader", MB_OK);
		return nullptr;
	}

	std::vector<std::shared_ptr<Fbx::FbxModelData>> models(mMeshDatas.size());

	for (int i = 0; i < static_cast<int>(mMeshDatas.size()); i++)
	{
		DirectX::XMMATRIX xmMat;
		auto translate = mNodeDatas[i]->EvaluateLocalTranslation();
		float scale = 1.0f;
		xmMat = DirectX::XMMatrixTranslation(static_cast<float>(translate[0] * scale), static_cast<float>(translate[1] * scale), static_cast<float>(translate[2] * scale));
		mGeometryOffset = ConvertXMMATRIXToXMFloat4x4(xmMat);
		//auto mat = DirectX::XMMatrixTranslation(10, 20, -30);
		models[i] = MainLoad(mMeshDatas[i], modelPath);
		models[i]->modelPath = modelPath;
	}

	LoadSkeletons();

	auto rtn = ConnectMeshes(models);

	DestroyScence(mScene);
	mManager->Destroy();
	ClearTmpInfo();

	return rtn;
}

void FbxLoader::LoadCurve(Fbx::AnimCurveData& curveData)
{
	if (curveData.curve == nullptr)
	{
		return;
	}
	int keyCount = curveData.curve->KeyGetCount();
	curveData.keys.resize(keyCount);
	Fbx::AnimKeyData keydata;
	for (int key = 0; key < keyCount; ++key)
	{
		keydata.time = curveData.curve->KeyGetTime(key).Get();
		keydata.value = curveData.curve->KeyGetValue(key);
		curveData.keys[key] = (keydata);
	}
}

void FbxLoader::DestroyScence(fbxsdk::FbxScene* scene)
{
	DestroyNode(scene->GetRootNode());
	scene->Destroy();
}

void FbxLoader::DestroyNode(fbxsdk::FbxNode* node)
{
	if (node == nullptr)
	{
		return;
	}
	int count = node->GetChildCount();
	for (int i = 0; i < count; ++i)
	{
		DestroyNode(node->GetChild(i));
	}

	node->Destroy();
}

/**ローカル関数定義**/

void StoreDiffuse(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.diffuse.element.x = static_cast<float>(lambert->Diffuse.Get()[0]);
		material.diffuse.element.y = static_cast<float>(lambert->Diffuse.Get()[1]);
		material.diffuse.element.z = static_cast<float>(lambert->Diffuse.Get()[2]);
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.diffuse.element.x = static_cast<float>(phong->Diffuse.Get()[0]);
		material.diffuse.element.y = static_cast<float>(phong->Diffuse.Get()[1]);
		material.diffuse.element.z = static_cast<float>(phong->Diffuse.Get()[2]);
	}
}

void StoreDiffuseFactor(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.diffuseFactor.element = static_cast<float>(lambert->DiffuseFactor.Get());
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.diffuseFactor.element = static_cast<float>(phong->DiffuseFactor.Get());
	}
}

void StoreAmbient(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.ambient.element.x = static_cast<float>(lambert->Ambient.Get()[0]);
		material.ambient.element.y = static_cast<float>(lambert->Ambient.Get()[1]);
		material.ambient.element.z = static_cast<float>(lambert->Ambient.Get()[2]);
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.ambient.element.x = static_cast<float>(phong->Ambient.Get()[0]);
		material.ambient.element.y = static_cast<float>(phong->Ambient.Get()[1]);
		material.ambient.element.z = static_cast<float>(phong->Ambient.Get()[2]);
	}
}

void StoreAmbientFactor(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.ambientFactor.element = static_cast<float>(lambert->DiffuseFactor.Get());
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.ambientFactor.element = static_cast<float>(phong->DiffuseFactor.Get());
	}
}

void StoreSpecular(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		material.specular.element.x = 0.0f;
		material.specular.element.y = 0.0f;
		material.specular.element.z = 0.0f;
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.specular.element.x = static_cast<float>(phong->Specular.Get()[0]);
		material.specular.element.y = static_cast<float>(phong->Specular.Get()[1]);
		material.specular.element.z = static_cast<float>(phong->Specular.Get()[2]);
	}
}

void StoreSpecularFactor(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceMaterial::ClassId))
	{
		material.specularFactor.element = 1.0f;
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.specularFactor.element = static_cast<float>(phong->SpecularFactor.Get());
	}
}

void StoreShininess(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceMaterial::ClassId))
	{
		material.shininess.element = 1.0f;
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.shininess.element = static_cast<float>(phong->Shininess.Get());
	}
}

void StoreEmissive(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.emissive.element.x = static_cast<float>(lambert->Emissive.Get()[0]);
		material.emissive.element.y = static_cast<float>(lambert->Emissive.Get()[1]);
		material.emissive.element.z = static_cast<float>(lambert->Emissive.Get()[2]);
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.emissive.element.x = static_cast<float>(phong->Emissive.Get()[0]);
		material.emissive.element.y = static_cast<float>(phong->Emissive.Get()[1]);
		material.emissive.element.z = static_cast<float>(phong->Emissive.Get()[2]);
	}
}

void StoreEmissiveFactor(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.emissiveFactor.element = static_cast<float>(lambert->EmissiveFactor.Get());
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.emissiveFactor.element = static_cast<float>(phong->EmissiveFactor.Get());
	}
}

void StoreTransparentColor(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.transparentColor.element.x = static_cast<float>(lambert->TransparentColor.Get()[0]);
		material.transparentColor.element.y = static_cast<float>(lambert->TransparentColor.Get()[1]);
		material.transparentColor.element.z = static_cast<float>(lambert->TransparentColor.Get()[2]);
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.transparentColor.element.x = static_cast<float>(phong->TransparentColor.Get()[0]);
		material.transparentColor.element.y = static_cast<float>(phong->TransparentColor.Get()[1]);
		material.transparentColor.element.z = static_cast<float>(phong->TransparentColor.Get()[2]);
	}
}

void StoreTransparencyFactor(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
{
	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
	{
		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
		material.transparencyFactor.element = static_cast<float>(lambert->TransparencyFactor.Get());
	}
	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
	{
		auto phong = (fbxsdk::FbxSurfacePhong*)(surfaceMaterial);
		material.transparencyFactor.element = static_cast<float>(phong->TransparencyFactor.Get());
	}
}

//void StoreNormalMap(Fbx::FbxMaterial& material, const fbxsdk::FbxSurfaceMaterial* surfaceMaterial)
//{
//	if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
//	{
//		auto lambert = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
//		material.normalmap.element.x = static_cast<float>(lambert->NormalMap.Get()[0]);
//		material.normalmap.element.y = static_cast<float>(lambert->NormalMap.Get()[1]);
//		material.normalmap.element.z = static_cast<float>(lambert->NormalMap.Get()[2]);
//	}
//	else if (surfaceMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
//	{
//		auto phong = (fbxsdk::FbxSurfaceLambert*)(surfaceMaterial);
//		material.normalmap.element.x = static_cast<float>(phong->NormalMap.Get()[0]);
//		material.normalmap.element.y = static_cast<float>(phong->NormalMap.Get()[1]);
//		material.normalmap.element.z = static_cast<float>(phong->NormalMap.Get()[2]);
//	}
//}

//ファイルパスの文字コード変換関数　FbxUTF8ToAnsi　などを使った変換をしていないので一部ファイルは読み込めない
bool GetTexture(fbxsdk::FbxProperty& prop, Fbx::FbxTexturesSet& textures)
{
	bool rtn = false;
	Fbx::FbxTexturesSet texSet;
	Fbx::FbxTexture tex;

	int textureCount = prop.GetSrcObjectCount<fbxsdk::FbxTexture>();

	for (int i = 0; i < textureCount; ++i)
	{
		auto layeredTexture = prop.GetSrcObject<fbxsdk::FbxLayeredTexture>(i);
		if (layeredTexture != nullptr)
		{
			int inTextureNum = layeredTexture->GetSrcObjectCount<fbxsdk::FbxTexture>();
			for (int j = 0; j < inTextureNum; ++j)
			{
				auto fbxTex = layeredTexture->GetSrcObject<fbxsdk::FbxTexture>(j);
				auto fileTex = fbxsdk::FbxCast<FbxFileTexture>(fbxTex);
				tex.textureName = fileTex->GetName();
				tex.texturePath = fileTex->GetRelativeFileName();
				tex.uvSetName = fileTex->UVSet.Get().Buffer();
				texSet.push_back(tex);
				rtn = true;
			}
		}
		else
		{
			auto fbxTex = prop.GetSrcObject<fbxsdk::FbxTexture>(i);
			auto fileTex = fbxsdk::FbxCast<FbxFileTexture>(fbxTex);
			tex.textureName = fileTex->GetName();
			tex.texturePath = fileTex->GetRelativeFileName();
			tex.uvSetName = fileTex->UVSet.Get().Buffer();
			texSet.push_back(tex);
			rtn = true;
		}
	}

	textures = std::move(texSet);

	char doubleslash = '\\';
	if (textures.size() != 0)
	{
		for (bool convertFinish = false; !convertFinish;)
		{
			size_t size = textures.back().texturePath.find(doubleslash);
			if (size <= textures.back().texturePath.size())
			{
				textures.back().texturePath[size] = '/';
			}
			else
			{
				convertFinish = true;
			}
		}
	}

	return rtn;
}

/**ローカル関数定義終了**/

void FbxLoader::LoadMatarial(std::shared_ptr<Fbx::FbxModelData> model, fbxsdk::FbxMesh * mesh)
{
	//START store texture path

	//コメントアウトしている部分は未対応
	const std::vector<std::string> TEXTURE_TABLE = {
		fbxsdk::FbxSurfaceMaterial::sDiffuse,
		fbxsdk::FbxSurfaceMaterial::sDiffuseFactor,
		fbxsdk::FbxSurfaceMaterial::sAmbient,
		fbxsdk::FbxSurfaceMaterial::sAmbientFactor,
		fbxsdk::FbxSurfaceMaterial::sSpecular,
		fbxsdk::FbxSurfaceMaterial::sSpecularFactor,
		fbxsdk::FbxSurfaceMaterial::sShininess,
		fbxsdk::FbxSurfaceMaterial::sEmissive,
		fbxsdk::FbxSurfaceMaterial::sEmissiveFactor,
		//fbxsdk::FbxSurfaceMaterial::sBump,
		fbxsdk::FbxSurfaceMaterial::sTransparentColor,
		fbxsdk::FbxSurfaceMaterial::sTransparencyFactor,
		//fbxsdk::FbxSurfaceMaterial::sNormalMap,
		//fbxsdk::FbxSurfaceMaterial::sReflection,
		//fbxsdk::FbxSurfaceMaterial::sReflectionFactor,
	};

	const std::vector <std::function<void(Fbx::FbxMaterial&, const fbxsdk::FbxSurfaceMaterial*)>> MATERIAL_TABLE = {
		StoreDiffuse,
		StoreDiffuseFactor,
		StoreAmbient,
		StoreAmbientFactor,
		StoreSpecular,
		StoreSpecularFactor,
		StoreShininess,
		StoreEmissive,
		StoreEmissiveFactor,
		StoreTransparentColor,
		StoreTransparencyFactor,
		//StoreNormalMap,
	};

	//マテリアルは１メッシュに一つの場合のみ対応
	int materialcount = mesh->GetNode()->GetMaterialCount();
	int elementMaterialCount = mesh->GetElementMaterialCount();
	mMaterialSets.clear();
	mMaterialSets.resize(materialcount);
	auto polygonVerticesCount = mesh->GetPolygonVertexCount();
	auto polygonCount = mesh->GetPolygonCount();
	mMaterialIDPerPolygon.clear();
	mMaterialIDPerPolygon.reserve(polygonCount);

	for (int element = 0; element < elementMaterialCount; ++element)
	{
		auto elementMaterial = mesh->GetElementMaterial(element);
		auto attribute = elementMaterial->GetMappingMode();

		if (attribute == fbxsdk::FbxLayerElement::eByPolygon)
		{
			fbxsdk::FbxLayerElementArrayTemplate<int>& polygonMaterialArray = elementMaterial->GetIndexArray();
			unsigned int indexCount = polygonMaterialArray.GetCount();
			Fbx::MaterialIndexSet matSet(static_cast<unsigned int>(polygonMaterialArray.GetAt(0)), 0U);//ループの中でインクリメントするのでカウントは0初期化
			for (unsigned int i = 0; i < indexCount; ++i)
			{
				unsigned int currentRefMaterialId = polygonMaterialArray.GetAt(i);
				mMaterialSets[currentRefMaterialId].materialId = currentRefMaterialId;
				mMaterialSets[currentRefMaterialId].polygonIds.push_back(i);
				++mMaterialSets[currentRefMaterialId].polygonCount;
				mMaterialIDPerPolygon.push_back(currentRefMaterialId);
			}
		}
		else if (attribute == fbxsdk::FbxLayerElement::eAllSame)
		{
			mMaterialSets[0].polygonCount = polygonCount;
			mMaterialIDPerPolygon.resize(polygonCount);
			std::fill(mMaterialIDPerPolygon.begin(), mMaterialIDPerPolygon.end(), 0);
		}
	}

	if (materialcount == 0)
	{
		mMaterialSets.resize(1);
		mMaterialSets[0].polygonCount = polygonCount;
		mMaterialIDPerPolygon.resize(polygonCount);
		std::fill(mMaterialIDPerPolygon.begin(), mMaterialIDPerPolygon.end(), 0);
	}

	model->materials.resize(materialcount);
	for (int i = 0; i < materialcount; ++i)
	{
		fbxsdk::FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetMaterial(i);

		for (unsigned int j = 0; j < static_cast<unsigned int>(TEXTURE_TABLE.size()); ++j)
		{
			auto findPorp = surfaceMaterial->FindProperty(TEXTURE_TABLE[j].data());
			if (findPorp.IsValid())
			{
				Fbx::FbxTexturesSet texSet;
				if (GetTexture(findPorp, texSet))
				{
					model->materials[i].SetTexture((Fbx::FbxMaterial::eELEMENT_TYPE)(j), texSet);
				}
				else
				{
					MATERIAL_TABLE[j](model->materials[i], surfaceMaterial);
				}
			}
		}
	}

	if (model->materials.size() == 0)
	{
		Fbx::FbxMaterial fbxmat;
		model->materials.resize(1);
		model->materials[0] = fbxmat;
	}

	//EBD store texture path
}

void FbxLoader::LoadSkeletons()
{
	//START skeleton Load
	fbxsdk::FbxNode* root = mScene->GetRootNode();
	NodeTree skeletonTree = {};
	DirectX::XMStoreFloat4x4(&skeletonTree.globalPosition, DirectX::XMMatrixIdentity());
	std::vector<fbxsdk::FbxNode*> skeletonNode;
	StackSearchNode(root, fbxsdk::FbxNodeAttribute::eSkeleton, skeletonTree, [&skeletonNode](fbxsdk::FbxNode* node) {
		skeletonNode.push_back(node);
	});

	mSkeletons.resize(skeletonNode.size());
	//基本的には2倍で足りると思う
	mSkeletonIndices.reserve(skeletonNode.size() * 2);

	unsigned int skeletonNum = static_cast<unsigned int>(skeletonNode.size());
	unsigned int skeletonIndex = 0;
	Fbx::FbxSkeleton skl;
	skl.parentIndex = UINT_MAX;

	for (int i = 0; i < skeletonTree.children.size(); ++i)
	{
		StoreSkeleton(skl, skeletonTree.children[i]);
		mSkeletons[skeletonIndex++] = skl;
		CreateskeletonData(skeletonTree.children[i], mSkeletonIndices, mSkeletons, skeletonIndex, 0);
	}
	mSkeletonIndices.shrink_to_fit();

	if (skeletonNum <= 0U)
	{
		MessageBox(nullptr, L"Skeleton Error <not exist skeleton>", L"FBXModelLoader", MB_OK);
	}

	//END skeleton Load
}
