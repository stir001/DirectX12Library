#pragma once
/**
*	@file Transform3DCalculator.h
*	@brief 3D�`��I�u�W�F�N�g�𑀍삷��Ƃ��Ɏg���v�Z������N���X�̒�`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/21
*/
#include "TransformMatrixCalculator.h"

/**
*	@ingroup Calculator
*	@class Transform3DCalculator
*	@brief 3D�`��I�u�W�F�N�g�𑀍삷��Ƃ��Ɏg���v�Z������
*/
class Transform3DCalculator : public TransformMatrixCalculator
{
public:
	Transform3DCalculator();
	~Transform3DCalculator();

	/**
	*	@brief	���s�ړ��s����v�Z�����݂̍s��ɂ�����
	*	@param[in]	pos		���s�ړ�����
	*/
	void AddPositon(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	�g��k���s����v�Z�����݂̍s��ɂ�����
	*	@param[in]	scale	�g�嗦
	*/
	void AddScale(const DirectX::XMFLOAT3& scale);

	/**
	*	@brief	x�����S�̉�]�s����v�Z�����݂̍s��ɂ�����
	*	@param[in]	deg		��]��(degree,�x���@)
	*/
	void AddRotaX(float deg);

	/**
	*	@brief	y�����S�̉�]�s����v�Z�����݂̍s��ɂ�����
	*	@param[in]	deg		��]��(degree,�x���@)
	*/
	void AddRotaY(float deg);

	/**
	*	@brief	z�����S�̉�]�s����v�Z�����݂̍s��ɂ�����
	*	@param[in]	deg		��]��(degree,�x���@)
	*/
	void AddRotaZ(float deg);

	/**
	*	@brief	���݂̍s��ɉ�]��\���l������������s����v�Z�����݂̍s��ɂ�����
	*	@param[in]	quaternion	��]��\���l����
	*/
	void AddRotaQuaternion(const DirectX::XMFLOAT4& quaternion);

	/**
	*	@brief	���݂̍s����擾����
	*	@return	���݂̍s��
	*/
	DirectX::XMFLOAT4X4 GetAMatrix();

	/**
	*	���݂̍s���P�ʍs��ɂ���
	*/
	void Init();
private:
	/**
	*	@brief	XMFLOAT4X4��XMMATRIX���E���炩����
	*	@param[in]	matF44	��Z����XMFLOAT4X4
	*	@param[in]	matXM	��Z����XMMATRIX
	*	@return	�v�Z����(XMFLOAT4X4)
	*/
	DirectX::XMFLOAT4X4 Multiplication(DirectX::XMFLOAT4X4& matF44,const DirectX::XMMATRIX& matXM);

	/**
	*	�v�Z���Ă���A�t�B���ϊ��s��
	*/
	DirectX::XMFLOAT4X4 mAMatrix;
};

