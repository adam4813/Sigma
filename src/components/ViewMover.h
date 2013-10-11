#pragma once

#include "../IMoverComponent.h"
#include <list>

struct IGLView;

class ViewMover : public IMoverComponent {
public:
    SET_COMPONENT_ID("ViewMover");
	ViewMover() : IMoverComponent(0) { }
	ViewMover(const int entityID);

	/**
	 * \brief Apply all forces in this mover's list.
	 *
	 * Physics movers apply forces on a transform object.
	 * \param[in] const double delta Change in time since the last call.
	 */
	void ApplyForces(const double delta);

	/**
	 * \brief Sets the view that this mover acts on.
	 *
	 * \param[in] IGLView * view The view to apply the forces on.
	 */
	void View(IGLView* view);
	const IGLView* View();
private:
	IGLView* view;
};
