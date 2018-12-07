#pragma once
/**
*	@file IActionDefiner.h
*	@brief �Փˎ��̓�����`���邽�߂̊��N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/12/04
*/
#include <memory>
#include <DirectXMath.h>

class CollisionDetector;
class BulletCollisionShape;

/**
*	@class IActionDefiner
*	@brief	�Փˎ��̃A�N�V�������`�����ꂷ��N���X
*/
class IActionDefiner
{
public:
	/**
	*	@param[in]	shape	�Փˌ`��
	*	@param[in]	tag		���g�̃^�O(���[�U�[�����R�ɒ�`)
	*/
	IActionDefiner(std::shared_ptr<BulletCollisionShape> shape, int tag);
	~IActionDefiner();

	/**
	*	@brief	�Փ˂��Ă���Ԃ����ƌĂяo�����֐�
	*	@param[in]	tag		�e�R���W�����ɐݒ肷��^�O
	*/
	virtual void Action(int tag) = 0;

	/**
	*	@brief	���g�̃^�O���擾����
	*	@return	���g�̃^�O
	*/
	int GetTag() const;

	/**
	*	@brief	�����蔻����u�Ԉړ�������
	*	@param[in]	x	�ړ�������x���W
	*	@param[in]	y	�ړ�������y���W
	*	@param[in]	z	�ړ�������z���W
	*/
	void Translate(float x, float y, float z);

	/**
	*	@brief	�����蔻����u�Ԉړ�������
	*	@param[in]	pos	�ړ���������W
	*/
	void Translate(const DirectX::XMFLOAT3& pos);
private:
	/**
	*	�Փˌ��m�p�I�u�W�F�N�g
	*/
	std::shared_ptr<CollisionDetector> mDetector;
};

