#pragma once
/**
*	@file ImageControllerStructure.h
*	@brief 2D画像を扱う頂点情報の定義
*
*	@author 真鍋奨一郎
*
*	@par 最終更新日	2019/3/07
*/
#include <DirectXMath.h>

/**
*	@struct ImageVertex
*	@brief 2Dとして表示するときの頂点情報
*/
struct ImageVertex
{
	/**
	*	座標
	*/
	DirectX::XMFLOAT4 pos;

	/**
	*	ガンマ値
	*/
	float gamma;

	/**
	*	頂点ID
	*/
	unsigned int vertexID;
};

struct ImageUVSet
{
	/**
	*	uv座標
	*/
	DirectX::XMFLOAT2 uv[4];
};

/**
*	@struct	Image3DVertex
*	@brief	3Dとして表示するときの頂点情報
*/
struct Image3DVertex
{
	/**
	*	座標
	*/
	DirectX::XMFLOAT4 pos;

	/**
	*	法線
	*/
	DirectX::XMFLOAT4 normal;

	/**
	*	uv座標
	*/
	DirectX::XMFLOAT2 uv;

	/**
	*	ガンマ値
	*/
	float gamma;
};