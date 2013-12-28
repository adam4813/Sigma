#pragma once
#include "../IBulletShape.h"
#include "Sigma.h"

namespace Sigma{
	class GLMesh;
	class BulletShapeMesh : public IBulletShape {
	public:
		SET_COMPONENT_TYPENAME("BulletShapeMesh");
		BulletShapeMesh(const id_t entityID = 0) : IBulletShape(entityID) { }
		~BulletShapeMesh() {
			if (this->btmesh != nullptr) {
				delete this->btmesh;
			}
		}

		void SetMesh(const GLMesh* mesh);
	private:
		btTriangleMesh* btmesh;
	};
}
