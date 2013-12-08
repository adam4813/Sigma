#ifndef WORLDORIENTATION_H
#define WORLDORIENTATION_H

#include "components/SharedPointerMap.hpp"
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>
#include <iterator>
#include "gtest/gtest_prod.h"
#include <cstdio>


namespace Sigma {
    typedef unsigned int type_id;

    struct orientation_type {
        orientation_type() : alpha(0), beta(0), gamma(0) {};
        orientation_type(float alpha, float beta, float gamma) : alpha(alpha), beta(beta), gamma(gamma) {};

        float alpha;
        float beta;
        float gamma;

        bool operator==(const orientation_type& v) const {
            return this->alpha == v.alpha && this->beta == v.beta && this->gamma == v.gamma;
        }
    };

    /** \brief A vector storing orientations in continuous location
     *  with a reference map for fast access and data lifecycle management
     *
     *  The orientations are 3 floats representing the Euler angles.
     *
     *  operator() and OrientationWrite() methods access to the map, while
     *  AllOrientations gives a pointer to the vector.
     *
     *  The order of elements in the vector is given by the iterator on id's
     *
     *  All modifications are reflected if the vector references are
     *  not invalidated externally (i.e. don't add elements to vectors directly).
     */
    class WorldOrientation {
         public:


            /** Default constructor */
            WorldOrientation() {};

            /** Default destructor */
            virtual ~WorldOrientation();

            /** \brief Get a orientation for an entity
             *
             * \param entity_id the id of the entity
             * \return a weak pointer on the orientation
             *
             */
            const std::weak_ptr<const orientation_type> operator()(type_id entity_id) const;

            /** \brief Set a orientation by reference
            *
            *
            * \param entity_id type_id id of the entity
            * \return SharedPointerMap<type_id, vec3>& a reference on the 
orientation
            *
            */
            SharedPointerMap<type_id, orientation_type>& OrientationWrite(type_id entity_id);


            /** \brief Remove an orientation
            *
            * \param entity_id type_id id of the entity
            * \return void
            *
            */
            void RemoveEntityOrientation(type_id entity_id);

            /** \brief Get the internal orientations vector
            *
            * \return const std::vector<Sigma::vec3>* the orientation vector
            *
            */
            const std::vector<orientation_type>* AllOrientations() { return &orientations; }

            /** \brief Get an iterator on the id
             *
             *  The iterator gives the id in the same order as the internal orientation vector
             *
             * \return std::vector<type_id>::const_iterator the iterator
             *
             */
            std::vector<type_id>::const_iterator IteratorEntityId() const noexcept { return id_vector.cbegin(); }

        private:
            SharedPointerMap<type_id, orientation_type> orientation_guard;

            // the vector containing orientations
            std::vector<orientation_type> orientations;
            // the id of the objects of the vector
            std::vector<type_id> id_vector;
    };
}
#endif // WORLDORIENTATION_H
