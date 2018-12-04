#include "stdafx.h"
#include "BulletlibLink.h"
#include "CapsuleCollisionShape.h"
#include <btBulletDynamicsCommon.h>


CapsuleCollisionShape::CapsuleCollisionShape(float radius, float height)
{
	mCollisionShape = std::make_shared<btCapsuleShape>(radius, height);
}


CapsuleCollisionShape::~CapsuleCollisionShape()
{
}
