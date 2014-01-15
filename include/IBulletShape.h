#pragma once
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "IComponent.h"
#include "Sigma.h"

#include <memory>

namespace Sigma{
	class IBulletShape : public IComponent {
	public:
		IBulletShape(const id_t entityID = 0) : IComponent(entityID) {};
		virtual ~IBulletShape() {};

		virtual void InitializeRigidBody(float x, float y, float z, float rx, float ry, float rz) {
			btTransform transform;
			transform.setIdentity();
			transform.setRotation(btQuaternion(btVector3(0,1,0), btRadians(ry)));
			transform.setOrigin(btVector3(x, y, z));
			auto b = new btRigidBody(0, new btDefaultMotionState(transform), GetCollisionShape());
			SetRigidBody(b);
			b->setContactProcessingThreshold(BT_LARGE_FLOAT);
			b->setCcdMotionThreshold(.5);
			b->setCcdSweptSphereRadius(0);
		};

		btRigidBody* GetRigidBody() const { return this->body.get(); };

	protected:
		btCollisionShape* GetCollisionShape() const { return this->shape.get(); };
	    void SetCollisionShape(btCollisionShape* s) { shape = std::move(std::unique_ptr<btCollisionShape>(s)); }
	    void SetRigidBody(btRigidBody* b) { body = std::move(std::unique_ptr<btRigidBody>(b)); }

    private:
	    std::unique_ptr<btCollisionShape> shape;
		std::unique_ptr<btRigidBody> body;
	};
}
