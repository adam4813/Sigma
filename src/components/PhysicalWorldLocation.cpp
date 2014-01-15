#include "components/PhysicalWorldLocation.h"
#include "components/SigmaMotionState.h"

namespace Sigma {
	VectorMap<id_t, position_type> PhysicalWorldLocation::pphysical;
	VectorMap<id_t, orientation_type> PhysicalWorldLocation::ophysical;
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

	std::vector<std::unique_ptr<IECSComponent>> PhysicalWorldLocation::AddEntityPosition(const id_t id, coordinate_type x, coordinate_type y,
				   coordinate_type z, const coordinate_type rx, const coordinate_type ry, const coordinate_type rz) {
		pphysical[id] = position_type(x, y, z);
		ophysical[id] = orientation_type(rx, ry, rz);
		GLTransform transform;
		// Set the view mover's view pointer.
		transform.SetEuler(true);
		transform.SetMaxRotation(glm::vec3(45.0f,0,0));
		transform_map.insert(std::make_pair(id, transform));
		transform_ptr_map.emplace(id, std::shared_ptr<GLTransform>(&transform_map.at(id)));
		(*updated_set)[id] = true;
		std::cout << "adding entity " << id << " (" << x << ", " << y << ", " << z << ")" << std::endl;
		return std::vector<std::unique_ptr<IECSComponent>>();
	}

	std::vector<std::unique_ptr<IECSComponent>> PhysicalWorldLocation::AddEntity(const id_t id, const std::vector<Property> &properties) {
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float rx = 0.0f;
		float ry = 0.0f;
		float rz = 0.0f;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &*propitr;

			if (p->GetName() == "x") {
				x = p->Get<float>();
			}
			else if (p->GetName() == "y") {
				y = p->Get<float>();
			}
			else if (p->GetName() == "z") {
				z = p->Get<float>();
			}
			else if (p->GetName() == "rx") {
				rx = p->Get<float>();
			}
			else if (p->GetName() == "ry") {
				ry = p->Get<float>();
			}
			else if (p->GetName() == "rz") {
				rz = p->Get<float>();
			}
		}
		return AddEntityPosition(id, x, y, z, rx, ry, rz);
	}
}
