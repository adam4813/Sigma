#include "GLFPSController.h"

namespace Sigma{
namespace event{
namespace handler{

    const float GLFPSController::SPEED_TRANSLATE   = 2.0f;
    const float GLFPSController::SPEED_ROTATE      = 20.0f * 3.14159f;
    const float GLFPSController::BOOST_MULTIPLIER  = 2.0f;

    GLFPSController::GLFPSController(IGLView* view, ViewMover* mover) : mover(mover) {
        // Set the view mover's view pointer.
        this->mover->View(view);

        // Clear out the internal key state buffers.
        memset(this->keys, 0, sizeof(this->keys));
        memset(this->keyState, 0, sizeof(this->keyState));
        // set to 1 all keys that should trigger an event in this class
		this->keys['W'] = 1; this->keys['B'] = 1; this->keys['S'] = 1;
        this->keys['A'] = 1; this->keys['D'] = 1;
    }

    void GLFPSController::KeyStateChange(const unsigned int key, const KEY_STATE state) {

        // Store the new key state
        this->keyState[key] = state;

        float speed = this->keyState['B'] == KS_DOWN ? SPEED_TRANSLATE * BOOST_MULTIPLIER : SPEED_TRANSLATE;

        // Translation keys
        float fwd = 0.0f, strafe = 0.0f, rise = 0.0f;
        if (this->keyState['W'] == KS_DOWN) // Move forward
            fwd += speed;
        if (this->keyState['S'] == KS_DOWN) // Move backward
            fwd -= speed;
        if (this->keyState['A'] == KS_DOWN) // Strafe left
            strafe -= speed;
        if (this->keyState['D'] == KS_DOWN) // Strafe right
            strafe += speed;

        // remove previous force and add new one
        this->mover->RemoveForce(this->_translate);
        this->_translate = glm::vec3(strafe, rise, fwd);
        this->mover->AddForce(this->_translate);

    } // function KeyStateChange

	void GLFPSController::MouseMove(float dx, float dy) {
		float pitch=0.0f, yaw=0.0f;

		// Motion from center to far left/right of screen should
		// result in 90 degree turn
		yaw = dx*90.0f;
		pitch = dy*45.0f;

		// remove previous force and add new one
        this->mover->RemoveRotationForce(this->_rotate);
        this->_rotate = glm::vec3(pitch, yaw, 0.0f);
        this->mover->AddRotationForce(this->_rotate);
	}
}
}
} // namespace Sigma::event::handler
