#pragma once
/**
*	@file CollisionDetector.h
*	@brief �Փ˂����m���邽�߂̃N���X
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/12/04
*/
#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <btBulletDynamicsCommon.h>
#include <memory>
#include <functional>

enum class BulletShapeType;
enum class BulletCollisionState;
class BulletGhostObject;
class BulletCollisionShape;
class btGhostObject;
class IActionDefiner;

/**
*	@class CollisionDetector
*	@brief	�����ghost�Ƃ̏Փ˂����m����
*/
class CollisionDetector : public btActionInterface
{
public:
	/**
	*	@param[in]	shape	�Փˌ��m����ghost�̌^
	*	@param[in]	tag		ghost�ɐݒ肷�郆�[�U�[��`�̃^�O
	*/
	CollisionDetector(std::shared_ptr<BulletCollisionShape> shape, int tag);
	virtual ~CollisionDetector();

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
	*	@brief	�Փ˂���ghost��ݒ肷��
	*	@param[in]	ghost	�Փˌ��m�Ɏg��ghost(���E�ւ̓o�^�͂ق��ł��K�v����)
	*/
	void SetCollisionObject(std::shared_ptr<BulletGhostObject> ghost);

	/**
	*	@brief	�Փˌ`���ݒ肷��
	*	@param[in]	�Փ˂���`��
	*/
	void SetShape(std::shared_ptr<BulletCollisionShape> shape);

	/**
	*	@brief	��Ԃ�ݒ肷��
	*	@param[in]	state	�ݒ肷����(bit�P�ʂł̃t���O�ŊǗ�)
	*/
	void SetCollisionState(int state);

	/**
	*	@brief	��Ԃ�ݒ肷��
	*	@param[in]	state	�ݒ肷����(bit�P�ʂł̃t���O�ŊǗ�)
	*/
	void SetCollisionState(BulletCollisionState state);

	/**
	*	@brief	���S���W��C�ӂ̓_�ɐݒ肷��
	*	@param[in]�@x	�ݒ肷��x���W
	*	@param[in]	y	�ݒ肷��y���W
	*	@param[in]	z	�ݒ肷��z���W
	*/
	void Translate(float x, float y, float z);

	/**
	*	@brief	ghost���擾����
	*	@return	�ێ����Ă���ghost
	*/
	std::shared_ptr<BulletGhostObject> GetGhostObject();

	/**
	*	@brief	bullet��ghostObject(btGhostObject)�̃|�C���^���擾����
	*	@return	btGhostObject�̃|�C���^
	*/
	btGhostObject* GetbtGhostPtr();

	/**
	*	@brief	�Փˌ��m���Ă���Ԃ����ƌĂ΂��֐���ݒ肷��
	*	@param[in]	action	�Փˌ��m���Ɏ��s�����A�N�V����
	*/
	void SetAction(std::function<void(int)> action);

	/**
	*	@brief �^�O���擾����
	*	@return �ێ����Ă���^�O
	*/
	int GetTag() const;

	/**
	*	@brief	�^�O��ݒ肷��
	*	@param[in]	�ݒ肷��^�O
	*/
	void SetTag(int tag);

protected:
	/**
	*	�Փˌ��m�p�̃I�u�W�F�N�g
	*/
	std::shared_ptr<BulletGhostObject> mGhost;

	/**
	*	�Փˎ��ɋN�����A�N�V�����̊֐�
	*/
	std::function<void(int)> mAction;
};

