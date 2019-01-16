#include "stdafx.h"

#include "BulletCollisionShape.h"



BulletCollisionShape::BulletCollisionShape()
{
}

BulletCollisionShape::BulletCollisionShape(std::shared_ptr<btCollisionShape> collision)
	: mCollisionShape(collision)
{
}


BulletCollisionShape::~BulletCollisionShape()
{
}

std::shared_ptr<btCollisionShape> BulletCollisionShape::GetShape() const
{
	return mCollisionShape;
}