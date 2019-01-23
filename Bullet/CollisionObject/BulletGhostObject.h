#pragma once
/**
*	@file BulletGhostObject.h
*	@brief �Փˌ��m�p�̃I�u�W�F�N�g
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/12/5
*/

#include "Base/ICollisionObject.h"
#include <memory>
#include <DirectXMath.h>

class btGhostObject;
class BulletCollisionShape;
class PhysicsSystem;

/**
*	@class BulletGhostObject
*	@brief �Փˌ��m�I�u�W�F�N�g
*/
class BulletGhostObject :public ICollisionObject
{
public:
	/**
	*	
	*/
	BulletGhostObject(int worldID);

	/**
	*	@param[in]	shape	�Փˌ`��
	*	@param[in]	worldID	�ŗL��ID
	*/
	BulletGhostObject(std::shared_ptr<BulletCollisionShape> collisionShape, int worldID);
	virtual ~BulletGhostObject();

	/**
	*	@brief	�Փˌ`��̐ݒ�
	*	@param[in]	collisionShape	�Փˌ`��
	*/
	void SetCollisionShape(std::shared_ptr<BulletCollisionShape> collisionShape);

	/**
	*	@brief	�^�O1��ݒ肷��
	*	@param[in]	tag	�ݒ肷��^�O1
	*/
	void SetTag1(int tag);

	/**
	*	@brief	�^�O2��ݒ肷��
	*	@param[in]	tag	�ݒ肷��^�O2
	*/
	void SetTag2(int tag);

	/**
	*	@brief	btCollisionObject�̃|�C���^���擾����
	*	@return btCollisionObject�̃|�C���^
	*/
	std::shared_ptr<btCollisionObject> GetPtr() const;

	/**
	*	@brief	�d�����Ă���I�u�W�F�N�g�̐����擾����(AABB�P�ʂ��O���[�v�P��?)
	*	@return �d���I�u�W�F�N�g�̐�
	*/
	int GetNumOvwelappingObjects();

	/**
	*	@brief	�d�����Ă���I�u�W�F�N�g���擾����
	*	@param[in]	�I�u�W�F�N�g�̃C���f�b�N�X
	*	@return		�d���I�u�W�F�N�g
	*/
	btCollisionObject* GetOverlappingObject(int i);

	/**
	*	@brief	�R���W�����̏�Ԃ�ݒ肷��
				BulletCollisionState�����Ƃɐݒ肷��
	*/
	void SetCollisionState(int flags);

	/**
	*	@brief	�ʒu���w��̏ꏊ�ֈړ�������
	*	@param[in]	x		�ړ�������x���W
	*	@param[in]	y		�ړ�������y���W
	*	@param[in]	z		�ړ�������z���W
	*/
	void SetOrigin(float x, float y, float z);

	/**
	*	@brief	�ʒu���w��̏ꏊ�ֈړ�������
	*	@param[in]	origin	�ړ���������W
	*/
	void SetOrigin(DirectX::XMFLOAT3 origin);
private:
	/**
	*	���E����o�^���O��
	*/
	void RemoveWorld() override;

	/**
	*	�o���b�g��ghost
	*/
	std::shared_ptr<btGhostObject> mGhost;

	/**
	*	�Փˌ`��
	*/
	std::shared_ptr<BulletCollisionShape> mShape;
};

