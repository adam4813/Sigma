#pragma once
#ifndef SIMPLE_PHYSICS_H
#define SIMPLE_PHYSICS_H


#include "../IFactory.h"
#include "../ISystem.h"
#include "../components/ViewMover.h"

#include <string>
#include <vector>
#include <map>
#include "../AABBTree.h"

class Property;
class IMoverComponent;

class SimplePhysics
    : public Sigma::IFactory, public Sigma::ISystem<Sigma::IMoverComponent> {
public:
	SimplePhysics() { }
	~SimplePhysics() {
		for (auto treeitr = this->colliders.begin(); treeitr != this->colliders.end(); ++treeitr) {
			delete treeitr->second;
		}
	};
	/**
	 * \brief Starts the Simple Physics system.
	 *
	 * \return bool Returns false on startup failure.
	 */
	bool Start() { }

	/**
	 * \brief Causes an update in the system based on the change in time.
	 *
	 * Updates the state of the system based off how much time has elapsed since the last update.
	 * \param[in] const float delta The change in time since the last update
	 * \return bool Returns true if we had an update interval passed.
	 */
	bool Update(const double delta);

    std::map<std::string,FactoryFunction> getFactoryFunctions();

	Sigma::IComponent* createPhysicsMover(const unsigned int entityID, const std::vector<Property> &properties) ;
	Sigma::IComponent* createViewMover(const unsigned int entityID, const std::vector<Property> &properties) ;
	Sigma::IComponent* createAABBTree(const unsigned int entityID, const std::vector<Property> &properties) ;

	Sigma::AABBTree* GetCollider(const unsigned int entityID) {
		if (this->colliders.find(entityID) != this->colliders.end()) {
			return this->colliders[entityID];
		}
		return nullptr;
	}

private:
	std::map<unsigned int, Sigma::AABBTree*> colliders;
}; // class SimplePhysics

#endif // SIMPLE_PHYSICS_H
