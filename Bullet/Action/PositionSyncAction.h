#pragma once
#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <memory>
#include <vector>

class BulletRigidBody;
class BulletGhostObject;

class PositionSyncAction : public btActionInterface
{
public:
	PositionSyncAction();
	~PositionSyncAction();

	/**
	*	@brief	bullet������Ă΂��֐��@���̒��ŏՓ˂����m����
	*	@param[in]	collidionWorld	���̂�ghost�Ȃǂ�ێ����Ă��鐢�E
	*	@param[in]	deltaTime		�X�e�b�v����
	*/
	void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTimeStep);

	/**
	*	@brief	bullet������Ă΂��֐� �K�v�Ȃ��̂Ŗ�����
	*	@param[in]	debugDrawer		�f�o�b�O�`��p�̃N���X
	*/
	void debugDraw(btIDebugDraw* debugDrawer);

	/**
	*	@brief	���W�𓯊�����ghost��ǉ�����
	*	@param[in]	ghost	��������ghost
	*/
	void AddSyncGhosts(std::shared_ptr<BulletGhostObject> ghost);

	/**
	*	@brief	��������ghost���폜����
	*	@param[in]	ghostIndex	�폜����ghost�̃C���f�b�N�X
	*/
	void RemoveSyncGhost(unsigned int ghostIndex);

private:
	std::shared_ptr<BulletRigidBody> mRigidBody;
	std::vector<std::shared_ptr<BulletGhostObject>> mSyncGhosts;
};

