#include "components/WebGUIComponent.h"

namespace Sigma {
	void WebGUIView::InjectKeyboardEvent(const unsigned int key, const Sigma::event::KEY_STATE state) {
		Awesomium::WebKeyboardEvent key_event;
		memset(&key_event, 0, sizeof(Awesomium::WebKeyboardEvent));
	
		char* buf = new char[20];
		Awesomium::GetKeyIdentifierFromVirtualKeyCode(Awesomium::KeyCodes::AK_A, &buf);
		memcpy(key_event.key_identifier, buf, 20);
		delete[] buf;

		key_event.native_key_code = key;
		key_event.virtual_key_code = key;

		if (state == Sigma::event::KS_UP) {
			key_event.type = WebKeyboardEvent::kTypeKeyUp;
			this->view->InjectKeyboardEvent(key_event);
		}
		else {
			key_event.type = WebKeyboardEvent::kTypeKeyDown;

			key_event.text[0] = key;
			key_event.unmodified_text[0] = key;

			this->view->InjectKeyboardEvent(key_event);
		}
	}

	bool WebGUIView::InjectMouseMove(float x, float y) {
		if(this->mouseDown == 0) {
			if ((x > this->x) && (x < (this->x + this->width))) {
				if ((y > this->y) && (y < (this->y + this->height))) {
					this->view->Focus();
					this->hasFocus = true;
					this->view->InjectMouseMove((x - this->x) * this->windowWidth, (y - this->y) * this->windowHeight);
					return true;
				}
			}
			this->view->Unfocus();
			this->hasFocus = false;
		}
		else {
			this->view->InjectMouseMove((x - this->x) * this->windowWidth, (y - this->y) * this->windowHeight);
			return true;
		}
		return false;
	}

	bool WebGUIView::InjectMouseDown(const Sigma::event::BUTTON btn, float x, float y) {
		if (btn != Sigma::event::BUTTON::LEFT) { return false; }
		if ((x > this->x) && (x < (this->x + this->width))) {
			if ((y > this->y) && (y < (this->y + this->height))) {
				this->view->Focus();
				this->hasFocus = true;
				this->view->InjectMouseMove((x - this->x) * this->windowWidth, (y - this->y) * this->windowHeight);
				this->view->InjectMouseDown(kMouseButton_Left);
				this->mouseDown = 1;
				//this->view->InjectMouseUp(kMouseButton_Left);
				return true; // Early return to prevent focus loss.
			}
		}
		this->view->Unfocus();
		this->hasFocus = false;
		return false;
	}

	bool WebGUIView::InjectMouseUp(const Sigma::event::BUTTON btn, float x, float y) {
		if (this->hasFocus) {
			this->view->InjectMouseMove((x - this->x) * this->windowWidth, (y - this->y) * this->windowHeight);
			this->view->InjectMouseUp(kMouseButton_Left);
			this->mouseDown = 0;
			return true;
		}
		this->view->Unfocus();
		this->hasFocus = false;
		return false;
	}

	void WebGUIView::InjectCharDown(const unsigned int c) {
		if (this->hasFocus) {
			Awesomium::WebKeyboardEvent key_event;
			memset(&key_event, 0, sizeof(Awesomium::WebKeyboardEvent));

			char* buf = new char[20];
			Awesomium::GetKeyIdentifierFromVirtualKeyCode(Awesomium::KeyCodes::AK_A, &buf);
			memcpy(key_event.key_identifier, buf, 20);
			delete[] buf;

			key_event.native_key_code = c;
			key_event.virtual_key_code = c;

			if (c >= 32) {
				key_event.type = WebKeyboardEvent::kTypeChar;

				key_event.text[0] = c;
				key_event.unmodified_text[0] = c;

				this->view->InjectKeyboardEvent(key_event);
				key_event.type = WebKeyboardEvent::kTypeKeyUp;
				this->view->InjectKeyboardEvent(key_event);
			}
		}
	}
}