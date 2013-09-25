#pragma once

#include <string>


/**
  * \brief A class to contain a generic property.
  *
  * This class is used to pass around generic properties.
  * Properties have a name and a value. The value is stored in
  * vholder and is accessed by calling Get() with the 
  * appropriate type.
  */
class Property {
private:
	Property() { }
public:
	Property(const Property &other) {
		this->name = other.name;
		this->vholder = other.vholder;
		const_cast<Property&>(other).vholder = nullptr;
	}

	/**
	 * \brief Sets the name and value of the property.
	 *
	 * \param[in/out] std::string name The name of the property
	 * \param[in/out] t value The value of the property. typename is inferred on usage.
	 */
	template <typename t>
	Property(std::string name, t value) : name(name), vholder(new ValueHolder<t>(value)) { }


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
	 * \brief Gets the name of this property.
	 *
	 * \returns   std::string The name of this property.
	 */
	std::string GetName() { return this->name; }
private:
	/**
	  * \brief ValueHolderBase is a common base type that can be used to holder a pointer to a specialized templated version of ValueHolder.
	  *
	  */
	class ValueHolderBase {
	public:
		virtual ~ValueHolderBase() { }
	};

	
	/**
	  * \brief A generic value holder type.
	  *
	  */
	template <typename t>
	class ValueHolder : public ValueHolderBase {
	public:
		ValueHolder(t value) : value(value) {}
		t Get() { return this->value; }
	private:
		t value;
	};

	std::string name; // Name of this property.
	ValueHolderBase* vholder; // The value held by this property.
};
