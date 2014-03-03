#include "components/PhysicsMover.h"
#include "GLTransform.h"

namespace Sigma {
	PhysicsMover::PhysicsMover(const id_t entityID) : IComponent(entityID) {
		ControllableMove::AddEntity(entityID);
	}

	void PhysicsMover::ApplyForces(const id_t id, const double delta) {
		glm::vec3 deltavec = glm::vec3(delta);

		// Rotation forces first
		// TODO : use the id parameter
		auto rotationForces = ControllableMove::getRotationForces(this->GetEntityID());
		if (rotationForces == nullptr) {
				assert(0 && "id does not exist");
		}

		for (auto rotitr = rotationForces->begin(); rotitr != rotationForces->end(); ++rotitr) {
			this->transform->Rotate((*rotitr) * deltavec);
		}

		// Directional forces second, as Move depends on transform.orientation
		// TODO : use the id parameter
		auto forces = ControllableMove::getForces(this->GetEntityID());
		if (forces == nullptr) {
				assert(0 && "id does not exist");
		}

		for (auto forceitr = forces->begin(); forceitr != forces->end(); ++forceitr) {
			//this->transform->Translate((*forceitr) * deltavec);

			// For now we are applying forces relative to the orientation
			// i.e. ships engines "10.0f forward" instead of "-10.0f z"
			this->transform->Move((*forceitr) * deltavec);
		}
	}
}
