#include "entities/BulletMover.h"

#include <bullet/btBulletDynamicsCommon.h>
#include "components/PhysicalWorldLocation.h"
#include "systems/EntitySystem.h"

namespace Sigma {

	BulletMover::BulletMover(const id_t entityID, const std::vector<Property>& properties) : IEntity(entityID) {
        IEntity::entitySystem->addFeature(this, FeatureID("PhysicalWorldLocation"), properties);
        IEntity::entitySystem->addFeature(this, FeatureID("InterpolatedMovement"), properties);
        IEntity::entitySystem->addFeature(this, FeatureID("ControllableMove"), properties);
	}

	// TODO : make the Bullet simulation static so that we can
	// put this in the constructor without passing the world as argument
	void BulletMover::InitializeRigidBody(const std::vector<Property>& properties) {
		// Add the body component
		IEntity::entitySystem->addFeature(this, FeatureID("RigidBody"), properties);
		// TODO: move this. Is it really necessary anyway ?
        RigidBody::getBody(entityID)->setActivationState(DISABLE_DEACTIVATION);
	}
}
