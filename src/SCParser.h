#pragma once
#include <string>
#include <vector>

// SC files contain data for Sigma Component creation
// Each line begins with a key. These keys describe the rest of the line.
// There is no whitespace allowed between the key and the rest of the line
// The keys and the line are as follows:
// ----------
// * - A comment. e.g.
// *I am a comment just ignore me.
// ----------
// @ - Entity name. This denotes the start of a new entity. e.g.
// @entity1
// ...
// 
// @entity2
// ----------
// # - The ID of the entity or component that is closest above it. e.g.
// #2
// ----------
// & - The type of the component to create. e.g.
// &GLIcoSphere
// ----------
// > - A component property. Each line has a value and a type.
// The last letter in the line dictates the type of the value. e.g.
// Each property is added to the component it follows.
// >100i
// >10.0f
// >I am a strings
// ----------
// NOTE a blank line or comment line must follow each component.
//
// To create multiple components for each entity just make add a new component line (&).
// @entity1
// #1
// &GLIcoSphere
// >prop1=1.0f
// 
// &GLMesh
// >prop2=2i

class Property;

namespace Sigma {
	namespace parser {
		/**
		 * The component creation properties.
		 */
		struct Component {
			Component() : type("") { }
			std::string type;
			std::vector<Property> properties;
		};
		/**
		 * The entity meta data.
		 */
		struct Entity {
			Entity() : id(0), name("") { }
			std::vector<Component> components;
			int id;
			std::string name;
		};

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
			const Sigma::parser::Entity* GetEntity(const unsigned int index);
		private:
			std::vector<Sigma::parser::Entity> entities;
			std::string fname;
		};
	}
}