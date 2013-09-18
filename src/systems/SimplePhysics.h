#pragma  once

#include "../IFactory.h"

#include <string>
#include <vector>
#include <map>

class Property;
class IMoverComponent;

class SimplePhysics
    : public IFactory  {
public:
	SimplePhysics() { }
	~SimplePhysics();
	/**
	 * \brief Starts the Simple Physics system.
	 *
	 * \returns bool Returns false on startup failure.
	 */
	bool Start() { }

	/**
	 * \brief Causes an update in the system based on the change in time.
	 *
	 * Updates the state of the system based off how much time has elapsed since the last update.
	 * \param[in] const float delta The change in time since the last update
	 * \returns bool Returns true if we had an update interval passed.
	 */
	bool Update(const double delta);

	/**
	 * \brief Retrieve the component that belongs to the given entity ID
	 *
	 * \param[in] int entityID
	 * \returns   void* The component that belongs to the entity ID or nullptr if no component exists for the given ID.
	 */
	void* GetComponent(int entityID);

    std::map<std::string,FactoryFunction> getFactoryFunctions();
	void createPhysicsMover(const std::string type, const unsigned int entityID, std::vector<Property> &properties) ;
	void createViewMover(const std::string type, const unsigned int entityID, std::vector<Property> &properties) ;
private:
	std::map<int, std::vector<IMoverComponent*> > components; // A mapping of entity ID to a vector containing all of it's components.
};
