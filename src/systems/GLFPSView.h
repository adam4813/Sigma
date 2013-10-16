#pragma once

#include "IGLView.h"

struct GLFPSView : public IGLView {
	GLFPSView();

	void UpdateViewMatrix();

	void Move(float right, float up, float forward);

	void Rotate(float x, float y, float z);

	float rotX, rotY;
};
