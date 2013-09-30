#pragma once

class AABBCollider {
public:
	AABBCollider(float center[3], float halfSize[3]) {
		this->center[0] = center[0]; this->center[1] = center[1]; this->center[2] = center[2];
		this->halfSize[0] = halfSize[0]; this->halfSize[1] = halfSize[1]; this->halfSize[2] = halfSize[2];
	}

	bool CheckCollision(const AABBCollider& other);
	void MoveTo(float center[3]) {
		this->center[0] = center[0]; this->center[1] = center[1]; this->center[2] = center[2];
	}
private:
	float center[3];
	float halfSize[3];
};