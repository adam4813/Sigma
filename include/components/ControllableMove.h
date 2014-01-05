#ifndef CONTROLLABLEMOVE_H_INCLUDED
#define CONTROLLABLEMOVE_H_INCLUDED

#include <list>
#include <unordered_map>
#include <memory>

#include "glm/glm.hpp"
#include "Sigma.h"
#include "Property.h"
#include "IComponent.h"
#include "GLTransform.h"
#include "VectorMap.hpp"

namespace Sigma {
	typedef std::list<glm::vec3> forces_list; // The list of forces for each entity
	typedef std::list<glm::vec3> rotationForces_list;

	/** \brief A component for entities with movements under external control
	 *
	 * It stores a list of forces to apply.
	 *
	 * A function provides a way to apply the forces to entities
	 *
	 * NB: Currently movements are constrained in the horizontal plan
	 */
	class ControllableMove {
	public:
		SET_STATIC_COMPONENT_TYPENAME("ControllableMove");

		/* All functions are static for convenience purpose (be accesssible from everywhere).
		 * A component must be able to be pure static except special cases.
		 * Making the component pure static is a good practice (I think...)
		 */

		ControllableMove() = delete; // default ctor
		virtual ~ControllableMove() {}; // default dtor
		ControllableMove(ControllableMove& cm) = delete; // copy constructor
		ControllableMove(ControllableMove&& cm) = delete; // move constructor
		ControllableMove& operator=(ControllableMove& cm) = delete; // copy assignment
		ControllableMove& operator=(ControllableMove&& cm) = delete; // move assignment

		// Note that all data have the same lifecycle : created at once, deleted at once
		// If this is not the case, split the component
		static bool AddEntity(const id_t id) {
			if (getForces(id) == nullptr && getRotationForces(id) == nullptr) {
				forces_map.emplace(id, forces_list());
				rotationForces_map.emplace(id, rotationForces_list());
				cumulatedForces_map.set(id) = glm::vec3();
				return true;
			}
			return false;
		}

		static void RemoveEntity(const id_t id) {
			forces_map.erase(id);
			rotationForces_map.erase(id);
			cumulatedForces_map.RemoveElement(id);
		}

		/** \brief Add a force to the list for a specific entity.
		 *
		 * Adds the force to the list. Adding of a duplicate force is checked for and ignored.
		 * \param id const id_t the id of the entity
		 * \param force glm::vec3 The force to add to the list.
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
		 * \brief Apply all forces to the body of all entities that have this component.
		 *
		 * Sets the rigid body's linear force.
		 *
		 * NB: You must call CumulateForces() before to refresh forces
		 */
		static void ApplyForces(const double delta);

		/** \brief Compute the cumulated forces for all entities that have this component
		 *
		 * The cumulated force is stored in a map to be retrieved in order to be applied
		 * to a body, or to be broadcasted on the network
		 *
		 */
		static void CumulateForces();

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
		static VectorMap<id_t, glm::vec3> cumulatedForces_map;
	};
}

#endif // CONTROLLABLEMOVE_H_INCLUDED
