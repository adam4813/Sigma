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
				this->transform.SetEuler(true);
				this->transform.SetMaxRotation(glm::vec3(45.0f,0,0));

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

				glm::vec3 translation(0.0f, 0.0f, 0.0f);

				// Translation keys
				if (this->keyState['W'] == KS_DOWN) { // Move forward
					translation -= speed*GLTransform::FORWARD_VECTOR;
				}
				if (this->keyState['S'] == KS_DOWN) { // Move backward
					translation += speed*GLTransform::FORWARD_VECTOR;
				}
				if (this->keyState['A'] == KS_DOWN) { // Strafe left
					translation += speed*GLTransform::RIGHT_VECTOR;
				}
				if (this->keyState['D'] == KS_DOWN) { // Strafe right
					translation -= speed*GLTransform::RIGHT_VECTOR;
				}

				// remove previous force and add new one
				if (this->mover) {
					this->mover->RemoveForce(this->translation);
					this->translation = translation;
					this->mover->AddForce(this->translation);
				}

			} // function KeyStateChange

			void FPSCamera::MouseMove(float x, float y, float dx, float dy) {
				if (this->mover && this->mouseLook) {
					this->mover->RotateTarget(dy*SPEED_ROTATE,dx*SPEED_ROTATE,0.0f);
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
				/*glm::mat4 viewMatrix = glm::rotate(glm::mat4(1.0f), this->transform.GetPitch(), GLTransform::RIGHT_VECTOR);
				viewMatrix = glm::rotate(viewMatrix, this->transform.GetYaw(), GLTransform::UP_VECTOR);

				viewMatrix = glm::translate(viewMatrix, -1.0f * this->transform.GetPosition());

				return viewMatrix;*/

				// This is more precise
				return glm::lookAt(this->transform.GetPosition(),
								   this->transform.GetPosition()+this->transform.GetForward(),
								   GLTransform::UP_VECTOR);
			}

			void FPSCamera::SetMover(BulletMover* m){
				if (m) {
					m->SetTransform(*this->Transform());
					this->mover = m;
				}
			}
		}
	}
} // namespace Sigma::event::handler
