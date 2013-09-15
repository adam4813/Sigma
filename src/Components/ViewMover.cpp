#include "ViewMover.h"

#include "../Systems/IGLView.h"

ViewMover::ViewMover(const int entityID) : IMoverComponent(entityID) {

}

void ViewMover::ApplyForces(const double delta) {
	glm::vec3 deltavec = glm::vec3(delta);
	for (auto forceitr = this->forces.begin(); forceitr != this->forces.end(); ++forceitr) {
		this->view->Move((*forceitr) * deltavec);
	}
}

void ViewMover::View(IGLView* view) {
	this->view = view;
}
