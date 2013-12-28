#include "components/BulletMover.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

namespace Sigma {

	BulletMover::BulletMover(const id_t entityID) : IBulletShape(entityID), transform(nullptr) {}

	BulletMover::~BulletMover() {}

	void BulletMover::ApplyForces(const double delta) {
		if (this->transform) {
			glm::vec3 deltavec(delta);
			glm::vec3 totalForce;
			glm::vec3 targetrvel;

			for (auto rotitr = this->rotationForces.begin(); rotitr != this->rotationForces.end(); ++rotitr) {
				this->transform->Rotate((*rotitr) * deltavec);
			}

			// Inertial rotation
			targetrvel = _rotationtarget * deltavec;
			if(fabs(targetrvel.x) > 0.0001f || fabs(targetrvel.y) > 0.0001f || fabs(targetrvel.z) > 0.0001f) {
				targetrvel = this->transform->Restrict(targetrvel);
				this->transform->Rotate(targetrvel);
				_rotationtarget -= targetrvel;
			}
			targetrvel = this->transform->Restrict(targetrvel);
			this->rotationForces.clear();

			for (auto forceitr = this->forces.begin(); forceitr != this->forces.end(); ++forceitr) {
				totalForce += *forceitr;
			}

			glm::vec3 finalForce = (totalForce.z * this->transform->GetForward()) + 
								   (totalForce.y * this->transform->GetUp()) + 
								   (totalForce.x * this->transform->GetRight());

			body->setActivationState(DISABLE_DEACTIVATION);
			this->body->setLinearVelocity(btVector3(finalForce.x, this->body->getLinearVelocity().y() + 0.000000001f, finalForce.z));
		}
	}

	void BulletMover::UpdateTransform() {
		if (this->transform) {
			btTransform trans;
			this->body->getMotionState()->getWorldTransform(trans);
			this->transform->TranslateTo(trans.getOrigin().x(),trans.getOrigin().y(), trans.getOrigin().z());
		}
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

	void BulletMover::InitializeRigidBody() {
		if(this->transform) {
			this->InitializeRigidBody(
				this->transform->GetPosition().x,
				this->transform->GetPosition().y,
				this->transform->GetPosition().z,
				this->transform->GetPitch(),
				this->transform->GetYaw(),
				this->transform->GetRoll()
			);
		}
		else {
			this->InitializeRigidBody(0,1.5f,0,0,0,0);
		}
	}

	// immediate mode rotation (for mouse motion)
	void BulletMover::RotateNow(float x, float y, float z) {
		if (this->transform) {
			this->transform->Rotate(x,y,z);
		}
	}
	void BulletMover::RotateTarget(float x, float y, float z) {
		if (this->transform) {
			this->_rotationtarget += glm::vec3(x,y,z);
		}
	}
}
