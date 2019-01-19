#pragma once
#include "Base/RenderingPassObject.h"
#include <memory>
#include <wrl.h>

class Dx12BufferObject;
class LightObject;
class Dx12DescriptorHeapObject;
struct ID3D12Device;

class ShadowmapPass :
	public RenderingPassObject
{
public:
	ShadowmapPass(const Microsoft::WRL::ComPtr<ID3D12Device>& dev);
	~ShadowmapPass();

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
	void ExecutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue);

	/**
	*	ExcutePathの実行後に実行される。CommnadListのリセットをする
	*/
	void ResetCommandList();

	/**
	*	コマンドリストを取得する
	*/
	std::shared_ptr<Dx12CommandList> GetCommandList();

	/**
	*	@brief	シャドウマップテクスチャを取得する
	*	@return	shadowmap
	*/
	std::shared_ptr<Dx12BufferObject> GetShadowmap();
private:
	std::shared_ptr<Dx12BufferObject> mShadowmapDepth;
	std::shared_ptr<LightObject> mDirectionalLight;
	std::shared_ptr<Dx12DescriptorHeapObject> mShadowDepthDescHeap;
	float mBufferSize;
};

