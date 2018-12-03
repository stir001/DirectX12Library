#pragma once
#include "bullet\Shape\BulletCollisionShape.h"
#include <DirectXMath.h>

class BoxCollisionShape :
	public BulletCollisionShape
{
public:
	/**
	*	@param[in]	length	�ӂ̒���
	*/
	BoxCollisionShape(const DirectX::XMFLOAT3& length);
	~BoxCollisionShape();
};

