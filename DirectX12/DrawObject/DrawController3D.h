#pragma once
/**
*	@file DrawController3D.h
*	@brief 3D�Ƃ��đ��삷�邽�߂̃R���g���[���[�̒�`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/15
*/
#include "DrawObjectController.h"
#include <DirectXMath.h>

class ConstantBufferObject;
class IndexBufferObject;
class VertexBufferObject;
class Dx12DescriptorHeapObject;

/**
*	@ingroup DrawObjectController
*	@class DrawController3D
*	@brief 3D�Ƃ��đ��삷�邽�߂̊֐����`
*/
class DrawController3D :
	public DrawObjectController
{
public:
	/**
	*	@param[in]	modelName	���삷����̖̂��O
	*	@param[in]	dev		ID3D12Device�̎Q��
	*	@param[in]	cmdList	�`��̃R�}���h��ςރR�}���h���X�g
	*/
	DrawController3D(const std::string& modelName, const Microsoft::WRL::ComPtr<ID3D12Device>& dev,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& cmdList);
	virtual ~DrawController3D();

	/**
	*	DescriptorHeap���č\�z����
	*/
	virtual void UpdateDescriptorHeap() = 0;

	/**
	*	@brief �J�����o�b�t�@��ݒ肷��
	*	@param cameraBuffer	�J�������ێ��p�o�b�t�@
	*/
	virtual void SetCameraBuffer(std::shared_ptr<ConstantBufferObject> cameraBuffer);

	/**
	*	@brief	��_���w��̏ꏊ�ɕ��s�ړ�������
	*	@param[in]	pos		���s�ړ����������ꏊ
	*/
	virtual void SetPosition(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	x,y,z���{�Ŋg�嗦��ݒ肷��
	*	@param[in]	�ݒ肵�����g�嗦
	*/
	virtual void SetScale(float scale);

	/**
	*	@brief x,y,z�e�����Ŋg�嗦��ݒ肷��
	*	@param[in]	scaleX	x�̊g�嗦
	*	@param[in]	scaleY	y�̊g�嗦
	*	@param[in]	scaleZ	z�̊g�嗦
	*/
	virtual void SetScale(float scaleX, float scaleY, float scaleZ);

	/**
	*	@brief	x�����S�Œǉ��ŉ�]������
	*	@param[in]	deg		��]��(degree,�x���@)
	*/
	virtual void AddRotaX(float deg);

	/**
	*	@brief	y�����S�Œǉ��ŉ�]������
	*	@param[in]	deg		��]��(degree,�x���@)
	*/
	virtual void AddRotaY(float deg);

	/**
	*	@brief	z�����S�Œǉ��ŉ�]������
	*	@param[in]	deg		��]��(degree,�x���@)
	*/
	virtual void AddRotaZ(float deg);

	/**
	*	@brief	�C�ӂ̎��ŉ�]����l������ݒ肷��
	*	@param[in]	quaternion	���Ɖ�]�ʂ�\���l����
	*/
	virtual void SetRotaQuaternion(const DirectX::XMFLOAT4& quaternion);

	/**
	*	@brief	�{�[���̐F��ݒ肷��
	*	@param[in]	color	�ݒ肷��F
	*/
	virtual void SetSkeletonColor(DirectX::XMFLOAT4& color);

	/**
	*	@brief	�{�[���̐F��ݒ肷��
	*	@param[in]	color	�ݒ肷��F
	*/
	virtual void SetSkeletonColor(float r, float g, float b, float a = 1.0f);

protected:
	/**
	*	mModelMatrix(�A�t�B���s��)���X�V����
	*/
	void UpdateMatrix();

	/**
	*/
	void UpdateSkeltonColor();

	/**
	*	�A�t�B���s����������ރo�b�t�@
	*/
	std::shared_ptr<ConstantBufferObject> mModelMatrixBuffer;

	/**
	*	�A�t�B���s���ێ�
	*/
	DirectX::XMFLOAT4X4 mModelMatrix;

	/**
	*	�g�嗦
	*/
	DirectX::XMFLOAT3 mScale;

	/**
	*	��_�̍��W
	*/
	DirectX::XMFLOAT3 mPos;

	/**
	*	��]�s��
	*/
	DirectX::XMFLOAT4X4 mRotationMatrix;

	/**
	*	��]��\���l����
	*/
	DirectX::XMFLOAT4 mQuaternion;

	/**
	*	�{�[���̐F���
	*/
	DirectX::XMFLOAT4 mSkeletonColor;

	/**
	*	����\������Ƃ��Ɏg�p����rootsignature
	*/
	std::shared_ptr<RootSignatureObject> mSkeletonRootsignature;

	/**
	*	����\������Ƃ��Ɏg�p����pipelinestate
	*/
	std::shared_ptr<PipelineStateObject> mSkeletonPipelineState;

	/**
	*	���p�̃C���f�b�N�X�o�b�t�@
	*/
	std::shared_ptr<IndexBufferObject> mSkeletonIndexBuffer;

	/**
	*	���p�̒��_�o�b�t�@
	*/
	std::shared_ptr<VertexBufferObject> mSkeletonVertexBuffer;

	/**
	*	���p�̐F���o�b�t�@
	*/
	std::shared_ptr<ConstantBufferObject> mSkeletonColorConstantBuffer;

	/**
	*	���p��DescriptorHeap
	*/
	std::shared_ptr<Dx12DescriptorHeapObject> mSkeletonHeap;

	/**
	*	�J�����p�R���X�^���g�o�b�t�@
	*/
	std::shared_ptr<ConstantBufferObject> mCameraBuffer;
};

