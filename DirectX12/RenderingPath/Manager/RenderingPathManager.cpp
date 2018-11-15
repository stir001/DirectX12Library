#include "stdafx.h"
#include "RenderingPathManager.h"
#include "Util/CharToWChar.h"
#include "SwapChain/SwapChainObject.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "RenderingPath/Base/RenderingPathObject.h"

#include "d3dx12.h"
#include <algorithm>
#include <assert.h>

RenderingPathManager* RenderingPathManager::mInstance = nullptr;

RenderingPathManager::RenderingPathManager()
{
}


RenderingPathManager::~RenderingPathManager()
{
	mRenderingPathObjects.clear();
	mSwapChain.reset();
	mFence.Reset();
}

void RenderingPathManager::Init(Microsoft::WRL::ComPtr<ID3D12Device>& dev, Microsoft::WRL::ComPtr<IDXGIFactory4>& factory, HWND hwnd)
{
	mRenderingPathObjects.clear();
	mCmdQueue.Reset();

	mDevice = dev;
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mRenderCmdAllocator));
	mRenderCmdAllocator->SetName(L"RenderingCommandAllocator");

	mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mRenderCmdAllocator.Get(), nullptr, IID_PPV_ARGS(&mRenderCmdList));
	mRenderCmdList->SetName(L"RenderingCommandList");

	//mRenderCmdAllocator->Reset();
	//mRenderCmdList->Reset(mRenderCmdAllocator.Get(), nullptr);

	mDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&mCmdQueue));
	mCmdQueue->SetName(L"RenderingCommandQueue");

	mSwapChain.reset(new SwapChainObject(hwnd, mDevice, factory, mCmdQueue));

	DXGI_SWAP_CHAIN_DESC desc;
	mSwapChain->GetSwapChain()->GetDesc(&desc);
	mWidth = desc.BufferDesc.Width;
	mHeight = desc.BufferDesc.Height;

	mDevice->CreateFence(mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
	mFence->SetName(L"RenderingManagerFence");

	//InitFunctionObject_t nonFunc = [](CmdListsArg_t, RTResourcesArg_t, RTDescHeapArg_t) {};
	//LastFunctionObject_t nonlastFunc = [](CmdListsArg_t, RTResourcesArg_t) {};

	//Microsoft::WRL::ComPtr<ID3D12Resource> resoruce;//シェーダーリソースビューとして作成してから渡す
	//CreateDummyRenderTarget(resoruce);

	//std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> firstTarget;
	//firstTarget.push_back(resoruce);

	//unsigned int index = AddRenderingPathObject("FirstPath", firstTarget);

	//mRenderingPathObjects[index].isActive = true;
}

void RenderingPathManager::Render()
{
	for (auto& pathObj : mRenderingPathObjects)
	{
		if (!pathObj->IsActive())
		{
			continue;
		}
		pathObj->PreExcuteUpdate();
		pathObj->ExcutePath(mCmdQueue);

		WaitCmdQueue();

		pathObj->ResetCommandList();
	}

	CopyLastPathRenderTarget();

	HRESULT result = mSwapChain->SwapChainPresent(1, 0);
	assert(!FAILED(result));

	float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	D3D12_RECT rect = {};
	rect.right = mWidth;
	rect.bottom = mHeight;

	mRenderCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mSwapChain->GetCurrentRenderTarget().Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	mRenderCmdList->ClearRenderTargetView(mSwapChain->GetCurrentRTVHeap(), clearColor, 1, &rect);

	mRenderCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mSwapChain->GetCurrentRenderTarget().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	for (auto& pathObj : mRenderingPathObjects)
	{
		if (!pathObj->IsActive())
		{
			continue;
		}
		pathObj->FirstUpdate();
	}
}

void RenderingPathManager::CopyLastPathRenderTarget()
{
	auto& lastPath = mRenderingPathObjects.back();
	D3D12_RESOURCE_STATES lastPathBeforeState = D3D12_RESOURCE_STATE_RENDER_TARGET;
	D3D12_RESOURCE_STATES lastPathAfterState = D3D12_RESOURCE_STATE_COPY_SOURCE;
	Microsoft::WRL::ComPtr<ID3D12Resource> lastPathResrouce = lastPath->GetRenderTarget();
	mRenderCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(lastPathResrouce.Get(), lastPathBeforeState, lastPathAfterState));

	D3D12_RESOURCE_STATES swapChainBeforeState = D3D12_RESOURCE_STATE_PRESENT;
	D3D12_RESOURCE_STATES swapChainAfterState = D3D12_RESOURCE_STATE_COPY_DEST;
	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResrouce = mSwapChain->GetCurrentRenderTarget();
	mRenderCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(swapChainResrouce.Get(), swapChainBeforeState, swapChainAfterState));

	mRenderCmdList->CopyResource(mSwapChain->GetCurrentRenderTarget().Get(), lastPath->GetRenderTarget().Get());

	mRenderCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(lastPathResrouce.Get(), lastPathAfterState, lastPathBeforeState));
	mRenderCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(swapChainResrouce.Get(), swapChainAfterState, D3D12_RESOURCE_STATE_RENDER_TARGET));

	mRenderCmdList->OMSetRenderTargets(1, &mSwapChain->GetCurrentRTVHeap(), false, nullptr);

	mRenderCmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(swapChainResrouce.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, swapChainBeforeState));

	mRenderCmdList->Close();

	mCmdQueue->ExecuteCommandLists(1, (ID3D12CommandList**)mRenderCmdList.GetAddressOf());

	WaitCmdQueue();

	mRenderCmdAllocator->Reset();
	mRenderCmdList->Reset(mRenderCmdAllocator.Get(), nullptr);

}

unsigned int RenderingPathManager::GetNumCuurentPath() const
{
	return static_cast<unsigned int>(mRenderingPathObjects.size());
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> RenderingPathManager::GetRenderingPathCommandList(unsigned int pathIndex) const
{
	if (pathIndex < mRenderingPathObjects.size())
	{
		return mRenderingPathObjects[pathIndex]->GetCommandList();
	}
	return nullptr;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> RenderingPathManager::GetRenderingPathCommandList(const std::string& pathName) const
{
	unsigned int index = GetRenderingPathIndex(pathName);
	if (index != UINT_MAX)
	{
		return mRenderingPathObjects[index]->GetCommandList();
	}

	return nullptr;
}

bool RenderingPathManager::AddRenderPath(std::shared_ptr<RenderingPathObject>& pathObj, unsigned int& out_PathIndex)
{
	auto& itr = std::find_if(mRenderingPathObjects.begin(), mRenderingPathObjects.end(), [&pathObj](const std::shared_ptr<RenderingPathObject>& value) {return value->GetPathName() == pathObj->GetPathName(); });
	if (itr != mRenderingPathObjects.end())
	{
		out_PathIndex = UINT_MAX;
		return false;
	}
	out_PathIndex = AddRenderingPathObject(pathObj);
	return true;
}

bool RenderingPathManager::InsertRenderPath(std::shared_ptr<RenderingPathObject>& pathObj, unsigned int insertPathIndex)
{
	if (insertPathIndex <= mRenderingPathObjects.size())
	{
		auto itr = mRenderingPathObjects.begin();
		mRenderingPathObjects.insert((itr + insertPathIndex), pathObj);
		return true;
	}
	return false;
}

unsigned int RenderingPathManager::AddRenderingPathObject(std::shared_ptr<RenderingPathObject>& pathObj)
{
	mRenderingPathObjects.push_back(pathObj);
	return static_cast<unsigned int>(mRenderingPathObjects.size() - 1);
}

unsigned int RenderingPathManager::GetRenderingPathIndex(const std::string& pathName) const
{
	for (unsigned int i = 0; i < mRenderingPathObjects.size(); ++i)
	{
		if (mRenderingPathObjects[i]->GetPathName() == pathName)
		{
			return i;
		}
	}
	return UINT_MAX;
}

bool RenderingPathManager::DeleteRenderingPath(unsigned int pathIndex)
{
	if (mRenderingPathObjects.size() > pathIndex)
	{
		mRenderingPathObjects.erase(mRenderingPathObjects.begin() + pathIndex);
		return true;
	}

	return false;
}

bool RenderingPathManager::DeleteRenderingPath(const std::string & pathName)
{
	unsigned int index = GetRenderingPathIndex(pathName);
	if (index != UINT_MAX)
	{
		mRenderingPathObjects.erase(mRenderingPathObjects.begin() + index);
		return true;
	}
	return false;
}

D3D12_CPU_DESCRIPTOR_HANDLE RenderingPathManager::GetCurrentRTVHeapHandle() const
{
	return mSwapChain->GetCurrentRTVHeap();
}

void RenderingPathManager::WaitCmdQueue()
{
	mCmdQueue->Signal(mFence.Get(), ++mFenceValue);
	UINT64 value = mFence->GetCompletedValue();
	UINT64 u64max = UINT64_MAX;
	while (value != mFenceValue)
	{
		value = mFence->GetCompletedValue();
		if (value == UINT64_MAX)
		{
			HRESULT result = mDevice->GetDeviceRemovedReason();
			assert(!FAILED(result));
		}
	}
}

std::shared_ptr<Dx12DescriptorHeapObject> RenderingPathManager::GetCurrentRTVDescHeap() const
{
	return mSwapChain->GetDescriptorHeap();
}

Microsoft::WRL::ComPtr<ID3D12Resource> RenderingPathManager::GetCurrentRenderTarget() const
{
	return mSwapChain->GetCurrentRenderTarget();
}

void RenderingPathManager::SetIsActiveValue(unsigned int pathIndex, bool isActive)
{
	if (mRenderingPathObjects.size() >= pathIndex)
	{
		auto& pathObj = mRenderingPathObjects[pathIndex];
		if (pathObj->IsActive() == isActive)
		{
			return;
		}
		pathObj->SetActive(isActive);
		if (isActive)
		{
			pathObj->FirstUpdate();
		}
		else
		{
			pathObj->ResetCommandList();
		}
	}
}

void RenderingPathManager::SetIsActiveValue(std::string pathName, bool isActive)
{
	unsigned int index = GetRenderingPathIndex(pathName);
	if (index != UINT_MAX)
	{
		auto& pathObj = mRenderingPathObjects[index];
		if (pathObj->IsActive() == isActive)
		{
			return;
		}
		pathObj->SetActive(isActive);
		if (isActive)
		{
			pathObj->FirstUpdate();
		}
		else
		{
			pathObj->ResetCommandList();
		}
	}
}

void RenderingPathManager::AllPathClear()
{
	mRenderingPathObjects.clear();
	mRenderingPathObjects.shrink_to_fit();
}
