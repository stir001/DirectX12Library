#pragma once
#include "ConstantBufferObject.h"
#include <vector>
/**
*	@file AdditionalConstantBuffer.h
*	@brief �ォ��ǉ�����R���X�^���g�o�b�t�@
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/10
*/

#include <functional>
#include <d3d12.h>
#include <wrl.h>

/**
*	@ingroup Dx12Resoruce
*	@class AdditionalConstatntBuffer
*	@brief �ォ��ǉ�����R���X�^���g�o�b�t�@
*/
template <typename T>
class AdditionalConstatntBuffer : public ConstantBufferObject
{
public:
	/**
	*	@param[in]	name	�R���X�^���g�o�b�t�@�̖��O
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	data	�������ރf�[�^�̃|�C���^
	*/
	AdditionalConstatntBuffer(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Device>& dev, T* data)
		: ConstantBufferObject(name, dev, sizeof(*data), 1),
		mData(data)
	{
		mWriteBuffer = [&]() {
			WriteBuffer256Alignment(mData, sizeof(*mData), 1);
		};
		mWriteBuffer();
		SetUpdate(mWriteBuffer);
	}

	/**
	*	@param[in]	name	�R���X�^���g�o�b�t�@�̖��O
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	data	�������ރf�[�^��vector
	*/
	AdditionalConstatntBuffer(const std::string& name, Microsoft::WRL::ComPtr<ID3D12Device>& dev, std::vector<T>* data)
		:ConstantBufferObject(name, dev, sizeof(data[0], data.size())),
		mVectorDatas(data)
	{
		mWriteBuffer = [&]() {
			WriteBuffer256Alignment(&(*mVectorDatas)[0], sizeof((*mVectorDatas)[0]), (*mVectorDatas).size());
		};

		mWriteBuffer();
		SetUpdate(mWriteBuffer);
	}

	~AdditionalConstatntBuffer()
	{
	}

private:
	/**
	*	�R���X�^���g�o�b�t�@�ɏ������ނ��߂̃����_��
	*/
	std::function<void(void)> mWriteBuffer;

	/**
	*	�������ރf�[�^
	*/
	T* mData;

	/**
	*	�������ރf�[�^��vector
	*/
	std::vector<T>* mVectorDatas;
};

