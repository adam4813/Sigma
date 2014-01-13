#ifndef ENTITYSYSTEM_H_INCLUDED
#define ENTITYSYSTEM__H_INCLUDED

#include <vector>
#include <memory>
#include "Sigma.h"
#include "Property.h"
#include "IECSComponent.h"
#include "systems/FactorySystem.h"

namespace Sigma {
	class IEntity;
	class FactorySystem;

    /** \brief A system that manage the components of entities
     */
	class EntitySystem {
		typedef std::pair<ComponentID, std::unique_ptr<IECSComponent>> element_t;
	public:
		EntitySystem(FactorySystem* factory);
		virtual ~EntitySystem() {};

		template<class T>
		T* getComponent(IEntity* e);

        /** \brief Add a feature, i.e a set of predefined components, to an entity
         *
         * \param e IEntity* the entity
         * \param fid const FeatureID& the feature ID
         * \param properties const Property& the property object to initialize the feature
         *
         */
		void addFeature(IEntity* e, const FeatureID& fid, const std::vector<Property>& properties);

		void RemoveFeature(IEntity *e, const FeatureID& cid);
			// TODO : add method to remove component in factory ?
			//componentFactory.remove(FeatureID, e->GetEntityID());
			// get a list of components to remove in entity map
	private:
		FactorySystem* componentFactory;
	};
}

#endif // ENTITYSYSTEM_H_INCLUDED
