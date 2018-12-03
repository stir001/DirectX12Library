#pragma once
#include "bullet\Shape\BulletCollisionShape.h"
#include <DirectXMath.h>

class StaticPlaneShape :
	public BulletCollisionShape
{
public:
	/**
	*	@param[in]	constans	原点からの法線方向へのオフセット
	*	@param[in]	normal		法線
	*/
	StaticPlaneShape(float constans
		, const DirectX::XMFLOAT3& normal = DirectX::XMFLOAT3(0.f, 1.f, 0.f));
	~StaticPlaneShape();
};

