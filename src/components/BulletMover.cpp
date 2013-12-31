#include "components/BulletMover.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "components/PhysicalWorldLocation.h"

namespace Sigma {

	BulletMover::BulletMover(const id_t entityID) : entityID(entityID) {
	    // we create the components
        InterpolatedMovement::AddEntity(entityID);
        ControllableMove::AddEntity(entityID);
        // position is hardcoded
        PhysicalWorldLocation::AddEntity(entityID, 0, 1.5, 0, 0, 0, 0);
	}

	BulletMover::~BulletMover() {}

	void BulletMover::InitializeRigidBody(btDiscreteDynamicsWorld* world) {
		RigidBody::AddEntity(entityID);
		world->addRigidBody(RigidBody::getBody(entityID));
        RigidBody::getBody(entityID)->setActivationState(DISABLE_DEACTIVATION);
	}
}
