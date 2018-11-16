#include "stdafx.h"
#include "Dx12Ctrl.h"
#include "d3dx12.h"
#include "Texture/TextureLoader.h"
#include "Buffer/DepthBufferObject.h"
#include "Camera/Dx12Camera.h"
#include "Camera/CameraHolder.h"
#include "Util/CharToWChar.h"
#include "RenderingPath/Manager/RenderingPathManager.h"
#include "CommandList/Dx12CommandList.h"
#include "DescriptorHeap/Dx12DescriptorHeapObject.h"
#include "DrawObject/Image/Loader/ImageLoader.h"
#include "Shader/ShaderCompiler.h"
#include "DrawObject/Primitive2D/Primitive2DManager.h"
#include "DrawObject/Fbx/FbxLoader.h"
#include "RenderingPath/FirstPathObject.h"

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

Dx12Ctrl* Dx12Ctrl::inst = nullptr;

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
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

Dx12Ctrl::Dx12Ctrl() :mWndHeight(720), mWndWidth(1280),mClrcolor{0.5f,0.5f,0.5f,1.0f}
, mViewPort{ 0,0,static_cast<float>(mWndWidth),static_cast<float>(mWndHeight),0,1.0f},mRect{0,0,mWndWidth,mWndHeight }
,mCmdAllocator(nullptr),mCmdList(nullptr),mCmdQueue(nullptr),mFactory(nullptr)
,result(S_OK),mFenceValue(0)
,mWindowName("DirectX12")
{
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
#ifdef _DEBUG
	{
		ID3D12Debug* debug;
		D3D12GetDebugInterface(IID_PPV_ARGS(&debug));
		debug->EnableDebugLayer();
		debug->Release();
	}
#endif
	//
	mWinHInstance = winHInstance;
	//

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

	InitWindowCreate();

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

	mCamera = std::make_shared<Dx12Camera>(mWndWidth, mWndHeight);
	mCameraHolder = std::make_shared<CameraHolder>(mWndWidth, mWndHeight, mDev);
	mCameraHolder->CreateCamera(DirectX::XMFLOAT3(0, 20, -30), DirectX::XMFLOAT3(0.0f, -9.0f, 0.0f));

	//RendringManagerÉNÉâÉXÇÃèâä˙âªèàóù
	InitFirstPath();

	return true;
}

void Dx12Ctrl::InitFirstPath()
{
	RenderingPathManager::Instance().Init(mDev, mFactory, mhWnd);

	std::shared_ptr<RenderingPathObject> fpath = std::make_shared<FirstPathObject>(
		mDev, mDepthDescHeap, mWndWidth, mWndHeight);

	unsigned int renderingPathIndex;
	RenderingPathManager::Instance().AddRenderPath(fpath, renderingPathIndex);

	fpath->FirstUpdate();
}

void Dx12Ctrl::InitWindowCreate()
{
	WNDCLASSEX w = {};
	w.lpfnWndProc = (WNDPROC)WindowProcedure;
	w.lpszClassName = _T("DirectX12");
	w.hInstance = mWinHInstance;
	w.hIcon = LoadIcon(w.hInstance, _T("DirectX12"));
	w.cbSize = sizeof(WNDCLASSEX);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hIcon;
	RegisterClassEx(&w);

	RECT wrc = { 0,0,mWndWidth,mWndHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	const char* name = mWindowName.data();
	std::string strName;
	strName.reserve(mWindowName.size());
	for (auto& s : mWindowName)
	{
		strName.push_back(s);
	}
	strName.push_back('\0');
	size_t size = strName.size();
	wchar_t* buff = nullptr;
	ToWChar(&buff, size, strName.data(), size);

	HWND hwnd = CreateWindow(w.lpszClassName,
		buff,
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

	delete buff;

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
	mShaders.clear();
	mDepthDescHeap.reset();
	TextureLoader::Destroy();
	RenderingPathManager::Destroy();
	FbxLoader::Destroy();
	ImageLoader::Destroy();
	ShaderCompiler::Destroy();
	mCamera.reset();
	Primitive2DManager::Destory();
	mDepthBuffer.reset();
	mCmdAllocator.Reset();
	mCmdList.Reset();
	mCmdQueue.Reset();
	mFactory.Reset();
	mFence.Reset();
	ReportLiveObject();
}

void Dx12Ctrl::SetWindowSize(int inw, int inh)
{
	mWndWidth = inw;
	mWndHeight = inh;
	mViewPort.Width = static_cast<float>(inw);
	mViewPort.Height = static_cast<float>(inh);
	mRect.right = inw;
	mRect.bottom = inh;
}

void Dx12Ctrl::SetWindowName(std::string& inWindowName)
{
	mWindowName = inWindowName;
}

const HWND& Dx12Ctrl::GetWndHandle() const
{
	return mhWnd;
}

std::shared_ptr<DepthBufferObject> Dx12Ctrl::GetDepthBuffer() const
{
	return mDepthBuffer;
}

D3D12_CPU_DESCRIPTOR_HANDLE Dx12Ctrl::GetDepthCpuHandle() const
{
	return mDepthDescHeap->GetCPUHeapHandleStart();
}

std::shared_ptr<Dx12Camera> Dx12Ctrl::GetCamera() const
{
	return mCamera;
}

std::shared_ptr<CameraHolder> Dx12Ctrl::GetCameraHolder()
{
	return mCameraHolder;
}

const D3D12_VIEWPORT& Dx12Ctrl::GetViewPort() const
{
	return mViewPort;
}

const D3D12_RECT& Dx12Ctrl::GetRect() const
{
	return mRect;
}