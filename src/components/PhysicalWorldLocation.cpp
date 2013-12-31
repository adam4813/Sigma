#include "components/PhysicalWorldLocation.h"

namespace Sigma {
    WorldPosition PhysicalWorldLocation::pphysical;
    WorldOrientation PhysicalWorldLocation::ophysical;

    void PhysicalWorldLocation::AddEntity(const id_t id, const coordinate_type x, const coordinate_type y,
                   const coordinate_type z, const coordinate_type rx, const coordinate_type ry, const coordinate_type rz) {
        pphysical.PositionWrite_x(id) = x;
        pphysical.PositionWrite_y(id) = y;
        pphysical.PositionWrite_z(id) = z;
        ophysical.OrientationWrite(id) = orientation_type(rx, ry, rz);
        std::cout << "adding entity " << id << " (" << x << ", " << y << ", " << z << ")" << std::endl;
    }
}
