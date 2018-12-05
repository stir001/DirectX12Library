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

class CollisionDetector;
class BulletCollisionShape;

/**
*	@class IActionDefiner
*	@brief	�Փˎ��̃A�N�V�������`����K�肷��N���X
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
private:
	/**
	*	�Փˌ��m�p�I�u�W�F�N�g
	*/
	std::shared_ptr<CollisionDetector> mDetector;
};

