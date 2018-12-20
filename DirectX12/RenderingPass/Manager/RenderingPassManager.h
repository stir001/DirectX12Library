#pragma once
#include <vector>
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <string>
#include <dxgi1_4.h>
#include <functional>

class SwapChainObject;
class Dx12DescriptorHeapObject;
class RenderingPassObject;


enum class DefaultPass
{
	BackGround,
	Model,
	UI,
	Max
};

/**
*レンダリングパスが順番に呼ばれることを保証する
*すべてのパスが終了した後コマンドリストをリセットする
*/
class RenderingPassManager
{
public:
	~RenderingPassManager();

	static RenderingPassManager& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new RenderingPassManager();
		}
		return *mInstance;
	}

	static void Destroy()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}

	/**
	*	RenderingManagerの初期化処理
	*/
	void Init(Microsoft::WRL::ComPtr<ID3D12Device>& dev, Microsoft::WRL::ComPtr<IDXGIFactory4>& factory, HWND hwnd);

	void Render();

	/**
	*	パス番号は0から
	*	パスのインデックスからそのパスのコマンドを積むべきコマンドリストを取得する
	*	DeleteRenderingpassメソッドを呼んだ後だとインデックスが崩れるがコマンドリスト自体は変化しない
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPassCommandList(unsigned int passIndex) const;

	/**
	*	パスの名前からそのパスのコマンドを積むべきコマンドリストを取得する
	*	DeleteRenderingpassメソッドを呼んだ後だとインデックスが崩れるがコマンドリスト自体は変化しない
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPassCommandList(const std::string& passName) const;

	/**
	*	パスの名前からそのパスのインデックスを取得する
	*	レンダリングパスのインデックスを取得する
	*	DeleteRenderingpassメソッドを呼んだ後だとインデックスが崩れるので注意
	*/
	unsigned int GetRenderingPassIndex(const std::string& passName) const;


	/**
	*
	*/
	bool AddRenderingPass(const std::shared_ptr<RenderingPassObject>& passObj, unsigned int& out_PassIndex);

	/**
	*	@brief	パスを指定の場所に挿入する
	*/
	bool InsertRenderingPass(std::shared_ptr<RenderingPassObject>& passObj, unsigned int insertPassIndex);

	/**
	*	指定したレンダリングパスのコマンドリストをパス上から削除する
	*	これを実行した後はパスのインデックスが崩れるので注意
	*/
	bool DeleteRenderingPass(unsigned int passIndex);
	bool DeleteRenderingPass(const std::string& passName);

	/**
	*	パスの全体の数を取得
	*/
	unsigned int GetNumCuurentPass() const;

	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTVHeapHandle() const;
	std::shared_ptr<Dx12DescriptorHeapObject> GetCurrentRTVDescHeap() const;

	Microsoft::WRL::ComPtr<ID3D12Resource> GetCurrentRenderTarget() const;

	void SetIsActiveValue(unsigned int renderingpass, bool isActive);
	void SetIsActiveValue(std::string passName, bool isActive);

	void AllPassClear();

private:
	RenderingPassManager();
	RenderingPassManager(const RenderingPassManager&) = delete;
	RenderingPassManager(const RenderingPassManager&&) = delete;
	RenderingPassManager& operator=(const RenderingPassManager&) = delete;
	RenderingPassManager& operator=(const RenderingPassManager&&) = delete;

	static RenderingPassManager* mInstance;

	std::vector<std::shared_ptr<RenderingPassObject>> mRenderingPassObjects;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mRenderCmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mRenderCmdList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCmdQueue;
	Microsoft::WRL::ComPtr<ID3D12Device> mDevice;

	std::shared_ptr<SwapChainObject> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mFenceValue;

	unsigned int mWidth;
	unsigned int mHeight;

	unsigned int AddRenderingPassObject(const std::shared_ptr<RenderingPassObject>& passObj);

	void WaitCmdQueue();
	void CopyLastPassRenderTarget();
};