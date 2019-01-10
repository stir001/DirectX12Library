#include "stdafx.h"
#include "RenderingPassManager.h"
#include "Util/CharToWChar.h"
#include "SwapChain/SwapChainObject.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "RenderingPass/Base/RenderingPassObject.h"
#include "RenderingPass/SkyBoxPass.h"
#include "RenderingPass/ModelPass.h"
#include "RenderingPass/BackGroundPass.h"
#include "RenderingPass/UIPass.h"
#include "DrawObject/SkyBox.h"
#include "CommandList/Dx12CommandList.h"
#include "Master/Dx12Ctrl.h"
#include "Buffer/RendertargetObject.h"

#include "d3dx12.h"
#include <algorithm>
#include <assert.h>

RenderingPassManager* RenderingPassManager::mInstance = nullptr;

RenderingPassManager::RenderingPassManager()
{
}

RenderingPassManager::~RenderingPassManager()
{
	mRenderingPassObjects.clear();
	mSwapChain.reset();
	mFence.Reset();
}

void RenderingPassManager::Init(Microsoft::WRL::ComPtr<ID3D12Device>& dev, Microsoft::WRL::ComPtr<IDXGIFactory4>& factory, HWND hwnd)
{
	auto& d12 = Dx12Ctrl::Instance();

	mRenderingPassObjects.clear();
	mCmdQueue.Reset();

	mDevice = dev;
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	mRenderCmdList = std::make_shared<Dx12CommandList>("RenderingCommandList", dev);

	mDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&mCmdQueue));
	mCmdQueue->SetName(L"RenderingCommandQueue");

	mSwapChain.reset(new SwapChainObject(hwnd, mDevice, factory, mCmdQueue));

	DXGI_SWAP_CHAIN_DESC desc;
	mSwapChain->GetSwapChain()->GetDesc(&desc);
	mWidth = desc.BufferDesc.Width;
	mHeight = desc.BufferDesc.Height;

	mDevice->CreateFence(mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
	mFence->SetName(L"RenderingManagerFence");

	auto device = d12.GetDev();
	auto wndSize = d12.GetWindowSize();

	auto rendertarget = std::make_shared<RendertargetObject>("MainRendertarget", device, wndSize.x, wndSize.y);

	std::vector<std::shared_ptr<Dx12BufferObject>> buffers(1);
	buffers[0] = rendertarget;

	auto descheap = std::make_shared<Dx12DescriptorHeapObject>("MainRTVDescHeap", device, buffers, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	auto skyBoxPass = std::make_shared<SkyBoxPass>(device, descheap, rendertarget, wndSize.x, wndSize.y);

	std::shared_ptr<BackGroundPass> backpass = std::make_shared<BackGroundPass>(device, descheap, rendertarget, wndSize.x, wndSize.y);

	std::shared_ptr<ModelPass> modelpass = std::make_shared<ModelPass>(
		device, d12.GetDepthDescHeap(), descheap, wndSize.x, wndSize.y, d12.GetCameraHolder());

	std::shared_ptr<UIPass> uipass = std::make_shared<UIPass>(device, descheap, rendertarget, wndSize.x, wndSize.y);

	unsigned int renderingPathIndex;
	auto& passMgr = RenderingPassManager::Instance();
	passMgr.AddRenderingPass(skyBoxPass, renderingPathIndex);
	passMgr.AddRenderingPass(backpass, renderingPathIndex);
	passMgr.AddRenderingPass(modelpass, renderingPathIndex);
	passMgr.AddRenderingPass(uipass, renderingPathIndex);

	//skyBoxPass->FirstUpdate();
	backpass->FirstUpdate();
	modelpass->FirstUpdate();
	uipass->FirstUpdate();
}

void RenderingPassManager::Render()
{
	for (auto& pathObj : mRenderingPassObjects)
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

	CopyLastPassRenderTarget();

	HRESULT result = mSwapChain->SwapChainPresent(1, 0);
	assert(!FAILED(result));

	float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	D3D12_RECT rect = {};
	rect.right = mWidth;
	rect.bottom = mHeight;

	mRenderCmdList->TransitionBarrier(mSwapChain->GetCurrentRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET);

	mRenderCmdList->ClearRenderTargetView(mSwapChain->GetCurrentRTVHeap(), clearColor, &rect, 1);

	mRenderCmdList->TransitionBarrier(mSwapChain->GetCurrentRenderTarget(),
		D3D12_RESOURCE_STATE_PRESENT);

	for (auto& pathObj : mRenderingPassObjects)
	{
		if (!pathObj->IsActive())
		{
			continue;
		}
		pathObj->FirstUpdate();
	}
}

void RenderingPassManager::CopyLastPassRenderTarget()
{
	auto& lastPath = mRenderingPassObjects.back();
	auto lastPathResrouce = lastPath->GetRenderTarget();
	mRenderCmdList->TransitionBarrier(lastPathResrouce, D3D12_RESOURCE_STATE_COPY_SOURCE);

	auto swapChainResrouce = mSwapChain->GetCurrentRenderTarget();
	mRenderCmdList->TransitionBarrier(swapChainResrouce, D3D12_RESOURCE_STATE_COPY_DEST);

	mRenderCmdList->CopyResource(mSwapChain->GetCurrentRenderTarget(), lastPath->GetRenderTarget());

	mRenderCmdList->TransitionBarrier(lastPathResrouce, D3D12_RESOURCE_STATE_RENDER_TARGET);
	mRenderCmdList->TransitionBarrier(swapChainResrouce, D3D12_RESOURCE_STATE_RENDER_TARGET);

	mRenderCmdList->OMSetRenderTargets(1, mSwapChain->GetCurrentRTVHeap());

	mRenderCmdList->TransitionBarrier(swapChainResrouce, D3D12_RESOURCE_STATE_PRESENT);

	mRenderCmdList->Close();

	mCmdQueue->ExecuteCommandLists(1, (ID3D12CommandList**)mRenderCmdList->GetCommandList().GetAddressOf());

	WaitCmdQueue();

	mRenderCmdList->Reset();

}

unsigned int RenderingPassManager::GetNumCuurentPass() const
{
	return static_cast<unsigned int>(mRenderingPassObjects.size());
}

std::shared_ptr<Dx12CommandList> RenderingPassManager::GetRenderingPassCommandList(unsigned int pathIndex) const
{
	if (pathIndex < mRenderingPassObjects.size())
	{
		return mRenderingPassObjects[pathIndex]->GetCommandList();
	}
	return nullptr;
}

std::shared_ptr<Dx12CommandList> RenderingPassManager::GetRenderingPassCommandList(const std::string& pathName) const
{
	unsigned int index = GetRenderingPassIndex(pathName);
	if (index != UINT_MAX)
	{
		return mRenderingPassObjects[index]->GetCommandList();
	}

	return nullptr;
}

bool RenderingPassManager::AddRenderingPass(const std::shared_ptr<RenderingPassObject>& pathObj, unsigned int& out_PathIndex)
{
	auto& itr = std::find_if(mRenderingPassObjects.begin(), mRenderingPassObjects.end(), [&pathObj](const std::shared_ptr<RenderingPassObject>& value) {return value->GetPassName() == pathObj->GetPassName(); });
	if (itr != mRenderingPassObjects.end())
	{
		out_PathIndex = UINT_MAX;
		return false;
	}
	out_PathIndex = AddRenderingPassObject(pathObj);
	return true;
}

bool RenderingPassManager::InsertRenderingPass(std::shared_ptr<RenderingPassObject>& pathObj, unsigned int insertPassIndex)
{
	if (insertPassIndex <= mRenderingPassObjects.size())
	{
		auto itr = mRenderingPassObjects.begin();
		mRenderingPassObjects.insert((itr + insertPassIndex), pathObj);
		return true;
	}
	return false;
}

unsigned int RenderingPassManager::AddRenderingPassObject(const std::shared_ptr<RenderingPassObject>& pathObj)
{
	mRenderingPassObjects.push_back(pathObj);
	return static_cast<unsigned int>(mRenderingPassObjects.size() - 1);
}

unsigned int RenderingPassManager::GetRenderingPassIndex(const std::string& pathName) const
{
	for (unsigned int i = 0; i < mRenderingPassObjects.size(); ++i)
	{
		if (mRenderingPassObjects[i]->GetPassName() == pathName)
		{
			return i;
		}
	}
	return UINT_MAX;
}

bool RenderingPassManager::DeleteRenderingPass(unsigned int pathIndex)
{
	if (mRenderingPassObjects.size() > pathIndex)
	{
		mRenderingPassObjects.erase(mRenderingPassObjects.begin() + pathIndex);
		return true;
	}

	return false;
}

bool RenderingPassManager::DeleteRenderingPass(const std::string & pathName)
{
	unsigned int index = GetRenderingPassIndex(pathName);
	if (index != UINT_MAX)
	{
		mRenderingPassObjects.erase(mRenderingPassObjects.begin() + index);
		return true;
	}
	return false;
}

D3D12_CPU_DESCRIPTOR_HANDLE RenderingPassManager::GetCurrentRTVHeapHandle() const
{
	return mSwapChain->GetCurrentRTVHeap();
}

void RenderingPassManager::WaitCmdQueue()
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

std::shared_ptr<Dx12DescriptorHeapObject> RenderingPassManager::GetCurrentRTVDescHeap() const
{
	return mSwapChain->GetDescriptorHeap();
}

std::shared_ptr<Dx12BufferObject> RenderingPassManager::GetCurrentRenderTarget() const
{
	return mSwapChain->GetCurrentRenderTarget();
}

void RenderingPassManager::SetIsActiveValue(unsigned int pathIndex, bool isActive)
{
	if (mRenderingPassObjects.size() >= pathIndex)
	{
		auto& pathObj = mRenderingPassObjects[pathIndex];
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

void RenderingPassManager::SetIsActiveValue(std::string pathName, bool isActive)
{
	unsigned int index = GetRenderingPassIndex(pathName);
	if (index != UINT_MAX)
	{
		auto& pathObj = mRenderingPassObjects[index];
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

void RenderingPassManager::AllPassClear()
{
	mRenderingPassObjects.clear();
	mRenderingPassObjects.shrink_to_fit();
}
