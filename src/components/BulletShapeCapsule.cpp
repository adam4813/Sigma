#include "components/BulletShapeCapsule.h"
#include "resources/Mesh.h"

namespace Sigma {
	void BulletShapeCapsule::InitializeRigidBody(float x, float y, float z, float rx, float ry, float rz) {
		btCapsuleShape* shape = new btCapsuleShape(this->radius, this->height);
		btScalar mass = 1;
		btVector3 fallInertia(0,0,0);
		this->motionState = new btDefaultMotionState(btTransform(btQuaternion(rx, ry, rz, 1),btVector3(x, y, z)));
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape,fallInertia);
		shape->calculateLocalInertia(mass,fallInertia);
		btRigidBody* body = new btRigidBody(fallRigidBodyCI);

		SetCollisionShape(shape);
		SetRigidBody(body);
	}
}
