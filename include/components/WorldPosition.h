#ifndef WORLDPOSITION_H_INCLUDED
#define WORLDPOSITION_H_INCLUDED

// TODO: move this to a general inclusion file
// 1000 km with dm precision at border
#define VFAR_DISTANCE_MAX 1E+6d
// 100 km with cm precision at border
#define FAR_DISTANCE_MAX 1E+5d
// 10 km with mm precision at border
#define NORMAL_DISTANCE_MAX 1E+4d
// 1 km
#define NEAR_DISTANCE_MAX 1E+3d

#include <limits>
// mask to compute absolute value of float with a simple AND
#define FABS_MASK 0x7FFFFFFF
#define DABS_MASK 0x7FFFFFFFFFFFFFFF

#include "SharedPointerMap.hpp"
#include "BitArray.hpp"
#include "Sigma.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>
#include <iterator>

#if defined(__GNUG__)
#include"components/WorldPosition_SIMD_gcc.hpp"
#else
#include"components/WorldPosition_SIMD.hpp"
#endif // GCC

namespace Sigma {
    typedef double coordinate_type;
    typedef std::vector<coordinate_type, AlignedVectorAllocator<coordinate_type>> aligned_vector_type;
    typedef std::shared_ptr<float> coordinate_array;

    /** \brief A position
     */
    struct position_type {
        position_type() : x(0), y(0), z(0) {};
        position_type(coordinate_type x, coordinate_type y, coordinate_type z) : x(x), y(y), z(z) {};

        coordinate_type x;
        coordinate_type y;
        coordinate_type z;

        bool operator==(const position_type& v) const {
            return this->x == v.x && this->y == v.y && this->z == v.z;
        }
    };

    /** \brief An array of positions
     */
    struct position_array {
        position_array() : x(coordinate_array()), y (coordinate_array()), z(coordinate_array()), length(0) {};

        position_array(coordinate_array x, coordinate_array y, coordinate_array z, size_t s) : x(x),
            y(y), z(z), length(s) {};

        virtual ~position_array() {};

        position_array(position_array&& p) {
            x = std::move(p.x);
            y = std::move(p.y);
            z = std::move(p.z);
            length = p.length;
        }

        position_array& operator=(position_array&& p) {
            std::swap(x, p.x);
            std::swap(y, p.y);
            std::swap(z, p.z);
            length = p.length;
            return *this;
        }

        coordinate_array x;
        coordinate_array y;
        coordinate_array z;
        size_t length;
    };

    struct position_ptr {
        position_ptr(const std::weak_ptr<const coordinate_type>& ptr) : ptr(ptr) {};
        virtual ~position_ptr() {};

        operator const std::weak_ptr<const coordinate_type>() const { return ptr; };
        operator const coordinate_type() const { return *ptr.lock(); }

        bool expired() const { return ptr.expired(); };

        std::shared_ptr<const coordinate_type> lock() const { return ptr.lock(); };

        std::weak_ptr<const coordinate_type> ptr;
    };

    /** \brief 3 vectors storing positions in continuous location
     *  with a reference map for fast access and data lifecycle management
     *
     *  The positions are 3 doubles representing the cartesian coordinates.
     *
     */
    class WorldPosition {
    public:
        /** Default constructor */
        WorldPosition(const size_t& n = 256) {
            positions_x.reserve(n);
            positions_y.reserve(n);
            positions_z.reserve(n);
        };

        /** Default destructor */
        virtual ~WorldPosition();

        /** \brief Get the x coordinate for an entity
         *
         * \param entity_id the id of the entity
         * \return a weak pointer on the x coordinate
         *
         */
        const position_ptr x(const id_t entity_id) const;

        /** \brief Get the y coordinate for an entity
         *
         * \param entity_id the id of the entity
         * \return a weak pointer on the y coordinate
         *
         */
        const position_ptr y(const id_t entity_id) const;

        /** \brief Get the z coordinate for an entity
         *
         * \param entity_id the id of the entity
         * \return a weak pointer on the z coordinate
         *
         */
        const position_ptr z(const id_t entity_id) const;

        /** \brief Set a x coordinate by reference
        *
        *
        * \param entity_id id of the entity
        * \return SharedPointerMap<type_id, coordinate_type>& a reference on the coordinate
        *
        */
        SharedPointerMap<id_t, coordinate_type>& PositionWrite_x(const id_t entity_id);

        /** \brief Set a y coordinate by reference
        *
        *
        * \param entity_id id of the entity
        * \return SharedPointerMap<type_id, coordinate_type>& a reference on the coordinate
        *
        */
        SharedPointerMap<id_t, coordinate_type>& PositionWrite_y(const id_t entity_id);

        /** \brief Set a z coordinate by reference
        *
        *
        * \param entity_id id of the entity
        * \return SharedPointerMap<type_id, coordinate_type>& a reference on the coordinate
        *
        */
        SharedPointerMap<id_t, coordinate_type>& PositionWrite_z(const id_t entity_id);

        /** \brief Remove a position
        *
        * \param entity_id id of the entity
        * \return void
        *
        */
        void RemoveEntityPosition(const id_t entity_id);

        /** \brief Get the internal x coordinate vector
        *
        * \return const std::vector<coordinate_type>* the x coordinate vector
        *
        */
        const aligned_vector_type* All_x() { return &positions_x; }

        /** \brief Get the internal y coordinate vector
        *
        * \return const std::vector<coordinate_type>* the y coordinate vector
        *
        */
        const aligned_vector_type* All_y() { return &positions_y; }

        /** \brief Get the internal z coordinate vector
        *
        * \return const std::vector<coordinate_type>* the z coordinate vector
        *
        */
        const aligned_vector_type* All_z() { return &positions_z; }

        /** \brief Get an iterator on the id
         *
         *  The iterator gives the id in the same order as the internal position vector
         *
         * \return std::vector<const id_t>::const_iterator the iterator
         *
         */
        std::vector<id_t>::const_iterator IteratorEntityId() const { return id_vector.cbegin(); }

        /** \brief Get an array of all positions relative to (x, y, z) converted to float
         *
         * This performs a translation of (-x, -y, -z) and a conversion to float.
         *
         * \param x const coordinate_type x coordinate of the center
         * \param y const coordinate_type y coordinate of the center
         * \param z const coordinate_type z coordinate of the center
         * \return position_array a struct containing 3 pointers to arrays
         *
         */
        std::unique_ptr<position_array> RelativeTo(const coordinate_type x, const coordinate_type y, const coordinate_type z);

        /** \brief Get the bitarray of the positions that are in view range
         *
         * \param x const coordinate_type x coordinate of the center
         * \param y const coordinate_type y coordinate of the center
         * \param z const coordinate_type z coordinate of the center
         * \param distance the view range to consider
         * \return a bitarray of positions in the view range specified
         *
         */
        std::shared_ptr<BitArray<unsigned short>> InViewPositions(const coordinate_type x,
                                                  const coordinate_type y,
                                                  const coordinate_type z,
                                                  const coordinate_type distance) const;

    private:
        /** \brief Resize the vector
         *
         * \param v aligned_vector_type& the vector to resize
         * \param SharedPointerMap<id_t,coordinate_type>& gf the map impacted by the resize
         * \return void
         *
         */
        void Resize(aligned_vector_type& v, SharedPointerMap<id_t, coordinate_type>& gf);

        /** \brief Returns a rvalue to write a coordinate
         *
         * \param entity_id type_id the id to write
         * \param SharedPointerMap<type_id, coordinate_type>& gf the array/map to write (x, y or z)
         * \return SharedPointerMap<type_id, coordinate_type>& A rvalue
         *
         */
        SharedPointerMap<id_t, coordinate_type>& PositionWrite(const id_t entity_id, SharedPointerMap<id_t, coordinate_type>& gf);

        id_t FindIdElement(const id_t id) const {
            auto ptr = position_guard_x.Read(id);
            if (! ptr.expired()) {
                auto p = ptr.lock().get();
                return (p - positions_x.data());
            }
            return -1;
        };

        // the maps containing pointers to positions
        SharedPointerMap<id_t, coordinate_type> position_guard_x;
        SharedPointerMap<id_t, coordinate_type> position_guard_y;
        SharedPointerMap<id_t, coordinate_type> position_guard_z;

        // the vectors containing positions
        aligned_vector_type positions_x;
        aligned_vector_type positions_y;
        aligned_vector_type positions_z;

        // the vector of ids of the objects
        std::vector<id_t> id_vector;

        // The translation class we will use (gcc inline/VS intrinsic/...)
        Translate translate;
    };
}

#endif // WORLDPOSITION_H_INCLUDED
