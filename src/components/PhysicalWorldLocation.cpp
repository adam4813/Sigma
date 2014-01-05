#include "components/PhysicalWorldLocation.h"

namespace Sigma {
	WorldPosition PhysicalWorldLocation::pphysical;
	WorldOrientation PhysicalWorldLocation::ophysical;
	std::shared_ptr<BitArray<unsigned int>> PhysicalWorldLocation::updated_set = BitArray<unsigned int>::Create();
    std::unordered_map<id_t, GLTransform> PhysicalWorldLocation::transform_map;
    std::unordered_map<id_t, std::shared_ptr<GLTransform>> PhysicalWorldLocation::transform_ptr_map;

    void PhysicalWorldLocation::UpdateTransform() {
    	auto it = GetIteratorUpdatedID();
        for (auto it = GetIteratorUpdatedID(); **it < GetUpdatedSize(); ++(*it)) {
            auto position = getPosition(**it);
            auto transform = GetTransform(**it);
            if (transform) {
				transform->TranslateTo(position->x, position->y, position->z);
            }
        }
    }

	void PhysicalWorldLocation::AddEntity(const id_t id, const coordinate_type x, const coordinate_type y,
				   const coordinate_type z, const coordinate_type rx, const coordinate_type ry, const coordinate_type rz) {
		pphysical.PositionWrite_x(id) = x;
		pphysical.PositionWrite_y(id) = y;
		pphysical.PositionWrite_z(id) = z;
		ophysical.OrientationWrite(id) = orientation_type(rx, ry, rz);
		GLTransform transform;
		// Set the view mover's view pointer.
		transform.SetEuler(true);
		transform.SetMaxRotation(glm::vec3(45.0f,0,0));
		transform_map.insert({{id, transform}});
		transform_ptr_map.emplace(id, std::shared_ptr<GLTransform>(&transform_map.at(id)));
		(*updated_set)[id] = true;
		std::cout << "adding entity " << id << " (" << x << ", " << y << ", " << z << ")" << std::endl;
	}

	void PhysicalWorldLocation::setPosition(const id_t id, const position_type& position) {
		pphysical.PositionWrite_x(id) = position.x;
		pphysical.PositionWrite_y(id) = position.y;
		pphysical.PositionWrite_z(id) = position.z;
		(*updated_set)[id] = true;
		std::cout << "Modifying entity " << id << " (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
	}
}
