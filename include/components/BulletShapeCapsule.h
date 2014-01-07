#pragma once
#include "../IBulletShape.h"
#include "Sigma.h"

namespace Sigma{
	class GLMesh;
	class BulletShapeCapsule : public IBulletShape {
	public:
		SET_COMPONENT_TYPENAME("BulletShapeCapsule");
		BulletShapeCapsule(const id_t entityID = 0) : IBulletShape(entityID) { }
		~BulletShapeCapsule() { }

		void SetCapsuleSize(float radius, float height) {
			this->radius = radius;
			this->height = height;
		}

		void InitializeRigidBody(float x, float y, float z, float rx, float ry, float rz);
	private:
		float radius;
		float height;
	};
}
