#pragma once

#include "IGLView.h"

struct GLFPSView : public IGLView {
	GLFPSView();

	const glm::mat4 GetViewMatrix();
	virtual void Move(float right, float up, float forward);
	virtual glm::vec3 Restrict(glm::vec3 rotation);
};