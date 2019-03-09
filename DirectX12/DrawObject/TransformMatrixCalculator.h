#pragma once
/**
*	@addtogroup Calculator
*	@file TransformMatrixCalculator.h
*	@brief ���s�ړ��A�g��E�k���A��]�̌v�Z������N���X�̊��N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/9/20
*/
#include <DirectXMath.h>

/**
*	@ingroup Calculator
*	@class TransformMatrixCalculator��
*	@brief ���s�ړ��A�g��E�k���A��]�̌v�Z��������N���X
*/
class TransformMatrixCalculator
{
public:
	TransformMatrixCalculator();
	virtual ~TransformMatrixCalculator();

	/**
	*	@brief	�C�ӂ̗ʕ��s�ړ�������
	*	@param[in]	offset	�ւ����ړ�����
	*/
	virtual void AddPositon(const DirectX::XMFLOAT3& offset) = 0;

	/**
	*	@brief	�C�ӂ̊g�嗦��ǉ�����
	*	@param[in]	scale	�ǉ��g�嗦
	*/
	virtual void AddScale(const DirectX::XMFLOAT3& scale) = 0;

	/**
	*	@brief	x�����S�Œǉ��ŉ�]������
	*	@param[in]	rad		��]��(radian,�ʓx�@)
	*/
	virtual void AddRotaX(float rad) = 0;

	/**
	*	@brief	y�����S�Œǉ��ŉ�]������
	*	@param[in]	rad		��]��(radian,�ʓx�@)
	*/
	virtual void AddRotaY(float rad) = 0;

	/**
	*	@brief	z�����S�Œǉ��ŉ�]������
	*	@param[in]	rad		��]��(radian,�ʓx�@)
	*/
	virtual void AddRotaZ(float rad) = 0;

	/**
	*	@brief	�l�����Ŏw�肳�ꂽ�ʒǉ��ŉ�]������
	*	@param[in]	quaternion	�ǉ��ŉ�]�������]��\���Ă���l����
	*/
	virtual void AddRotaQuaternion(const DirectX::XMFLOAT4& quaternion) = 0;

	/**
	*	���܂Ōv�Z�����s��𓾂�
	*/
	virtual DirectX::XMFLOAT4X4 GetAMatrix() = 0;
};

