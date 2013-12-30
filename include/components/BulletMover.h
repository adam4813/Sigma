#pragma once

#include "IBulletShape.h"
#include "components/InterpolatedMovement.h"
#include "components/ControllableMove.h"
#include "components/RigidBody.h"
#include "GLTransform.h"
#include "Sigma.h"

namespace Sigma {

	class BulletMover : public IBulletShape {
	public:
		SET_COMPONENT_TYPENAME("BulletMover");
		BulletMover() : IBulletShape(0) { }
		BulletMover(const id_t entityID);
		~BulletMover();

		/**
		 * \brief Initialize the rigid body.
		 *
		 * \param[in] float x, y, z The initial position.
		 * \param[in] float rx, ry, rz The initial rotation.
		 * \return    void
		 */
		void InitializeRigidBody(float x, float y, float z, float rx, float ry, float rz);

		/**
		 * \brief Initalize the rigid body using the current transform or defaults if none
		 *
		 * \return void
		 */
		void InitializeRigidBody(btDiscreteDynamicsWorld* world);

		void SetTransform(GLTransform& transform) {
			ControllableMove::SetTransform(IBulletShape::GetEntityID(), &transform);
		}
	};
}
