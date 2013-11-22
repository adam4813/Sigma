#pragma once
#ifndef FPSVIEW_H
#define FPSVIEW_H

#include "systems/KeyboardInputSystem.h"
#include "systems/MouseInputSystem.h"
#include "systems/IGLView.h"

namespace Sigma {
	class BulletMover;

	namespace event {
		namespace handler {
			// A type of handler. This handler controls an OpenGL 6 DOF view.
			class FPSCamera : public IKeyboardEventHandler, public IMouseEventHandler, public IGLView {
			public:
			    SET_COMPONENT_TYPENAME("FPS_CAMERA");

				FPSCamera(int entityID);

				/**
				 * \brief Triggered whenever a key state change event happens
				 *
				 * This method adjusts the view mover according to various key state changes.
				 * \param key The key for which the state change is happening
				 * \param state The new state of the key (KS_UP or KS_DOWN)
				 * \return void
				 */
				void KeyStateChange(const unsigned int key, const KEY_STATE state);

				/**
				 * \brief Handles a change in mouse position.
				 *
				 * \param[in/out] float dx, dy The change in mouse position.
				 * \param[in/out] float dy
				 */
				virtual void MouseMove(float x, float y, float dx, float dy);

				// Not used but required to implement.
				virtual void MouseDown(Sigma::event::BUTTON btn, float x, float y) {}
				virtual void MouseUp(Sigma::event::BUTTON btn, float x, float y) {}

				/**
				 * \brief Updates and returns the view matrix.
				 *
				 * \return const glm::mat4 The current view matrix.
				 */
				const glm::mat4 GetViewMatrix();

				/**
				 * \brief Sets the view mover for this event handler.
				 *
				 * The view mover does the moving.
				 * \param[in/out] ViewMover * m The view mover.
				 * \return    void 
				 */
				virtual void SetMover(BulletMover* m);
			private:
				BulletMover* mover; // The view mover component that applies the rotations and forces set in the trigger method.
                static const float SPEED_TRANSLATE, SPEED_ROTATE, BOOST_MULTIPLIER; // Speed variables
                glm::vec3 translation; // Current translation.
			};
		}
	}
}

#endif // FPSVIEW_H

