#include "systems/BulletPhysics.h"
#include "components/BulletShapeMesh.h"
#include "components/GLMesh.h"
#include "components/BulletShapeSphere.h"

namespace Sigma {
	BulletPhysics::~BulletPhysics() {
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

		return true;
	}

	void BulletPhysics::initViewMover() {
		this->mover.InitializeRigidBody(dynamicsWorld);
	}

	std::map<std::string,Sigma::IFactory::FactoryFunction>
    BulletPhysics::getFactoryFunctions()
	{
		using namespace std::placeholders;
		std::map<std::string,Sigma::IFactory::FactoryFunction> retval;
		retval["BulletShapeMesh"] = std::bind(&BulletPhysics::createBulletShapeMesh,this,_1,_2);
		retval["BulletShapeSphere"] = std::bind(&BulletPhysics::createBulletShapeSphere,this,_1,_2);
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
			else if (p->GetName() == "meshFile") {
				std::cerr << "Loading mesh: " << p->Get<std::string>() << std::endl;
				GLMesh meshFile(0);
				meshFile.LoadMesh(p->Get<std::string>());
				mesh->SetMesh(&meshFile);
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
        IMoverComponent::ComputeInterpolatedForces(delta);

	    // TODO : we use 0 as a fake entity id, to be replaced
		this->mover.ApplyForces(0, delta);

		dynamicsWorld->stepSimulation(delta, 10);

		this->mover.UpdateTransform();

		return true;
	}
}
