#include "entities/BulletMover.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "components/PhysicalWorldLocation.h"

namespace Sigma {

	BulletMover::BulletMover(const id_t entityID) : entityID(entityID) {
	    // we add the components (if they already exist, nothing is done)
        InterpolatedMovement::AddEntity(entityID);
        ControllableMove::AddEntity(entityID);
        // TODO : the position should already exist
        // We must check that this is the case
        // position is currently hardcoded
        PhysicalWorldLocation::AddEntityPosition(entityID, 0, 1.5, 0, 0, 0, 0);
	}

	// TODO : make the Bullet simulation static so that we can
	// put this in the constructor without passing the world as argument
	void BulletMover::InitializeRigidBody(btDiscreteDynamicsWorld* world) {
		// Add the body component
		if (RigidBody::AddEntity(entityID, std::vector<Property>())) {
			// Add the body to the world
			// TODO: move this to the RigidBody component
			world->addRigidBody(RigidBody::getBody(entityID));
		}
		// TODO: move this. Is it really necessary anyway ?
        RigidBody::getBody(entityID)->setActivationState(DISABLE_DEACTIVATION);
	}
}
