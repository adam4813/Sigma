#include "ViewMover.h"

#include "../systems/IGLView.h"

ViewMover::ViewMover(const int entityID) : IMoverComponent(entityID) {

}

void ViewMover::ApplyForces(const double delta) {
	glm::vec3 deltavec = glm::vec3(delta);
	for (auto forceitr = this->forces.begin(); forceitr != this->forces.end(); ++forceitr) {
		this->view->Move((*forceitr) * deltavec);
	}
	for (auto rotitr = this->rotationForces.begin(); rotitr != this->rotationForces.end(); ++rotitr) {
		this->view->Rotate((*rotitr) * deltavec);
	}
}

void ViewMover::View(IGLView* view) {
	this->view = view;
}
const IGLView* ViewMover::View() {
	return this->view;
}

void ViewMover::AddRotationForce(glm::vec3 rotation) {
	for (auto rotitr = this->rotationForces.begin(); rotitr != this->rotationForces.end(); ++rotitr) {
		if ((*rotitr) == rotation) {
			return;
		}
	}
	this->rotationForces.push_back(rotation);
}

void ViewMover::RemoveRotationForce(glm::vec3 rotation) {
	this->rotationForces.remove(rotation);
}