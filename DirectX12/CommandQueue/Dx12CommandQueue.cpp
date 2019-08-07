#include "stdafx.h"
#include "Dx12CommandQueue.h"
#include "CommandList/Dx12CommandList.h"
#include "Master/Dx12Ctrl.h"
#include "Fence/Dx12Fence.h"
#include "Util/CharToWChar.h"
#include <d3d12.h>

Dx12CommandQueue::Dx12CommandQueue(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, D3D12_COMMAND_LIST_TYPE type)
	: mEventHandle(CreateEvent(nullptr, false, false, nullptr))
{
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = type;

	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(mCmdQueue.GetAddressOf()));

	std::wstring wstr;

	ToWChar(wstr, name);
	wstr += L"CommandQueue";
	mCmdQueue->SetName(wstr.data());
}

Dx12CommandQueue::~Dx12CommandQueue()
{
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue> Dx12CommandQueue::Get()
{
	return mCmdQueue;
}

void Dx12CommandQueue::Signal()
{
	auto& fence = Dx12Ctrl::Instance().GetFence();
	mFenceValue = fence->IncrementValue();
	mCmdQueue->Signal(fence->GetFence().Get(), mFenceValue);
}

void Dx12CommandQueue::ExecuteCommandList(std::shared_ptr<Dx12CommandList>& cmdList)
{
	ID3D12CommandList* c = reinterpret_cast<ID3D12CommandList*>(cmdList->GetCommandList().Get());
	mCmdQueue->ExecuteCommandLists(1, &c);
}

void Dx12CommandQueue::ExecuteCommandLists(std::shared_ptr<Dx12CommandList>* cmdLists, unsigned int cmdListNum)
{
	std::vector<ID3D12CommandList*> cmdList;
	for (unsigned int i = 0; i < cmdListNum; ++i)
	{
		cmdList.push_back(cmdLists[i]->GetCommandList().Get());
	}
	mCmdQueue->ExecuteCommandLists(cmdListNum, cmdList.data());
}

void Dx12CommandQueue::Wait()
{
	auto& fence = Dx12Ctrl::Instance().GetFence();
	fence->GetFence()->SetEventOnCompletion(mFenceValue, mEventHandle);
	WaitForSingleObject(mEventHandle, INFINITE);
	/*auto& fence = Dx12Ctrl::Instance().GetFence()->GetFence();
	auto value = fence->GetCompletedValue();
	while (mFenceValue > value)
	{
		value = fence->GetCompletedValue();
	}
	auto hr = mCmdQueue->Wait(fence.Get(), mFenceValue);
	auto value2 = fence->GetCompletedValue();
	assert(mFenceValue >= value2);*/
}
