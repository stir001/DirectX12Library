#pragma once
/**
*	@file RendertargetObject.h
*	@brief Rendertargetを作成するクラス
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2018/9/11
*/
#include "Dx12BufferObject.h"

/**
*	@ingroup Dx12Resource
*	@class RenderTargetObject
*	@brief RenderTargetを作成する
*/
class RendertargetObject : public Dx12BufferObject
{
public:
	/**
	*	@param[in]	name	バッファの名前
	*	@param[in]	dev		ID3D12Deviceの参照
	*	@param[in]	width	バッファの横幅
	*	@param[in]	height	バッファの縦幅
	*	@param[in]	format	バッファの一要素に対応したDXGI_FORMAT
	*/
	RendertargetObject(const std::string& name,const Microsoft::WRL::ComPtr<ID3D12Device>& dev, 
		unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	/**
	*	@param[in]	name	バッファの名前
	*	@param[in]	rsc		すでに作成しているRenderTarget
	*/
	RendertargetObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc);
	~RendertargetObject();

	/**
	*	@brief	RenderTargetを初期化するためのカラーを設定する
	*	@param[in]	clrValue	初期化するときの値
	*/
	void SetClearValue(D3D12_CLEAR_VALUE clrValue);

	/**
	*	@brief	初期化するときの値を得る
	*	@return	現在設定している初期化する値
	*/
	D3D12_CLEAR_VALUE GetClearValue() const;
private:
	/**
	*	初期化する値を保持
	*/
	D3D12_CLEAR_VALUE mClearValue;
};

