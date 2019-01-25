#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <wrl.h>

struct ID3D12GraphicsCommandList;
class VertexBufferObject;
class PipelineStateObject;
class RootSignatureObject;

const DirectX::XMFLOAT4 INIT_COLOR = { 1.0f,1.0f,1.0f,1.0f };

struct PrimitiveVertex
{
	DirectX::XMFLOAT4 pos;
	DirectX::XMFLOAT4 normal;
	DirectX::XMFLOAT2 uv;

	PrimitiveVertex()
	{
		pos = { 0, 0, 0, 1};
		normal = { 0, 0, 0, 1 };
		uv = { 0, 0 };
	}

	PrimitiveVertex(DirectX::XMFLOAT3& inpos, DirectX::XMFLOAT3& innorm, DirectX::XMFLOAT2& inuv)
	{
		pos = { inpos.x, inpos.y, inpos.z, 1.0f };
		normal = { innorm.x, innorm.y, innorm.z, 1.0f };
		uv = inuv;
	}

	PrimitiveVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
	{
		pos = { x, y, z, 1.0f };
		normal = { nx, ny, nz, 1.0f };
		uv = { u,v };
	}
};

class PrimitiveObject
{
public:
	PrimitiveObject(const std::string& name);
	virtual ~PrimitiveObject();

	virtual const std::string& GetName() const;
	virtual const std::vector<PrimitiveVertex>& GetVertices() const;
	virtual const std::vector<unsigned int>& GetIndices() const;
	virtual const DirectX::XMFLOAT4& GetColor() const;
	virtual void SetColor(const DirectX::XMFLOAT4& color);
protected:
	std::vector<PrimitiveVertex> mVertices;
	std::vector<unsigned int> mIndices;
	std::string mName;
	DirectX::XMFLOAT4 mColor;
};

