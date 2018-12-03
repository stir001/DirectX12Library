#include "stdafx.h"
#include "BulletlibLink.h"
#include "BoxCollisionShape.h"



BoxCollisionShape::BoxCollisionShape(const DirectX::XMFLOAT3& length)
{
	mCollisionShape = std::make_shared<btBoxShape>(btVector3(length.x * 0.5f, length.y * 0.5f, length.z * 0.5f));
}


BoxCollisionShape::~BoxCollisionShape()
{
}
