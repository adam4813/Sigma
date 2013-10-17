#pragma once
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"
#include "IComponent.h"


namespace Sigma{
	class IBulletShape : public IComponent {
	public:
		IBulletShape(const int entityID = 0) : IComponent(entityID) { }
		virtual ~IBulletShape() {
			if (this->body != nullptr) {
				delete this->body;
			}
			if (this->shape != nullptr) {
				delete this->shape;
			}
			if (this->motionState != nullptr) {
				delete this->motionState;
			}
		}

		virtual void InitializeRigidBody(float x, float y, float z, float rx, float ry, float rz) {
			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(btVector3(x, y, z));
			transform.setRotation(btQuaternion(rx, ry, rz, 1.0f));
			this->motionState = new btDefaultMotionState(transform);
			this->body = new btRigidBody(0, this->motionState, this->shape);
			this->body->setContactProcessingThreshold(BT_LARGE_FLOAT);
			this->body->setCcdMotionThreshold(.5);
			this->body->setCcdSweptSphereRadius(0);
		}

		btRigidBody* GetRigidBody() const { return this->body; }

	protected:
		btCollisionShape* shape;
		btRigidBody* body;
		btDefaultMotionState* motionState;
	};
}