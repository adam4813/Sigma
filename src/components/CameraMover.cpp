#include "components/CameraMover.h"

#include "systems/IGLView.h"

CameraMover::CameraMover(const int entityID) : Sigma::IMoverComponent(entityID) {

}

void CameraMover::ApplyForces(const double delta) {
	glm::vec3 deltavec(delta);
	glm::vec3 totalForce;
	glm::vec3 targetrvel;

	for (auto forceitr = this->forces.begin(); forceitr != this->forces.end(); ++forceitr) {
		totalForce += *forceitr;
	}

	this->view->Move(totalForce * deltavec);

	for (auto rotitr = this->rotationForces.begin(); rotitr != this->rotationForces.end(); ++rotitr) {
		glm::vec3 rotation_amount = (*rotitr) * deltavec;

		// Restricts the total rotation allowed to a specified range
		rotation_amount = this->view->Restrict(rotation_amount);

		this->view->Transform.Rotate(rotation_amount);
	}

	// Inertial rotation
	targetrvel = _rotationtarget * deltavec;
	if(fabs(targetrvel.x) > 0.001f || fabs(targetrvel.y) > 0.001f || fabs(targetrvel.z) > 0.001f) {
		targetrvel = this->view->Restrict(targetrvel);
		this->view->Transform.Rotate(targetrvel);
		_rotationtarget -= targetrvel;
	}
	targetrvel = this->view->Restrict(targetrvel);
	this->view->Transform.Rotate(targetrvel);
}

// immediate mode rotation (for mouse motion)
void CameraMover::RotateNow(float x, float y, float z) {
	this->view->Transform.Rotate(x,y,z);
}
void CameraMover::RotateTarget(float x, float y, float z) {
	this->_rotationtarget += glm::vec3(x,y,z);
}

void CameraMover::View(Sigma::IGLView* view) {
	this->view = view;
}

Sigma::IGLView* CameraMover::View() {
	return this->view;
}
