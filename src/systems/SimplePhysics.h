#pragma  once

#include <string>
#include <vector>
#include <map>

class Property;
class IMoverComponent;

class SimplePhysics {
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
	 * \brief A factory to create new components of a given type.
	 *
	 * A factory method to create various components and add them to the system. These components will be used during the system update method
	 * \param[in] const std::string type The type of componenet to create
	 * \param[in] const int entityID The ID of the entity this component belongs to.
	 * \param[in] std::vector<Property> &properties A vector containing the properties to apply to the created component.
	 * \returns   void* The newly create component
	 */
	IMoverComponent* Factory(const std::string type, const unsigned int entityID, std::vector<Property> &properties) ;

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
private:
	std::map<int, std::vector<IMoverComponent*> > components; // A mapping of entity ID to a vector containing all of it's components.
};
