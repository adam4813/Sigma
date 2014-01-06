#pragma once
#include "../IBulletShape.h"
#include "Sigma.h"

namespace Sigma{
	class GLMesh;
	class BulletShapeSphere : public IBulletShape {
	public:
		SET_COMPONENT_TYPENAME("BulletShapeSphere");
		BulletShapeSphere(const id_t entityID = 0) : IBulletShape(entityID) { }
		~BulletShapeSphere() { }

		void SetRadius(float radius);
	private:
	};
}
