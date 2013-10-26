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

	SET_COMPONENT_ID("PointLight");

	/*
	 * /brief Sets this point light as the
	 *        light used by the given shader
	 *
	 * /params shader: the shader to use
	 *
	 */
	void Activate(GLSLShader *shader);

	glm::vec3 position;
	glm::vec4 color;
	float intensity;

	float radius;

	float cAttenuation;
	float lAttenuation;
	float qAttenuation;
};
}
#endif