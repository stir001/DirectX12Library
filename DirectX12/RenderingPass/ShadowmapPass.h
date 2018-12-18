#pragma once
#include "Base/RenderingPassObject.h"
#include <memory>

class DepthBufferObject;
class LightObject;

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
	void ExcutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue);

	/**
	*	ExcutePathの実行後に実行される。CommnadListのリセットをする
	*/
	void ResetCommandList();

	/**
	*	コマンドリストを取得する
	*/
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList();
private:
	std::shared_ptr<DepthBufferObject> mShadowmapDepth;
	std::shared_ptr<LightObject> mDirectionalLight;
};

