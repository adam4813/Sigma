#include "components/BulletMover.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

namespace Sigma {

	BulletMover::BulletMover(const id_t entityID) : IBulletShape(entityID), transform(nullptr) {
        IMoverComponent::AddEntity(entityID);
	}

	BulletMover::~BulletMover() {}

	void BulletMover::ApplyForces(const id_t id, const double delta) {
        IMoverComponent::ComputeInterpolatedForces(IBulletShape::GetEntityID(), delta, this->transform);

        // TODO : use the id parameter
        auto finalForce = IMoverComponent::GetTransformedForces(IBulletShape::GetEntityID(), this->transform);

        GetRigidBody()->setLinearVelocity(btVector3(finalForce->x, GetRigidBody()->getLinearVelocity().y() + 0.000000001f, finalForce->z));
	}

	void BulletMover::UpdateTransform() {
		if (this->transform) {
			btTransform trans;
			GetRigidBody()->getMotionState()->getWorldTransform(trans);
			this->transform->TranslateTo(trans.getOrigin().x(),trans.getOrigin().y(), trans.getOrigin().z());
		}
	}

	void BulletMover::InitializeRigidBody(float x, float y, float z, float rx, float ry, float rz) {
		auto shape = new btCapsuleShape(0.3f, 1.3f);
		SetCollisionShape(shape);
		btScalar mass = 1;
		btVector3 fallInertia(0,0,0);
		auto motionState =	new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(x, y, z)));
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape,fallInertia);
		shape->calculateLocalInertia(mass,fallInertia);
		SetRigidBody(new btRigidBody(fallRigidBodyCI));
	}

	void BulletMover::InitializeRigidBody() {
		if(this->transform) {
			this->InitializeRigidBody(
				this->transform->GetPosition().x,
				this->transform->GetPosition().y,
				this->transform->GetPosition().z,
				this->transform->GetPitch(),
				this->transform->GetYaw(),
				this->transform->GetRoll()
			);
		}
		else {
			this->InitializeRigidBody(0,1.5f,0,0,0,0);
		}
        GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
	}

	// immediate mode rotation (for mouse motion)
	void BulletMover::RotateNow(float x, float y, float z) {
		if (this->transform) {
			this->transform->Rotate(x,y,z);
		}
	}
	void BulletMover::RotateTarget(float x, float y, float z) {
		if (this->transform) {
			IMoverComponent::RotateTarget(IBulletShape::GetEntityID(), x, y, z);
		}
	}
}
