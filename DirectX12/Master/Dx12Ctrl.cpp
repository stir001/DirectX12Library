#include "stdafx.h"
#include "Dx12Ctrl.h"
#include "Texture/TextureLoader.h"
#include "Buffer/DepthBufferObject.h"
#include "Buffer/RendertargetObject.h"
#include "Camera/Dx12Camera.h"
#include "Camera/CameraHolder.h"
#include "Util/CharToWChar.h"
#include "RenderingPass/Manager/RenderingPassManager.h"
#include "CommandList/Dx12CommandList.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "DrawObject/Image/Loader/ImageLoader.h"
#include "Shader/ShaderCompiler.h"
#include "DrawObject/Primitive2D/Primitive2DManager.h"
#include "DrawObject/Fbx/FbxLoader.h"
#include "DrawObject/Primitive3D/PrimitiveCreator.h"
#include "Bullet/System/PhysicsSystem.h"


#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <tchar.h>
#include <DirectXMath.h>
#include <string>
#include <functional>
#include <Windows.h>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

Dx12Ctrl* Dx12Ctrl::mInstance = nullptr;

LRESULT(*winProc)(HWND, UINT, WPARAM, LPARAM);

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	if (msg == WM_SIZE)
	{
		Dx12Ctrl::Instance().UpdateWindowSize();
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

HRESULT Dx12Ctrl::ReportLiveObject()
{
#ifdef _DEBUG
	return mDebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);
#else
	return S_OK;
#endif
}

void Dx12Ctrl::SetWinProc(LRESULT(*proc)(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam))
{
	winProc = proc;
}

void Dx12Ctrl::UpdateWindowSize()
{
	RECT rect;
	auto isGet = GetClientRect(mhWnd, &rect);
	mWndWidth = rect.right;
	mWndHeight = rect.bottom;
}

void Dx12Ctrl::SetIcon(const std::string & iconName)
{
	mIconName = iconName;
}

Dx12Ctrl::Dx12Ctrl() :mWndHeight(720), mWndWidth(1280),mClrcolor{0.5f,0.5f,0.5f,1.0f}
,mCmdAllocator(nullptr),mCmdList(nullptr),mCmdQueue(nullptr),mFactory(nullptr)
,mDev(nullptr), mCameraHolder(nullptr)
,result(S_OK),mFenceValue(0), mWndProc(WindowProcedure)
,mWindowName("DirectX12")
{
	winProc = WindowProcedure;
	setlocale(LC_ALL, "japanese");
}

Dx12Ctrl::~Dx12Ctrl()
{
	Release();
	//ReportLiveObject();
#ifdef _DEBUG
	mDebugDevice.Reset();
#endif // _DEBUG
	mDev.Reset();
}

Microsoft::WRL::ComPtr<ID3D12Device>& Dx12Ctrl::GetDev()
{	
	return mDev;
}

bool Dx12Ctrl::Dx12Init( HINSTANCE winHInstance)
{
	mWinHInstance = winHInstance;
	InitWindowCreate();

#ifdef _DEBUG
	{
		ID3D12Debug* debug;
		D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
		debug->EnableDebugLayer();
		debug->Release();
	}
#endif


	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};
	D3D_FEATURE_LEVEL level = {};

	HRESULT result = S_OK;

	result = CreateDXGIFactory1(IID_PPV_ARGS(&mFactory));

	Microsoft::WRL::ComPtr<IDXGIAdapter1>	hardwareAdapter;
	Microsoft::WRL::ComPtr<IDXGIAdapter1>	adapter;
	hardwareAdapter = nullptr;

	std::wstring searchStr = L"NVIDIA";

	for (UINT i = 0; DXGI_ERROR_NOT_FOUND != mFactory->EnumAdapters1(i, &adapter); i++) {
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		std::wstring description = desc.Description;
		size_t size = description.find(searchStr);
		if (size >= description.size())
		{
			continue;
		}

		for (auto i : levels) {
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), i, __uuidof(ID3D12Device), nullptr))) {
				level = i;
				hardwareAdapter = adapter;
				break;
			}
		}
	}

	if (hardwareAdapter)
	{
		result = D3D12CreateDevice(hardwareAdapter.Get(), level, IID_PPV_ARGS(&mDev));
	}
	else
	{
		for (auto i : levels) {
			if (SUCCEEDED(D3D12CreateDevice(nullptr, i, IID_PPV_ARGS(&mDev)))) {
				level = i;
				break;
			}
		}
	}
#ifdef _DEBUG
	mDev->QueryInterface(mDebugDevice.GetAddressOf());
#endif // _DEBUG
	adapter.Detach();

	if (result != S_OK)
	{
		mDev = nullptr;
		return false;
	}

	mDev->SetName(L"ID3D12Device");

	mDepthBuffer = std::make_shared<DepthBufferObject>("MasterDepthBuffer", mDev, mWndWidth, mWndHeight);
	std::vector<std::shared_ptr<Dx12BufferObject>> t_buffer;
	t_buffer.push_back(mDepthBuffer);
	mDepthDescHeap = std::make_shared<Dx12DescriptorHeapObject>("DepthDescriptorHeap",mDev, t_buffer, D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	t_buffer.clear();

	result = mDev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCmdAllocator));
	mCmdAllocator->SetName(L"MasterCommandAllocator");

	result = mDev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCmdAllocator.Get(), nullptr, IID_PPV_ARGS(&mCmdList));
	mCmdList->SetName(L"MasterCommandList");

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	result = mDev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&mCmdQueue));
	
	result = mDev->CreateFence(mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));

	mCameraHolder = std::make_shared<CameraHolder>(mWndWidth, mWndHeight, mDev);
	D3D12_VIEWPORT viewport = {0, 0, static_cast<float>(mWndWidth), static_cast<float>(mWndHeight), 0.0f, 1.0f};
	D3D12_RECT sissorRect = { 0, 0, mWndWidth, mWndHeight };
	mCameraHolder->AddCamera(DirectX::XMFLOAT3(0, 20, -30), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), viewport, sissorRect);

	//RendringManagerÉNÉâÉXÇÃèâä˙âªèàóù
	InitRenderPath();

	return true;
}

void Dx12Ctrl::InitRenderPath()
{
	RenderingPassManager::Instance().Init(mDev, mFactory, mhWnd);
}

void Dx12Ctrl::InitWindowCreate()
{
	RECT wrc = { 0,0,mWndWidth,mWndHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	std::string strName = mWindowName;
	strName.push_back('\0');
	size_t size = strName.size();
	std::wstring buff;
	ToWChar(buff,strName);
	std::wstring icon;
	strName = mIconName;
	strName.push_back('\0');
	ToWChar(icon, strName);

	WNDCLASSEX w = {};
	w.lpfnWndProc = (WNDPROC)winProc;
	w.lpszClassName = buff.data();
	w.hInstance = mWinHInstance;
	w.hIcon = LoadIcon(w.hInstance, icon.data());
	w.hIconSm = w.hIcon;
	w.cbSize = sizeof(WNDCLASSEX);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	RegisterClassEx(&w);

	HWND hwnd = CreateWindow(w.lpszClassName,
		buff.data(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		w.hInstance,
		nullptr);

	mhWnd = hwnd;

	ShowWindow(hwnd, SW_SHOW);
}

Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& Dx12Ctrl::GetCmdAllocator()
{
	return mCmdAllocator;
}

Microsoft::WRL::ComPtr<ID3D12CommandQueue>& Dx12Ctrl::GetCmdQueue()
{
	return mCmdQueue;
}

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& Dx12Ctrl::GetCmdList()
{
	return mCmdList;
}

Microsoft::WRL::ComPtr<IDXGIFactory4> Dx12Ctrl::GetFactory()
{
	return mFactory;
}

UINT64 Dx12Ctrl::GetFenceValue() const
{
	return mFenceValue;
}

void Dx12Ctrl::CmdQueueSignal()
{
	mCmdQueue->Signal(mFence.Get(), ++mFenceValue);
	UINT64 value = mFence->GetCompletedValue();
	UINT64 u64max = UINT64_MAX;
	while (value != mFenceValue)
	{
		value = mFence->GetCompletedValue();
		if (value == UINT64_MAX)
		{
			GetDeviceRemoveReason();
		}
	}
}

HRESULT Dx12Ctrl::GetDeviceRemoveReason()
{
	result = mDev->GetDeviceRemovedReason();
	if (FAILED(result))
	{
		MessageBox(mhWnd, L"Device removed", L"ERROR", MB_OK);
	}
	return result;
}

HRESULT Dx12Ctrl::CheckResult()
{
	if (FAILED(result))
	{
		return GetDeviceRemoveReason();
	}
	return result;
}
HRESULT Dx12Ctrl::CheckResult(HRESULT r)
{
	return GetDeviceRemoveReason();
}

DirectX::XMFLOAT2 Dx12Ctrl::GetWindowSize() const
{
	DirectX::XMFLOAT2 size = { static_cast<float>(mWndWidth), static_cast<float>(mWndHeight) };
	return size;
}

void Dx12Ctrl::Release()
{
	mDepthDescHeap.reset();
	TextureLoader::Destroy();
	RenderingPassManager::Destroy();
	FbxLoader::Destroy();
	ImageLoader::Destroy();
	ShaderCompiler::Destroy();
	mCameraHolder.reset();
	Primitive2DManager::Destory();
	mDepthBuffer.reset();
	mCmdAllocator.Reset();
	mCmdList.Reset();
	mCmdQueue.Reset();
	mFactory.Reset();
	mFence.Reset();
	PrimitiveCreator::Destroy();
	PhysicsSystem::Destory();

	ReportLiveObject();
}

void Dx12Ctrl::SetWindowSize(int inw, int inh)
{
	mWndWidth = inw;
	mWndHeight = inh;
}

void Dx12Ctrl::SetWindowName(std::string& inWindowName)
{
	mWindowName = inWindowName;
}

const HWND& Dx12Ctrl::GetWindowHandle() const
{
	return mhWnd;
}

std::shared_ptr<DepthBufferObject> Dx12Ctrl::GetDepthBuffer() const
{
	return mDepthBuffer;
}

std::shared_ptr<Dx12DescriptorHeapObject> Dx12Ctrl::GetDepthDescHeap() const
{
	return mDepthDescHeap;
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12Ctrl::GetDepthCpuHandle() const
{
	return mDepthDescHeap->GetCPUHeapHandleStart();
}

std::shared_ptr<Dx12Camera> Dx12Ctrl::GetCamera(unsigned int index) const
{
	return mCameraHolder->GetCamera(index);
}

std::shared_ptr<CameraHolder> Dx12Ctrl::GetCameraHolder()
{
	return mCameraHolder;
}
