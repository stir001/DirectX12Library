#pragma once
#include <btBulletDynamicsCommon.h>
#include <memory>

class BulletCollisionShape
{
public:
	BulletCollisionShape();
	BulletCollisionShape(std::shared_ptr<btCollisionShape> collision);
	virtual ~BulletCollisionShape();

	std::shared_ptr<btCollisionShape> GetShape() const;
protected:
	std::shared_ptr<btCollisionShape> mCollisionShape;
};

