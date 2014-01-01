#include "systems/GLSixDOFView.h"

GLSixDOFView::GLSixDOFView(const id_t entity_id) : IGLView(entity_id) {}

const glm::mat4 GLSixDOFView::GetViewMatrix() {
	return glm::lookAt(this->transform->GetPosition(),
		  			   this->transform->GetPosition() + GLTransform::FORWARD_VECTOR * this->transform->GetOrientation(),
					   GLTransform::UP_VECTOR * this->transform->GetOrientation());
}

void GLSixDOFView::Move(float right, float up, float forward) {
	this->transform->Translate(((GLTransform::FORWARD_VECTOR * forward) +
						(GLTransform::UP_VECTOR * up) +
						(GLTransform::RIGHT_VECTOR * right)) * this->transform->GetOrientation());
}
