#include "components/ControllableMove.h"
#include "components/PhysicalWorldLocation.h"
#include "components/RigidBody.h"

namespace Sigma {
    std::unordered_map<id_t, forces_list> ControllableMove::forces_map;
    std::unordered_map<id_t, rotationForces_list> ControllableMove::rotationForces_map;
    std::unordered_map<id_t, glm::vec3> ControllableMove::cumulatedForces_map;
    std::unordered_map<id_t, GLTransform*> ControllableMove::transform_map;

    void ControllableMove::UpdateTransform() {
        for (auto it = transform_map.begin(); it != transform_map.end(); it++) {
            auto position = PhysicalWorldLocation::getPosition(it->first);
            if (position != nullptr) {
                it->second->TranslateTo(position->x, position->y, position->z);
            }
        }
    }

    void ControllableMove::CumulateForces() {
        // TODO: optimize the way to get the transform and the cumulated forces
        for (auto itr = forces_map.begin(); itr != forces_map.end(); ++itr) {
            auto transform = ControllableMove::GetTransform(itr->first);
            auto cumulatedForces = ControllableMove::getCumulatedForces(itr->first);
            if (transform != nullptr) {
                glm::vec3 t;
                for (auto forceitr = itr->second.begin(); forceitr != itr->second.end(); ++forceitr) {
                    t += *forceitr;
                }
                *cumulatedForces = (t.z * transform->GetForward()) +
                           (t.y * transform->GetUp()) +
                           (t.x * transform->GetRight());
            }
        }
    }

    void ControllableMove::ApplyForcesToBody() {
        for (auto itr = cumulatedForces_map.begin(); itr != cumulatedForces_map.end(); ++itr) {
            auto body = RigidBody::getBody(itr->first);
            if (body != nullptr) {
                auto finalForce = itr->second;
                body->setLinearVelocity(btVector3(finalForce.x, body->getLinearVelocity().y() + 0.000000001f, finalForce.z));
            }
        }
    }

}
