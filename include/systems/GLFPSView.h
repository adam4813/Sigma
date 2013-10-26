#pragma once

#include "IGLView.h"

struct GLFPSView : public IGLView {

    SET_COMPONENT_ID("FPS_VIEW");

	GLFPSView(int entityID);

	const glm::mat4 GetViewMatrix();
	virtual void Move(float right, float up, float forward);
	virtual glm::vec3 Restrict(glm::vec3 rotation);
};
