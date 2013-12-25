#include "controllers/FPSCamera.h"
#include "components/BulletMover.h"

namespace Sigma{
	namespace event{
		namespace handler{
			const float FPSCamera::SPEED_TRANSLATE   = 2.0f;
			const float FPSCamera::SPEED_ROTATE      = 20.0f * 3.14159f;
			const float FPSCamera::BOOST_MULTIPLIER  = 2.0f;

			FPSCamera::FPSCamera(int entityID) : IGLView(entityID), mouseLook(false) {
				// Set the view mover's view pointer.
				this->Transform.SetEuler(true);
				this->Transform.SetMaxRotation(glm::vec3(45.0f,0,0));

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

				// Translation keys
				float fwd = 0.0f, strafe = 0.0f, rise = 0.0f;
				if (this->keyState['W'] == KS_DOWN) { // Move forward
					fwd += speed;
				}
				if (this->keyState['S'] == KS_DOWN) { // Move backward
					fwd -= speed;
				}
				if (this->keyState['A'] == KS_DOWN) { // Strafe left
					strafe -= speed;
				}
				if (this->keyState['D'] == KS_DOWN) { // Strafe right
					strafe += speed;
				}

				// remove previous force and add new one
				if (this->mover) {
					this->mover->RemoveForce(this->translation);
					this->translation = glm::vec3(strafe, rise, fwd);
					this->mover->AddForce(this->translation);
				}

			} // function KeyStateChange

			void FPSCamera::LostKeyboardFocus() {
				if (this->mover) {
					this->mover->ClearForces();
				}
			}

			void FPSCamera::MouseMove(float x, float y, float dx, float dy) {
				if (this->mover && this->mouseLook) {
					this->mover->RotateNow(dy*10.0f,dx*10.0f,0.0f);
				}
			}

			void FPSCamera::MouseDown(BUTTON btn, float x, float y) {
				if (btn == RIGHT) {
					this->mouseLook = true;
				}
			}

			void FPSCamera::MouseUp(BUTTON btn, float x, float y) {
				if (btn == RIGHT) {
					this->mouseLook = false;
				}
			}

			const glm::mat4 FPSCamera::GetViewMatrix() {
				// Limit rotation to pitch and yaw, apply pitch first to ensure
				// yaw rotation happens correctly
				glm::mat4 viewMatrix = glm::rotate(glm::mat4(1.0f), this->Transform.GetPitch(), glm::vec3(1.0f, 0, 0));
				viewMatrix = glm::rotate(viewMatrix, this->Transform.GetYaw(), glm::vec3(0, 1.0f, 0));

				viewMatrix = glm::translate(viewMatrix, -1.0f * this->Transform.GetPosition());

				return viewMatrix;
			}

			void FPSCamera::SetMover(BulletMover* m){
				if (m) {
					m->SetTransform(this->Transform);
					this->mover = m;
				}
			}
		}
	}
} // namespace Sigma::event::handler
