#include "systems/ResourceSystem.h"

namespace Sigma {
	namespace resource {
#ifndef _WIN32
		std::once_flag ResourceSystem::only_one;
		std::shared_ptr<ResourceSystem> ResourceSystem::instance = nullptr;
#endif // _WIN32
	}
}