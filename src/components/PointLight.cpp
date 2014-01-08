#include "components/PointLight.h"

namespace Sigma {
	PointLight::PointLight(const id_t entityID) : IComponent(entityID) {
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		this->intensity = 1.0f;

		this->radius = 1.0f;
		this->falloff = 2.0f;
	}
}
