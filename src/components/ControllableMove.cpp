#include "components/ControllableMove.h"

namespace Sigma {
    std::unordered_map<id_t, forces_list> ControllableMove::forces_map;
    std::unordered_map<id_t, rotationForces_list> ControllableMove::rotationForces_map;
    std::unordered_map<id_t, GLTransform*> ControllableMove::transform_map;
}
