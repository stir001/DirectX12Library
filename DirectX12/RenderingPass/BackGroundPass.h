#pragma once
#include "Base\RenderingPassObject.h"
#include <d3d12.h>

class RendertargetObject;
class Dx12DescriptorHeapObject;

class BackGroundPass :
	public RenderingPassObject
{
public:
	BackGroundPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		std::shared_ptr<Dx12DescriptorHeapObject> rtvHeap,
		std::shared_ptr<RendertargetObject> rendertarget,
		int wWidth, int wHeight);
	~BackGroundPass();

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
	*	Rendertargetクラスを取得する
	*/	
	std::shared_ptr<RendertargetObject> GetRendertargetObject();

	/**
	*	DescriptorHeapクラスを取得する
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> GetRtvDescHeapObject();

private:
	std::shared_ptr<RendertargetObject> mRendertarget;
	std::shared_ptr<Dx12DescriptorHeapObject> mRtvDescHeap;
	D3D12_VIEWPORT mViewPort;
	D3D12_RECT mScissorRect;
};

