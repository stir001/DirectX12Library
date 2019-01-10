#include "stdafx.h"
#include "SkyBox.h"
#include "CommandList/Dx12CommandList.h"
#include "Master/Dx12Ctrl.h"
#include "Buffer/ConstantBufferObject.h"
#include "Buffer/IndexBufferObject.h"
#include "Buffer/VertexBufferObject.h"
#include "Buffer/ShaderResourceObject.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "Primitive3D/PrimitiveCube.h"
#include "Camera/CameraHolder.h"
#include "Camera/Dx12Camera.h"
#include "Util/XMFloatOperators.h"
#include "Texture/TextureLoader.h"
#include "Texture/TextureObject.h"
#include "PipelineState/SkyBoxPipelineState.h"
#include "Rootsignature/SkyBoxRootSignature.h"
#include <algorithm>

SkyBox::SkyBox(const std::shared_ptr<Dx12CommandList>& cmdList
	, const std::string skyBoxTextures[6], const std::shared_ptr<CameraHolder>& holder)
	: mCmdList(cmdList), mCameraHolder(holder)
{
	for (int i = 0; i < static_cast<int>(Direction::max); ++i)
	{
		mSkyBoxTextures.SetTex(static_cast<SkyBoxTextures::TexPathID>(i), skyBoxTextures[i]);
	}

	Init();
}

SkyBox::SkyBox(const std::shared_ptr<Dx12CommandList>& cmdList
	, const SkyBoxTextures& textures, const std::shared_ptr<CameraHolder>& holder)
	: mCmdList(cmdList), mCameraHolder(holder)
{
	mSkyBoxTextures = textures;

	Init();
}

SkyBox::~SkyBox()
{
}

void SkyBox::Draw()
{
	mCmdList->SetPipelineState(mSkyBoxPipelineState);
	mCmdList->SetGraphicsRootSignature(mSkyBoxRootSignature);
	mCmdList->IASetVertexBuffers({ &mVertexBuffer }, 1);
	mCmdList->IASetIndexBuffer(mIndexBuffer);
	mCmdList->SetDescriptorHeap(mDescHeap);
	mDescHeap->SetGraphicsDescriptorTable(mCmdList, 0, 0);
	mDescHeap->SetGraphicsDescriptorTable(mCmdList, 1, 1);
	unsigned int planeIndexNum = 6;
	unsigned int textureRootParamater = 2;
	mCmdList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (int i = 0; i < static_cast<int>(Direction::max); ++i)
	{
		mDescHeap->SetGraphicsDescriptorTable(mCmdList, i + 2, textureRootParamater);
		mCmdList->DrawIndexedInstanced(planeIndexNum, 1, planeIndexNum * i);
	}
}

void SkyBox::SetCommandList(const std::shared_ptr<Dx12CommandList>& cmdList)
{
	mCmdList = cmdList;
}

void SkyBox::UpdateCameraBuffer()
{
	auto holder = mCameraHolder.lock();
	int cameraNum = holder->GetCameraNum();
	for (int i = 0; i < cameraNum; ++i)
	{
		auto& camera = holder->GetCamera(i);
		mCBufferElement.projections[i] = GetSkyBoxProjection(camera);
	}
	mProjectionBuffer->WriteBuffer256Alignment(&mCBufferElement, sizeof(mCBufferElement), 1);
}

void SkyBox::Init()
{
	SkyBoxVertex v;
	std::shared_ptr<PrimitiveCube> cube = std::make_shared<PrimitiveCube>(mSkyBoxRange);
	auto& vertices = cube->GetVertices();
	mIndices = cube->GetIndices();
	mVertices.resize(vertices.size());
	std::copy(vertices.begin(), vertices.end(), mVertices.begin());
	FixUV();

	auto& device = Dx12Ctrl::Instance().GetDev();

	unsigned int vertexSize = static_cast<unsigned int>(sizeof(mVertices[0]));
	unsigned int vertexNum = static_cast<unsigned int>(mVertices.size());
	mVertexBuffer = std::make_shared<VertexBufferObject>("SkyBoxVertexBuffer", device, vertexSize, vertexNum);
	mVertexBuffer->WriteBuffer(mVertices.data(), vertexSize * vertexNum);

	unsigned int indexSize = static_cast<unsigned int>(sizeof(mIndices[0]));
	unsigned int indexNum = static_cast<unsigned int>(mIndices.size());
	mIndexBuffer = std::make_shared<IndexBufferObject>("SkyBoxIndexBuffer", device, indexSize, indexNum);
	mIndexBuffer->WriteBuffer(mIndices.data(), indexSize * indexNum);
	
	mCameraBuffer = mCameraHolder.lock()->GetCamerasBuffer();
	mProjectionBuffer = std::make_shared<ConstantBufferObject>("SkyBoxConstantBuffer"
		, device, static_cast<unsigned int>(sizeof(mCBufferElement)), 1);
	unsigned int cBufferNum = 2;
	std::vector<std::shared_ptr<Dx12BufferObject>> buffers(static_cast<int>(Direction::max) + cBufferNum);
	buffers[0] = mCameraBuffer;
	buffers[1] = mProjectionBuffer;
	mTextures.resize(static_cast<int>(Direction::max));
	for (int i = 0; i < static_cast<int>(Direction::max); ++i)
	{
		mTextures[i] = TextureLoader::Instance().LoadTexture(mSkyBoxTextures.GetTex(static_cast<SkyBoxTextures::TexPathID>(i)));
		buffers[i + cBufferNum] = mTextures[i]->GetShaderResource();
	}

	mDescHeap = std::make_shared<Dx12DescriptorHeapObject>("SkyBoxDescHeap", device, buffers, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	mSkyBoxRootSignature = std::make_shared<SkyBoxRootSignature>(device);
	mSkyBoxPipelineState = std::make_shared<SkyBoxPipelineState>(mSkyBoxRootSignature, device);
}

DirectX::XMFLOAT4X4 SkyBox::GetSkyBoxProjection(std::shared_ptr<Dx12Camera> camera)
{
	float fov = camera->GetFov();
	auto size = camera->GetViewPortSize();
	auto projection = DirectX::XMMatrixPerspectiveFovLH(fov, static_cast<float>(size.x) / static_cast<float>(size.y), mSkyBoxRange * 0.6f * 0.5f, mSkyBoxRange * 1.8f * 0.5f);
	DirectX::XMFLOAT4X4 rtn = ConvertXMMATRIXToXMFloat4x4(projection);
	return rtn;
}

void SkyBox::FixUV()
{
	//front
	unsigned int planeVertexNum = 4;
	unsigned int vert = 0;
	for (unsigned int i = 0; i < planeVertexNum; ++i, ++vert)
	{
		mVertices[vert].uv.x = 1.0f - mVertices[vert].uv.x;
		mVertices[vert].uv.y = 1.0f - mVertices[vert].uv.y;
	}
	//vert += planeVertexNum;

	//back
	vert += planeVertexNum;

	//right
	for (unsigned int i = 0; i < planeVertexNum; ++i, ++vert)
	{
		float uvx = mVertices[vert].uv.x;
		mVertices[vert].uv.x = 1.0f - mVertices[vert].uv.y;
		mVertices[vert].uv.y = uvx;
	}

	//left
	for (unsigned int i = 0; i < planeVertexNum; ++i, ++vert)
	{
		float uvx = mVertices[vert].uv.x;
		mVertices[vert].uv.x = mVertices[vert].uv.y;
		mVertices[vert].uv.y = 1.0f - uvx;
	}

	//top
	for (unsigned int i = 0; i < planeVertexNum; ++i, ++vert)
	{
		mVertices[vert].uv.x = 1.0f - mVertices[vert].uv.x;
		mVertices[vert].uv.y = 1.0f - mVertices[vert].uv.y;
	}

	//bottom
	for (unsigned int i = 0; i < planeVertexNum; ++i, ++vert)
	{
		mVertices[vert].uv.x = 1.0f - mVertices[vert].uv.x;
		mVertices[vert].uv.y = 1.0f - mVertices[vert].uv.y;
	}
}

SkyBoxVertex SkyBoxVertex::operator=(const PrimitiveVertex& v)
{
	this->pos = v.pos;
	this->uv = {v.uv.x, v.uv.y };
	return *this;
}

std::string SkyBoxTextures::GetTex(TexPathID id) const
{
	std::string rtn;
	switch (id)
	{
	case SkyBoxTextures::Front:
		rtn = this->front;
		break;
	case SkyBoxTextures::Back:
		rtn = this->back;
		break;
	case SkyBoxTextures::Right:
		rtn = this->right;
		break;
	case SkyBoxTextures::Left:
		rtn = this->left;
		break;
	case SkyBoxTextures::Top:
		rtn = this->top;
		break;
	case SkyBoxTextures::Bottom:
		rtn = this->bottom;
		break;
	default:
		break;
	}
	return rtn;
}

void SkyBoxTextures::SetTex(TexPathID id, const std::string & path)
{
	switch (id)
	{
	case SkyBoxTextures::Front:
		front = path;
		break;
	case SkyBoxTextures::Back:
		back = path;
		break;
	case SkyBoxTextures::Right:
		right = path;
		break;
	case SkyBoxTextures::Left:
		left = path;
		break;
	case SkyBoxTextures::Top:
		top = path;
		break;
	case SkyBoxTextures::Bottom:
		bottom = path;
		break;
	default:
		break;
	}
}
