#pragma once
#include "../IBulletShape.h"

namespace Sigma{
	class GLMesh;
	class BulletShapeSphere : public IBulletShape {
	public:
		SET_COMPONENT_ID("BulletShapeSphere");
		BulletShapeSphere(const int entityID = 0) : IBulletShape(entityID) { }
		~BulletShapeSphere() {
		}

		void SetRadius(float radius);
	private:
	};
}