#pragma once
/**
*	@file ShaderResourceObject.h
*	@brief ShaderResource���쐬����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/11
*/

#include "Dx12BufferObject.h"

/**
*	@ingroup Dx12Resource
*	@class ShaderResourceObject
*	@brief ShaderResource���쐬���ێ�����
*/
class ShaderResourceObject :
	public Dx12BufferObject
{
public:
	/**
	*	@param[in]	name	�o�b�t�@�̖��O
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	width	�o�b�t�@�̉���
	*	@param[in]	height	�o�b�t�@�̏c��
	*	@param[in]	format	�v�f��ɑΉ�����DXGI_FORMAT
	*/
	ShaderResourceObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	/**
	*	@param[in]	name	�o�b�t�@�̖��O
	*	@param[in]	rsc		���łɍ쐬��Ă���ShaderResource
	*/
	ShaderResourceObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc);
	~ShaderResourceObject();

private:
	/**
	*	����������Ƃ��̒l
	*/
	D3D12_CLEAR_VALUE mClearValue;
};

