#pragma once
#ifndef IMOVER_COMPONENT_H
#define IMOVER_COMPONENT_H

#include <list>
#include <iostream>

#include "IComponent.h"
#include "glm/glm.hpp"
#include "Sigma.h"

namespace Sigma{

    class IMoverComponent: public IComponent {
    public:
        IMoverComponent() : entity_id(0) {} // Default ctor setting entity ID to 0.
        IMoverComponent(const id_t entityID) : entity_id(entityID) {} // Ctor that sets the entity ID.

        /**
         * \brief Add a force to the list.
         *
         * Adds the force to the list. Adding of a duplicate force is checked for and ignored.
         * \param[in] glm::vec3 force The force to add to the list.
         */
        void AddForce(glm::vec3 force) {
            for (auto forceitr = this->forces.begin(); forceitr != this->forces.end(); ++forceitr) {
                if ((*forceitr) == force) {
                    return;
                }
            }
            this->forces.push_back(force);
        }

        /**
         * \brief Removes a force from the list.
         *
         * \param[in] glm::vec3 force The force to remove. Is is evaluated based on glm's comparisson.
         */
        void RemoveForce(const glm::vec3& force) {
            this->forces.remove(force);
        }

        void AddRotationForce(glm::vec3 rotation) {
            for (auto rotitr = this->rotationForces.begin(); rotitr != this->rotationForces.end(); ++rotitr) {
                if ((*rotitr) == rotation) {
                    return;
                }
            }
            this->rotationForces.push_back(rotation);
        }

        void RemoveRotationForce(glm::vec3 rotation) {
            this->rotationForces.remove(rotation);
        }

        /**
         * \brief Removes all forces from the list.
         */
        void ClearForces() {
            this->forces.clear();
            this->rotationForces.clear();
        }

        /**
         * \brief Apply all forces in this mover's list.
         *
         * This needs to be overridden by each derived type as they might act on different types of objects.
         * Physics movers apply forces on a transform object.
         * \param[in] const double delta Change in time since the last call.
         */
        virtual void ApplyForces(const double delta) = 0;

    protected:
        std::list<glm::vec3> forces; // The list of forces to apply each update loop.
        std::list<glm::vec3> rotationForces;
        const id_t entity_id; // the id of the entity owning this component
    }; // class IMoverComponent
} // namespace Sigma

#endif // IMOVER_COMPONENT_H
