#pragma once

#include <list>
#include "../IBulletShape.h"
#include "ViewMover.h"

struct IGLView;
namespace Sigma {
	class BulletMover : public IBulletShape, public ViewMover {
	public:
		SET_COMPONENT_ID("BulletMover");
		BulletMover() : IBulletShape(0) { }
		BulletMover(const int entityID);
		~BulletMover();

		/**
		 * \brief Apply all forces in this mover's list.
		 *
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
		 * \brief Updates the internal view's transform to match its collision rigid body.
		 *
		 * \return    void 
		 */
		void UpdateView();
	private:
	};
}
