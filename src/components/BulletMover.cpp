#include "BulletMover.h"

#include "../systems/IGLView.h"
#include "bullet/btBulletCollisionCommon.h"
#include "bullet/btBulletDynamicsCommon.h"

namespace Sigma {

	BulletMover::BulletMover(const int entityID) : IBulletShape(entityID) {
	}

	BulletMover::~BulletMover() {
	}

	void BulletMover::ApplyForces(const double delta) {
		glm::vec3 deltavec(delta);
		glm::vec3 totalForce;

		for (auto forceitr = this->forces.begin(); forceitr != this->forces.end(); ++forceitr) {
			totalForce += *forceitr;
		}

		//totalForce *= deltavec;

		glm::vec3 right_direction = glm::normalize(glm::cross(this->view->Transform.GetForward(), GLTransform::UP_VECTOR));
		glm::vec3 forward_direction = glm::normalize(glm::cross(GLTransform::UP_VECTOR, right_direction));

		glm::vec3 finalForce = (totalForce.z * -forward_direction) + (totalForce.y * GLTransform::UP_VECTOR) + (totalForce.x * -right_direction);

		body->setActivationState(DISABLE_DEACTIVATION);
		this->body->setLinearVelocity(btVector3(finalForce.x, this->body->getLinearVelocity().y() + 0.000000001f, finalForce.z));

		// There are no forces here
		for (auto rotitr = this->rotationForces.begin(); rotitr != this->rotationForces.end(); ++rotitr) {
			this->view->Transform.Rotate((*rotitr) * deltavec);
		}

		this->rotationForces.clear();
	}

	void BulletMover::UpdateView() {
		btTransform trans;
		this->body->getMotionState()->getWorldTransform(trans);
		this->view->Transform.TranslateTo(trans.getOrigin().x(),trans.getOrigin().y(), trans.getOrigin().z());
	}

	void BulletMover::InitializeRigidBody(float x, float y, float z, float rx, float ry, float rz) {
		this->shape = new btCapsuleShape(0.3f, 1.3f);
		btScalar mass = 1;
		btVector3 fallInertia(0,0,0);
		this->motionState =	new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(x, y, z)));
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape,fallInertia);
		this->shape->calculateLocalInertia(mass,fallInertia);
		this->body = new btRigidBody(fallRigidBodyCI);
	}
}