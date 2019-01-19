#pragma once
#include <wrl.h>
#include <memory>
#include <string>

struct ID3D12CommandQueue;
struct ID3D12GraphicsCommandList;
struct ID3D12Resource;

class Dx12CommandList;
class Dx12BufferObject;

class RenderingPassObject
{
public:
	RenderingPassObject(const std::string& pathName);
	virtual ~RenderingPassObject();

	/**
	*	各フレームの一番最初に実行される
	*/
	virtual void FirstUpdate() = 0;	

	/**
	*	このパスのExcutePathが呼ばれる直前に実行される
	*	基本的にここでコマンドリストはcloseを呼ぶ
	*/
	virtual void PreExcuteUpdate() = 0;

	/**
	*	コマンドリストの実行をする。GPU待ちの責任は負わない
	*/
	virtual void ExecutePath(Microsoft::WRL::ComPtr<ID3D12CommandQueue>& cmdQueue) = 0;

	/**
	*	ExcutePathの実行後に実行される。CommnadListのリセットをする
	*/
	virtual void ResetCommandList() = 0;

	/**
	*	コマンドリストを取得する
	*/
	virtual std::shared_ptr<Dx12CommandList> GetCommandList() = 0;

	/**
	*	最終レンダリング結果を返す関数　一番最後のパスのみ必須それ以外は実装しなくてもいい
	*/
	virtual std::shared_ptr<Dx12BufferObject> GetRenderTarget();

	/**
	*	このパスを有効にするか無効にするかの設定を行う
	*/
	virtual void SetActive(bool isActive);

	/**
	*	このパスが有効か無効かを返す:true 有効, false 無効
	*/
	virtual bool IsActive() const;

	const std::string& GetPassName() const;
protected:
	std::shared_ptr<Dx12CommandList> mCmdList;
	std::string mPassName;
private:
	bool mIsActive;
};

