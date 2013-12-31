#pragma once

#include "IBulletShape.h"
#include "components/InterpolatedMovement.h"
#include "components/ControllableMove.h"
#include "components/RigidBody.h"
#include "GLTransform.h"
#include "Sigma.h"

namespace Sigma {

	class BulletMover {
	public:
		BulletMover(const id_t entityID);
		~BulletMover();

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
