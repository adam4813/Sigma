#include "controllers/FPSCamera.h"
#include "components/PhysicsController.h"

namespace Sigma{
	namespace event{
		namespace handler{
			const float FPSCamera::SPEED_TRANSLATE   = 2.0f;
			const float FPSCamera::SPEED_ROTATE      = 20.0f * 3.14159f;
			const float FPSCamera::BOOST_MULTIPLIER  = 2.0f;

			FPSCamera::FPSCamera(PhysicsController& controller) : controller(controller), mouseLook(false) {
				// Clear out the internal key state buffers.
				memset(this->keys, 0, sizeof(this->keys));
				memset(this->keyState, 0, sizeof(this->keyState));

				// set to 1 all keys that should trigger an event in this class
				this->keys['W'] = 1; this->keys['B'] = 1; this->keys['S'] = 1;
				this->keys['A'] = 1; this->keys['D'] = 1;
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
		}
	}
} // namespace Sigma::event::handler
