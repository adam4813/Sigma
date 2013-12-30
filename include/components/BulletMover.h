#pragma once

#include "IBulletShape.h"
#include "IMoverComponent.h"
#include "components/ControllableMove.h"
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
         * \brief Apply all forces in this mover's list.
         *
         * Calculates the total force and sets the rigid body's linear force.
         * Physics movers apply forces on a transform object.
         * \param id const id_t the id of the entity
         * \param[in] const double delta Change in time since the last call.
         */
        void ApplyForces(const id_t id, const double delta);

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
		void InitializeRigidBody();

		/**
		 * \brief Updates the transform to match its collision rigid body.
		 *
		 * \return void
		 */
		void UpdateTransform();

		void SetTransform(GLTransform& transform) {
			ControllableMove::SetTransform(IBulletShape::GetEntityID(), &transform);
		}
	};
}
