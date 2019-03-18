#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <memory>
#include <vector>
#include <string>
#include <wrl.h>
#include <DirectXMath.h>

#ifdef _DEBUG
#include <cassert>
#endif // _DEBUG

#define DX12CTRL_INSTANCE Dx12Ctrl& d12 = Dx12Ctrl::Instance();

class SwapChainObject;
class DescriptorHeapManager;
class RootSignatureObject;
class TextureLoader;
class PipelineStateObject;
class TextureObject;
class DepthBufferObject;
class Dx12Camera;
class Dx12DescriptorHeapObject;
class CameraHolder;

namespace DirectX
{
 struct XMFLOAT2;
};

class Dx12Ctrl
{
public:
	~Dx12Ctrl();

	static Dx12Ctrl& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new Dx12Ctrl();
		}

		return *mInstance;
	}

	static void Destroy()
	{
		if (mInstance != nullptr)
		{
			mInstance->Release();
			delete mInstance;
			mInstance = nullptr;
		}
	}

	HRESULT result;

#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12DebugDevice> mDebugDevice;
#endif // _DEBUG


	Microsoft::WRL::ComPtr<ID3D12Device>& GetDev();
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& GetCmdAllocator();
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCmdQueue();
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& GetCmdList();
	Microsoft::WRL::ComPtr<IDXGIFactory4> GetFactory();
	std::shared_ptr<DepthBufferObject> GetDepthBuffer() const;
	std::shared_ptr<Dx12DescriptorHeapObject> GetDepthDescHeap() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetDepthCpuHandle() const;

	void CmdQueueSignal();
	UINT64 GetFenceValue() const;
	bool Dx12Init(HINSTANCE winHInstance);
	DirectX::XMFLOAT2 GetWindowSize() const;
	void SetWindowSize(int inw, int inh);
	void SetWindowName(std::string& inWindowName);

	const HWND& GetWindowHandle() const;

	HRESULT GetDeviceRemoveReason();
	HRESULT CheckResult();
	HRESULT CheckResult(HRESULT r);

	void Release();

	std::shared_ptr<Dx12Camera> GetCamera(unsigned int index) const;
	std::shared_ptr<CameraHolder> GetCameraHolder();

	template<typename T>
	LONG CheckComPtrRefCount(Microsoft::WRL::ComPtr<T>& comptr)
	{
		LONG count = comptr.Get()->AddRef() - 1;
		comptr.Get()->Release();
		return count;
	}

	void SetInstanceHandle(HINSTANCE& hInstance) {
		mWinHInstance = hInstance;
	};

	HRESULT ReportLiveObject();

	void SetWinProc(LRESULT(*proc)(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam));
	void UpdateWindowSize();
private:
	Dx12Ctrl();
	Dx12Ctrl(const Dx12Ctrl&);
	Dx12Ctrl(const Dx12Ctrl&&);
	Dx12Ctrl& operator=(const Dx12Ctrl&);

	static Dx12Ctrl* mInstance;

	LRESULT (*mWndProc)(HWND, UINT, WPARAM, LPARAM);
	HINSTANCE mWinHInstance;
	int mWndHeight;
	int mWndWidth;
	std::string mWindowName;
	Microsoft::WRL::ComPtr<ID3D12Device> mDev;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mCmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCmdQueue;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCmdList;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	Microsoft::WRL::ComPtr<IDXGIFactory4> mFactory;
	std::shared_ptr<DepthBufferObject> mDepthBuffer;
	std::shared_ptr<Dx12DescriptorHeapObject> mDepthDescHeap;
	std::shared_ptr<CameraHolder> mCameraHolder;
	UINT64 mFenceValue;
	HWND mhWnd;
	float mClrcolor[4];

	void InitWindowCreate();
	void InitRenderPath();
};