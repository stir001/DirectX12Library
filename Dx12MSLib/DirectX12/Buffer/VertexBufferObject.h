#pragma once
/**
*	@file VertexBufferObject.h
*	@brief VertexBuffer���쐬����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/11
*/

#include "Dx12BufferObject.h"

/**
*	@ingroup Dx12Resource
*	@class VertexBufferObject
*	@brief VertexBuffer���쐬���ĕێ�����
*/
class VertexBufferObject :
	public Dx12BufferObject
{
public:
	/**
	*	@param[in]	name	�o�b�t�@�̖��O
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	elementsize	�v�f��̃T�C�Y
	*	@param[in]	elementcount	�v�f�̑���
	*/
	VertexBufferObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, 
		unsigned int elementsize, unsigned int elementcount);

	/**
	*	@param[in]	name	�o�b�t�@�̖��O
	*	@param[in]	rsc		���łɍ쐬������Ă���VertexBuffer
	*	@param[in]	elementsize	�v�f��̃T�C�Y
	*	@param[in]	elementcount	�v�f�̑���
	*/
	VertexBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc,
		unsigned int elementSize, unsigned int elementCount);
	~VertexBufferObject();

	/**
	*	@brief	ID3D12GraphicsCommnadList��VertexBuffer���Z�b�g����
	*	@param[in]	cmdList	VertexBuffer���Z�b�g����R�}���h���X�g
	*/
	void SetBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList) const;

	/**
	*	@brief	VertexBufferView�𓾂�
	*	@return	���݂�VertexBufferView
	*/
	D3D12_VERTEX_BUFFER_VIEW GetView() const;
private:
	/**
	*	���݂�VertexBufferVIew
	*/
	D3D12_VERTEX_BUFFER_VIEW mVbView;
};

