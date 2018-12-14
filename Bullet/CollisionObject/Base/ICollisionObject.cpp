#include "stdafx.h"
#include "ICollisionObject.h"

int operator|(const BulletCollisionState lval, const BulletCollisionState rval)
{
	return static_cast<int>(lval) | static_cast<int>(rval);
}

int operator|(const BulletCollisionState lval, const int rval)
{
	return static_cast<int>(lval) | rval;
}

int operator|(const int lval, const BulletCollisionState rval)
{
	return lval | static_cast<int>(rval);
}

int operator&(const const BulletCollisionState lval, const BulletCollisionState rval)
{
	return static_cast<int>(lval) & static_cast<int>(rval);
}

int operator&(const BulletCollisionState lval, const int rval)
{
	return static_cast<int>(lval) & rval;
}

int operator&(const int lval, const BulletCollisionState rval)
{
	return lval | static_cast<int>(rval);
}

ICollisionObject::ICollisionObject(int worldID) : mWorldID(worldID), mTag1(-1), mTag2(-1)
{
}


ICollisionObject::~ICollisionObject()
{
}

int ICollisionObject::GetTag1() const
{
	return mTag1;
}

int ICollisionObject::GetTag2() const
{
	return mTag2;
}

int ICollisionObject::GetWorldID() const
{
	return mWorldID;
}
