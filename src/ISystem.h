#pragma once

#include <map>
#include <vector>

class IComponent;

/**
  * \brief A class to contain a generic property.
  *
  * This class is used to pass around generic properties.
  * Properties have a name and a value. The value is stored in
  * vholder and is accessed by calling Get() with the 
  * appropriate type. Values can be called by calling Set()
  * with the correct type, or this can be inferred implicitly.
  */
class Property {
private:
	Property() {}
public:
	Property(const Property &other) {
		this->name = other.name;
		this->vholder = other.vholder;
		const_cast<Property&>(other).vholder = nullptr;
	}
	Property(std::string name) : name(name), vholder(nullptr) {}
	~Property() { delete this->vholder; }

	/**
	 * \brief Retrieves the value in vholder.
	 *
	 * Calls the Get() method of ValueHolder with the given template type.
	 * \returns   t The value with the given template type.
	 */
	template <typename t>
	t Get() { return static_cast<ValueHolder<t>*>(this->vholder)->Get(); }

	/**
	 * \brief Sets the value of vholder
	 *
	 * Checks if vholder is nullptr and deletes it if it isn't. Then it creates a new ValueHolder for the given template type, and then calls Set() with the supplied value.
	 * \param[out] t value The value to be stored.
	 * \returns   void
	 */
	template <typename t>
	void Set(t value) {
		if (this->vholder != nullptr) {
			delete this->vholder;
		}

		ValueHolder<t>* v = new ValueHolder<t>();
		v->Set(value);
		this->vholder = v;
	}

	/**
	 * \brief Gets the name of this property.
	 *
	 * \returns   std::string The name of this property.
	 */
	std::string GetName() {
		return this->name;
	}
private:
	template <typename t>
	class ValueHolder {
	public:
		ValueHolder() {}
		void Set(t value)  { this->value = value; }
		t Get() { return this->value; }
	private:
		t value;
	};

	std::string name;
	void* vholder;
};

class ISystem {
public:
	/**
	 * \brief A factory to create new components of a given type.
	 *
	 * A factory method to create various components and add them to the system. These components will be used during the system update method
	 * \param[in] const std::string type The type of componenet to create
	 * \param[in] const int entityID The ID of the entity this component belongs to.
	 * \param[in] std::vector<Property> &properties A vector containing the properties to apply to the created component.
	 * \returns   IComponent* The newly create component
	 * \exception  
	 */
	//virtual IComponent* Factory(const std::string type, const unsigned int entityID, std::vector<Property> &properties) = 0;

	/**
	 * \brief Causes an update in the system based on the change in time.
	 *
	 * Updates the state of the system based off how much time has elapsed since the last update.
	 * \param[in] const float delta The change in time since the last update
	 * \returns bool Returns true if we had an update interval passed. 
	 * \exception  
	 */
	//virtual bool Update(const double delta) = 0;

	/**
	 * \brief Retrieve the component that belongs to the given entity ID
	 * 
	 * \param[in/out] int entityID
	 * \returns   IComponent* The component that belongs to the entity ID or nullptr if no component exists for the given ID.
	 * \exception  
	 */
	//virtual IComponent* GetComponent(int entityID) = 0;
protected:
	std::map<int, std::vector<IComponent*>> components; // A mapping of entity ID to a vector containing all of it's components.
};