#include "components/ControllableMove.h"
#include "components/PhysicalWorldLocation.h"
#include "components/RigidBody.h"

namespace Sigma {
    std::unordered_map<id_t, forces_list> ControllableMove::forces_map;
    std::unordered_map<id_t, rotationForces_list> ControllableMove::rotationForces_map;
    VectorMap<id_t, glm::vec3> ControllableMove::cumulatedForces_map;
    std::unordered_map<id_t, GLTransform> ControllableMove::transform_map;
    std::unordered_map<id_t, std::shared_ptr<GLTransform>> ControllableMove::transform_ptr_map;

    void ControllableMove::UpdateTransform() {
        for (auto it = transform_map.begin(); it != transform_map.end(); it++) {
            auto position = PhysicalWorldLocation::getPosition(it->first);
            if (position) {
                it->second.TranslateTo(position->x, position->y, position->z);
            }
        }
    }

    void ControllableMove::CumulateForces() {
        // TODO: optimize the way to get the transform (we should host it !)
        auto cf_it = cumulatedForces_map.getVector()->begin();
        for (auto itr = forces_map.begin(); itr != forces_map.end(); ++itr, ++cf_it) {
            auto transform = ControllableMove::GetTransform(itr->first);
            if (transform != nullptr) {
                glm::vec3 t;
                for (auto forceitr = itr->second.begin(); forceitr != itr->second.end(); ++forceitr) {
                    t += *forceitr;
                }
                *cf_it = (t.z * transform->GetForward()) +
                           (t.y * transform->GetUp()) +
                           (t.x * transform->GetRight());
            }
        }
    }

    void ControllableMove::ApplyForcesToBody(const double delta) {
        auto cf_it = cumulatedForces_map.getVector()->cbegin();
        for (auto kitr = cumulatedForces_map.IteratorKeyBegin(); kitr != cumulatedForces_map.IteratorKeyEnd(); ++kitr, ++cf_it) {
            auto body = RigidBody::getBody(*kitr);
            auto finalForce = cf_it;
            if (body != nullptr) {
                body->setLinearVelocity(btVector3(finalForce->x, body->getLinearVelocity().y() + 0.000000001f, finalForce->z));
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
