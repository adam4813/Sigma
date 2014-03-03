#include "components/BulletShapeSphere.h"

namespace Sigma {
	void Sigma::BulletShapeSphere::SetRadius(float radius) {
		SetCollisionShape(new btSphereShape(radius));
	}
}
