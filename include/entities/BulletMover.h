#pragma once

#include <vector>
#include <string>

#include "IBulletShape.h"
#include "Property.h"
#include "components/InterpolatedMovement.h"
#include "components/ControllableMove.h"
#include "components/RigidBody.h"
#include "GLTransform.h"
#include "Sigma.h"

namespace Sigma {

    /** \brief BulletMover is an helper class for an entity that represents a local player
     */
	class BulletMover {
	public:

		/* An entity is not defined by the instanciation of its class, but by the components
		 * that it owns. An entity has no data or code except create and delete functions.
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

		BulletMover(const id_t entityID);
		virtual ~BulletMover() {};

		/**
		 * \brief Initalize the rigid body using the current transform or defaults if none
		 *
		 * it adds the body to the simulation
		 *
		 * \return void
		 */
		void InitializeRigidBody(btDiscreteDynamicsWorld* world);

		const id_t GetEntityID() const { return entityID; };
	private:
        const id_t entityID;
	};
}
