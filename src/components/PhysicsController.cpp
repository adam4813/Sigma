#include "components/PhysicsController.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

namespace Sigma {
	PhysicsController::~PhysicsController() {}

	void PhysicsController::UpdateForces(const double delta) {
		glm::vec3 deltavec(delta);
		glm::vec3 totalForce;
		glm::vec3 targetrvel;
		glm::vec3 targettvel;

		for (auto rotitr = this->rotationForces.begin(); rotitr != this->rotationForces.end(); ++rotitr) {
			this->transform.Rotate((*rotitr) * deltavec);
		}

		// Inertial rotation
		targetrvel = this->rotationTarget * deltavec * this->rotationSpeed;
		if(fabs(targetrvel.x) > 0.0001f || fabs(targetrvel.y) > 0.0001f || fabs(targetrvel.z) > 0.0001f) {
			targetrvel = this->transform.Restrict(targetrvel);
			this->transform.Rotate(targetrvel);
			this->rotationTarget -= targetrvel;
		}
		targetrvel = this->transform.Restrict(targetrvel);


		for (auto forceitr = this->forces.begin(); forceitr != this->forces.end(); ++forceitr) {
			totalForce += *forceitr;
		}

		glm::vec3 finalForce = (totalForce.z * this->transform.GetForward()) + 
								(totalForce.y * this->transform.GetUp()) + 
								(totalForce.x * this->transform.GetRight());

		// Inertial translation
		targettvel = (this->translationTarget - this->transform.GetPosition()) * deltavec * this->translationSpeed;
		if(fabs(targettvel.x) > 0.0001f || fabs(targettvel.y) > 0.0001f || fabs(targettvel.z) > 0.0001f) {
			this->transform.Translate(targettvel);
		}

		finalForce += targettvel;

		this->shape.GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
		this->shape.GetRigidBody()->setLinearVelocity(btVector3(finalForce.x, this->shape.GetRigidBody()->getLinearVelocity().y() + 0.000000001f, finalForce.z));
	}

	void PhysicsController::UpdateTransform() {
		btTransform trans;
		this->shape.GetRigidBody()->getMotionState()->getWorldTransform(trans);
		this->transform.TranslateTo(trans.getOrigin().x(),trans.getOrigin().y(), trans.getOrigin().z());
	}

	void PhysicsController::Rotate(float x, float y, float z) {
		glm::vec3 rotationVec(x, y, z);
		rotationVec = this->transform.Restrict(rotationVec);
		this->transform.Rotate(rotationVec);
	}

	void PhysicsController::RotateTo(float x, float y, float z, float speedX, float speedY, float speedZ) {
		this->rotationTarget += glm::vec3(x, y, z);
		this->rotationSpeed = glm::vec3(speedX, speedY, speedZ);
	}

	void PhysicsController::Translate(float x, float y, float z) {
		glm::vec3 translationVec(x, y, z);
		this->transform.Translate(translationVec);
	}

	void PhysicsController::TranslateTo(float x, float y, float z, float speedX, float speedY, float speedZ) {
		this->translationTarget += glm::vec3(x, y, z);
		this->translationSpeed = glm::vec3(speedX, speedY, speedZ);
	}

}
