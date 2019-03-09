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
class Dx12CommandList;
class Dx12BufferObject;

enum class DefaultPass
{
	SkyBox,
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
	std::shared_ptr<Dx12CommandList> GetRenderingPassCommandList(unsigned int passIndex) const;

	/**
	*	DefaultPassで指定
	*	パスのインデックスからそのパスのコマンドを積むべきコマンドリストを取得する
	*	一度でもRenderinsPassを編集すると有効なものが返ってくる保証はなくなる
	*/
	std::shared_ptr<Dx12CommandList> GetRenderingPassCommandList(DefaultPass passIndex) const;

	/**
	*	パスの名前からそのパスのコマンドを積むべきコマンドリストを取得する
	*	DeleteRenderingpassメソッドを呼んだ後だとインデックスが崩れるがコマンドリスト自体は変化しない
	*/
	std::shared_ptr<Dx12CommandList> GetRenderingPassCommandList(const std::string& passName) const;

	/**
	*	パスの名前からそのパスのインデックスを取得する
	*	レンダリングパスのインデックスを取得する
	*	DeleteRenderingpassメソッドを呼んだ後だとインデックスが崩れるので注意
	*/
	unsigned int GetRenderingPassIndex(const std::string& passName) const;

	std::shared_ptr<RenderingPassObject> GetRenderingPassObject(unsigned int passIndex) const;

	std::shared_ptr<RenderingPassObject> GetRenderingPassObject(const std::string& passName) const;

	/**
	*	@brief	パスを追加する 
	*	@note	FirstUpdateは呼ばれないので適宜呼ぶこと
	*	@param[in]	passObj	追加するパス
	*	@param[out]	out_passIndex	追加したパスのインデックス
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

	/**
	*	指定したレンダリングパスのコマンドリストをパス上から削除する
	*	これを実行した後はパスのインデックスが崩れるので注意
	*/
	bool DeleteRenderingPass(const std::string& passName);

	/**
	*	パスの全体の数を取得
	*/
	unsigned int GetNumCuurentPass() const;

	/**
	*	現在のスワップチェインのレンダーターゲットのヒープハンドルを取得する
	*/
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRTVHeapHandle() const;

	/**
	*	現在のスワップチェインのレンダーターゲットのヒープを取得する
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> GetCurrentRTVDescHeap() const;

	/**
	*	現在のスワップチェインのレンダーターゲットを取得する
	*/
	std::shared_ptr<Dx12BufferObject> RenderingPassManager::GetCurrentRenderTarget() const;

	/**
	*	パスのアクティブ、非アクティブを設定する
	*	@param[in]	renderingpass	レンダリングパスの番号
	*	@param[in]	isActive	ture:アクティブ	false:非アクティブ
	*/
	void SetIsActiveValue(unsigned int renderingpass, bool isActive);

	/**
	*	パスのアクティブ、非アクティブを設定する
	*	@param[in]	passName	レンダリングパスの名前
	*	@param[in]	isActive	ture:アクティブ	false:非アクティブ
	*/
	void SetIsActiveValue(std::string passName, bool isActive);

	/**
	*	すべてのパスを削除する
	*/
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
	std::shared_ptr<Dx12CommandList> mRenderCmdList;
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