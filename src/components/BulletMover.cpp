#include "components/BulletMover.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

namespace Sigma {

	BulletMover::BulletMover(const id_t entityID) : entityID(entityID) {
	    CanMove::AddEntity(entityID);
        InterpolatedMovement::AddEntity(entityID);
        ControllableMove::AddEntity(entityID);
	}

	BulletMover::~BulletMover() {}

	void BulletMover::InitializeRigidBody(float x, float y, float z, float rx, float ry, float rz) {
		RigidBody::AddEntity(entityID, x, y, z, rx, ry, rz);
	}

	void BulletMover::InitializeRigidBody(btDiscreteDynamicsWorld* world) {
	    auto transform = ControllableMove::GetTransform(entityID);
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
		world->addRigidBody(RigidBody::getBody(entityID));
        RigidBody::getBody(entityID)->setActivationState(DISABLE_DEACTIVATION);
	}
}
