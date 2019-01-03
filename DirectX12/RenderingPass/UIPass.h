#pragma once
#include "Base\RenderingPassObject.h"
#include <d3d12.h>

class Dx12DescriptorHeapObject;
class RendertargetObject;

class UIPass :
	public RenderingPassObject
{
public:
	UIPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		std::shared_ptr<Dx12DescriptorHeapObject> rtvHeap,
		std::shared_ptr<RendertargetObject> rendertarget,
		int wWidth, int wHeight);
	~UIPass();

	/**
	*	各フレームの一番最初に実行される
	*/
	void FirstUpdate();

	/**
	*	このパスのExcutePathが呼ばれる直前に実行される
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
	*	最終レンダリング結果を返す関数　一番最後のパスのみ必須それ以外は実装しないでもいい
	*/
	std::shared_ptr<Dx12BufferObject> GetRenderTarget();

private:
	D3D12_VIEWPORT mViewPort;
	D3D12_RECT mSciddorRect;
	std::shared_ptr<Dx12DescriptorHeapObject> mRtvDescHeap;
	std::shared_ptr<RendertargetObject> mRendertarget;
};

