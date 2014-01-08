#pragma once

#include <list>

#include "IBulletShape.h"
#include "GLTransform.h"
#include "Sigma.h"

namespace Sigma {
	/**
	 * \brief PhysicsController is a class used to translate and rotate a physics body
	 * and adjust its entity's transform to the new translation and orientation.
	 *
	 */
	class PhysicsController {
	public:
		PhysicsController(IBulletShape& s, GLTransform& t) : shape(s), transform(t) { }
		~PhysicsController();

		/**
		 * \brief Updates the current forces on the BulletShape's RigidBody.
		 *
		 * Calculates the total force and sets the rigid body's linear force.
		 * \param[in] const double delta Change in time since the last call.
		 */
		void UpdateForces(const double delta);

		/**
		 * \brief Updates the transform to match its collision rigid body.
		 *
		 * \return void
		 */
		void UpdateTransform();

		/**
		 * \brief Rotates the transform now.
		 *
		 * \param[in] float x, y, z The amount to rotate.
		 * \return void
		 */
		void Rotate(float x, float y, float z);

		/**
		 * \brief Sets the target rotation for the transform.
		 *
		 * Rotate the transform to the target at a given speed.
		 * \param[in] float x, y, z The target rotation.
		 * \param[in] float speedX, speedY, speedZ The speed at which to rotate per second.
		 * \return void
		 */
		void RotateTo(float x, float y, float z, float speedX, float speedY, float speedZ);

		/**
		 * \brief Translate the transform now..
		 *
		 * \param[in] float x, y, z The new translation.
		 * \return void
		 */
		void Translate(float x, float y, float z);

		/**
		 * \brief Sets the target translation for the transform.
		 *
		 * 
		 * \param[in] float x, y, z The target translation.
		 * \param[in] float speedX, speedY, speedZ The speed at which to translate per second.
		 * \return void
		 */
		void TranslateTo(float x, float y, float z, float speedX, float speedY, float speedZ);

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

		/**
		 * \brief Add a rotation force to the list.
		 *
		 * \param[in] glm::vec3 rotation The rotational force to add.
		 * \return void
		 */
		void AddRotationForce(glm::vec3 rotation) {
			for (auto rotitr = this->rotationForces.begin(); rotitr != this->rotationForces.end(); ++rotitr) {
				if ((*rotitr) == rotation) {
					return;
				}
			}
			this->rotationForces.push_back(rotation);
		}

		/**
		 * \brief Remove a rotation force.
		 *
		 * \param[in] glm::vec3 rotation The rotational force to remove. Is is evaluated based on glm's comparisson.
		 * \return void
		 */
		void RemoveRotationForce(glm::vec3 rotation) {
			this->rotationForces.remove(rotation);
		}

		/**
		 * \brief Removes all forces from the list.
		 * \return void
		 */
		void ClearForces() {
			this->forces.clear();
			this->rotationForces.clear();
		}

	private:
		GLTransform& transform;
		IBulletShape& shape;

		glm::vec3 rotationTarget;
		glm::vec3 rotationSpeed;
		glm::vec3 translationTarget;
		glm::vec3 translationSpeed;

		std::list<glm::vec3> forces; // The list of forces to apply each update loop.
		std::list<glm::vec3> rotationForces;
	};
}
