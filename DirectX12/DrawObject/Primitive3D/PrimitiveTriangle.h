#pragma once
#include "PrimitiveObject.h"
class PrimitiveTriangle :
	public PrimitiveObject
{
public:
	PrimitiveTriangle(const DirectX::XMFLOAT3& pos1, const DirectX::XMFLOAT3& pos2, const DirectX::XMFLOAT3& pos3);
	~PrimitiveTriangle();
};

