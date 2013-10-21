#include "GLFPSView.h"

GLFPSView::GLFPSView() {}

const glm::mat4 GLFPSView::GetViewMatrix() {
	//return glm::lookAt(this->Transform.GetPosition(),
	//	  			   this->Transform.GetPosition() + -this->Transform.GetForward(),
					   //GLTransform::UP_VECTOR);

	// Limit rotation to pitch and yaw, apply pitch first to ensure
	// yaw rotation happens correctly
	glm::mat4 viewMatrix = glm::rotate(glm::mat4(1.0f), this->Transform.GetPitch(), glm::vec3(1.0f, 0, 0));
	viewMatrix = glm::rotate(viewMatrix, this->Transform.GetYaw(), glm::vec3(0, 1.0f, 0));

	viewMatrix = glm::translate(viewMatrix, -1.0f * this->Transform.GetPosition());

	return viewMatrix;
}

// Currently not working
glm::vec3 GLFPSView::Restrict(glm::vec3 rotation) {
	glm::vec3 rot = rotation;

	float current_pitch = this->Transform.GetPitch();
	float new_pitch = current_pitch + rotation.x;

	if(new_pitch > 45.0f) {
		rot.x = 45.0f - current_pitch;
	} else if(new_pitch < -45.0f) {
		rot.x = -45.0f - current_pitch;
	}

	return rot;
}

void GLFPSView::Move(float right, float up, float forward) {

	// Figure out the movement directions along the xz plane, to allow for proper movement
	// wh
	glm::vec3 right_direction = glm::normalize(glm::cross(this->Transform.GetForward(), GLTransform::UP_VECTOR));
	glm::vec3 forward_direction = glm::normalize(glm::cross(GLTransform::UP_VECTOR, right_direction));

	this->Transform.Translate(
						(forward * -forward_direction) +
						(up * GLTransform::UP_VECTOR) +
						(right * -right_direction)
	);
}
