#pragma once
/**
*	@file DepthBufferObject.h
*	@brief DepthStencilBufferを作成するクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/10
*/
#include "Dx12BufferObject.h"
#include <d3d12.h>
#include <wrl.h>

/**
*	@ingroup Dx12Resource
*	@class DepthBufferObject
*	@brief DepthStencilBufferを作成する
*/
class DepthBufferObject :
	public Dx12BufferObject
{
public:
	/**
	*	@param[in]	name	バッファの名前
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	height	バッファの縦幅
	*	@param[in]	width	バッファの横幅
	*	@param[in]	format	バッファの一要素のDXGI_FORMAT
	*/
	DepthBufferObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		int height, int width, DXGI_FORMAT format = DXGI_FORMAT_D32_FLOAT);

	/**
	*	@param[in]	name		バッファの名前
	*	@param[in]	resource	すでに作成済みのコンスタントバッファ
	*	@param[in]	state		初期のD3D12_RESOURCE_STATES
	*/
	DepthBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& resource, D3D12_RESOURCE_STATES state);
	~DepthBufferObject();
	
	/**
	*	@brief	DepthStencilBufferを取得する
	*	@return	保持しているDepthStencilBuffer
	*/
	Microsoft::WRL::ComPtr<ID3D12Resource> GetDepthBuffer();
private:

	/**
	*	Mapをしてはいけないので何もしない関数にオーバーライドしている
	*/
	void Map();
};

