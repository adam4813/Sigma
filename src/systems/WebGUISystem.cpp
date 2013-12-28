#include "systems/WebGUISystem.h"
#include "Property.h"
#include "components/WebGUIComponent.h"
#include "systems/OpenGLSystem.h"

namespace Sigma {
	std::map<std::string,Sigma::IFactory::FactoryFunction> WebGUISystem::getFactoryFunctions() {
		using namespace std::placeholders;
		std::map<std::string,Sigma::IFactory::FactoryFunction> retval;
		retval["WebGUIView"] = std::bind(&WebGUISystem::createWebGUIView,this,_1,_2);

		return retval;
	}

	bool WebGUISystem::Start() {
		std::cout << "Setting up web view." << std::endl;
		// Create the WebCore singleton with default configuration
		WebConfig config;
		config.log_level = Awesomium::LogLevel::kLogLevel_Verbose;
		this->web_core = WebCore::Initialize(config);

		return true;
	}

	bool WebGUISystem::Update(const double delta) {
		this->web_core->Update();
		for (auto eitr = this->_Components.begin(); eitr != this->_Components.end(); ++eitr) {
			for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
				if (citr->second->GetSurface()->is_dirty()) {
					// Update texture here. Not sure of the proper api for it
					// http://www.awesomium.com/docs/1_7_2/cpp_api/class_awesomium_1_1_bitmap_surface.html
					citr->second->UpdateTexture();
				}
			}
		}

		return true;
	}

	IComponent* WebGUISystem::createWebGUIView(const id_t entityID, const std::vector<Property> &properties) {
		float x, y, width, height;
		bool transparent = false;
		std::string textureName = "";
		WebURL url;

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
				url = WebURL(WSLit(p->Get<std::string>().c_str()));
			}
		}
		WebGUIView* webview = new WebGUIView(entityID);
		WebView* view = web_core->CreateWebView(width * this->windowWidth, height * this->windowHeight);

		if (!url.IsValid()) {
			std::cerr << "Invalid URL" << std::endl;
		}
		view->LoadURL(url);
		view->SetTransparent(transparent);

		while (view->IsLoading()) {
			this->web_core->Update();
		}
		// Call one last update just in case
		this->web_core->Update();
		//view->Focus();

		// Get the WebView's rendering Surface. The default Surface is of
		// type 'BitmapSurface', we must cast it before we can use it.
		BitmapSurface* surface = (BitmapSurface*)view->surface();

		// For now point a debug image of the result in the binary directory
		if (surface != 0) {
			surface->SaveToJPEG(WSLit("./result.jpg"));
			std::cout << "Web test successful!" << std::endl;
		} else {
			std::cout << "Web test failure!" << std::endl;
			delete webview;
			return nullptr;
		}

		if (Sigma::OpenGLSystem::textures.find(textureName) == Sigma::OpenGLSystem::textures.end()) {
			Sigma::resource::GLTexture texture;
			texture.Format(GL_BGRA);
			texture.LoadDataFromMemory(surface->buffer(), surface->width(), surface->height());
			Sigma::OpenGLSystem::textures[textureName] = texture;
		}
		else {
			Sigma::OpenGLSystem::textures[textureName].Format(GL_BGRA);
			Sigma::OpenGLSystem::textures[textureName].LoadDataFromMemory(surface->buffer(), surface->width(), surface->height());
		}

		webview->SetCaputeArea(x, y, width, height);
		webview->SetWindowSize(this->windowWidth, this->windowHeight);
		webview->SetWebView(view);
		webview->SetSurface(surface);
		webview->SetTexture(&Sigma::OpenGLSystem::textures[textureName]);
		this->addComponent(entityID, webview);
		return webview;
	}
}
