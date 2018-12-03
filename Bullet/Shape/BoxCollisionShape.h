#pragma once
#include "bullet\Shape\BulletCollisionShape.h"
#include <DirectXMath.h>

class BoxCollisionShape :
	public BulletCollisionShape
{
public:
	/**
	*	@param[in]	length	•Ó‚Ì’·‚³
	*/
	BoxCollisionShape(const DirectX::XMFLOAT3& length);
	~BoxCollisionShape();
};

