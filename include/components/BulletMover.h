#pragma once

#include "IBulletShape.h"
#include "IMoverComponent.h"
#include "GLTransform.h"

namespace Sigma {

	class BulletMover : public IBulletShape, public IMoverComponent {
	public:
		SET_COMPONENT_TYPENAME("BulletMover");
		BulletMover() : IBulletShape(0), transform(nullptr) { }
		BulletMover(const int entityID);
		~BulletMover();

		/**
		 * \brief Apply all forces in this mover's list.
		 *
		 * Calculates the total force and sets the rigid body's linear force.
		 * Physics movers apply forces on a transform object.
		 * \param[in] const double delta Change in time since the last call.
		 */
		void ApplyForces(const double delta);

		/**
		 * \brief Initialize the rigid body.
		 *
		 * \param[in] float x, y, z The initial position.
		 * \param[in] float rx, ry, rz The initial rotation.
		 * \return    void 
		 */
		void InitializeRigidBody(float x, float y, float z, float rx, float ry, float rz);

		/**
		 * \brief Updates the transform to match its collision rigid body.
		 *
		 * \return void 
		 */
		void UpdateTransform();

		/**
		 * \brief Rotates the transform now.
		 *
		 * Rotates the transform immediately instead of adding it to the list.
		 * \param[in/out] float x, y, z The amount to rotate.
		 * \return void
		 */
		void RotateNow(float x, float y, float z);
		/**
		 * \brief Sets the target rotation amount.
		 *
		 * 
		 * \param[in/out] float x, y, z The amount to rotate to.
		 * \return void
		 */
		void RotateTarget(float x, float y, float z);

		void SetTransform(GLTransform& transform) {
			this->transform = &transform;
		}
	private:
		GLTransform* transform;
		glm::vec3 _rotationtarget;
	};
}
