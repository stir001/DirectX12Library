#include "stdafx.h"
#include "PMDController.h"
#include "PMDModel.h"
#include "Master/Dx12Ctrl.h"
#include "RootSignature/RootSignatureObject.h"
#include "PipelineState/PipelineStateObject.h"
#include "Buffer/ConstantBufferObject.h"
#include "Buffer/IndexBufferObject.h"
#include "Buffer/VertexBufferObject.h"
#include "Buffer/ConstantBufferObject.h"
#include "Buffer/ShaderResourceObject.h"
#include "Texture/TextureObject.h"
#include "Animation/VMD/VMDPlayer.h"
#include "Light/DirectionalLight.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "Camera/Dx12Camera.h"
#include "CommandList/Dx12CommandList.h"
#include "Util/XMFloatOperators.h"
#include "Animation/AnimationPlayerManager.h"

PMDController::PMDController(std::shared_ptr<PMDModel>& model, std::shared_ptr<DirectionalLight>& dlight, const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	std::shared_ptr<Dx12CommandList>& cmdList)
	: DrawController3D(name, dev, cmdList), mModel(model)
	, mDirLight(dlight), mBundleUpdate(&PMDController::UpdateBundle)
	, mConstantBufferOffset(0U), mTextureNum(0U)
{
	mBoneMatrixBuffer = std::make_shared<ConstantBufferObject>("PMDBoneMatrixBuffer", mDevice, static_cast<unsigned int>(sizeof(DirectX::XMMATRIX) * mModel->mBoneDatas.size()), 1);
	mBoneMatrix.resize(mModel->mBoneDatas.size());
	for (auto& bm : mBoneMatrix)  DirectX::XMStoreFloat4x4(&bm, DirectX::XMMatrixIdentity());
	mBoneMatrixBuffer->WriteBuffer(&mBoneMatrix[0], static_cast<unsigned int>(sizeof(DirectX::XMMATRIX) * mModel->mBoneDatas.size()));
	mVmdPlayer = std::make_shared<VMDPlayer>(mModel->mBoneDatas, mModel->mBoneNode, mBoneMatrix,mBoneMatrixBuffer);

	CreateDescriptorHeap(dev, name);
}

PMDController::~PMDController()
{
	if (mVmdPlayer->GetID() != -1)
	{
		AnimationPlayerManager::Instance().WaitAnimation();
	}
}

void PMDController::Draw()
{
	(this->*mBundleUpdate)();
	mDescHeap->SetDescriptorHeap(mCmdList);
	mCmdList->ExecuteBundle(mBundleCmdList);
	mCmdList->SetDrawController(shared_from_this());
}

void PMDController::DrawShadowmap()
{
	mShadowmapCmdList->SetPipelineState(mShadowmapPipeline);
	mShadowmapCmdList->SetGraphicsRootSignature(mShadowmapRootsignature);
	mModel->SetVertexBuffer(mShadowmapCmdList);
	mModel->SetIndexBuffer(mShadowmapCmdList);
	mDescHeap->SetDescriptorHeap(mShadowmapCmdList);
	SetConstantBuffers(mShadowmapCmdList);
	mShadowmapCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mShadowmapCmdList->DrawIndexedInstanced(static_cast<UINT>(mModel->mIndices.size()), 1, 0, 0, 0);
	mShadowmapCmdList->SetDrawController(shared_from_this());
}

void PMDController::DrawShadow()
{
	mCmdList->SetPipelineState(mShadowToonRenderPipeline);
	mCmdList->SetGraphicsRootSignature(mShadowToonRenderRootsignature);
	mModel->SetVertexBuffer(mCmdList);
	mModel->SetIndexBuffer(mCmdList);
	mShadowRenderDescHeap->SetDescriptorHeap(mCmdList);


	auto toonPair = std::pair<std::shared_ptr<PipelineStateObject>, std::shared_ptr<RootSignatureObject>>(mShadowToonRenderPipeline, mShadowToonRenderRootsignature);
	auto basicPair = std::pair<std::shared_ptr<PipelineStateObject>, std::shared_ptr<RootSignatureObject>>(mShadowRenderPipeline, mShadowRenderRootsignature);
	mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DrawWhileSetTable(mCmdList, toonPair, basicPair);
	mCmdList->SetDrawController(shared_from_this());
}

void PMDController::SetMotion(std::shared_ptr<VMDMotion> motion)
{
	mVmdPlayer->SetVMD(motion);
}

void PMDController::PlayMotion(bool loopFlag)
{
	mVmdPlayer->Play(loopFlag);
}

void PMDController::StopMotion()
{
	mVmdPlayer->Stop();
}

void PMDController::SetLight(std::shared_ptr<DirectionalLight> dlight)
{
	mDirLight = dlight;
}

void PMDController::SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelineState)
{
	mBundleUpdate = &PMDController::UpdateBundle;
	DrawObjectController::SetPipelineState(pipelineState);
}

void PMDController::SetGraphicsRootSignature(std::shared_ptr<RootSignatureObject>& rootsiganture)
{
	mBundleUpdate = &PMDController::UpdateBundle;
	DrawObjectController::SetGraphicsRootSignature(rootsiganture);
}

void PMDController::SetToonPipelineState(std::shared_ptr<PipelineStateObject>& pipelineState)
{
	mBundleUpdate = &PMDController::UpdateBundle;
	mToonPipeline = pipelineState;
}

void PMDController::SetToonRootSignature(std::shared_ptr<RootSignatureObject>& rootsiganture)
{
	mBundleUpdate = &PMDController::UpdateBundle;
	mToonRootsignature = rootsiganture;
}

void PMDController::SetShadowmapRootsignature(std::shared_ptr<RootSignatureObject>& rootsignature)
{
	mShadowmapRootsignature = (rootsignature);
}

void PMDController::SetShadowmapPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate)
{
	mShadowmapPipeline = pipelinestate;
}

void PMDController::SetShadowRenderRootsignature(std::shared_ptr<RootSignatureObject>& rootsignature)
{
	mShadowToonRenderRootsignature = rootsignature;
}

void PMDController::SetShadowRenderPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate)
{
	mShadowToonRenderPipeline = pipelinestate;
}

void PMDController::SetShadowBasicRenderPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate)
{
	mShadowRenderPipeline = pipelinestate;
}

void PMDController::SetShadowBasicRenderRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature)
{
	mShadowRenderRootsignature = rootsignature;
}

void PMDController::SetShadowmapCommandList(std::shared_ptr<Dx12CommandList> cmdList)
{
	mShadowmapCmdList = cmdList;
}

void PMDController::UpdateDescriptorHeap()
{
}

void PMDController::DrawWhileSetTable(std::shared_ptr<Dx12CommandList>& cmdList
	, std::pair<std::shared_ptr<PipelineStateObject>, std::shared_ptr<RootSignatureObject>> toonPair
	, std::pair<std::shared_ptr<PipelineStateObject>, std::shared_ptr<RootSignatureObject>> basicPair)
{
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	unsigned int indexOffset = 0;
	unsigned int offsetCount = 0;

	for (auto& material : mModel->mMaterials)
	{
		if (material.toonIndex != 255)
		{
			cmdList->SetPipelineState(toonPair.first);
			cmdList->SetGraphicsRootSignature(toonPair.second);
			mDescHeap->SetGraphicsDescriptorTable(cmdList, material.texid, PMDModel::eROOT_PARAMATER_INDEX_TEXTURE);
			mDescHeap->SetGraphicsDescriptorTable(cmdList, mTextureNum + material.toonIndex, PMDModel::eROOT_PARAMATER_INDEX_TOON);
			mShadowRenderDescHeap->SetGraphicsDescriptorTable(cmdList
				, static_cast<unsigned int>(mModel->GetTextureObjects().size() + mModel->mToonTextures.size())
				, PMDModel::eROOT_PARAMATER_INDEX_SHADOWMAP);
		}
		else
		{
			cmdList->SetPipelineState(basicPair.first);
			cmdList->SetGraphicsRootSignature(basicPair.second);
			mDescHeap->SetGraphicsDescriptorTable(cmdList, material.texid, PMDModel::eROOT_PARAMATER_INDEX_TEXTURE);
			mShadowRenderDescHeap->SetGraphicsDescriptorTable(cmdList
				, static_cast<unsigned int>(mModel->GetTextureObjects().size() + mModel->mToonTextures.size())
				, PMDModel::eROOT_PARAMATER_INDEX_SHADOWMAP - 1);
		}
		SetConstantBuffers(cmdList);
		SetMaterial(cmdList, static_cast<unsigned int>(mConstantBufferOffset + PMDModel::eROOT_PARAMATER_INDEX_MATERIAL), offsetCount);
		cmdList->DrawIndexedInstanced(material.indexCount, 1, indexOffset, 0, 0);
		indexOffset += material.indexCount;
		++offsetCount;
	}
}


void PMDController::SetMaterial(std::shared_ptr<Dx12CommandList>& cmdList, unsigned int resourceIndex, unsigned int offsetCount)
{
	mDescHeap->SetGraphicsDescriptorTable(cmdList, resourceIndex, PMDModel::eROOT_PARAMATER_INDEX_MATERIAL, offsetCount);
}

void PMDController::CreateDescriptorHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, const std::string & name)
{
	auto texObjs = mModel->GetTextureObjects();
	auto toonTexs = mModel->mToonTextures;
	mTextureNum = static_cast<unsigned int>(texObjs.size());
	mConstantBufferOffset = static_cast<unsigned int>(mTextureNum + toonTexs.size());
	std::vector<std::shared_ptr<Dx12BufferObject>> buffers;
	buffers.reserve(mConstantBufferOffset + PMDModel::CONSTANT_BUFFER_NUM);
	for (auto& tex : texObjs)
	{
		buffers.push_back(tex->GetShaderResource());
	}
	for (auto toon : toonTexs)
	{
		buffers.push_back(toon->GetShaderResource());
	}
	buffers.push_back(mCameraBuffer);
	buffers.push_back(mDirLight->GetLightBuffer());
	buffers.push_back(mBoneMatrixBuffer);
	buffers.push_back(mModelMatrixBuffer);
	buffers.push_back(mModel->GetMaterialBuffer());
	std::string descName = name + "DescriptorHeap";
	mDescHeap = std::make_shared<Dx12DescriptorHeapObject>(descName, dev, buffers, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void PMDController::SetConstantBuffers(std::shared_ptr<Dx12CommandList>& cmdList)
{
	unsigned int resourceIndex = mConstantBufferOffset;
	mDescHeap->SetGraphicsDescriptorTable(cmdList, resourceIndex++
		, PMDModel::eROOT_PARAMATER_INDEX_CAMERA);
	mDescHeap->SetGraphicsDescriptorTable(cmdList, resourceIndex++
		, PMDModel::eROOT_PARAMATER_INDEX_LIGHT);
	mDescHeap->SetGraphicsDescriptorTable(cmdList, resourceIndex++
		, PMDModel::eROOT_PARAMATER_INDEX_BONE_MATRIX);
	mDescHeap->SetGraphicsDescriptorTable(cmdList, resourceIndex++
		, PMDModel::eROOT_PARAMATER_INDEX_MODEL_MATRIX);
}

void PMDController::UpdateBundle()
{
	mBundleCmdList->Reset();
	auto bundle = mBundleCmdList;
	bundle->SetPipelineState(mPipelinestate);
	bundle->SetGraphicsRootSignature(mRootsignature);

	mModel->SetIndexBuffer(bundle);
	mModel->SetVertexBuffer(bundle);
	mDescHeap->SetDescriptorHeap(bundle);

	auto toonPair = std::pair<std::shared_ptr<PipelineStateObject>, std::shared_ptr<RootSignatureObject>>(mToonPipeline, mToonRootsignature);
	auto basicPair = std::pair<std::shared_ptr<PipelineStateObject>, std::shared_ptr<RootSignatureObject>>(mPipelinestate, mRootsignature);

	DrawWhileSetTable(bundle, toonPair, basicPair);
	mBundleCmdList->Close();
	mBundleUpdate = &PMDController::NonUpdateBundle;
}

void PMDController::NonUpdateBundle()
{

}

void PMDController::CreateShadowRenderDescHeap(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, const std::string & name)
{
	auto texObjs = mModel->GetTextureObjects();
	auto toonTexs = mModel->mToonTextures;
	mTextureNum = static_cast<unsigned int>(texObjs.size());
	mConstantBufferOffset = static_cast<unsigned int>(mTextureNum + toonTexs.size()) + 1;
	std::vector<std::shared_ptr<Dx12BufferObject>> buffers;
	buffers.reserve(mConstantBufferOffset + PMDModel::CONSTANT_BUFFER_NUM);
	for (auto& tex : texObjs)
	{
		buffers.push_back(tex->GetShaderResource());
	}
	for (auto toon : toonTexs)
	{
		buffers.push_back(toon->GetShaderResource());
	}
	mShadowmapTexture->CreateShaderResourceViewDesc();
	buffers.push_back(mShadowmapTexture);
	buffers.push_back(mCameraBuffer);
	buffers.push_back(mDirLight->GetLightBuffer());
	buffers.push_back(mBoneMatrixBuffer);
	buffers.push_back(mModelMatrixBuffer);
	buffers.push_back(mModel->GetMaterialBuffer());
	std::string descName = name + "DescriptorHeap";
	mShadowRenderDescHeap = std::make_shared<Dx12DescriptorHeapObject>(descName, dev, buffers, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	mDescHeap = mShadowRenderDescHeap;
}

void PMDController::SetShadowmap(std::shared_ptr<Dx12BufferObject> shadowmap)
{
	mShadowmapTexture = shadowmap;
}
