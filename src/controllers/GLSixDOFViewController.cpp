#include "GLSixDOFViewController.h"

Sigma::event::handler::GLSixDOFViewController::GLSixDOFViewController(IGLView* view, ViewMover* mover) : mover(mover) {
	// Set the view mover's view pointer.
	this->mover->View(view);
					
	// Clear out the internal key state buffers.
	memset(this->keys, 0, sizeof(this->keys));
	memset(this->keyState, 0, sizeof(this->keyState));
	this->keys['W'] = 1; this->keys['B'] = 1; this->keys['S'] = 1;
	this->keys['A'] = 1; this->keys['D'] = 1; this->keys['F'] = 1;
	this->keys['G'] = 1; this->keys['E'] = 1; this->keys['C'] = 1;
	this->keys['Q'] = 1; this->keys['Z'] = 1; this->keys['R'] = 1;
	this->keys['T'] = 1;
}
				
void Sigma::event::handler::GLSixDOFViewController::KeyStateChange(const unsigned int key, const KEY_STATE state) {
	// Store the new key state
	this->keyState[key] = state;

	const float movement_speed = 15.0f;

	// Translation keys

	if (key == 'W') { // Move forward
		if (state == KS_UP) {
			if (this->keyState['B'] == KS_DOWN) {
				this->mover->RemoveForce(glm::vec3(0.0f, 0.0f, movement_speed*10.0f));
			}
			else {
				this->mover->RemoveForce(glm::vec3(0.0f, 0.0f, movement_speed));
			}
		} else {
			if (this->keyState['B'] == KS_DOWN) {
				this->mover->AddForce(glm::vec3(0.0f, 0.0f, movement_speed*10.0f));
				this->mover->RemoveForce(glm::vec3(0.0f, 0.0f, -movement_speed*10.0f));
			}
			else {
				this->mover->AddForce(glm::vec3(0.0f, 0.0f, movement_speed));
				this->mover->RemoveForce(glm::vec3(0.0f, 0.0f, -movement_speed));
			}
		}
	} else if (key == 'S') { // Move backward
		if (state == KS_UP) {
			if (this->keyState['B'] == KS_DOWN) {
				this->mover->RemoveForce(glm::vec3(0.0f, 0.0f, -movement_speed*10.0f));
			}
			else {
				this->mover->RemoveForce(glm::vec3(0.0f, 0.0f, -movement_speed));
			}
		}
		else {
			if (this->keyState['B'] == KS_DOWN) {
				this->mover->AddForce(glm::vec3(0.0f, 0.0f, -movement_speed*10.0f));
				this->mover->RemoveForce(glm::vec3(0.0f, 0.0f, movement_speed*10.0f));
			}
			else {
				this->mover->AddForce(glm::vec3(0.0f, 0.0f, -movement_speed));
				this->mover->RemoveForce(glm::vec3(0.0f, 0.0f, movement_speed));
			}
		}
	}
	if (key == 'B') { // Boost on
		if (state == KS_DOWN) {
			if (this->keyState['W'] == KS_DOWN) {
				this->mover->AddForce(glm::vec3(0.0f, 0.0f, movement_speed*10.0f));
				this->mover->RemoveForce(glm::vec3(0.0f, 0.0f, movement_speed));
			} else if (this->keyState['S'] == KS_DOWN) {
				this->mover->AddForce(glm::vec3(0.0f, 0.0f, -movement_speed*10.0f));
				this->mover->RemoveForce(glm::vec3(0.0f, 0.0f, -movement_speed));
			}
		}
		else { // Boost off
			if (this->keyState['W'] == KS_DOWN) {
				this->mover->AddForce(glm::vec3(0.0f, 0.0f, movement_speed));
				this->mover->RemoveForce(glm::vec3(0.0f, 0.0f, movement_speed*10.0f));
			} else if (this->keyState['S'] == KS_DOWN) {
				this->mover->AddForce(glm::vec3(0.0f, 0.0f, -movement_speed));
				this->mover->RemoveForce(glm::vec3(0.0f, 0.0f, -movement_speed*10.0f));
			}
		}
	}

	if (key == 'F') { // Strafe left
		if (state == KS_UP) {
			this->mover->RemoveForce(glm::vec3(-movement_speed,0.0f,0.0f));
		}
		else {
			this->mover->AddForce(glm::vec3(-movement_speed,0.0f,0.0f));
			this->mover->RemoveForce(glm::vec3(movement_speed,0.0f,0.0f));
		}
	} else if (key == 'G') { // Strafe right
		if (state == KS_UP) {
			this->mover->RemoveForce(glm::vec3(movement_speed,0.0f,0.0f));
		}
		else {
			this->mover->AddForce(glm::vec3(movement_speed,0.0f,0.0f));
			this->mover->RemoveForce(glm::vec3(-movement_speed,0.0f,0.0f));
		}
	}

	if (key == 'E') { // Move up
		if (state == KS_UP) {
			this->mover->RemoveForce(glm::vec3(0.0f,movement_speed,0.0f));
		}
		else {
			this->mover->AddForce(glm::vec3(0.0f,movement_speed,0.0f));
			this->mover->RemoveForce(glm::vec3(0.0f,-movement_speed,0.0f));
		}
	} else if (key == 'C') { // Move down
		if (state == KS_UP) {
			this->mover->RemoveForce(glm::vec3(0.0f,-movement_speed,0.0f));
		}
		else {
			this->mover->AddForce(glm::vec3(0.0f,-movement_speed,0.0f));
			this->mover->RemoveForce(glm::vec3(0.0f,movement_speed,0.0f));
		}
	}

	// Rotation Keys

	// Set the value to positive or negative for forward/backward and 0 for no change.
	// To remove a force on an axis set its rem_ to the correct direction.
	// To add a force on an axis set its add_ to the correct direction,
	//	and set the corresponding rem_ to the opposite direction to remove it.
	int add_x = 0, add_y = 0, add_z = 0;
	int rem_x = 0, rem_y = 0, rem_z = 0;

	if (key == 'A') {
		if (state == KS_UP) {
			rem_y = -1;
		}
		else {
			add_y = -1;
			rem_y = 1;
		}
	} else if (key == 'D') {
		if (state == KS_UP) {
			rem_y = 1;
		}
		else {
			add_y = 1;
			rem_y = -1;
		}
	}

	if (key == 'Q') { // Pitch Up
		if (state == KS_UP) {
			rem_x  = -1;
		}
		else {
			add_x = -1;
			rem_x = 1;
		}
	} else if (key == 'Z') { // Pitch Down
		if (state == KS_UP) {
			rem_x = 1;
		}
		else {
			add_x = 1;
			rem_x = -1;
		}
	}

	if (key == 'R') { // Roll left
		if (state == KS_UP) {
			rem_z = -1;
		}
		else {
			add_z = -1;
			rem_z = 1;
		}
	} else if (key == 'T') { // Roll right
		if (state == KS_UP) {
			rem_z = 1;
		}
		else {
			add_z = 1;
			rem_z = -1;
		}
	}

	// Add and remove rotation forces based on the variables set above.
	this->mover->AddRotationForce(glm::vec3(add_x*45.0f, add_y*45.0f, add_z*45.0f));
	this->mover->RemoveRotationForce(glm::vec3(rem_x*45.0f, rem_y*45.0f, rem_z*45.0f));
}