#include "components/BulletShapeMesh.h"
#include "resources/Mesh.h"

namespace Sigma {
	void BulletShapeMesh::SetMesh(const resource::Mesh* mesh, btVector3* scale) {
		if(!mesh) {
			return;
		}
		this->btmesh = new btTriangleMesh();
		for (unsigned int i = 0; i < mesh->FaceCount(); ++i) {
			const resource::Face* f = mesh->GetFace(i);
			const resource::Vertex* v1 = mesh->GetVertex(f->v1);
			const resource::Vertex* v2 = mesh->GetVertex(f->v2);
			const resource::Vertex* v3 = mesh->GetVertex(f->v3);
			this->btmesh->addTriangle(btVector3(v1->x, v1->y, v1->z), btVector3(v2->x, v2->y, v2->z), btVector3(v3->x, v3->y, v3->z));
		}

		SetCollisionShape(new btScaledBvhTriangleMeshShape(new btBvhTriangleMeshShape(this->btmesh, false), *scale));
	}

	// continence function for an even scale across all dimensions
	void BulletShapeMesh::SetMesh(const resource::Mesh* mesh, const float scale) {
		SetMesh(mesh, new btVector3(scale, scale, scale));
	}

	// for backward compatibility, uses scale = 1.0f
	void BulletShapeMesh::SetMesh(const resource::Mesh* mesh) {
		SetMesh(mesh, 1.0f);
	}
}
