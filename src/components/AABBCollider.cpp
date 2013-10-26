#include "components/AABBCollider.h"
#include <cmath>

bool AABBCollider::CheckCollision(const AABBCollider& other) {
	if(fabs(this->center[0] - other.center[0]) < this->halfSize[0] + other.halfSize[0]) {
		if(fabs(this->center[1] - other.center[1]) < this->halfSize[1] + other.halfSize[1]) {
			if(fabs(this->center[2] - other.center[2]) < this->halfSize[2] + other.halfSize[2]) {
				return true;
			}
		}
	}

	return false;
}

