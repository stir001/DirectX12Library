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
class RenderingPathObject;


/**
*レンダリングパスが順番に呼ばれることを保証する
*すべてのパスが終了した後コマンドリストをリセットする
*/
class RenderingPathManager
{
public:
	~RenderingPathManager();

	static RenderingPathManager& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new RenderingPathManager();
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
	*RenderingManagerの初期化処理
	*/
	void Init(Microsoft::WRL::ComPtr<ID3D12Device>& dev, Microsoft::WRL::ComPtr<IDXGIFactory4>& factory, HWND hwnd);

	void Render();

	///**
	//*パス番号は0から
	//*パスのインデックスからそのパスのコマンドを積むべきコマンドリストを取得する
	//*DeleteRenderingPathメソッドを呼んだ後だとインデックスが崩れるがコマンドリスト自体は変化しない
	//*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(unsigned int pathIndex) const;

	///**
	//*パスの名前からそのパスのコマンドを積むべきコマンドリストを取得する
	//*DeleteRenderingPathメソッドを呼んだ後だとインデックスが崩れるがコマンドリスト自体は変化しない
	//*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetRenderingPathCommandList(const std::string& pathName) const;

	/**
	*パスの名前からそのパスのインデックスを取得する
	*レンダリングパスのインデックスを取得する
	*DeleteRenderingPathメソッドを呼んだ後だとインデックスが崩れるので注意
	*/
	unsigned int GetRenderingPathIndex(const std::string& pathName) const;


	/**
	*/
	bool AddRenderPath(std::shared_ptr<RenderingPathObject>& pathObj, unsigned int& out_PathIndex);

	bool InsertRenderPath(std::shared_ptr<RenderingPathObject>& pathObj, unsigned int insertPathIndex);

	/**
	*指定したレンダリングパスのコマンドリストをパス上から削除する
	*これを実行した後はパスのインデックスが崩れるので注意
	*/
	bool DeleteRenderingPath(unsigned int pathIndex);
	bool DeleteRenderingPath(const std::string& pathName);

	/**
	*パスの全体の数を取得
	*/
	unsigned int GetNumCuurentPath() const;

	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTVHeapHandle() const;
	std::shared_ptr<Dx12DescriptorHeapObject> GetCurrentRTVDescHeap() const;

	Microsoft::WRL::ComPtr<ID3D12Resource> GetCurrentRenderTarget() const;

	void SetIsActiveValue(unsigned int renderingPath, bool isActive);
	void SetIsActiveValue(std::string pathName, bool isActive);

	void AllPathClear();

private:
	RenderingPathManager();
	RenderingPathManager(const RenderingPathManager&) = delete;
	RenderingPathManager(const RenderingPathManager&&) = delete;
	RenderingPathManager& operator=(const RenderingPathManager&) = delete;
	RenderingPathManager& operator=(const RenderingPathManager&&) = delete;

	static RenderingPathManager* mInstance;

	std::vector<std::shared_ptr<RenderingPathObject>> mRenderingPathObjects;

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mRenderCmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mRenderCmdList;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCmdQueue;
	Microsoft::WRL::ComPtr<ID3D12Device> mDevice;

	std::shared_ptr<SwapChainObject> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mFenceValue;

	unsigned int mWidth;
	unsigned int mHeight;

	unsigned int AddRenderingPathObject(std::shared_ptr<RenderingPathObject>& pathObj);

	void WaitCmdQueue();
	void CopyLastPathRenderTarget();
};