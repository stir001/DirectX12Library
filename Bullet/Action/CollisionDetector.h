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
#include <map>
#include <DirectXMath.h>

enum class BulletShapeType;
enum class BulletCollisionState;
class BulletGhostObject;
class BulletCollisionShape;
class btGhostObject;
class IActionDefiner;
class CollisionActionCaller;
struct CalliedAction;

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
	CollisionDetector(std::shared_ptr<BulletCollisionShape> shape
		, int tag, std::shared_ptr<CalliedAction> calliedAction);
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
	void SetOrigin(float x, float y, float z);

	/**
	*	@brief	ghost���擾����
	*	@return	�ێ����Ă���ghost
	*/
	std::shared_ptr<BulletGhostObject> GetPtr();

	/**
	*	@brief	�^�O1��ݒ肷��
	*	@param[in]	�ݒ肷��^�O1
	*/
	void SetTag1(int tag);

	/**
	*	@brief �^�O1���擾����
	*	@return �ێ����Ă���^�O1
	*/
	int GetTag1() const;

	/**
	*	@brief	�^�O2��ݒ肷��
	*	@param[in]	�ݒ肷��^�O2
	*/
	void SetTag2(int tag);

	/**
	*	@brief	�^�O2���擾����
	*	@return	�ێ����Ă���^�O2
	*/
	int GetTag2() const;

	/**
	*	@brief	���x��ݒ肷��
	*	@param[in]	vel	�ݒ肷�鑬�x
	*/
	void SetVelocity(const DirectX::XMFLOAT3& vel);
private:
	/**
	*	@brief	�y�A���������Ă��邩�ǂ����𔻒肷��
	*	@param[in]	pair	�y�A
	*/
	bool IsCollide(btBroadphasePair* pair);

	int GetOtherProxyID(btBroadphasePair* pair, btBroadphaseProxy* mine);

	/**
	*	�Փˌ��m�p�̃I�u�W�F�N�g
	*/
	std::shared_ptr<BulletGhostObject> mGhost;

	std::map<int, std::shared_ptr<CollisionActionCaller>> mCallers;

	/**
	*	�A�N�V�������ĂԃN���X�̐ݒ�ۑ��p
	*/
	std::shared_ptr<CalliedAction> mCalliedAction;
};

