#pragma once
/**
*	@file RendertargetObject.h
*	@brief Rendertarget���쐬����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/11
*/
#include "Dx12BufferObject.h"

/**
*	@ingroup Dx12Resource
*	@class RenderTargetObject
*	@brief RenderTarget���쐬����
*/
class RendertargetObject : public Dx12BufferObject
{
public:
	/**
	*	@param[in]	name	�o�b�t�@�̖��O
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	width	�o�b�t�@�̉���
	*	@param[in]	height	�o�b�t�@�̏c��
	*	@param[in]	format	�o�b�t�@�̈�v�f�ɑΉ�����DXGI_FORMAT
	*/
	RendertargetObject(const std::string& name,const Microsoft::WRL::ComPtr<ID3D12Device>& dev, 
		unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	/**
	*	@param[in]	name	�o�b�t�@�̖��O
	*	@param[in]	rsc		���łɍ쐬���Ă���RenderTarget
	*	@param[in]	state	�f�t�H���g�̃��\�[�X�X�e�[�g
	*/
	RendertargetObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc
	 , D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_RENDER_TARGET);
	~RendertargetObject();

	/**
	*	@brief	RenderTarget�����������邽�߂̃J���[��ݒ肷��
	*	@param[in]	clrValue	����������Ƃ��̒l
	*/
	void SetClearValue(D3D12_CLEAR_VALUE clrValue);

	/**
	*	@brief	����������Ƃ��̒l�𓾂�
	*	@return	���ݐݒ肵�Ă��鏉��������l
	*/
	D3D12_CLEAR_VALUE GetClearValue() const;
private:
	/**
	*	����������l��ێ�
	*/
	D3D12_CLEAR_VALUE mClearValue;
};

