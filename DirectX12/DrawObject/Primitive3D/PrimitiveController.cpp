#include "stdafx.h"
#include "PrimitiveController.h"
#include "PrimitiveObject.h"
#include "Buffer/VertexBufferObject.h"
#include "Buffer/IndexBufferObject.h"
#include "Buffer/ConstantBufferObject.h"
#include "Buffer/ShaderResourceObject.h"
#include "Util/XMFloatOperators.h"
#include "Texture/TextureObject.h"
#include "Texture/TextureLoader.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "PipelineState/PipelineStateObject.h"
#include "Rootsignature/RootSignatureObject.h"
#include "Light/LightObject.h"
#include "Master/Dx12Ctrl.h"
#include "CommandList/Dx12CommandList.h"


PrimitiveController::PrimitiveController(std::shared_ptr<PrimitiveObject> primitive
	, Microsoft::WRL::ComPtr<ID3D12Device>& dev
	, std::shared_ptr<Dx12CommandList>& cmdList)
	: DrawController3D(primitive->GetName(), dev, cmdList)
	, mPrimitive(primitive)
	, mInstanceUpdate(&PrimitiveController::UpdateInstanceVertexBuffer)
{
	Initialize(dev);
}

PrimitiveController::PrimitiveController(std::vector<PrimitiveVertex>& vertices, std::vector<unsigned int> indices
	, Microsoft::WRL::ComPtr<ID3D12Device>& dev, std::shared_ptr<Dx12CommandList>& cmdList)
	: DrawController3D("CustumPrimitive", dev, cmdList)
{
	mPrimitive = std::make_shared<PrimitiveObject>("CustumPrimitive", vertices, indices);
	Initialize(dev);
}


PrimitiveController::~PrimitiveController()
{
}

void PrimitiveController::SetTexture(std::shared_ptr<TextureObject>& texture)
{
	mTexObj = texture;
	mDescHeapUpdate = &PrimitiveController::UpdateDescriptorHeap;
}

void PrimitiveController::SetLightBuffer(std::shared_ptr<LightObject>& light)
{
	mLightBuffer = light->GetLightBuffer();
	mDescHeapUpdate = &PrimitiveController::UpdateDescriptorHeap;
}

void PrimitiveController::Instancing(std::vector<DirectX::XMFLOAT3>& instancePositions)
{
	mInstanceDatas.resize(instancePositions.size());
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixIdentity());
	for (unsigned int i = 0; i < static_cast<unsigned int>(instancePositions.size()); ++i)
	{
		mInstanceDatas[i].aMatrix = mat;
		mInstanceDatas[i].offset = ConvertToXMFloat4(instancePositions[i]);
	}

	mInstanceVertexBuffer = std::make_shared<VertexBufferObject>(mPrimitive->GetName() + "InstanceVertexBuffer", mDevice, static_cast<unsigned int>(sizeof(mInstanceDatas[0])), static_cast<unsigned int>(mInstanceDatas.size()));
	mInstanceUpdate = &PrimitiveController::UpdateInstanceVertexBuffer;
}

void PrimitiveController::SetInstancingMatrix(std::vector<DirectX::XMFLOAT4X4>& matrix, unsigned int startIndex, unsigned int endIndex)
{
	for (unsigned int i = startIndex; i <= endIndex; ++i)
	{
		mInstanceDatas[i].aMatrix = matrix[i - startIndex];
	}
	mInstanceUpdate = &PrimitiveController::UpdateInstanceVertexBuffer;
}

DirectX::XMFLOAT4X4 PrimitiveController::GetMatrix() const
{
	return mInstanceDatas[0].aMatrix;
}

void PrimitiveController::Draw()
{
	(this->*mInstanceUpdate)();
	(this->*mDescHeapUpdate)();

	mCmdList->SetPipelineState(mPipelinestate);
	mCmdList->SetGraphicsRootSignature(mRootsignature);
	mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	std::shared_ptr<VertexBufferObject> vertexBuffers[] = { mVertexBuffer, mInstanceVertexBuffer };
	mCmdList->IASetVertexBuffers(vertexBuffers, 2);
	mCmdList->IASetIndexBuffer(mIndexBuffer);
	mDescHeap->SetDescriptorHeap(mCmdList);
	unsigned int resourceIndex = 0;
	mDescHeap->SetGraphicsDescriptorTable(mCmdList, resourceIndex++, eROOT_PARAMATER_INDEX_TEXTURE);
	mDescHeap->SetGraphicsDescriptorTable(mCmdList, resourceIndex++, eROOT_PARAMATER_INDEX_CAMERA);
	mDescHeap->SetGraphicsDescriptorTable(mCmdList, resourceIndex++, eROOT_PARAMATER_INDEX_LIGHT);

	mCmdList->DrawIndexedInstanced(static_cast<UINT>(mPrimitive->GetIndices().size()), static_cast<UINT>(mInstanceDatas.size()), 0, 0, 0);
	mCmdList->SetDrawController(shared_from_this());
}

void PrimitiveController::SetPosition(const DirectX::XMFLOAT3 & pos)
{
	DrawController3D::SetPosition(pos);
	mInstanceDatas[0].aMatrix = mModelMatrix;
	mInstanceUpdate = &PrimitiveController::UpdateInstanceVertexBuffer;
}

void PrimitiveController::SetScale(float scale)
{
	DrawController3D::SetScale(scale);
	mInstanceDatas[0].aMatrix = mModelMatrix;
	mInstanceUpdate = &PrimitiveController::UpdateInstanceVertexBuffer;
}

void PrimitiveController::SetScale(float scaleX, float scaleY, float scaleZ)
{
	DrawController3D::SetScale(scaleX, scaleY, scaleZ);
	mInstanceDatas[0].aMatrix = mModelMatrix;
	mInstanceUpdate = &PrimitiveController::UpdateInstanceVertexBuffer;
}

void PrimitiveController::AddRotaX(float rad)
{
	DrawController3D::AddRotaX(rad);
	mInstanceDatas[0].aMatrix = mModelMatrix;
	mInstanceUpdate = &PrimitiveController::UpdateInstanceVertexBuffer;
}

void PrimitiveController::AddRotaY(float rad)
{
	DrawController3D::AddRotaY(rad);
	mInstanceDatas[0].aMatrix = mModelMatrix;
	mInstanceUpdate = &PrimitiveController::UpdateInstanceVertexBuffer;
}

void PrimitiveController::AddRotaZ(float rad)
{
	DrawController3D::AddRotaZ(rad);
	mInstanceDatas[0].aMatrix = mModelMatrix;
	mInstanceUpdate = &PrimitiveController::UpdateInstanceVertexBuffer;
}

void PrimitiveController::SetColor(const DirectX::XMFLOAT4& color, int index)
{
	mPrimitive->SetColor(color);
	mInstanceDatas[index].color = color;
	mInstanceUpdate = &PrimitiveController::UpdateInstanceVertexBuffer;
}

void PrimitiveController::SetShadowmapRootSignature(std::shared_ptr<RootSignatureObject> rootsignature)
{
	mShadowmapRootSignature = rootsignature;
}

void PrimitiveController::SetShadowmapPipelineState(std::shared_ptr<PipelineStateObject> pipelinestate)
{
	mShadowmapPipeline = pipelinestate;
}

void PrimitiveController::SetShadowRenderRootSignature(std::shared_ptr<RootSignatureObject> rootsignature)
{
	mShadowRenderRootSignature = rootsignature;
}

void PrimitiveController::SetShadowRenderPipelineState(std::shared_ptr<PipelineStateObject> pipelinestate)
{
	mShadowRenderPipeline = pipelinestate;
}

void PrimitiveController::SetShadowmapTexture(std::shared_ptr<Dx12BufferObject> shadowmapTex)
{
	mShadowmapTexture = shadowmapTex;
}

void PrimitiveController::CreateShadowmapDescHeap()
{
	std::vector<std::shared_ptr<Dx12BufferObject>> buffers(4);
	buffers[eROOT_PARAMATER_INDEX_TEXTURE] = mTexObj->GetShaderResource();
	buffers[eROOT_PARAMATER_INDEX_CAMERA] = mCameraBuffer;
	buffers[eROOT_PARAMATER_INDEX_LIGHT] = mLightBuffer;
	buffers[eROOT_PARAMATER_INDEX_SHADOWMAP] = mShadowmapTexture;
	mShadowmapTexture->CreateShaderResourceViewDesc();

	mDescHeap = std::make_shared<Dx12DescriptorHeapObject>(mPrimitive->GetName() + "DescriptorHeap", mDevice, buffers, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void PrimitiveController::DrawShadowmap()
{
	UpdateInstanceVertexBuffer();
	mShadowmapCmdList->SetPipelineState(mShadowmapPipeline);
	mShadowmapCmdList->SetGraphicsRootSignature(mShadowmapRootSignature);
	mDescHeap->SetDescriptorHeap(mShadowmapCmdList);
	std::shared_ptr<VertexBufferObject> vertexBuffers[] = { mVertexBuffer, mInstanceVertexBuffer };
	mShadowmapCmdList->IASetVertexBuffers(vertexBuffers, 2);
	mShadowmapCmdList->IASetIndexBuffer(mIndexBuffer);
	mShadowmapCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	unsigned int resourceIndex = 0;
	mDescHeap->SetGraphicsDescriptorTable(mShadowmapCmdList, resourceIndex++, eROOT_PARAMATER_INDEX_TEXTURE);
	mDescHeap->SetGraphicsDescriptorTable(mShadowmapCmdList, resourceIndex++, eROOT_PARAMATER_INDEX_CAMERA);
	mDescHeap->SetGraphicsDescriptorTable(mShadowmapCmdList, resourceIndex++, eROOT_PARAMATER_INDEX_LIGHT);

	mShadowmapCmdList->DrawIndexedInstanced(static_cast<UINT>(mPrimitive->GetIndices().size()), static_cast<UINT>(mInstanceDatas.size()), 0, 0, 0);
	mShadowmapCmdList->SetDrawController(shared_from_this());
}

void PrimitiveController::DrawShadow()
{
	UpdateInstanceVertexBuffer();
	mCmdList->SetPipelineState(mShadowRenderPipeline);
	mCmdList->SetGraphicsRootSignature(mShadowRenderRootSignature);
	mDescHeap->SetDescriptorHeap(mCmdList);
	std::shared_ptr<VertexBufferObject> vertexBuffers[] = { mVertexBuffer, mInstanceVertexBuffer };
	mCmdList->IASetVertexBuffers(vertexBuffers, 2);
	mCmdList->IASetIndexBuffer(mIndexBuffer);
	mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	unsigned int resourceIndex = 0;
	mDescHeap->SetGraphicsDescriptorTable(mCmdList, resourceIndex++, eROOT_PARAMATER_INDEX_TEXTURE);
	mDescHeap->SetGraphicsDescriptorTable(mCmdList, resourceIndex++, eROOT_PARAMATER_INDEX_CAMERA);
	mDescHeap->SetGraphicsDescriptorTable(mCmdList, resourceIndex++, eROOT_PARAMATER_INDEX_LIGHT);
	mDescHeap->SetGraphicsDescriptorTable(mCmdList, resourceIndex++, eROOT_PARAMATER_INDEX_SHADOWMAP);

	mCmdList->DrawIndexedInstanced(static_cast<UINT>(mPrimitive->GetIndices().size()), static_cast<UINT>(mInstanceDatas.size()), 0, 0, 0);
	mCmdList->SetDrawController(shared_from_this());
}

void PrimitiveController::SetShadowmapCommandList(std::shared_ptr<Dx12CommandList>& cmdList)
{
	mShadowmapCmdList = cmdList;
}

void PrimitiveController::SetGraphicsRootSignature(const std::shared_ptr<RootSignatureObject>& rootSignature)
{
	mRootsignature = rootSignature;
}

void PrimitiveController::SetPipelineState(const std::shared_ptr<PipelineStateObject>& pipelineState)
{
	mPipelinestate = pipelineState;
}

void PrimitiveController::UpdateInstanceVertexBuffer()
{
	mInstanceVertexBuffer->WriteBuffer(&mInstanceDatas[0], static_cast<unsigned int>(sizeof(mInstanceDatas[0]) * mInstanceDatas.size()));
	mInstanceUpdate = &PrimitiveController::NonUpdate;
}

void PrimitiveController::NonUpdate()
{
}

void PrimitiveController::Initialize(Microsoft::WRL::ComPtr<ID3D12Device>& dev)
{
	const auto& indices = mPrimitive->GetIndices();
	mIndexBuffer = std::make_shared<IndexBufferObject>(mPrimitive->GetName() + "IndexBuffer", dev, static_cast<unsigned int>(sizeof(indices[0])), static_cast<unsigned int>(indices.size()));
	mIndexBuffer->WriteBuffer(mPrimitive->GetIndices().data(), static_cast<unsigned int>(sizeof(indices[0]) * indices.size()));

	const auto& vertices = mPrimitive->GetVertices();
	mVertexBuffer = std::make_shared<VertexBufferObject>(mPrimitive->GetName() + "VertexBuffer", dev, static_cast<unsigned int>(sizeof(vertices[0])), static_cast<unsigned int>(vertices.size()));
	mVertexBuffer->WriteBuffer(vertices.data(), static_cast<unsigned int>(sizeof(vertices[0]) * vertices.size()));

	InstanceDatas data;
	DirectX::XMStoreFloat4x4(&data.aMatrix, DirectX::XMMatrixIdentity());
	data.offset = DirectX::XMFLOAT4(0, 0, 0, 1);
	data.color = mPrimitive->GetColor();
	mInstanceDatas.push_back(data);
	mInstanceVertexBuffer = std::make_shared<VertexBufferObject>(mPrimitive->GetName() + "InstanceVertexBuffer", dev, static_cast<unsigned int>(sizeof(mInstanceDatas[0])), 1U);

	mTexObj = TextureLoader::Instance().CreateSingleColorTexture(0.0f);

	mInstanceUpdate = &PrimitiveController::UpdateInstanceVertexBuffer;
	mDescHeapUpdate = &PrimitiveController::UpdateDescriptorHeap;
}

void PrimitiveController::SetRotaQuaternion(const DirectX::XMFLOAT4& quaternion)
{
	DrawController3D::SetRotaQuaternion(quaternion);
	mInstanceDatas[0].aMatrix = mModelMatrix;
	mInstanceUpdate = &PrimitiveController::UpdateInstanceVertexBuffer;
}

void PrimitiveController::UpdateDescriptorHeap()
{
	std::vector<std::shared_ptr<Dx12BufferObject>> buffers(3);
	buffers[eROOT_PARAMATER_INDEX_TEXTURE] = mTexObj->GetShaderResource();
	buffers[eROOT_PARAMATER_INDEX_CAMERA] = mCameraBuffer;
	buffers[eROOT_PARAMATER_INDEX_LIGHT] = mLightBuffer;

	mDescHeap = std::make_shared<Dx12DescriptorHeapObject>(mPrimitive->GetName() + "DescriptorHeap", mDevice, buffers, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	mDescHeapUpdate = &PrimitiveController::NonUpdate;
}
