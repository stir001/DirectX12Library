#include "stdafx.h"

#include "SphereCollisionShape.h"
#include <btBulletDynamicsCommon.h>


SphereCollisionShape::SphereCollisionShape(float radius)
{
	mCollisionShape = std::make_shared<btSphereShape>(radius);
}


SphereCollisionShape::~SphereCollisionShape()
{
}
