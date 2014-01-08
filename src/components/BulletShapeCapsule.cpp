#include "components/BulletShapeCapsule.h"
#include "components/GLMesh.h"

namespace Sigma {
	void BulletShapeCapsule::InitializeRigidBody(float x, float y, float z, float rx, float ry, float rz) {
		this->shape = new btCapsuleShape(this->radius, this->height);
		btScalar mass = 1;
		btVector3 fallInertia(0,0,0);
		this->motionState = new btDefaultMotionState(btTransform(btQuaternion(rx, ry, rz, 1),btVector3(x, y, z)));
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape,fallInertia);
		this->shape->calculateLocalInertia(mass,fallInertia);
		this->body = new btRigidBody(fallRigidBodyCI);
	}
}
