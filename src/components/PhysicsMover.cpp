#include "components/PhysicsMover.h"
#include "GLTransform.h"

PhysicsMover::PhysicsMover(const int entityID) : IMoverComponent(entityID) {

}

void PhysicsMover::ApplyForces(const double delta) {
	glm::vec3 deltavec = glm::vec3(delta);

	// Rotation forces first
	for (auto rotitr = this->rotationForces.begin(); rotitr != this->rotationForces.end(); ++rotitr) {
		this->transform->Rotate((*rotitr) * deltavec);
	}

	// Directional forces second, as Move depends on transform.orientation
	for (auto forceitr = this->forces.begin(); forceitr != this->forces.end(); ++forceitr) {
		//this->transform->Translate((*forceitr) * deltavec);

		// For now we are applying forces relative to the orientation
		// i.e. ships engines "10.0f forward" instead of "-10.0f z"
		this->transform->Move((*forceitr) * deltavec);
	}
}
