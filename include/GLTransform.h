#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Sigma.h"

namespace Sigma {
	class GLTransform {
	public:

		const static glm::vec3 FORWARD_VECTOR;
		const static glm::vec3 UP_VECTOR;
		const static glm::vec3 RIGHT_VECTOR;

		GLTransform() : orientation(glm::quat(1,0,0,0)),
						position(glm::vec3(0,0,0)),
						rotation(glm::vec3(0,0,0)),
						scale(glm::vec3(0,0,0)),
						translateMatrix(glm::mat4(1.0f)),
						rotateMatrix(glm::mat4(1.0f)),
						scaleMatrix(glm::mat4(1.0f)),
						Euler(false),
						parentID(0) {}

		void Translate(float x, float y, float z) {
			this->position += glm::vec3(x, y, z);
			this->translateMatrix = glm::translate(glm::mat4(1.0f), this->position);
			this->MMhasChanged = true;
		}

		void TranslateTo(float x, float y, float z) {
			this->position = glm::vec3(x, y, z);
			this->translateMatrix = glm::translate(x, y, z);
			this->MMhasChanged = true;
		}

		// Helper functions.
		void Translate(glm::vec3 trans) {
			Translate(trans.x, trans.y, trans.z);
		}

		void TranslateTo(glm::vec3 trans) {
			TranslateTo(trans.x, trans.y, trans.z);
		}

		void Rotate(float x, float y, float z) {
			this->rotation += glm::vec3(x, y, z);

			if(this->Euler) {
				this->rotateMatrix = glm::rotate(glm::mat4(1.0f), this->rotation.y, UP_VECTOR);
				this->rotateMatrix = glm::rotate(this->rotateMatrix, this->rotation.x, RIGHT_VECTOR);
				this->rotateMatrix = glm::rotate(this->rotateMatrix, this->rotation.z, FORWARD_VECTOR);
			}
			else {
				glm::quat qX = glm::angleAxis(x, RIGHT_VECTOR);
				glm::quat qY = glm::angleAxis(y, UP_VECTOR);
				glm::quat qZ = glm::angleAxis(z, FORWARD_VECTOR);
				glm::quat change = qX * qY * qZ;
				this->orientation = glm::normalize(change * this->orientation);
				this->rotateMatrix = glm::mat4_cast(this->orientation);
			}

			this->MMhasChanged = true;
		}

		void Rotate(glm::vec3 rot) {
			Rotate(rot.x, rot.y, rot.z);
		}

		void Scale(float x, float y, float z) {
			this->scale = this->scale*glm::vec3(x, y, z);
			this->scaleMatrix = glm::scale(this->scaleMatrix, x, y, z);
			this->MMhasChanged = true;
		}

		void Scale(glm::vec3 scale) {
			Scale(scale.x, scale.y, scale.z);
		}

		/*
		 * \brief Helper function to enable moving
		 * \the view based on global orientation vectors
		 *
		 * \param[in] float right
		 * \param[in] float up
		 * \param[in] float forward
		 *
		 * \returns void
		*/
		void Move(float right, float up, float forward) {
			this->Translate((GetForward() * forward) + (GetRight() * right) + (GetUp() * up));
		}

		// NOTE:  These methods only work if the transform's scale is uniform
		glm::vec3 GetRight() {
			glm::mat4 currTransform = this->GetMatrix();
			glm::vec3 right_vector(currTransform[0][0], currTransform[0][1], currTransform[0][2]);
			return glm::normalize(right_vector);
		}

		glm::vec3 GetUp() {
			glm::mat4 currTransform = this->GetMatrix();
			glm::vec3 up_vector(currTransform[1][0], currTransform[1][1], currTransform[1][2]);
			return glm::normalize(up_vector);
		}

		glm::vec3 GetForward() {
			glm::mat4 currTransform = this->GetMatrix();
			glm::vec3 back_vector(currTransform[2][0], currTransform[2][1], currTransform[2][2]);
			return -1.0f * glm::normalize(back_vector);
		}

		float GetPitch() const {
			return this->rotation.x;
		}

		float GetYaw() const {
			return this->rotation.y;
		}

		float GetRoll() const {
			return this->rotation.z;
		}

		// Convenience overload
		void Move(glm::vec3 vec) {
			this->Move(vec.x, vec.y, vec.z);
		}

		const glm::mat4 GetMatrix();

		const glm::mat4 GetMatrixInverse() {
			return glm::inverse(this->GetMatrix());
		}

		const glm::vec3 GetPosition() const {
			return this->position;
		}

		/**
		 * \brief Pulls the position vector directly from the transform matrix
		 *        useful when this matrix is the child of another
		 *
		 * \returns the vec3 position vector
		 */
		glm::vec3 ExtractPosition() {
			glm::mat4 matrix = this->GetMatrix();

			return glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
		}

		/**
		 * \brief Gets the direction from the transform matrix
		 *        This only works if scale is uniform
		 *
		 * \returns the vec3 direction vector
		 */
		glm::vec3 ExtractDirection() {
			glm::mat4 curTransform = this->GetMatrix();
			return -1.0f * glm::normalize(glm::vec3(curTransform[2][0], curTransform[2][1], curTransform[2][2]));
		}

		const glm::quat GetOrientation() const {
			return this->orientation;
		}

		void SetEuler(const bool euler) {
			this->Euler = euler;
		}

		void SetMaxRotation(glm::vec3 maxrot) {
			this->maxRotation = maxrot;
		}


		/**
		 * \brief Restricts the rotation to a defined value.
		 *
		  * If the rotation value passed in is greater than the restriction the restricted value is returned.
		 * \param[in/out] glm::vec3 rotation The attempted rotation.
		 * \return    glm::vec3 The new restricted rotation.
		 */
		glm::vec3 Restrict(glm::vec3 rotation) {
			glm::vec3 rot = rotation;

			if (this->maxRotation.x >= 0) {
				float current_pitch = this->rotation.x;
				float new_pitch = current_pitch + rotation.x;

				if(new_pitch > this->maxRotation.x) {
					rot.x = this->maxRotation.x - current_pitch;
				}
				else if(new_pitch < -this->maxRotation.x) {
					rot.x = -this->maxRotation.x - current_pitch;
				}
			}

			if (this->maxRotation.y > 0) {
				float current_yaw = this->rotation.y;
				float new_yaw = current_yaw + rotation.y;

				if(new_yaw > this->maxRotation.y) {
					rot.y = this->maxRotation.y - current_yaw;
				}
				else if(new_yaw < -this->maxRotation.y) {
					rot.x = -this->maxRotation.y - current_yaw;
				}
			}

			if (this->maxRotation.z > 0) {
				float current_roll = this->rotation.z;
				float new_roll = current_roll + rotation.z;

				if(new_roll > this->maxRotation.z) {
					rot.z = this->maxRotation.z - current_roll;
				}
				else if(new_roll < -this->maxRotation.z) {
					rot.z = -this->maxRotation.z - current_roll;
				}
			}

			return rot;
		}

		void SetParentID(id_t id) { this->parentID = id; }

	private:
		glm::quat orientation;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		glm::mat4 transformMatrix;
		glm::mat4 scaleMatrix;
		glm::mat4 rotateMatrix;
		glm::mat4 translateMatrix;

		glm::vec3 maxRotation;

		id_t parentID;

		bool MMhasChanged; // Set to true if the modelMatrix has changed and needs to be updated
		bool Euler; // Set to true to toggle rotation matrix construction between quaternions and euler angles
	};
}
