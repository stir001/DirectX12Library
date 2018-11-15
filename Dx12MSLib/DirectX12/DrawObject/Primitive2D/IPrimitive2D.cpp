#include "stdafx.h"
#include "IPrimitive2D.h"
#include "Buffer/VertexBufferObject.h"
#include "Master/Dx12Ctrl.h"

IPrimitive2D::IPrimitive2D(unsigned int vertexCount, const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
	:DrawObjectController(name, dev, cmdList),
	mVertexBuffer(new VertexBufferObject("Primitive2D", Dx12Ctrl::Instance().GetDev(), sizeof(Primitive2DVertex), vertexCount))
{
}


IPrimitive2D::~IPrimitive2D()
{
	delete mVertexBuffer;
}
