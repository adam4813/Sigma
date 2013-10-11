#include "SimplePhysics.h"
#include "Property.h"
#include "../components/PhysicsMover.h"
#include "../components/ViewMover.h"

std::map<std::string,Sigma::IFactory::FactoryFunction>
    SimplePhysics::getFactoryFunctions()
{
    using namespace std::placeholders;
    std::map<std::string,Sigma::IFactory::FactoryFunction> retval;
	retval["PhysicsMover"] = std::bind(&SimplePhysics::createPhysicsMover,this,_1,_2);
    retval["ViewMover"] = std::bind(&SimplePhysics::createViewMover,this,_1,_2);

	// Not supported in VS2012
    /*{
        {"PhysicsMover",std::bind(&SimplePhysics::createPhysicsMover,this,_1,_2,_3)},
        {"ViewMover",std::bind(&SimplePhysics::createViewMover,this,_1,_2,_3)}
    };*/
    return retval;
}

void SimplePhysics::createPhysicsMover(const unsigned int entityID, std::vector<Property> &properties) {
		PhysicsMover* mover = new PhysicsMover(entityID);

    for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
        Property*  p = &(*propitr);
        if (p->GetName() == "transform") {
            GLTransform* t = p->Get<GLTransform*>();
            mover->Transform(t);
        }
	}
	this->addComponent(entityID,mover);
}

void SimplePhysics::createViewMover(const unsigned int entityID, std::vector<Property> &properties)
{
	ViewMover* mover = new ViewMover(entityID);
	this->addComponent(entityID,mover);
}

bool SimplePhysics::Update(const double delta) {
	// Move
	for (auto eitr = this->_Components.begin(); eitr != this->_Components.end(); ++eitr) {
		for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
			citr->second->ApplyForces(delta);
		}
	}

	// Check for collisions and set position to contact point
	for (auto itr = this->_Components.begin(); itr != this->_Components.end(); ++itr) {

	}

	return true;
}
