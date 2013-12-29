#pragma once
#ifndef PHSYICS_MOVER_H
#define PHSYICS_MOVER_H

#include "../IMoverComponent.h"
#include "Sigma.h"

class GLTransform;

class PhysicsMover : public Sigma::IComponent {
public:
    SET_COMPONENT_TYPENAME("PhysicsMover");
	PhysicsMover() : Sigma::IComponent(0) { }
	PhysicsMover(const id_t entityID);

    /**
     * \brief Apply all forces in this mover's list.
     *
     * Physics movers apply forces on a transform object.
     * \param id const id_t the id of the entity
     * \param[in] const double delta Change in time since the last call.
     */
    void ApplyForces(const id_t id, const double delta);

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
