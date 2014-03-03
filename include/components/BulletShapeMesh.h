#pragma once
#include "../IBulletShape.h"
#include "Sigma.h"

namespace Sigma{
	namespace resource {
		class Mesh;
	}
	class BulletShapeMesh : public IBulletShape {
	public:
		SET_COMPONENT_TYPENAME("BulletShapeMesh");
		BulletShapeMesh(const id_t entityID = 0) : IBulletShape(entityID), btmesh(nullptr) {}
		~BulletShapeMesh() {
			if (this->btmesh != nullptr) {
				delete this->btmesh;
			}
		}

		void SetMesh(const resource::Mesh* mesh, btVector3* scale);
		void SetMesh(const resource::Mesh* mesh, float scale);
		void SetMesh(const resource::Mesh* mesh);

	private:
		btTriangleMesh* btmesh;
	};
}
