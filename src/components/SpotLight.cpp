#include "components/SpotLight.h"

namespace Sigma {
	SpotLight::SpotLight(const unsigned int entityID) : IComponent(entityID) {
		this->transform.TranslateTo(glm::vec3(0.0f, 0.0f, 0.0f));
		this->transform.Rotate(glm::vec3(0.0f, 0.0f, 0.0f));
		this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		this->intensity = 1.0f;

		this->outerAngle = 3.14159f * 0.15f;
		this->innerAngle = 3.14159f * 0.03f;
		this->cosInnerAngle = glm::cos(this->innerAngle);
		this->cosOuterAngle = glm::cos(this->outerAngle);

		this->enabled = true;
	}
}