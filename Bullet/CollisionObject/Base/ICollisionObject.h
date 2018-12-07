#pragma once
/**
*	@file ICollisionObject.h
*	@brief �Փ˃I�u�W�F�N�g�̒�`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/12/5
*/
#include <memory>

class PhysicsSystem;
class btCollisionObject;

/**
*	@class ICollisionObject
*	@brief �Փ˃I�u�W�F�N�g�̒�`
*/
class ICollisionObject
{
	friend PhysicsSystem;
public:

	ICollisionObject(int worldID);
	virtual ~ICollisionObject();

	/**
	*	@brief	�^�O��ݒ肷��
	*	@param[in]	tag		���[�U�[��`�̃^�O
	*/
	virtual void SetTag(int tag) = 0;

	/**
	*	@brief	btCollisionObject�̃|�C���^���擾����
	*	@return btCollisionObject�̃|�C���^
	*/
	virtual std::shared_ptr<btCollisionObject> GetPtr() const = 0;

	/**
	*	@brief	���[�U�[��`�̃^�O���擾����
	*	@return	�^�O
	*/
	int GetTag() const;

	/**
	*	@brief	���[���h�ʎ��ʎq���擾����
	*	@return	���ʎq
	*/
	int GetWorldID() const;

protected:
	/**
	*	���g�𐢊E����폜����
	*/
	virtual void RemoveWorld() = 0;

	/**
	*	���[�U�[��`�̃^�O �f�t�H���g��-1
	*/
	int mTag;
private:
	/**
	*	�ʎ��ʎq
	*/
	int mWorldID;
};

