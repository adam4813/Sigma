#pragma once
#ifndef PHSYICS_MOVER_H
#define PHSYICS_MOVER_H

#include "../IMoverComponent.h"
class GLTransform;

class PhysicsMover : public Sigma::IMoverComponent {
public:
    SET_COMPONENT_TYPENAME("PhysicsMover");
	PhysicsMover() : Sigma::IMoverComponent(0) { }
	PhysicsMover(const int entityID);

	/**
	 * \brief Apply all forces in this mover's list.
	 *
	 * Physics movers apply forces on a transform object.
	 * \param[in] const double delta Change in time since the last call.
	 */
	void ApplyForces(const double delta);

	/**
	 * \brief Sets the transform this mover acts on.
	 *
	 *
	 * \param[in] GLTransform * transform The pointer to the transform to manipulate.
	 */
	void SetTransform(GLTransform* transform) {
		this->transform = transform;
	}

	/**
	 * \brief Retrieves the transform object for this component.
	 *
	 * \return GLTransform& The transform object.
	 */
	GLTransform* Transform() const { return this->transform; }
protected:
	GLTransform* transform; // The transform that forces are applied to.
}; // class PhysicsMover

#endif // PHSYICS_MOVER_H
