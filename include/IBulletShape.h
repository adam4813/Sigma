#pragma once
#define BT_USE_DOUBLE_PRECISION
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "IComponent.h"
#include "components/SigmaMotionState.h"


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

		virtual void InitializeRigidBody(float x, float y, float z, float rx, float ry, float rz);
		btRigidBody* GetRigidBody() const { return this->body; }

	protected:
		btCollisionShape* shape;
		btRigidBody* body;
		SigmaMotionState* motionState;
	};
}
