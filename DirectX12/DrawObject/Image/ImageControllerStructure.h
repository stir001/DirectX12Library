#pragma once
/**
*	@file ImageControllerStructure.h
*	@brief 2D�摜���������_���̒�`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2019/3/07
*/
#include <DirectXMath.h>

/**
*	@struct ImageVertex
*	@brief 2D�Ƃ��ĕ\������Ƃ��̒��_���
*/
struct ImageVertex
{
	/**
	*	���W
	*/
	DirectX::XMFLOAT4 pos;

	/**
	*	�K���}�l
	*/
	float gamma;

	/**
	*	���_ID
	*/
	unsigned int vertexID;
};

struct ImageUVSet
{
	/**
	*	uv���W
	*/
	DirectX::XMFLOAT2 uv[4];
};

/**
*	@struct	Image3DVertex
*	@brief	3D�Ƃ��ĕ\������Ƃ��̒��_���
*/
struct Image3DVertex
{
	/**
	*	���W
	*/
	DirectX::XMFLOAT4 pos;

	/**
	*	�@��
	*/
	DirectX::XMFLOAT4 normal;

	/**
	*	uv���W
	*/
	DirectX::XMFLOAT2 uv;

	/**
	*	�K���}�l
	*/
	float gamma;
};