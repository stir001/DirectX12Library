#pragma once
#include "bullet\Shape\BulletCollisionShape.h"
#include <DirectXMath.h>

class CapsuleCollisionShape :
	public BulletCollisionShape
{
public:
	/**
	*	@param[in]	radius	îºåa
	*	@param[in]	height	çÇÇ≥
	*/
	CapsuleCollisionShape(float radius, float height);
	~CapsuleCollisionShape();
};

