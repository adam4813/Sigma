#include "components/CanMove.h"

namespace Sigma {
   std::unordered_map<id_t, forces_list> CanMove::forces_map;
   std::unordered_map<id_t, rotationForces_list> CanMove::rotationForces_map;
}
