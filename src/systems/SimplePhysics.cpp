#include "systems/SimplePhysics.h"
#include "Property.h"
#include "components/PhysicsMover.h"
#include "components/ViewMover.h"
#include "systems/IGLView.h"

std::map<std::string,Sigma::IFactory::FactoryFunction>
    SimplePhysics::getFactoryFunctions()
{
    using namespace std::placeholders;
    std::map<std::string,Sigma::IFactory::FactoryFunction> retval;
	retval["PhysicsMover"] = std::bind(&SimplePhysics::createPhysicsMover,this,_1,_2);
	retval["ViewMover"] = std::bind(&SimplePhysics::createViewMover,this,_1,_2);
	retval["AABBTree"] = std::bind(&SimplePhysics::createAABBTree,this,_1,_2);

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

void SimplePhysics::createViewMover(const unsigned int entityID, std::vector<Property> &properties) {
	ViewMover* mover = new ViewMover(entityID);
	this->addComponent(entityID,mover);
}

void SimplePhysics::createAABBTree(const unsigned int entityID, std::vector<Property> &properties) {
	Sigma::AABBTree* tree = new Sigma::AABBTree(entityID);
	float scale = 1.0f;
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float rx = 0.0f;
	float ry = 0.0f;
	float rz = 0.0f;
	float halfsize = 0.0f;
	int componentID = 0;
	unsigned int subdivisions=0;
	std::string filename = "";

	for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
		Property*  p = &(*propitr);
		if (p->GetName() == "scale") {
			scale = p->Get<float>();
		} else if (p->GetName() == "x") {
			x = p->Get<float>();
		} else if (p->GetName() == "y") {
			y = p->Get<float>();
		} else if (p->GetName() == "z") {
			z = p->Get<float>();
		} else if (p->GetName() == "rx") {
			rx = p->Get<float>();
		} else if (p->GetName() == "ry") {
			ry = p->Get<float>();
		} else if (p->GetName() == "rz") {
			rz = p->Get<float>();
		} else if (p->GetName() == "halfsize") {
			halfsize = p->Get<float>();
		} else if (p->GetName() == "meshFile") {
			filename = p->Get<std::string>();
		} else if (p->GetName() == "subdivisions") {
			subdivisions = p->Get<int>();
		}
	}

	tree->Offset(glm::vec3(x,y,z));
	tree->Rotation(glm::vec3(rx,ry,rz));
	tree->Halfsize(halfsize);
	tree->Populate(filename);
	for (unsigned int i = 2; i < subdivisions; ++ i) {
		tree->Subdivivde(nullptr, i);
	}
	this->colliders[entityID] = tree;
}

bool SimplePhysics::Update(const double delta) {
	// Check for collisions and add the collision point normal to the view movers
	for (auto itr = this->colliders.begin(); itr != this->colliders.end(); ++itr) {
		for (auto eitr = this->_Components.begin(); eitr != this->_Components.end(); ++eitr) {
			for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
				ViewMover* mover = static_cast<ViewMover*>(citr->second.get());

				glm::vec3 cameraPos = mover->View()->Transform.GetPosition() - itr->second->Offset();

				unsigned int collisions = itr->second->CollisionCheck(cameraPos, 0.10f);
				for (unsigned int i = 0; i < collisions; ++i) {
					mover->AddNormalForce(itr->second->GetCollisionPoint(i)->normal);
				}
			}
		}
	}

	// Move
	for (auto eitr = this->_Components.begin(); eitr != this->_Components.end(); ++eitr) {
		for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
			citr->second->ApplyForces(delta);
		}
	}

	return true;
}
