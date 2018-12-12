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
	*	@param[in]	tag1	���g�̃^�O1(���[�U�[�����R�ɒ�`)
	*/
	IActionDefiner(std::shared_ptr<BulletCollisionShape> shape, int tag1);
	~IActionDefiner();

	/**
	*	@brief	�Փ˂����ŏ��ɌĂяo�����֐�
	*	@brief	tag		�Փ˂����R���W�����̃^�O
	*/
	virtual void OnAction(int tag1, int tag2) = 0;

	/**
	*	@brief	�Փ˂��Ă����OnAction�̌ジ���ƌĂяo�����֐�
	*	@param[in]	tag		�Փ˂����R���W�����̃^�O
	*/
	virtual void StayAction(int tag1, int tag2) = 0;

	/**
	*	@brief	�Փ˂��Ȃ��Ȃ������ɔ�яo�����֐�
	*	@param[in]	tag		�Փ˂����R���W�����̃^�O
	*/
	virtual void ExitAction(int tag1, int tag2) = 0;

	/**
	*	@brief	�^�O1��ݒ肷��
	*	@param[in]	tag	�ݒ肷��^�O1
	*/
	void SetTag1(int tag);

	/**
	*	@brief	���g�̃^�O1���擾����
	*	@return	�^�O1
	*/
	int GetTag1() const;

	/**
	*	@brief	�^�O2��ݒ肷��
	*	@param[in]	tag	�ݒ肷��^�O2
	*/
	void SetTag2(int tag);

	/**
	*	@brief	���g�̃^�O2���擾����
	*	@return	�^�O2
	*/
	int GetTag2() const;

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

