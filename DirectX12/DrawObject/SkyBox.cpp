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
#include <algorithm>

SkyBox::SkyBox(const std::shared_ptr<Dx12CommandList>& cmdList, const std::string skyBoxTextures[6])
	: mCmdList(cmdList), mCameraHolder(Dx12Ctrl::Instance().GetCameraHolder())
{
	for (int i = 0; i < static_cast<int>(Direction::max); ++i)
	{
		mSkyBoxTextures.SetTex(static_cast<SkyBoxTextures::TexPathID>(i), skyBoxTextures[i]);
	}
}

SkyBox::SkyBox(const std::shared_ptr<Dx12CommandList>& cmdList, const SkyBoxTextures& textures)
	: mCmdList(cmdList)
{
	mSkyBoxTextures = textures;
}

SkyBox::~SkyBox()
{
}

void SkyBox::Draw()
{
	UpdateCameraBuffer();
	mCmdList->SetPipelineState(mSkyBoxPipelineState);
	mCmdList->SetGraphicsRootSignature(mSkyBoxRootSignature);
	mCmdList->IASetVertexBuffers({ &mVertexBuffer }, 1);
	mCmdList->IASetIndexBuffer(mIndexBuffer);
	mCmdList->SetDescriptorHeap(mDescHeap);
	
	unsigned int planeIndexNum = 6;
	unsigned int textureRootParamater = 1;
	for (int i = 1; i < static_cast<int>(Direction::max) + 1; ++i)
	{
		mDescHeap->SetGraphicsDescriptorTable(mCmdList, i, textureRootParamater);
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

	auto& device = Dx12Ctrl::Instance().GetDev();

	mVertexBuffer = std::make_shared<VertexBufferObject>("SkyBoxVertexBuffer", device, static_cast<unsigned int>(sizeof(mVertices[0])), static_cast<unsigned int>(mVertices.size()));
	mIndexBuffer = std::make_shared<IndexBufferObject>("SkyBoxIndexBuffer", device, static_cast<unsigned int>(sizeof(mIndices[0])), static_cast<unsigned int>(mIndices.size()));
	
	mCameraBuffer = mCameraHolder.lock()->GetCamerasBuffer();

	std::vector<std::shared_ptr<Dx12BufferObject>> buffers(static_cast<int>(Direction::max) + 1);
	buffers[0] = mCameraBuffer;
	for (int i = 1; i < static_cast<int>(Direction::max) + 1; ++i)
	{
		mTextures[i] = TextureLoader::Instance().LoadTexture(mSkyBoxTextures.GetTex(static_cast<SkyBoxTextures::TexPathID>(i)));
		buffers[i] = mTextures[i]->GetShaderResource();
	}

	mDescHeap = std::make_shared<Dx12DescriptorHeapObject>("SkyBoxDescHeap", device, buffers, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	mProjectionBuffer = std::make_shared<ConstantBufferObject>("SkyBoxConstantBuffer"
		, device, static_cast<unsigned int>(sizeof(mCBufferElement)), 1);
}

DirectX::XMFLOAT4X4 SkyBox::GetSkyBoxProjection(std::shared_ptr<Dx12Camera> camera)
{
	float fov = camera->GetFov();
	auto size = camera->GetViewPortSize();
	auto projection = DirectX::XMMatrixPerspectiveFovLH(fov, static_cast<float>(size.x) / static_cast<float>(size.y), mSkyBoxRange * 0.95f, mSkyBoxRange * 1.5f);
	DirectX::XMFLOAT4X4 rtn = ConvertXMMATRIXToXMFloat4x4(projection);
	return rtn;
}

SkyBoxVertex SkyBoxVertex::operator=(const PrimitiveVertex & v)
{
	this->pos = v.pos;
	this->uv = {(1 - v.uv.x), uv.y };
	return *this;
}

//SkyBoxVertex SkyBoxVertex::operator=(const PrimitiveVertex v)
//{
//	this->pos = v.pos;
//	this->uv = { (1 - v.uv.x), uv.y };
//	return *this;
//}

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
