#include <iostream>

#include "LogSystem.h"
#include "../Components/LogQueue.h"

LogSystem::LogSystem() {

}

LogSystem::~LogSystem() {
	for (auto mapitr = this->components.begin(); mapitr != this->components.end(); ++mapitr) {
		for (auto vecitr = mapitr->second.begin(); vecitr < mapitr->second.end(); ++vecitr) {
			delete *vecitr;
		}
	}
}

IComponent* LogSystem::Factory(const std::string type, const unsigned int entityID) {
	if (type == "LogQueue") {
		LogQueue* log = new LogQueue(entityID);
		this->components[entityID].push_back(log);
		return log;
	}

	return nullptr;
}

void LogSystem::Update(const float delta) {
	for (auto mapitr = this->components.begin(); mapitr != this->components.end(); ++mapitr) {
		for (auto vecitr = mapitr->second.begin(); vecitr < mapitr->second.end(); ++vecitr) {
			LogQueue* log = static_cast<LogQueue*>(*vecitr);
			if (log->GetSize() > 0) {
				std::cout<< log->GetTag() << std::endl; // LOg the tag.
				do {
					std::cout<< "\t\t" << log->GetMessage() << std::endl; // Log each message indented
				} while (log->GetSize() > 0);
				std::cout<< log->GetTag() << std::endl; // Print the closing tag
			}
		}
	}
}

IComponent* LogSystem::GetComponent(int entityID) {
	return nullptr;
}
