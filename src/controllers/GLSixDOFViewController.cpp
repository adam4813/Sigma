#include "controllers/GLSixDOFViewController.h"

namespace Sigma{
namespace event{
namespace handler{

    const float GLSixDOFViewController::SPEED_TRANSLATE   = 2.0f;
    const float GLSixDOFViewController::SPEED_ROTATE      = 20.0f * 3.14159f;
    const float GLSixDOFViewController::BOOST_MULTIPLIER  = 2.0f;

    GLSixDOFViewController::GLSixDOFViewController(IGLView* view, BulletMover* mover) : mover(mover) {
        // Set the view mover's view pointer.
		// Dereferencing possibly null pointer!
        ControllableMove::SetTransform(this->mover->GetEntityID(), view->Transform());

        // Clear out the internal key state buffers.
        memset(this->keys, 0, sizeof(this->keys));
        memset(this->keyState, 0, sizeof(this->keyState));
        // set to 1 all keys that should trigger an event in this class
		this->keys['W'] = 1; this->keys['B'] = 1; this->keys['S'] = 1;
        this->keys['A'] = 1; this->keys['D'] = 1; this->keys['F'] = 1;
        this->keys['G'] = 1; this->keys['E'] = 1; this->keys['C'] = 1;
		this->keys['Q'] = 1; this->keys['Z'] = 1; this->keys['R'] = 1;
        this->keys['T'] = 1;
    }

    void GLSixDOFViewController::KeyStateChange(const unsigned int key, const KEY_STATE state) {

        // Store the new key state
        this->keyState[key] = state;

        float speed = this->keyState['B'] == KS_DOWN ? SPEED_TRANSLATE * BOOST_MULTIPLIER : SPEED_TRANSLATE;

        // Translation keys
        float fwd = 0.0f, strafe = 0.0f, rise = 0.0f;
        if (this->keyState['W'] == KS_DOWN) // Move forward
            fwd += speed;
        if (this->keyState['S'] == KS_DOWN) // Move backward
            fwd -= speed;
        if (this->keyState['F'] == KS_DOWN) // Strafe left
            strafe -= speed;
        if (this->keyState['G'] == KS_DOWN) // Strafe right
            strafe += speed;
        if (this->keyState['E'] == KS_DOWN) // Move up
           rise += speed;
        if (this->keyState['C'] == KS_DOWN) // Move down
            rise -= speed;
        // remove previous force and add new one
        CanMove::RemoveForce(this->mover->GetEntityID(), this->_translate);
        this->_translate = glm::vec3(strafe, rise, fwd);
        CanMove::AddForce(this->mover->GetEntityID(), this->_translate);

        // Rotation Keys
        float pitch = 0.0f, yaw = 0.0f, roll = 0.0f;
        if (this->keyState['A'] == KS_DOWN) // pan (yaw) left
            yaw -= SPEED_ROTATE;
        if (this->keyState['D'] == KS_DOWN) // pan (yaw) right
            yaw += SPEED_ROTATE;
        if (this->keyState['Q'] == KS_DOWN) // pitch Up
            pitch -= SPEED_ROTATE;
        if (this->keyState['Z'] == KS_DOWN) // pitch Down
            pitch += SPEED_ROTATE;
        if (this->keyState['R'] == KS_DOWN) // roll left
            roll -= SPEED_ROTATE;
        if (this->keyState['T'] == KS_DOWN) // roll right
            roll += SPEED_ROTATE;

        // remove previous force and add new one
        CanMove::RemoveRotationForce(this->mover->GetEntityID(), this->_rotate);
        this->_rotate = glm::vec3(pitch, yaw, roll);
        CanMove::AddRotationForce(this->mover->GetEntityID(), this->_rotate);

    } // function KeyStateChange
} } } // namespace Sigma::event::handler
