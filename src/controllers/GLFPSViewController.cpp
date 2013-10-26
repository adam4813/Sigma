#include "controllers/GLFPSViewController.h"

namespace Sigma{
	namespace event{
		namespace handler{
			const float GLFPSViewController::SPEED_TRANSLATE   = 2.0f;
			const float GLFPSViewController::SPEED_ROTATE      = 20.0f * 3.14159f;
			const float GLFPSViewController::BOOST_MULTIPLIER  = 2.0f;

			GLFPSViewController::GLFPSViewController(ViewMover* mover) : mover(mover) {
				// Set the view mover's view pointer.
				this->Transform.SetEuler(true);
				this->mover->View(this);

				// Clear out the internal key state buffers.
				memset(this->keys, 0, sizeof(this->keys));
				memset(this->keyState, 0, sizeof(this->keyState));
				// set to 1 all keys that should trigger an event in this class
				this->keys['W'] = 1; this->keys['B'] = 1; this->keys['S'] = 1;
				this->keys['A'] = 1; this->keys['D'] = 1;
			}

			void GLFPSViewController::KeyStateChange(const unsigned int key, const KEY_STATE state) {

				// Store the new key state
				this->keyState[key] = state;

				float speed = this->keyState['B'] == KS_DOWN ? SPEED_TRANSLATE * BOOST_MULTIPLIER : SPEED_TRANSLATE;

				// Translation keys
				float fwd = 0.0f, strafe = 0.0f, rise = 0.0f;
				if (this->keyState['W'] == KS_DOWN) // Move forward
					fwd += speed;
				if (this->keyState['S'] == KS_DOWN) // Move backward
					fwd -= speed;
				if (this->keyState['A'] == KS_DOWN) // Strafe left
					strafe -= speed;
				if (this->keyState['D'] == KS_DOWN) // Strafe right
					strafe += speed;

				// remove previous force and add new one
				this->mover->RemoveForce(this->_translate);
				this->_translate = glm::vec3(strafe, rise, fwd);
				this->mover->AddForce(this->_translate);

			} // function KeyStateChange

			void GLFPSViewController::MouseMove(float dx, float dy) {
				// Inertial motion
				this->mover->RotateTarget(dx,dy,0.0f);
			}

			const glm::mat4 GLFPSViewController::GetViewMatrix() {
				// Limit rotation to pitch and yaw, apply pitch first to ensure
				// yaw rotation happens correctly
				glm::mat4 viewMatrix = glm::rotate(glm::mat4(1.0f), this->Transform.GetPitch(), glm::vec3(1.0f, 0, 0));
				viewMatrix = glm::rotate(viewMatrix, this->Transform.GetYaw(), glm::vec3(0, 1.0f, 0));

				viewMatrix = glm::translate(viewMatrix, -1.0f * this->Transform.GetPosition());

				return viewMatrix;
			}

			// Currently not working
			glm::vec3 GLFPSViewController::Restrict(glm::vec3 rotation) {
				glm::vec3 rot = rotation;

				float current_pitch = this->Transform.GetPitch();
				float new_pitch = current_pitch + rotation.x;

				if(new_pitch > 45.0f) {
					rot.x = 45.0f - current_pitch;
				} else if(new_pitch < -45.0f) {
					rot.x = -45.0f - current_pitch;
				}

				return rot;
			}

			void GLFPSViewController::Move(float right, float up, float forward) {

				// Figure out the movement directions along the xz plane, to allow for proper movement.
				glm::vec3 right_direction = glm::normalize(glm::cross(this->Transform.GetForward(), GLTransform::UP_VECTOR));
				glm::vec3 forward_direction = glm::normalize(glm::cross(GLTransform::UP_VECTOR, right_direction));

				this->Transform.Translate(
					(forward * -forward_direction) +
					(up * GLTransform::UP_VECTOR) +
					(right * -right_direction)
					);
			}
		}
	}
} // namespace Sigma::event::handler
