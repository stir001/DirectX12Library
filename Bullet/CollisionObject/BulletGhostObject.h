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
	*	@param[in]	worldID	�ŗL��ID
	*/
	BulletGhostObject(int worldID);

	/**
	*	@param[in]	shape	�Փˌ`��
	*	@param[in]	worldID	�ŗL��ID
	*/
	BulletGhostObject(std::shared_ptr<BulletCollisionShape> collisionShape, int worldID);
	virtual ~BulletGhostObject();

	/**
	*	@brief	�o���b�g��GhostObject���擾����
	*	@return	btGhostObject
	*/
	std::shared_ptr<btGhostObject> GetGhostObject();

	/**
	*	@brief	�Փˌ`��̐ݒ�
	*	@param[in]	collisionShape	�Փˌ`��
	*/
	void SetCollisionShape(std::shared_ptr<BulletCollisionShape> collisionShape);

	/**
	*	@brief	�^�O��ݒ肷��
	*	@param[in]	tag	�ݒ肷��^�O
	*/
	void SetTag(int tag);
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

