#include "stdafx.h"
#include "PrimitiveCone.h"
#include "Util/XMFloatOperators.h"


PrimitiveCone::PrimitiveCone(float radius, float height, unsigned int div)
	: PrimitiveObject("PrimitiveCone")
{
	PrimitiveVertex vert;

	float unitHeight = height / static_cast<float>(div);
	float unitCircleDiv = 3.14159265f * 2.0f / static_cast<float>(div);
	float unitRadius = radius / static_cast<float>(div);
	const float yOffset = -height / 2.0f;

	//法線作成
	std::vector<DirectX::XMFLOAT4> normals(div + 1);
	for (unsigned int i = 0; i < div + 1; ++i)
	{
		float currentRadius = radius;
		DirectX::XMFLOAT4 pos;
		pos.x = sinf(unitCircleDiv * i) * currentRadius;
		pos.z = cosf(unitCircleDiv * i) * currentRadius;
		pos.y = height;

		auto unitPosVec = Normalize({ pos.x, pos.y, pos.z });

		DirectX::XMFLOAT3 upVec = { 0.0f, 1.0f, 0.0f };

		auto projRatio = Dot(unitPosVec, upVec);

		auto projVec = unitPosVec * projRatio;

		auto normalVec = Normalize(projVec - upVec);
		normals[i] = ConvertToXMFloat4(normalVec);
	}

	//コーン上部作成
	const unsigned int roopCount = div + 1;
	for (unsigned int yDiv = 0; yDiv < roopCount; ++yDiv)
	{
		for (unsigned int circleDiv = 0; circleDiv < roopCount; ++circleDiv)
		{
			float currentRadius = unitRadius * yDiv;
			vert.pos.x = sinf(unitCircleDiv * circleDiv) * currentRadius;
			vert.pos.z = cosf(unitCircleDiv * circleDiv) * currentRadius;
			vert.pos.y = unitHeight * yDiv + yOffset;

			vert.normal = normals[circleDiv];

			mVertices.push_back(vert);
		}
	}

	//コーンの底作成
	for (unsigned int circleDiv = 0; circleDiv < roopCount; ++circleDiv)
	{
		float currentRadius = radius;
		vert.pos.x = sinf(unitCircleDiv * circleDiv) * currentRadius;
		vert.pos.z = cosf(unitCircleDiv * circleDiv) * currentRadius;
		vert.pos.y = height + yOffset;

		vert.normal = { 0.0f, 1.0f, 0.0f, 1.0f };

		mVertices.push_back(vert);
	}

	//インデックス作成
	mIndices.resize(roopCount * 6 * (div - 1) + 3 * (roopCount - 1) + (div - 1) * 3);

	//一番上
	unsigned int idx = 0;
	for (unsigned int i = 0; i < roopCount - 1; ++i)
	{
		mIndices[idx++] = i;
		mIndices[idx++] = i + roopCount;
		mIndices[idx++] = i + 1 + roopCount;
	}

	//一番上以外
	for (unsigned int i = 1; i < div; ++i)
	{
		for (unsigned int j = 0; j < div; ++j)
		{
			int offset = i * roopCount;
			mIndices[idx++] = j + offset;
			mIndices[idx++] = j + roopCount + offset;
			mIndices[idx++] = j + 1 + roopCount + offset;

			mIndices[idx++] = j + offset;
			mIndices[idx++] = j + 1 + roopCount + offset;
			mIndices[idx++] = j + 1 + offset;
		}
	}

	//蓋
	unsigned int pivotVertxNum =static_cast<unsigned int>(mVertices.size() - roopCount);
	unsigned int secondVertexNum = pivotVertxNum + 1;
	unsigned int thirdVertexNum = pivotVertxNum + 2;
	for (unsigned int i = 0; i < div - 1; ++i)
	{
		mIndices[idx++] = pivotVertxNum;
		mIndices[idx++] = thirdVertexNum;
		mIndices[idx++] = secondVertexNum;
		
		secondVertexNum = thirdVertexNum;
		++thirdVertexNum;
	}

}


PrimitiveCone::~PrimitiveCone()
{
}
