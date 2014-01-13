#include "components/ControllableMove.h"
#include "components/PhysicalWorldLocation.h"
#include "components/RigidBody.h"

namespace Sigma {
    VectorMap<id_t, forces_list> ControllableMove::forces_map;
    std::unordered_map<id_t, rotationForces_list> ControllableMove::rotationForces_map;
    VectorMap<id_t, glm::vec3> ControllableMove::cumulatedForces_map;

    void ControllableMove::CumulateForces() {
        // TODO: vectorize this
        auto cf_it = cumulatedForces_map.getVector()->begin();
        auto f_it = forces_map.getVector()->begin();
        for (auto kitr = forces_map.IteratorKeyBegin(); kitr != forces_map.IteratorKeyEnd(); ++kitr, ++cf_it, ++f_it) {
            auto transform = PhysicalWorldLocation::GetTransform(*kitr);
            if (transform != nullptr) {
                glm::vec3 t;
                for (auto forceitr = f_it->begin(); forceitr != f_it->end(); ++forceitr) {
                    t += *forceitr;
                }
                *cf_it = (t.z * transform->GetForward()) +
                           (t.y * transform->GetUp()) +
                           (t.x * transform->GetRight());
            }
        }
    }

	// TODO: Optimize the creation of the btVector3
    void ControllableMove::ApplyForces(const double delta) {
        auto cf_it = cumulatedForces_map.getVector()->cbegin();
        for (auto kitr = cumulatedForces_map.IteratorKeyBegin(); kitr != cumulatedForces_map.IteratorKeyEnd(); ++kitr, ++cf_it) {
            auto body = RigidBody::getBody(*kitr);
            auto finalForce = cf_it;
            if (! body.expired()) {
                body.lock()->setLinearVelocity(btVector3(finalForce->x, body.lock()->getLinearVelocity().y() + 0.000000001f, finalForce->z));
            }
            else {
				auto position = PhysicalWorldLocation::getPosition(*kitr);
				if (position) {
					position->x += finalForce->x * delta;
					position->z += finalForce->z * delta;
					PhysicalWorldLocation::setPosition(*kitr, *position);
				}
            }
        }
    }

}
