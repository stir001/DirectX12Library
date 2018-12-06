#include "stdafx.h"
#include "BulletlibLink.h"
#include "BulletRigidBody.h"
#include "bullet/System/PhysicsSystem.h"
#include "bullet/Shape/BulletCollisionShape.h"

#include <btBulletDynamicsCommon.h>

int operator|(const BulletCollisionState lval, const BulletCollisionState rval)
{
	return static_cast<int>(lval) | static_cast<int>(rval);
}


BulletRigidBody::BulletRigidBody(std::shared_ptr<BulletCollisionShape> collisionShape, int worldID
	, const DirectX::XMFLOAT3& pos)
	: ICollisionObject(worldID), mCollisionShape(collisionShape)
	, mMass(1.0f)
{
	CreateRigidBody();
	Translate(pos.x, pos.y, pos.z);
}


BulletRigidBody::~BulletRigidBody()
{
	RemoveWorld();
}

std::shared_ptr<btRigidBody> BulletRigidBody::GetRigidBody()
{
	return mRigidBody;
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
	mRigidBody->translate(btVector3(x, y, z));
}

void BulletRigidBody::Translate(const DirectX::XMFLOAT3 & pos)
{
	Translate(pos.x, pos.y, pos.z);
}

DirectX::XMFLOAT4X4 BulletRigidBody::GetWorldTransform() const
{
	btTransform transform;
	mMotionState->getWorldTransform(transform);
	btScalar mat[16];
	transform.getOpenGLMatrix(mat);
	DirectX::XMFLOAT4X4 rtn{
		(float)mat[0], (float)mat[1], (float)mat[2] , (float)mat[3]
		, (float)mat[4], (float)mat[5], (float)mat[6], (float)mat[7]
		, (float)mat[8], (float)mat[9], (float)mat[10], (float)mat[11]
		, (float)mat[12] , (float)mat[13], (float)mat[14], (float)mat[15]
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

void BulletRigidBody::SetTag(int tag)
{
	mTag = tag;
	mRigidBody->setUserIndex(tag);
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
}

btCollisionObject * BulletRigidBody::GetPtr() const
{
	return mRigidBody.get();
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

	auto trans = mRigidBody->getWorldTransform();
}


