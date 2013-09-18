#include "SimplePhysics.h"
#include "../Property.h"
#include "../components/PhysicsMover.h"
#include "../components/ViewMover.h"

std::map<std::string,IFactory::FactoryFunction> SimplePhysics::getFactoryFunctions()
{
    using namespace std::placeholders;
    std::map<std::string,IFactory::FactoryFunction> retval=
    {
        {"PhysicsMover",std::bind(&SimplePhysics::createPhysicsMover,this,_1,_2,_3)},
        {"ViewMover",std::bind(&SimplePhysics::createViewMover,this,_1,_2,_3)}
    };
    return retval;
}

void SimplePhysics::createPhysicsMover(const std::string type, const unsigned int entityID, std::vector<Property> &properties) {
		PhysicsMover* mover = new PhysicsMover(entityID);

    for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
        Property*  p = &(*propitr);
        if (p->GetName() == "transform") {
            GLTransform* t = p->Get<GLTransform*>();
            mover->Transform(t);
        }
	}
    this->components[entityID].push_back(mover);
}

void SimplePhysics::createViewMover(const std::string type, const unsigned int entityID, std::vector<Property> &properties)
{
	ViewMover* mover = new ViewMover(entityID);
    this->components[entityID].push_back(mover);
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
