#pragma once
/**
*	@file	PhysicsSystem.h
*	@brief	bullet��world���ӂ̋@�\�����N���X��`
*
*	@author �^�珧��Y
*
*	@par �ŏI�X�V��	2018/12/14
*/
#include <memory>
#include <DirectXMath.h>
#include <map>

class BulletDebugDrawDx;
class BulletRigidBody;
class BulletCollisionShape;
class CollisionDetector;
class BulletGhostObject;
class btGhostObject;
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btGhostPairCallback;
class btCollisionObject;
class Dx12CommandList;

/**
*	@enum	BulletShapeType
*	@brief	�`��̃^�C�v��`
*/
enum class BulletShapeType
{
	BOX,
	SPHERE,
	CYLINDER,
	CAPSULE,
	PLANE,
	CONE
};

/**
*	@class PhysicsSystem
*	@brief rigidBody��collision�֌W���Ǘ�����
*/
class PhysicsSystem : public std::enable_shared_from_this<PhysicsSystem>
{
public:
	~PhysicsSystem();

	/**
	*	�C���X�^���X�̎Q�Ƃ��擾����
	*/
	static PhysicsSystem& Instance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new PhysicsSystem();
		}
		return *mInstance;
	}

	/**
	*	�C���X�^���X��j������
	*/
	static void Destory()
	{
		if (mInstance != nullptr)
		{
			delete mInstance;
			mInstance = nullptr;
		}
	}

	/**
	*	�f�o�b�O�`��
	*/
	void DebugDraw();

	/**
	*	@brief	���̂�system�̊Ǘ����ɒǉ�����
	*	@param[in]	collider	�ǉ����鍄��
	*/
	void AddRigidBody(std::shared_ptr<BulletRigidBody> rigid);

	/**
	*	���̃V���~���[�V����������
	*/
	void Simulation();

	/**
	*	@brief ���̃V���~���[�V����������
	*	@param[in]	de;taTime	�i�߂鎞��
	*/
	void Simulation(float deltaTime);

	/**
	*	@brief	���̂��Ǘ�������������
	*	@param[in]	collider	������鍄��
	*/
	void RemoveRigidBody(std::shared_ptr<BulletRigidBody> rigid);

	/**
	*	@brief ���̂��Ǘ�������������
	*	@param[in]	tag		������鍄�̂̃^�O
	*/
	void RemoveRigidBody(int worldID);

	/**
	*	@brief	rigidBody���쐬����
	*	@param[in]	type	�쐬����`��
	*	@param[in]	data	�`��ɂ��قȂ� (x,y,z)
			BOX			(x, y, z)				�e�����̕ӂ̒���
			SPHERE		(radius, nan, nan)		x���a y����	z����
			CYLINDER	(radius, height, nan)	x���a y���� z����
			CAPSULE		(radius, height, nan)	x���a y���� z����
			PLANE		(x, y, z)				x�@�� y�@�� z�@�� 
			CONE		(radius, height, nan)	x���a y���� z����
	*	@param[in]	pos		�����ʒu
	*	@return		��������RigidBody
	*/
	std::shared_ptr<BulletRigidBody> CreateRigidBody(const BulletShapeType type
		, const DirectX::XMFLOAT3& data , const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));

	/**
	*	@brief	rigidBody���쐬����
	*	@param[in]	shape	�K������`��
	*	@param[in]	pos		�����ʒu
	*/
	std::shared_ptr<BulletRigidBody> CreateRigidBody(const std::shared_ptr<BulletCollisionShape> shape
		, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));

	/**
	*	@brief	�R���W�����`����쐬����
	*	@param[in]	type	�쐬����`��
	*	@param[in]	data	�`��ɂ��قȂ� (x,y,z)
			BOX			(x, y, z)				�e�����̕ӂ̒���
			SPHERE		(radius, nan, nan)		x���a y����	z����
			CYLINDER	(radius, height, nan)	x���a y���� z����
			CAPSULE		(radius, height, nan)	x���a y���� z����
			PLANE		(x, y, z)				x�@�� y�@�� z�@�� 
			CONE		(radius, height, nan)	x���a y���� z����
	*	@return		���������R���W�����`��
	*/
	std::shared_ptr<BulletCollisionShape> CreateCollisionShape(const BulletShapeType type
		, const DirectX::XMFLOAT3& data);

	/**
	*	@brief	ghost���쐬����
	*	@param[in]	type	�쐬����`��
	*	@param[in]	data	�`��ɂ��قȂ� (x,y,z)
			BOX			(x, y, z)				�e�����̕ӂ̒���
			SPHERE		(radius, nan, nan)		x���a y����	z����
			CYLINDER	(radius, height, nan)	x���a y���� z����
			CAPSULE		(radius, height, nan)	x���a y���� z����
			PLANE		(x, y, z)				x�@�� y�@�� z�@��
			CONE		(radius, height, nan)	x���a y���� z����
	*	@param[in]	pos		�����ʒu
	*	@return		��������ghost
	*/
	std::shared_ptr<BulletGhostObject> CreateGhostObject(const BulletShapeType type
		, const DirectX::XMFLOAT3& data, const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));

	/**
	*	@brief	ghost�𐶐�����
	*	@param[in]	shape	��������ghost�̌`��
	*	@return		��������ghost
	*/
	std::shared_ptr<BulletGhostObject> CreateGhostObject(std::shared_ptr<BulletCollisionShape> shape
	,  const DirectX::XMFLOAT3& pos = DirectX::XMFLOAT3(0.f, 0.f, 0.f));

	/**
	*	@brief	�A�N�V������ǉ�����
	*	@param[in]	action	�ǉ�����A�N�V����
	*/
	void AddAction(std::shared_ptr<CollisionDetector> action);

	/**
	*	@brief	�A�N�V�������폜����
	*	@param[in]	action	�폜����A�N�V����
	*/
	void RemoveIgnoreAction(std::shared_ptr<CollisionDetector> action);

	/**
	*	@brief	�S�[�X�g��ǉ�����
		@param[in]	ghost	�ǉ�����S�[�X�g
	*/
	void AddGhost(std::shared_ptr<BulletGhostObject> ghost);

	/**
	*	@brief	Ghost�𐢊E����폜����
	*	@parma[in]	worldID	�폜����ghost��ID
	*/
	void RemoveGhost(int worldID);

	/**
	*	@brief	Ghost�𐢊E����폜����
	*	@param[in]	ghost	�폜����ghost
	*/
	void RemoveGhost(std::shared_ptr<BulletGhostObject> ghost);

	/**
	*	@brief	�Փ˂��镨�̖̂��g�p�̍ŏ�����ID���擾����
	*/
	int GetAvailableMinID();

	void SetRenderCommnadList(std::shared_ptr<Dx12CommandList>& cmdList);
private:
	/**
	*	�V���O���g���p�C���X�^���X
	*/
	static PhysicsSystem* mInstance;

	PhysicsSystem();
	PhysicsSystem(const PhysicsSystem&) = delete;
	PhysicsSystem(const PhysicsSystem&&) = delete;

	/**
	*	�Ǘ����Ă���I�u�W�F�N�g���������
	*/
	void Release();

	/**
	*	�L��ʑ��t�F�[�Y
	*/
	std::shared_ptr<btBroadphaseInterface> mBroadphase;

	/**
	*	����ʑ��t�F�[�Y�ݒ�
	*/
	std::shared_ptr<btDefaultCollisionConfiguration> mCollisionConfiguration;

	/**
	*	����ʑ��t�F�[�Y
	*/
	std::shared_ptr<btCollisionDispatcher> mDispatcher;

	/**
	*	�S������
	*/
	std::shared_ptr<btSequentialImpulseConstraintSolver> mSolver;

	/**
	*	���E
	*/
	std::shared_ptr<btDiscreteDynamicsWorld> mWorld;

	/**
	*	�f�o�b�O�`��V�X�e��
	*/
	std::shared_ptr<BulletDebugDrawDx> mDebugDrawer;

	/**
	*	�Փˑ̕ێ��p
	*/
	std::map<int, std::shared_ptr<btCollisionObject>> mCollisions;

	/**
	*	���݂̎���
	*/
	long mTime;

	/**
	*	ghost���y�A�����O���邽�߂̃R�[���o�b�N�N���X
	*/
	std::shared_ptr<btGhostPairCallback> mGhostCallBack;
};

