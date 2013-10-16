#pragma once

//#include "glm/glm.hpp"
//#include "glm/ext.hpp"

#include "../GLTransform.h"

struct IGLView {
	GLTransform Transform;

	virtual const glm::mat4 GetViewMatrix() = 0;

	/*glm::mat4 ViewMatrix;
	glm::vec3 position;
	glm::quat orientation;

	IGLView() {
		this->position = glm::vec3(0.0f,0.0f,0.0f);
		this->orientation = glm::quat(0.0f,0.0f,1.0f,0.0f);
	}
	virtual void UpdateViewMatrix() = 0;*/

	virtual void Move(float right, float up, float forward) = 0;

	// Helper function.
	void Move(glm::vec3 trans) {
		Move(trans.x, trans.y, trans.z);
	}
	/*
	virtual void Rotate(float x, float y, float z) = 0;

	// Helper function.
	void Rotate(glm::vec3 rot) {
		Rotate(rot.x, rot.y, rot.z);
	}*/
};