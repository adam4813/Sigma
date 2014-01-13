#ifndef IENTITY_H_INCLUDED
#define IENTITY_H_INCLUDED

#include <unordered_map>
#include "IECSComponentData.h"
#include "IECSComponent.h"

namespace Sigma {

	class EntitySystem;

    /** \brief An interface for entities
     */
	class IEntity {
	public:
		IEntity(const id_t entityID) : entityID(entityID) {};
		virtual ~IEntity() {};

		template<class T>
		T* GetAs();

		const id_t GetEntityID() const { return entityID; };

		static EntitySystem* entitySystem;
		std::unordered_map<ComponentID,std::unique_ptr<IECSComponent>> Componentmap;

	protected:
		const id_t entityID;
	};
}

#endif // IENTITY_H_INCLUDED
