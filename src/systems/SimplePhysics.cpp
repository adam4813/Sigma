#include "SimplePhysics.h"
#include "Property.h"
#include "../components/PhysicsMover.h"
#include "../components/ViewMover.h"

std::map<std::string,IFactory::FactoryFunction> SimplePhysics::getFactoryFunctions()
{
    using namespace std::placeholders;
    std::map<std::string,IFactory::FactoryFunction> retval;
	retval["PhysicsMover"] = std::bind(&SimplePhysics::createPhysicsMover,this,_1,_2,_3);
    retval["ViewMover"] = std::bind(&SimplePhysics::createViewMover,this,_1,_2,_3);

	// Not supported in VS2012
    /*{
        {"PhysicsMover",std::bind(&SimplePhysics::createPhysicsMover,this,_1,_2,_3)},
        {"ViewMover",std::bind(&SimplePhysics::createViewMover,this,_1,_2,_3)}
    };*/
    return retval;
}

void SimplePhysics::createPhysicsMover(const std::string type, const unsigned int entityID, std::vector<Property> &properties) {
	PhysicsMover* mover = new PhysicsMover(entityID);

    for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
        Property*  p = &(*propitr);
        if (p->GetName() == "transform") {
            GLTransform* t = p->Get<GLTransform*>();
            mover->SetTransform(t);
		} else if(p->GetName() == "rx") {
			mover->AddRotationForce(glm::vec3(p->Get<float>(), 0.0f, 0.0f));
        } else if(p->GetName() == "ry") {
			mover->AddRotationForce(glm::vec3(0.0f, p->Get<float>(), 0.0f));
		} else if(p->GetName() == "rz") {
			mover->AddRotationForce(glm::vec3(0.0f, 0.0f, p->Get<float>()));
		} else if(p->GetName() == "r") {
			mover->AddForce(glm::vec3(p->Get<float>(), 0.0f, 0.0f));
		} else if(p->GetName() == "u") {
			mover->AddForce(glm::vec3(0.0f, p->Get<float>(), 0.0f));
		} else if(p->GetName() == "f") {
			mover->AddForce(glm::vec3(0.0f, 0.0f, p->Get<float>()));
		}
	}
	this->addComponent(entityID,mover);
}

void SimplePhysics::createViewMover(const std::string type, const unsigned int entityID, std::vector<Property> &properties)
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
