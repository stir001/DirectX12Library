#include "stdafx.h"
#include "BulletlibLink.h"
#include "BulletDebugDrawDx.h"
#include "Rootsignature/WireFrameRootSignature.h"
#include "PipelineState//WireFramePipelineState.h"
#include "RenderingPass/Manager/RenderingPassManager.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "Master/Dx12Ctrl.h"
#include "Camera/CameraHolder.h"
#include "Buffer/VertexBufferObject.h"
#include "Buffer/ConstantBufferObject.h"

#include <btBulletDynamicsCommon.h>

BulletDebugDrawDx::BulletDebugDrawDx(const Microsoft::WRL::ComPtr<ID3D12Device>& dev)
	: mMode(DebugDrawModes::DBG_NoDebug), mDev(dev)
	, mCamerasBuffer(Dx12Ctrl::Instance().GetCameraHolder()->GetCamerasBuffer())
{
	mRootsignature = (std::make_shared<WireFrameRootSignature>(mDev));
	mPipelinestate = (std::make_shared<WireFramePipelineState>(mRootsignature, mDev));

	mCmdList = RenderingPassManager::Instance().GetRenderingPassCommandList(static_cast<unsigned int>(DefaultPass::Model));
	std::vector<std::shared_ptr<Dx12BufferObject>> buffers = { mCamerasBuffer };

	mDescHeap = std::make_shared<Dx12DescriptorHeapObject>("btDebugDrawHeap"
		, mDev
		, buffers
		, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}


BulletDebugDrawDx::~BulletDebugDrawDx()
{
}

void BulletDebugDrawDx::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{
	mVertices.emplace_back(DebugDrawVertex(from, color));
	mVertices.emplace_back(DebugDrawVertex(to, color));
}

void BulletDebugDrawDx::setDebugMode(int debugMode)
{
	mMode = debugMode;
}

int BulletDebugDrawDx::getDebugMode() const
{
	return mMode;
}

void BulletDebugDrawDx::drawContactPoint(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color)
{
}

void BulletDebugDrawDx::reportErrorWarning(const char * warningString)
{
	std::string str = warningString;
	MessageBoxA(nullptr, warningString, "BulletDebugDrawDx error", MB_OK);
}

void BulletDebugDrawDx::draw3dText(const btVector3 & location, const char * textString)
{
}

void BulletDebugDrawDx::DebugDraw()
{
	if (mVertices.size() < 2) return;

	unsigned int dataSize = static_cast<unsigned int>(sizeof(mVertices[0]));
	unsigned int dataNum = static_cast<unsigned int>(mVertices.size());
	mVertexBuffer = std::make_shared<VertexBufferObject>("btDebugVertexBuffer",mDev, dataSize, dataNum);
	mVertexBuffer->WriteBuffer(mVertices.data(), dataSize * dataNum);

	mCmdList->SetPipelineState(mPipelinestate->GetPipelineState().Get());
	mCmdList->SetGraphicsRootSignature(mRootsignature->GetRootSignature().Get());
	mCmdList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	mVertexBuffer->SetBuffer(mCmdList);
	mDescHeap->SetDescriptorHeap(mCmdList);
	mDescHeap->SetGprahicsDescriptorTable(mCmdList, 0, 0);
	mCmdList->DrawInstanced(static_cast<unsigned int>(mVertices.size()), 1, 0, 0);
	mVertices.clear();
}
