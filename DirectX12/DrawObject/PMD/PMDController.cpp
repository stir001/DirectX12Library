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
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
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
		AnimationPlayerManager::Instance().WaitSafeFree();
	}
}

void PMDController::Draw()
{
	(this->*mBundleUpdate)();
	mDescHeap->SetDescriptorHeap(mCmdList);
	mCmdList->ExecuteBundle(mBundleCmdList->GetCommandList().Get());
}

void PMDController::DrawShadowmap()
{
	mShadowmapCmdList->SetPipelineState(mShadowmapPipeline->GetPipelineState().Get());
	mShadowmapCmdList->SetGraphicsRootSignature(mShadowmapRootsignature->GetRootSignature().Get());
	mModel->SetVertexBuffer(mShadowmapCmdList);
	mModel->SetIndexBuffer(mShadowmapCmdList);
	mDescHeap->SetDescriptorHeap(mShadowmapCmdList);
	DrawWhileSetTable(mShadowmapCmdList);
}

void PMDController::DrawShadow()
{
	mCmdList->SetPipelineState(mShadowRenderPipeline->GetPipelineState().Get());
	mCmdList->SetGraphicsRootSignature(mShadowRenderRootsignature->GetRootSignature().Get());
	mModel->SetVertexBuffer(mCmdList);
	mModel->SetIndexBuffer(mCmdList);
	mShadowRenderDescHeap->SetDescriptorHeap(mCmdList);
	mShadowRenderDescHeap->SetGprahicsDescriptorTable(mCmdList, static_cast<unsigned int>(mModel->GetTextureObjects().size() + mModel->mToonTextures.size()));
	DrawWhileSetTable(mShadowmapCmdList);
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

void PMDController::SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsiganture)
{
	mBundleUpdate = &PMDController::UpdateBundle;
	DrawObjectController::SetRootSignature(rootsiganture);
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
	mShadowRenderRootsignature = rootsignature;
}

void PMDController::SetShadowRenderPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate)
{
	mShadowRenderRootsignature = pipelinestate;
}

void PMDController::SetShadowmapCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList)
{
	mShadowmapCmdList = cmdList;
}

void PMDController::UpdateDescriptorHeap()
{
}


void PMDController::DrawWhileSetTable(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	unsigned int indexOffset = 0;
	unsigned int offsetCount = 0;

	for (auto& material : mModel->mMaterials)
	{
		if (material.toonIndex != 255)
		{
			SetTextureWithToon(cmdList, material);
		}
		else
		{
			SetTexture(cmdList, material);
		}
		SetConstantBuffers(cmdList);
		SetMaterial(cmdList, static_cast<unsigned int>(mConstantBufferOffset + PMDModel::eROOT_PARAMATER_INDEX_MATERIAL), offsetCount);
		cmdList->DrawIndexedInstanced(material.indexCount, 1, indexOffset, 0, 0);
		indexOffset += material.indexCount;
		++offsetCount;
	}
}

void PMDController::SetTexture(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, PMDMaterial& material)
{
	cmdList->SetPipelineState(mPipelinestate->GetPipelineState().Get());
	cmdList->SetGraphicsRootSignature(mRootsignature->GetRootSignature().Get());
	mDescHeap->SetGprahicsDescriptorTable(cmdList, material.texid, PMDModel::eROOT_PARAMATER_INDEX_TEXTURE);
}

void PMDController::SetTextureWithToon(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, PMDMaterial & material)
{
	cmdList->SetPipelineState(mToonPipeline->GetPipelineState().Get());
	cmdList->SetGraphicsRootSignature(mToonRootsignature->GetRootSignature().Get());
	mDescHeap->SetGprahicsDescriptorTable(cmdList, material.texid, PMDModel::eROOT_PARAMATER_INDEX_TEXTURE);
	mDescHeap->SetGprahicsDescriptorTable(cmdList, mTextureNum + material.toonIndex, PMDModel::eROOT_PARAMATER_INDEX_TOON);
}

void PMDController::SetMaterial(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList, unsigned int resourceIndex, unsigned int offsetCount)
{
	mDescHeap->SetGprahicsDescriptorTable(cmdList, resourceIndex, PMDModel::eROOT_PARAMATER_INDEX_MATERIAL, offsetCount);
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
	mDescHeap = std::make_unique<Dx12DescriptorHeapObject>(descName, dev, buffers, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void PMDController::SetConstantBuffers(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
{
	unsigned int resourceIndex = mConstantBufferOffset;
	mDescHeap->SetGprahicsDescriptorTable(cmdList, resourceIndex++
		, PMDModel::eROOT_PARAMATER_INDEX_CAMERA);
	mDescHeap->SetGprahicsDescriptorTable(cmdList, resourceIndex++
		, PMDModel::eROOT_PARAMATER_INDEX_LIGHT);
	mDescHeap->SetGprahicsDescriptorTable(cmdList, resourceIndex++
		, PMDModel::eROOT_PARAMATER_INDEX_BONE_MATRIX);
	mDescHeap->SetGprahicsDescriptorTable(cmdList, resourceIndex++
		, PMDModel::eROOT_PARAMATER_INDEX_MODEL_MATRIX);
}

void PMDController::UpdateBundle()
{
	mBundleCmdList->Reset();
	const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& bundle = mBundleCmdList->GetCommandList();
	bundle->SetPipelineState(mPipelinestate->GetPipelineState().Get());
	bundle->SetGraphicsRootSignature(mRootsignature->GetRootSignature().Get());

	mModel->SetIndexBuffer(bundle);
	mModel->SetVertexBuffer(bundle);
	mDescHeap->SetDescriptorHeap(bundle);

	DrawWhileSetTable(bundle);
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
	buffers.push_back(mShadowmapTexture);
	buffers.push_back(mCameraBuffer);
	buffers.push_back(mDirLight->GetLightBuffer());
	buffers.push_back(mBoneMatrixBuffer);
	buffers.push_back(mModelMatrixBuffer);
	buffers.push_back(mModel->GetMaterialBuffer());
	std::string descName = name + "DescriptorHeap";
	mShadowRenderDescHeap = std::make_unique<Dx12DescriptorHeapObject>(descName, dev, buffers, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}
