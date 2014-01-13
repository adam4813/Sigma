#include "IEntity.h"
#include "systems/EntitySystem.h"

namespace Sigma {
	EntitySystem* IEntity::entitySystem = 0;

	template<class T>
	T* IEntity::GetAs() {
		return entitySystem->getComponent<T>(this);
	}
}
