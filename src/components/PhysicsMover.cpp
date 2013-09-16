#include "PhysicsMover.h"
#include "../GLTransform.h"

PhysicsMover::PhysicsMover(const int entityID) : IMoverComponent(entityID) {

}

void PhysicsMover::ApplyForces(const double delta) {
	glm::vec3 deltavec = glm::vec3(delta);
	for (auto forceitr = this->forces.begin(); forceitr != this->forces.end(); ++forceitr) {
		this->transform->Translate((*forceitr) * deltavec);
	}
}
