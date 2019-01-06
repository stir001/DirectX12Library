#pragma once
#include "Base/RenderingPassObject.h"
class SkyBoxPass :
	public RenderingPassObject
{
public:
	SkyBoxPass();
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
};

