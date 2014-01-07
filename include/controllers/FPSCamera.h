#pragma once
#ifndef FPSVIEW_H
#define FPSVIEW_H

#include <glm/glm.hpp>

#include "systems/KeyboardInputSystem.h"
#include "systems/MouseInputSystem.h"

#include "OS.h"

namespace Sigma {
	class PhysicsController;

	namespace event {
		namespace handler {
			class FPSCamera : public IKeyboardEventHandler, public IMouseEventHandler {
			public:
				FPSCamera(PhysicsController& controller);

				/**
				 * \brief Triggered whenever a key state change event happens
				 *
				 * This method adjusts the view controller according to various key state changes.
				 * \param key The key for which the state change is happening
				 * \param state The new state of the key (KS_UP or KS_DOWN)
				 * \return void
				 */
				void KeyStateChange(const unsigned int key, const KEY_STATE state);

				/**
				 * \brief Called when focus for this controller has been lost.
				 *
				 * \return void
				 */
				void LostKeyboardFocus();

				/**
				 * \brief Handles a change in mouse position.
				 *
				 * \param[in] float dx, dy The change in mouse position.
				 * \param[in] float dy
				 */
				virtual void MouseMove(float x, float y, float dx, float dy);

				/**
				 * \brief Enables mouse look when the right mouse button is clicked.
				 *
				 * \param[in] BUTTON btn The button that was clicked.
				 * \param[in] float x, y Position of the cursor when the click occurred.
				 * \return void
				 */
				virtual void MouseDown(BUTTON btn, float x, float y);

				/**
				 * \brief Currently does nothing.
				 *
				 * \param[in] BUTTON btn The button that was released.
				 * \param[in] float x, y Position of the cursor when the release occurred.
				 * \return void
				 */
				virtual void MouseUp(BUTTON btn, float x, float y);
				
				OS* os; //Very ugly, but there is no good way to do it right now
			private:
				PhysicsController& controller; 
				static const float SPEED_TRANSLATE, SPEED_ROTATE, BOOST_MULTIPLIER; // Speed variables
				glm::vec3 translation; // Current translation.
				bool mouseLook;
			};
		}
	}
}
#endif // FPSVIEW_H
