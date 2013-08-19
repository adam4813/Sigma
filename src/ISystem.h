#pragma once

#include <map>
#include <vector>

class IComponent;

template <typename t>
class ValueHolder {
public:
	ValueHolder() {}
	void Set(t value)  { this->value = value; }
	t Get() { return this->value; }
private:
	t value;
};

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

	template <typename t>
	t Get() { return static_cast<ValueHolder<t>*>(this->vholder)->Get(); }

	template <typename t>
	void Set(t value) {
		if (this->vholder != nullptr) {
			delete this->vholder;
		}

		ValueHolder<t>* v = new ValueHolder<t>();
		v->Set(value);
		this->vholder = v;
	}

	std::string GetName() {
		return this->name;
	}
private:
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
	 * \returns   IComponent* The newly create component
	 * \exception  
	 */
	virtual IComponent* Factory(const std::string type, const unsigned int entityID, std::vector<Property> &properties) = 0;

	/**
	 * \brief Causes an update in the system based on the change in time.
	 *
	 * Updates the state of the system based off how much time has elapsed since the last update.
	 * \param[in] const float delta The change in time since the last update
	 * \returns   void 
	 * \exception  
	 */
	virtual void Update(const double delta) = 0;

	/**
	 * \brief Retrieve the component that belongs to the given entity ID
	 * 
	 * \param[in/out] int entityID
	 * \returns   IComponent* The component that belongs to the entity ID or nullptr if no component exists for the given ID.
	 * \exception  
	 */
	virtual IComponent* GetComponent(int entityID) = 0;
protected:
	std::map<int, std::vector<IComponent*>> components; // A mapping of entity ID to a vector containing all of it's components.
};