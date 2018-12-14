#pragma once
/**
*	@file BulletRigidBody.h
*	@brief btRigidBody�̃��b�p�[�N���X��`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/12/14
*/
#include "Base/ICollisionObject.h"
#include <memory>
#include <DirectXMath.h>

class btRigidBody;
class btMotionState;
class BulletCollisionObject;
class PhysicsSystem;
class BulletCollisionShape;
class IActionDefiner;

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
	*	@brief	���ʂ�ݒ肷��
	*	@param[in]	mass	�ݒ肷�鎿��
	*/
	virtual void SetMass(float mass);

	/**
	*	@brief ���݂̈ʒu����w�蕪�������S�_���ړ�������(add�̌v�Z)	
				�u�Ԉړ��Ȃ̂œ����蔻�莞�ɕs����N����\������
	*	@param[in]	x	x�ړ�����
	*	@param[in]	y	y�ړ�����
	*	@param[in]	z	z�ړ�����
	*/
	virtual void Translate(float x, float y, float z);

	/**
	*	@brief ���݂̈ʒu����w�蕪�������S�_���ړ�������(add�̌v�Z)	
				�u�Ԉړ��Ȃ̂œ����蔻�莞�ɕs����N����\������
	*	@param[in]	pos	�ړ��������
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
	*	@brief	�C�ӂ̃^�O1��ݒ肷��
	*	@param[in]	tag		�ݒ肷��^�O1
	*/
	void SetTag1(int tag);

	/**
	*	@brief	�C�ӂ̃^�O2��ݒ肷��
	*	@param[in]	tag		�ݒ肷��^�O2
	*/
	void SetTag2(int tag);

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

	/**
	*	@brief	btCollisionObject�̃|�C���^���擾����
	*	@return btCollisionObject�̃|�C���^
	*/
	std::shared_ptr<btCollisionObject> GetPtr() const;

	/**
	*	@brief	btRigidBody�̃|�C���^���擾����
	*	@return btRigidBody�̃|�C���^
	*/
	std::shared_ptr<btRigidBody> GetRigidPtr() const;

	/**
	*	@brief	�����蔻����Ƃ�Ȃ��A�N�V�������`����
	*	@param[in]	ignoreAction	��������A�N�V����
	*/
	void SetIgnoreAction(std::shared_ptr<IActionDefiner> ignoreAction);

	/**
	*	@brief	�������Z���ɉ�]����͂̋������`����
	*	@param[in]	factor	��]����v�f�̑傫��(0�ŉ�]���Ȃ�)
	*/
	void SetAcnglerFactor(float factor);

	/**
	*	@brief	���݂̏Փˌ`��̒��S�_���W�𓾂�
	*	@return	���݂̏Փˌ`��̒��S���W
	*/
	DirectX::XMFLOAT3 GetOrigin() const;

	/**
	*	@brief	���̂ɗ͂�������
	*	@param[in]	x	x�������̗͂̐���
	*	@param[in]	y	y�������̗͂̐���
	*	@param[in]	z	z�������̗͂̐���
	*/
	void AddForce(float x, float y, float z);

	/**
	*	@broef	���̂ɗ͂�������
	*	@param[in]	force	������͂̐���
	*/
	void AddForce(const DirectX::XMFLOAT3& force);

	/**
	*	@brief	���̂ɏu�ԓI�ɗ͂�������
	*	@param[in]	x	x�������̗͂̐���
	*	@param[in]	y	y�������̗͂̐���
	*	@param[in]	z	z�������̗͂̐���
	*/
	void AddImpulse(float x, float y, float z);

	/**
	*	@brief	���̂ɏu�ԓI�ɗ͂�������
	*	@param[in]	impulse	������͂̐���
	*/
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	/**
	*	@brief	���x��ݒ肷��
	*	@param[in]	x	x�������̑��x����
	*	@param[in]	y	y�������̑��x����
	*	@param[in]	z	z�������̑��x����
	*/
	void SetVelocity(float x, float y, float z);

	/**
	*	@brief	���x��ݒ肷��
	*	@param[in]	vel	�ݒ肷�鑬�x����
	*/
	void SetVelocity(const DirectX::XMFLOAT3& vel);

	/**
	*	@brief	���x���擾����
	*	@return	���݂̑��x
	*/
	DirectX::XMFLOAT3 GetVelocity() const;
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
	*	�������Z��L���ȏ�Ԃɂ���
	*/
	void Activation();

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

