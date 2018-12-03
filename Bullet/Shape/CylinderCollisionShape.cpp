#include "stdafx.h"
#include "BulletlibLink.h"
#include "CylinderCollisionShape.h"



CylinderCollisionShape::CylinderCollisionShape(float radius, float height)
{
	mCollisionShape = std::make_shared<btCylinderShape>(btVector3(radius, height, radius));
}


CylinderCollisionShape::~CylinderCollisionShape()
{
}
