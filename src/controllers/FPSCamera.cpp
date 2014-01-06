#include "controllers/FPSCamera.h"
#include "entities/BulletMover.h"

namespace Sigma{
	namespace event{
		namespace handler{
			const float FPSCamera::SPEED_TRANSLATE   = 2.0f;
			const float FPSCamera::SPEED_ROTATE      = 20.0f * 3.14159f;
			const float FPSCamera::BOOST_MULTIPLIER  = 2.0f;

			FPSCamera::FPSCamera(int entityID) : IGLView(entityID), mouseLook(false) {
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
					translation -= speed*GLTransform::RIGHT_VECTOR;
				}
				if (this->keyState['D'] == KS_DOWN) { // Strafe right
					translation += speed*GLTransform::RIGHT_VECTOR;
				}

				// remove previous force and add new one
				if (this->mover) {
					ControllableMove::RemoveForce(this->mover->GetEntityID(), this->translation);
					this->translation = translation;
					ControllableMove::AddForce(this->mover->GetEntityID(), this->translation);
				}

			} // function KeyStateChange

			void FPSCamera::LostKeyboardFocus() {
				if (this->mover) {
					ControllableMove::ClearForces(this->mover->GetEntityID());
				}
			}

			void FPSCamera::MouseMove(float x, float y, float dx, float dy) {
				if (this->mover && this->mouseLook) {
					// NOTE: dy is positive when the mouse is moved down, so it must be inverted
					//       for some reason, dx needs to be inverted as well, perhaps because
					//       negative z is forward in opengl
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
