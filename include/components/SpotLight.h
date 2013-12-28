#pragma once

#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "glm/glm.hpp"
#include "IComponent.h"
#include "systems/GLSLShader.h"
#include "GLTransform.h"
#include "Sigma.h"

namespace Sigma {
	class SpotLight : public IComponent {
	public:
		SpotLight(const id_t entityID);
		virtual ~SpotLight() {}

		SET_COMPONENT_TYPENAME("SpotLight");

		//glm::vec3 position;
		//glm::vec3 direction;
		GLTransform transform;
		glm::vec4 color;
		float intensity;

		float angle;
		float cosCutoff;
		float exponent;

		bool enabled;

		bool IsEnabled() { return enabled; }
	};
}
#endif
