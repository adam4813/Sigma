#include "BulletPhysics.h"
#include "..\components\BulletShapeMesh.h"
#include "..\Components\GLMesh.h"

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
		this->solver = new btSequentialImpulseConstraintSolver;
		this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->broadphase, this->solver, this->collisionConfiguration);
		this->dynamicsWorld->setGravity(btVector3(0,-10,0));
		return true;
	}
	
	std::map<std::string,Sigma::IFactory::FactoryFunction>
    BulletPhysics::getFactoryFunctions()
	{
		using namespace std::placeholders;
		std::map<std::string,Sigma::IFactory::FactoryFunction> retval;
		retval["BulletShapeMesh"] = std::bind(&BulletPhysics::createBulletShapeMesh,this,_1,_2);
		return retval;
	}

	void BulletPhysics::createBulletShapeMesh(const unsigned int entityID, std::vector<Property> &properties) {
		BulletShapeMesh* mesh = new BulletShapeMesh(entityID);

		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			Property*  p = &*propitr;

			if (p->GetName() == "x") {
				x = p->Get<float>();
				continue;
			} else if (p->GetName() == "y") {
				y = p->Get<float>();
				continue;
			} else if (p->GetName() == "z") {
				z = p->Get<float>();
				continue;
			} else if (p->GetName() == "meshFile") {
				std::cerr << "Loading mesh: " << p->Get<std::string>() << std::endl;
				GLMesh meshFile(0);
				meshFile.LoadMesh(p->Get<std::string>());
				mesh->SetMesh(&meshFile);
			}
		}
		mesh->InitializeRigidBody(x, y, z);

		this->dynamicsWorld->addRigidBody(mesh->GetRigidBody());

		this->addComponent(entityID, mesh);
	}
}