#include "stdafx.h"
#include "Primitive2DLine.h"
#include "Util/XMFloatOperators.h"
#include "Buffer/VertexBufferObject.h"
#include "Master/Dx12Ctrl.h"
#include "PipelineState/PipelineStateObject.h"
#include "Rootsignature/RootSignatureObject.h"

Primitive2DLine::Primitive2DLine(const DirectX::XMFLOAT3& point1, const DirectX::XMFLOAT3& point2
	, const Microsoft::WRL::ComPtr<ID3D12Device> dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
	:IPrimitive2D(4,"2DLine", dev, cmdList), mColor(0.f, 0.f, 0.f), mBaseRad(0.f), mAddRad(0.f), mScale(1.0f),mCenter(0.f,0.f,0.f)
{
	DX12CTRL_INSTANCE
	
	mVertices.resize(4);
	DirectX::XMFLOAT3 defaultColor = { 0.f ,1.f, 0.f };

	DirectX::XMFLOAT3 sub = point1 - point2;
	sub.z = 0;
	mCenter = sub * 0.5f;

	DirectX::XMFLOAT3 unitX = { 1.f, 0.f, 0.f };
	mBaseRad = acosf(DotXMFloat3(unitX, NormalizeXMFloat3(sub)));

	mLength = sqrt(DotXMFloat3(sub, sub));

	SetPoints(point1, point2);
	SetColor(defaultColor);

	UpdateBuffer();
}


Primitive2DLine::~Primitive2DLine()
{
}

void Primitive2DLine::SetCenter(const DirectX::XMFLOAT3& pos)
{
	DirectX::XMFLOAT3 unitP1 = NormalizeXMFloat3(mVertices[0].pos - mCenter);
	DirectX::XMFLOAT3 unitP2 = NormalizeXMFloat3(mVertices[1].pos - mCenter);
	//mCenter = pos;

	SetPoints(unitP1 * (mLength / 2.f), unitP2 * (mLength / 2.0f));
	UpdateBuffer();
}

void Primitive2DLine::Draw()
{
	mCmdList->SetPipelineState(mPipelinestate->GetPipelineState().Get());
	mCmdList->SetGraphicsRootSignature(mRootsignature->GetRootSignature().Get());
	mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	mVertexBuffer->SetBuffer(mCmdList);
	mCmdList->DrawInstanced(4, 1, 0, 0);
}

void Primitive2DLine::UpdateBuffer()
{
	mVertexBuffer->WriteBuffer(&mVertices[0], static_cast<unsigned int>(sizeof(mVertices[0]) * mVertices.size()));
}

void Primitive2DLine::SetScale(float scale)
{
	mScale = scale;

	DirectX::XMFLOAT3 point1vec = mVertices[0].pos - mCenter;
	DirectX::XMFLOAT3 point2vec = mVertices[1].pos - mCenter;

	float p1scale = ((mLength * mScale) / 2.f);
	float p2scale = ((mLength * mScale) / 2.f);

	SetPoints(NormalizeXMFloat3(point1vec) * p1scale + mCenter, NormalizeXMFloat3(point2vec) * p2scale + mCenter);
}

void Primitive2DLine::UpdateVertex()
{

}

void Primitive2DLine::SetBasePoints(const DirectX::XMFLOAT3& point1, const DirectX::XMFLOAT3& point2)
{
	mScale = 1.0f;
	SetPoints(point1, point2);
}

void Primitive2DLine::SetPoints(const DirectX::XMFLOAT3& point1, const DirectX::XMFLOAT3& point2)
{
	DX12CTRL_INSTANCE
	DirectX::XMFLOAT2 wndSize = d12.GetWindowSize();

	mVertices[0].pos.x = point1.x  / (wndSize.x * 0.5f);
	mVertices[0].pos.y = point1.y  / (wndSize.y * 0.5f);
	mVertices[0].pos.z = point1.z;


	mVertices[1].pos.x = point2.x / (wndSize.x * 0.5f);
	mVertices[1].pos.y = point2.y / (wndSize.y * 0.5f);
	mVertices[1].pos.z = point2.z;


	mVertices[2].pos.x = (point1.x + 1.0f) / (wndSize.x * 0.5f);
	mVertices[2].pos.y = (point1.y + 1.0f) / (wndSize.y * 0.5f);
	mVertices[2].pos.z = point1.z;


	mVertices[3].pos.x = (point2.x + 1.0f) / (wndSize.x * 0.5f);
	mVertices[3].pos.y = (point2.y + 1.0f) / (wndSize.y * 0.5f);
	mVertices[3].pos.z = point2.z;

	mCenter = (point1 - point2) * 0.5f;
	mLength = GetLengthXMFloat3(point1 - point2);

	UpdateBuffer();
}

void Primitive2DLine::SetColor(const DirectX::XMFLOAT3& color)
{
	for (auto& v : mVertices)
	{
		v.color = color;
	}

	UpdateBuffer();
}

void Primitive2DLine::AddCenterPos(const DirectX::XMFLOAT3 & val)
{
	mCenter += val;
}
