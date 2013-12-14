#include "components/WorldPosition.h"

namespace Sigma {
    typedef unsigned int type_id;

    WorldPosition::~WorldPosition() {}

    const position_ptr WorldPosition::x(type_id entity_id) const {
        return position_guard_x.Read(entity_id);
    }

    const position_ptr WorldPosition::y(type_id entity_id) const {
        return position_guard_y.Read(entity_id);
    }

    const position_ptr WorldPosition::z(type_id entity_id) const {
        return position_guard_z.Read(entity_id);
    }

    SharedPointerMap<type_id, coordinate_type>& WorldPosition::PositionWrite_x(type_id entity_id) {
        return PositionWrite(entity_id, position_guard_x);
    }

    SharedPointerMap<type_id, coordinate_type>& WorldPosition::PositionWrite_y(type_id entity_id) {
        return PositionWrite(entity_id, position_guard_y);
    }

    SharedPointerMap<type_id, coordinate_type>& WorldPosition::PositionWrite_z(type_id entity_id) {
        return PositionWrite(entity_id, position_guard_z);
    }

    SharedPointerMap<type_id, coordinate_type>& WorldPosition::PositionWrite(type_id entity_id, SharedPointerMap<type_id, coordinate_type>& gf) {
        if (! gf.Exist(entity_id) && (id_vector.empty() || id_vector.back() != entity_id)) {
            // check if we will resize
            if (positions_x.size() == positions_x.capacity()) {
                Resize(positions_x, position_guard_x);
            }
            if (positions_y.size() == positions_y.capacity()) {
                Resize(positions_y, position_guard_y);
            }
            if (positions_z.size() == positions_z.capacity()) {
                Resize(positions_z, position_guard_z);
            }
            // if this is a new entity, create its position in the storage vector
            Sigma::coordinate_type my_vec = 0;
            positions_x.emplace_back(my_vec);
            position_guard_x.SetElement(entity_id, &positions_x.back());
            positions_y.emplace_back(my_vec);
            position_guard_y.SetElement(entity_id, &positions_y.back());
            positions_z.emplace_back(my_vec);
            position_guard_z.SetElement(entity_id, &positions_z.back());
            // save id of last object
            id_vector.push_back(entity_id);
            translate.IncreaseLength();
        }
        // mark the position as updated
        MarkUpdated(entity_id);
        // return a callback containing unique_ptr
        return gf.Write(entity_id);
    }

    void WorldPosition::Resize(aligned_vector_type& v, SharedPointerMap<type_id, coordinate_type>& gf) {
        // make a copy of the vector with a double size capacity
        aligned_vector_type v_copy(v.size() << 1);
        v_copy = v;
        // replace the pointers in the map
        auto it = id_vector.begin();
        for (Sigma::coordinate_type& element : v_copy) {
            gf.SetElement(*(it++), &element);
        }
        // swap the original and the copy
        std::swap(v, v_copy);
    }

    void WorldPosition::RemoveEntityPosition(type_id entity_id) {
        coordinate_type* addr_x = position_guard_x.Remove(entity_id);
        coordinate_type* addr_y = position_guard_y.Remove(entity_id);
        coordinate_type* addr_z = position_guard_z.Remove(entity_id);
        size_t index = addr_x - positions_x.data();
        type_id last_id = id_vector.back();
        if (last_id != entity_id) {
                // swap with last element
                *addr_x = std::move(positions_x.back());
                position_guard_x.SetElement(last_id, addr_x);
                *addr_y = std::move(positions_y.back());
                position_guard_y.SetElement(last_id, addr_y);
                *addr_z = std::move(positions_z.back());
                position_guard_z.SetElement(last_id, addr_z);
                id_vector[index] = std::move(last_id);
        }
        // remove last element
        positions_x.pop_back();
        positions_y.pop_back();
        positions_z.pop_back();
        id_vector.pop_back();
        translate.DecreaseLength();
    }

    std::unique_ptr<position_array> WorldPosition::RelativeTo(const coordinate_type x, const coordinate_type y,
                                              const coordinate_type z) {
        auto xa = translate.RelativeTo(x, positions_x);
        auto ya = translate.RelativeTo(y, positions_y);
        auto za = translate.RelativeTo(z, positions_z);
        return std::unique_ptr<position_array>(new position_array(std::move(xa), std::move(ya), std::move(za), positions_x.size()));
    }

    std::shared_ptr<BitArray> WorldPosition::InViewPositions(const coordinate_type x,
                                                             const coordinate_type y,
                                                             const coordinate_type z,
                                                             const coordinate_type distance) const {
        auto bitmap = BitArray::Create(positions_x.size(), true);
        translate.InViewPositions(x, positions_x, distance, *bitmap);
        translate.InViewPositions(y, positions_y, distance, *bitmap);
        translate.InViewPositions(z, positions_z, distance, *bitmap);
        return bitmap;
    }

}

