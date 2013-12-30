#ifndef CONTROLLABLEMOVE_H_INCLUDED
#define CONTROLLABLEMOVE_H_INCLUDED

#include <unordered_map>

#include "Sigma.h"
#include "IComponent.h"
#include "GLTransform.h"

namespace Sigma {
    class ControllableMove : IComponent {
    public:
		SET_COMPONENT_TYPENAME("ControllableMove");

		ControllableMove() {};

		virtual ~ControllableMove() {};

		static bool AddEntity(const id_t id) {
		    if (GetTransform(id) == nullptr) {
                transform_map.emplace(id, nullptr);
                return true;
		    }
		    return false;
		}

		static void RemoveEntity(const id_t id) {
		    transform_map.erase(id);
		}

		static GLTransform* GetTransform(const id_t id) {
		    auto itt = transform_map.find(id);
		    if (itt != transform_map.end()) {
                return itt->second;
		    }
		    return nullptr;
		}

		static void SetTransform(const id_t id, GLTransform* transform) {
            transform_map.at(id) = transform;
		}

    private:
        static std::unordered_map<id_t, GLTransform*> transform_map;
    };
}

#endif // CONTROLLABLEMOVE_H_INCLUDED
