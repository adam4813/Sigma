#pragma once
#ifndef VIEW_MOVER_H
#define VIEW_MOVER_H

#include "../IMoverComponent.h"
#include <list>

namespace Sigma{
    struct IGLView;
}

class CameraMover : public Sigma::IMoverComponent {
public:
    SET_COMPONENT_TYPENAME("CameraMover");
	CameraMover() : Sigma::IMoverComponent(0) { }
	CameraMover(const int entityID);

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

	void RotateNow(float,float,float);
	void RotateTarget(float,float,float);

protected:
	Sigma::IGLView* view;
	std::list<glm::vec3> normalForces;
	glm::vec3 _rotationtarget;
}; // class CameraMover

#endif // VIEW_MOVER_H
