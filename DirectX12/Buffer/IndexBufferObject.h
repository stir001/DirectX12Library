#pragma once
/**
*	@file IndexBufferObject.h
*	@brief	IndexBuffer���쐬����N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/11
*/
#include "Dx12BufferObject.h"

class Dx12CommandList;

/**
*	@ingroup Dx12Resource
*	@class IndexBufferObject
*	@brief �����ɉ�����IndexBuffer���쐬����
*/
class IndexBufferObject :
	public Dx12BufferObject
{
public:
	/**
	*	@param[in]	name	�o�b�t�@�̖��O
	*	@param[in]	elementsize	�v�f��̃T�C�Y
	*	@param[in]	elementcount	�v�f�̑���
	*	@param[in]	format	Index�̃T�C�Y�ɍ��킹��DXGI_FORMAT
	*/
	IndexBufferObject(const std::string& name, const Microsoft::WRL::ComPtr<ID3D12Device>& dev, 
		unsigned int elementsize, unsigned int elementcount, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT);

	/**
	*	@param[in]	name	�o�b�t�@�̖��O
	*	@param[in]	rsc		���łɍ쐬���Ă���IndexBuffer
	*/
	IndexBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc);
	~IndexBufferObject();

	/**
	*	indexBufferView���擾����
	*/
	D3D12_INDEX_BUFFER_VIEW GetView() const;
private:
	D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
};

