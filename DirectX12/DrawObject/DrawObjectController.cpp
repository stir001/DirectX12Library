#include "stdafx.h"
#include "DrawObjectController.h"
#include "CommandList/Dx12CommandList.h"


DrawObjectController::DrawObjectController(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList)
	:mCmdList(cmdList), mDevice(dev)
{
	mBundleCmdList = std::make_shared<Dx12CommandList>(name, mDevice, D3D12_COMMAND_LIST_TYPE_BUNDLE);
	mBundleCmdList->Close();
}

DrawObjectController::~DrawObjectController()
{
	mCmdList.Reset();
}

void DrawObjectController::SetRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature)
{
	mRootsignature = rootsignature;
}

void DrawObjectController::SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate)
{
	mPipelinestate = pipelinestate;
}

void DrawObjectController::SetCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdlist)
{
	mCmdList = cmdlist;
}
