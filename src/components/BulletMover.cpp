#include "components/BulletMover.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

namespace Sigma {

	BulletMover::BulletMover(const id_t entityID) : IBulletShape(entityID) {
        IMoverComponent::AddEntity(entityID);
        ControllableMove::AddEntity(entityID);
	}

	BulletMover::~BulletMover() {}

	void BulletMover::ApplyForces(const id_t id, const double delta) {
	    auto transform = ControllableMove::GetTransform(IBulletShape::GetEntityID());
	    if (transform != nullptr) {
            // TODO : use the id parameter
            auto finalForce = IMoverComponent::GetTransformedForces(IBulletShape::GetEntityID(), transform);

            GetRigidBody()->setLinearVelocity(btVector3(finalForce->x, GetRigidBody()->getLinearVelocity().y() + 0.000000001f, finalForce->z));
	    }
	}

	void BulletMover::UpdateTransform() {
	    auto transform = ControllableMove::GetTransform(IBulletShape::GetEntityID());
		if (transform != nullptr) {
			btTransform trans;
			GetRigidBody()->getMotionState()->getWorldTransform(trans);
			transform->TranslateTo(trans.getOrigin().x(),trans.getOrigin().y(), trans.getOrigin().z());
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
	    auto transform = ControllableMove::GetTransform(IBulletShape::GetEntityID());
		if(transform != nullptr) {
			this->InitializeRigidBody(
				transform->GetPosition().x,
				transform->GetPosition().y,
				transform->GetPosition().z,
				transform->GetPitch(),
				transform->GetYaw(),
				transform->GetRoll()
			);
		}
		else {
			this->InitializeRigidBody(0,1.5f,0,0,0,0);
		}
        GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
	}

	// immediate mode rotation (for mouse motion)
	void BulletMover::RotateNow(float x, float y, float z) {
	    auto transform = ControllableMove::GetTransform(IBulletShape::GetEntityID());
		if (transform != nullptr) {
			transform->Rotate(x,y,z);
		}
	}
	void BulletMover::RotateTarget(float x, float y, float z) {
		IMoverComponent::RotateTarget(IBulletShape::GetEntityID(), x, y, z);
	}
}
