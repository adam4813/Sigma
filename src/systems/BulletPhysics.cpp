#include "systems/BulletPhysics.h"
#include "components/BulletShapeMesh.h"
#include "components/GLMesh.h"
#include "components/BulletShapeSphere.h"
#include "components/PhysicalWorldLocation.h"

namespace Sigma {
	// We need ctor and dstor to be exported to a dll even if they don't do anything
	BulletPhysics::BulletPhysics() {}
	BulletPhysics::~BulletPhysics() {
		if (this->mover != nullptr) {
			delete this->mover;
		}
		if (this->dynamicsWorld != nullptr) {
			delete this->dynamicsWorld;
		}
		if (this->solver != nullptr) {
			delete this->solver;
		}
		if (this->dispatcher != nullptr) {
			delete this->dispatcher;
		}
		if (this->collisionConfiguration != nullptr) {
			delete this->collisionConfiguration;
		}
		if (this->broadphase != nullptr) {
			delete this->broadphase;
		}
	}

	bool BulletPhysics::Start() {
		this->broadphase = new btDbvtBroadphase();
		this->collisionConfiguration = new btDefaultCollisionConfiguration();
		this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);
		this->solver = new btSequentialImpulseConstraintSolver();
		this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->collisionConfiguration);
		this->dynamicsWorld->setGravity(btVector3(0,-10,0));
		RigidBody::setWorld(dynamicsWorld);
		return true;
	}

	void BulletPhysics::initViewMover(GLTransform& t) {
		this->moverSphere = new BulletShapeCapsule(1);
		this->moverSphere->SetCapsuleSize(0.3f, 1.3f);
		this->moverSphere->InitializeRigidBody(
			t.GetPosition().x,
			t.GetPosition().y,
			t.GetPosition().z,
			t.GetPitch(),
			t.GetYaw(),
			t.GetRoll());
		this->mover = new PhysicsController(*moverSphere, t);
		this->dynamicsWorld->addRigidBody(this->moverSphere->GetRigidBody());
	}

	std::map<std::string,Sigma::IFactory::FactoryFunction>
    BulletPhysics::getFactoryFunctions() {
		using namespace std::placeholders;
		std::map<std::string,Sigma::IFactory::FactoryFunction> retval;
		retval["BulletShapeMesh"] = std::bind(&BulletPhysics::createBulletShapeMesh,this,_1,_2);
		retval["BulletShapeSphere"] = std::bind(&BulletPhysics::createBulletShapeSphere,this,_1,_2);
		return retval;
	}

	std::map<std::string,Sigma::IECSFactory::FactoryFunction>
    BulletPhysics::getECSFactoryFunctions() {
		using namespace std::placeholders;
		std::map<std::string,Sigma::IECSFactory::FactoryFunction> retval;
		retval[ControllableMove::getComponentTypeName()] = std::bind(&ControllableMove::AddEntity,_1);
		retval[InterpolatedMovement::getComponentTypeName()] = std::bind(&InterpolatedMovement::AddEntity,_1);
		retval[PhysicalWorldLocation::getComponentTypeName()] = std::bind(&PhysicalWorldLocation::AddEntity,_1,_2);
		retval[RigidBody::getComponentTypeName()] = std::bind(&RigidBody::AddEntity,_1,_2);
		return retval;
	}

	IComponent* BulletPhysics::createBulletShapeMesh(const id_t entityID, const std::vector<Property> &properties) {
		BulletShapeMesh* mesh = new BulletShapeMesh(entityID);

		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float rx = 0.0f;
		float ry = 0.0f;
		float rz = 0.0f;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &*propitr;

			if (p->GetName() == "scale") {
				scale = p->Get<float>();
			}
			else if (p->GetName() == "x") {
				x = p->Get<float>();
			}
			else if (p->GetName() == "y") {
				y = p->Get<float>();
			}
			else if (p->GetName() == "z") {
				z = p->Get<float>();
			}
			else if (p->GetName() == "rx") {
				rx = p->Get<float>();
			}
			else if (p->GetName() == "ry") {
				ry = p->Get<float>();
			}
			else if (p->GetName() == "rz") {
				rz = p->Get<float>();
			}
			else if (p->GetName() == "meshFile") {
				std::cerr << "Loading mesh: " << p->Get<std::string>() << std::endl;
				GLMesh meshFile(0);
				meshFile.LoadMesh(p->Get<std::string>());
				mesh->SetMesh(&meshFile, scale);
			}
		}
		mesh->InitializeRigidBody(x, y, z, rx, ry, rz);

		this->dynamicsWorld->addRigidBody(mesh->GetRigidBody());

		this->addComponent(entityID, mesh);

		return mesh;
	}

	IComponent* BulletPhysics::createBulletShapeSphere(const id_t entityID, const std::vector<Property> &properties) {
		BulletShapeSphere* sphere = new BulletShapeSphere(entityID);

		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float rx = 0.0f;
		float ry = 0.0f;
		float rz = 0.0f;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			const Property*  p = &*propitr;

			if (p->GetName() == "x") {
				x = p->Get<float>();
			}
			else if (p->GetName() == "y") {
				y = p->Get<float>();
			}
			else if (p->GetName() == "z") {
				z = p->Get<float>();
			}
			else if (p->GetName() == "rx") {
				rx = p->Get<float>();
			}
			else if (p->GetName() == "ry") {
				ry = p->Get<float>();
			}
			else if (p->GetName() == "rz") {
				rz = p->Get<float>();
			}
			else if (p->GetName() == "radius") {
				sphere->SetRadius(p->Get<float>());
			}
		}

		sphere->InitializeRigidBody(x, y, z, rx, ry, rz);

		this->dynamicsWorld->addRigidBody(sphere->GetRigidBody());

		this->addComponent(entityID, sphere);

		return sphere;
	}

	bool BulletPhysics::Update(const double delta) {
		this->mover->UpdateForces(delta);
		// Make entities with target move a little
		InterpolatedMovement::ComputeInterpolatedForces(delta);
		// It's time to sum all the forces
		ControllableMove::CumulateForces();
		// We inject the movement in the simulation or directly
		ControllableMove::ApplyForces(delta);
		// We step the simulation
		dynamicsWorld->stepSimulation(delta, 10);
		// We update the transform component with updated data of the PhysicalWorldLocation component
		PhysicalWorldLocation::UpdateTransform();
		PhysicalWorldLocation::ClearUpdatedSet();
		this->mover->UpdateTransform();

		return true;
	}
}
