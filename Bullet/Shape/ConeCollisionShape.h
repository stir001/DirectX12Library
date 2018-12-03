#pragma once
#include "bullet\Shape\BulletCollisionShape.h"
#include <DirectXMath.h>

class ConeCollisionShape :
	public BulletCollisionShape
{
public:
	/**
	*	@param[in]	radius	îºåa
	*	@param[in]	height	çÇÇ≥
	*/
	ConeCollisionShape(float radius, float height);
	~ConeCollisionShape();
};

