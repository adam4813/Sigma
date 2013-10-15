#include "ViewMover.h"

#include "../systems/IGLView.h"

ViewMover::ViewMover(const int entityID) : IMoverComponent(entityID) {

}

void ViewMover::ApplyForces(const double delta) {
	glm::vec3 deltavec(delta);

	glm::vec3 totalForce;
	for (auto forceitr = this->forces.begin(); forceitr != this->forces.end(); ++forceitr) {
		totalForce += *forceitr;
	}

	for (glm::vec3 normal : this->normalForces) {
		float scalar = glm::dot(normal, totalForce);
		if (scalar < 0) {
			totalForce = totalForce - scalar * normal;
		}
	}
	this->normalForces.clear();

	this->view->Move(totalForce * deltavec);

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

void ViewMover::AddNormalForce(glm::vec3 normal) {
	this->normalForces.push_back(normal);
}
