#include <iostream>

#include "SceneManager.h"

void SceneManager::SetEntity(int id, TRANSFORM position) {
	this->entitySRT[id] = position;
}

TRANSFORM* SceneManager::GetEntity(int id) {
	if (this->entitySRT.find(id) != this->entitySRT.end()) {
		return &this->entitySRT.at(id);
	}

	return nullptr;
}
