#include "controllers/GUIController.h"
#include "components/WebGUIComponent.h"


namespace Sigma {
	namespace event {
		namespace handler {
			GUIController::GUIController() : hasFocus(false), gui(nullptr) {
				// Clear out the internal key state buffers.
				memset(this->keys, 1, sizeof(this->keys)); // We respond to every keypress
				memset(this->keyState, 0, sizeof(this->keyState));
				memset(this->chars, 1, sizeof(this->chars)); // We respond to every keypress
			}

			void GUIController::KeyStateChange(const unsigned int key, const KEY_STATE state) {
				// Store the new key state
				if (this->gui) {
					this->gui->InjectKeyboardEvent(key, state);
				}
			}

			void GUIController::CharDown(const unsigned int c) {
				if (this->gui) {
					this->gui->InjectCharDown(c);
				}
			}

			void GUIController::MouseDown(Sigma::event::BUTTON btn, float x, float y) {
				// Store the new key state
				if (this->gui) {
					if (this->gui->InjectMouseDown(btn, x, y)) {
						this->keyboardSystem->RequestFocusLock(this);
						this->mouseSystem->RequestFocusLock(this);
					}
					else {
						this->keyboardSystem->ReleaseFocusLock(this);
						this->mouseSystem->ReleaseFocusLock(this);
					}
				}
			}

			void GUIController::MouseUp(Sigma::event::BUTTON btn, float x, float y) {
				// Store the new key state
				if (this->gui) {
					if (this->gui->InjectMouseUp(btn, x, y)) {
						this->keyboardSystem->RequestFocusLock(this);
						this->mouseSystem->RequestFocusLock(this);
					}
					else {
						this->keyboardSystem->ReleaseFocusLock(this);
						this->mouseSystem->ReleaseFocusLock(this);
					}
				}
			}

			void GUIController::MouseMove(float x, float y, float dx, float dy) {
				// Store the new key state
				if (this->gui) {
					if (this->gui->InjectMouseMove(x, y)) {
						this->keyboardSystem->RequestFocusLock(this);
						this->mouseSystem->RequestFocusLock(this);
					}
					else {
						this->keyboardSystem->ReleaseFocusLock(this);
						this->mouseSystem->ReleaseFocusLock(this);
					}
				}
			}
		}
	}
}
