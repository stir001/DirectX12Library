#pragma once
#include "PrimitiveObject.h"
class PrimitiveCapsule :
	public PrimitiveObject
{
public:
	PrimitiveCapsule(float radius, float length, unsigned int div);
	~PrimitiveCapsule();
};

