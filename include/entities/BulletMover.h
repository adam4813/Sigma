#pragma once

#include <vector>
#include <string>

#include "IBulletShape.h"
#include "Property.h"
#include "composites/InterpolatedMovement.h"
#include "composites/ControllableMove.h"
#include "composites/RigidBody.h"
#include "GLTransform.h"
#include "Sigma.h"
#include "Property.h"
#include "systems/EntitySystem.h"

namespace Sigma {

    /** \brief BulletMover is an helper class for an entity that represents a local player
     */
	class BulletMover : public IEntity{
	public:

		/* An entity is not defined by the instanciation of its class, but by the components
		 * that it owns.
		 *
		 * If you manually add all the components that define an entity, then you must have created
		 * an entity strictly equivalent to the one created through instanciation. This class is just an helper to
		 * add all necessary components.
		 *
		 * Instanciating an entity allows to keep an OOP reference on the entity.
		 *
		 * The normal way to reference an entity is to keep the id for later use.
		 *
		 */

		DLL_EXPORT BulletMover(const id_t entityID, const std::vector<Property>& properties);
		virtual ~BulletMover() {};

		/**
		 * \brief Initalize the rigid body using the current transform or defaults if none
		 *
		 * it adds the body to the simulation
		 *
		 * \return void
		 */
		DLL_EXPORT void InitializeRigidBody(const std::vector<Property>& properties);

	private:
		std::weak_ptr<btRigidBody>* body;
	};
}
