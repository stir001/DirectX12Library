#include "stdafx.h"
#include "PMDLoader.h"
#include "PMDModel.h"
#include "PMDController.h"
#include "Buffer/IndexBufferObject.h"
#include "Buffer/VertexBufferObject.h"
#include "Buffer/ConstantBufferObject.h"
#include "Rootsignature/RootSignatureObject.h"
#include "Rootsignature/PMDToonRootsignature.h"
#include "Rootsignature/PMDBasicRootSignature.h"
#include "Texture/TextureObject.h"
#include "Master/Dx12Ctrl.h"
#include "Animation/VMD/VMDPlayer.h"
#include "Texture/TextureLoader.h"
#include "PipelineState/PipelineStateObject.h"
#include "PipelineState/PMDToonPipelineState.h"
#include "PipelineState/PMDBasicPipelineState.h"
#include "Light/DirectionalLight.h"
#include "Shader/ShaderCompiler.h"
#include "Util/File.h"
#include "Util/XMFloatOperators.h"
#include "RenderingPass/Manager/RenderingPassManager.h"
#include "BulletInclude.h"

#include <d3d12.h>
#include <algorithm>


PMDLoader::PMDLoader():mLight(std::make_shared<DirectionalLight>(1.f,-1.f,1.f))
{
	mCmdList = RenderingPassManager::Instance().GetRenderingPassCommandList(static_cast<unsigned int>(DefaultPass::Model));
	Microsoft::WRL::ComPtr<ID3D12Device> dev = Dx12Ctrl::Instance().GetDev();
	CreateRootsignature(dev);
	CreatePipelineState(dev);
}

PMDLoader::~PMDLoader()
{
}

std::shared_ptr<PMDController> PMDLoader::Load(const std::string& path)
{
	auto itr = mModels.find(path);
	if (itr != mModels.end())
	{
		return CreateController((*itr).second, path);
	}
	mFp = std::make_shared<File>(path);
	GetRelativePath(path);
	mLoadingmodel = PMDModel::Create();

	mLoadingmodel->mPath = path;
	mModels[path] = mLoadingmodel;
	LoadHeader();
	LoadVertex();
	LoadIndex();
	LoadMaterial();
	LoadBone();
	LoadIkList();
	LoadSkin();
	LoadSkinDisp();
	LoadBoneFrameDisp();
	LoadBoneDisp();
	SeekEngligh();
	LoadToonPath();
	LoadRigidBody();
	LoadJoint();

	mFp->Close();

	CreateIndexBuffer();
	CreateVertexBuffer();
	CreateTexture();
	CreateToonTexture();
	CreateMaterialBuffer();

	std::shared_ptr<PMDController> controller = CreateController(mLoadingmodel, path);
	mLoadingmodel = nullptr;
	return controller;
}

void PMDLoader::LoadHeader()
{
	mFp->LoadFile(&mLoadingmodel->mHeader.fileType[0], 3);
	mFp->LoadFile(&mLoadingmodel->mHeader.version);
	mFp->LoadFile(&mLoadingmodel->mHeader.name[0],sizeof(mLoadingmodel->mHeader.name));
	mFp->LoadFile(&mLoadingmodel->mHeader.comment[0], sizeof(mLoadingmodel->mHeader.comment));
	mFp->LoadFile(&mLoadingmodel->mHeader.vertexcount);
}

void PMDLoader::LoadVertex()
{
	mLoadingmodel->mVertexes.resize(mLoadingmodel->mHeader.vertexcount);
	for (auto& v : mLoadingmodel->mVertexes)
	{
		mFp->LoadFile(&v.pos);
		mFp->LoadFile(&v.normal);
		mFp->LoadFile(&v.uv.x);
		mFp->LoadFile(&v.uv.y);
		mFp->LoadFile(&v.boneNum[0], 2);
		mFp->LoadFile(&v.boneWeight);
		mFp->LoadFile(&v.edgeFlag);
	}
}

void PMDLoader::LoadIndex()
{
	unsigned int indexcount = 0;
	mFp->LoadFile(&indexcount);
	mLoadingmodel->mIndices.resize(indexcount);
	mFp->LoadFile(&mLoadingmodel->mIndices[0], static_cast<unsigned int>(mLoadingmodel->mIndices.size()));
}

void PMDLoader::LoadMaterial()
{
	unsigned int materialcount = 0;
	mFp->LoadFile(&materialcount);
	mLoadingmodel->mMaterials.resize(materialcount);
	std::vector<char*> texpaths;
	for (auto& m : mLoadingmodel->mMaterials)
	{
		mFp->LoadFile(&m.diffuse);
		mFp->LoadFile(&m.alpha);
		mFp->LoadFile(&m.specularity);
		mFp->LoadFile(&m.specular);
		mFp->LoadFile(&m.ambient);
		mFp->LoadFile(&m.toonIndex);
		mFp->LoadFile(&m.edgeFlag);
		mFp->LoadFile(&m.indexCount);
		mFp->LoadFile(&m.texturePath[0], sizeof(m.texturePath));
		if (m.texturePath[0] != 0)
		{
			auto itr = std::find_if(texpaths.begin(), texpaths.end(), [&](char* tpath) {return strcmp(tpath , m.texturePath) == 0; });
			if (itr == texpaths.end())
			{
				m.texid = mLoadingmodel->mTexturecount;
				texpaths.push_back(m.texturePath);
				mLoadingmodel->mTexturecount++;
			}
			else
			{
				m.texid = static_cast<int>(itr - texpaths.begin());
			}
			
		}
		else m.texid = -1;
	}
}

void PMDLoader::LoadBone()
{
	unsigned short bonecount = 0;
	mFp->LoadFile(&bonecount);
	mLoadingmodel->mBoneDatas.resize(bonecount);
	mLoadingmodel->mBoneNode.node.resize(bonecount);
	int index = 0;
	for (auto& b : mLoadingmodel->mBoneDatas)
	{
		mFp->LoadFile(&b.boneName[0], 20);
		mFp->LoadFile(&b.parentIndex);
		mFp->LoadFile(&b.tailIndex);
		mFp->LoadFile(&b.type);
		mFp->LoadFile(&b.ikParentIndex);
		mFp->LoadFile(&b.pos);
		if (/*b.tailIndex != 0 && */b.parentIndex != 0xffff)
		{
			mLoadingmodel->mBoneNode.node[b.parentIndex].push_back(index);
		}
		index++;
	}
}

void PMDLoader::LoadIkList()
{
	mFp->LoadFile(&mLoadingmodel->mIkLists.ikCount);
	mLoadingmodel->mIkLists.ikDatas.resize(mLoadingmodel->mIkLists.ikCount);
	for (auto& ik : mLoadingmodel->mIkLists.ikDatas)
	{
		mFp->LoadFile(&ik.ikBoneIndex);
		mFp->LoadFile(&ik.ikTargetBoneIndex);
		mFp->LoadFile(&ik.ikChainLength);
		mFp->LoadFile(&ik.iterations);
		mFp->LoadFile(&ik.controlWeight);
		ik.ikChildBoneIndex.resize(ik.ikChainLength);
		mFp->LoadFile(&ik.ikChildBoneIndex[0], ik.ikChainLength);
	}
}

void PMDLoader::LoadSkin()
{
	mFp->LoadFile(&mLoadingmodel->mSkins.skincount);
	mLoadingmodel->mSkins.datas.resize(mLoadingmodel->mSkins.skincount);
	for (auto& s : mLoadingmodel->mSkins.datas)
	{
		mFp->LoadFile(&s.skinName[0], sizeof(s.skinName));
		mFp->LoadFile(&s.skinVertexCount);
		mFp->LoadFile(&s.skintype);
		s.vertices.resize(s.skinVertexCount);
		mFp->LoadFile(&s.vertices[0], s.skinVertexCount);
	}
}

void PMDLoader::LoadSkinDisp()
{
	mFp->LoadFile(&mLoadingmodel->mSkinDisps.skindispcount);
	mLoadingmodel->mSkinDisps.skinIndices.resize(mLoadingmodel->mSkinDisps.skindispcount);
	mFp->LoadFile(&mLoadingmodel->mSkinDisps.skinIndices[0], mLoadingmodel->mSkinDisps.skindispcount);
}

void PMDLoader::LoadBoneFrameDisp()
{
	mFp->LoadFile(&mLoadingmodel->mBoneFrameDisps.boneDispNameCount);
	unsigned char count = mLoadingmodel->mBoneFrameDisps.boneDispNameCount;
	mLoadingmodel->mBoneFrameDisps.names.resize(count);
	for (auto& n : mLoadingmodel->mBoneFrameDisps.names)
	{
		mFp->LoadFile(&n.name[0], sizeof(n.name));
	}
}

void PMDLoader::LoadBoneDisp()
{
	mFp->LoadFile(&mLoadingmodel->mBoneDisps.boneocunt);
	mLoadingmodel->mBoneDisps.boneDispIndices.resize(mLoadingmodel->mBoneDisps.boneocunt);
	for (auto& i : mLoadingmodel->mBoneDisps.boneDispIndices) {
		mFp->LoadFile(&i.boneIndex);
		mFp->LoadFile(&i.boneDispFrameIndex);
	}
}

void PMDLoader::SeekEngligh()
{
	//‰pŒêƒwƒbƒ_
	mFp->SeekFile(sizeof(char));
	mFp->SeekFile(sizeof(char) * 20);
	mFp->SeekFile(sizeof(char) * 256);
	//‰pŒêƒ{[ƒ“–¼
	mFp->SeekFile(static_cast<int>(sizeof(char) * 20 * mLoadingmodel->mBoneDatas.size()));
	//‰pŒêƒXƒLƒ“ƒŠƒXƒg
	mFp->SeekFile(sizeof(char) * 20 * (mLoadingmodel->mSkins.skincount - 1));
	//‰pŒêƒ{[ƒ“˜g–¼•\Ž¦ƒŠƒXƒg
	mFp->SeekFile(sizeof(char) * 50 * (mLoadingmodel->mBoneFrameDisps.boneDispNameCount));
}

void PMDLoader::LoadToonPath()
{
	mFp->LoadFile(&mLoadingmodel->mToonpath.path[0][0], 100 * 10);
}

void PMDLoader::LoadRigidBody()
{
	mFp->LoadFile(&mLoadingmodel->mRigidBody.rigidbodycount);
	mLoadingmodel->mRigidBody.datas.resize(mLoadingmodel->mRigidBody.rigidbodycount);
	for (auto& r : mLoadingmodel->mRigidBody.datas)
	{
		mFp->LoadFile(&r.rigidBodyName[0], sizeof(r.rigidBodyName));
		mFp->LoadFile(&r.rigidbodyRelBoneIndex);
		mFp->LoadFile(&r.rigidbodyGroupIndex);
		mFp->LoadFile(&r.rigidbodyGroupTarget);
		r.rigidbodyGroupTarget = 0xffff - r.rigidbodyGroupTarget;
		mFp->LoadFile(&r.shapeType);
		mFp->LoadFile(&r.shapeW);
		mFp->LoadFile(&r.shapeH);
		mFp->LoadFile(&r.shapeD);
		mFp->LoadFile(&r.pos);
		mFp->LoadFile(&r.rot);
		mFp->LoadFile(&r.weight);
		mFp->LoadFile(&r.posDim);
		mFp->LoadFile(&r.rotDim);
		mFp->LoadFile(&r.recoil);
		mFp->LoadFile(&r.friction);
		mFp->LoadFile(&r.rigidbodyType);
	}
}

void PMDLoader::LoadJoint()
{
	mFp->LoadFile(&mLoadingmodel->mJoint.jointcount);
	mLoadingmodel->mJoint.datas.resize(mLoadingmodel->mJoint.jointcount);
	for (auto& j : mLoadingmodel->mJoint.datas)
	{
		mFp->LoadFile(&j.jointName[0],sizeof(j.jointName));
		mFp->LoadFile(&j.jointRigidbodyA);
		mFp->LoadFile(&j.jointRigidbodyB);
		mFp->LoadFile(&j.jointPos);
		mFp->LoadFile(&j.jointRot);
		mFp->LoadFile(&j.constrainPos1);
		mFp->LoadFile(&j.constrainPos2);
		mFp->LoadFile(&j.constrainRot1);
		mFp->LoadFile(&j.constrainRot2);
		mFp->LoadFile(&j.springPos);
		mFp->LoadFile(&j.springRot);
	}
}

void PMDLoader::CreateIndexBuffer()
{
	mLoadingmodel->mIndexBuffer = std::make_shared<IndexBufferObject>("PMDIndexBuffer", Dx12Ctrl::Instance().GetDev(), static_cast<unsigned int>(sizeof(mLoadingmodel->mIndices[0])),static_cast<unsigned int>(mLoadingmodel->mIndices.size()), DXGI_FORMAT_R16_UINT);
	mLoadingmodel->mIndexBuffer->WriteBuffer(&mLoadingmodel->mIndices[0], static_cast<unsigned int>(sizeof(mLoadingmodel->mIndices[0]) * mLoadingmodel->mIndices.size()));
}

void PMDLoader::CreateVertexBuffer()
{
	mLoadingmodel->mVertexBuffer = std::make_shared<VertexBufferObject>("PMDVertexBuffer", Dx12Ctrl::Instance().GetDev(), static_cast<unsigned int>(sizeof(mLoadingmodel->mVertexes[0])), static_cast<unsigned int>(mLoadingmodel->mVertexes.size()));
	mLoadingmodel->mVertexBuffer->WriteBuffer(&mLoadingmodel->mVertexes[0], static_cast<unsigned int>(sizeof(mLoadingmodel->mVertexes[0]) * mLoadingmodel->mVertexes.size()));
}

void PMDLoader::CreateTexture()
{
	mLoadingmodel->mTextureObjects.resize(mLoadingmodel->mTexturecount);
	mLoadingmodel->mTextureObjects.reserve(mLoadingmodel->mMaterials.size());
	unsigned int materialTextures = 0;
	for (auto& mat : mLoadingmodel->mMaterials)
	{
		if (mat.texid == -1)
		{
			mat.texid = mLoadingmodel->mTexturecount + materialTextures++;
			mLoadingmodel->mTextureObjects.push_back(
				TextureLoader::Instance().CreateSingleColorTexture({mat.diffuse.x, mat.diffuse.y, mat.diffuse.z, 1.0f}));
		}
		else
		{
			std::string loadPath = mRelativePath + mat.texturePath;
			mLoadingmodel->mTextureObjects[mat.texid] = TextureLoader::Instance().LoadTexture(loadPath);
		}
	}
	mLoadingmodel->mTextureObjects.shrink_to_fit();
}

void PMDLoader::CreateToonTexture()
{
	const unsigned int texNum = 10U;
	mLoadingmodel->mToonTextures.resize(texNum);
	for (unsigned int i = 0; i < texNum; ++i)
	{
		auto path = mRelativePath + mLoadingmodel->mToonpath.path[i * 10];
		mLoadingmodel->mToonTextures[i] = TextureLoader::Instance().LoadTexture(path);
	}
}

void PMDLoader::CreateMaterialBuffer()
{
	mLoadingmodel->mMaterialBuffer = std::make_shared<ConstantBufferObject>("PMDMaterialBuffer", Dx12Ctrl::Instance().GetDev(),static_cast<unsigned int>(sizeof(Dx12Material)), static_cast<unsigned int>(mLoadingmodel->mMaterials.size()));
	mLoadingmodel->mD12mat.resize(mLoadingmodel->mMaterials.size());
	for (unsigned int i = 0; i < static_cast<unsigned int>(mLoadingmodel->mMaterials.size()); i++)
	{
		mLoadingmodel->mD12mat[i].alpha = mLoadingmodel->mMaterials[i].alpha;
		mLoadingmodel->mD12mat[i].diffuse = ConvertXMFloat3ToXMFloat4(mLoadingmodel->mMaterials[i].diffuse);
		mLoadingmodel->mD12mat[i].ambient = ConvertXMFloat3ToXMFloat4(mLoadingmodel->mMaterials[i].ambient);
		mLoadingmodel->mD12mat[i].specular = ConvertXMFloat3ToXMFloat4(mLoadingmodel->mMaterials[i].specular);
		mLoadingmodel->mD12mat[i].specularity = mLoadingmodel->mMaterials[i].specularity;
	}

	mLoadingmodel->mMaterialBuffer->WriteBuffer256Alignment(mLoadingmodel->mD12mat.data(), sizeof(Dx12Material), static_cast<unsigned int>(mLoadingmodel->mMaterials.size()));
}


void PMDLoader::CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{

	mPipelinestate = std::make_shared<PMDBasicPipelineState>(mRootsignature, dev);

	mToonPipelineState = std::make_shared<PMDToonPipelineState>(mToonRootsiganture, dev);
}

void PMDLoader::CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	
	mRootsignature = std::make_shared<PMDBasicRootSignature>(dev);

	mToonRootsiganture = std::make_shared<PMDToonRootsignature>(dev);
}

std::shared_ptr<PMDController> PMDLoader::CreateController(std::shared_ptr<PMDModel>& model, const std::string& path)
{
	std::shared_ptr<PMDController> controller = std::make_shared<PMDController>(model, mLight, GetModelName(path),
		Dx12Ctrl::Instance().GetDev(), mCmdList);
	controller->SetLight(mLight);
	controller->SetRootSignature(mRootsignature);
	controller->SetPipelineState(mPipelinestate);
	controller->SetToonPipelineState(mToonPipelineState);
	controller->SetToonRootSignature(mToonRootsiganture);
	return controller;
}

std::string PMDLoader::GetModelName(const std::string & path) const
{
	size_t length = path.rfind('/') + 1;
	std::string rtn(path.begin() + length, path.end());
	return rtn;
}

void PMDLoader::CreateRigidBodis()
{
	for (auto data : mLoadingmodel->mRigidBody.datas)
	{
		BulletShapeType type;
		DirectX::XMFLOAT3 info;
		if (data.shapeType == 0)
		{
			type = BulletShapeType::SPHERE;
			info.x = data.shapeW;
		}
		else if (data.shapeType == 1)
		{
			type = BulletShapeType::BOX;
			info.x = data.shapeW;
			info.y = data.shapeH;
			info.z = data.shapeD;
		}
		else if (data.shapeType == 2)
		{
			type = BulletShapeType::CAPSULE;
			info.x = data.shapeW;
			info.y = data.shapeH;
		}
		auto rigid = PhysicsSystem::Instance().CreateRigidBody(type, info);
		rigid->SetMass(data.weight);
		BulletCollisionState state;
		if (data.rigidbodyType == 0)
		{
			state = BulletCollisionState::KINEMATIC;
		}
		else if (data.rigidbodyType == 1 || data.rigidbodyType == 2)
		{
			state = BulletCollisionState::STATIC;
		}
		rigid->SetCollisionState(state);
		rigid->SetFriction(data.friction);
		rigid->SetSpinFriction(data.rotDim);
		rigid->SetRestitution(data.recoil);
		rigid->SetYawPitchRoll(data.rot);
		rigid->SetOrigin(data.pos);
		
		
	}
}
