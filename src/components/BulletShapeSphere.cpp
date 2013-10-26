#include "components/BulletShapeSphere.h"
#include "components/GLMesh.h"

namespace Sigma {
	void Sigma::BulletShapeSphere::SetRadius(float radius) {
		this->shape = new btSphereShape(radius);
	}
}
