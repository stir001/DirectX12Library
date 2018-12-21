#include "stdafx.h"
#include "DrawController3D.h"
#include "Buffer/ConstantBufferObject.h"
#include "Rootsignature/SkeletonRootSignature.h"
#include "PipelineState/SkeletonPipelineState.h"
#include "Camera/CameraHolder.h"
#include "Camera/Dx12Camera.h"
#include "Master/Dx12Ctrl.h"

using namespace DirectX;

DrawController3D::DrawController3D(const std::string& modelName, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList): 
	DrawObjectController(modelName +"Bundle", dev, cmdList)
	, mScale{ 1.0f, 1.0f, 1.0f }, mPos(0, 0, 0)
	,mQuaternion{0,0,0,1}, mSkeletonColor(1.0f,1.0f,1.0f,1.0f)
	,mSkeletonRootsignature(std::make_shared<SkeletonRootSignature>(dev))
	,mSkeletonPipelineState(std::make_shared<SkeletonPipelineState>(mSkeletonRootsignature, dev))
{
	DirectX::XMStoreFloat4x4(&mModelMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&mRotationMatrix, DirectX::XMMatrixIdentity());
	std::string cbufferName = modelName;
	cbufferName += "MatrixBuffer";
	mModelMatrixBuffer = std::make_shared<ConstantBufferObject>(cbufferName, dev, static_cast<unsigned int>(sizeof(mModelMatrix)), 1U);
	cbufferName = modelName + "SkeltonColorBuffer";
	mSkeletonColorConstantBuffer = std::make_shared<ConstantBufferObject>(cbufferName, dev, static_cast<unsigned int>(sizeof(mSkeletonColor)), 1U);
	Dx12Ctrl::Instance().GetCameraHolder()->SetCameraBuffer(this);
	UpdateSkeltonColor();
	UpdateMatrix();
}

DrawController3D::~DrawController3D()
{
}

void DrawController3D::SetCameraBuffer(std::shared_ptr<ConstantBufferObject> cameraBuffer)
{
	mCameraBuffer = cameraBuffer;
}

void DrawController3D::SetPosition(const DirectX::XMFLOAT3& pos)
{
	mPos = pos;
	UpdateMatrix();
}

void DrawController3D::SetScale(float scale)
{
	mScale = { scale,scale,scale };
	UpdateMatrix();
}

void DrawController3D::SetScale(float scaleX, float scaleY, float scaleZ)
{
	mScale = { scaleX,scaleY,scaleZ };
	UpdateMatrix();
}

void  DrawController3D::AddRotaX(float deg)
{
	DirectX::XMStoreFloat4x4(&mRotationMatrix, XMLoadFloat4x4(&mRotationMatrix) * DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(deg)));
	UpdateMatrix();
}

void  DrawController3D::AddRotaY(float deg)
{
	DirectX::XMStoreFloat4x4(&mRotationMatrix, XMLoadFloat4x4(&mRotationMatrix) * DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(deg)));
	UpdateMatrix();
}

void  DrawController3D::AddRotaZ(float deg)
{
	DirectX::XMStoreFloat4x4(&mRotationMatrix, XMLoadFloat4x4(&mRotationMatrix) * DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(deg)));
	UpdateMatrix();
}

void DrawController3D::SetRotaQuaternion(const DirectX::XMFLOAT4& quaternion)
{
	mQuaternion = quaternion;
	UpdateMatrix();
}

void DrawController3D::SetSkeletonColor(DirectX::XMFLOAT4 & color)
{
	mSkeletonColor = color;
	UpdateSkeltonColor();
}

void DrawController3D::SetSkeletonColor(float r, float g, float b, float a)
{
	mSkeletonColor = {r, g, b, a};
	UpdateSkeltonColor();
}

void DrawController3D::UpdateMatrix()
{
	XMMATRIX mat = XMMatrixIdentity();
	XMVECTOR q = XMLoadFloat4(&mQuaternion);
	mat *= DirectX::XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	mat *= DirectX::XMMatrixRotationQuaternion(q);
	mat *= DirectX::XMLoadFloat4x4(&mRotationMatrix);
	mat *= DirectX::XMMatrixTranslation(mPos.x, mPos.y, mPos.z);
	DirectX::XMStoreFloat4x4(&mModelMatrix, mat);

	mModelMatrixBuffer->WriteBuffer256Alignment(&mModelMatrix, static_cast<unsigned int>(sizeof(mModelMatrix)), 1U);
}

void DrawController3D::UpdateSkeltonColor()
{
	mSkeletonColorConstantBuffer->WriteBuffer256Alignment(&mSkeletonColor, static_cast<unsigned int>(sizeof(mSkeletonColor)), 1U);
}

