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

enum class BulletCollisionState
{
	STATIC = 1,			//!�C�ӈړ��s�I�u�W�F�N�g �����v�Z����  �����蔻�肠�� ��A�N�e�B�u������
	KINEMATIC = 2,		//!�C�ӈړ��\�I�u�W�F�N�g �����v�Z�Ȃ�  �����蔻�肠�� ��A�N�e�B�u���Ȃ�
	NON_CONTACT = 4,	//!�C�ӈړ��s�I�u�W�F�N�g �d�͂̂�?���� �����蔻��Ȃ� ��A�N�e�B�u������
	CHARACTER = 16,		//!�C�ӈړ��s�I�u�W�F�N�g �����v�Z����  �����蔻�肠�� ��A�N�e�B�u���Ȃ�
};

int operator|(const BulletCollisionState lval, const BulletCollisionState rval);
int operator|(const BulletCollisionState lval, const int rval);
int operator|(const int lval, const BulletCollisionState rval);
int operator&(const BulletCollisionState lval, const BulletCollisionState rval);
int operator&(const BulletCollisionState lval, const int rval);
int operator&(const int lval, const BulletCollisionState rval);

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
	*	@brief	btCollisionObject�̃|�C���^���擾����
	*	@return btCollisionObject�̃|�C���^
	*/
	virtual std::shared_ptr<btCollisionObject> GetPtr() const = 0;

	/**
	*	@brief	�^�O��ݒ肷��
	*	@param[in]	tag		���[�U�[��`�̃^�O
	*/
	virtual void SetTag1(int tag) = 0;

	/**
	*	@brief	���[�U�[��`�̃^�O1���擾����
	*	@return	�^�O1
	*/
	int GetTag1() const;

	virtual void SetTag2(int tag) = 0;

	/**
	*	@brief	���[�U�[��`�̃^�O2���擾��
	*	@return	�^�O2
	*/
	int GetTag2() const;

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
	*	���[�U�[��`�̃^�O1 �f�t�H���g��-1
	*/
	int mTag1;

	/**
	*	���[�U�[��`�̃^�O2	�f�t�H���g��-1
	*/
	int mTag2;
private:
	/**
	*	�ʎ��ʎq
	*/
	int mWorldID;
};

