#pragma once
/**
*	@file DepthBufferObject.h
*	@brief DepthStencilBuffer���쐬����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/10
*/
#include "Dx12BufferObject.h"
#include <d3d12.h>
#include <wrl.h>

/**
*	@ingroup Dx12Resource
*	@class DepthBufferObject
*	@brief DepthStencilBuffer���쐬����
*/
class DepthBufferObject :
	public Dx12BufferObject
{
public:
	/**
	*	@param[in]	name	�o�b�t�@�̖��O
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	height	�o�b�t�@�̏c��
	*	@param[in]	width	�o�b�t�@�̉���
	*	@param[in]	format	�o�b�t�@�̈�v�f��DXGI_FORMAT
	*/
	DepthBufferObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		int height, int width, DXGI_FORMAT format = DXGI_FORMAT_D32_FLOAT);

	/**
	*	@param[in]	name		�o�b�t�@�̖��O
	*	@param[in]	resource	���łɍ쐬�ς݂̃R���X�^���g�o�b�t�@
	*	@param[in]	state		������D3D12_RESOURCE_STATES
	*/
	DepthBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& resource, D3D12_RESOURCE_STATES state);
	~DepthBufferObject();
	
	/**
	*	@brief	DepthStencilBuffer���擾����
	*	@return	�ێ����Ă���DepthStencilBuffer
	*/
	Microsoft::WRL::ComPtr<ID3D12Resource> GetDepthBuffer();
private:

	/**
	*	Map�����Ă͂����Ȃ��̂ŉ������Ȃ��֐��ɃI�[�o�[���C�h���Ă���
	*/
	void Map();
};

