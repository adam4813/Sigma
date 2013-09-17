#pragma once

#include <stdexcept>
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
