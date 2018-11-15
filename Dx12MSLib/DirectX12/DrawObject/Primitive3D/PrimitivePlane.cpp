#include "stdafx.h"
#include "PrimitivePlane.h"
#include "Util/XMFloatOperators.h"

using namespace DirectX;

const unsigned int PLANE_INDICES[] = { 0, 1, 2, 1, 3, 2};

PrimitivePlane::PrimitivePlane(DirectX::XMFLOAT3 p, float len, float hei, DirectX::XMFLOAT3 norm) 
	: PrimitiveObject("PrimitivePlane"), mLength(len), mHeight(hei), mPos(p)
{

	mNormal = NormalizeXMFloat3(norm);

	DirectX::XMVECTOR leftUp = { -mLength / 2.0f, 0, mHeight / 2.0f };
	DirectX::XMVECTOR rightUp = { mLength / 2.0f, 0, mHeight / 2.0f };
	DirectX::XMVECTOR leftDown = { -mLength / 2.0f, 0, -mHeight / 2.0f };
	DirectX::XMVECTOR rightDown = { mLength / 2.0f, 0,  -mHeight / 2.0f };

	DirectX::XMVECTOR center = { p.x, p.y, p.z };

	DirectX::XMFLOAT3 defNorm = { 0, 1, 0 };

	DirectX::XMFLOAT3 inNorm = { mNormal.x, mNormal.y, mNormal.z };

	float cos = DotXMFloat3(defNorm, inNorm);
	float rad = acosf(DotXMFloat3(defNorm, inNorm));

	DirectX::XMMATRIX m = XMMatrixIdentity();

	if (defNorm == -inNorm)
	{
		defNorm = { 0, 0, 1 };

		DirectX::XMFLOAT3 cross = CrossXMFloat3(defNorm, inNorm);

		DirectX::XMVECTOR q = CreateQuoternion(cross, XMConvertToDegrees(rad));

		m = XMMatrixRotationQuaternion(q);
	}
	else if (defNorm != inNorm)
	{
		DirectX::XMFLOAT3 cross = CrossXMFloat3(defNorm, inNorm);

		DirectX::XMVECTOR q = CreateQuoternion(cross, XMConvertToDegrees(rad));

		m = XMMatrixRotationQuaternion(q);
	}

	leftUp = XMVector3Transform(leftUp, m);
	rightUp = XMVector3Transform(rightUp, m);
	leftDown = XMVector3Transform(leftDown, m);
	rightDown = XMVector3Transform(rightDown, m);

	DirectX::XMFLOAT3 luPos;
	XMStoreFloat3(&luPos, leftUp + center);

	DirectX::XMFLOAT3 ruPos;
	XMStoreFloat3(&ruPos, rightUp + center);

	DirectX::XMFLOAT3 ldPos;
	XMStoreFloat3(&ldPos, leftDown + center);

	DirectX::XMFLOAT3 rdPos;
	XMStoreFloat3(&rdPos, rightDown + center);

	unsigned int vertexCount = 4;
	mVertices.reserve(vertexCount);
	DirectX::XMFLOAT2 uv = { 0,0 };
	mVertices.push_back(PrimitiveVertex(luPos, mNormal, uv));

	uv.x = 1;
	mVertices.push_back(PrimitiveVertex(ruPos, mNormal, uv));

	uv.x = 0;
	uv.y = 1;
	mVertices.push_back(PrimitiveVertex(ldPos, mNormal, uv));

	uv.x = 1;
	mVertices.push_back(PrimitiveVertex(rdPos, mNormal, uv));

	unsigned int indicesNum = _countof(PLANE_INDICES);
	mIndices.resize(indicesNum);
	for (unsigned int i = 0; i < indicesNum; ++i)
	{
		mIndices[i] = PLANE_INDICES[i];
	}
}

PrimitivePlane::~PrimitivePlane()
{
}

