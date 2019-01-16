#include "stdafx.h"

#include "CylinderCollisionShape.h"
#include <btBulletDynamicsCommon.h>


CylinderCollisionShape::CylinderCollisionShape(float radius, float height)
{
	mCollisionShape = std::make_shared<btCylinderShape>(btVector3(radius, height, radius));
}


CylinderCollisionShape::~CylinderCollisionShape()
{
}
