#pragma once
#include "bullet\Shape\BulletCollisionShape.h"
#include <DirectXMath.h>

class ConeCollisionShape :
	public BulletCollisionShape
{
public:
	/**
	*	@param[in]	radius	���a
	*	@param[in]	height	����
	*/
	ConeCollisionShape(float radius, float height);
	~ConeCollisionShape();
};

