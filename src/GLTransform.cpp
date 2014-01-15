#include "GLTransform.h"
#include "components/PhysicalWorldLocation.h"

namespace Sigma {
	const glm::vec3 GLTransform::FORWARD_VECTOR(0.0f,0.0f,-1.0f);
	const glm::vec3 GLTransform::UP_VECTOR(0.0f,1.0f,0.0f);
	const glm::vec3 GLTransform::RIGHT_VECTOR(1.0f,0.0f,0.0f);

	const glm::mat4 GLTransform::GetMatrix() {
		if (this->MMhasChanged) {
			this->transformMatrix =  this->translateMatrix * this->rotateMatrix * this->scaleMatrix;
			this->MMhasChanged = false;
		}

		auto transform = PhysicalWorldLocation::GetTransform(this->parentID);
		if(transform) {
			return transform->GetMatrix() * this->transformMatrix;
		}

		return this->transformMatrix;
	}
}