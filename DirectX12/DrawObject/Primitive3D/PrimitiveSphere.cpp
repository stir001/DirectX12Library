#include "stdafx.h"
#include "PrimitiveSphere.h"
#include "Util/XMFloatOperators.h"

PrimitiveSphere::PrimitiveSphere(float radius, unsigned int div): PrimitiveObject("PrimitiveSphere")
{
	CreateVertices(radius, div);
	CreateIndices(div);
}


PrimitiveSphere::~PrimitiveSphere()
{
}

void PrimitiveSphere::CreateVertices(float radius, unsigned int div)
{
	PrimitiveVertex vert;
	vert.pos = { 0.f,radius, 0.f, 1.f };
	vert.normal = { 0.f, 1.0f, 0.0f, 1.0f };
	const DirectX::XMFLOAT3 center = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 vertPos = { vert.pos.x, vert.pos.y,vert.pos.z };
	mVertices.reserve(div * (div + 1));

	const float unitYDiv = DirectX::XM_PI / div;
	const float unitUDiv = 1.0f / div;
	const float unitVDiv = 1.0f / div;
	const float unitRadian = DirectX::XM_PI * 2.0f / div;

	const unsigned int circleVertNum = div + 1;

	//y•ûŒü•ªŠ„
	for (unsigned int yDiv = 0; yDiv < circleVertNum; ++yDiv)
	{
		//‰~‚Ì•ªŠ„
		for (unsigned int circleDiv = 0; circleDiv < circleVertNum; ++circleDiv)
		{
			float rRatio = sin(unitRadian * yDiv * 0.5f);
			float pi = unitRadian * circleDiv;
			vertPos = { radius * sin(pi) * rRatio,
						radius * cos(unitYDiv * yDiv),
						radius * cos(pi) * rRatio };
			vert.pos = { vertPos.x, vertPos.y, vertPos.z, 1.0f };
			DirectX::XMFLOAT3 v = Normalize(vertPos - center);
			vert.normal = { v.x, v.y, v.z, 1.0f };
			vert.uv = { unitUDiv * circleDiv, unitVDiv * yDiv };
			mVertices.push_back(vert);
		}
	}
}

void PrimitiveSphere::CreateIndices(unsigned int div)
{
	unsigned int indexNo = 0;
	mIndices.reserve(div * 2 + (div - 2) * 2);
	const unsigned int circleVertNum = div + 1;
	//ˆê”Ôã‚¾‚¯
	for (unsigned int circleDiv = 0; circleDiv < div; ++circleDiv)
	{
			mIndices.push_back(circleDiv + 0);
			mIndices.push_back(circleDiv + (0 + 1) * circleVertNum + 1);
			mIndices.push_back(circleDiv + (0 + 1) * circleVertNum);
	}

	//ˆê”Ôã‚Æˆê”Ô‰ºˆÈŠO
	for (unsigned int yDiv = 1; yDiv < div - 1; ++yDiv)
	{
		for (unsigned int circleDiv = 0; circleDiv < circleVertNum - 1; ++circleDiv)
		{
			mIndices.push_back(circleDiv + yDiv * circleVertNum);
			mIndices.push_back(circleDiv + (yDiv + 1) * circleVertNum + 1);
			mIndices.push_back(circleDiv + (yDiv + 1) * circleVertNum);

			mIndices.push_back(circleDiv + yDiv * circleVertNum);
			mIndices.push_back(circleDiv + yDiv * circleVertNum + 1);
			mIndices.push_back(circleDiv + (yDiv + 1) * circleVertNum + 1);
		}
	}

	//ˆê”Ô‰º‚¾‚¯
	for (unsigned int circleDiv = 0; circleDiv < div; ++circleDiv)
	{
		mIndices.push_back(circleDiv + (div - 1) * circleVertNum);
		mIndices.push_back(circleDiv + (div - 1) * circleVertNum + 1);
		mIndices.push_back(circleDiv + (div + 0) * circleVertNum + 1);
	}
}
