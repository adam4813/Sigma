#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class GLTransform {
public:
	GLTransform() : orientation(glm::quat(1,0,0,0)) {}
	void Translate(float x, float y, float z) {
		this->translateMatrix = glm::translate(this->translateMatrix, x, y, z);
		this->MMhasChanged = true;
	}

	// Helper function.
	void Translate(glm::vec3 trans) {
		Translate(trans.x, trans.y, trans.z);
	}

	void Rotate(float x, float y, float z) {
		glm::quat qX = glm::angleAxis(x, 1.0f,0.0f,0.0f);
		glm::quat qY = glm::angleAxis(y, 0.0f,1.0f,0.0f);
		glm::quat qZ = glm::angleAxis(z, 0.0f,0.0f,1.0f);
		glm::quat change = qX * qY * qZ;
		this->orientation = glm::normalize(change * this->orientation);
		this->rotateMatrix = glm::mat4_cast(this->orientation);
		this->MMhasChanged = true;
	}

	void Rotate(glm::vec3 rot) {
		Rotate(rot.x, rot.y, rot.z);
	}

	void Scale(float x, float y, float z) {
		this->scaleMatrix = glm::scale(this->scaleMatrix, x, y, z);
		this->MMhasChanged = true;
	}

	void Scale(glm::vec3 scale) {
		Scale(scale.x, scale.y, scale.z);
	}

	glm::mat4 ModelMatrix() {
		if (this->MMhasChanged) {
			this->modelMatrix = glm::mat4(1.0f);
			this->modelMatrix =  this->translateMatrix * this->rotateMatrix * this->scaleMatrix;
			this->MMhasChanged = false;
		}
		return this->modelMatrix;
	}

private:
	glm::mat4 modelMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 rotateMatrix;
	glm::quat orientation;
	glm::mat4 translateMatrix;
	bool MMhasChanged; // Set to true if the modelMatrix has changed and needs to be updated
};