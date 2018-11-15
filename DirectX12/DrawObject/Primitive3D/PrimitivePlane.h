#pragma once
#include "PrimitiveObject.h"
#include <DirectXMath.h>

class PrimitivePlane :
	public PrimitiveObject
{
public:
	PrimitivePlane(DirectX::XMFLOAT3 pos, float length, float height, DirectX::XMFLOAT3 norm);
	~PrimitivePlane();

private:
	DirectX::XMFLOAT3 mPos;//íÜêSç¿ïW
	float mLength;
	float mHeight;
	DirectX::XMFLOAT3 mNormal;
	DirectX::XMFLOAT4X4 mMatrix;
};