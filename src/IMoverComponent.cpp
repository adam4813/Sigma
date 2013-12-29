#include "IMoverComponent.h"

namespace Sigma {
    std::unordered_map<id_t, forces_list> IMoverComponent::forces_map; // The list of forces to apply each update loop.
    std::unordered_map<id_t, rotationForces_list> IMoverComponent::rotationForces_map;
    std::unordered_map<id_t, glm::vec3> IMoverComponent::rotationtarget_map;
}
