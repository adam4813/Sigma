#include "components/PointLight.h"

namespace Sigma {
PointLight::PointLight(const unsigned int entityID) : IComponent(entityID) {
	this->cAttenuation = 1.0f;
	this->lAttenuation = 0.1f;
	this->qAttenuation = 0.001f;

	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->intensity = 1.0f;

	this->radius = 1.0f;
}

void PointLight::Activate(GLSLShader *shader) {
	if(shader) {
		// Set attenuation values
		glUniform1f(glGetUniformLocation((*shader).GetProgram(), "cAttenuation"), this->cAttenuation);
		glUniform1f(glGetUniformLocation((*shader).GetProgram(), "lAttenuation"), this->lAttenuation);
		glUniform1f(glGetUniformLocation((*shader).GetProgram(), "qAttenuation"), this->qAttenuation);
	
		// Set Light position and color
		glUniform3f(glGetUniformLocation((*shader).GetProgram(), "lightPositionW"), this->position.x, this->position.y, this->position.z);
		glUniform4f(glGetUniformLocation((*shader).GetProgram(), "lightColor"), this->color.r, this->color.g, this->color.b, this->color.a);
		glUniform1f(glGetUniformLocation((*shader).GetProgram(), "lightIntensity"), this->intensity);
	}
}
}