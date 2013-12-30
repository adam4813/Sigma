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

		static bool AddEntity(const id_t id, float x, float y, float z, float rx, float ry, float rz) {
		    if (getBody(id) == nullptr) {
                auto shape = new btCapsuleShape(0.3f, 1.3f);
                btScalar mass = 1;
                btVector3 fallInertia(0,0,0);
                auto motionState =	new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(x, y, z)));
                btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape,fallInertia);
                shape->calculateLocalInertia(mass,fallInertia);
                body_map.emplace(id, btRigidBody(fallRigidBodyCI));
                return true;
		    }
		    return false;
		}

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
