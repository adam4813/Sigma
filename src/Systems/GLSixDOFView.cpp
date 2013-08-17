#include "GLSixDOFView.h"

const glm::vec3 IGLView::FORWARD_VECTOR(0.0f,0.0f,-1.0f);
const glm::vec3 IGLView::UP_VECTOR(0.0f,1.0f,.0f);
const glm::vec3 IGLView::RIGHT_VECTOR(glm::cross(FORWARD_VECTOR, UP_VECTOR));

GLSixDOFView::GLSixDOFView() { }

void GLSixDOFView::UpdateViewMatrix() {
	glm::normalize(this->orientation);
	this->ViewMatrix = glm::lookAt(this->position, this->position + FORWARD_VECTOR * this->orientation, UP_VECTOR * this->orientation);
}

void GLSixDOFView::Move(float right, float up, float forward) {
	this->position += (FORWARD_VECTOR * forward) * this->orientation;
	this->position += (UP_VECTOR * up) * this->orientation;
	this->position += (RIGHT_VECTOR * right) * this->orientation;
}

void GLSixDOFView::Rotate(float x, float y, float z) {
	glm::quat qX = glm::angleAxis(x, 1.0f,0.0f,0.0f);
	glm::quat qY = glm::angleAxis(y, 0.0f,1.0f,0.0f);
	glm::quat qZ = glm::angleAxis(z, 0.0f,0.0f,1.0f); // TODO: Fix roll rotation.
	glm::quat change = qX * qY * qZ;
	this->orientation = change * this->orientation;
}
