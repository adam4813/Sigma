#include "components/SpotLight.h"

namespace Sigma {
	SpotLight::SpotLight(const unsigned int entityID) : IComponent(entityID) {
		this->transform.TranslateTo(glm::vec3(0.0f, 0.0f, 0.0f));
		this->transform.Rotate(glm::vec3(0.0f, 0.0f, 0.0f));
		this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		this->intensity = 1.0f;

		this->angle = 3.14159f * 0.25f;
		this->cosCutoff = glm::cos(this->angle);
		this->exponent = 2.0f;

		this->enabled = true;
	}
}