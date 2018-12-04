#include "stdafx.h"
#include "BulletlibLink.h"
#include "StaticPlaneShape.h"
#include <btBulletDynamicsCommon.h>


StaticPlaneShape::StaticPlaneShape(float constans
	, const DirectX::XMFLOAT3& normal)
{
	mCollisionShape = std::make_shared<btStaticPlaneShape>(btVector3(normal.x, normal.y, normal.z), constans);
}


StaticPlaneShape::~StaticPlaneShape()
{
}
