#include "BulletShapeMesh.h"
#include "GLMesh.h"

namespace Sigma {
	void Sigma::BulletShapeMesh::SetMesh(const GLMesh* mesh) {
		this->btmesh = new btTriangleMesh();
		for (unsigned int i = 0; i < mesh->GetFaceCount(); ++i) {
			const Sigma::Face* f = mesh->GetFace(i);
			const Sigma::Vertex* v1 = mesh->GetVertex(f->v1);
			const Sigma::Vertex* v2 = mesh->GetVertex(f->v2);
			const Sigma::Vertex* v3 = mesh->GetVertex(f->v3);
			this->btmesh->addTriangle(btVector3(v1->x, v1->y, v1->z), btVector3(v2->x, v2->y, v2->z), btVector3(v3->x, v3->y, v3->z));
		}

		this->shape = new btBvhTriangleMeshShape(this->btmesh, false);
	}
}
