#pragma once

#include "../IComponent.h"
#include "systems/KeyboardInputSystem.h"
#include "systems/MouseInputSystem.h"
#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>
#include "resources/GLTexture.h"

using namespace Awesomium;

class WebGUIComponent : public Sigma::IComponent {
public:
    SET_COMPONENT_TYPENAME("WebGUIComponent");
    WebGUIComponent() : IComponent(0), texture(nullptr) { }
    WebGUIComponent(const int entityID) : IComponent(entityID), texture(nullptr) { };

	void SetTexture(Sigma::resource::GLTexture* texture) {
		this->texture = texture;
	}

	void UpdateTexture() {
		if (this->texture) {
			this->texture->LoadDataFromMemory(this->surface->buffer(), this->surface->width(), this->surface->height());
		}
	}

	void SetWebView(WebView* view) {
		this->view = view;
	}

	void SetSurface(BitmapSurface* surface) {
		this->surface = surface;
	}

	WebView* GetWebView() {
		return this->view;
	}

	BitmapSurface* GetSurface() {
		return this->surface;
	}

	void InjectKeyboardEvent(const unsigned int key, const Sigma::event::KEY_STATE state);

	void InjectCharDown(const unsigned int c);

	void InjectMouseDown(const Sigma::event::BUTTON btn, float x, float y);
private:
	WebView* view;
	BitmapSurface* surface;
	Sigma::resource::GLTexture* texture;
};
