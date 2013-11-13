#pragma  once

#include "IFactory.h"
#include "ISystem.h"
#include "components/WebGUIComponent.h"

#include <string>
#include <vector>
#include <map>

#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>

class Property;

using namespace Awesomium;

namespace Sigma {
	class WebGUISystem : public IFactory, public ISystem<WebGUIView> {
	public:
		WebGUISystem() : web_core(nullptr) { }
		~WebGUISystem() { };
		/**
		 * \brief Starts the Awesomium WebGUI system.
		 *
		 * \return bool Returns false on startup failure.
		 */
		bool Start();

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
	private:
		WebCore* web_core;
		unsigned int windowWidth, windowHeight; // The width of the overall window for converting mouse coordinate normals.
	};
}
