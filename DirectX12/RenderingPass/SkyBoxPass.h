#pragma once
#include "Base/RenderingPassObject.h"

class SkyBox;
class Dx12DescriptorHeapObject;
class RendertargetObject;
class CameraHolder;
struct ID3D12Device;
struct SkyBoxTextures;

class SkyBoxPass :
	public RenderingPassObject
{
public:
	SkyBoxPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		std::shared_ptr<Dx12DescriptorHeapObject> rtvHeap,
		std::shared_ptr<RendertargetObject> rendertarget,
		int width, int height, SkyBoxTextures& textures);
	~SkyBoxPass();

	/**
	*	各フレームの一番最初に実行される
	*/
	 void FirstUpdate();

	/**
	*	このパスのExcutePathが呼ばれる直前に実行される
	*	基本的にここでコマンドリストはcloseを呼ぶ
	*/
	 void PreExcuteUpdate();

	/**
	*	コマンドリストの実行をする。GPU待ちの責任は負わない
	*/
	 void ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue);

	/**
	*	ExcutePathの実行後に実行される。CommnadListのリセットをする
	*/
	 void ResetCommandList();

	/**
	*	コマンドリストを取得する
	*/
	 std::shared_ptr<Dx12CommandList> GetCommandList();

	 /**
	 *	最終レンダリング結果を返す関数　一番最後のパスのみ必須それ以外は実装しなくてもいい
	 */
	 virtual std::shared_ptr<Dx12BufferObject> GetRenderTarget();

private:
	std::shared_ptr<SkyBox> mSkyBox;
	std::shared_ptr<RendertargetObject> mRendertarget;
	std::shared_ptr<CameraHolder> mHolder;
	std::shared_ptr<Dx12DescriptorHeapObject> mRtvDescHeap;
	int mWndWidth;
	int mWndHeight;
};

