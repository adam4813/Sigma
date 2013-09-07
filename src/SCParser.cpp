#include "SCParser.h"
#include "ISystem.h"
#include <fstream>
#include <iostream>

bool SCParser::Parse(const std::string& fname) {
	this->fname = fname;
	std::ifstream in(this->fname, std::ios::in);
	if (!in) {
		std::cerr << "Cannot open " << fname << std::endl;
		return false;
	}

	std::string line;
	Sigma::Parser::Entity* currentEntity = nullptr;
	while (getline(in, line)) {

		char key[1];
		key[0] = line.substr(0,1)[0];
		if (key[0] == '@') {
			if (currentEntity != nullptr) {
				this->entities.push_back(*currentEntity);
				delete currentEntity;
			}
			currentEntity = new Sigma::Parser::Entity();
			currentEntity->name = line.substr(1);
		}
		else if (key[0] == '#') {
			currentEntity->id = atoi(line.substr(1).c_str());
		}
		else if (key[0] == '&') {
			Sigma::Parser::Component c;
			c.type = line.substr(1);
			while (getline(in, line)) {
				if (line.substr(0,1) == ">") {
					std::string propName = line.substr(1, line.find("=") - 1);
					std::string propValue = line.substr(line.find("=") + 1);
					std::string propType = propValue.substr(propValue.size() - 1);
					propValue = propValue.substr(0, propValue.size() - 1);
					Property p(propName);

					if (propType == "f") {
						p.Set<float>(atof(propValue.c_str()));
					}
					else if (propType == "s") {
						p.Set<std::string>(propValue);
					}
					else if (propType == "i") {
						p.Set<int>(atoi(propValue.c_str()));
					}
					c.properties.push_back(p);
				} else {
					break;
				}
			}
			currentEntity->components.push_back(c);
		}
	}
	if (currentEntity != nullptr) {
		this->entities.push_back(*currentEntity);
		delete currentEntity;
	}
	return true;
}

unsigned int SCParser::EntityCount() {
	return this->entities.size();
}

const Sigma::Parser::Entity* SCParser::GetEntity(const unsigned int index) {
	if (index < this->entities.size()) {
		return &this->entities[index];
	}
	return nullptr;
}