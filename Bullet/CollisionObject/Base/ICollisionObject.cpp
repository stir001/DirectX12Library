#include "stdafx.h"
#include "ICollisionObject.h"


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
