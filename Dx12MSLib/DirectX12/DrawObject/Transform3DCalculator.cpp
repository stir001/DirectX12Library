#include "stdafx.h"
#include "Transform3DCalculator.h"
#include "Util/XMFloatOperators.h"

Transform3DCalculator::Transform3DCalculator(): mAMatrix(ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixIdentity()))
{
}

Transform3DCalculator::~Transform3DCalculator()
{
}

void Transform3DCalculator::AddPositon(const DirectX::XMFLOAT3& pos)
{
	mAMatrix = Multiplication(mAMatrix, DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z));
}

void Transform3DCalculator::AddScale(const DirectX::XMFLOAT3& scale)
{
	mAMatrix = Multiplication(mAMatrix, DirectX::XMMatrixScaling(scale.x, scale.y, scale.z));
}

void Transform3DCalculator::AddRotaX(float deg)
{
	mAMatrix = Multiplication(mAMatrix, DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(deg)));
}

void Transform3DCalculator::AddRotaY(float deg)
{
	mAMatrix = Multiplication(mAMatrix, DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(deg)));
}

void Transform3DCalculator::AddRotaZ(float deg)
{
	mAMatrix = Multiplication(mAMatrix, DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(deg)));
}

void Transform3DCalculator::AddRotaQuaternion(const DirectX::XMFLOAT4& quaternion)
{
	DirectX::XMVECTOR q = XMLoadFloat4(&quaternion);
	mAMatrix = Multiplication(mAMatrix, DirectX::XMMatrixRotationQuaternion(q));
}

DirectX::XMFLOAT4X4 Transform3DCalculator::GetAMatrix()
{
	return mAMatrix;
}

void Transform3DCalculator::Init()
{
	mAMatrix = ConvertXMMATRIXToXMFloat4x4(DirectX::XMMatrixIdentity());
}

DirectX::XMFLOAT4X4 Transform3DCalculator::Multiplication(DirectX::XMFLOAT4X4& matF44,const DirectX::XMMATRIX& matXM)
{
	DirectX::XMMATRIX mat = DirectX::XMLoadFloat4x4(&matF44);
	mat *= matXM;
	return ConvertXMMATRIXToXMFloat4x4(mat);
}

