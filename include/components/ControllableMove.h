#ifndef CONTROLLABLEMOVE_H_INCLUDED
#define CONTROLLABLEMOVE_H_INCLUDED

#include <unordered_map>

#include <bullet/btBulletDynamicsCommon.h>
#include "Sigma.h"
#include "IComponent.h"
#include "GLTransform.h"
#include "components/RigidBody.h"

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

        /** \brief Update the transform of all entities having this component
         *
         * The transform is updated from the model
         *
         */
		static void UpdateTransform() {
            btTransform trans;
		    for (auto it = transform_map.begin(); it != transform_map.end(); it++) {
                // TODO: Use a position component instead of RigidBody component to get the position
                auto body = RigidBody::getBody(it->first);
                if (body != nullptr) {
                    body->getMotionState()->getWorldTransform(trans);
                    it->second->TranslateTo(trans.getOrigin().x(),trans.getOrigin().y(), trans.getOrigin().z());
                }
		    }
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
