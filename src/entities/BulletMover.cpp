#include "entities/BulletMover.h"

#include <bullet/btBulletDynamicsCommon.h>
#include "components/PhysicalWorldLocation.h"

namespace Sigma {

	BulletMover::BulletMover(const id_t entityID, const std::vector<Property>& properties) : IEntity(entityID), body(0) {
        IEntity::entitySystem->addComposite(this, CompositeID("PhysicalWorldLocation"), properties);
        IEntity::entitySystem->addComposite(this, CompositeID("InterpolatedMovement"), properties);
        IEntity::entitySystem->addComposite(this, CompositeID("ControllableMove"), properties);
	}

	// TODO : make the Bullet simulation static so that we can
	// put this in the constructor without passing the world as argument
	void BulletMover::InitializeRigidBody(const std::vector<Property>& properties) {
		// Add the body component
		IEntity::entitySystem->addComposite(this, CompositeID("RigidBody"), properties);
		// store the body component
		body = GetAs<Body>()->get();
		// TODO: move this. Is it really necessary anyway ?
        RigidBody::getBody(entityID).lock()->setActivationState(DISABLE_DEACTIVATION);
	}
}
