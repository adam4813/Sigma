#include "controllers/FPSCamera.h"
#include "components/PhysicsController.h"
#include "entities/BulletMover.h"

namespace Sigma{
	namespace event{
		namespace handler{
			const float FPSCamera::SPEED_TRANSLATE   = 2.0f;
			const float FPSCamera::SPEED_ROTATE      = 20.0f * 3.14159f;
			const float FPSCamera::BOOST_MULTIPLIER  = 2.0f;

			FPSCamera::FPSCamera(PhysicsController& controller) : controller(controller), mouseLook(false) {
				// Clear out the internal key state buffers.
				this->keys.push_back('W');
				this->keys.push_back('A');
				this->keys.push_back('S');
				this->keys.push_back('D');
				this->keys.push_back('B');
			}

			void FPSCamera::KeyStateChange(const unsigned int key, const KEY_STATE state) {
				// Store the new key state
				this->keyState[key] = state;

				float speed = this->keyState['B'] == KS_DOWN ? SPEED_TRANSLATE * BOOST_MULTIPLIER : SPEED_TRANSLATE;

				glm::vec3 newTranslation(0.0f, 0.0f, 0.0f);

				// Translation keys
				if (this->keyState['W'] == KS_DOWN) { // Move forward
					newTranslation -= speed*GLTransform::FORWARD_VECTOR;
				}
				if (this->keyState['S'] == KS_DOWN) { // Move backward
					newTranslation += speed*GLTransform::FORWARD_VECTOR;
				}
				if (this->keyState['A'] == KS_DOWN) { // Strafe left
					newTranslation -= speed*GLTransform::RIGHT_VECTOR;
				}
				if (this->keyState['D'] == KS_DOWN) { // Strafe right
					newTranslation += speed*GLTransform::RIGHT_VECTOR;
				}

				// remove previous force and add new one
				this->controller.RemoveForce(this->translation);
				this->translation = newTranslation;
				this->controller.AddForce(this->translation);
			} // function KeyStateChange

			void FPSCamera::LostKeyboardFocus() {
				this->controller.ClearForces();
			}

			void FPSCamera::MouseMove(float x, float y, float dx, float dy) {
				if (this->mouseLook) {
					// NOTE: dy is positive when the mouse is moved down, so it must be inverted
					//       for some reason, dx needs to be inverted as well, perhaps because
					//       negative z is forward in opengl
					float xRot = dy * SPEED_ROTATE * -1.0f;
					float yRot = dx * SPEED_ROTATE * -1.0f;
					
					this->controller.Rotate(xRot, yRot, 0.0f);
					InterpolatedMovement::RotateTarget(this->mover->GetEntityID(),\
									-1.0f * dy *SPEED_ROTATE,-1.0f * dx * SPEED_ROTATE,0.0f);
				}
			}

			void FPSCamera::MouseDown(BUTTON btn, float x, float y) {
				if (btn == RIGHT) {
					this->mouseLook = !this->mouseLook;
					os->ToggleMouseLock();
				}
			}

			//Does nothing, but has to be here because of IMouseEventHandler
			void FPSCamera::MouseUp(BUTTON btn, float x, float y) {}

			const glm::mat4 FPSCamera::GetViewMatrix() {
				// Limit rotation to pitch and yaw, apply pitch first to ensure
				// yaw rotation happens correctly
				/*glm::mat4 viewMatrix = glm::rotate(glm::mat4(1.0f), this->transform.GetPitch(), GLTransform::RIGHT_VECTOR);
				viewMatrix = glm::rotate(viewMatrix, this->transform.GetYaw(), GLTransform::UP_VECTOR);

				viewMatrix = glm::translate(viewMatrix, -1.0f * this->transform.GetPosition());

				return viewMatrix;*/

				// This is more precise
				glm::mat4 view =  glm::lookAt(this->transform->GetPosition(),
											  this->transform->GetPosition()+this->transform->GetForward(),
											  this->transform->GetUp());

				return view;
				//return this->transform.GetMatrix();
			}

			void FPSCamera::SetMover(BulletMover* m){
				if (m) {
					SetTransform(PhysicalWorldLocation::GetTransform(m->GetEntityID()));
					this->mover = m;
				}
			}
		}
	}
} // namespace Sigma::event::handler
