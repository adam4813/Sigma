#pragma once
#ifndef GLSIXDOFVIEWCONTROLLER_H
#define GLSIXDOFVIEWCONTROLLER_H

#include "systems/KeyboardInputSystem.h"
#include "systems/IGLView.h"
#include "entities/BulletMover.h"

namespace Sigma {
	namespace event {
		namespace handler {
			// A type of handler. This handler controls an OpenGL 6 DOF view.
			class GLSixDOFViewController : public IKeyboardEventHandler {
			private:
				GLSixDOFViewController() { }
			public:
				GLSixDOFViewController(IGLView* view, BulletMover* mover);

				/**
				 * \brief Triggered whenever a key state change event happens
				 *
				 * This method adjusts the view mover according to various key state changes.
				 * \param key The key for which the state change is happening
				 * \param state The new state of the key (KS_UP or KS_DOWN)
				 * \return void
				 */
				void KeyStateChange(const unsigned int key, const KEY_STATE state);
			private:
				BulletMover* mover; // The view mover component that applies the rotations and forces set in the trigger method.
                static const float SPEED_TRANSLATE, SPEED_ROTATE, BOOST_MULTIPLIER;
                glm::vec3 _translate, _rotate;
			};
		}
	}
}

#endif // GLSIXDOFVIEWCONTROLLER_H
