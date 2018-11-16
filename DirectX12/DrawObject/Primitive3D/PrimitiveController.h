#pragma once
#include "DrawObject/DrawController3D.h"
#include <vector>

class PrimitiveObject;
class VertexBufferObject;
class ConstantBufferObject;
class Dx12DescriptorHeapObject;
class IndexBufferObject;
class LightObject;
class Dx12Camera;
class TextureObject;

class PrimitiveController :
	public DrawController3D
{
public:
	PrimitiveController(std::shared_ptr<PrimitiveObject> primitive
		,Microsoft::WRL::ComPtr<ID3D12Device>& dev
		, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	~PrimitiveController();

	void SetTexture(std::shared_ptr<TextureObject>& texture);
	void SetLightBuffer(std::shared_ptr<LightObject>& light);
	void Instancing(std::vector<DirectX::XMFLOAT3>& instancePositions);
	void SetInstancingMatrix(std::vector<DirectX::XMFLOAT4X4>& matrix, unsigned int startIndex, unsigned int endIndex);
	void Draw();

	void UpdateDescriptorHeap();
protected:
	void UpdateInstanceVertexBuffer();
	void NonUpdate();

	enum eROOT_PARAMATER_INDEX
	{
		eROOT_PARAMATER_INDEX_TEXTURE,
		eROOT_PARAMATER_INDEX_CAMERA,
		eROOT_PARAMATER_INDEX_LIGHT,
		eROOT_PARAMATER_INDEX_MAX
	};

	struct InstanceDatas
	{
		DirectX::XMFLOAT4X4 aMatrix;
		DirectX::XMFLOAT4 offset;
	};
	std::shared_ptr<PrimitiveObject> mPrimitive;
	std::shared_ptr<VertexBufferObject> mVertexBuffer;
	std::shared_ptr<IndexBufferObject> mIndexBuffer;
	std::vector<InstanceDatas> mInstanceDatas;
	std::shared_ptr<VertexBufferObject> mInstanceVertexBuffer;
	std::shared_ptr<Dx12DescriptorHeapObject> mDescHeap;
	std::shared_ptr<ConstantBufferObject> mLightBuffer;
	std::shared_ptr<TextureObject> mTexObj;
	void (PrimitiveController::*mInstanceUpdate)();
	void (PrimitiveController::*mDescHeapUpdate)();
};

