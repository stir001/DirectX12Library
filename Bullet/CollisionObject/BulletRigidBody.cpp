#include "stdafx.h"
#include "BulletlibLink.h"
#include "BulletRigidBody.h"
#include "bullet/System/PhysicsSystem.h"
#include "bullet/Shape/BulletCollisionShape.h"
#include "Bullet/Action/IActionDefiner.h"
#include "Bullet/CollisionObject/BulletGhostObject.h"

#include <btBulletDynamicsCommon.h>
#include <algorithm>

BulletRigidBody::BulletRigidBody(std::shared_ptr<BulletCollisionShape> collisionShape, int worldID
	, const DirectX::XMFLOAT3& pos)
	: ICollisionObject(worldID), mCollisionShape(collisionShape)
	, mMass(1.0f)
{
	CreateRigidBody();
	SetOrigin(pos.x, pos.y, pos.z);
}


BulletRigidBody::~BulletRigidBody()
{
	RemoveWorld();
	for (auto action : mIgnoreActions)
	{
		mRigidBody->setIgnoreCollisionCheck(action->GetGhost()->GetPtr().get(), false);
	}
	mIgnoreActions.clear();
}

void BulletRigidBody::SetMass(float mass)
{
	mMass = mass;
	btVector3 bodyInertia;
	mCollisionShape->GetShape()->calculateLocalInertia(mMass, bodyInertia);
	mRigidBody->setMassProps(mMass, bodyInertia);
}

void BulletRigidBody::Translate(float x, float y, float z)
{
	btVector3 offset = { x,y,z };
	mRigidBody->translate(offset);
	mRigidBody->forceActivationState(ACTIVE_TAG);
	if (!(x == 0 && y == 0 && z == 0))
	{
		mRigidBody->setLinearVelocity(offset.normalized());
	}
}

void BulletRigidBody::Translate(const DirectX::XMFLOAT3 & pos)
{
	SetOrigin(pos.x, pos.y, pos.z);
}

DirectX::XMFLOAT4X4 BulletRigidBody::GetWorldTransform() const
{
	btTransform transform;
	mMotionState->getWorldTransform(transform);
	btScalar mat[16];
	transform.getOpenGLMatrix(mat);
	DirectX::XMFLOAT4X4 rtn{
		mat[0], mat[1], mat[2] , mat[3]
		, mat[4], mat[5], mat[6], mat[7]
		, mat[8], mat[9], mat[10], mat[11]
		, mat[12] , mat[13], mat[14], mat[15]
	};

	return rtn;
}

void BulletRigidBody::SetWorldTransform(const DirectX::XMFLOAT4X4& matrix)
{
	btScalar mat[16] =
	{
		matrix._11, matrix._12, matrix._13, matrix._14
		, matrix._21, matrix._22, matrix._23, matrix._24
		, matrix._31, matrix._32, matrix._33, matrix._34
		, matrix._41, matrix._42, matrix._43, matrix._44
	};
	btTransform transform;
	mMotionState->getWorldTransform(transform);
	transform.setOrigin(btVector3(matrix._41, matrix._42, matrix._43));
	transform.setFromOpenGLMatrix(mat);
	mMotionState->setWorldTransform(transform);
}

void BulletRigidBody::SetTag1(int tag)
{
	mTag1 = tag;
	mRigidBody->setUserIndex(tag);
}

void BulletRigidBody::SetTag2(int tag)
{
	mTag2 = tag;
	mRigidBody->setUserIndex2(tag);
}

void BulletRigidBody::SetCollisionState(int state)
{
	mRigidBody->setCollisionFlags(state);
}

void BulletRigidBody::SetCollisionState(BulletCollisionState state)
{
	mRigidBody->setCollisionFlags(static_cast<int>(state));
}

void BulletRigidBody::SetFriction(float friction)
{
	mRigidBody->setFriction(friction);
}

void BulletRigidBody::SetSpinFriction(float friction)
{
	mRigidBody->setSpinningFriction(friction);
}

void BulletRigidBody::SetRestitution(float restitution)
{
	mRigidBody->setRestitution(restitution);
}

void BulletRigidBody::SetYawPitchRoll(const DirectX::XMFLOAT3 & ypr)
{
	SetYawPitchRoll(ypr.x, ypr.y, ypr.z);
}

void BulletRigidBody::SetYawPitchRoll(float x, float y, float z)
{
	btMatrix3x3 mat;
	mat.setIdentity();
	mat.setEulerYPR(x, y, z);
	btTransform trans = mRigidBody->getWorldTransform();
	trans.setBasis(mat);
	mRigidBody->setWorldTransform(trans);
}

void BulletRigidBody::SetOrigin(const DirectX::XMFLOAT3& pos)
{
	SetOrigin(pos.x, pos.y, pos.z);
}

void BulletRigidBody::SetOrigin(float x, float y, float z)
{
	auto trans = mRigidBody->getWorldTransform();
	trans.setOrigin(btVector3(x, y, z));
	mRigidBody->setWorldTransform(trans);
	mRigidBody->forceActivationState(ACTIVE_TAG);
}

std::shared_ptr<btCollisionObject> BulletRigidBody::GetPtr() const
{
	return mRigidBody;
}

std::shared_ptr<btRigidBody> BulletRigidBody::GetRigidPtr() const
{
	return mRigidBody;
}

void BulletRigidBody::SetIgnoreAction(std::shared_ptr<IActionDefiner> ignoreAction)
{
	mRigidBody->setIgnoreCollisionCheck(ignoreAction->GetGhost()->GetPtr().get(), true);
	mIgnoreActions.push_back(ignoreAction);
}

void BulletRigidBody::RemoveIgnoreAction(std::shared_ptr<IActionDefiner> ignoreAction)
{
	auto find = std::find_if(mIgnoreActions.begin(), mIgnoreActions.end(), [ignoreAction](std::shared_ptr<IActionDefiner> action) 
	{
		return ignoreAction->GetGhost()->GetWorldID() == action->GetGhost()->GetWorldID();
	});
	if (find != mIgnoreActions.end())
	{
		mIgnoreActions.erase(find);
		mRigidBody->setIgnoreCollisionCheck(ignoreAction->GetGhost()->GetPtr().get(), false);
	}
}

void BulletRigidBody::SetAngularFactor(float factor)
{
	mRigidBody->setAngularFactor(factor);
}

DirectX::XMFLOAT3 BulletRigidBody::GetOrigin() const
{
	auto trans = mRigidBody->getWorldTransform();
	auto origin = trans.getOrigin();
	return DirectX::XMFLOAT3(origin.x(), origin.y(), origin.z());
}

void BulletRigidBody::AddForce(float x, float y, float z)
{
	Activation();
	mRigidBody->applyCentralForce({ x,y,z });
}

void BulletRigidBody::AddForce(const DirectX::XMFLOAT3 & force)
{
	AddForce(force.x, force.y, force.z);
}

void BulletRigidBody::AddImpulse(float x, float y, float z)
{
	Activation();
	mRigidBody->applyCentralImpulse({ x, y, z });
}

void BulletRigidBody::AddImpulse(const DirectX::XMFLOAT3 & impulse)
{
	AddImpulse(impulse.x, impulse.y, impulse.z);
}

void BulletRigidBody::SetVelocity(float x, float y, float z)
{
	Activation();
	mRigidBody->setLinearVelocity({ x,y,z });
}

void BulletRigidBody::SetVelocity(const DirectX::XMFLOAT3 & vel)
{
	SetVelocity(vel.x, vel.y, vel.z);
}

DirectX::XMFLOAT3 BulletRigidBody::GetVelocity() const
{
	auto vel = mRigidBody->getLinearVelocity();
	return DirectX::XMFLOAT3(vel.x(),vel.y(),vel.z());
}

void BulletRigidBody::SetGravity(const DirectX::XMFLOAT3 & gravity)
{
	mRigidBody->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

void BulletRigidBody::SetGravity(float x, float y, float z)
{
	mRigidBody->setGravity({ x,y,z });
}

void BulletRigidBody::RemoveWorld()
{
	PhysicsSystem::Instance().RemoveRigidBody(GetWorldID());
}

void BulletRigidBody::CreateRigidBody()
{
	mMotionState = std::make_shared<btDefaultMotionState>();
	btVector3 bodyInertia;
	mCollisionShape->GetShape()->calculateLocalInertia(mMass, bodyInertia);
	auto bodyCI = btRigidBody::btRigidBodyConstructionInfo(mMass, mMotionState.get(), mCollisionShape->GetShape().get(), bodyInertia);

	bodyCI.m_restitution = 0.5f;
	bodyCI.m_friction = 1.0f;

	mRigidBody = std::make_shared<btRigidBody>(bodyCI);

	int flag = mRigidBody->getCollisionFlags();
	mRigidBody->setCollisionFlags(static_cast<int>(BulletCollisionState::CHARACTER) | flag);
}

void BulletRigidBody::Activation()
{
	mRigidBody->forceActivationState(ACTIVE_TAG);
}


