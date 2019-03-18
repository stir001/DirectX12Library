#pragma once
/**
*	@addtogroup Dx12Resource
*	@file Dx12BufferObject.h
*	@brief ID3D12Resource�������N���X�̒�`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/10
*/

#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include <vector>
#include <string>

class Dx12BufferViewDesc;

/**
*	@ingroup Dx12Resource
*	@class Dx12BufferObject
*	@brief ID3D12Resource���������N���X
*			�e�r���[����邽�߂̊֐��������K�v�ɉ����ČĂяo�����Ƃ�view���쐬����
*/
class Dx12BufferObject
{
public:
	/**
	*	@param[in]	name	ID3D12Resource�ɂ��閼�O
	*/
	Dx12BufferObject(const std::string& name);

	/**
	*	@param[in]	rsc				���炩���ߍ���Ă��������̃N���X�ɃZ�b�g������Resource
	*	@param[in]	name			ID3D12Resource�ɂ��閼�O
	*	@param[in]	elementSize		�v�f���byte�P�ʂ̃T�C�Y
	*	@param[in]	elementCount	�v�f��������̂�
	*/
	Dx12BufferObject(Microsoft::WRL::ComPtr<ID3D12Resource>& rsc, const std::string& name,
		unsigned int elemenetSize, unsigned int elementCount);
	virtual ~Dx12BufferObject();

	/**
	*	@brief	pData�̗v�f��ID3D12Resource�Ƀ}�b�v����
	*	@param[in]	pData			�������ރf�[�^�̃|�C���^
	*	@param[in]	amountDataSize	data�S�̂̃o�C�g�P�ʂ̃T�C�Y
	*/
	void WriteBuffer(const void* pData, unsigned int amountDatasSize);

	/**
	*	@brief	pData�̗v�f��ID3D12Resource�ɗv�f���Ƃ�256�A���C�����g���Ȃ���}�b�v����
	*	@param[in]	pData		�f�[�^�̃|�C���^
	*	@param[in]	dataSize	�f�[�^��̃T�C�Y
	*	@param[in]	dataCount	�f�[�^�̑���
	*/
	void WriteBuffer256Alignment(const void* pData, unsigned int dataSize, unsigned int dataCount);

	/**
	*	D3D12_CONSTANT_BUFFER_VIEW_DESC���쐬����
	*/
	virtual void CreateConstantBufferViewDesc();

	/**
	*	D3D12_UNORDERD_ACCESS_VIEW_DESC���쐬����
	*/
	virtual void CreateUnorderdAccessViewDesc();

	/**
	*	D3D12_SHADER_RESOURCE_VIEW_DESC���쐬����
	*/
	virtual void CreateShaderResourceViewDesc();

	/**
	*	D3D12_RENDER_TARGET_VIEW_DESC���쐬����
	*/
	virtual void CreateRenderTargetViewDesc();

	/**
	*	D3D12_DEPTH_STENCIL_VIEW_DESC���쐬����
	*/
	virtual void CreateDepthStecilViewDesc();

	/**
	*	@brief	�ێ����Ă���ID3D12Resource�̎Q�Ƃ��擾����
	*	@return	ID3D12Resource��ComPtr�̎Q��
	*/
	Microsoft::WRL::ComPtr<ID3D12Resource>& GetBuffer();

	/**
	*	@brief	���݂�D3D12_RESOURCE_STATES���擾����
	*	@return	���݂�D3D12_RESOURCE_STATES
	*/
	D3D12_RESOURCE_STATES GetCurrentState() const;

	/**
	*	@brief	D3D12_RESOURCE_STATES��ݒ肷��
	*	@param[in]	state	�ݒ肷��D3D12_RESOURCE_STATES
	*/
	void SetCurrentState(D3D12_RESOURCE_STATES state);

	/**
	*	@brief	������Ԃ�D3D12_RESOURCE_STATES���擾����
	*	@return	������Ԃ�D3D12_RESOURCE_STATES
	*/
	D3D12_RESOURCE_STATES GetDefaultState() const;

	/**
	*	@brief	�쐬����ViewDesc���擾����
	*	@return	�쐬����Dx12BufferViewDesc
	*/
	const std::shared_ptr<Dx12BufferViewDesc>& GetViewDesc() const;

	/**
	*	@brief	�o�b�t�@�̖��O���擾����
	*	@return	�o�b�t�@�̖��O
	*/
	std::string GetName() const;

protected:
	/**
	*	�ێ����Ă���ID3D12Resource
	*/
	Microsoft::WRL::ComPtr<ID3D12Resource> mBuffer;

	/**
	*	���݂̃X�e�[�g
	*/
	D3D12_RESOURCE_STATES mCurrentState;

	/**
	*	�����X�e�[�g
	*/
	D3D12_RESOURCE_STATES mDeafultState;

	/**
	*	�쐬����ViewDesc
	*/
	std::shared_ptr<Dx12BufferViewDesc> mViewDescs;

	/**
	*	ID3D12Resource�̖��O
	*/
	std::wstring mName;

	/**
	*	�v�f���byte�P�ʂ̃T�C�Y
	*/
	unsigned int mElementSize;

	/**
	*	�v�f�̑���
	*/
	unsigned int mElementCount;

	/**
	*	ID3D12Resource���N���A����Ƃ��ɒl
	*/
	D3D12_CLEAR_VALUE mClearValue;

#ifdef _DEBUG
	/**
	*	�f�o�b�O���p�@�v���p�e�B�ꎞ�ۑ�
	*/
	D3D12_HEAP_PROPERTIES mHeapProp;

	/**
	*	�f�o�b�O���p�@Desc�ꎞ�ۑ��p
	*/
	D3D12_RESOURCE_DESC mRscDesc;
#endif
	/**
	*	�v�f���}�b�v����Ƃ��Ɏg�p����̈�
	*/
	void* mElementBuffer;

	/**
	*	�v�f���}�b�v����
	*/
	virtual void Map();
};

