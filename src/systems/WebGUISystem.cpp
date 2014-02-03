#include "systems/WebGUISystem.h"
#include "Property.h"
#include "components/WebGUIComponent.h"
#include "systems/OpenGLSystem.h"

#ifndef NO_CEF
#include "cef_url.h"
#endif

#include "Sigma.h"

namespace Sigma {

	// We need ctor and dstor to be exported to a dll even if they don't do anything
	// this avoids needing to export getFactoryFunctions() which is only used by Sigma
	WebGUISystem::WebGUISystem() { }
	WebGUISystem::~WebGUISystem() { }

	std::map<std::string,Sigma::IFactory::FactoryFunction> WebGUISystem::getFactoryFunctions() {
		using namespace std::placeholders;
		std::map<std::string,Sigma::IFactory::FactoryFunction> retval;
		retval["WebGUIView"] = std::bind(&WebGUISystem::createWebGUIView,this,_1,_2);

		return retval;
	}

#ifndef NO_CEF
	bool WebGUISystem::Start(CefMainArgs& mainArgs) {
		LOG << "Setting up web view.";
		CefRefPtr<WebGUISystem> ourselves(this);
		CefSettings settings;
#ifdef CEFDEV
		CefInitialize(mainArgs, settings, ourselves.get(), nullptr);
#else
		CefInitialize(mainArgs, settings, ourselves.get());
#endif
		return true;
	}
#endif
	bool WebGUISystem::Update(const double delta) {
#ifndef NO_CEF
		CefDoMessageLoopWork();
#endif
		return true;
	}

	IComponent* WebGUISystem::createWebGUIView(const id_t entityID, const std::vector<Property> &properties) {
		float x, y, width, height;
		bool transparent = false;
		std::string textureName = "";
		std::string url;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &(*propitr);

			if (p->GetName() == "left") {
				x = p->Get<float>();
			}
			else if (p->GetName() == "top") {
				y = p->Get<float>();
			}
			else if (p->GetName() == "width") {
				width = p->Get<float>();
			}
			else if (p->GetName() == "height") {
				height = p->Get<float>();
			}
			else if (p->GetName() == "textureName") {
				textureName = p->Get<std::string>();
			}
			else if (p->GetName() == "transparent") {
				transparent = p->Get<bool>();
			}
			else if (p->GetName() == "URL") {
				url = p->Get<std::string>();
			}
		}
		WebGUIView* webview = new WebGUIView(entityID);
#ifndef NO_CEF
		CefString cefurl(url);
		CefURLParts parts;
		if (!CefParseURL(cefurl, parts)) {
			LOG_WARN << "Invalid URL";
		}
#endif

		// Check if the texture is loaded and load it if not.
		if (Sigma::OpenGLSystem::textures.find(textureName) == Sigma::OpenGLSystem::textures.end()) {
			Sigma::resource::GLTexture texture;
			Sigma::OpenGLSystem::textures[textureName] = texture;
			Sigma::OpenGLSystem::textures[textureName].Format(GL_BGRA);
			Sigma::OpenGLSystem::textures[textureName].GenerateGLTexture(this->windowWidth, this->windowHeight);
		}

		// It should be loaded, but in case an error occurred double check for it.
		if (Sigma::OpenGLSystem::textures.find(textureName) != Sigma::OpenGLSystem::textures.end()) {
			webview->SetTexture(&Sigma::OpenGLSystem::textures[textureName]);
		}

		webview->SetCaputeArea(x, y, width, height);
		webview->SetWindowSize(this->windowWidth, this->windowHeight);
		this->addComponent(entityID, webview);

#ifndef NO_CEF
		CefWindowInfo windowInfo;
		windowInfo.SetAsOffScreen(nullptr);
		windowInfo.SetTransparentPainting(transparent);

		CefBrowserSettings settings;
		CefRefPtr<Sigma::WebGUIView> client(webview);
		CefBrowserHost::CreateBrowser(windowInfo, client.get(), url, settings, nullptr);
#endif
		return webview;
	}
}
