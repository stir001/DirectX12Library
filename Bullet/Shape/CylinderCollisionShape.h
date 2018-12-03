#pragma once
#include "bullet\Shape\BulletCollisionShape.h"
#include <DirectXMath.h>

class CylinderCollisionShape :
	public BulletCollisionShape
{
public:
	/**
	*	@param[in]	radius	îºåa
	*	@param[in]	height	çÇÇ≥
	*/
	CylinderCollisionShape(float radius, float height);
	~CylinderCollisionShape();
};

