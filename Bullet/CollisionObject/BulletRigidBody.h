#pragma once
/**
*	@file BulletRigidBody.h
*	@brief btRigidBody�̃��b�p�[�N���X��`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/11/26
*/
#include "Base/ICollisionObject.h"
#include <memory>
#include <DirectXMath.h>

class btRigidBody;
class btMotionState;
class BulletCollisionObject;
class PhysicsSystem;
class BulletCollisionShape;

enum class BulletCollisionState 
{
	STATIC = 1,			//!�C�ӈړ��s�I�u�W�F�N�g �����v�Z����  �����蔻�肠�� ��A�N�e�B�u������
	KINEMATIC = 2,		//!�C�ӈړ��\�I�u�W�F�N�g �����v�Z�Ȃ�  �����蔻�肠�� ��A�N�e�B�u���Ȃ�
	NON_CONTACT = 4,	//!�C�ӈړ��s�I�u�W�F�N�g �d�͂̂�?���� �����蔻��Ȃ� ��A�N�e�B�u������
	CHARACTER = 16,		//!�C�ӈړ��s�I�u�W�F�N�g �����v�Z����  �����蔻�肠�� ��A�N�e�B�u���Ȃ�
};

int operator|(const BulletCollisionState lval, const BulletCollisionState rval);

/**
*	@class BulletRigidBody
*	@brief btRigidBody�̃��b�p�[�N���X
*/
class BulletRigidBody : public ICollisionObject
{
public:
	/**
	*	@param[in]	collisionShape	rigidBody�ɐݒ肷��`��
	*	@param[in]	pos		�����ʒu
	*/
	BulletRigidBody(std::shared_ptr<BulletCollisionShape> collisionShape, int worldID
		, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));
	virtual ~BulletRigidBody();

	/**
	*	btRigidBody���擾����
	*/
	virtual std::shared_ptr<btRigidBody> GetRigidBody();

	/**
	*	@brief	���ʂ�ݒ肷��
	*	@param[in]	mass	�ݒ肷�鎿��
	*/
	virtual void SetMass(float mass);

	/**
	*	@brief �ʒu��ݒ肷��	
				�u�Ԉړ��Ȃ̂œ����蔻�莞�ɕs����N����\������
	*	@param[in]	x	x���W
	*	@param[in]	y	y���W
	*	@param[in]	z	z���W
	*/
	virtual void Translate(float x, float y, float z);

	/**
	*	@brief �ʒu��ݒ肷��
				�u�Ԉړ��Ȃ̂œ����蔻�莞�ɕs����N����\������
	*	@param[in]	pos	�ݒ肷����W
	*/
	virtual void Translate(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	���[���h�ϊ��s����擾����
	*	@return	���[���h�ϊ��s��
	*/
	DirectX::XMFLOAT4X4 GetWorldTransform() const;

	/**
	*	@brief	���[���h�ϊ��s���ݒ肷��
	*	@param[in]	matrix	���[���h�ϊ��s��
	*/
	void SetWorldTransform(const DirectX::XMFLOAT4X4& matrix);

	/**
	*	@brief	�C�ӂ̃^�O��ݒ肷��
				�R���W�����̔��莞�ɃI�u�W�F�N�g�𔻕ʂ��邽�߂̃^�O
	*	@param[in]	tag		�ݒ肷��^�O
	*/
	void SetTag(int tag);

	/**
	*	@brief	�R���W�����̏�Ԃ�ύX����
	*		enum BulletCollisionState	�̒l����ɂ���
	*	@param[in]	state	�ݒ肷��X�e�[�g
	*/
	void SetCollisionState(int state);

	/**
	*	@brief	�R���W�����̏�Ԃ�ύX����
	*		enum BulletCollisionState	�̒l����ɂ���
	*	@param[in]	state	�ݒ肷��X�e�[�g
	*/
	void SetCollisionState(BulletCollisionState state);

	/**
	*	@brief	���C�͂�ݒ肷��
	*	@param[in]	friction	���C��
	*/
	void SetFriction(float friction);

	/**
	*	@brief	��]���C�͂�ݒ肷��
	*	@param[in]	friction	���C��
	*/
	void SetSpinFriction(float friction);

	/**
	*	@brief	�����W����ݒ肷��
	*	@param[in]	restitution	�����W��
	*/
	void SetRestitution(float restitution);

	/**
	*	@brief	Yaw Pitch Roll��ݒ肷��
	*	@param[in]	ypr		(Pitch,Yaw,Roll)
	*/
	void SetYawPitchRoll(const DirectX::XMFLOAT3& ypr);

	/**
	*	@brief	Yaw Pitch Roll��ݒ肷��
	*	@param[in]	x	Pitch
	*	@param[in]	y	Yaw
	*	@param[in]	z	Roll
	*/
	void SetYawPitchRoll(float x, float y,float z);

	/**
	*	@brief	���S�_��ݒ肷��
	*	@param[in]	pos		�ݒ肷����W
	*/
	void SetOrigin(const DirectX::XMFLOAT3& pos);

	/**
	*	@brief	���S�_��ݒ肷��
	*	@param[in]	x	�ݒ肷��x���W
	*	@param[in]	y	�ݒ肷��y���W
	*	@param[in]	z	�ݒ肷��z���W
	*/
	void SetOrigin(float x, float y, float z);
private:
	/**
	*	���E���玩�����폜����
	*/
	void RemoveWorld() override;

	/**
	*	btRigidBody���쐬����
	*/
	virtual void CreateRigidBody();

	/**
	*	rigidBody
	*/
	std::shared_ptr<btRigidBody> mRigidBody;

	/**
	*	collision�̌`��
	*/
	std::shared_ptr<BulletCollisionShape> mCollisionShape;

	/**
	*	���[�V�����X�e�[�g	���̂Ƃ̓����ɕK�v
	*/
	std::shared_ptr<btMotionState> mMotionState;

	/**
	*	����
	*/
	float mMass;
};

