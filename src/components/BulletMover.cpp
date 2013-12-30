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

            RigidBody::getBody(IBulletShape::GetEntityID())->setLinearVelocity(btVector3(finalForce->x, RigidBody::getBody(IBulletShape::GetEntityID())->getLinearVelocity().y() + 0.000000001f, finalForce->z));
	    }
	}

	void BulletMover::InitializeRigidBody(float x, float y, float z, float rx, float ry, float rz) {
		RigidBody::AddEntity(IBulletShape::GetEntityID(), x, y, z, rx, ry, rz);
		SetCollisionShape(RigidBody::getBody(IBulletShape::GetEntityID())->getCollisionShape());
	}

	void BulletMover::InitializeRigidBody(btDiscreteDynamicsWorld* world) {
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
		world->addRigidBody(RigidBody::getBody(IBulletShape::GetEntityID()));
        RigidBody::getBody(IBulletShape::GetEntityID())->setActivationState(DISABLE_DEACTIVATION);
	}
}
