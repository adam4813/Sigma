#include "components/BulletShapeSphere.h"
#include "resources/Mesh.h"

namespace Sigma {
	void Sigma::BulletShapeSphere::SetRadius(float radius) {
		SetCollisionShape(new btSphereShape(radius));
	}
}
