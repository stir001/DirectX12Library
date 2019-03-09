#pragma once
/**
*	@file ConstantBufferObject.h
*	@brief ID3D12Resource�̃R���X�^���g�o�b�t�@�̈������`����
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/09/10
*/

#include "Dx12BufferObject.h"
#include <functional>

/**
*	@ingroup Dx12Resource
*	@class ConstantBufferObject
*	@brief ConstantBuffer�������Ɋ�Â��č쐬����
*/
class ConstantBufferObject :
	public Dx12BufferObject
{
public:
	/**
	*	@param[in]	name	�R���X�^���g�o�b�t�@�̖��O
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	elementsize	�������ޗv�f��̃T�C�Y
	*	@param[in]	elementcount	�������ޗv�f�̑���
	*/
	ConstantBufferObject(const std::string& name,const Microsoft::WRL::ComPtr<ID3D12Device>& dev, 
		unsigned int elementsize, unsigned int elementcount);

	/**
	*	@param[in]	name	�R���X�^���g�o�b�t�@�̖��O
	*	@param[in]	rsc		���łɍ쐬�ς݂̃R���X�^���g�o�b�t�@
	*	@param[in]	state		������D3D12_RESOURCE_STATES
	*	@param[in]	elementsize	�������ޗv�f��̃T�C�Y
	*	@param[in]	elementcount	�������ޗv�f�̑���
	*/
	ConstantBufferObject(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Resource>& rsc,
		D3D12_RESOURCE_STATES state, unsigned int elementsize, unsigned int elementcount);
	~ConstantBufferObject();

	/**
	*	�R���X�^���g�o�b�t�@���X�V����
	*/
	void UpdateInstanceMatrix();
	
protected:
	/**
	*	���ʂȍX�V������ǉ�����
	*/
	void SetUpdate(std::function<void(void)>& updataFunc);

private:
	/**
	*	���ʂȏꍇ�̍X�V����
	*/
	std::function<void(void)> mSetBufferUpdate;
};

