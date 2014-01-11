#include "components/WorldOrientation.h"


namespace Sigma {
    typedef unsigned int type_id;

    WorldOrientation::~WorldOrientation() {}

    const WeakPtrWrapper<orientation_type> WorldOrientation::at(const id_t entity_id) {
        return orientation_guard.at(entity_id);
    }

    WeakPtrWrapper<orientation_type> WorldOrientation::operator[](const id_t entity_id) {
        if (! orientation_guard.count(entity_id)) {
            // check if we will resize
            if (orientations.size() == orientations.capacity()) {
                // make a copy of the vector with a double size capacity
                std::vector<Sigma::orientation_type> orientations_copy(orientations.size() << 1);
                orientations_copy = orientations;
                // replace the pointers in the map
                auto it = id_vector.cbegin();
                for (Sigma::orientation_type& element : orientations_copy) {
                    orientation_guard.add(*(it++), &element);
                }
                // swap the original and the copy
                std::swap(orientations, orientations_copy);
            }

            // if this is a new entity, create its orientation in the storage vector
            auto my_vec = Sigma::orientation_type(0, 0, 0);
            orientations.emplace_back(my_vec);
            // save id of last object
            id_vector.push_back(entity_id);
            return orientation_guard.add(entity_id, &orientations.back());
        }
        // return a callback containing unique_ptr
        return orientation_guard.at(entity_id);
    }

    void WorldOrientation::RemoveEntityOrientation(const id_t entity_id) {
        orientation_type* addr = orientation_guard.clear(entity_id);
        size_t index = (addr - orientations.data()) / sizeof(Sigma::orientation_type);
        id_t last_id = id_vector.back();
        if (last_id != entity_id) {
                // swap with last element
                *addr = std::move(orientations.back());
                orientation_guard.at(last_id) = *addr;
                id_vector[index] = std::move(last_id);
        }
        // remove last element
        orientations.pop_back();
        id_vector.pop_back();
    }
}
