#pragma once
#include "DrawObject/DrawObjectController.h"

#include <DirectXMath.h>
#include <vector>

class VertexBufferObject;
struct D3D12_VERTEX_BUFFER_VIEW;


struct Primitive2DVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
};


class IPrimitive2D : public DrawObjectController
{
public:
	IPrimitive2D(unsigned int vertexCount,const std::string& name,const Microsoft::WRL::ComPtr<ID3D12Device>& dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	virtual ~IPrimitive2D();
protected:
	std::vector<Primitive2DVertex> mVertices;
	VertexBufferObject* mVertexBuffer;
};

