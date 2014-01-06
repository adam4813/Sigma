#ifndef WORLDORIENTATION_H
#define WORLDORIENTATION_H

#include "SharedPointerMap.hpp"
#include "Sigma.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>
#include <iterator>


namespace Sigma {
    struct orientation_type {
        orientation_type() : alpha(0), beta(0), gamma(0) {};
        orientation_type(double alpha, double beta, double gamma) : alpha(alpha), beta(beta), gamma(gamma) {};

        double alpha;
        double beta;
        double gamma;

        bool operator==(const orientation_type& v) const {
            return this->alpha == v.alpha && this->beta == v.beta && this->gamma == v.gamma;
        }
    };

    struct orientation_ptr {
        orientation_ptr(const std::weak_ptr<const orientation_type>& ptr) : ptr(ptr) {};
        virtual ~orientation_ptr() {};

        operator const std::weak_ptr<const orientation_type>() const { return ptr; };
        operator const orientation_type() const { return *ptr.lock(); }

        bool operator==(const orientation_ptr& p) const {
            return (*p.lock() == *this->lock());
        }

        bool expired() const { return ptr.expired(); };

        std::shared_ptr<const orientation_type> lock() const { return ptr.lock(); };

        std::weak_ptr<const orientation_type> ptr;
    };


    /** \brief A vector storing orientations in continuous location
     *  with a reference map for fast access and data lifecycle management
     *
     *  The orientations are 3 doubles representing the Euler angles.
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
            const orientation_ptr euler(const id_t entity_id) const;

            /** \brief Set a orientation by reference
            *
            *
            * \param entity_id id of the entity
            * \return SharedPointerMap<type_id, vec3>& a reference on the orientation
            *
            */
            SharedPointerMap<id_t, orientation_type>& OrientationWrite(const id_t entity_id);


            /** \brief Remove an orientation
            *
            * \param entity_id id of the entity
            * \return void
            *
            */
            void RemoveEntityOrientation(const id_t entity_id);

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
            std::vector<id_t>::const_iterator IteratorEntityId() const { return id_vector.cbegin(); }

        private:
            SharedPointerMap<id_t, orientation_type> orientation_guard;

            // the vector containing orientations
            std::vector<orientation_type> orientations;
            // the id of the objects of the vector
            std::vector<id_t> id_vector;
    };
}
#endif // WORLDORIENTATION_H
