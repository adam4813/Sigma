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

	void SetCaputeArea(float x, float y, float width, float height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	void SetWindowSize(unsigned int width, unsigned int heigit) {
		this->windowWidth = width;
		this->windowHeight = heigit;
	}

	void InjectKeyboardEvent(const unsigned int key, const Sigma::event::KEY_STATE state);

	void InjectCharDown(const unsigned int c);

	void InjectMouseDown(const Sigma::event::BUTTON btn, float x, float y);
private:
	WebView* view;
	BitmapSurface* surface;
	Sigma::resource::GLTexture* texture;

	float x, y, width, height; // The region in which to capture mouse inputs.
	unsigned int windowWidth; // The width of the window
	unsigned int windowHeight; // The height of the window
};
