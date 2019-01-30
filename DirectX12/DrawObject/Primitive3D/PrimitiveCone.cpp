#include "stdafx.h"
#include "PrimitiveCone.h"


PrimitiveCone::PrimitiveCone()
	: PrimitiveObject("PrimitiveCone")
{
	float radius = 50.0f;
	float height = 40.0f;
	unsigned int div = 10;

	PrimitiveVertex vert;

	float unitHeight = height / static_cast<float>(div);
	float unitCircleDiv = 3.14159265f * 2.0f / static_cast<float>(div);
	float unitRadius = 10.0f / static_cast<float>(div);


	const unsigned int roopCount = div + 1;
	for (unsigned int yDiv = 0; yDiv < roopCount; ++yDiv)
	{
		for (unsigned int circleDiv = 0; circleDiv < roopCount; ++circleDiv)
		{
			float currentRadius = unitRadius * yDiv;
			vert.pos.x = sinf(unitRadius * circleDiv) * currentRadius;
			vert.pos.z = cosf(unitRadius * circleDiv) * currentRadius;
			vert.pos.y = unitHeight * yDiv;
			mVertices.push_back(vert);
		}
	}

	//インデックス作成
	mIndices.resize(roopCount * 6 * (div - 1) + 3 * roopCount);

	//一番上
	unsigned int idx = 0;
	for (unsigned int i = 0; i < roopCount - 1; ++i)
	{
		mIndices[idx++] = i;
		mIndices[idx++] = i + roopCount;
		mIndices[idx++] = i + 1 + roopCount;
	}

	////一番上以外
	//for (int i = 1, idx = roopCount; i < div - 1; ++i)
	//{
	//	for (int j = 0; j < roopCount; ++j)
	//	{
	//		int offset = i * roopCount;
	//		mIndices[idx++] = j + offset;
	//		mIndices[idx++] = j + roopCount + offset;
	//		mIndices[idx++] = j + 1 + roopCount + offset;

	//		mIndices[idx++] = j + roopCount + offset;
	//		mIndices[idx++] = j + 1 + offset;
	//		mIndices[idx++] = j + 1 + roopCount + offset;
	//	}
	//}

}


PrimitiveCone::~PrimitiveCone()
{
}
