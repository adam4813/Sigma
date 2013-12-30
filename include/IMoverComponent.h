#pragma once
#ifndef IMOVER_COMPONENT_H
#define IMOVER_COMPONENT_H

#include <list>
#include <iostream>
#include <unordered_map>
#include <memory>

#include "IComponent.h"
#include "glm/glm.hpp"
#include "Sigma.h"
#include "GLTransform.h"
#include "components/ControllableMove.h"

namespace Sigma{
    typedef std::list<glm::vec3> forces_list; // The list of forces for each entity
    typedef std::list<glm::vec3> rotationForces_list;

    class IMoverComponent : IComponent {
    public:
		SET_COMPONENT_TYPENAME("InterpolatedMovement");
        IMoverComponent() { } // Default ctor setting entity ID to 0.

        virtual ~IMoverComponent() {};

        static bool AddEntity(const id_t id) {
            if (getForces(id) == nullptr && getRotationForces(id) == nullptr) {
                forces_map.emplace(id, forces_list());
                rotationForces_map.emplace(id, rotationForces_list());
                rotationtarget_map.emplace(id, glm::vec3());
                return true;
            }
            return false;
        };

        static void RemoveEntity(const id_t id) {
            forces_map.erase(id);
            rotationForces_map.erase(id);
            rotationtarget_map.erase(id);
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

        /** \brief Compute interpolated forces.
         *
         * This function will compute the interpolated position of all entities that have this component
         * It will update the transformation matrix of the ControllableMove component
         *
         * \param delta const double Change in time since the last call.
         */
        static void ComputeInterpolatedForces(const double delta) {
            glm::vec3 deltavec(delta);
            for (auto it = rotationForces_map.begin(); it != rotationForces_map.end(); it++) {
                auto transform = ControllableMove::GetTransform(it->first);
                if (transform != nullptr) {

                    // TODO : use the id parameter
                    auto rotationForces = it->second;
                    for (auto rotitr = rotationForces.begin(); rotitr != rotationForces.end(); ++rotitr) {
                        transform->Rotate((*rotitr) * deltavec);
                    }
                    it->second.clear();
                }
            }

                // Inertial rotation
            for (auto it2 = rotationtarget_map.begin(); it2 != rotationtarget_map.end(); it2++) {
                auto transform = ControllableMove::GetTransform(it2->first);
                if (transform != nullptr) {
                    glm::vec3 targetrvel;
                    auto _rotationtarget = it2->second;

                    targetrvel = _rotationtarget * deltavec;
                    if(fabs(targetrvel.x) > 0.0001f || fabs(targetrvel.y) > 0.0001f || fabs(targetrvel.z) > 0.0001f) {
                        targetrvel = transform->Restrict(targetrvel);
                        transform->Rotate(targetrvel);
                        _rotationtarget -= targetrvel;
                        it2->second = _rotationtarget;
                    }
                }
            }
        };

        static void RotateTarget(const id_t id, float x, float y, float z) {
            auto _rotationtarget = getRotationTarget(id);
            if (_rotationtarget == nullptr) {
                assert(0 && "id does not exist");
            }
            *_rotationtarget += glm::vec3(x,y,z);
        }

        static std::unique_ptr<glm::vec3> GetTransformedForces(const id_t id, GLTransform* transform) {
            auto forces = getForces(id);
			if (forces == nullptr) {
                    assert(0 && "id does not exist");
			}

            glm::vec3 t;
			for (auto forceitr = forces->begin(); forceitr != forces->end(); ++forceitr) {
				t += *forceitr;
			}

	        auto finalForce = new glm::vec3( (t.z * transform->GetForward()) +
                               (t.y * transform->GetUp()) +
                               (t.x * transform->GetRight()) );

            return std::unique_ptr<glm::vec3>(finalForce);
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

        static glm::vec3* getRotationTarget(const id_t id) {
            auto rts = rotationtarget_map.find(id);
            if (rts != rotationtarget_map.end()) {
                return &rts->second;
            }
            return nullptr;
        }

    private:
        static std::unordered_map<id_t, forces_list> forces_map; // The list of forces to apply each update loop.
        static std::unordered_map<id_t, rotationForces_list> rotationForces_map;
        static std::unordered_map<id_t, glm::vec3> rotationtarget_map;
    }; // class IMoverComponent
} // namespace Sigma

#endif // IMOVER_COMPONENT_H
