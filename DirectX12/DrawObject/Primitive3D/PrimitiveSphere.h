#pragma once
#include "PrimitiveObject.h"

class PrimitiveSphere :
	public PrimitiveObject
{
public:
	PrimitiveSphere(float radius, unsigned int div);
	~PrimitiveSphere();

private:
	void CreateVertices(float radius, unsigned int div);
	void CreateIndices(unsigned int div);
};

