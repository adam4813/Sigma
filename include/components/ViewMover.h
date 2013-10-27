#pragma once
#ifndef VIEW_MOVER_H
#define VIEW_MOVER_H

#include "../IMoverComponent.h"
#include <list>

namespace Sigma{
    struct IGLView;
}

class ViewMover : public Sigma::IMoverComponent {
public:
    SET_COMPONENT_TYPENAME("ViewMover");
	ViewMover() : Sigma::IMoverComponent(0) { }
	ViewMover(const int entityID);

	/**
	 * \brief Apply all forces in this mover's list.
	 *
	 * Physics movers apply forces on a transform object.
	 * \param[in] const double delta Change in time since the last call.
	 */
	virtual void ApplyForces(const double delta);

	/**
	 * \brief Sets the view that this mover acts on.
	 *
	 * \param[in] IGLView * view The view to apply the forces on.
	 */
	void View(Sigma::IGLView* view);
	Sigma::IGLView* View();

	void AddNormalForce( glm::vec3 normal );
	void RotateNow(float,float,float);
	void RotateTarget(float,float,float);

protected:
	Sigma::IGLView* view;
	std::list<glm::vec3> normalForces;
	glm::vec3 _rotationtarget;
}; // class ViewMover

#endif // VIEW_MOVER_H
