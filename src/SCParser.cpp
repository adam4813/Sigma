#include "SCParser.h"
#include "Property.h"
#include "strutils.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace Sigma {
	namespace parser {
		bool SCParser::Parse(const std::string& fname) {
			this->fname = fname;
			std::ifstream in(this->fname, std::ios::in);

			// Some type of error opening the file
			if (!in) {
				std::cerr << "Cannot open sc file " << fname << std::endl;
				return false;
			}

			std::string line;
			Sigma::parser::Entity* currentEntity = nullptr;

			while (getline(in, line)) {
        // strip line's whitespace
        line = rtrim(line);
        // Strip C style comments
        line = rcomment(line);

				char key[1];
				key[0] = line.substr(0,1)[0];
				if (key[0] == '@') { // name
					if (currentEntity != nullptr) {
						this->entities.push_back(*currentEntity); // Copy
						delete currentEntity; // delete the heap original
					}
					currentEntity = new Sigma::parser::Entity();
					currentEntity->name = line.substr(1);
				}
				else if (key[0] == '#') { // id
					currentEntity->id = atoi(line.substr(1).c_str());
				}
				else if (key[0] == '&') { // component type
					Sigma::parser::Component c;
					c.type = line.substr(1);
					while (getline(in, line)) {
                        // strip line's whitespace
                        line = rtrim(line);
						if (line.substr(0,1) == ">") { // property line
							std::string propName = line.substr(1, line.find("=") - 1);
							std::string propValue = line.substr(line.find("=") + 1);
							std::string propType = propValue.substr(propValue.size() - 1);
							propValue = propValue.substr(0, propValue.size() - 1);

							if (propType == "f") { // float
								Property p(propName, static_cast<float>(atof(propValue.c_str())));
								c.properties.push_back(p);
							}
							else if (propType == "s") { // string
								Property p(propName, propValue);
								c.properties.push_back(p);
							}
							else if (propType == "i") { // int
								Property p(propName, atoi(propValue.c_str()));
								c.properties.push_back(p);
							}
							else if (propType == "b") {
								// bool can be represented in many ways.
								// Let us rely on the implementation of the std lib to match the value against the representations it knows.
								bool b;
								std::stringstream ss(propValue);
								ss >> b;
								Property p(propName, b);
								c.properties.push_back(p);
							}
						} else if (line.substr(0,1) == "#") { // id
							Property p("id", atoi(line.substr(1).c_str()));
							c.properties.push_back(p);
						} else {
							break;
						}
					}
					if(currentEntity != nullptr) {
						currentEntity->components.push_back(c);
					}
					else {
						std::cerr << "Attempted to add component to undefined entity." << std::endl;
					}
				}
			}
			if (currentEntity != nullptr) {
				this->entities.push_back(*currentEntity); // copy
				delete currentEntity; // delete the heap original
			}
			return true; // Successfully parsed a file. It might have been empty though.
		}

		unsigned int SCParser::EntityCount() {
			return this->entities.size();
		}

		Sigma::parser::Entity* SCParser::GetEntity(const unsigned int index) {
			if (index < this->entities.size()) {
				return &this->entities[index];
			}
			return nullptr;
		}
	}
}
