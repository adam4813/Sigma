#pragma once

#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "glm/glm.hpp"
#include "IComponent.h"
#include "systems/GLSLShader.h"

namespace Sigma {
	class SpotLight : public IComponent {
	public:
		SpotLight(const unsigned int entityID);
		virtual ~SpotLight() {}

		SET_COMPONENT_TYPENAME("SpotLight");

		glm::vec3 position;
		glm::vec3 direction;
		glm::vec4 color;
		float intensity;

		float angle;
		float cosCutoff;
		float exponent;
	};
}
#endif