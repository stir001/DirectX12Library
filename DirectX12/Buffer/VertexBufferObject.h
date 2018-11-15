#pragma once
/**
*	@file VertexBufferObject.h
*	@brief VertexBufferを作成するクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/11
*/

#include "Dx12BufferObject.h"

/**
*	@ingroup Dx12Resource
*	@class VertexBufferObject
*	@brief VertexBufferを作成して保持する
*/
class VertexBufferObject :
	public Dx12BufferObject
{
public:
	/**
	*	@param[in]	name	バッファの名前
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	elementsize	要素一つのサイズ
	*	@param[in]	elementcount	要素の総数
	*/
	VertexBufferObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, 
		unsigned int elementsize, unsigned int elementcount);

	/**
	*	@param[in]	name	バッファの名前
	*	@param[in]	rsc		すでに作成さえれているVertexBuffer
	*	@param[in]	elementsize	要素一つのサイズ
	*	@param[in]	elementcount	要素の総数
	*/
	VertexBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc,
		unsigned int elementSize, unsigned int elementCount);
	~VertexBufferObject();

	/**
	*	@brief	ID3D12GraphicsCommnadListにVertexBufferをセットする
	*	@param[in]	cmdList	VertexBufferをセットするコマンドリスト
	*/
	void SetBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const;

	/**
	*	@brief	VertexBufferViewを得る
	*	@return	現在のVertexBufferView
	*/
	D3D12_VERTEX_BUFFER_VIEW GetView() const;
private:
	/**
	*	現在のVertexBufferVIew
	*/
	D3D12_VERTEX_BUFFER_VIEW mVbView;
};

