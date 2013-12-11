#include "components/PhysicalWorldComponent.h"

namespace Sigma {
    WorldPosition PhysicalWorldComponent::pphysical;
    WorldOrientation PhysicalWorldComponent::ophysical;

    void PhysicalWorldComponent::AddObject(type_id id, const coordinate_type x, const coordinate_type y,
                   const coordinate_type z, const coordinate_type rx, const coordinate_type ry, const coordinate_type rz) {
        pphysical.PositionWrite_x(id) = x;
        pphysical.PositionWrite_y(id) = y;
        pphysical.PositionWrite_z(id) = z;
        ophysical.OrientationWrite(id) = orientation_type(rx, ry, rz);
        std::cout << "adding entitiy " << id << " (" << x << ", " << y << ", " << z << ")" << std::endl;
    }
}
