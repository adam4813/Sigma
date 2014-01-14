#include "systems/EntitySystem.h"

namespace Sigma {
	// IEntity implementation
	EntitySystem* IEntity::entitySystem = 0;

	EntitySystem::EntitySystem(FactorySystem* factory) : componentFactory(factory) { IEntity::entitySystem = this; }

	void EntitySystem::addFeature(IEntity* e, const FeatureID& fid, const std::vector<Property>& properties) {
		// create feature in ECS and receive a vector of unique_ptr<IECSComponent>
		auto componentECS = componentFactory->createECS(fid, e->GetEntityID(), properties);
		// store the pointers of the components in the IEntity component map
		for (auto itr = componentECS.begin(); itr != componentECS.end(); itr++) {
			e->Componentmap.emplace((*itr)->getComponentTypeName(), std::move(*itr));
		}
	}

	void EntitySystem::RemoveFeature(IEntity *e, const FeatureID& cid) {
		// TODO : add method to remove component in factory ?
		//componentFactory.remove(FeatureID, e->GetEntityID());
		// get a list of components to remove in entity map
	}
}
