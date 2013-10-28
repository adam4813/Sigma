#include "systems/WebGUISystem.h"
#include "Property.h"
#include "components/WebGUIComponent.h"
#include "systems/OpenGLSystem.h"

// Various macro definitions
//Override later
#define WIDTH   1024
#define HEIGHT  786

std::map<std::string,Sigma::IFactory::FactoryFunction>
    WebGUISystem::getFactoryFunctions()
{
    using namespace std::placeholders;
    std::map<std::string,Sigma::IFactory::FactoryFunction> retval;
	//retval["PhysicsMover"] = std::bind(&SimplePhysics::createPhysicsMover,this,_1,_2,_3);
    //retval["ViewMover"] = std::bind(&SimplePhysics::createViewMover,this,_1,_2,_3);

	// Not supported in VS2012
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
			if (citr->second->GetSurface()->is_dirty()){
                // Update texture here. Not sure of the proper api for it
                // http://www.awesomium.com/docs/1_7_2/cpp_api/class_awesomium_1_1_bitmap_surface.html
				citr->second->UpdateTexture();
			}
		}
	}

	return true;
}

void WebGUISystem::createWebView(const std::string type, const unsigned int entityID, std::vector<Property> &properties) {
    //WebGUIComponent* webview = new WebGUIComponent(entityID);
	//this->addComponent(entityID, webview);
}

void WebGUISystem::createWebView(const std::string type, const unsigned int entityID, std::vector<Property> &properties, const char* targeturl) {
    // This method should be removed as fast as possible and set web view used instead
    WebGUIComponent* webview = new WebGUIComponent(entityID);
    // Load a certain URL into our WebView instance
    WebView* view = web_core->CreateWebView(WIDTH, HEIGHT);
    WebURL url(WSLit(targeturl));
	if (!url.IsValid()) {
		std::cerr << "Invalid URL" << std::endl;
	}
    view->LoadURL(url);
    // Wait for our WebView to finish loading
    // TODO: Move to Update()? Change so loading pages dont block
    while (view->IsLoading())
        this->web_core->Update();
    this->web_core->Update();
	view->Focus();

    // Get the WebView's rendering Surface. The default Surface is of
    // type 'BitmapSurface', we must cast it before we can use it.
    BitmapSurface* surface = (BitmapSurface*)view->surface();

    // Make sure our surface is not NULL-- it may be NULL if the WebView
    // process has crashed.
    if (surface != 0) {
      // Save our BitmapSurface to a JPEG image in the current
      // working directory.
        surface->SaveToJPEG(WSLit("./result.jpg"));
        std::cout << "Web test successful!" << std::endl;
    } else {
        std::cout << "Web test failure!" << std::endl;
	}

	if (Sigma::OpenGLSystem::textures.find("gui") == Sigma::OpenGLSystem::textures.end()) {
		Sigma::resource::GLTexture texture;
		texture.LoadDataFromMemory(surface->buffer(), surface->width(), surface->height());
		Sigma::OpenGLSystem::textures["gui"] = texture;
	}

    webview->SetWebView(view);
    webview->SetSurface(surface);
    webview->SetTexture(&Sigma::OpenGLSystem::textures["gui"]);
	this->addComponent(entityID, webview);
}

void WebGUISystem::setWebView(const unsigned int entityID, const std::string url){
    // Need a way to grab a component to change its url
    // At a later time also need a way to set width and height as well
}


