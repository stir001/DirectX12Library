#pragma once
#include "DirectX12/DrawObject/DrawObjectLoader.h"

#include <list>
#include <DirectXMath.h>
#include <memory>

class IPrimitive2D;
class Primitive2DLine;

class Primitive2DManager : public DrawObjectLoader
{
public:
	~Primitive2DManager();

	static Primitive2DManager& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new Primitive2DManager();
		}

		return *mInstance;
	}

	static void Destory()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
		}
		mInstance = nullptr;
	}

	std::shared_ptr<Primitive2DLine> CreatePrimitive2DLine(const DirectX::XMFLOAT3& point1, const DirectX::XMFLOAT3& point2);
private:
	Primitive2DManager();
	Primitive2DManager(const Primitive2DManager&);
	Primitive2DManager(const Primitive2DManager&&);
	Primitive2DManager& operator = (const Primitive2DManager&);
	Primitive2DManager* operator = (const Primitive2DManager*);


	void CreatePipelineState(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	void CreateRootsignature(Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	
	static Primitive2DManager* mInstance;
	std::list<std::shared_ptr<IPrimitive2D>> mPrimitives;
};

