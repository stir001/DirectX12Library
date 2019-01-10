#pragma once
#include "Base/RenderingPassObject.h"
#include <d3d12.h>
#include <vector>

class Dx12DescriptorHeapObject;
class RendertargetObject;
class CameraHolder;

class ModelPass :
	public RenderingPassObject
{
public:
	ModelPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev, std::shared_ptr<Dx12DescriptorHeapObject> depthHeap,
		 std::shared_ptr<Dx12DescriptorHeapObject> rtvHeap,
		 int wWidth, int wHeight, std::shared_ptr<CameraHolder> holder);
	~ModelPass();

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

private:
	D3D12_RECT mWndRect;
	std::shared_ptr<CameraHolder> mCameraHolder;
	std::shared_ptr<Dx12DescriptorHeapObject> mDepthHeap;
	std::shared_ptr<Dx12DescriptorHeapObject> mRtvHeap;
};

