#include "GLFPSView.h"

GLFPSView::GLFPSView() { }

void GLFPSView::UpdateViewMatrix() {
	glm::normalize(this->orientation);
	this->ViewMatrix = glm::lookAt(this->position, this->position + FORWARD_VECTOR * this->orientation, UP_VECTOR);
	this->position.y = 1.0f; // TODO: Move this into a SetHeight method as FPS cameras are controlled via the orientation for side/side and back/forward but not height.
}

void GLFPSView::Move(float right, float up, float forward) {
	this->position += (FORWARD_VECTOR * forward) * this->orientation;
	this->position += (RIGHT_VECTOR * right) * this->orientation;
}

void GLFPSView::Rotate(float x, float y, float z) {
	glm::quat qX = glm::angleAxis(x, 1.0f,0.0f,0.0f);
	glm::quat qY = glm::angleAxis(y, 0.0f,1.0f,0.0f);
	glm::quat qZ = glm::angleAxis(z, 0.0f,0.0f,1.0f);
	glm::quat change = qX * qY * qZ;
	this->orientation = change * this->orientation;
}
