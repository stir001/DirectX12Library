#include "stdafx.h"
#include "PrimitiveTriangle.h"
#include "Util/XMFloatOperators.h"

PrimitiveTriangle::PrimitiveTriangle(const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2, const DirectX::XMFLOAT3& pos3)
	: PrimitiveObject("PrimitiveTriangle")
{
	DirectX::XMFLOAT3 p1Top2Vec = pos2 - pos1;
	DirectX::XMFLOAT3 p1Top3Vec = pos3 - pos1;

	auto normal = Normalize(Cross(p1Top2Vec, p1Top3Vec));

	mVertices.resize(3);
	PrimitiveVertex vert;
	vert.pos = ConvertToXMFloat4(pos1);
	vert.normal = ConvertToXMFloat4(normal);
	mVertices[0] = vert;

	vert.pos = ConvertToXMFloat4(pos2);
	mVertices[1] = vert;

	vert.pos = ConvertToXMFloat4(pos3);
	mVertices[2] = vert;

	mIndices = { 0,2,1 };
}


PrimitiveTriangle::~PrimitiveTriangle()
{
}
