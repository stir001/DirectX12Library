#include "stdafx.h"
#include "ICollisionObject.h"


ICollisionObject::ICollisionObject(int worldID) : mWorldID(worldID), mTag(-1)
{
}


ICollisionObject::~ICollisionObject()
{
	RemoveWorld();
}

int ICollisionObject::GetTag() const
{
	return mTag;
}

int ICollisionObject::GetWorldID() const
{
	return mWorldID;
}
