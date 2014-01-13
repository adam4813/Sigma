#include "components/RigidBody.h"
#include "components/PhysicalWorldLocation.h"

namespace Sigma {
	std::unordered_map<id_t, btRigidBody*> RigidBody::body_map;
	btDiscreteDynamicsWorld* RigidBody::dynamicsWorld;

	std::vector<std::unique_ptr<IECSComponent>> RigidBody::AddEntity(const id_t id, const std::vector<Property> &properties) {
		if (getBody(id) == nullptr) {
			btCollisionShape* shape = nullptr;
			std::string shape_type;

			for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
				const Property*  p = &*propitr;

				if (p->GetName() == "shape") {
					shape_type = p->Get<std::string>();
					break;
				}
				//error
			}

			if (shape_type == "capsule") {
				float radius = 0.0f;
				float height = 0.0f;
				for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
					const Property  p = *propitr;
					if (p.GetName() == "radius") {
						radius = p.Get<float>();
					}
					else if (p.GetName() == "height") {
						height = p.Get<float>();
					}
				}
				shape = new btCapsuleShape(radius, height);
			}
			else if (shape_type == "sphere") {
				float radius = 0.0f;
				for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
					const Property*  p = &*propitr;
					if (p->GetName() == "radius") {
						radius = p->Get<float>();
					}
				}
				shape = new btSphereShape(radius);
			}
			else if (shape_type == "mesh") {
				float scale = 1.0f;
				GLMesh glmesh(0);

				for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
					const Property*  p = &*propitr;
					if (p->GetName() == "scale") {
						scale = p->Get<float>();
					}
					else if (p->GetName() == "meshFile") {
						std::cerr << "Loading mesh: " << p->Get<std::string>() << std::endl;
						GLMesh glmesh(0);
						glmesh.LoadMesh(p->Get<std::string>());
					}
				}

				auto btmesh = new btTriangleMesh();
				for (unsigned int i = 0; i < glmesh.GetFaceCount(); ++i) {
					const Sigma::Face* f = glmesh.GetFace(i);
					const Sigma::Vertex* v1 = glmesh.GetVertex(f->v1);
					const Sigma::Vertex* v2 = glmesh.GetVertex(f->v2);
					const Sigma::Vertex* v3 = glmesh.GetVertex(f->v3);
					btmesh->addTriangle(btVector3(v1->x, v1->y, v1->z), btVector3(v2->x, v2->y, v2->z), btVector3(v3->x, v3->y, v3->z));
				}

				shape = new btScaledBvhTriangleMeshShape(new btBvhTriangleMeshShape(btmesh, false), btVector3(scale, scale, scale));

			}

			if (shape) {
				btScalar mass = 1;
				btVector3 fallInertia(0,0,0);
				auto motionState =	PhysicalWorldLocation::GetMotionState(id);
				btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, motionState, shape,fallInertia);
				shape->calculateLocalInertia(mass,fallInertia);
				auto body = new btRigidBody(fallRigidBodyCI);
				body_map.insert(std::make_pair(id, body));
				// add the body to the world
				dynamicsWorld->addRigidBody(body);

				// TODO: return the components
				return std::vector<std::unique_ptr<IECSComponent>>();
			}
		}
		return std::vector<std::unique_ptr<IECSComponent>>();
	}
}
