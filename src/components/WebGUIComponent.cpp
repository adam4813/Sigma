#include "components/WebGUIComponent.h"
#include "resources/Texture.h"

namespace Sigma {
	void WebGUIView::InjectKeyboardEvent(const unsigned int key, const Sigma::event::KEY_STATE state) {
#ifndef NO_CEF
		if (this->hasFocus) {
			CefKeyEvent key_event;
			memset(&key_event, 0, sizeof(CefKeyEvent));

			key_event.native_key_code = key;

			if (state == Sigma::event::KS_UP) {
				key_event.type = KEYEVENT_KEYUP;
				this->browserHost->SendKeyEvent(key_event);
			}
			else {
				key_event.type = KEYEVENT_KEYDOWN;
				this->browserHost->SendKeyEvent(key_event);
			}
		}
#endif
	}

	bool WebGUIView::InjectMouseMove(float x, float y) {
#ifndef NO_CEF
		if (!this->browserHost) {
			return false;
		}
		CefMouseEvent mouse_event;
		mouse_event.x = static_cast<int>((x - this->x) * this->windowWidth);
		mouse_event.y = static_cast<int>((y - this->y) * this->windowHeight);

		if(this->mouseDown == 0) {
			if ((x > this->x) && (x < (this->x + this->width))) {
				if ((y > this->y) && (y < (this->y + this->height))) {
					this->browserHost->SendFocusEvent(true);
					this->hasFocus = true;
					this->browserHost->SendMouseMoveEvent(mouse_event, false);
					return true;
				}
			}
			this->browserHost->SendFocusEvent(false);
			this->hasFocus = false;
		}
		else {
			this->browserHost->SendMouseMoveEvent(mouse_event, false);
			return true;
		}
#endif
		return false;
	}

	bool WebGUIView::InjectMouseDown(const Sigma::event::BUTTON btn, float x, float y) {
#ifndef NO_CEF
		if (btn != Sigma::event::BUTTON::LEFT) { return false; }
		if ((x > this->x) && (x < (this->x + this->width))) {
			if ((y > this->y) && (y < (this->y + this->height))) {
				CefMouseEvent mouse_event;
				mouse_event.x = static_cast<int>((x - this->x) * this->windowWidth);
				mouse_event.y = static_cast<int>((y - this->y) * this->windowHeight);

				this->browserHost->SendFocusEvent(true);
				this->hasFocus = true;
				this->browserHost->SendMouseClickEvent(mouse_event, MBT_LEFT, false, 1);
				this->mouseDown = 1;
				//this->browserHost->SendMouseClickEvent(mouse_event, MBT_LEFT, true, 1);
				return true; // Early return to prevent focus loss.
			}
		}
		this->browserHost->SendFocusEvent(false);
#endif
		this->hasFocus = false;
		return false;
	}

	bool WebGUIView::InjectMouseUp(const Sigma::event::BUTTON btn, float x, float y) {
#ifndef NO_CEF
		if (this->hasFocus) {
			CefMouseEvent mouse_event;
			mouse_event.x = static_cast<int>((x - this->x) * this->windowWidth);
			mouse_event.y = static_cast<int>((y - this->y) * this->windowHeight);
			this->browserHost->SendMouseClickEvent(mouse_event, btn == Sigma::event::BUTTON::LEFT ? MBT_LEFT : (btn == Sigma::event::BUTTON::MIDDLE ? MBT_MIDDLE : MBT_RIGHT), true, 1);
			this->mouseDown = 0;
			return true;
		}
		this->browserHost->SendFocusEvent(false);
#endif
		this->hasFocus = false;
		return false;
	}

	void WebGUIView::InjectCharDown(const unsigned int c) {
#ifndef NO_CEF
		if (this->hasFocus) {
			CefKeyEvent key_event;
			memset(&key_event, 0, sizeof(CefKeyEvent));

			key_event.native_key_code = c;

			if (c >= 32) {
				key_event.type = KEYEVENT_CHAR;

				key_event.character = c;
				key_event.unmodified_character = c;

				this->browserHost->SendKeyEvent(key_event);
				key_event.type = KEYEVENT_KEYUP;
				this->browserHost->SendKeyEvent(key_event);
			}
		}
#endif
	}
}
