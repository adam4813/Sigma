#ifndef RIGIDBODY_H_INCLUDED
#define RIGIDBODY_H_INCLUDED

#include <unordered_map>

#include <bullet/btBulletDynamicsCommon.h>
#include "IComponent.h"

namespace Sigma {
    class RigidBody : IComponent {
    public:
		SET_COMPONENT_TYPENAME("RigidBody");

        RigidBody() {};

        virtual ~RigidBody() {};

		static bool AddEntity(const id_t id);

		static void RemoveEntity(const id_t id) {
		    body_map.erase(id);
		};

        static btRigidBody* getBody(const id_t id) {
            auto body = body_map.find(id);
            if (body != body_map.end()) {
                return &body->second;
            }
            return nullptr;
        };

    private:
       static std::unordered_map<id_t, btRigidBody> body_map;
    };
}

#endif // RIGIDBODY_H_INCLUDED
