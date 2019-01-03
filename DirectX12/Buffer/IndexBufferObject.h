#pragma once
/**
*	@file IndexBufferObject.h
*	@brief	IndexBufferを作成するクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/11
*/
#include "Dx12BufferObject.h"

class Dx12CommandList;

/**
*	@ingroup Dx12Resource
*	@class IndexBufferObject
*	@brief 引数に応じてIndexBufferを作成する
*/
class IndexBufferObject :
	public Dx12BufferObject
{
public:
	/**
	*	@param[in]	name	バッファの名前
	*	@param[in]	elementsize	要素一個のサイズ
	*	@param[in]	elementcount	要素の総数
	*	@param[in]	format	Indexのサイズに合わせたDXGI_FORMAT
	*/
	IndexBufferObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, 
		unsigned int elementsize, unsigned int elementcount, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT);

	/**
	*	@param[in]	name	バッファの名前
	*	@param[in]	rsc		すでに作成しているIndexBuffer
	*/
	IndexBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc);
	~IndexBufferObject();

	/**
	*	indexBufferViewを取得する
	*/
	D3D12_INDEX_BUFFER_VIEW GetView() const;
private:
	D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
};

