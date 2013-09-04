#pragma once
#include <string>
#include <vector>

class Property;

namespace Sigma {
	namespace Parser {
		/**
		 * The component creation properties.
		 */
		struct Component {
			std::string type;
			std::vector<Property> properties;
		};
		/**
		 * The entity meta data.
		 */
		struct Entity {
			std::vector<Component> components;
			int id;
			std::string name;
		};
	}
}

class SCParser {
public:
	SCParser() { }

	/**
	 * \brief Parses a file into entities and their components.
	 *
	 * Given a file name the parser will parse through the file and create an entity list. These entities will have their components parsed as well.
	 * \param[in] const std::string & fname Name of the file to parse.
	 * \returns   bool false if there was a file opening issue.
	 */
	bool Parse(const std::string& fname);

	/**
	 * \brief The number of entities parsed.
	 *
	 * \returns   unsigned int The number of entities parsed.
	 */
	unsigned int EntityCount();

	/**
	 * \brief Gets an entity at the specific index.
	 *
	 * Returns an entity pointer. This is a pointer to an entity in the list and not a copy.
	 * \param[in] const unsigned int index The index of the entity list.
	 * \returns   const Entity* The entity from the list or nullptr if the index is OOB.
	 * \exception  
	 */
	const Sigma::Parser::Entity* GetEntity(const unsigned int index);
private:
	std::vector<Sigma::Parser::Entity> entities;
	std::string fname;
};