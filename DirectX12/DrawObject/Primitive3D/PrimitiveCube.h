#pragma once
#include "PrimitiveObject.h"
#include <vector>

class PrimitivePlane;

class PrimitiveCube :
	public PrimitiveObject
{
public:
	PrimitiveCube(float length);
	~PrimitiveCube();
private:
	float mLength;
	DirectX::XMFLOAT3 mPos;

	void InitCube();
	void StoreVertex(unsigned int vArrayIndexOffset, const std::shared_ptr<PrimitivePlane>& plane);
	void StoreIndex(unsigned int iArrayIndexOffset, unsigned int vArrayIndexOffset, const std::shared_ptr<PrimitivePlane>& plane);
	//vArrayIndexOffsetとiArrayIndexOffsetを内部で変更するので参照をとっている
	void StoreInfo(const std::shared_ptr<PrimitivePlane>& plane, unsigned int& vArrayIndexOffset, unsigned int& iArrayIndexOffset);
};

