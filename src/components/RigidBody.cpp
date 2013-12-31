#include "components/RigidBody.h"
#include "components/PhysicalWorldLocation.h"

namespace Sigma {
    std::unordered_map<id_t, btRigidBody> RigidBody::body_map;

    bool RigidBody::AddEntity(const id_t id) {
        if (getBody(id) == nullptr) {
            auto shape = new btCapsuleShape(0.3f, 1.3f);
            btScalar mass = 1;
            btVector3 fallInertia(0,0,0);
            auto motionState =	PhysicalWorldLocation::GetMotionState(id);
            btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape,fallInertia);
            shape->calculateLocalInertia(mass,fallInertia);
            body_map.emplace(id, btRigidBody(fallRigidBodyCI));
            return true;
        }
        return false;
    }
}
