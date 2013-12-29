#pragma once
#ifndef IMOVER_COMPONENT_H
#define IMOVER_COMPONENT_H

#include <list>
#include <iostream>
#include <unordered_map>

#include "IComponent.h"
#include "glm/glm.hpp"
#include "Sigma.h"

namespace Sigma{

    class IMoverComponent: public IComponent {

        typedef std::list<glm::vec3> forces_list; // The list of forces for each entity
        typedef std::list<glm::vec3> rotationForces_list;

    public:
		SET_COMPONENT_TYPENAME("InterpolatedMovement");
//        IMoverComponent() {} // Default ctor.

        // TODO : delete these ctors when inheritance will be destroyed
        IMoverComponent() : IComponent(0) { } // Default ctor setting entity ID to 0.
        IMoverComponent(const int entityID) : IComponent(entityID) { } // Ctor that sets the entity ID.

        virtual ~IMoverComponent() {};

        bool AddEntity(const id_t id) {
            if (getForces(id) == nullptr && getRotationForces(id) == nullptr) {
                forces_map.emplace(id, forces_list());
                rotationForces_map.emplace(id, rotationForces_list());
                return true;
            }
            return false;
        };

        void RemoveEntity(const id_t id) {
            forces_map.erase(id);
            rotationForces_map.erase(id);
        };

        /** \brief Add a force to the list for a specific entity.
         *
         * Adds the force to the list. Adding of a duplicate force is checked for and ignored.
         * \param id const id_t the id of the entity
         * \param force glm::vec3 The force to add to the list.
         * \return void
         *
         */
        void AddForce(const id_t id, glm::vec3 force) {
            auto forces = getForces(id);
            if (forces == nullptr) {
                assert(0 && "id does not exist");
            }
            for (auto forceitr = forces->begin(); forceitr != forces->end(); ++forceitr) {
                if ((*forceitr) == force) {
                    return;
                }
            }
            forces->push_back(force);
        }

        /**
         * \brief Removes a force from the list for a specific id.
         *
         * \param id const id_t the id of the entity
         * \param glm::vec3 force The force to remove. Is is evaluated based on glm's comparisson.
         */
        void RemoveForce(const id_t id, const glm::vec3& force) {
            auto forces = getForces(id);
            if (forces == nullptr) {
                assert(0 && "id does not exist");
            }
            forces->remove(force);
        }

        void AddRotationForce(const id_t id, const glm::vec3 rotation) {
            auto rotationForces = getRotationForces(id);
            if (rotationForces == nullptr) {
                assert(0 && "id does not exist");
            }
            for (auto rotitr = rotationForces->begin(); rotitr != rotationForces->end(); ++rotitr) {
                if ((*rotitr) == rotation) {
                    return;
                }
            }
            rotationForces->push_back(rotation);
        }

        void RemoveRotationForce(const id_t id, glm::vec3 rotation) {
            auto rotationForces = getRotationForces(id);
            if (rotationForces == nullptr) {
                assert(0 && "id does not exist");
            }
            rotationForces->remove(rotation);
        }

        /**
         * \brief Removes all forces from the list.
         *
         * \param id const id_t the id of the entity
         */
        void ClearForces(const id_t id) {
            auto forces = getForces(id);
            if (forces == nullptr) {
                assert(0 && "id does not exist");
            }
            forces->clear();

            auto rotationForces = getRotationForces(id);
            if (rotationForces == nullptr) {
                assert(0 && "id does not exist");
            }
            rotationForces->clear();
        }

        /**
         * \brief Apply all forces in this mover's list.
         *
         * This needs to be overridden by each derived type as they might act on different types of objects.
         * Physics movers apply forces on a transform object.
         * \param id const id_t the id of the entity
         * \param[in] const double delta Change in time since the last call.
         */
        virtual void ApplyForces(const id_t id, const double delta) = 0;

        forces_list* getForces(const id_t id) {
            auto forces = forces_map.find(id);
            if (forces != forces_map.end()) {
                return &forces->second;
            }
            return nullptr;
        }

        rotationForces_list* getRotationForces(const id_t id) {
            auto rotationForces = rotationForces_map.find(id);
            if (rotationForces != rotationForces_map.end()) {
                return &rotationForces->second;
            }
            return nullptr;
        }

    private:
        std::unordered_map<id_t, forces_list> forces_map; // The list of forces to apply each update loop.
        std::unordered_map<id_t, rotationForces_list> rotationForces_map;
    }; // class IMoverComponent
} // namespace Sigma

#endif // IMOVER_COMPONENT_H
