#include "components/PointLight.h"

namespace Sigma {
	PointLight::PointLight(const unsigned int entityID) : IComponent(entityID) {
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		this->intensity = 1.0f;

		this->radius = 1.0f;
		this->falloff = 2.0f;
	}

	void PointLight::Activate(GLSLShader *shader) {
		if(shader) {
			glUniform1f(glGetUniformLocation((*shader).GetProgram(), "radius"), this->radius);
			glUniform1f(glGetUniformLocation((*shader).GetProgram(), "falloff"), this->falloff);
	
			// Set Light position and color
			glUniform3f(glGetUniformLocation((*shader).GetProgram(), "lightPositionW"), this->position.x, this->position.y, this->position.z);
			glUniform4f(glGetUniformLocation((*shader).GetProgram(), "diffuseLightColor"), this->color.r, this->color.g, this->color.b, this->color.a);
			glUniform1f(glGetUniformLocation((*shader).GetProgram(), "diffuseLightIntensity"), this->intensity);
		}
	}
}