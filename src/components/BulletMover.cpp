#include "components/BulletMover.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

namespace Sigma {

	BulletMover::BulletMover(const id_t entityID) : entityID(entityID) {
        InterpolatedMovement::AddEntity(entityID);
        ControllableMove::AddEntity(entityID);
	}

	BulletMover::~BulletMover() {}

	void BulletMover::InitializeRigidBody(btDiscreteDynamicsWorld* world) {
	    auto transform = ControllableMove::GetTransform(entityID);
		if(transform != nullptr) {
		RigidBody::AddEntity(entityID,
				transform->GetPosition().x,
				transform->GetPosition().y,
				transform->GetPosition().z,
				transform->GetPitch(),
				transform->GetYaw(),
				transform->GetRoll()
			);
		}
		else {
			RigidBody::AddEntity(entityID,0,1.5f,0,0,0,0);
		}
		world->addRigidBody(RigidBody::getBody(entityID));
        RigidBody::getBody(entityID)->setActivationState(DISABLE_DEACTIVATION);
	}
}
