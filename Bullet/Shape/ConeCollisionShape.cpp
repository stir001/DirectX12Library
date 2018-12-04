#include "stdafx.h"
#include "BulletlibLink.h"
#include "ConeCollisionShape.h"
#include <btBulletDynamicsCommon.h>


ConeCollisionShape::ConeCollisionShape(float radius, float height)
{
	mCollisionShape = std::make_shared<btConeShape>(radius, height);
}


ConeCollisionShape::~ConeCollisionShape()
{
}
