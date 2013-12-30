#ifndef CANMOVE_H_INCLUDED
#define CANMOVE_H_INCLUDED

#include <list>
#include <unordered_map>
#include <memory>

#include "glm/glm.hpp"
#include "IComponent.h"
#include "GLTransform.h"
#include "components/ControllableMove.h"
#include "components/RigidBody.h"

namespace Sigma{
    typedef std::list<glm::vec3> forces_list; // The list of forces for each entity
    typedef std::list<glm::vec3> rotationForces_list;

    class CanMove : IComponent {
    public:
		SET_COMPONENT_TYPENAME("CanMove");
        CanMove() { } // Default ctor setting entity ID to 0.

        virtual ~CanMove() {};

        static bool AddEntity(const id_t id) {
            if (getForces(id) == nullptr && getRotationForces(id) == nullptr) {
                forces_map.emplace(id, forces_list());
                rotationForces_map.emplace(id, rotationForces_list());
                return true;
            }
            return false;
        };

        static void RemoveEntity(const id_t id) {
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
        static void AddForce(const id_t id, glm::vec3 force) {
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
        static void RemoveForce(const id_t id, const glm::vec3& force) {
            auto forces = getForces(id);
            if (forces == nullptr) {
                assert(0 && "id does not exist");
            }
            forces->remove(force);
        }

        static void AddRotationForce(const id_t id, const glm::vec3 rotation) {
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

        static void RemoveRotationForce(const id_t id, glm::vec3 rotation) {
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
        static void ClearForces(const id_t id) {
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
         * \brief Apply all forces of all entities that have this component.
         *
         * Calculates the total force and sets the rigid body's linear force.
         */
        static void ApplyForces() {
            for (auto itr = forces_map.begin(); itr != forces_map.end(); ++itr) {
                auto transform = ControllableMove::GetTransform(itr->first);
                auto body = RigidBody::getBody(itr->first);
                if (transform != nullptr) {
                    glm::vec3 t;
                    for (auto forceitr = itr->second.begin(); forceitr != itr->second.end(); ++forceitr) {
                        t += *forceitr;
                    }
                    auto finalForce = (t.z * transform->GetForward()) +
                               (t.y * transform->GetUp()) +
                               (t.x * transform->GetRight());

                    body->setLinearVelocity(btVector3(finalForce.x, body->getLinearVelocity().y() + 0.000000001f, finalForce.z));
                }
            }
        }

        static forces_list* getForces(const id_t id) {
            auto forces = forces_map.find(id);
            if (forces != forces_map.end()) {
                return &forces->second;
            }
            return nullptr;
        }

        static rotationForces_list* getRotationForces(const id_t id) {
            auto rotationForces = rotationForces_map.find(id);
            if (rotationForces != rotationForces_map.end()) {
                return &rotationForces->second;
            }
            return nullptr;
        }

    private:
        static std::unordered_map<id_t, forces_list> forces_map; // The list of forces to apply each update loop.
        static std::unordered_map<id_t, rotationForces_list> rotationForces_map;
    }; // class IMoverComponent
} // namespace Sigma


#endif // CANMOVE_H_INCLUDED
