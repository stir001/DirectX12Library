#include "stdafx.h"
#include "PrimitiveController.h"
#include "PrimitiveObject.h"
#include "Buffer/VertexBufferObject.h"
#include "Buffer/IndexBufferObject.h"
#include "Buffer/ConstantBufferObject.h"
#include "Buffer/ShaderResourceObject.h"
#include "Util/XMFloatOperators.h"
#include "Camera/Dx12Camera.h"
#include "Texture/TextureObject.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "PipelineState/PipelineStateObject.h"
#include "Rootsignature/RootSignatureObject.h"
#include "Light/LightObject.h"
#include "Master/Dx12Ctrl.h"


PrimitiveController::PrimitiveController(std::shared_ptr<PrimitiveObject> primitive
	, Microsoft::WRL::ComPtr<ID3D12Device>& dev
	, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList
	, std::shared_ptr<Dx12Camera>& camera)
	: DrawController3D(primitive->GetName(),dev, cmdList)
	, mPrimitive(primitive)
	, mInstanceUpdate(&PrimitiveController::UpdateInstanceVertexBuffer)
{
	const auto& indices = primitive->GetIndices();
	mIndexBuffer.reset(new IndexBufferObject(primitive->GetName() + "IndexBuffer", dev, sizeof(indices[0]), static_cast<unsigned int>(indices.size())));
	mIndexBuffer->WriteBuffer(primitive->GetIndices().data(), static_cast<unsigned int>(sizeof(indices[0]) * indices.size()));

	const auto& vertices = primitive->GetVertices();
	mVertexBuffer.reset(new VertexBufferObject(primitive->GetName() + "VertexBuffer", dev, sizeof(vertices[0]), static_cast<unsigned int>(vertices.size())));
	mVertexBuffer->WriteBuffer(vertices.data(),static_cast<unsigned int>(sizeof(vertices[0]) * vertices.size()));

	InstanceDatas data;
	DirectX::XMStoreFloat4x4(&data.aMatrix, DirectX::XMMatrixIdentity());
	data.offset = DirectX::XMFLOAT4(0, 0, 0, 1);
	mInstanceDatas.push_back(data);
	mInstanceVertexBuffer.reset(new VertexBufferObject(primitive->GetName() + "InstanceVertexBuffer", dev, sizeof(mInstanceDatas[0]), 1));

	mInstanceUpdate = &PrimitiveController::UpdateInstanceVertexBuffer;
	mDescHeapCreate = &PrimitiveController::CreateDescHeap;
}


PrimitiveController::~PrimitiveController()
{
}

void PrimitiveController::SetTexture(std::shared_ptr<TextureObject>& texture)
{
	mTexObj = texture;
	mDescHeapCreate = &PrimitiveController::CreateDescHeap;
}

void PrimitiveController::SetLightBuffer(std::shared_ptr<LightObject>& light)
{
	mLightBuffer = light->GetLightBuffer();
	mDescHeapCreate = &PrimitiveController::CreateDescHeap;
}

void PrimitiveController::Instancing(std::vector<DirectX::XMFLOAT3>& instancePositions)
{
	mInstanceDatas.clear();
	mInstanceDatas.resize(instancePositions.size());
	DirectX::XMFLOAT4X4 mat;
	DirectX::XMStoreFloat4x4(&mat, DirectX::XMMatrixIdentity());
	for (unsigned int i = 0; i < static_cast<unsigned int>(instancePositions.size()); ++i)
	{
		mInstanceDatas[i].aMatrix = mat;
		mInstanceDatas[i].offset = ConvertXMFloat3ToXMFloat4(instancePositions[i]);
	}

	mInstanceVertexBuffer.reset(new VertexBufferObject(mPrimitive->GetName() + "InstanceVertexBuffer", Dx12Ctrl::Instance().GetDev(), sizeof(mInstanceDatas[0]), static_cast<unsigned int>(mInstanceDatas.size())));
	mInstanceUpdate = &PrimitiveController::UpdateInstanceVertexBuffer;
}

void PrimitiveController::SetInstancingMatrix(std::vector<DirectX::XMFLOAT4X4>& matrix, unsigned int startIndex, unsigned int endIndex)
{
	for (unsigned int i = 0; i <= endIndex - startIndex; ++i)
	{
		mInstanceDatas[startIndex + i].aMatrix = matrix[i];
	}
	mInstanceUpdate = &PrimitiveController::UpdateInstanceVertexBuffer;
}

void PrimitiveController::Draw()
{
	(this->*mInstanceUpdate)();
	(this->*mDescHeapCreate)();

	mCmdList->SetPipelineState(mPipelinestate->GetPipelineState().Get());
	mCmdList->SetGraphicsRootSignature(mRootsignature->GetRootSignature().Get());
	mCmdList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	const D3D12_VERTEX_BUFFER_VIEW vbViews[] = { mVertexBuffer->GetView(), mInstanceVertexBuffer->GetView() };
	mCmdList->IASetVertexBuffers(0, 2, vbViews);
	mIndexBuffer->SetBuffer(mCmdList);
	mDescHeap->SetDescriptorHeap(mCmdList);
	unsigned int resourceIndex = 0;
	mDescHeap->SetGprahicsDescriptorTable(mCmdList, resourceIndex++, eROOT_PARAMATER_INDEX_TEXTURE);
	mDescHeap->SetGprahicsDescriptorTable(mCmdList, resourceIndex++, eROOT_PARAMATER_INDEX_CAMERA);
	mDescHeap->SetGprahicsDescriptorTable(mCmdList, resourceIndex++, eROOT_PARAMATER_INDEX_LIGHT);

	mCmdList->DrawIndexedInstanced(static_cast<UINT>(mPrimitive->GetIndices().size()), static_cast<UINT>(mInstanceDatas.size()), 0, 0, 0);
}

void PrimitiveController::UpdateInstanceVertexBuffer()
{
	mInstanceVertexBuffer->WriteBuffer(&mInstanceDatas[0], static_cast<unsigned int>(sizeof(mInstanceDatas[0]) * mInstanceDatas.size()));
	mInstanceUpdate = &PrimitiveController::NonUpdate;
}

void PrimitiveController::NonUpdate()
{
}

void PrimitiveController::CreateDescHeap()
{
	std::vector<std::shared_ptr<Dx12BufferObject>> buffers(eROOT_PARAMATER_INDEX_MAX);
	buffers[eROOT_PARAMATER_INDEX_TEXTURE] = mTexObj->GetShaderResource();
	buffers[eROOT_PARAMATER_INDEX_CAMERA] = mCameraBuffer;
	buffers[eROOT_PARAMATER_INDEX_LIGHT] = mLightBuffer;

	mDescHeap.reset(new Dx12DescriptorHeapObject(mPrimitive->GetName() + "DescriptorHeap", Dx12Ctrl::Instance().GetDev(), buffers, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	mDescHeapCreate = &PrimitiveController::NonUpdate;
}
