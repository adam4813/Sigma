#ifndef CONTROLLABLEMOVE_H_INCLUDED
#define CONTROLLABLEMOVE_H_INCLUDED

#include <list>
#include <unordered_map>
#include <memory>

#include "glm/glm.hpp"
#include "Sigma.h"
#include "IComponent.h"
#include "GLTransform.h"
#include "VectorMap.hpp"

namespace Sigma {
	typedef std::list<glm::vec3> forces_list; // The list of forces for each entity
	typedef std::list<glm::vec3> rotationForces_list;

	/** \brief A component for entities with movements under external control
	 *
	 * It stores a transformation matrix and a list of forces to apply.
	 *
	 * A function provides a way to apply the forces to the body of the entities
	 *
	 * NB: Movements are constrained in the horizontal plan
	 */
	class ControllableMove : IComponent {
	public:
		SET_COMPONENT_TYPENAME("ControllableMove");

		ControllableMove() {};

		virtual ~ControllableMove() {};

		static bool AddEntity(const id_t id) {
			if (GetTransform(id) == nullptr && getForces(id) == nullptr && getRotationForces(id) == nullptr) {
				forces_map.emplace(id, forces_list());
				rotationForces_map.emplace(id, rotationForces_list());
				cumulatedForces_map.set(id) = glm::vec3();
				GLTransform transform;
				// Set the view mover's view pointer.
				transform.SetEuler(true);
				transform.SetMaxRotation(glm::vec3(45.0f,0,0));
				transform_map.insert({{id, transform}});
				transform_ptr_map.emplace(id, std::shared_ptr<GLTransform>(&transform_map.at(id)));
				return true;
			}
			return false;
		}

		static void RemoveEntity(const id_t id) {
			forces_map.erase(id);
			rotationForces_map.erase(id);
			cumulatedForces_map.RemoveElement(id);
			transform_map.erase(id);
			transform_ptr_map.erase(id);
		}

		/** \brief Update the transform of all entities having this component
		 *
		 * The transform is updated from the model
		 *
		 */
		static void UpdateTransform();

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
		static void ApplyForcesToBody(const double delta);

		/** \brief Compute the cumulated forces for all entities that have this component
		 *
		 * The cumulated force is stored in a map to be retrieved in order to be applied
		 * to a body, or to be broadcasted on the network
		 *
		 */
		static void CumulateForces();

		static std::shared_ptr<GLTransform> GetTransform(const id_t id) {
			auto itt = transform_ptr_map.find(id);
			if (itt != transform_ptr_map.end()) {
				return itt->second;
			}
			return nullptr;
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
		static VectorMap<id_t, glm::vec3> cumulatedForces_map;
		static std::unordered_map<id_t, GLTransform> transform_map;
		// we need this hack to keep a shared_ptr for each transform since SpatialComponent
		// uses its own GLTransform or a shared one, so we must provide a shared_ptr
		// TODO: SpatialComponent should be redefined (what is its purpose ?)
		static std::unordered_map<id_t, std::shared_ptr<GLTransform>> transform_ptr_map;
	};
}

#endif // CONTROLLABLEMOVE_H_INCLUDED
