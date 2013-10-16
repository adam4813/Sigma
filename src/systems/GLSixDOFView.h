#pragma once

#include "IGLView.h"

struct GLSixDOFView : public IGLView {
	GLSixDOFView();

	const glm::mat4 GetViewMatrix();
	void Move(float right, float up, float forward);
};
