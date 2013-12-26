#pragma  once

#include "IFactory.h"
#include "ISystem.h"
#include "components/WebGUIComponent.h"

#include <string>
#include <vector>
#include <map>

#include "cef_app.h"
#include "cef_client.h"
#include "cef_render_process_handler.h"

class Property;

namespace Sigma {
	class WebGUISystem : public IFactory, public ISystem<WebGUIView>, public CefApp, public CefBrowserProcessHandler, public CefRenderProcessHandler {
	public:
		WebGUISystem() { }
		~WebGUISystem() { };
		/**
		 * \brief Initializes the Chromium Embedded Framework.
		 *
		 * \param[in] CefMainArgs mainArgs Command line arguments, passed from main()
		 * \return bool Returns false on startup failure.
		 */
		bool Start(CefMainArgs& mainArgs);

		void SetWindowSize(unsigned int width, unsigned int height) {
			this->windowWidth = width;
			this->windowHeight = height;
		}

		/**
		 * \brief Causes an update in the system based on the change in time.
		 *
		 * Updates the state of the system based off how much time has elapsed since the last update.
		 * \param[in] const float delta The change in time since the last update
		 * \return bool Returns true if we had an update interval passed.
		 */
		bool Update(const double delta);

		std::map<std::string,FactoryFunction> getFactoryFunctions();

		IComponent* createWebGUIView(const unsigned int entityID, const std::vector<Property> &properties);

		// CefApp
		virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE {
			return this;
		}

		virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE {
			return this;
		}
	private:
		unsigned int windowWidth, windowHeight; // The width of the overall window for converting mouse coordinate normals.

		IMPLEMENT_REFCOUNTING(WebGUISystem);
	};
}
