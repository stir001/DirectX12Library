#include "stdafx.h"
#include "PrimitiveCube.h"
#include "PrimitivePlane.h"

unsigned int PLANE_VAERTEX_NUM = 4;
unsigned int PLANE_NUM = 6;
unsigned int PLANE_INDICES_NUM = 6;

PrimitiveCube::PrimitiveCube(float length): PrimitiveObject("PrimitiveCube") , mLength(length), mPos(0,0,0)
{
	InitCube();
}

PrimitiveCube::~PrimitiveCube()
{
}

void PrimitiveCube::InitCube()
{
	mVertices.resize(PLANE_NUM * PLANE_VAERTEX_NUM);
	mIndices.resize(PLANE_NUM * PLANE_INDICES_NUM);

	unsigned int vArrayIndexOffset = 0;
	unsigned int iArrayIndexOffset = 0;
	std::shared_ptr<PrimitivePlane> plane1(std::make_shared<PrimitivePlane>(DirectX::XMFLOAT3(mPos.x, mPos.y, mPos.z + mLength * 0.5f), mLength, mLength, DirectX::XMFLOAT3(0, 0, 1)));
	StoreInfo(plane1, vArrayIndexOffset, iArrayIndexOffset);
	
	std::shared_ptr<PrimitivePlane> plane2(std::make_shared<PrimitivePlane>(DirectX::XMFLOAT3(mPos.x, mPos.y, mPos.z - mLength * 0.5f), mLength, mLength, DirectX::XMFLOAT3(0, 0, -1)));
	StoreInfo(plane2, vArrayIndexOffset, iArrayIndexOffset);

	std::shared_ptr<PrimitivePlane> plane3(std::make_shared<PrimitivePlane>(DirectX::XMFLOAT3(mPos.x + mLength * 0.5f, mPos.y, mPos.z), mLength, mLength, DirectX::XMFLOAT3(1, 0, 0)));
	StoreInfo(plane3, vArrayIndexOffset, iArrayIndexOffset);

	std::shared_ptr<PrimitivePlane> plane4(std::make_shared<PrimitivePlane>(DirectX::XMFLOAT3(mPos.x - mLength * 0.5f, mPos.y, mPos.z), mLength, mLength, DirectX::XMFLOAT3(-1, 0, 0)));
	StoreInfo(plane4, vArrayIndexOffset, iArrayIndexOffset);
	
	std::shared_ptr<PrimitivePlane> plane5(std::make_shared<PrimitivePlane>(DirectX::XMFLOAT3(mPos.x, mPos.y + mLength * 0.5f, mPos.z), mLength, mLength, DirectX::XMFLOAT3(0, 1, 0)));
	StoreInfo(plane5, vArrayIndexOffset, iArrayIndexOffset);
	
	std::shared_ptr<PrimitivePlane> plane6(std::make_shared<PrimitivePlane>(DirectX::XMFLOAT3(mPos.x, mPos.y - mLength * 0.5f, mPos.z), mLength, mLength, DirectX::XMFLOAT3(0, -1, 0)));
	StoreInfo(plane6, vArrayIndexOffset, iArrayIndexOffset);
}

void PrimitiveCube::StoreVertex(unsigned int vArrayIndexOffset, const std::shared_ptr<PrimitivePlane>& plane)
{
	auto& v = plane->GetVertices();
	for (unsigned int i = 0; i < v.size(); ++i)
	{
		mVertices[vArrayIndexOffset + i] = v[i];
	}
}

void PrimitiveCube::StoreIndex(unsigned int iArrayIndexOffset, unsigned int vArrayIndexOffset, const std::shared_ptr<PrimitivePlane>& plane)
{
	auto& idx = plane->GetIndices();
	for (unsigned int i = 0; i < idx.size(); ++i)
	{
		mIndices[iArrayIndexOffset + i] = idx[i] + vArrayIndexOffset;
	}
}

void PrimitiveCube::StoreInfo(const std::shared_ptr<PrimitivePlane>& plane, unsigned int& vArrayIndexOffset, unsigned int& iArrayIndexOffset)
{
	StoreVertex(vArrayIndexOffset, plane);
	StoreIndex(iArrayIndexOffset, vArrayIndexOffset, plane);
	vArrayIndexOffset += PLANE_VAERTEX_NUM;
	iArrayIndexOffset += PLANE_INDICES_NUM;
}
