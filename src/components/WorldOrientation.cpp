#include "components/WorldOrientation.h"


namespace Sigma {
    typedef unsigned int type_id;

    WorldOrientation::~WorldOrientation() {}

    const std::weak_ptr<const orientation_type> WorldOrientation::operator()(type_id entity_id) const {
        return orientation_guard.Read(entity_id);
    }

    SharedPointerMap<type_id, orientation_type>& WorldOrientation::OrientationWrite(type_id entity_id) {
        if (! orientation_guard.Exist(entity_id)) {
            // check if we will resize
            if (orientations.size() == orientations.capacity()) {
                // make a copy of the vector with a double size capacity
                std::vector<Sigma::orientation_type> orientations_copy(orientations.size() << 1);
                orientations_copy = orientations;
                // replace the pointers in the map
                auto it = id_vector.begin();
                for (Sigma::orientation_type& element : orientations_copy) {
                    orientation_guard.SetElement(*(it++), &element);
                }
                // swap the original and the copy
                std::swap(orientations, orientations_copy);
            }

            // if this is a new entity, create its orientation in the storage vector
            auto my_vec = Sigma::orientation_type(0, 0, 0);
            orientations.emplace_back(my_vec);
            orientation_guard.SetElement(entity_id, &orientations.back());
            // save id of last object
            id_vector.push_back(entity_id);
        }
        // return a callback containing unique_ptr
        return orientation_guard.Write(entity_id);
    }

    void WorldOrientation::RemoveEntityOrientation(type_id entity_id) {
        orientation_type* addr = orientation_guard.Remove(entity_id);
        size_t index = (addr - orientations.data()) / sizeof(Sigma::orientation_type);
        type_id last_id = id_vector.back();
        if (last_id != entity_id) {
                // swap with last element
                *addr = std::move(orientations.back());
                id_vector[index] = std::move(last_id);
                orientation_guard.SetElement(last_id, addr);
        }
        // remove last element
        orientations.pop_back();
        id_vector.pop_back();
    }
}
