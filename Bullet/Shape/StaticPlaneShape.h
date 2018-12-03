#pragma once
#include "bullet\Shape\BulletCollisionShape.h"
#include <DirectXMath.h>

class StaticPlaneShape :
	public BulletCollisionShape
{
public:
	/**
	*	@param[in]	constans	���_����̖@�������ւ̃I�t�Z�b�g
	*	@param[in]	normal		�@��
	*/
	StaticPlaneShape(float constans
		, const DirectX::XMFLOAT3& normal = DirectX::XMFLOAT3(0.f, 1.f, 0.f));
	~StaticPlaneShape();
};

