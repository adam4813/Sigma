#include "IBulletShape.h"
#include "controllers/PhysicalWorldController.h"

namespace Sigma {
    void IBulletShape::InitializeRigidBody(float x, float y, float z, float rx, float ry, float rz) {
        btTransform transform;
        transform.setIdentity();
        transform.setRotation(btQuaternion(btVector3(0,1,0), btRadians(ry)));
        transform.setOrigin(btVector3(x, y, z));
//			this->motionState = new btDefaultMotionState(transform);
        this->motionState = PhysicalWorldController::GetMotionState(this->GetEntityID());
        this->body = new btRigidBody(0.0d, this->motionState, this->shape);
        this->body->setContactProcessingThreshold(BT_LARGE_FLOAT);
        this->body->setCcdMotionThreshold(.5);
        this->body->setCcdSweptSphereRadius(0);
    }
}
