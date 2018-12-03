#pragma once
#include "bullet\Shape\BulletCollisionShape.h"
#include <DirectXMath.h>

class SphereCollisionShape :
	public BulletCollisionShape
{
public:
	/**
	*	@param[in]	radius	”¼Œa
	*/
	SphereCollisionShape(float radius);
	~SphereCollisionShape();
};

