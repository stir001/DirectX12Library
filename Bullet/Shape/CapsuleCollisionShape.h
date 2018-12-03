#pragma once
#include "bullet\Shape\BulletCollisionShape.h"
#include <DirectXMath.h>

class CapsuleCollisionShape :
	public BulletCollisionShape
{
public:
	/**
	*	@param[in]	radius	���a
	*	@param[in]	height	����
	*/
	CapsuleCollisionShape(float radius, float height);
	~CapsuleCollisionShape();
};

