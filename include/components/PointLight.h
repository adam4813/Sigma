#pragma once

#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "glm/glm.hpp"
#include "IComponent.h"
#include "systems/GLSLShader.h"

namespace Sigma {
	class PointLight : public IComponent {
	public:
		PointLight(const unsigned int entityID);
		virtual ~PointLight() {}

		SET_COMPONENT_TYPENAME("PointLight");

		glm::vec3 position;
		glm::vec4 color;
		float intensity;

		float radius;
		float falloff;
	};
}
#endif