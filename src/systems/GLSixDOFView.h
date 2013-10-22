#pragma once

#include "IGLView.h"

struct GLSixDOFView : public IGLView {
	GLSixDOFView();

	const glm::mat4 GetViewMatrix();
	virtual void Move(float right, float up, float forward);
	virtual glm::vec3 Restrict(glm::vec3 rotation) { return rotation; }
};
