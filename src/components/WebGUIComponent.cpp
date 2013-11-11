#include "components/WebGUIComponent.h"

void WebGUIComponent::InjectKeyboardEvent(const unsigned int key, const Sigma::event::KEY_STATE state) {
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

void WebGUIComponent::InjectMouseDown(const Sigma::event::BUTTON btn, float x, float y) {
	if ((x > this->x) && (x < (this->x + this->width))) {
		if ((y > this->y) && (y < (this->y + this->height))) {
			this->view->InjectMouseMove((x - this->x) * this->windowWidth, (y - this->y) * this->windowHeight);
			this->view->InjectMouseDown(kMouseButton_Left);
			this->view->InjectMouseUp(kMouseButton_Left);
		}
	}
}

void WebGUIComponent::InjectCharDown(const unsigned int c) {
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
