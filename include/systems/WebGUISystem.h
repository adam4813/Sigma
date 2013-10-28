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

class WebGUISystem
    : public Sigma::IFactory, public Sigma::ISystem<WebGUIComponent> {
public:
	WebGUISystem() { }
	~WebGUISystem() { };
	/**
	 * \brief Starts the Awesomium WebGUI system.
	 *
	 * \return bool Returns false on startup failure.
	 */
	bool Start();

	/**
	 * \brief Causes an update in the system based on the change in time.
	 *
	 * Updates the state of the system based off how much time has elapsed since the last update.
	 * \param[in] const float delta The change in time since the last update
	 * \return bool Returns true if we had an update interval passed.
	 */
	bool Update(const double delta);

    std::map<std::string,FactoryFunction> getFactoryFunctions();

	void createWebView(const std::string type, const unsigned int entityID, std::vector<Property> &properties);
	void createWebView(const std::string type, const unsigned int entityID, std::vector<Property> &properties, const char* targeturl);
	void setWebView(const unsigned int entityID, const std::string url);
private:
	WebCore* web_core;
};
