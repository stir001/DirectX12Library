#pragma once
#include "bullet\Shape\BulletCollisionShape.h"
#include <DirectXMath.h>

class CylinderCollisionShape :
	public BulletCollisionShape
{
public:
	/**
	*	@param[in]	radius	���a
	*	@param[in]	height	����
	*/
	CylinderCollisionShape(float radius, float height);
	~CylinderCollisionShape();
};

