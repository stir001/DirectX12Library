#include "stdafx.h"
#include "DrawObjectController.h"
#include "CommandList/Dx12CommandList.h"


DrawObjectController::DrawObjectController(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
	std::shared_ptr<Dx12CommandList>& cmdList)
	:mCmdList(cmdList), mDevice(dev)
{
	mBundleCmdList = std::make_shared<Dx12CommandList>(name, mDevice, D3D12_COMMAND_LIST_TYPE_BUNDLE);
	mBundleCmdList->Close();
}

DrawObjectController::~DrawObjectController()
{
}

void DrawObjectController::SetGraphicsRootSignature(std::shared_ptr<RootSignatureObject>& rootsignature)
{
	mRootsignature = rootsignature;
}

void DrawObjectController::SetPipelineState(std::shared_ptr<PipelineStateObject>& pipelinestate)
{
	mPipelinestate = pipelinestate;
}

void DrawObjectController::SetCommandList(std::shared_ptr<Dx12CommandList>& cmdlist)
{
	mCmdList = cmdlist;
}
