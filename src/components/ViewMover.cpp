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

	glm::vec3 outForce = totalForce;
	for (auto forceitr = this->normalForces.begin(); forceitr != this->normalForces.end(); ++forceitr) {
		float scalar = glm::dot(*forceitr, totalForce);
		outForce -= (scalar + 1.0f) * *forceitr;
	}

	this->normalForces.clear();

	this->view->Move(outForce * deltavec);

	for (auto rotitr = this->rotationForces.begin(); rotitr != this->rotationForces.end(); ++rotitr) {
		glm::vec3 rotation_amount = (*rotitr) * deltavec;

		// Restricts the total rotation allowed to a specified range
		//rotation_amount = this->view->Restrict(rotation_amount);

		this->view->Transform.Rotate(rotation_amount);
	}
}

void ViewMover::View(IGLView* view) {
	this->view = view;
}

IGLView* ViewMover::View() {
	return this->view;
}

void ViewMover::AddNormalForce(glm::vec3 normal) {
	this->normalForces.push_back(normal);
}
