#include "SimplePhysics.h"
#include "../Property.h"
#include "../Components/PhysicsMover.h"
#include "../Components/ViewMover.h"

IMoverComponent* SimplePhysics::Factory(const std::string type, const unsigned int entityID, std::vector<Property> &properties) {
	if (type == "PhysicsMover") {
		PhysicsMover* mover = new PhysicsMover(entityID);

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			Property*  p = &(*propitr);
			if (p->GetName() == "transform") {
				GLTransform* t = p->Get<GLTransform*>();
				mover->Transform(t);
			}
		}
		this->components[entityID].push_back(mover);
		return mover;
	}
	else if (type == "ViewMover") {
		ViewMover* mover = new ViewMover(entityID);
		this->components[entityID].push_back(mover);
		return mover;
	}
	return nullptr;
}

bool SimplePhysics::Update(const double delta) {
	// Move
	for (auto eitr = this->components.begin(); eitr != this->components.end(); ++eitr) {
		for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
			(*citr)->ApplyForces(delta);
		}
	}

	// Check for collisions and set position to contact point
	for (auto itr = this->components.begin(); itr != this->components.end(); ++itr) {

	}

	return true;
}

void* SimplePhysics::GetComponent(int entityID) {
	if (this->components.find(entityID) != this->components.end()) {
		return this->components[entityID][0];
	}
	return nullptr;
}

SimplePhysics::~SimplePhysics() {
	for (auto eitr = this->components.begin(); eitr != this->components.end(); ++eitr) {
		for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
			delete (*citr);
		}
	}
}
