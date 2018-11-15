#pragma once
/**
*	@file ShaderResourceObject.h
*	@brief ShaderResourceを作成するクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/11
*/

#include "Dx12BufferObject.h"

/**
*	@ingroup Dx12Resource
*	@class ShaderResourceObject
*	@brief ShaderResourceを作成し保持する
*/
class ShaderResourceObject :
	public Dx12BufferObject
{
public:
	/**
	*	@param[in]	name	バッファの名前
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	width	バッファの横幅
	*	@param[in]	height	バッファの縦幅
	*	@param[in]	format	要素一個に対応するDXGI_FORMAT
	*/
	ShaderResourceObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	/**
	*	@param[in]	name	バッファの名前
	*	@param[in]	rsc		すでに作成れているShaderResource
	*/
	ShaderResourceObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc);
	~ShaderResourceObject();

private:
	/**
	*	初期化するときの値
	*/
	D3D12_CLEAR_VALUE mClearValue;
};

