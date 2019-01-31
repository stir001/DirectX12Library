#pragma once
#include "PrimitiveObject.h"

class PrimitiveCone : public PrimitiveObject
{
public:
	PrimitiveCone(float radius, float height, unsigned int div);
	~PrimitiveCone();
};

