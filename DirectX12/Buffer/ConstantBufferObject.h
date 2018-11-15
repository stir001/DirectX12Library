#pragma once
/**
*	@file ConstantBufferObject.h
*	@brief ID3D12Resourceのコンスタントバッファの扱いを定義する
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/09/10
*/

#include "Dx12BufferObject.h"
#include <functional>

/**
*	@ingroup Dx12Resource
*	@class ConstantBufferObject
*	@brief ConstantBufferを引数に基づいて作成する
*/
class ConstantBufferObject :
	public Dx12BufferObject
{
public:
	/**
	*	@param[in]	name	コンスタントバッファの名前
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	elementsize	書き込む要素一個のサイズ
	*	@param[in]	elementcount	書き込む要素の総数
	*/
	ConstantBufferObject(const std::string& name,const Microsoft::WRL::ComPtr<ID3D12Device>& dev, 
		unsigned int elementsize, unsigned int elementcount);

	/**
	*	@param[in]	name	コンスタントバッファの名前
	*	@param[in]	rsc		すでに作成済みのコンスタントバッファ
	*	@param[in]	state		初期のD3D12_RESOURCE_STATES
	*	@param[in]	elementsize	書き込む要素一個のサイズ
	*	@param[in]	elementcount	書き込む要素の総数
	*/
	ConstantBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc,
		D3D12_RESOURCE_STATES state, unsigned int elementsize, unsigned int elementcount);
	~ConstantBufferObject();

	/**
	*	コンスタントバッファを更新する
	*/
	void UpdateBuffer();
	
protected:
	/**
	*	特別な更新処理を追加する
	*/
	void SetUpdate(std::function<void(void)>& updataFunc);

private:
	/**
	*	特別な場合の更新処理
	*/
	std::function<void(void)> mSetBufferUpdate;
};

