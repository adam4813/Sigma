#pragma once

#include "IGLView.h"

struct GLFPSView : public IGLView {
	GLFPSView();

	const glm::mat4 GetViewMatrix();
	void Move(float right, float up, float forward);
};