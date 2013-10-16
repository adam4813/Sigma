#pragma once
#ifndef GLFPSCONTROLLER_H
#define GLFPSCONTROLLER_H

#include "../systems/KeyboardInputSystem.h"
#include "../systems/MouseInputSystem.h"
#include "../components/ViewMover.h"
#include "../systems/IGLView.h"

namespace Sigma {
	namespace event {
		namespace handler {
			// A type of handler. This handler controls an OpenGL 6 DOF view.
			class GLFPSController : public IKeyboardEventHandler, public IMouseHandler {
			private:
				GLFPSController() { }
			public:
				GLFPSController(IGLView* view, ViewMover* mover);

				/**
				 * \brief Triggered whenever a key state change event happens
				 *
				 * This method adjusts the view mover according to various key state changes.
				 * \param key The key for which the state change is happening
				 * \param state The new state of the key (KS_UP or KS_DOWN)
				 * \return void
				 */
				void KeyStateChange(const unsigned int key, const KEY_STATE state);
				virtual void MouseMove(float dx, float dy);
				virtual void MouseDown() {}
				virtual void MouseUp() {}
			private:
				ViewMover* mover; // The view mover component that applies the rotations and forces set in the trigger method.
                static const float SPEED_TRANSLATE, SPEED_ROTATE, BOOST_MULTIPLIER;
                glm::vec3 _translate, _rotate;
			};
		}
	}
}

#endif // GLFPSCONTROLLER_H
