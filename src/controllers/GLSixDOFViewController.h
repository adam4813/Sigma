#include "../systems/KeyboardInputSystem.h"
#include "../components/ViewMover.h"
#include "../systems/IGLView.h"

namespace Sigma {
	namespace event {
		namespace handler {
			// A type of handler. This handler controls an OpenGL 6 DOF view.
			class GLSixDOFViewController : public IKeyboardEventHandler {
			private:
				GLSixDOFViewController() { }
			public:
				GLSixDOFViewController(IGLView* view, ViewMover* mover);

				/**
				 * \brief Triggered whenever a key state change event happens
				 *
				 * This method adjusts the view mover according to various key state changes.
				 * \param[in] const unsigned int key The key for which the state change is happening
				 * \param[in] const KEY_STATE state // The new state of the key.
				 * \return   void
				 * \exception
				 */
				void KeyStateChange(const unsigned int key, const KEY_STATE state);
			private:
				ViewMover* mover; // The view mover component that applies the rotations and forces set in the trigger method.
			};
		}
	}
}
