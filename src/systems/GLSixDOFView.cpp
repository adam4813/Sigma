#include "GLSixDOFView.h"

GLSixDOFView::GLSixDOFView() {}

const glm::mat4 GLSixDOFView::GetViewMatrix() {
	return glm::lookAt(this->Transform.GetPosition(),
		  			   this->Transform.GetPosition() + GLTransform::FORWARD_VECTOR * this->Transform.GetOrientation(),
					   GLTransform::UP_VECTOR * this->Transform.GetOrientation());
}

void GLSixDOFView::Move(float right, float up, float forward) {
	this->Transform.Translate(((GLTransform::FORWARD_VECTOR * forward) +
						(GLTransform::UP_VECTOR * up) +
						(GLTransform::RIGHT_VECTOR * right)) * this->Transform.GetOrientation());
}
